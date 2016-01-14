// each object body in the heap, has at the beginning the token related to that object.
// from the token we may get it's class and from the class every characteristic of the object.
// When the body is freed the token t is replaced and holds the total size fredd.
// Because the compressor scans that memory area when it finds an integer interprets it as a 
// non signed number. So when the number is negative it's first bit must be cleared, in order to get one more bit of 
// capacity for the maximum size.
// So the deallocator knows how many free bytes follow.
// That means that the maximum size of allocation units that an object may hold is given by half of the maximum value 
// a uWord is able to hold.

// An object is referenced by a token that is an index in the vectors table.
// So the object is vector+body. The body has a head+tail ( the tail contains the actual data ).
// The head starts with a backreference to the vector.
// If the object belongs to a fixed size class then the class holds the size of the object. 
// A special value should be hold in that field if the size instead of being fixed is variable.
// So the compressor may find in any case the body size (head+tail).
// A size of 0 isn't acceptable.
// Must be clarified if the size includes or not the initial Token.

template <typename sizeT>
struct hGeneric_{
	typedef sizeT size_t;

	Token token;
	hGeneric_(Token t):token{t}{}
// The generic body has a null size for it's data. The above may be overridden by derivative classes
	sizeT sizeofData(){return 0;}; 
};

using hGeneric=hGeneric_<uWord>;

template <typename T,typename sizeT,sizeT sz=1> 
// T is the type of the object contained in the tail,sz is the static number of such objects
// The hFixBase during definition takes as parameter both: a) the type of holdeed data "T" and the 
// "sizeT" type that is the holder for the size of data.
struct hFixBase:hGeneric_<sizeT>{

	typedef T element_t;static const sizeT size=sz;

	hFixBase(Token t):hGeneric_<sizeT>{t}{};
	
// the dataP() returns the first address to the contained data
	T* dataP(){return (T*)(this+1);}
// the data(i) returns a reference to the i-th datum
	T& data(uWord i){return *(i+(T*)(this+1));}
// the sizeofData() returns the whole size of the contained data
	uWord sizeofData(){return sizeof(T)*size;}
};

// the usual hFix header is defined as holding "Word" and having a type "uWord" as the holder for the size of data.
using hFix=hFixBase<Word,uWord>;


// the hVarBase needs a type for the holded data "T", needs a type for the variable holding the number of holded data "sizeT".
template <typename T,typename sizeT> 
struct hVarBase_:hGeneric_<sizeT>{

	typedef T element_t;typedef sizeT size_t;

	sizeT size;
	
// the meaning of the 3 bellow functions is the same as in the hFixBase.
	T* dataP(){return (T*)(this+1);}
	T& data(uWord i){return *(i+(T*)(this+1));}
	sizeT sizeofData(){return sizeof(T)*size;};

	hVarBase_(Token t,sizeT sz):hGeneric_<sizeT>(t),size(sz){}

};

template<typename T> using hVarBase=hVarBase_<T,uWord>;
template<typename T> using hVarBaseS=hVarBase_<T,suWord>;

// an aggregate is a variable size array with a pointer to the first free element.
// the size of the body is determined from the maxn field.

// Size of structures doesn't works correctly when data is used.
// the size is shown as bigger.

template <typename T,typename sizeT> 
struct hAggregateBase_:hVarBase_<T,sizeT>{
	typedef T element_t;typedef sizeT size_t;
	sizeT n;
	T* dataP(){return (T*)(this+1);}
	T& data(uWord i){return *(i+(T*)(this+1));}
	//hAggregateBase_(Token t,sizeT sz,sizeT n0=0):hVarBase_<T,sizeT>{t,sz}{};  // must be deleted if found that isn't needed
	hAggregateBase_(Token t,sizeT sz,sizeT n0=0):hVarBase_<T,sizeT>{t,sz},n{n0}{};
};

// bellow we have two kind of aggregares with different kind of possible maximum size for the elements.

/*
template<typename T> using hAggregateBase=hAggregateBase_<T,uWord>;
template<typename T> using hAggregateBaseS=hAggregateBase_<T,suWord>;
*/

using hAggregate=hAggregateBase_<Word,suWord>;
using hAggregateL=hAggregateBase_<Word,uWord>;

// the queue is not chekced or correctly implemented, must be developed further !!! Not to be used as is actually !!!
template <typename T,typename sizeT>
struct hQueueBase_:hAggregateBase_<T,sizeT> {

	typedef T element_t;
	typedef sizeT size_t;

	sizeT nStart;
	T* dataP(){return (T*)(this+1);}
	T& data(uWord i){return *(i+(T*)(this+1));}
};

/*
A document describing the prototype implementation must be included here or refered at.
In little words a prototype is a structure in the sense of C.
That means that a structure is something that has a list of field names with associated values.
In the simplest case a structure has it's names and values all inside the structure.
In the generic case, some names reside in an anchestor from were the structure inherits.
So there are structures that have more names than values, so in that case names are to be used from descendant structures.
*/

// It seems that prototype as defined isn't actually used as such so it is commented 

/*

template <typename T,typename sizeT> 
struct hPrototypeBase_:hAggregateBase_<T,sizeT>{
	sizeT fieldsNum,levelsNum;
	
	T* values(){return (T*)(this+1);}
	T* fields(){return (T*)(this+1)+this->n;}
	T* levels(){return (T*)(this+1)+this->n+fieldsNum;}

	T* dataP(){return (T*)(this+1);}
	T& data(uWord i){return *(i+(T*)(this+1));}
	
};

//using hPrototype=hPrototypeBase_<uint8_t,Word>;
//using hPrototype=hPrototypeBase_<uint16_t,Word>;

using hPrototype=hPrototypeBase_<Word,suWord>;
using hPrototypeL=hPrototypeBase_<Word,uWord>;

*/

template<typename T> bool same(const T& a,const T& b){return &a==&b;}







