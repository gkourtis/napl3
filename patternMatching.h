enum patternType{all,any};

Aggregate allPos(Aggregate s,Word startPos=0);
Aggregate anything(Aggregate s,AggregateOrdered positions);
AggregateOrdered matchEnd(Aggregate s,AggregateOrdered positions);
AggregateOrdered match(const Aggregate s,AggregateOrdered positions, Obj pat,AggregateOrdered r=0);
AggregateOrdered matchFor(Aggregate s,AggregateOrdered positions,Obj pat,uWord n1,uWord n2);
void matchFor_(uWord n1,uWord n2);
