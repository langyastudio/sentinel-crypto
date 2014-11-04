#ifndef _AFXSTD_3D6ED50C_A0F8_47D5_B238_A63C9BDEC809_INCLUDE_H_
#define _AFXSTD_3D6ED50C_A0F8_47D5_B238_A63C9BDEC809_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif//!_MSC_VER >= 1200

#include <bitset>
#include <vector>

using namespace std;


class deskey
{
private:
	static int cp1[];		//置换矩阵1
	static int cp2[];		//置换矩阵2
	static int totrot[];	//位移矩阵
	vector<string> keys;

public:
	deskey(string const key);
	vector<string> subkeyinit(int flag);
};

class tripledes
{
private:
	static int ip1[];
	static int ip2[];
	static int e[];
	static int p_box[];
	static int s_box[];

public:
	static void loopf(string &input,vector<string> key,bool flag);						//实现des加密,flag=1表示加密，flag=0表示解密
	static void f(string &input,const string key);										//用于单轮加密
	static string encrypt(string &input,string key,bool flag);							//实现3des加密,flag=1表示加密，flag=0表示解密
};

class tool
{
public:
	static string s_to_b(string str);					//字符串转化成其二进制储存编码
	static string b_to_s(string b);						//将二进制按unicode码转化成字符串
	static string d_to_b(unsigned long long a);			//十进制转化成二进制
	static string d_to_b(string a);						//用于转换较长的二进制数
	static unsigned long long b_to_d(string b);			//二进制转化为十进制
	static string b_to_ds(string b);	                //用于转换超过64位的二进制数
	static string format_b_g(string &b,size_t n);		//通过高位添零或截取低位的方式使二进制串变成n位
	static string format_b_d(string &b,size_t n);		//通过低位添零或截取高位的方式使二进制串变成n位
	static string c_to_b(char c);						//将字符转化成二进制流
	static string h_to_b(string b);						//将字符形式的十六进制数字转化成二进制
	static string b_to_h(string b);						//将二进制转化成字符形式的十六进制数字
	static string off_z(string &str);					//去掉高位的零
};

//--------------------------------------------------------------------------
//								DES Encrypt
//--------------------------------------------------------------------------
class CAES
{
private:
	vector<string> b64;								//用于储存由文本分割转化得到的六十四位二进制串
	string key;

public:
	CAES(string input,string key);					//分割input，生成b64
	string encrypt();								//加密
	string decrypt();								//解密
};

#endif  

