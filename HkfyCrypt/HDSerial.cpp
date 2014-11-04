#include "stdafx.h"
#include "HDSerial.h"

void ChangeByteOrder(PCHAR szString, USHORT uscStrSize) 
{ 
	USHORT  i	= 0; 
	CHAR	temp= '\0'; 

	for (i = 0; i < uscStrSize; i+=2) 
	{ 
		temp			= szString[i]; 
		szString[i]		= szString[i+1]; 
		szString[i+1]	= temp; 
	} 
}

//--------------------------------------------------------------
//						硬盘序列号
//--------------------------------------------------------------
BOOL GetHDSerial(char *lpszHD, int len/*=128*/)
{ 
	BOOL		bRtn     = FALSE;
	DWORD		bytesRtn = 0;
	char		szhd[80] = {0}; 
	PIDSECTOR	phdinfo; 
	HANDLE		hDrive	 = NULL; 
	GETVERSIONOUTPARAMS vers; 
	SENDCMDINPARAMS		in; 
	SENDCMDOUTPARAMS	out; 

	ZeroMemory(&vers, sizeof(vers)); 
	ZeroMemory(&in  , sizeof(in)); 
	ZeroMemory(&out , sizeof(out)); 

	//搜索四个物理硬盘，取第一个有数据的物理硬盘
	for (int j=0; j<4; j++)
	{ 
		sprintf(szhd,	"\\\\.\\PhysicalDrive%d",	j); 
		hDrive = CreateFileA(szhd, 
						GENERIC_READ|GENERIC_WRITE, 
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						OPEN_EXISTING,
						0,
						0); 
		if (NULL == hDrive)
		{ 
			continue; 
		} 

		if (!DeviceIoControl(hDrive, DFP_GET_VERSION, 0, 0, &vers, sizeof(vers), &bytesRtn,0))
		{ 
			goto FOREND;
		} 

		//If IDE identify command not supported, fails 
		if (!(vers.fCapabilities&1))
		{ 
			goto FOREND;
		} 

		//Identify the IDE drives 
		if (j&1)
		{ 
			in.irDriveRegs.bDriveHeadReg = 0xb0; 
		}
		else
		{ 
			in.irDriveRegs.bDriveHeadReg = 0xa0; 
		} 

		if (vers.fCapabilities&(16>>j))
		{ 
			//We don't detect a ATAPI device. 
			goto FOREND;
		}
		else
		{ 
			in.irDriveRegs.bCommandReg = 0xec; 
		} 
		in.bDriveNumber = j; 
		in.irDriveRegs.bSectorCountReg  = 1; 
		in.irDriveRegs.bSectorNumberReg = 1; 
		in.cBufferSize = 512; 

		if (!DeviceIoControl(hDrive, DFP_RECEIVE_DRIVE_DATA, &in, sizeof(in), &out, sizeof(out), &bytesRtn,0))
		{ 
			//"DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA"<<endl; 
			goto FOREND; 			
		} 
		phdinfo=(PIDSECTOR)out.bBuffer; 

		char	s[21] = {0};		
		memcpy(s, phdinfo->sSerialNumber, 20); 		
		s[20] = 0; 
		ChangeByteOrder(s, 20); 
		
		//删除空格字符
		int ix = 0;
		for (ix=0; ix<20; ix++)
		{
			if (s[ix] == ' ')
			{
				continue;
			}
			break;
		}
		memcpy(lpszHD, s+ix, 20);
		
		bRtn = TRUE;
		break;
FOREND:
		CloseHandle(hDrive); 
		hDrive = NULL; 
	}

	CloseHandle(hDrive); 
	hDrive = NULL; 
	return(bRtn);
}