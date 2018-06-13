/**
 * !!!WARNING!!!
 *  This file cannot be included in other .h files TODO why?
 *  These macros cannot be used at inline methods declared in .h files TODO why?
 *  If you do this, it may SEGFAULT only when being run. Compiler and linker will warn nothing about the problem!
 */

#ifndef INCLUDE_DBGMSGPROJ_H_

  #define INCLUDE_DBGMSGPROJ_H_

  #ifdef DBGMSG

    #include "dbgmsg.h"

    /********************************************************************************************
     * CUSTOM / PROJECT SPECIFIC, modify at will
     ************************************************************/
    #ifndef DBGMSG_OBJ //do NOT define DBGMSG_OBJ this in your project cpp files!

      class dbgmsgproj { public:
        #if defined(__V2_H__) && !defined(DBGAV2)
          static std::stringstream dbgV2(v2 v2Val,const char* c){
            std::stringstream ss;
            ss<<c<<"/";
            ss<<"X,Y="<<((int)v2Val.X)<<","<<((int)v2Val.Y)<<"/";
            return ss;
          }
          #define DBGAV2(v2)  dbgmsgproj::dbgV2(v2,DBGTOSTR(v2)).str()
          #define DBGSV2(v2)  DBGSS(DBGAV2(v2))
        #endif

        #if defined(__BITMAP_H__) && !defined(DBGBLD)
          #include <sstream>
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
          #define DBGBLD(rb) DBGSS(dbgmsgproj::dbgBLD(rb,DBGTOSTR(rb)).str())
        #endif

        #if defined(__GRAPHICS_H__) && defined(DBGMSG_STRETCHREGION) && !defined(DBGSRI) //TODO if stretchRegion moves to graphics.h, DBGMSG_STRETCHREGION can be removed everywhere
          static std::stringstream dbgSRI(stretchRegion& SR,const char* strInfo){
            blitdata& rB=SR.B;
            std::stringstream ss;
            ss<<strInfo
              <<"["<<SR.iIndex<<"]SR@"
              <<"bEnabled="<<SR.bEnabled<<"/"
              <<"bForceXBRZ="<<SR.bUseXBRZ<<"/"
              <<"id="<<SR.strId<<"/"
              <<dbgmsgproj::dbgBLD(rB,"").str()
            ;
            return ss;
          }
          #define DBGSRI(info)  DBG1( dbgmsgproj::dbgSRI(rSR,info).str())
          #define DBGSR         DBGSRI("")
        #endif
      };

      #if defined(__CHAR_H__) && !defined(DBGCHAR)
        #define DBGCHAR(ch,info) DBG8(info, ch->CHAR_NAME(DEFINITE), ch->GetAssignedName().CStr(), ch, ch->GetID(), ch->IsPlayer(), ch->GetStack()->GetItems(), ch->GetEquipments() ) //, std::bitset<16>(ch->GetFlags()) )
      #endif

      #if defined(__ITEM_H__) && !defined(DBGITEM)
        #define DBGITEM(it,info) DBG3(info,it,(it==NULL?0:it->GetID()))
      #endif

      #if defined(__ERROR_H__) && !defined(DBGABORT)
        #define DBGABORT(x...) ABORT(x) //will only abort in development mode
      #endif

    #endif //DBGMSG_OBJ

  #else
    //dbgmsg macro erasers
    #define DBGOE
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

    // proj specifics
    #define DBGAV2(v2)
    #define DBGSV2(v2)

    #define DBGBLD(rb)
    #define DBGSRI(info)

    #define DBGSR

    #define DBGCHAR(ch,info)
    #define DBGITEM(it,info)
    #define DBGABORT(x...)

  #endif //DBGMSG

#endif //INCLUDE_DBGMSGPROJ_H_
