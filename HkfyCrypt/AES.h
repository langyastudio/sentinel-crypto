#ifndef _AFXSTD_620892B6_BE7B_449F_BF8F_F7448162389B_INCLUDE_H_
#define _AFXSTD_620892B6_BE7B_449F_BF8F_F7448162389B_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif//!_MSC_VER >= 1200

  
//--------------------------------------------------------------------------
//								AES Encrypt
//--------------------------------------------------------------------------
class  CDSA  
{  
public:  
    //! #定义ENUM保存AES密钥的长度  
    /*! 
        以下这种ENUM保存三个密钥长度的选择 
        @see enum ENUM_KeySize_ 
        @attention 不同长度的密钥对应的加密的字符串长度不同 
    */  
    typedef enum ENUM_KeySize_  
	{  
		BIT128 = 0,  
        BIT192,  
        BIT256  
    }ENUM_KEYSIZE;  
public:  
    //! #AES的构造函数  
    /*! 
      @see AES( ENUM_KEYSIZE keysize, BYTE *key ) 
      @param[ENUM_KEYSIZE] 对密钥长度进行初始化设置 
      @param[BYTE]		   输入相应的密钥 
      其中应该对AES类中的一些变量进行相应的初始化 
    */  
    CDSA( ENUM_KEYSIZE keysize, BYTE *key );  
    //! #AES的析构函数  
    /*! 
     @see ~AES(void) 
      释放内存 
    */  
    ~CDSA(void);  
    //! #AES算法中的加密函数模块  
    /*! 
      @see void Cipher( BYTE *input, BYTE *output ); 
      @param[in]  input为输入的明文 
      @param[out] output为加密之后的密文 
      @return[void] 不返回任何值 
    */  
    void Cipher( BYTE *input, BYTE *output );  
    //! #AES算法中的解密函数模块  
    /*! 
      @see void InvCipher( BYTE *input, BYTE *output ); 
      @param[in]  input为输入的密文 
      @param[in]  output为解密之后的明文 
      @return[void] 不返回任何值 
    */  
    void InvCipher( BYTE *input, BYTE *output );  

private:  
    BYTE *RotWord( BYTE *word );  
    BYTE *SubWord( BYTE *word );  
    void AddRoundKey(int round);  
    void SubBytes();  
    void InvSubBytes();  
    void ShiftRows();  
    void InvShiftRows();  
    void MixColumns();  
    void InvMixColumns();  
    static BYTE gfmultby01(BYTE b)  //乘1  
    {  
        return b;  
    }  
  
    static BYTE gfmultby02(BYTE b) //乘2  
    {  
        if (b < 0x80)  
            return (BYTE)(int)(b <<1);  
        else  
            return (BYTE)( (int)(b << 1) ^ (int)(0x1b) );  
    }  
  
    static BYTE gfmultby03(BYTE b)  
    {  
        return (BYTE) ( (int)gfmultby02(b) ^ (int)b );//GF域的加法运算就是异或  
    }  
  
    static BYTE gfmultby09(BYTE b)  
    {  
        return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^  
            (int)b );  
    }  
  
    static BYTE gfmultby0b(BYTE b)  
    {  
        return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^  
            (int)gfmultby02(b) ^  
            (int)b );  
    }  
  
    static BYTE gfmultby0d(BYTE b)  
    {  
        return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^  
            (int)gfmultby02(gfmultby02(b)) ^  
            (int)(b) );  
    }  
  
    static BYTE gfmultby0e(BYTE b)  
    {  
        return (BYTE)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^  
            (int)gfmultby02(gfmultby02(b)) ^  
            (int)gfmultby02(b) );  
    }  

    //! #代表以字为单位的块长  
    /*! 
    @Brief 代表以字为单位的块长 
    */  
    int Nb; //  
    //! #代表以字为单位的密钥长度  
    /*! 
    @Brief 代表以字为单位的密钥长度 
    */  
    int Nk;//  
    //! #轮数 ,轮数是10、12或14中的任意一个并且是基于密码分析学理论的  
    /*! 
    @Brief 轮数 ,轮数是10、12或14中的任意一个并且是基于密码分析学理论的 
    @attention 它直接取决于密钥长度 
    */  
    int Nr;//轮数 ,轮数是10、12或14中的任意一个并且是基于密码分析学理论的。  
    //  
    //! #the seed key  
    /*! 
    @Brief   size will be 4 * keySize from ctor 
    */  
    BYTE *key;//  
    typedef struct BYTE4_  
    {  
        BYTE w[4];  
    }BYTE4;  
    BYTE4 *w;  
    LPBYTE State[4];    
};  


#endif //!_AFXSTD_620892B6_BE7B_449F_BF8F_F7448162389B_INCLUDE_H_
