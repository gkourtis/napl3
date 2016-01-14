#include <climits>

unsigned long msecs(); // msecs covers a period of almost 50 days - for long having 32 bits - from the boot of the PC, afterwards it starts again.
template <typename T, size_t N> size_t countof( T (&array)[N] ){return N;} // countof gives the number of elements of an array

bool assert0(bool f,const char *fmsg=nullptr,const char *tmsg=nullptr);

bool postClear(bool &b);
bool postSet(bool &b);
template<typename type> struct backup{
	type v0,*v0P;
	backup(type &v,type new_v){v0=v;v=new_v;v0P=&v;}
	~backup(){*v0P=v0;}
};

template<class T> size_t bitsizeof(T){return CHAR_BIT*sizeof(T);}

/*
extern "C"{
	char * readline(char *prompt);
  void add_history(char  *line);
}
*/
//wchar_t num2digit(unsigned int n);
//unsigned int digit2num(wchar_t wc);
std::wstring num2wstring(unsigned long n,int digits,int base);
//string wstring2string(wstring ws);
//wstring string2wstring(string s);
//wstring file2wstring(wstring wfn);
//string cwd();
//wstring wcwd();

/*
template <typename A,typename B> bool min(const A& a, const A& b)
{if(sizeof(A)>=sizeof(B)) return min(a,(A)b); else return min((B)a,b);}

template <typename A,typename B> bool max(const A& a, const A& b)
{if(sizeof(A)>=sizeof(B)) return min(a,A(b)); else return min(B(a),b);}
*/

const char* compiler=
#if defined(__clang__)
	"Clang"
#elif defined(__ICC) || defined(__INTEL_COMPILER)
	"Intel ICC/ICPC"
#elif defined(__GNUC__) || defined(__GNUG__)
	"GNU GCC/G++"
#elif defined(__HP_cc) || defined(__HP_aCC)
	"Hewlett-Packard C/aC++."
#elif defined(__IBMC__) || defined(__IBMCPP__)
	"IBM XL C/C++"
#elif defined(_MSC_VER)
	"Microsoft Visual Studio"
#elif defined(__PGI)
	"Portland Group PGCC/PGCPP"
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	"Oracle Solaris Studio"
#endif
;

template<typename T,typename... TT>
inline bool manyCompare(T subj,T x,TT... args)
	{return (subj==x) || manyCompare(subj,args...); }

template<typename T> 
inline bool manyCompare(T subj){return false;}

#define SWITCH(x) { auto __SWITCHARG=x; if(false){
#define endSWITCH }}
#define DEFAULT }else{

#define CASE(args...) }else if(manyCompare(__SWITCHARG,args)){



