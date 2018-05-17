/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

/* Compiled through materset.cpp */

#define DBGMSG_BLITDATA
#include "dbgmsgproj.h"

int iDrawTot=3;
int iCleanTot=2; //min 1

hiteffect::hiteffect() : entity(HAS_BE), Next(NULL), iState(-1), bBlitdataWasSet(false) {DBGSTK;} //iState -1 indicates this constructor

square* hiteffect::GetSquareUnderEntity(int) const {
  return setup.LSquareUnder;
}

#define ITGSTATE_INIT    1 //in order always
#define ITGSTATE_SETBLD  2
#define ITGSTATE_SENDTOHELL 2 //must be the state value before cleaup bypassing previous
#define ITGSTATE_CLEANUP 3
#define ITGSTATE_DESTROY 4

#define DBGHITEFFINFO \
    DBG1(iState); \
    DBG6("pointers",this,setup.WhoHits,setup.WhoIsHit,bmpHitEffect,setup.itemEffectReference); \
    DBG5(setup.Type,bWhoIsHitDied,iDrawCount,bBlitdataWasSet,DBGAV2(v2HitFromSqrPos)); \
    DBGSV2(v2HitToSqrPos); \
    DBGEXEC(if(setup.WhoHits ->Exists())DBG1(setup.WhoHits ->GetName(DEFINITE).CStr())); \
    DBGEXEC(if(setup.WhoIsHit->Exists())DBG1(setup.WhoIsHit->GetName(DEFINITE).CStr())); \
    DBGEXEC(if(setup.itemEffectReference->Exists())DBGSC(setup.itemEffectReference->GetName(DEFINITE).CStr())); \
    DBGSV2(bmpHitEffect->GetSize()); \
    DBGBLD(bldFinalDraw); \
    DBGSTK;

