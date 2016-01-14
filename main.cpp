#include "includes.cpp"

int main(){

	std::cout<<"main started mmStart"<<&M<<std::endl;
	assert0(chdir("/home/gk/Dropbox/workspace01/napl22")==0,"unable to change dir");
	stopETs.store(false,0);
	ETs.reserve(0x100);
	compact(M);
#if true
	int fin,i;
	if(	(fin=open("naplStart0.napl",0,S_IRUSR))>=0){
		i=startET(fin,dup(fileno(stdout)));
	}
	while(!ETs[i].threadEnded){}
	if(ETs[i].joinable()) 
		ETs[i].join();
#endif
	HandleIncomingConnections(8000); 

		for(size_t i=0;i<ETs.size();i++){
			std::cout<<"ETs["<<i<<"] threadEnded="<<ETs[i].threadEnded<<" joinable="<<ETs[i].joinable()<<std::endl;
			if(ETs[i].joinable()) ETs[i].join();
		}

		for(size_t i=0;i<ETs.size();i++){
			std::cout<<"ETs["<<i<<"] threadEnded="<<ETs[i].threadEnded<<" joinable="<<ETs[i].joinable()<<std::endl;
		}

		return 0;
}

