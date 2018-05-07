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

hiteffect::hiteffect() : entity(HAS_BE), Next(0), iState(-1), bBlitdataWasSet(false) { }

square* hiteffect::GetSquareUnderEntity(int) const {
  return setup.LSquareUnder;
}

#define DBGSTATE_INIT    1 //in order always
#define DBGSTATE_SETBLD  2
#define DBGSTATE_CLEANUP 3
#define DBGSTATE_DESTROY 4
#define DBGHITEFFINFO \
    DBGSI(iState); \
    DBG6("pointers",this,setup.WhoHits,setup.WhoIsHit,bmpHitEffect,setup.itemEffectReference); \
    DBG3(setup.Type,bWhoIsHitDied,DBGAV2(v2HitFromSqrPos)); \
    DBGSV2(v2HitToSqrPos); \
    DBG2(setup.WhoHits->GetName(DEFINITE).CStr(),setup.WhoIsHit->GetName(DEFINITE).CStr()); \
    DBGBLD(bldFinalDraw); \
    DBGSV2(bmpHitEffect->GetSize()); \
    DBGSC(setup.itemEffectReference->GetName(DEFINITE).CStr()); \
    DBGSTK;

hiteffect::hiteffect(hiteffectSetup s)
: entity(HAS_BE), Next(0), iDrawCount(0), iState(0), bBlitdataWasSet(false)
{
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
  default:
    ABORT("unsupported hiteffect mode %d",setup.iMode);
  }

  switch(setup.Type) //TODO use arm, feet, head material colors
  {
   case WEAPON_ATTACK:
    setup.itemEffectReference->Draw(bldLum); //the item* cant be stored as it may break and be sent to hell after here...
    break;
   case UNARMED_ATTACK:
    if(bldLum.Src.X==0)bldLum.Src.X = 16;
    /* no break */
   case KICK_ATTACK:
    if(bldLum.Src.X==0)bldLum.Src.X = 32;
    /* no break */
   case BITE_ATTACK: // X is at 0
    bldLum.Src.Y = 112;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(bldLum);
    break;
  }

  /**
   * GivenDir
   * 012
   * 3 4
   * 567
   *
   * TODO what sets these stuff so randomly?
   *      From the tests, top right as 0 degrees HERE is only for 1 3 4 6. (but not always...)
   *      From the tests, top left  is 0 degrees HERE for         0 2 5 7. (but not always...)
   *
   * TODO the items graphics are not all pointing to the upper right as 0 degrees!!! fix the graphics file!!! ? or set a hint for each item?
   *
   * For now, this will only work for items like the single bladed axe and mace (unless they suddenly come pre-flipped TODO what sets these stuff so randomly?)
   *
   * See doc MirrorFlipRotateToDegrees.jpg
   *
   * TODO The below may only work AFTER the day all items are poiting to the upper right ALWAYS (not the random that is now)
   */
  bool bRandomDir=true; // TODO this should be kept true until all items are properly pointing to upper right when reaching this method! because otherwise it will just keep a wrong rotation and will look worse than it already is...
  static blitdata bldRotate = [](){bldRotate = DEFAULT_BLITDATA;bldRotate.Border = TILE_V2;return bldRotate;}();
  bldRotate.Bitmap=NULL; //reset

  // reset static blitdata dir, grant it is drawn pointing to default top right
  bldRotate.Flags &= ~MIRROR;
  bldRotate.Flags &= ~FLIP;
  bldRotate.Flags &= ~ROTATE;

  DBGEXEC(if(setup.WhoHits->IsPlayer())DBG3(DBGI(setup.Type),DBGI(setup.GivenDir),DBGC(setup.itemEffectReference->GetName(DEFINITE).CStr())));
  // there is no horizontal or vertical easy rotations implemented yet TODO but ex.: spears would not fit in 16x16 w/o shrinking it...
  int iDir = bRandomDir ? clock()%8 : setup.GivenDir;
  if(iDir==1)iDir = clock()%2==0 ? 0 : 2;
  if(iDir==3)iDir = clock()%2==0 ? 0 : 5;
  if(iDir==4)iDir = clock()%2==0 ? 2 : 7;
  if(iDir==6)iDir = clock()%2==0 ? 5 : 7;
  switch(iDir){ //this reuses the blitdata!
  case 0:
    bldRotate.Flags |= ROTATE|FLIP|MIRROR; //270 degrees
    break;
  case 2: //default is top right
    break;
  case 5:
    bldRotate.Flags |= FLIP|MIRROR; //180 degrees
    break;
  case 7:
    bldRotate.Flags |= ROTATE; //90 degrees
    break;
  }

  (bmpHitEffect = bldRotate.Bitmap = new bitmap(TILE_V2))->ClearToColor(TRANSPARENT_COLOR); DBGLN; //TODO is clear unnecessary?
  bldLum.Bitmap->NormalBlit(bldRotate); DBGLN; //this "rotates"

  SetIntegrityState(DBGSTATE_INIT);
}

