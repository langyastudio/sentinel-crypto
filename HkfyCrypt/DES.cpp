#include "stdafx.h"  
#include "des.h"


int deskey::cp1[56]={	56, 48, 40, 32, 24, 16, 8,		
	0, 57, 49, 41, 33, 25, 17, 
	9, 1, 58, 50, 42, 34, 26,		
	18, 10, 2, 59, 51, 43, 35, 
	62, 54, 46, 38, 30, 22, 14,     
	6, 61, 53, 45, 37, 29, 21, 
	13, 5, 60, 52, 44, 36, 28,		
	20, 12, 4, 27, 19, 11, 3};


int deskey::cp2[48]={	13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9, 
	22, 18, 11, 3, 25, 7, 15, 6, 26, 19, 12, 1, 
	40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47, 
	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };
int deskey::totrot[16]={ 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };
deskey::deskey(string const key){						//从输入的字符构造三个64位密钥（第八位不是奇偶校验位），输入字符不足192位用‘0’补足
	string s=tool::s_to_b(key);
	tool::format_b_g(s,192);
	this->keys.push_back(s.substr(0,64));
	this->keys.push_back(s.substr(64,64));
	this->keys.push_back(s.substr(128,64));
}
vector<string> deskey::subkeyinit(int flag){				//用于生成每一轮加密所需的16个子密钥，flag标示生成子密钥用得密钥序号
	vector<string> subkeys;									//用于储存16个子密钥
	string kcp1(56,'0');
	//第一次置换
	for (int i=0;i<56;i++){
		kcp1[i]=keys[flag][deskey::cp1[i]];
	}
	//分成左右两个子密钥
	string lkey(kcp1,0,28);
	string rkey(kcp1,28,28);
	for(int i=0;i<16;i++){
		//lkey循环左移
		string temp=lkey.substr(28-deskey::totrot[i]);
		lkey=temp+lkey;
		lkey.erase(28,deskey::totrot[i]);
		//rkey循环左移
		temp=rkey.substr(28-deskey::totrot[i]);
		rkey=temp+rkey;
		rkey.erase(28,deskey::totrot[i]);
		string kcp2=lkey+rkey;
		string subkey(48,'0');
		//第二次置换
		for(int j=0;j<48;j++){
			subkey[j]=kcp2[deskey::cp2[j]];
		}
		subkeys.push_back(subkey);
	}
	return subkeys;
}


int tripledes::ip1[64]={57,    49,    41,    33,    25,    17,     9,     1,
	59,    51,    43,    35,    27,    19,    11,     3,
	61,    53,    45,    37,    29,    21,    13,     5,
	63,    55,    47,    39,    31,    23,    15,     7,
	56,    48,    40,    32,    24,    16,     8,     0,
	58,    50,    42,    34,    26,    18,    10,     2,
	60,    52,    44,    36,    28,    20,    12,     4,
	62,    54,    46,    38,    30,    22,    14,     6};



int tripledes::ip2[64]={39,     7,    47,    15,    55,    23,    63,    31,
	38,     6,    46,    14,    54,    22,    62,    30,
	37,     5,    45,    13,    53,    21,    61,    29,
	36,     4,    44,    12,    52,    20,    60,    28,
	35,     3,    43,    11,    51,    19,    59,    27,
	34,     2,    42,    10,    50,    18,    58,    26,
	33,     1,    41,     9,    49,    17,    57,    25,
	32,     0,    40,     8,    48,    16,    56,    24};



int tripledes::e[48]={  31,     0,     1,     2,     3,     4,
	3,     4,     5,     6,     7,     8,
	7,     8,     9,    10,    11,    12,
	11,    12,    13,    14,    15,    16,
	15,    16,    17,    18,    19,    20,
	19,    20,    21,    22,    23,    24,
	23,    24,    25,    26,    27,    28,
	27,    28,    29,    30,    31,     0};


int tripledes::p_box[32]={  15,     6,    19,    20,
	28,    11,    27,    16,
	0,    14,    22,    25,
	4,    17,    30,     9,
	1,     7,    23,    13,
	31,    26,     2,     8,
	18,    12,    29,     5,
	21,    10,     3,    24};


