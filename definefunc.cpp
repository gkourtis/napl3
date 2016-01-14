obj definefunc(String s,void (*p)(),int executionLevel=1) {
	obj o;
	if(executionLevel!=1){
		o=newDeferred(primitive2t(p));o.executionLevel(executionLevel);
	}else
		o.w=primitive2t(p);
	dict.define(s,o);
	return o;
}

obj definefunc(const wchar_t* ws,void (*p)(),int executionLevel=1){
	String s(wcs2StringT(ws));
	return definefunc(s,p,executionLevel);
}

/*
obj definefunc(const wchar_t* ws,void (ET::*p)(),int executionLevel=1){
	tString s(wcs2StringT(ws));
	return definefunc(s,p,executionLevel);
}
*/


#define def00(t,name,nameStr,code)\
void __attribute__((aligned(primitivesAlignement))) name(){code}\
__attribute__((constructor)) void autoname(){t=definefunc(L##nameStr,&name);}

#define def00Level(t,name,nameStr,code,level)\
void __attribute__((aligned(primitivesAlignement))) name(){code}\
__attribute__((constructor)) void autoname(){t=definefunc(L##nameStr,&name,level);}

Token lastToken;

#define def0(name,nameStr,code) def00(lastToken,name,nameStr,code)
#define defAuto(nameStr,code) def0(autoname,nameStr,code)
#define def(name,code) def0(name,#name,code)