void hiteffect::SetIntegrityState(int i){
  if(iState != i-1){
    DBGHITEFFINFO;
    ABORT("Wrong state usage(%d), must always be equal to current(%d)+1",i,iState);
  }

  iState=i;
}

hiteffect::~hiteffect()
{
  if(bmpHitEffect!=NULL)delete bmpHitEffect;
  iState=DBGSTATE_DESTROY; //not safe to  use this here: SetIntegrityState(DBGSTATE_DESTROY); as may have debug messages...
}

void hiteffect::Be()
{
  if(iDrawCount==iDrawTot)
  {
    setup.LSquareUnder->RemoveHitEffect(this);
    cleanup();
    SendToHell();
    return;
  }else{
    if(bBlitdataWasSet){
      CheckIntegrity(DBGSTATE_SETBLD);
      setup.LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible
    }
  }
}

void hiteffect::cleanup(){
  // TODO necessary?
  setup.LSquareUnder=NULL;
  setup.WhoHits=NULL;
  setup.WhoIsHit=NULL;
  setup.itemEffectReference=NULL;

  //not external reference, must be deleted at destructor: bmpHitEffect=NULL;

  LSquareUnderOfWhoHits=NULL;
  vExtraSquares.clear(); //TODO move to destructor? or is automatic?

  SetIntegrityState(DBGSTATE_CLEANUP);
}

void hiteffect::PrepareBlitdata(const blitdata& bld){
  bldFinalDraw=bld; DBG1(this);DBGBLD(bldFinalDraw); //copy
  bBlitdataWasSet=true;

  if(iState!=DBGSTATE_SETBLD){ //can be set more than once
    CheckIntegrity(DBGSTATE_INIT); //must be only  this one here
    SetIntegrityState(DBGSTATE_SETBLD);
  }
}

void hiteffect::End(){
  iDrawCount=iDrawTot;
}

truth hiteffect::DrawStep()
{
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

  //TODO use rotation?
  if(bAnimate){ //just to not fly if it died already
    //place at who hits to fly from it to who is hit
    bldFinalDraw.Dest += v2HitFromToSqrDiff*TILE_SIZE;

    //displacement, interpolate TODO could be at v2 class with To and fPercent!
    bldFinalDraw.Dest += -(v2HitFromToSqrDiff*TILE_SIZE)*(iDrawCount/(float)iDrawTot);
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

truth hiteffect::CheckIntegrity(int iDbgState) const{
  if(
      bldFinalDraw.Border.X==0 ||
      bldFinalDraw.Border.Y==0
  ){
#ifdef DBGMSG
    DBGHITEFFINFO;
    ABORT("invalid hit effect setup"); //will force exit right here during development
#endif

    if(iState!=iDbgState)ABORT("hiteffect not initialized or not setup properly %d!=%d",iState,iDbgState);

    return false;
  }
  return true;
}

void hiteffect::Draw() const //TODO is it being called other than thru DrawStep()?
{
  /**
   * As this is just an effect, it's funtionally shall not break the game.
   * So it will simply return if something is wrong.
   */
  if(!CheckIntegrity(DBGSTATE_SETBLD))return;

  if(iDrawCount<iDrawTot)
    bmpHitEffect->NormalMaskedBlit(bldFinalDraw); //TODO use alpha?
}
