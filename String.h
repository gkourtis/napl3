class String:public Aggregate{
	public:
	String(Token t):Aggregate(t){};
	String(obj o):Aggregate(o.w){};
	String():Aggregate(0){};
	//String(obj&& o):Aggregate(o.w){};
	Char* CharP(){return (Char*)Aggregate::dataP();}

	// the memory chunk returned bellow should be deleted after has been used.
	char* mbs(char* d=nullptr,Word sz=0) const;
	void lower();
	void upper();
};



