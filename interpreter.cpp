
void ET::statusString(std::ostream s){
	static int cnt=0;
	s<<std::setw(6)<<cnt++<<"exS:"<<exS.elements()<<" "<<"stacks:"<<stacks.elements()+1<<" "<<toString(stack())<<std::endl;
}


bool isHttpRequest(String s){
	obj o;
	if(dict.findIn(NAPL(HTTPREQUEST),false,false,false,&o)){
		AggregateOrdered a,r;a.push(0);
		etP->match(s,a,o,r);
		return r.elements()!=0;
	}else
		return false;
}

void ET::interpret(String cmd0){
	// ws indicates if the previous character was a whitespace
	bool ws=true,ms,b;obj C=0;tString cmd{cmd0};
	for(Char c:cmd){
		if(exitInterpretLoop)break;
		if(ws)newString(0); // create a new stack with execution level 0
		if(charCounter>0){
			stack().push(c);
			if((ws=(--charCounter==0))){Aggregate a=closeStack();for(Token i:a)stack().push(i);}
		}
		else
		 switch(c){
			case '\\': // one character escape character, the code is the same for u8 u16 u32 u64
				if(charLevel!=0) goto d;
				charCounter=1;ws=false;break;
			case u'«'	: // comment
			case u'¦'	: // alternating pattern string starting character
			case u'“'	: // sequential pattern string starting character 
			case '`'		:	// string starting character exists in all 

	ssc:		if(ws || charLevel>0 && c==delim ){
						delim=c;++charLevel;
					}
					if(!ws) goto d;
					ws=false;break;

			case u'»':ms = delim==u'«';C=classComment;goto sec; // sequential pattern string ending character exists only in u16 and above
			case  '|':ms = delim==u'¦';C=classPatternAny;goto sec;  // alternating pattern string ending character
			case u'”':ms = delim==u'“';C=classPatternAll;goto sec;  // alternating pattern string ending character
			case '\'':ms = delim==u'`';C=classString;goto sec; // string ending character
sec:		// a string end has been found without string start.
				if(!ms) goto d;
				if(--charLevel>0) goto d;
				// we have arrived to charLevel=0 closing the last ' so we have to collect the stack().	
				stack().Class(C);
				oldStack();
//				if(C==classComment) stack.drop();
				ws=true;break;
//			case L'"': // string starting and ending character
//				if(charLevel==0)goto ssc;else goto sec;
			default:
				if(c<0xFFF)
				if( (isspace(c)||iscntrl(c)) && charLevel<=0 && charCounter<=0 ) // if not continue into default
				{	b=stack().elements()>0;
					tString s{closeStack()};
					if(b) 
						execute(toObj(s));
					ws=true;
					break;
				}else{
d:				stack().push(c);ws=false;break;
				}
				else
					c=c+1;
		}
	}
//std::cout<<"interpret ended"<<std::endl;
}

void ET::interpretLoop(){	
	int cnt=0;
	while(!postClear(exitInterpretLoop)){
		//exS.print(L"exLvl: "+to_wstring(E.execution.l)+L":"+to_wstring(E.paren.l)+L" cnt:"+to_wstring(E.counter.l)+L" EX: "+to_wstring(exS.elements()));
		//stack.print(L"Stacks:"+to_wstring(stacks.elements())+L"  Stack:"+to_wstring(stack.elements())+L" ");
		//tracked(String,cmd,=NAPL( ));
		
		String s=getLine();
		
		if(s.elements()==0) 
			break;
		if(isHttpRequest(s))
			write("http request");
		interpret(s);
		std::stringstream ss;
			showStack(ss);
			ss<<std::dec<<"\n"<<std::setw(4)<<cnt++<<" E:"<<execution.l<<" P:"<<paren.l<<" C:"<<counter.l;
			ss<<" charL:"<<charLevel<<" charCounter:"<<charCounter;
			ss<<" exS:"<<exS.elements()<<" stacks:"<<stacks.elements()<<" \t"<<"n:"<<stack().elements()<<" "<<toString(stack())<<" || ";
			for(auto i:stack()) ss<<std::hex<<i<<" ";ss<<std::endl;
			//ss<<std::dec<<" freeMem="<<freeMem()<<" freeVectors="<<freeVectorsLU(freeObj)<<std::endl;
		write(ss);
	}
}


class getLine;