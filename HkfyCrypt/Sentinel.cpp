#include "stdafx.h"
#include "Sentinel.h"
#include "vendor_code.h"
#include "TinyXML/tinyxml.h"
#include "MD5.h"
#include "DES.h"
#include "AES.h"
#include "HDSerial.h"
#include "MacID.h"


CSentinel::CSentinel()
{
	m_pChasp = HASP_INVALID_HANDLE_VALUE;
	m_pError = new CErrorPrinter();
	m_pCode  = NULL;
	m_pChannels = 0;
}

CSentinel::~CSentinel()
{
	LogOut();	

	if (NULL != m_pError)
	{
		delete m_pError;
		m_pError = NULL;
	}

	if (NULL != m_pCode)
	{
		delete m_pCode;
		m_pCode = NULL;
	}
}

const char * CSentinel::GetErrorName( int errorCode )
{
	return(m_pError->GetError((hasp_status_t)errorCode));
}

int CSentinel::LogIn( unsigned long lFeatureId )
{
	if (NULL == m_pCode)
	{
		DecVendor();
	}

	LogOut();	

	//1.0 功能ID
	//lFeatureId

	//2.0 限制登录为HL锁+本地登录
	const char* scope = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<haspscope>"
		"    <hasp type=\"HASP-HL\" >"
		"        <license_manager hostname=\"localhost\" />"
		"    </hasp>"
		"</haspscope>";	
		
	//3.0 登录
	hasp_status_t status = hasp_login_scope(lFeatureId, scope, m_pCode, &m_pChasp);
		
	return(status);
}

int CSentinel::LogOut()
{
	hasp_status_t status = HASP_STATUS_OK;
	if (HASP_INVALID_HANDLE_VALUE != m_pChasp)
	{
		status = hasp_logout(m_pChasp);
		m_pChasp = HASP_INVALID_HANDLE_VALUE;
	}

	return(status);
}

int CSentinel::CheckRom()
{
	if (HASP_INVALID_HANDLE_VALUE == m_pChasp)
	{
		return(HASP_STATUS_OK);
	}
	
	hasp_size_t   size		= 0; //内存的尺寸
	hasp_status_t status	= HASP_STATUS_OK;
	unsigned char data[HASP_ROM_LEN] = {0};
	unsigned char szSorData[30]		 = {0};
	string		  strData;

	//1.0 获取只读内存的尺寸，判断是否为 112
	status = hasp_get_size(m_pChasp, HASP_FILEID_RO, &size);

	if (status != HASP_STATUS_OK)
	{
		goto END;
	}
	if (size != HASP_ROM_LEN)
	{
		status = HASP_MEM_RANGE;
		goto END;
	}

	//2.0 获取只读的内存数据
	status = hasp_read(m_pChasp, HASP_FILEID_RO, 0, size, data);

	if (status != HASP_STATUS_OK)
	{
		goto END;
	}

	//3.0 判断产品信息 0x0065-0x006F 11 HKFY_AUSKAR
	if(memcmp(data+0x0065, "HKFY_AUSKAR", 11) != 0)
	{
		status = HASP_INV_PRODUCT_INFO;
		goto END;
	}

	//4.0 判断版本信息 0x0062-0x0064 3 1.0
	if(memcmp(data+0x0062, "1.0", 3) != 0)
	{
		status = HASP_INV_VERSION_INFO;
		goto END;
	}

	//5.0 开发号信息 0x0058-0x0060 9 QMBECHKFY 
	if(memcmp(data+0x0058, "QMBECHKFY", 9) != 0)
	{
		status = HASP_INV_DEVELOP_INFO;
		goto END;
	}
	
	//6.0 加密与解密的数据对比
	//0x0002-0x001F 30 原始
	//0x0023-0x0040 30 加密	
	memcpy(szSorData, data+0x0002, 30);
	
	//使用MD5加密
	for (int ix=0; ix<30; ix++)
	{
		szSorData[ix] += ix/3 + ix/4;
	}
	{
		CRSA  rsa(szSorData+3, 23);
		strData = rsa.toString();			
	}	
	memcpy(szSorData, strData.c_str(), 30);

	if (memcmp(szSorData, data+0x0023, 30) != 0)
	{
		status = HASP_ERR_ENCRYPT_DATA_OF_ROM;
		goto END;
	}

	//7.0 采样的通道数 0x0020-0x0020 1 通道数
	{		
		m_pChannels = (*(data + 0x0020) - 'a');
		if (0x000A != m_pChannels) //有效的为10通道
		{
			m_pChannels = 0x0006;
		}
	}

END:
	return(status);
}

