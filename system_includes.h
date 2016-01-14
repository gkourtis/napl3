#include <type_traits>
#include <iterator>
#include <locale>
#include <iostream>
#include <fstream>
#include <iomanip>

//#include <omp.h>
#include <thread>
#include <atomic>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/user.h>

#undef _GLIBCXX_USE_INT128


extern "C"{
	#include <editline/readline.h>
	#include <editline/history.h>
}

void __attribute__((constructor(101))) initLocale(){
	std::locale::global(std::locale("el_GR.utf-8"));
	std::ios_base::Init i;
}

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

extern "C"{
#include <microhttpd.h>
#define PORT 8888
#include <ncurses.h>
}


#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include <malloc.h>
#include <stdio.h>
#include <limits>
#include <stdlib.h>
#include <string.h> // memset 
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <vector>
#include "/usr/include/stdint.h"
//#include <cwchar>
//#include <ctype.h>
//#include <csignal>
//#include <cmath>
//#include <time.h>
//#include <vector>
//#include <unistd.h>
//#include <sys/param.h>
//#include <sys/time.h>
//#include </usr/include/x86_64-linux-gnu/gmp.h>
#include <limits>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
//#include <ext/vstring.h>
//#include <cctype>
//#include <uchar.h>
//#include <bits/codecvt.h>

//#include <bits/c++config.h>