hiteffect::hiteffect(hiteffectSetup s)
: entity(HAS_BE), Next(NULL), iDrawCount(0), iState(0), bBlitdataWasSet(false), lStartTime(clock())
{
  chkCleanupAlready();

  static short int iHigh=0xFF*0.85;
  static col24 lumHigh=MakeRGB24(iHigh,iHigh,iHigh);

  static short int iL  =0xFF*0.15; //dark works better with already light graphics like arcanite or bone stuff
  static short int iIsh=0xFF*0.90;
  static col24 lumReddish =MakeRGB24(iIsh,iL,iL);
  static col24 lumGreenish=MakeRGB24(iL,iIsh,iL);
  static col24 lumBluish  =MakeRGB24(iL,iL,iIsh);
  static col24 lumYellowish =MakeRGB24(iIsh,iIsh,iL);

  static blitdata bldLum = [](){bldLum = DEFAULT_BLITDATA;
    bldLum.Bitmap = new bitmap(TILE_V2);
    bldLum.Border = TILE_V2; return bldLum;}();
  bldLum.Src=v2(); //reset
  bldLum.Bitmap->ClearToColor(TRANSPARENT_COLOR); //reset

  setup=s; DBG4(this,s.WhoHits,s.WhoIsHit,"WhoHits");DBG2(s.WhoHits->GetName(DEFINITE).CStr(),s.WhoIsHit->GetName(DEFINITE).CStr());

  bldFinalDraw=DEFAULT_BLITDATA; DBGLN;

  v2CamPos = game::GetCamera();

  v2HitFromSqrPos=s.WhoHits->GetPos();  DBGSV2(v2HitFromSqrPos);
  v2HitToSqrPos=s.WhoIsHit->GetPos();  DBGSV2(v2HitToSqrPos);
  v2HitFromToSqrDiff = v2HitFromSqrPos-v2HitToSqrPos; DBGSV2(v2HitFromToSqrDiff);
  if(v2HitFromToSqrDiff.X!=0 && v2HitFromToSqrDiff.Y!=0){ DBGLN; //diagonal
    v2 v2FromXY(Min(v2HitFromSqrPos.X,v2HitToSqrPos.X),Min(v2HitFromSqrPos.Y,v2HitToSqrPos.Y)); DBGSV2(v2FromXY);
    v2 v2ToXY(Max(v2HitFromSqrPos.X,v2HitToSqrPos.X),Max(v2HitFromSqrPos.Y,v2HitToSqrPos.Y)); DBGSV2(v2ToXY);
    for(int iY=v2FromXY.Y;iY<=v2ToXY.Y;iY++){
      for(int iX=v2FromXY.X;iX<=v2ToXY.X;iX++){
        vExtraSquares.push_back(s.WhoHits->GetLevel()->GetLSquare(iX,iY)); DBG3("vExtraSquares:add",iX,iY);
      }
    }
  }

  bWhoIsHitDied=s.WhoIsHit->IsDead(); DBGLN;

  LSquareUnderOfWhoHits=s.WhoHits->GetLSquareUnder(); DBGLN;

  switch(setup.iMode){
  case 1: //immersive
    bldLum.Luminance = NORMAL_LUMINANCE;
    break;
  case 2: //indicator
    bldLum.Luminance = lumHigh;
    break;
  case 3: //colored indicators where possible
    if(setup.WhoIsHit->IsPlayer() || setup.WhoIsHit->IsPet()){
      bldLum.Luminance = lumReddish;
    }else
    if(setup.WhoHits->IsPlayer()){
      bldLum.Luminance = lumBluish;
    }else
    if(setup.WhoHits->IsPet()){
      bldLum.Luminance = lumGreenish;
    }else{
      bldLum.Luminance = lumHigh;
    }
    break;
  case 4: //dynamic colored indicators or based on usefulness of the color indicator
    if(setup.WhoIsHit->IsPlayer() || setup.WhoHits->IsPlayer()){ //player being hit or hitting is w/o doubts the origin of the attack
      if(setup.Critical){
        bldLum.Luminance = lumReddish;
      }else{
        bldLum.Luminance = NORMAL_LUMINANCE; // do not highlight non critical strikes
      }
    }else
    if(setup.WhoIsHit->IsPet()){
      if(setup.Critical){
        bldLum.Luminance = lumReddish;
      }else{
        bldLum.Luminance = lumYellowish; // just to call player attention
      }
    }else
    if(setup.WhoHits->IsPet()){
      if(setup.itemEffectReference->IsWeapon(setup.WhoHits)){ //TODO why char as param?
        bldLum.Luminance = lumBluish;
      }else{
        bldLum.Luminance = lumGreenish;
      }
    }else{
      bldLum.Luminance = NORMAL_LUMINANCE; // unrelated to the player's team have no highlighting at all
    }

//    if(bldLum.Luminance == NORMAL_LUMINANCE){
//      if(dynamic_cast<bodypart*>(setup.itemEffectReference)!=NULL){
//        bldLum.Luminance = lumHigh;
//      }
//    }

    break;
  default: ABORT("unsupported hiteffect mode %d",setup.iMode);
  }

  int iDegrees=0;
  int iTipAt=2; //2 is default of most items TODO a simple database specifying where is the tip of each item gfx: 0 2 7 or 5
  bool bAllowDirRotate=true;
  bool bTypePrepared=false;
  switch(setup.Type) //TODO use arm, feet, head material colors
  {
   case WEAPON_ATTACK:
    if(!bTypePrepared)bTypePrepared=true;

    //TODO make it sure the item will be drawn pointing at the same direction found at it's graphics file
    setup.itemEffectReference->Draw(bldLum); //the item* cant be stored as it may break and be sent to hell after here...
    break;

   /**
    * These 3 specific graphics below are collected directly from the raw bitmap.
    */

   case UNARMED_ATTACK: // The punch used graphics looks messy if rotated at any direction as it has no direction (it points nowhere in the 2D, it is upwards in 3D).
    if(!bTypePrepared){bTypePrepared=true;
      bldLum.Src.X = 16;
    }
    /* no break */

   case KICK_ATTACK: // the used graphics points at dir 6 (down)
    if(!bTypePrepared){bTypePrepared=true;
      bldLum.Src.X = 32;
      switch(setup.GivenDir){
        case 0:case 1:case 2: iDegrees=180; break;
        case 3:               iDegrees= 90; break;
        case 4:               iDegrees=270; break;
        // ignored 5 (6) 7
      }
    }
    /* no break */

   case BITE_ATTACK: // the used graphics points at dir 4 (right)
    if(!bTypePrepared){bTypePrepared=true;
      switch(setup.GivenDir){
        case 0:case 1:case 2: iDegrees=270; break;
        case 3:               iDegrees=180; break;
        // ignored (4)
        case 5:case 6:case 7: iDegrees= 90; break;
      }
    }

    bldLum.Src.Y = 112;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(bldLum);

    bAllowDirRotate=false;

    break;

   default: DBGABORT("unsupported type mode %d",setup.Type);
  }

  /**
   * GivenDir, see doc MirrorFlipRotateToDegrees.jpg
   * 012
   * 3 4
   * 567
   */
  static blitdata bldRotate = [](){bldRotate = DEFAULT_BLITDATA;bldRotate.Border = TILE_V2;return bldRotate;}();
  bldRotate.Bitmap=NULL; //reset

  // reset static blitdata dir, grant it is drawn pointing to default top right
  bitmap::ResetBlitdataRotation(bldRotate);

  DBGEXEC(if(setup.WhoHits->IsPlayer())DBG3(DBGI(setup.Type),DBGI(setup.GivenDir),DBGC(setup.itemEffectReference->GetName(DEFINITE).CStr())));
  // there is no horizontal or vertical easy rotations implemented yet TODO but ex.: spears would not fit in 16x16 w/o shrinking it...
  if(bAllowDirRotate){
    int iDir = setup.GivenDir;

    if(iDir==1)iDir = clock()%2==0 ? 0 : 2;
    if(iDir==3)iDir = clock()%2==0 ? 0 : 5;
    if(iDir==4)iDir = clock()%2==0 ? 2 : 7;
    if(iDir==6)iDir = clock()%2==0 ? 5 : 7;

    switch(iDir){
      case 0: iDegrees=270; break;
      case 2: iDegrees=  0; break; //default at top right
      case 5: iDegrees=180; break;
      case 7: iDegrees= 90; break;
      default: DBGABORT("unsupported iDir %d",iDir);
    }

    // adjusts the requested rotation based on the tip default rotation
    switch(iTipAt){
      case 2:break; //default top right
      case 7:iDegrees+=-90;break; //lower right
      case 5:iDegrees+=180;break; //lower left
      case 0:iDegrees+= 90;break; //upper left
      default: DBGABORT("unsupported iTipAt %d",iTipAt); //should have been converted to one of these 4
    }

    // fix overlaps
    while(iDegrees>=360)iDegrees-=360;
    while(iDegrees<   0)iDegrees+=360;
  }

  switch(iDegrees){
    case 270: bldRotate.Flags |= ROTATE|FLIP|MIRROR; break;
    case 180: bldRotate.Flags |=        FLIP|MIRROR; break;
    case  90: bldRotate.Flags |= ROTATE;             break;
    case   0: break; //default
    default: DBGABORT("unsupported iDegrees %d",iDegrees);
  }

  DBG7(iDegrees, bAllowDirRotate,setup.GivenDir, bTypePrepared,bldRotate.Flags&ROTATE,bldRotate.Flags&FLIP,bldRotate.Flags&MIRROR);
  (bmpHitEffect = bldRotate.Bitmap = new bitmap(TILE_V2))->ClearToColor(TRANSPARENT_COLOR); DBGLN; //TODO is clear unnecessary?
  bldLum.Bitmap->NormalBlit(bldRotate); DBGLN; //this "rotates"

  SetIntegrityState(ITGSTATE_INIT);
}

