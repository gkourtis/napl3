/* 
Classes are divided into raw classes, with no body attached and normal classes with body.
Raw classes have only a method that is applicable to their descendants: the execute 
method. That method is stored in the Addr Field ( as what kind ?)
The body of normal classes carries information about the form of their descendants as execution method, 
sizeof head, sizeofelement, method to calculate the size of tail\
derived classes do not need to keep those informations, as these may derive from the anchestor. 
So finally there are 3 type of classes:
a) Raw classes
b) classes having a body with a classInfo structure
c) classes that do not have a classInfo structure but that derive it from anchestor classes.
*/
 
const uWord executionClassesNum=0x100;

void pushObj(obj o);
typedef uWord (hGeneric::*sizeofData_MP_t)();

/*
A classInfo0 structure describes characteristics of the descendants of a particular classInfo.
It is a raw description of it. That raw description will be made a descendant of a high level "object".
an executeFunction is available that always contains the function executed by descendants during execution.
SizeofHeadeisr is directly available as a value. 
The header is guaranted not to be 0 as it always has at least the size of token.
SizeofElement is also directly available
A classInfo could could omit it ( by allowing it to be -1 -- or we could decide to set that case 0).
the classInfo0 is appended after a header.
*/

const uWord noBody=(uWord)-1;

struct classInfo0{
// the first element of a classInfo is the address of a function that is the execution of an object belonging to that class
	void (*executeFunction)(obj);							//=&pushObj; 
// the size of the header of the body. If -1 means that there is no header and so no body. 
// We could also change and use the value 0 as the header may not be 0
	uWord sizeofHeader; 
	uWord sizeofElement; 
// the sizeofData is much more flexible as it is calculated using a method indicated by a method pointer. 
// The sizeofData usually depends on the sizeofElement and the max number of elements.
	union {
// the union is used in order to allow access in two ways to the method pointer
		sizeofData_MP_t sizeofData_MP{nullptr};	// here is a pointer to a method function.
		struct{
			void* sizeofDataP;
			size_t offset0;
		};
	};
};

// the classInfo is what is found directly in the Heap id est: a hAggregate header and a classInfo0.
struct classInfo:hAggregate,classInfo0{
	obj Class(){return token;}; // returns the Class of the classInfo
	bool hasHeapBody(){					// returns if the instances have a body.
		return sizeofHeader!=noBody;
	}
	classInfo(Token t,hAggregate::size_t sz,hAggregate::size_t n):hAggregate{t,sz,n}{}
};

//classInfo (&classInfos_)[classInfoN]=*reinterpret_cast<classInfo(*)[classInfoN]>(M.mmStart);

// given a class o, we get a reference to the classInfo structure.
classInfo& classInfos(obj o){
		return *(classInfo*)((ptr)o.V());
}

// A new class vector and body has been created.
// The function bellow initializes the classInfo structure as required 
// given a type of head a type of data and a number of data (default 1).
template<typename hT,typename dT,int n=1>
	void classInfoInit(obj newClass,obj fatherClass,bool fix=false)
	{ classInfo& i=classInfos(newClass);
		newClass.Class(fatherClass);
		i.sizeofData_MP=(sizeofData_MP_t)&hT::sizeofData;
// if the object is a fixed size with size fiting inside the ptr then the object hasn't a header and sizeof(hT)=noBody
#if true
		i.sizeofHeader= fix && sizeof(dT)*n<=sizeof(addrT) || sizeof(hT)==0  ?  noBody:sizeof(hT) ;
		i.sizeofElement= ( sizeof(dT)!=0 ? sizeof(dT) : noBody );
#else
		bool noBody= sizeof(hT)==0 || fix && sizeof(dT)*n<=sizeof(addrT);
		if(noBody){
			i.sizeofHeader=0;
			i.sizeofElement=0;
			i.sizeofTail=0;
		}else{
			i.sizeofHeader=sizeof(hT);
			i.sizeofElement=sizeof(dT);
			i.sizeofTail=n*sizeof(dT);
		}
#endif
	};

template<>
	void classInfoInit<void,void>(
		 obj newClass
		,obj fatherClass
		,bool fix
	)
	{ classInfo& i=classInfos(newClass);
		newClass.Class(fatherClass);
		i.sizeofData_MP=nullptr;
		i.sizeofHeader= noBody;
		i.sizeofElement= noBody;
	};

	// init a classInfo with instances that do not have body.
	void classInfoInitDefault(Obj c,Obj C){classInfoInit<void,void>(c,C,true);}
		
	// Given the class <T> init a classInfo with instances that have a hFixBase header and a fixed size datum.
	template<typename T> void classInfoInitFix(Obj c,Obj C){
		classInfoInit<hFixBase<T,uWord>,T>(c,C,true);
	};

// some classes do not have size information, instead only executional behavior

	template<typename... TT>
	void classInfoInitAggregate(TT... aa){classInfoInit<hAggregate,Word>(aa...);};

//bool baseClass(obj c){return c<_classEnd;}
/*
classInfo &classInfoOf(obj o){
	do{ 
		o=o.Class();if(o.Class()==classClassInfo) return classInfos(o);
	}while(o.Class()!=classClass);
	return *(classInfo*)nullptr;
}
*/
obj classInfoOf(obj o){
	do{ 
		o=o.Class();
		if(o.Class()==classClassInfo) return o;
	}while(o.Class()!=classClass);
	return 0;
}