int  CSentinel::CheckRam()
{
	if (HASP_INVALID_HANDLE_VALUE == m_pChasp)
	{
		return(HASP_STATUS_OK);
	}

	hasp_size_t   size		= 0; //内存的尺寸
	hasp_status_t status	= HASP_STATUS_OK;
	unsigned char data[HASP_RAM_LEN] = {0};
	string		  strMachineCode;	   //机器码
	int           iMachineCodeLen = 0;
	char		  *pMachineData	  = NULL;
	
	//1.0 获取读写内存的尺寸，判断是否为 112
	status = hasp_get_size(m_pChasp, HASP_FILEID_RW, &size);
	if (status != HASP_STATUS_OK)
	{
		goto END;
	}

	if (size != HASP_RAM_LEN)
	{
		status = HASP_MEM_RANGE;
		goto END;
	}

	//2.0 获取读写内存的数据	
	status = hasp_read(m_pChasp, HASP_FILEID_RW, 0, size, data);
	if (status != HASP_STATUS_OK)
	{
		goto END;
	}

	//3.0 获取机器码
	status = (hasp_status_t)GetUniMachineCode(strMachineCode);
	if (status != HASP_STATUS_OK)
	{
		goto END;
	}

	//4.0 机器码加密
	if (strMachineCode.length() < 16) //HL锁加密的数据最小长度为 16
		iMachineCodeLen = 16;
	else
		iMachineCodeLen = strMachineCode.length();
	pMachineData = new char[iMachineCodeLen];
	memset(pMachineData, 0, iMachineCodeLen);
	memcpy(pMachineData, strMachineCode.c_str(), strMachineCode.length());

	//原始的【机器码】加密
	status = hasp_encrypt(m_pChasp, pMachineData, iMachineCodeLen);
	if (status != HASP_STATUS_OK)
	{
		goto END;
	}

	//5.0 HL锁是否第一次使用
	int ix=0;
	for (ix=0; ix<HASP_RAM_LEN; ix++)
	{
		if (data[ix] != '\0')
		{
			break;
		}
	}

	if (ix != HASP_RAM_LEN) 
	{
		//5.1 该HL锁已经使用过
		unsigned char	szSorNum[16] = {0};
		unsigned char	szDesDecNum[16] = {0};
		unsigned char	szDesEncNum[16] = {0};
		int			    num			    = 0; //已经使用的机器数目

		//加密后的【机器的数目】 0x0001-0x0010 16
		memcpy(szSorNum, data+1, 16); //偏移1

		//5.11 解密【机器的数目】
		status = (hasp_status_t)DecMachineNum(szSorNum, szDesDecNum, num);
		if (status != HASP_STATUS_OK )
		{
			goto END;
		}	
		if (num < 1 || num > 5) //使用完毕
		{
			status = HASP_ERR_MACHINENUM;
			goto END;
		}

		//5.12 比较加密后的【机器码】
		int ix=0;
		for ( ix=0; ix<num; ix++)
		{
			if (memcmp(pMachineData, data+17+15*ix, 15) == 0 ) //加密后的【机器的数目】17+ 15*x
			{
				break;
			}
		}
		
		//比较失败
		if (ix >= num)
		{
			if (num == 5) //使用完毕
			{
				status = HASP_MACHINENUM_OUT_OF_BOUNDS;
				goto END;
			}
			//新增一个机器码
			{
				//【机器的数目】加密
				szDesDecNum[3] += 1; //机器的数目加1 
				EncMachineNum(szDesDecNum, szDesEncNum);

				//加密后的【机器的数目】与【机器码】写入HL锁中
				memcpy(data+1, szDesEncNum, 16); //偏移1
				memcpy(data+17+15*num, pMachineData, 15); //偏移17，认定15个字节做比较

				//写入文件中
				status = hasp_write(m_pChasp, HASP_FILEID_RW, 0, 112, data);
			}
		}
	}
	else
	{
		//5.2 该HL锁第一次使用
		unsigned char	szSorNum[16] = "\7q~0^x7{CfB%F\83";
		unsigned char	szDesNum[16] = {0};

		//原始【机器的数目】数据修改
		for (int ix=0; ix<16; ix++)
		{
			szSorNum[ix] -= ix/5;
		}
		szSorNum[3] = '1'; //1字符表示使用了一个机器码

		//【机器的数目】加密
		EncMachineNum(szSorNum, szDesNum);

		//加密后的【机器的数目】与【机器码】写入HL锁中
		memcpy(data+1, szDesNum, 16); //偏移1
		memcpy(data+17, pMachineData, 15); //偏移17，认定15个字节做比较

		//写入文件中
		status = hasp_write(m_pChasp, HASP_FILEID_RW, 0, 112, data);
	}

END:
	if (NULL != pMachineData)
	{
		delete []pMachineData;
		pMachineData = NULL;
	}
	return(status);
}