void hiteffect::SetIntegrityState(int iNewState){ DBG1(iNewState);
  chkCleanupAlready();

  if(iState != iNewState-1){
    DBGHITEFFINFO;
    if(iNewState!=ITGSTATE_CLEANUP) // cleanup is the end of it, no problem.
      DBGABORT("Wrong state usage(%d), must always be equal to current(%d)+1",iNewState,iState);
  }

  iState=iNewState;
}

hiteffect::~hiteffect()
{
  if(bmpHitEffect!=NULL)delete bmpHitEffect;
  iState=ITGSTATE_DESTROY; //not safe to  use this here: SetIntegrityState(ITGSTATE_DESTROY); as may have debug messages...
}

void hiteffect::Be()
{
  chkCleanupAlready();

  bool bHell=false;
  bool bClean=false;

  if(iDrawCount==iDrawTot)
    iCleanCount++;

  if(iCleanCount>0)
    bClean=true;

  if(bClean){
    //TODO is post clean (after drawing ends) unnecessary now that drawing is denied when camera changes pos?
    //TODO is post clean working at all?
    if(bBlitdataWasSet){
      CheckIntegrity(ITGSTATE_SETBLD); //before hell
      setup.LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible
    }
  }

  if(iDrawCount==iDrawTot && iCleanCount==iCleanTot)
    bHell=true;

  if(bHell){
    setup.LSquareUnder->RemoveHitEffect(this);
    iState=ITGSTATE_SENDTOHELL;
    cleanup();
    SendToHell();
  }

}

void hiteffect::chkCleanupAlready() const
{
  if(iState==ITGSTATE_CLEANUP)DBGABORT("hiteffect was cleanup already!");
}

void hiteffect::cleanup(){
  chkCleanupAlready();

  SetIntegrityState(ITGSTATE_CLEANUP); //before the actual cleanup as pointers are still set

  // TODO necessary?
  setup.LSquareUnder=NULL;
  setup.WhoHits=NULL;
  setup.WhoIsHit=NULL;
  setup.itemEffectReference=NULL;

  //not external reference, must be deleted at destructor: bmpHitEffect=NULL;

  LSquareUnderOfWhoHits=NULL;
  vExtraSquares.clear(); //TODO move to destructor? or is automatic?
}

