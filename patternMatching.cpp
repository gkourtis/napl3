enum patternType{all,any};

Aggregate ET::allPos(Aggregate s,Word startPos){
	uWord sn=s.elements();Aggregate r(sn,sn);
	for(uWord i=startPos;i<s.elements();i++) r[i]=i;
	return r;
}

AggregateOrdered ET::matchEnd(Aggregate s,AggregateOrdered positions){
	AggregateOrdered r;
	if(positions.keyExists(s.elements(),false))
		r.push(s.elements());
	return r;
}

 AggregateOrdered ET::match(const Aggregate s,AggregateOrdered positions, Obj pat,AggregateOrdered r){

	SWITCH(pat.Class())
		CASE(classInt)
			if(!r) 
				r=AggregateOrdered();
			uWord sn=s.elements();
			for(auto p:positions){
				if(0<=p && p+1<=sn && s[p]==pat.w) r.keyExists((Obj)(p+1));
			}
		CASE(classAggregate)
			stack().push(positions);
			execute(pat,true);
			r=stack().pop();
		CASE(classString,classPatternAll)
			AggregateOrdered r1=positions;
			for(auto pat1:(Aggregate)pat)
				r1=match(s,r1,pat1);
			if(!r) r=r1;else r+=r1;
		CASE(classPatternAny)
			if(!r) 
				r=AggregateOrdered();
			for(auto pat1:(Aggregate)pat)
				match(s,positions,pat1,r);
		DEFAULT
			assert0(false,"unexpected in matchAll");
	endSWITCH
	return r;
}

AggregateOrdered ET::matchFor(Aggregate s,AggregateOrdered positions,Obj pat,uWord n1,uWord n2){
	AggregateOrdered r=positions;
	for(uWord i=n1;i--!=0;){
		r=match(s,r,pat);
		if(r.elements()==0) return r;
	}
	AggregateOrdered r1=r;
	if(n2>=n1)
		for(uWord i=n2-n1;i--!=0;){
			r1=match(s,r1,pat);
			if(r1.elements()==0) return r;
			r+=r1;
		}
	return r;
}
	
void ET::matchFor_(uWord n1,uWord n2){
	Obj pat=stack().pop(),positions=stack().pop();
	stack().push(matchFor((AggregateOrdered)stack()[~1],positions,pat,n1,n2));
}

Aggregate ET::anything(Aggregate s,AggregateOrdered positions){
	Word n=positions.elements();
	return n==0 ? allPos(s,n) : allPos(s,positions[0]);
}


