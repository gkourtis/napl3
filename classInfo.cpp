
inline obj newClass(Obj fatherClass){
	obj o=reserveClass();
		o.Class(fatherClass);o.executionLevel(0);
	return o;
}

/*  * 
The classInfo data must reside in an integer number of dataT related to the aggregate in order to be able to define 
an integer numer of dataT that determines it's size.
So in case classInfo doesn't fit's in the above size it must be rounded up
*/

template<typename dT>
const size_t classInfoElements=roundUpDivision(sizeof(classInfo0),sizeof(dT));

template<typename dT>
const size_t classInfoPadSize=classInfoElements<dT>*sizeof(dT)-sizeof(classInfo0);

/* 
A typical classInfo structure is used besides other in order to hold:
a)	the address of the function used to execute instances of a specific class
b)	the size of the header of instances
		the size of the data elements of the object
		a pointer to a method function that allows to calcuate the size of the tail (data) section of the instance

The size of the header of instances may not be 0 as that size includes the token backreference
So if it is 0 it means that no body exist ( actually the same function is implemented by checking instead of 0 for -1)
As objects may be formed from dataElements the size of a single dataElement is saved as the sizeofElement
Fixed size objects, will contain thus a fixed size dataElements
Variable size objects, will contain thus a variable size dataElements
The pointer to the method function, is used in order to use the above data and calculate the size of the data portion
All those data are used from the memory compacter.
*/
const uWord noSize=(uWord)-1;
template<typename hT,typename dT>	obj newClassInfo(
	Obj fatherClass,bool fields,bool fix,size_t totalElements,size_t initialElements
){
/* 
The function creates a classInfo structure. The classInfo structure is a specific Aggregate.
The fatherClass is given. A bool fields is used in order to signal that the classInfo contains too fields.
A bool fix is used to specify if the instances have fixedSize (not variable as Aggregates are)
*/
	obj o=newClass(fatherClass);o->fields=fields;
		totalElements=MAX(totalElements,classInfoElements<dT>);
		initialElements=MAX(initialElements,classInfoElements<dT>);
		o->addr(allocate0(sizeof(classInfo)+sizeof(dT)*(totalElements-classInfoElements<dT>))); // gk20160102
		classInfo &i=*(classInfo*)(o->operator ptr());
		new (&i) classInfo(o.w,totalElements,initialElements);
		//if(totalElements==classInfoElements<dT> && initialElements==classInfoElements<dT>){
			i.executeFunction=&pushObj;
			i.sizeofData_MP=(sizeofData_MP_t)&hT::sizeofData;
// if the data held in the object have a fixed size and that size may be contained inside the addrT 
// then no body is created and the data is held directly inside the addrT
			i.sizeofHeader= fix && sizeof(dT)<=sizeof(addrT) || sizeof(hT)==0  ?  noSize:sizeof(hT) ;
			i.sizeofElement= ( sizeof(dT)!=0 ? sizeof(dT) : noSize );
		//}
	return o;
}


//#define Cl0(className,fatherClass) __attribute__((init_priority(102))) Obj className{newClass(fatherClass)};
	
//#define ClB(className,fatherClass,hT,dT,fix) __attribute__((init_priority(102))) Obj className=newClassInfo<hT,dT>(fatherClass,fix);

//#define ClA(className,fatherClass) ClB(className,fatherClass,hAggregate,Word,false);

//#define ClF(className,fatherClass,dT) ClB(className,fatherClass,hGeneric,dT,true);

#define Cl(L,className,fatherClass,dT) \
	__attribute__((init_priority(102))) Obj className{\
		#L[0]=='0' ? newClass(fatherClass) :\
		#L[0]=='A' ? newClassInfo<hAggregate,dT>(fatherClass,false,false,classInfoElements<dT>,classInfoElements<dT>):\
		#L[0]=='B' ? newClassInfo<hAggregate,dT>(fatherClass,true,false,0x8,0):\
		#L[0]=='F' ? newClassInfo<hGeneric,dT>(fatherClass,false,true,0,0) : obj{0}\
	};

// Cl0 creates a simple class with no body
// ClB creates basic classes with or without body
// ClA creates a class with classInfo body of type aggregate
// ClF create fixed body classes 
	
// here we create a class "classNotFound" in order to use it as a classNotFound indicator in classAnchestor searching
// it's father class is itself ( indicated by the freeClass fatherClass initialization ).
			
	classInfoNames

//__attribute__((constructor)) void setClassOf_classClass(){classClass.Class(classClassInfo);}

#undef Cl




