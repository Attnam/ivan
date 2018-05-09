#ifndef INCLUDE_DBGMSGPROJ_H_
#define INCLUDE_DBGMSGPROJ_H_

// just copy this file freely anywhere or it's contents
#ifdef DBGMSG
  #include "dbgmsg.h"

  /************************************************************
   * CUSTOM / PROJECT SPECIFIC, modify at will
   ************************************************************/
  #ifndef DBGMSG_OBJ //do NOT define this in your project cpp files!
    #include <bitset>

    #define DBGCHAR(ch,info) DBG8(info, ch->CHAR_NAME(DEFINITE), ch->GetAssignedName().CStr(), ch, ch->GetID(), ch->IsPlayer(), ch->GetStack()->GetItems(), ch->GetEquipments() ) //, std::bitset<16>(ch->GetFlags()) )
    #define DBGITEM(it,info) DBG3(info,it,(it==NULL?0:it->GetID()))
    #define DBGABORT(x...) ABORT(x) //will only abort in development mode

    class dbgmsgprj{
      public:

      #ifdef DBGMSG_STRETCHREGION
      #define DBGMSG_BLITDATA // alows the dependency
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
      #define DBGMSG_V2 // alows the dependency
      #define DBGBLD(rb) DBGSS(dbgmsgprj::dbgBLD(rb,DBGTOSTR(rb)).str())
      static std::stringstream dbgBLD(const blitdata& rB,const char* strInfo){
        std::stringstream ss;
        ss<<strInfo<<"/";
        ss<<"Src="<<rB.Src.X<<","<<rB.Src.Y<<"/";
        ss<<"Dest="<<rB.Dest.X<<","<<rB.Dest.Y<<"/";
        ss<<"Border="<<rB.Border.X<<","<<rB.Border.Y<<"/";
        ss<<"Stretch="<<rB.Stretch<<"/";
//        DBG2("rB.Bitmap",rB.Bitmap); // some(random)times this causes SEGFAULT: rB.Bitmap->GetSize()
        if(rB.Bitmap!=NULL)ss<<"BitmapSize="<<rB.Bitmap->GetSize().X<<","<<rB.Bitmap->GetSize().Y<<"/";
        return ss;
      }
      #endif //DBGMSG_BLITDATA

      #ifdef DBGMSG_V2
      #define DBGAV2(v2)  dbgmsgprj::dbgV2(v2,DBGTOSTR(v2)).str()
      #define DBGSV2(v2)  DBGSS(DBGAV2(v2))
      static std::stringstream dbgV2(v2 v2Val,const char* c){
        std::stringstream ss;
        ss<<c<<"/";
        ss<<"X,Y="<<((int)v2Val.X)<<","<<((int)v2Val.Y)<<"/";
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
  #define DBGS(SS)
  #define DBGEXEC(cmds)

  // project specific
  #define DBGAV2(v2)
  #define DBGSV2(v2)
  #define DBGBLD(rb)
  #define DBGCHAR(ch,info)
  #define DBGITEM(it,info)
  #define DBGSRI(info)
  #define DBGSR
  #define DBGABORT(x...)

#endif //DBGMSG

#endif //INCLUDE_DBGMSGPROJ_H_