// 采集的通道数
int  CSentinel::GetPointNum()
{
	return(m_pChannels);
}

//开发商代码的长度为 984 
int  CSentinel::DecVendor()
{	
	unsigned char  key[]	= "~`Az7\0*=] 'e&,#BM:w>g-??1\1@cT{P5.#} \"rL;/*"; //加密的密钥 实际为32位	
	string   strSorData;   
	unsigned char  szSorCode[984] = {0};

	memcpy(szSorCode, g_hkfy_MainCode, 984-1);
	
	//1.0 9 改为定值(原值为0x67)
	szSorCode[9]	= 0x67;
	szSorCode[983]  = 0x13; //最后一个字符

	//2.0 解密
	//考虑到字符串含有结束字符,不能强制赋值
	for (int ix=0; ix<984; ix++)
	{
		strSorData+= szSorCode[ix];
	}

	CAES	rs(strSorData,  string((char*)(key+2)));
	string strEnc = rs.decrypt();

	//3.0 加上特定的值修改原有数据
	if (NULL != m_pCode)
	{
		delete []m_pCode;
		m_pCode = NULL;
	}
	m_pCode = new unsigned char[984+1];
	memset(m_pCode, 0, 984+1);
	memcpy(m_pCode, strEnc.c_str(), 984);

	m_pCode[111] -= 1;
	m_pCode[222] -= 5;
	m_pCode[357] += 7;
	m_pCode[587] -= 2;
	m_pCode[785] -= 6;

	return(0);
}

int  CSentinel::GetHLInfo(string &strHLId,  string &strHLType)
{						
	//1.0 限制为HL锁+本地登录
	const char* scope = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<haspscope>"
		"    <hasp type=\"HASP-HL\" >"
		"        <license_manager hostname=\"localhost\" />"
		"    </hasp>"
		"</haspscope>";

	const char* format = 
		"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
		"<haspformat root=\"hasp_info\">"
		"    <hasp>"
		"        <attribute name=\"id\" />"
		"        <attribute name=\"type\" />      "
		"    </hasp>"
		"</haspformat>";

	char *info = NULL;
	hasp_status_t  status = HASP_STATUS_OK;

	status = hasp_get_info(scope, format, m_pCode, &info);
	if (status != HASP_STATUS_OK)
	{
		return(status);
	}

	//结果类似这样
	//  <?xml version="1.0" encoding="UTF-8" ?>
	//	<hasp_info>
	//		<hasp id="42478626" type="HASP-HL" />
	//	</hasp_info>

	//2.0 获取HL锁的ID号与类型
	TiXmlDocument xmlDoc;	

	//解析
	if (!xmlDoc.Parse(info, NULL, TIXML_ENCODING_UNKNOWN))
	{
		status = HASP_INV_HLID;
		goto END;
	}
	
	//获取根节点
	const TiXmlElement* pRootElem = xmlDoc.RootElement();
	if (NULL == pRootElem)
	{
		status = HASP_INV_HLID;
		goto END;
	}

	const TiXmlNode* pChild = pRootElem->FirstChild();
	if (NULL == pChild)
	{
		status = HASP_INV_HLID;
		goto END;
	}
	if ( pChild->Type() == TiXmlNode::ELEMENT &&  0 == strcmp(pChild->Value(),"hasp") )
	{
		TiXmlElement *pElem = (TiXmlElement*)(pChild);
		strHLId   = pElem->Attribute("id");
		strHLType = pElem->Attribute("type");
	}
	
END:
	hasp_free(info);
	return(status);
}

