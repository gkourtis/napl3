
const addrT vvSize=sizeof(vect)*vvElements;
const addrT meanObjSize=0x40; // the mean size of objects in agu is supposed 0x40
const addrT mmSize=sizeof(agu)*vvElements*meanObjSize;
//const size_t heapSize=mmSize+vvSize;

// addrT is unsigned. The starting address is calculated from M.vv-(maxAddrT+1)

__attribute__((aligned(PAGE_SIZE)))struct heap{
	agu mmStart[mmSize];
	ptr mm(){ return (ptr)&mmStart[mmSize]-std::numeric_limits<addrT>::max()-1;};
	vect vv[vvElements];
	void* vptr(addrT q){return (ptr)mm()+q;}
	
} M,M1;

//agu* mm=&M.mmStart[mmSize]-std::numeric_limits<addrT>::max()-1;

/*
void  __attribute__((constructor)) check_mm_vv(){
	if(addrT(mm-(agu*)M.vv)){
		std::cout<<" mmStart+sizeof(mmStart)="<<&mm<<" vv="<<&M.vv<<"\n";
		stop();
	}
}
*/
