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
square* hiteffect::GetSquareUnderEntity(int) const { return LSquareUnder; }

hiteffect::hiteffect(item* Item, lsquare* LSquareUnder, int iDrawTimeS, character* WhoIsHit, character* WhoHits, int Type, int GivenDir)
: entity(HAS_BE), Next(0), LSquareUnder(LSquareUnder), iDrawTimes(iDrawTimeS)
{
  static short int iL=0xFF*0.60;
  static col24 lum=MakeRGB24(iL,iL,iL);
  static short int iIsh=0xFF*0.90;
  static col24 lumBluish  =MakeRGB24(iL,iL,iIsh);
  static col24 lumGreenish=MakeRGB24(iL,iIsh,iL);
  static col24 lumReddish =MakeRGB24(iIsh,iL,iL);
  static blitdata bld = [](){
    blitdata B = DEFAULT_BLITDATA;
    B.Luminance = lum;
    B.Border = TILE_V2;
    return B;
  }();

  if(WhoIsHit->IsPlayer() || WhoIsHit->IsPet()){
    bld.Luminance = lumReddish;
  }else
  if(WhoHits->IsPlayer()){
    bld.Luminance = lumBluish;
  }else
  if(WhoHits->IsPet()){
    bld.Luminance = lumGreenish;
  }

  static bitmap* bmpTmp = new bitmap(TILE_V2);
  (bld.Bitmap = bmpTmp)->ClearToColor(TRANSPARENT_COLOR);
  // reset static blitdata dir, grant it is drawn pointing to default top right
  bld.Flags &= ~MIRROR;
  bld.Flags &= ~FLIP;
  bld.Flags &= ~ROTATE;
  Item->Draw(bld); //the item* cant be stored as it may break and be sent to hell after here...

  /**
   * GivenDir TODO make this functionality more global
   * 012
   * 3 4
   * 567
   * See doc MirrorFlipRotateToDegrees.jpg
   */
  DBGEXEC(if(WhoHits->IsPlayer())DBG3(DBGI(Type),DBGI(GivenDir),DBGC(Item->GetName(DEFINITE).CStr())));
  // there is no horizontal or vertical easy rotations implemented yet TODO but ex.: spears would not fit in 16x16 w/o shrinking it...
  if(GivenDir==1)GivenDir = clock()%2==0 ? 0 : 2;
  if(GivenDir==3)GivenDir = clock()%2==0 ? 0 : 5;
  if(GivenDir==4)GivenDir = clock()%2==0 ? 2 : 7;
  if(GivenDir==6)GivenDir = clock()%2==0 ? 5 : 7;
  switch(GivenDir){ //this reuses the blitdata!
  case 0:
//    bld.Flags |= ROTATE|FLIP|MIRROR; //270 degrees
    bld.Flags |= MIRROR; //270 degrees
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
    LSquareUnder->RemoveHitEffect(this);
    SendToHell();
    return;
  }else{
    LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible
  }
}

truth hiteffect::DrawStep(blitdata bld)
{
  if(iDrawTimes==0)return false;

  Draw(bld);

  iDrawTimes--; // AFTER drawing!

  LSquareUnder->SendStrongNewDrawRequest(); //too unbuffer all hit effects as soon as possible

  return true; //did draw now
}

void hiteffect::Draw(blitdata& bld) const
{
  if(iDrawTimes>0){ //TODO use rotation?
    bmpHitEffect->NormalMaskedBlit(bld);
  }
}
