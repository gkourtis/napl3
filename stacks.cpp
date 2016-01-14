void ET::newStack(Word executionLevel){
	stacks.push(newAggregate());
}
void ET::newString(Word executionLevel){
	stacks.push(::newString());
}

Aggregate ET::closeStack(){
	return stacks.pop();
}

void ET::openStack(){
	obj o=stack().pop();
	stacks.push(o);
}

void ET::oldStack(){
	obj o=closeStack();
	stack().push(o);
}

void ET::interStackMove(){
	stack().push( ((Aggregate)(stacks[~2])).pop() );
}

void ET::interStackDup(){
	stack().push( ((Aggregate)(stacks[~2])).tos() );
}

