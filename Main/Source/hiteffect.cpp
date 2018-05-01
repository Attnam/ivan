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

hiteffect::hiteffect(item* Item, lsquare* LSquareUnder, int iDrawTimeS, character* WhoIsHit, character* WhoHits)
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

  bmpHitEffect = bld.Bitmap = new bitmap(TILE_V2);
  bmpHitEffect->ClearToColor(TRANSPARENT_COLOR);

  v2 v2HitFromDir = LSquareUnder->GetPos() - WhoHits->GetPos(); DBGEXEC(if(WhoHits->IsPlayer())DBGSV2(v2HitFromDir));

  Item->Draw(bld); //the item* cant be stored as it may break and be sent to hell after here...
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
