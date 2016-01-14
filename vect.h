const Word VECT_COUNTER0=__COUNTER__+1;
#define Cl(x1,className,x3,x4) className = TokenMin+__COUNTER__-VECT_COUNTER0,
	enum class ClassT:Word { 
		classInfoNames
	};
#undef Cl

// if addrT has a size smaller than (void*) when extension of addrT happens in order to be
// addeed to an address then it should be negativelly extended due to the implementation.
// In order to support at least half of the addressable space we should at least sign extend it 
// during conversion to a larger size.
// If we need to support all the addressable space, than the simple solution is to use a 
// zero extended addrT ( so an unsigned type ) but to change the mm memory in order to 
// accomodate a negative high prefix.

typedef uint32_t addrT;

// debugging notes: the code doesn't work under the architecture m64.

// the addrT may be set as uint8_t, uint16_t, uint32_t.
// the problem is that in that case the addresss of functions should fit in that space
// and that isn't always the case.
// In case we need to do it, we may use the Token as an index into a table of primitives
// and align those primitives t some fixed multiple of 2.
// in that way, these primitives would have an implied class and an execution level of 1

// Explanation of readOnly: When an object is created as a unique instance, we may reuse it's space as a new 
// object when that is necessary ( e.g. when creating a String and using it in an operation ).
// When the object is created thus the flag is false.
// If a copy of the object is taken than the flag is set and modifications must create a clone where to operate.

// As there is duality between values and references, when a vector is used for a variable the reference flag is set
// and so the index of the vector in the stack indicates the reference to the vector and not the value contained.
// Instead values pushed should have the reference flag cleared, and so indicate the value itself.

#define 	bitFields bf(collect,1) bf(readOnly,1) bf(reference,1) bf(multipleInheritance,1) \
					bf(fields,1) bf(hideFields,1) bf(actionId,8) 

// the code betwen #define #undef counts the number of defined bits
#define bf(fieldName,bits) + bits
	const int bitsUsed=bitFields;
#undef bf


// the vect holds a class, an address, an execution level, and bit fields
struct __attribute__((packed)) vect{
	enum ClassT Class;
	addrT addr0;
	suWord executionLevel:(sizeof(suWord)*aguBits-bitsUsed);
#define bf(fieldName,bits) suWord fieldName:bits;
	bitFields
#undef bf

// methods
	inline ptr addr(const void* p);
	inline addrT addr(const addrT q);
	
	inline operator ptr();
	inline operator addrT();
	
	inline Token& value(){return *(Token*)&addr0;}
	
	#define fa(T,name) T name(){return (T)operator ptr();}
		fa(hGeneric_<uWord>*,hGenericP)
		fa(hAggregate*,hAggregateP)
		fa(hAggregateL*,hAggregatePL)
		fa(hFix*,hFixP)
		fa(hVarBase<agu>*,hVarBaseP)
		fa(primitiveP,executeP)
	#undef fA
};

#undef bitFields


void __attribute((constructor)) message_vect0(){
	std::cout<<" sizeof(Word)="<<sizeof(Word)<<" sizeof(addrT)="<<sizeof(addrT)<<" sizeof(vect)="<<std::dec<<sizeof(vect)<<"\n";
}