void hiteffect::PrepareBlitdata(const blitdata& bld){
  chkCleanupAlready();

  bldFinalDraw=bld; DBG1(this);DBGBLD(bldFinalDraw); //copy
  bitmap::ResetBlitdataRotation(bldFinalDraw); //prevent it from further rotating beyong the initial setup at constructor!!!
  bldFinalDraw.Stretch=0; //TODO why so big numbers here? was it uninitialized?
  bBlitdataWasSet=true;

  if(iState!=ITGSTATE_SETBLD){ //can be set more than once
    CheckIntegrity(ITGSTATE_INIT); //must be only  this one here
    SetIntegrityState(ITGSTATE_SETBLD);
  }
}

void hiteffect::End(){
  chkCleanupAlready();

  iDrawCount=iDrawTot;
}

truth hiteffect::DrawStep()
{
  chkCleanupAlready();

  if(iDrawCount==iDrawTot)return false;

  bool bDraw = true;
  bool bAnimate = true;

  if(ivanconfig::IsHideWeirdHitAnimationsThatLookLikeMiss()){
    //showing all animations helps on understanding there happened a hit, even if it looks like a miss or weird (kills before hitting) :(
    if(bAnimate && bWhoIsHitDied)bAnimate=false;
    if(bAnimate && !setup.WhoIsHit->Exists())bAnimate=false; //TODO is this crash safe?
    if(bDraw && bAnimate && setup.WhoIsHit->GetPos()!=v2HitToSqrPos){
      /*
       * TODO if the hit target moves, the effect would play flying to it's last location (where it was actually)
       *      and will look like a miss (what is wrong), so I tried setting it to instantly draw there,
       *      but still looks like a miss, so I just disabled the effect when that happens to not look confusing :(.
       */
      End(); //this will end the drawing to prevent it looking like a missed hit :(
      bDraw=false;

      bAnimate=false;
    }
  }

  static long lTimeoutDelay = CLOCKS_PER_SEC*3.0; //prevent effects older than 3 seconds from continue playing, the glitch is most visible during wizard autoplay mode
  if( clock() > (lStartTime+lTimeoutDelay) ){
    End();
    bDraw=false;
  }

  //TODO use rotation?
  if(bAnimate){ //just to not fly if it died already
    //place at who hits to fly from it to who is hit
    bldFinalDraw.Dest += v2HitFromToSqrDiff*TILE_SIZE;

    //displacement, interpolate TODO could be at v2 class with To and fPercent!
    bldFinalDraw.Dest += -(v2HitFromToSqrDiff*TILE_SIZE)*(iDrawCount/(float)iDrawTot);
  }

  if(game::GetCamera()!=v2CamPos){
    /**
     * As the hiteffect will draw LATER,
     * this means, if the camera moves, the blitdata will command to still draw relatively to where the camera WAS
     * and not where the camera IS!!
     * TODO improve this by updating the blitdata to the new camPos?
     */
    End();
    bDraw=false;
  }

  if(bDraw){
    Draw();
    iDrawCount++; // AFTER drawing!

    //clear all hit effects as soon as possible
    setup.LSquareUnder->SendStrongNewDrawRequest();
    LSquareUnderOfWhoHits->SendStrongNewDrawRequest();
    for(int i=0;i<vExtraSquares.size();i++){
      vExtraSquares[i]->SendStrongNewDrawRequest(); DBGSV2(vExtraSquares[i]->GetPos());
    }
  }

  return true; //did draw now
}

truth hiteffect::CheckIntegrity(int iItgState) const{
  chkCleanupAlready();

  if(
      bldFinalDraw.Border.X==0 ||
      bldFinalDraw.Border.Y==0
  ){
    DBGHITEFFINFO;

    if(iState!=iItgState)DBGABORT("hiteffect not initialized or not setup properly %d!=%d",iState,iItgState);

    DBGABORT("invalid hit effect setup"); //will force exit right here during development
    return false;
  }
  return true;
}

void hiteffect::Draw() const //TODO is it being called other than thru DrawStep()?
{
  chkCleanupAlready();

  if(!bBlitdataWasSet)return;

  /**
   * As this is just an effect, it's funtionally shall not break the game.
   * So it will simply return if something is wrong.
   */
  if(!CheckIntegrity(ITGSTATE_SETBLD))return;

  if(iDrawCount<iDrawTot)
    bmpHitEffect->NormalMaskedBlit(bldFinalDraw); //TODO use alpha?
}
