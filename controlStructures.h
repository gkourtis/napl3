void foreach(Aggregate a,Token action,bool inverseDirection=false);
void foreach2(Aggregate a,Token action,bool inverseDirection=false);

// it takes a flag b and a number or aggragate Token.
// If it is a number then exits that level of secondaries.If it is a Token it exits up to that secondary.
void exitSec(bool b,Token n_t);

