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

hiteffect::hiteffect() : entity(HAS_BE), Next(0) { }
square* hiteffect::GetSquareUnderEntity(int) const { return LSquareUnder; }

hiteffect::hiteffect(item* addItem, lsquare* LSquareUnder, int iDrawTimeS)
: entity(HAS_BE), Next(0), Item(addItem), LSquareUnder(LSquareUnder), iDrawTimes(iDrawTimeS)
{
}

hiteffect::~hiteffect()
{
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
  //  static blitdata bldShowHit = [](v2 v2Dest){ //static bool b = []() -> bool {return true;}; //static int iA = []() -> int { return 0; }();
  //    blitdata bld=DEFAULT_BLITDATA;
  //    bld.Bitmap=DOUBLE_BUFFER;
  //    bld.Border=TILE_V2;
  ////    bld.CustomData |= ALLOW_ANIMATE;
//      bld.Luminance = MakeRGB24(127,127,127); //NORMAL_LUMINANCE;
  //    return bld;
  //  }(HitPos);
  //  if(Weapon!=NULL){
  //    bldShowHit.Dest=game::CalculateScreenCoordinates(HitPos);
  //    Weapon->Draw(bldShowHit);
  ////    clock_t StartTime = clock();
  ////    while(clock() - StartTime < 0.5 * CLOCKS_PER_SEC){
  ////      Weapon->Draw(bldShowHit);
  ////    }

  if(iDrawTimes>0){
    static int iL=0xFF*0.70;
    static col24 lum=MakeRGB24(iL,iL,iL);
    bld.Luminance = lum;
    Item->Draw(bld); //TODO use rotation?
  }
//  iDrawTimes--;
}
