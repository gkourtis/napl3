void ET::foreach(Aggregate a,Token action,bool inverseDirection){
	if (!inverseDirection)
		for(auto i=a.begin();i!=a.end();++i) {stack().push(*i);execute(action,true);}
	else
		for(auto i=a.end();i-->=a.begin();)		{stack().push(*i);execute(action,true);}
}

void ET::foreach2(Aggregate a,Token action,bool inverseDirection){
	if (!inverseDirection)
		for(auto i=a.begin();i<a.end();) 		{stack().push2(*i++,*i++);execute(action,true);}
	else
		for(auto i=a.end();i>=a.begin();)	{stack().push2(*--i,*--i);execute(action,true);}
}

// it takes a flag b and a number or aggragate Token.
// If it is a number then exits that level of secondaries.If it is a Token it exits up to that secondary.
void ET::exitSec(bool b,Token n_t){
	if(b) leaveN=n_t;
//exitLevel=exS.elements()-n_t*2;
}
