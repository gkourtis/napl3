// An aggregate with ordered unique key
/*
bool AggregateOrdered::findIn(
	tObj key,
	bool insert,bool replace,bool asUndefined,
	obj *oP,uWord *iP,
	Word cols,Word keyCol,Word valueCol) const
{ 
// Searches for a key 
// if insert is true, and the key isn't found, it is adeed 
// if the key is found and replace is true, the value is replaced with the given value
// If both insert is true and replace is true, both actions happen
// If the asUndefined flag is true, the value to assign when a value must be set, is 
// a newly created object of class classUndefined.
// If the asUndefined flag is false, the value to assign when a value must be set, is the object pointed by oP.
// If the key is found or inserted, the position where the group of key,value,etc line was found is returned via the iP pointer.
// If not found false is returned and *iP indicates a position greater than the searched value.
// *iP is returned as an index in the array
	uWord i;bool b;
	assert0(0<=keyCol && keyCol<cols,"col out of range in AggregateOrdered::findIn");
	assert0(elements()%cols==0,"The elements divided cols not exact AggregateOrdered::findIn");
	int i1,i2,r,raws=elements()/cols;
	for(i1=0,i2=raws;i=(i1+i2)/2,i1!=i2; ){
		r=compare(key,(Aggregate)(*this)[i*cols+keyCol]);
		if(r>0) i1=i+1;
		else if(r<0) i2=i; 
		else{
			if(replace) goto l;
			if(oP) *oP=(*this)[i*cols+valueCol]; b=true;goto e;
		}
		// i1 includes i2 excludes.In case t>a[] then i1=i+1 to exclude if t<a[] then it is sufficient to set i2=i;
	}
	if(insert){
		ins_backdel(i*cols,cols);
		(*this)[i*cols+keyCol]=key.w;
l:		tobj o1(0); if(asUndefined){ o1=newUndefined();if(oP) *oP=o1;} else if(oP) o1=*oP;
		if(cols>=2)(*this)[i*cols+valueCol]=o1.w;
		b=true;
	}else b=false;
e:	if(iP) 
		*iP=i*cols;
	return b;
}
*/
bool AggregateOrdered::findIn_(
	Word keysNum,Obj keys[],
	bool insert,bool replace,bool asUndefined,
	obj *oP,uWord *iP,
	Word cols,Word keyCol,Word valueCol) const
{ 
// Searches for a key 
// if insert is true, and the key isn't found, it is adeed 
// if the key is found and replace is true, the value is replaced with the given value
// If both insert is true and replace is true, both actions happen
// If the asUndefined flag is true, the value to assign when a value must be set, is 
// a newly created object of class classUndefined.
// If the asUndefined flag is false, the value to assign when a value must be set, is the object pointed by oP.
// If the key is found or inserted, the position where the group of key,value,etc line was found is returned via the iP pointer.
// If not found false is returned and *iP indicates a position greater than the searched value.
// *iP is returned as an index in the array
	uWord i;bool b;
	assert0(0<=keyCol && keyCol<cols,"col out of range in AggregateOrdered::findIn");
	assert0(elements()%cols==0,"The elements divided cols not exact AggregateOrdered::findIn");
	int i1,i2,r=-1,raws=elements()/cols;
	for(i1=0,i2=raws;i=(i1+i2)/2,i1!=i2; ){
		for(Word k=0;k!=keysNum;k++){	
			r=compare(keys[k],(Aggregate)(*this)[i*cols+keyCol+k]);
			if(r>0){i1=i+1;break;}
			if(r<0){i2=i;break;}
		}
		if(r==0){
			if(replace) goto l;
			if(oP) *oP=(*this)[i*cols+valueCol]; b=true;goto e;
		}
	}
		// i1 includes i2 excludes.In case t>a[] then i1=i+1 to exclude if t<a[] then it is sufficient to set i2=i;

	if(insert){
		ins_backdel(i*cols,cols);
		for(Word k=0;k!=keysNum;k++)	
			(*this)[i*cols+keyCol+k]=keys[k];
l:		obj o1; 
			if(asUndefined)
				{ o1=newUndefined();if(oP) *oP=o1;} 
			else 
				if(oP) o1.w=(*oP).w;
		if(cols>keysNum)(*this)[i*cols+valueCol]=o1.w;
		b=true;
	}else b=false;
e:	if(iP) 
		*iP=i*cols;
	return b;
}

bool AggregateOrdered::findIn(
	Obj key,
	bool insert,bool replace,bool asUndefined,
	obj *oP,uWord *iP,
	Word cols,Word keyCol,Word valueCol) const
	{	Obj keys[]={key};
		return findIn_(1,keys,insert,replace,asUndefined,oP,iP,cols,keyCol,valueCol);
	}

bool AggregateOrdered::keyExists(Obj key,bool insert) const{
	return findIn(key,insert,false,false,nullptr,nullptr,1,0,0);
}

/*
bool AggregateOrdered::keysExists(Obj key1,Obj key2,bool insert) const{
	Obj keys[]={key1,key2};
	return findIn_(countof(keys),keys,insert,false,false,nullptr,nullptr,1,0,0);
}
*/

bool AggregateOrdered::revFindIn(Obj value,obj& name,uWord &i,Word cols,Word keyCol,Word valueCol) const{
	for(i=0;i<elements();i+=cols)
		if((*this)[i+valueCol]==value.w){name=(*this)[i+keyCol];return true;}
	return false;
}

obj AggregateOrdered::nameOf(Obj value) const{
	obj o{classNotFound};uWord i;
	revFindIn(value,o,i);
	return o;
}

obj AggregateOrdered::valueOf(Obj key) const{
	obj o{classNotFound};
	findIn(key,true,false,true,&o);
	return o;
}

void AggregateOrdered::define(Obj key,Obj o0) const{
// define assigns for the first time a value to a non existent name		
	assert0(findIn(key,true,false,false,(obj*)&o0),"Problem In define");
}

__attribute__((init_priority(103))) AggregateOrdered dict;
//__attribute((constructor)) void dictKeepAlive(){dict.keepAlive(true);}

// Aggregate comparison is defined as the sequential comparison of each constituent.
// Elementary types are equal if their values match.

//bool operator==(const obj o1,const obj o2){return true;};
String Int2String(Int n,Int digits=1,Int base=10,bool sign=false);

String keyOf(Obj o,String prefix){
	obj key{classNotFound};uWord i;
	if(dict.revFindIn(o,key,i)) 
		return key;
	else 
		return concatenateStrings(prefix,Int2String(o.w,8,16));
}

//AggregateOrdered::AggregateOrdered(Aggregate a){for(auto i:a) keyExists(i);}

void AggregateOrdered::operator+=(Aggregate a){
	for(auto i:a) 
		keyExists(i);
}


