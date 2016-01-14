// The locic besides garbage collection is the subsequent:
// Each aggregate, should point to at least 2 elements.
// If an aggregate contains only 1 pointed element than the operation is optimized
// and no stack frame is generated.
// The above thoughts, tell us that the necessary stack depth for tracking the 
// garbage collection position is log² vectorsNum.
// So if that space is preallocated, it will never serve a bigger stack to track the 
// garbage collection process.

bool isSize(size_t sz);
void compact(heap&),garbageCollect(heap&);
void setAlive(Obj d);
size_t freeVectors(std::atomic<obj>& freeo,bool setAlive=false);

Aggregate persistents;
obj persistent(obj o){persistents.push(o);return o;}