int tripledes::s_box[512]={	14,4,13,1,2,15,11,8,
	3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,
	10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,
	15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,
	5,11,3,14,10,0,6,13,

	15,1,8,14,6,11,3,4,
	9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,
	12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,
	5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,
	11,6,7,12,0,5,14,9,

	10,0,9,14,6,3,15,5,
	1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,
	2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,
	11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,
	4,15,14,3,11,5,2,12,

	7,13,14,3,0,6,9,10,
	1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,
	4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,
	15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,
	9,4,5,11,12,7,2,14,

	2,12,4,1,7,10,11,6,
	8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,
	5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,
	15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,
	6,15,0,9,10,4,5,3,

	12,1,10,15,9,2,6,8,
	0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,
	6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,
	7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,
	11,14,1,7,6,0,8,13,

	4,11,2,14,15,0,8,13,
	3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,
	14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,
	10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,
	9,5,0,15,14,2,3,12,

	13,2,8,4,6,15,11,1,
	10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,
	12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,
	0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,
	15,12,9,0,3,5,6,11};

void tripledes::loopf(string &input,vector<string> key,bool flag){
	string temp(64,'0');
	for(int i=0;i<64;i++){
		temp[i]=input[tripledes::ip1[i]];
	}
	string li=temp.substr(0,32);
	string ri=temp.substr(32,32);
	temp.clear();
	if(flag){
		for(int i=0;i<16;i++){
			temp=ri;
			tripledes::f(ri,key[i]);
			for(int i=0;i<32;i++){
				ri[i]=(li[i]+ri[i])%2+48;
			}
			li=temp;
			temp.clear();
		}
	}else{
		for(int i=0;i<16;i++){
			temp=ri;
			tripledes::f(ri,key[15-i]);
			for(int i=0;i<32;i++){
				ri[i]=(ri[i]+li[i])%2+48;
			}
			li=temp;
			temp.clear();
		}
	}
	temp=ri+li;
	for(int i=0;i<64;i++){
		input[i]=temp[tripledes::ip2[i]];
	}
}	

void tripledes::f(string &input,const string key){
	string temp(48,'0');
	//e盒扩充并且与子密钥异或运算，结果放到temp
	for(int i=0;i<48;i++){
		temp[i]=(input[tripledes::e[i]]+key[i])%2+48;
	}
	//s盒置换压缩  结果暂时放到input
	input.clear();
	for(int i=0;i<8;i++){
		string substr=tool::d_to_b(s_box[16*(int)tool::b_to_d(temp.substr(i*6,1)+temp.substr(i*6+5,1))+(int)tool::b_to_d(temp.substr(i*6+1,4))+64*i]);
		tool::format_b_g(substr,4);
		input+=substr;
	}
	//p盒置换
	temp.clear();
	temp=input;
	for(int i=0;i<32;i++){
		temp[i]=input[p_box[i]];
	}
	input=temp;
}
string tripledes::encrypt(string &input,string k,bool flag){
	deskey key(k);
	if(flag){
		tripledes::loopf(input,key.subkeyinit(0),1);
		tripledes::loopf(input,key.subkeyinit(1),0);
		tripledes::loopf(input,key.subkeyinit(2),1);
	}else{
		tripledes::loopf(input,key.subkeyinit(2),0);
		tripledes::loopf(input,key.subkeyinit(1),1);
		tripledes::loopf(input,key.subkeyinit(0),0);
	}
	return input;
}

string tool::s_to_b(string str){
	string b_str;
	for(size_t i=0;i<str.size();i++){
		bitset<8> b(str[i]);
		b_str+=b.to_string();
	}
	return b_str;
}

string tool::d_to_b(unsigned long long a){
	string s1="";
	while(a){
		int b=a%2;
		s1+=(b+48);
		a=a/2;
	}
	string s2=s1; 
	for(size_t i=0;i<s1.size();i++){
		s2[i]=s1[s1.size()-i-1];
	}
	return s2;
}

unsigned long long tool::b_to_d(string b){
	unsigned long long result=0;
	for(size_t i=0;i<b.size();i++){
		result+=(b[i]-48)<<(b.size()-i-1);
	}
	return result;
}

string tool::d_to_b(string a){
	string temp,b;
	while(a.size()){
		int carry=0;
		for(size_t i=0;i<a.size();i++){
			char c=a[i];
			a[i]=(a[i]+carry*10)/2+24;
			carry=c%2;
		}
		temp+=(carry+48);
		tool::off_z(a);
	}
	for(size_t i=0;i<temp.size();i++){
		b+=temp[temp.size()-1-i];
	}
	return b;
}

