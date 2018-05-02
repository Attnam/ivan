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

#define DBGMSG_V2
#include "dbgmsgproj.h"

hiteffect::hiteffect() : entity(HAS_BE), Next(0) { }
square* hiteffect::GetSquareUnderEntity(int) const { return setup.LSquareUnder; }

/**
 * TODO kept Type to use custom pictures for bite(bigMouthWithTeeths), kick(bigFoot) and unarmed(BigPunchHand) one day
 */
hiteffect::hiteffect(hiteffectSetup s)
: entity(HAS_BE), Next(0), iDrawTimes(3)
{
  static short int iHigh=0xFF*0.85;
  static col24 lumHigh=MakeRGB24(iHigh,iHigh,iHigh);
  static short int iL=0xFF*0.40; //darker works better with already light graphics
  static short int iIsh=0xFF*0.90;
  static col24 lumReddish =MakeRGB24(iIsh,iL,iL);
  static col24 lumGreenish=MakeRGB24(iL,iIsh,iL);
  static col24 lumBluish  =MakeRGB24(iL,iL,iIsh);
  static blitdata bld = [](){
    blitdata B = DEFAULT_BLITDATA;
    B.Border = TILE_V2;
    return B;
  }();

  setup=s;

  switch(setup.iMode){
  case 1: //immersive
    bld.Luminance = NORMAL_LUMINANCE;
    break;
  case 2: //indicator
    bld.Luminance = lumHigh;
    break;
  case 3: //colored indicators where possible
    if(setup.WhoIsHit->IsPlayer() || setup.WhoIsHit->IsPet()){
      bld.Luminance = lumReddish;
    }else
    if(setup.WhoHits->IsPlayer()){
      bld.Luminance = lumBluish;
    }else
    if(setup.WhoHits->IsPet()){
      bld.Luminance = lumGreenish;
    }else{
      bld.Luminance = lumHigh;
    }
    break;
  case 4: //dynamic colored indicators or not based on usefulness of the color indicator
    if(setup.WhoIsHit->IsPlayer() || setup.WhoHits->IsPlayer()){ //player being hit or hitting is w/o doubts the origin of the attack
      if(setup.Critical){
        bld.Luminance = lumReddish;
      }else{
        bld.Luminance = NORMAL_LUMINANCE;
      }
    }else
    if(setup.WhoIsHit->IsPet()){
      bld.Luminance = lumReddish;
    }else
    if(setup.WhoHits->IsPet()){
      if(setup.itemEffectReference->IsWeapon(setup.WhoHits)){ //TODO why char as param?
        bld.Luminance = lumBluish;
      }else{
        bld.Luminance = lumGreenish;
      }
    }else{
      bld.Luminance = NORMAL_LUMINANCE;
    }
    break;
  default:
    ABORT("unsupported hiteffect mode %d",setup.iMode);
  }

  static bitmap* bmpTmp = new bitmap(TILE_V2);
  (bld.Bitmap = bmpTmp)->ClearToColor(TRANSPARENT_COLOR);
  // reset static blitdata dir, grant it is drawn pointing to default top right
  bld.Flags &= ~MIRROR;
  bld.Flags &= ~FLIP;
  bld.Flags &= ~ROTATE;
  setup.itemEffectReference->Draw(bld); //the item* cant be stored as it may break and be sent to hell after here...

  /**
   * GivenDir TODO make this functionality more global?
   * 012
   * 3 4
   * 567
   * */

  /*
   * From the tests, top right as 0 degrees HERE is only for 1 3 4 6. (but not always...)
   * From the tests, top left  is 0 degrees HERE for         0 2 5 7. (but not always...)
   * TODO WHY!?!? what sets these stuff so randomly?
   * */

  /*
   * TODO the items graphics are not all pointing to the upper right as 0 degrees!!! fix the graphics file!!! ? or set a hint for each item?
   *
   * For now, this will only work for items like the single bleded axe and mace...
   * TODO unless they suddenly come pre-flipped, WHY???
   *
   * See doc MirrorFlipRotateToDegrees.jpg
   *
   * The below may only work AFTER all items are poiting to the upper right ALWAYS :(
   */
  bool bRandomDir=true; // this should be kept true until all items are properly pointing to upper right when reaching this method!

  DBGEXEC(if(setup.WhoHits->IsPlayer())DBG3(DBGI(setup.Type),DBGI(setup.GivenDir),DBGC(setup.itemEffectReference->GetName(DEFINITE).CStr())));
  // there is no horizontal or vertical easy rotations implemented yet TODO but ex.: spears would not fit in 16x16 w/o shrinking it...
  int iDir = bRandomDir ? clock()%8 : setup.GivenDir;
  if(iDir==1)iDir = clock()%2==0 ? 0 : 2;
  if(iDir==3)iDir = clock()%2==0 ? 0 : 5;
  if(iDir==4)iDir = clock()%2==0 ? 2 : 7;
  if(iDir==6)iDir = clock()%2==0 ? 5 : 7;
  switch(iDir){ //this reuses the blitdata!
  case 0:
    bld.Flags |= ROTATE|FLIP|MIRROR; //270 degrees
    break;
  case 2: //default is top right
    break;
  case 5:
    bld.Flags |= FLIP|MIRROR; //180 degrees
    break;
  case 7:
    bld.Flags |= ROTATE; //90 degrees
    break;
  }

  ( bld.Bitmap = bmpHitEffect = new bitmap(TILE_V2) )->ClearToColor(TRANSPARENT_COLOR); //TODO is clear unnecessary?
  bmpTmp->NormalBlit(bld); //this "rotates"
}

hiteffect::~hiteffect()
{
  delete bmpHitEffect;
}

void hiteffect::Be()
{
  if(iDrawTimes==0)
  {
    setup.LSquareUnder->RemoveHitEffect(this);
    SendToHell();
    return;
  }else{
    setup.LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible
  }
}

truth hiteffect::DrawStep(blitdata bld)
{
  if(iDrawTimes==0)return false;

  Draw(bld);

  iDrawTimes--; // AFTER drawing!

  setup.LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible

  return true; //did draw now
}

void hiteffect::Draw(blitdata& bld) const
{
  if(iDrawTimes>0){ //TODO use rotation?
    bmpHitEffect->NormalMaskedBlit(bld);
  }
}
