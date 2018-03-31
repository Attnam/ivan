#ifndef __DBGMSG_H__
#define __DBGMSG_H__

/* easy/non-cumbersome debug messages */

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define DBGFLN std::cerr<<__FILENAME__<<":"<<__LINE__<<":"

#ifdef DBGMSG
  #define DBG1(a) DBGFLN<<":("<<a<<")"<<std::endl;
  #define DBG2(a,b) DBGFLN<<"("<<a<<")("<<b<<")"<<std::endl;
  #define DBG3(a,b,c) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")"<<std::endl;
  #define DBG4(a,b,c,d) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")"<<std::endl;
  #define DBG5(a,b,c,d,e) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")("<<e<<")"<<std::endl;
  #define DBG6(a,b,c,d,e,f) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")("<<e<<")("<<f<<")"<<std::endl;
  #define DBG7(a,b,c,d,e,f,g) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")("<<e<<")("<<f<<")("<<g<<")"<<std::endl;
  #define DBG8(a,b,c,d,e,f,g,h) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")("<<e<<")("<<f<<")("<<g<<")("<<h<<")"<<std::endl;
  #define DBG9(a,b,c,d,e,f,g,h,i) DBGFLN<<"("<<a<<")("<<b<<")("<<c<<")("<<d<<")("<<e<<")("<<f<<")("<<g<<")("<<h<<")("<<i<<")"<<std::endl;
  #define DBGOK DBG2("Ok?",(bOk?"true":"false"))
#else
  #define DBG1
  #define DBG2
  #define DBG3
  #define DBG4
  #define DBG5
  #define DBG6
  #define DBG7
  #define DBG8
  #define DBG9
  #define DBGOK
#endif

#endif
