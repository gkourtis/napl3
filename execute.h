struct level{
	Token l;
	level(Token i):l{i}{};
	level():l{0}{};
	operator Token(){return l;}
//both postfix operators must take a dummy int parameter in order to differentiate from the prefix increment
	Token operator--(int){return l ? l-- : l;} 
	Token operator++(int){return l++;}										 
// prefix operators don't
	Token& operator--(){return l ? --l : l;}
	Token& operator++(){return ++l;}
};

Token curlyOpenT,curlyCloseT;


struct ET:std::thread{
	size_t index;int in,out;FILE *sin=nullptr,*sout=nullptr;
	Aggregate stacks,exS,trackedS;
	void setAlive();
	Aggregate stack(){return stacks.tos();}
	level execution=1,paren=0,counter=0;Int iCounter;
	level charLevel=0,charCounter=0;Char delim=0;
	Token leaveN=0;bool exitInterpretLoop=false;
	
	volatile bool threadEnded;
	//bool threadEnded;
	
	typedef void connectionHandler_t(size_t,std::atomic_bool*);
	ET(connectionHandler_t f,size_t ind,std::atomic_bool *readyP,int in_=-1,int out_=-1)
	:std::thread{f,ind,readyP}, in{in_}, out{out_}, index{ind}{
		++activeETs;
		if(in_==-1) in_=0;if(out_==-1) out_=in;sin=fdopen(in,"r");sout=fdopen(out,"w");
		newStack(0);puts("New Execution Thread created and running");
	}
/*	
	ET(int in_=-1,int out_=-1,index{ind})
	:std::thread{}, in{in_}, out{out_}, index{ind}{
		activeETs++;
		if(in_==-1) in_=0;if(out_==-1) out_=in;sin=fdopen(in,"r");sout=fdopen(out,"w");
		newStack(0);puts("New Execution Thread created and running");
	}
*/	
	void ETclose(){
		fclose(sin);fclose(sout);
		threadEnded=true;
		--activeETs;
	}
	
	int write(std::stringstream& ss){
		return ::write(out,ss.str().c_str(),ss.str().size());
	}
	
	int write(const char*s){return ::write(out,s,strlen(s));}
	
	void puts(const char* s){
		write(s);write("\n");
	}
	bool normal(),mustContinue();
	void execute(Obj o,bool forceFirst=false,Word debugLevel=0);
	void showStack(std::stringstream& ss);
	void showStack();
	int print(obj);
	void execute0(obj o,int d,bool forceFirst,int debugLevel);
	int getExecutionLevel(obj o,bool setIt=false);
	void setExecutionLevel();
	void curlyOpen(uWord level);
	void curlyClose(uint level);
	String getLine();
	void* connection_handler();
	
#include "stacks.h"
#include "controlStructures.h"
#include "patternMatching.h"
#include "interpreter.h"
#include "methods.h"

void executeMethod(Obj o);
template<typename... TT> void print(TT... tt);

};

void track(Token t){
	if(etP && etP->trackedS.w) etP->trackedS.push(t);
}
void untrack(){if(etP) etP->trackedS.pop();}

std::vector<ET> ETs;

std::atomic_bool abandon;


