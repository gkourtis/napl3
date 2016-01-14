String oout(obj o){
	return o.isInt() 
		?	concatenateStrings(NAPL([0x),Int2String(o.w,8,16),NAPL(]))
		: toString(o,false);
}

bool noPrimitiveClass(obj Class){
	return Class!=classPrimitive && Class!=classClass;
}

void dictShow(bool showAll=false){
	std::stringstream ss;
	ss<<"\n\n";
	for(uWord i=0;i<dict.elements();i+=2){
		if(showAll || noPrimitiveClass(obj(dict[i+1]).Class())){
			ss.width(3);ss<<i/2<<": ";
			ss.setf (std::ios_base::left);
			ss<<std::setw(20)<<oout(dict[i])<<" "<<oout(dict[i+1])<<std::endl; 
		}
	}
	etP->write(ss);
}

using namespace std;

void showObjss(stringstream& ss,Obj o){
	bool f=o.isToken();Word wc=WordBits/4,wa=sizeof(addrT)*2+2;
	obj C=o.Class();
	ss<<hex<<setw(wc)<<o.w<<"|";
	String v=dict.nameOf(C);
	if(v!=classNotFound){
		ss<<setw(20)<<v<<" ";
		ss<<setw(4)<<hex<<uWord(t2i(C));
	}else
		ss<<setw(20)<<hex<<C.w<<"   ";
	ss<<" |";

	if(o.isToken()){
		vect& v=o.V();	
		ss<<(v.readOnly 			? " X |" : "   |");
		ss<<(v.reference 			? " X |" : "   |");
		ss<<(o.hasBody() 			? " X |" : "   |");
//		ss<<(v.excludeFirst 	? " X |" : "   |");
		ss<<(v.fields				 	? " X |" : "   |");
		ss<< setw(7)<<v.executionLevel<<"|";
		ss<< setw(wa)<<v.addr0<<" |";
	}else{
			int w=3+4+8+3+wa+2;char s[w+1];memset(s,'-',w);s[w]=0;s[0]=' ';s[w-1]=' ';
		ss<<"   |   |   |   |       |";
		ss<<setw(wa+2)<<"|";
	}
	ss<<" "<<toString(o);
	ss<<endl;
}

void ET::showStack(stringstream& ss){
	Word wc=sizeof(Word)*2,wa=sizeof(addrT)*2+2;
	ss<<endl;
	ss<<"    Token |";
	ss<<string(24,' ')<<"|";
	ss<<"R/O|REF|Bdy|fld|  ExL  |";
	ss<<setw(wa+2)<<"Addr    |";
	ss<<endl<<string(100,'-')<<endl;
	for(auto i:etP->stack()) 
		showObjss(ss,i);
}

void ET::showStack(){
	stringstream ss;showStack(ss);write(ss);
}

int ET::print(obj o){
	return write(toString(o).mbs());
}

void compilerConf(){
	std::stringstream s;
	s<<std::dec;
	s<<" sizeof(vect)="<<sizeof(vect);
	s<<" sizeof(addr)="<<sizeof(addrT);
	s<<" sizeof(Class)="<<sizeof(vect::Class);
	s<<" Compiled with:"<<compiler;
	#if __x86_64__ || __ppc64__
		s<<" in 64bit mode";
	#else
		s<<" in 32bit mode";
	#endif
	s<<std::endl;
	std::cout<<s.str();
}

