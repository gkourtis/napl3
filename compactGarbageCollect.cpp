
//bool isStackAddress(ptr p){const int m=0xFF000000; return (int(p) & m) == m;}

#define clock std::chrono::high_resolution_clock

class setAlive;
bool isSize(size_t sz){return sz<0x100000;}

void checkforNullAddr(){
	for(size_t i=0,c=0;i<vvElements || (print(" with null addr:",c),false);i++){
		obj o(i2t(i));if(o->collect && o->addr0==0) 
			c++;
	}
}
// The locic besides garbage collection is the subsequent:
// Each aggregate, should point to at least 2 elements.
// If an aggregate contains only 1 pointed element than the operation is optimized
// and no stack frame is generated.
// The above thoughts, tell us that the necessary stack depth for tracking the 
// garbage collection position is log² vectorsNum.
// So if that space is preallocated, it will never serve a bigger stack to track the 
// garbage collection process.


void checkCompact(heap& H){
	ptr s;size_t sz;bool b;
	for(s=H.mmStart;s<H.mm()+freePos;){
		obj o=((hGeneric*)s)->token;
		if(o.isToken()){
			sz=o.sizeofBody();
			std::cout<<std::hex<<"addr:"<<(long)s<<" token:"<<o.w<<" sz:"<<std::dec<<o.sizeofBody()<<std::endl;
			s+=sz;
		}
		else
			if(isSize(o.w)){
				s+=o.w;std::cout<<"addr:"<<std::hex<<(long)s<<" sz:"<<std::dec<<sz<<std::endl;
			}else{
				std::cout<<"WrongSz:"<<std::dec<<o.w<<std::endl;
			}
	}
	if(s!=H.mm()+freePos) std::cout<<"Check compact s!=mm+freePos";
}

void compact(heap& H){
	waitAllStopped();
	print("Compact...");
	auto t1=clock::now();
	ptr s,d;size_t sz;bool b;
	for(s=d=H.mmStart;s<H.mm()+freePos;){
		obj o=((hGeneric*)s)->token;
		if(o.isToken()) {
			sz=o.sizeofBody();
			if(sz>=0x100000)
				std::cout<<"sz<0";
			if(s!=d) {
				memmove(d,s,sz);
				o.bodyP(d);
			}
			s+=sz,d+=sz;
		}
		else 
			if(isSize(o.w)) 
				s+=o.w;
			else 
				assert0(false,"unexpected in compact");
	}
	assert0(H.mm()+freePos==s,"unexpected freePos!=s in compact");
	freePos=d-H.mm();
#ifndef NDEBUG
	memset(d,0xAA,freeMem());
	//signalFreePos();
#endif
	auto t2=clock::now();
	auto microseconds=std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
	
	//double dt=t*clock::period::N/clock::period::D;
	print(" compacted in microseconds:",microseconds);

}


// The locic besides garbage collection is the subsequent:
// Each aggregate, should point to at least 2 elements.
// If an aggregate contains only 1 pointed element than the operation is optimized
// and no stack frame is generated.
// The above thoughts, tell us that the necessary stack depth for tracking the 
// garbage collection position is log² vectorsNum.
// So if that space is preallocated, it will never serve a bigger stack to track the 
// garbage collection process.

inline void setAlive(Obj d){
	Aggregate& A=*(Aggregate*)&d;// The locic besides garbage collection is the subsequent:
// Each aggregate, should point to at least 2 elements.
// If an aggregate contains only 1 pointed element than the operation is optimized
// and no stack frame is generated.
// The above thoughts, tell us that the necessary stack depth for tracking the 
// garbage collection position is log² vectorsNum.
// So if that space is preallocated, it will never serve a bigger stack to track the 
// garbage collection process.


	if(d.isToken() && d->collect ){
		

//		if(!(0<=(Word)d->Class && (Word)d->Class<0x100))
//			std::cout<<"Stop"<<std::endl;
			
		d->collect=false;
		
		obj c=d.Class();
		if(c.w==0)
			std::cout<<"c==0";
		SWITCH(c)
			//case _class0:
//			CASE(classClass,classUndefined,classPrimitive,classExtStruct,classFix,classInt,classFloat,classDouble,classInt64,classBitString,classVar,classField) return;
			CASE(classClass,classUndefined,classPrimitive,classInt,classFloat,classDouble,classInt64,classBitString) return;
			DEFAULT
				if(classString>=c) return; 
				if(classAggregate>=c){
					for(size_t i=0;i<A.elements();i++)
						setAlive(A[i]);
					return;
				}
				if(c.Class()==classClassInfo){ // means that it is a classInfo 
					setAlive(c.Class());
					return;
				} 
				assert0(false,"ProblemIn_switch_during_garbageCollect");
		endSWITCH
	}
}



void dictionarySetAlive(Aggregate d,int cols=2){
	for(uWord i=0;i<d.elements();i+=cols){
		obj o=d[i+1];
		if(!o.isInstanceOf(classUndefined)) setAlive(o);
	}
	for(uWord i=0;i<d.elements();i+=cols){
		obj o=d[i+1];
		if(!o->collect)
			setAlive(d[i]);
		else{
			d.ins_backdel(i+cols,-cols);
			i-=cols;
		}
	}
}

using namespace std::chrono;
high_resolution_clock::duration::rep benchMicroSeconds(high_resolution_clock::time_point t){
	return duration_cast<milliseconds>(high_resolution_clock::now()-t).count();
}

auto Now(){return std::chrono::high_resolution_clock::now();}

void cloneMemory(){
	auto t=clock::now();
		M1=M;
	print("clone ",sizeof(M),"bytes memory in ",benchMicroSeconds(t),"_milliseconds");
}
	
void garbageCollect(heap& H){
	waitAllStopped();
	cloneMemory();
	std::cout<<"\ngarbageCollect started";
	auto t1=clock::now();
	for(auto p=&H.vv[0];p!=&H.vv[vvElements];p++) p->collect=true;

	
	print(" vvElements=",vvElements);
	print(" freeClasses=",freeVectors(freeClass,true));
	print(" freeVectors=",freeVectors(freeObj,true));

	checkforNullAddr();

	// The code bellow kills all the ended tasks.
	for(auto i=0;i!=ETs.size();++i){
		if(ETs[i].threadEnded && ETs[i].joinable()){
			ETs[i].join();
			ETs.erase(ETs.begin()+i);--i;
		}else
			ETs[i].setAlive();
	}
	
	//dictionarySetAlive(dict);
	setAlive(dict);
	setAlive(persistents);
	
	for(auto& i:StringLitterals) setAlive(i.second);

	for(size_t i=0,c=0;i<vvElements || (print(" killed:",c),false);i++){
		Obj o=i2t(i);
		if(o->collect){
			c++;o.kill();
		}
	}
	
	auto t2=clock::now();
	auto microseconds=std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
	print("garbageCollect Ended in microseconds:",microseconds);
}
