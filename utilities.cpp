//#include <stdio.h>
#include <limits>
//#include <stdlib.h>
#include <string.h> // memset 
#include <iostream>
#include <fstream>
//#include <stdarg.h>
//#include <locale>
#include <vector>
#include <wchar.h>
#include <ctype.h>
#include <csignal>
//#include <string>
#include <cmath>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <sys/param.h>


// msecs covers a period of almost 50 days - for long having 32 bits - from the boot of the PC, afterwards it starts again.
unsigned long msecs(){timeval t;gettimeofday(&t,NULL);return t.tv_sec*1000+t.tv_usec/1000;}

// countof gives the number of elements of an array
//template <typename T, size_t N> size_t countof( T (&array)[N] ){return N;}

enum sign{less=-1,equal=0,more=1};
enum sign sign(int i){ return i ? (i<0 ? less : more ) : equal;}

bool postClear(bool &b){bool r=b;b=false;return r;} 
bool postSet(bool &b)		{bool r=b;b=true; return r;}

bool assert0(bool f,const char *fmsg,const char *tmsg){
	if(f){
		if(tmsg!=nullptr) std::cout<<tmsg<<std::endl;
	}else{
		if(fmsg!=nullptr) std::cout<<fmsg<<std::endl;
		sleep(5);exit(0);
	}
	return f;
}





#define concat(a,b) a##b
#define autoname1(cnt) concat(autoname_,cnt)
#define autoname autoname1(__COUNTER__)

#define autolname1(ln) concat(auton,ln)
#define autolname autolname1(__LINE__)

#define onStartupExecute_(func,structname) void func();struct structname{structname(){func();}} autoname;
#define onStartupExecute(func) onStartupExecute_(func,autoname)

void mbslenghts(const char *mbs,size_t maxBytes,size_t &i,size_t &mbchars){
// a multibyte string has two lengts: a) the number of total characters (bytes) b) the number of resulting multibyte characters ( b<=a ).
	size_t sz=mblen(nullptr,0);mbchars=0;
	for(i=0;i<maxBytes;mbchars++,i+=sz){
		if( 0==(sz=mblen(mbs+i,maxBytes-i))) return; 
	}
}

inline int bitLessEqual(int a,int b){return ~a&b;}

void stop(){
	std::cout<<"stopped";
}

constexpr int roundUpDivision(int dividend,int divisor){return ((dividend+divisor-1)/divisor);}


/*
wchar_t num2digit(unsigned int n){return n<10 ? L'0'+n : L'A'+(n-10);}
unsigned int digit2num(wchar_t wc){return wc<=L'9' ? wc-L'0' : wc-L'A'+10;}

wstring num2wstring(unsigned long n,int digits,int base=10){
// convert a long number according to base to a wstring
	wstring r;
	for(int i=digits;--i>=0;){r=num2digit(n%base)+r;n/=base;}
	return r;
}

string wstring2string(wstring ws){
// convert a wstring to string	
	string s(ws.length()*MB_LEN_MAX,'\0');
	size_t l=wcstombs((char*)s.data(),ws.data(),s.length());
	s.resize(l);
	return s;
}

wstring string2wstring(string s){
// convert a string to wstring	
	wstring ws(s.length(),L'\0');
	size_t l=mbstowcs((wchar_t*)ws.data(),s.data(),ws.length());
	ws.resize(l);
	return ws;
}

//string wstring2charP(wstring ws){return wstring2string(ws).c_str();}

//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

void assert0(bool f,const char *s){if(!f){wcout<<printf("%s\nn",s);exit(1);}}
	
wstring file2wstring(wstring wfn){
// given a wstring as the name of a file, the file is read as a multibyte and converted to wstring
	ifstream is(wstring2string(wfn));
	  is.seekg(0,ios::end);size_t l=is.tellg();string s(l,'\0');is.seekg(0);is.read(&s[0],l);
	is.close();
	return string2wstring(s);
}
	
string cwd(){
	char* s0=getcwd(NULL,0);
		string s(s0);
	free(s0);
	return s;
}

wstring wcwd(){
	char* s0=getcwd(NULL,0);
		wstring ws(MAXPATHLEN,0x0);int l=mbstowcs(&ws[0],s0,MAXPATHLEN);
	free(s0);
	ws.resize(l);return ws;
}

*/
