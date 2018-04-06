#ifndef FELIB_INCLUDE_DBGMSG_PROJECT_H_
#define FELIB_INCLUDE_DBGMSG_PROJECT_H_

/************************************************************
 * CUSTOM / PROJECT SPECIFIC, modify at will
 ************************************************************/
#ifndef DBGMSG_OBJ //do NOT define this in your project cpp files!

  #define DBGV2(v2,info)  DBGSS(dbgmsg_project::dbgV2(v2,info).str())
  #define DBGBLD(rb,info) DBGSS(dbgmsg_project::dbgBLD(rb,info).str())

  #ifdef DBGMSG_STRETCHREGION
    #define DBGSRI(info)  DBG1( dbgmsg_project::dbgSRI(rSR,info).str())
    #define DBGSR         DBGSRI("")
  #endif //DBGMSG_STRETCHREGION

  class dbgmsg_project{
    public:

    static std::stringstream dbgBLD(blitdata& rB,const char* strInfo){
      std::stringstream ss;
      ss<<strInfo<<"/";DBGLN;
      ss<<"Src="<<rB.Src.X<<","<<rB.Src.Y<<"/";DBGLN;
      ss<<"Dest="<<rB.Dest.X<<","<<rB.Dest.Y<<"/";DBGLN;
      ss<<"Border="<<rB.Border.X<<","<<rB.Border.Y<<"/";DBGLN;
      ss<<"Stretch="<<rB.Stretch<<"/";DBGLN;
      if(rB.Bitmap!=NULL)ss<<"BitmapSize="<<rB.Bitmap->GetSize().X<<","<<rB.Bitmap->GetSize().Y<<"/";DBGLN;
      return ss;
    }

    static std::stringstream dbgV2(v2 v2Val,const char* c){
      std::stringstream ss;
      ss<<c<<"/";DBGLN;
      ss<<"X="<<v2Val.X<<","<<v2Val.Y<<"/";DBGLN;
      return ss;
    }

    #ifdef DBGMSG_STRETCHREGION
    static std::stringstream dbgSRI(stretchRegion& SR,const char* strInfo){
      blitdata& rB=SR.B;
      std::stringstream ss;
      ss<<strInfo
        <<"["<<SR.iIndex<<"]SR@"
        <<"bForceXBRZ="<<SR.bUseXBRZ<<"/"
        <<"id="<<SR.strId<<"/"
        <<dbgBLD(rB,"").str()
      ;
      return ss;
    }
    #endif //DBGMSG_STRETCHREGION

  };

#endif //DBGMSG_GLOBAL

#endif /* FELIB_INCLUDE_DBGMSG_PROJECT_H_ */
