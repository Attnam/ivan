#ifndef INCLUDE_DBGMSGPROJ_H_
#define INCLUDE_DBGMSGPROJ_H_

// just copy this file freely anywhere or it's contents
#ifdef DBGMSG
  #include "dbgmsg.h"

  /************************************************************
   * CUSTOM / PROJECT SPECIFIC, modify at will
   ************************************************************/
  #ifndef DBGMSG_OBJ //do NOT define this in your project cpp files!

    #define DBGV2(v2)  DBGSS(dbgmsgprj::dbgV2(v2,DBGTOSTR(v2)).str())
    #define DBGBLD(rb) DBGSS(dbgmsgprj::dbgBLD(rb,DBGTOSTR(rb)).str())

//    #ifdef DBGMSG_STRETCHREGION
//      #define DBGSRI(info)  DBG1( dbgmsgprj::dbgSRI(rSR,info).str())
//      #define DBGSR         DBGSRI("")
//    #endif //DBGMSG_STRETCHREGION

    class dbgmsgprj{
      public:

      #ifdef DBGMSG_STRETCHREGION
      #define DBGMSG_BLITDATA
      #define DBGSRI(info)  DBG1( dbgmsgprj::dbgSRI(rSR,info).str())
      #define DBGSR         DBGSRI("")
      static std::stringstream dbgSRI(stretchRegion& SR,const char* strInfo){
        blitdata& rB=SR.B;
        std::stringstream ss;
        ss<<strInfo
          <<"["<<SR.iIndex<<"]SR@"
          <<"bEnabled="<<SR.bEnabled<<"/"
          <<"bForceXBRZ="<<SR.bUseXBRZ<<"/"
          <<"id="<<SR.strId<<"/"
          <<dbgBLD(rB,"").str()
        ;
        return ss;
      }
      #endif //DBGMSG_STRETCHREGION

      #ifdef DBGMSG_BLITDATA
      #define DBGMSG_V2
      #define DBGBLD(rb) DBGSS(dbgmsgprj::dbgBLD(rb,DBGTOSTR(rb)).str())
      static std::stringstream dbgBLD(const blitdata& rB,const char* strInfo){
        std::stringstream ss;
        ss<<strInfo<<"/";
        ss<<"Src="<<rB.Src.X<<","<<rB.Src.Y<<"/";
        ss<<"Dest="<<rB.Dest.X<<","<<rB.Dest.Y<<"/";
        ss<<"Border="<<rB.Border.X<<","<<rB.Border.Y<<"/";
        ss<<"Stretch="<<rB.Stretch<<"/";
        if(rB.Bitmap!=NULL)ss<<"BitmapSize="<<rB.Bitmap->GetSize().X<<","<<rB.Bitmap->GetSize().Y<<"/";
        return ss;
      }
      #endif //DBGMSG_BLITDATA

      #ifdef DBGMSG_V2
      #define DBGV2(v2)  DBGSS(dbgmsgprj::dbgV2(v2,DBGTOSTR(v2)).str())
      static std::stringstream dbgV2(v2 v2Val,const char* c){
        std::stringstream ss;
        ss<<c<<"/";
        ss<<"X="<<v2Val.X<<","<<v2Val.Y<<"/";
        return ss;
      }
      #endif //DBGMSG_V2

    };

  #endif //DBGMSG_OBJ

#else

  #define DBGSS(s)
  #define DBG1(a)
  #define DBG2(a,b)
  #define DBG3(a,b,c)
  #define DBG4(a,b,c,d)
  #define DBG5(a,b,c,d,e)
  #define DBG6(a,b,c,d,e,f)
  #define DBG7(a,b,c,d,e,f,g)
  #define DBG8(a,b,c,d,e,f,g,h)
  #define DBG9(a,b,c,d,e,f,g,h,i)
  #define DBGLN
  #define DBGSTK
  #define DBGTOSTR_(str)
  #define DBGTOSTR(str)
  #define DBGB(B)
  #define DBGSB(B)
  #define DBGI(I)
  #define DBGSI(I)
  #define DBGF(F)
  #define DBGSF(F)
  #define DBGC(C)
  #define DBGSC(C)

  // project specific
  #define DBGV2(v2)
  #define DBGBLD(rb)

  #define DBGSRI(info)
  #define DBGSR

#endif //DBGMSG

#endif //INCLUDE_DBGMSGPROJ_H_