int  CSentinel::GetUniMachineCode(string &strCode)
{
	char	szMac[128] = {0};
	char	szHD[128]  = {0};
	string	strHLId;
	string  strHLType;
	string	strUniCode;	//机器码
	BOOL	flag		  = FALSE;

	//1.0 MAC地址
	flag = GetMacByCmd(szMac, 128);
	if (flag)
	{
		strUniCode.append(szMac);
	}

	//2.0 HD地址
	flag = GetHDSerial(szHD, 128);
	if (flag)
	{
		strUniCode.append(szHD);
	}

	//3.0 HL锁的ID与Type
	int hlRlt = GetHLInfo(strHLId, strHLType);
	if (hlRlt != 0)
	{
		return(hlRlt);
	}

	strUniCode.append(strHLId);
	strUniCode.append(strHLType);//此时长度为35左右

	strCode = strUniCode;
	
	return(HASP_STATUS_OK);	
}

int  CSentinel::DecMachineNum(unsigned char szSorMachine[16], unsigned char szDesMachine[16], int &num)
{
	num	= -1;

	unsigned char  key[]	= "0.&\v 5\0*>y`Qa'S\8/+$pL;|@zM8<?-a"; //加密的密钥 实际为16位
	hasp_status_t  status	= HASP_STATUS_OK;

	//对定义的密钥进行修改
	for (int ix=0; ix<32; ix++)
	{
		key[ix] += ix/5;
	}

	CDSA		   desCls(CDSA::BIT128, key); 

	//AES 加密
	desCls.InvCipher(szSorMachine, szDesMachine);//解密的数据的长度为16

	//第四个为目前使用的数目
	if (isdigit(szDesMachine[3]) == 0)  //第三位索引号为实际使用的机器的数目
	{
		status = HASP_ERR_MACHINENUM;
		goto END;
	}

	switch (szDesMachine[3])
	{
	case '0':	num = 0;  break;
	case '1':	num = 1;  break;
	case '2':	num = 2;  break;
	case '3':	num = 3;  break;
	case '4':	num = 4;  break;
	case '5':	num = 5;  break;
	case '6':	num = 6;  break;
	case '7':	num = 7;  break;
	case '8':	num = 8;  break;
	case '9':	num = 9;  break;
	default:
		status = HASP_ERR_MACHINENUM;
		break;
	}

END:
	return(status);
}

int  CSentinel::EncMachineNum(unsigned char  szSorMachine[16], unsigned char  szDesMachine[16])
{
	unsigned char  key[]	= "0.&\v 5\0*>y`Qa'S\8/+$pL;|@zM8<?-a"; //加密的密钥 16位
	
	//对定义的密钥进行修改
	for (int ix=0; ix<32; ix++)
	{
		key[ix] += ix/5;
	}

	CDSA		   desCls(CDSA::BIT128, key);	//加密的数据的长度为16

	//AES 加密
	desCls.Cipher(szSorMachine, szDesMachine);
	
	return(HASP_STATUS_OK);
}