string tool::format_b_g(string &b,size_t n){
	if(b.size()<n){
		string temp(n-b.size(),'0');
		b=temp+b;
	}else{
		b=b.substr(b.size()-n);
	}
	return b;
}

string tool::format_b_d(string &b,size_t n){
	if(b.size()<n){
		string temp(n-b.size(),'0');
		b+=temp;
	}else{
		b=b.substr(b.size()-n);
	}
	return b;
}

string tool::c_to_b(char c){
	bitset<8> b;
	return b.to_string();
}

string tool::b_to_s(string b){
	string output;
	while(b.size()){
		string temp=b.substr(0,8);
		b.erase(0,8);
		bitset<8> c(temp);
		output+=c.to_ulong();
	}
	return output;
}


string tool::h_to_b(string b){
	string temp;
	for(size_t i=0;i<b.size();i++){
		switch(b[i]){
		case '0':
			temp+="0000";
			break;
		case '1':
			temp+="0001";
			break;
		case '2':
			temp+="0010";
			break;
		case '3':
			temp+="0011";
			break;
		case '4':
			temp+="0100";
			break;
		case '5':
			temp+="0101";
			break;
		case '6':
			temp+="0110";
			break;
		case '7':
			temp+="0111";
			break;
		case '8':
			temp+="1000";
			break;
		case '9':
			temp+="1001";
			break;
		case 'a':
			temp+="1010";
			break;
		case 'A':
			temp+="1010";
			break;
		case 'b':
			temp+="1011";
			break;
		case 'B':
			temp+="1011";
			break;
		case 'c':
			temp+="1100";
			break;
		case 'C':
			temp+="1100";
			break;
		case 'd':
			temp+="1101";
			break;
		case 'D':
			temp+="1101";
			break;
		case 'e':
			temp+="1110";
			break;
		case 'E':
			temp+="1110";
			break;
		case 'f':
			temp+="1111";
			break;
		case 'F':
			temp+="1111";
			break;
		default:
			cout<<"输入数值有误";
			break;
		}
	}
	return temp;
}
string tool::b_to_h(string b){
	string temp;
	string output;
	if(b.size()%4){
		tool::format_b_g(b,4*(b.size()/4+1));
	}
	while(b.size()){
		temp.clear();
		temp=b.substr(0,4);
		b.erase(0,4);
		if(temp=="0000"){
			output+='0';
		}else if(temp=="0001"){
			output+='1';
		}else if(temp=="0010"){
			output+='2';
		}else if(temp=="0011"){
			output+='3';
		}else if(temp=="0100"){
			output+='4';
		}else if(temp=="0101"){
			output+='5';
		}else if(temp=="0110"){
			output+='6';
		}else if(temp=="0111"){
			output+='7';
		}else if(temp=="1000"){
			output+='8';
		}else if(temp=="1001"){
			output+='9';
		}else if(temp=="1010"){
			output+='A';
		}else if(temp=="1011"){
			output+='B';
		}else if(temp=="1100"){
			output+='C';
		}else if(temp=="1101"){
			output+='D';
		}else if(temp=="1110"){
			output+='E';
		}else if(temp=="1111"){
			output+='F';
		}else{
			cout<<"输入有误";
		}
	}
	return output;
}

string tool::off_z(string &str){
	str.erase(0,str.find_first_not_of('0'));
	return str;
}

//--------------------------------------------------------------------------
//								DES Encrypt
//--------------------------------------------------------------------------
CAES::CAES(string input,string k)
{
	string temp=tool::s_to_b(input);
	while(temp.size()){
		this->b64.push_back(temp.substr(0,64));
		temp.erase(0,64);
	}
	tool::format_b_d(this->b64[b64.size()-1],64);
	this->key=k;
}

string CAES::encrypt()
{
	string temp;
	for(size_t i=0;i<this->b64.size();i++){
		tripledes::encrypt(b64[i],this->key,1);
		temp+=b64[i];
	}
	return tool::b_to_s(temp);
}
string CAES::decrypt()
{
	string temp;
	for(size_t i=0;i<this->b64.size();i++){
		tripledes::encrypt(b64[i],this->key,0);
		temp+=b64[i];
	}
	return temp=tool::b_to_s(temp);
}