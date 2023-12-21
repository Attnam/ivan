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

/* compiled thru command.cpp */

#include "dbgmsgproj.h"

struct swapweaponcfg{
  public:
  ulong iLeftId=0;
  ulong iRightId=0;

  void SetLR(bool bL, item* it){
    if(bL){
      iLeftId  = it!=NULL ? it->GetID() : 0;
    }else{
      iRightId = it!=NULL ? it->GetID() : 0;
    }
  }

  void SetL(item* it){
    SetLR(true,it);
  }
  void SetR(item* it){
    SetLR(false,it);
  }

  item* GetLR(bool bL){
    item* it = game::SearchItem(bL?iLeftId:iRightId); //slower? but granted as item::Fix() (at blacksmith) will change the pointer but keep the ID!!!
    if(it!=NULL)
      return it;

    // cleanup on not found
    if(bL)
      SetL(NULL);
    else
      SetR(NULL);

    return NULL;
  }

  item* GetL(){
    return GetLR(true);
  }
  item* GetR(){
    return GetLR(false);
  }

  void Save(outputfile& SaveFile){DBG2(iLeftId,iRightId);
    SaveFile << iLeftId;
    SaveFile << iRightId;
  }
  void Load(inputfile& SaveFile){DBGLN;
    ulong iLeftId =0;SaveFile >> iLeftId;
    SetL(iLeftId !=0 ? game::SearchItem(iLeftId ) : NULL); DBG1(iLeftId);

    ulong iRightId=0;SaveFile >> iRightId;
    SetR(iRightId!=0 ? game::SearchItem(iRightId) : NULL); DBG1(iRightId);
  }

  bool IsValid(){
    return GetL()!=NULL || GetR()!=NULL;
  }

  // temporary LIST CONTROLS
  int iKeyActivate=-1;
  int iKeyRm=-1;
  int iKeySwapArms=-1;
  int iKeyDown=-1;
  int iKeyUp=-1;
  void ClearListControls(){
    iKeyActivate=-1;
    iKeyRm=-1;
    iKeySwapArms=-1;
    iKeyDown=-1;
    iKeyUp=-1;
  }

};
std::vector<swapweaponcfg> vSWCfg;
std::vector<swapweaponcfg> vSWCfgRemoved; //wont be saved tho
int iSwapCurrentIndex=0;
static int awRL[2]={RIGHT_WIELDED_INDEX,LEFT_WIELDED_INDEX};
void commandsystem::ClearSwapWeapons()
{
  vSWCfg.clear();
}
void commandsystem::SaveSwapWeapons(outputfile& SaveFile)
{DBGLN;
  SaveFile << static_cast<ushort>(iSwapCurrentIndex);

  SaveFile << static_cast<ushort>(vSWCfg.size()); DBG2(iSwapCurrentIndex,vSWCfg.size());
  for(int i=0;i<vSWCfg.size();i++)
    vSWCfg[i].Save(SaveFile);

  //do not ClearSwapWeapons() as it saves 2 times subsequently TODO why?
  DBGLN;
}
void commandsystem::LoadSwapWeapons(inputfile& SaveFile)
{DBGLN;
  ClearSwapWeapons(); //make sure it is always cleaned from memory!
  if(game::GetCurrentSavefileVersion()<132)
    return;

  SaveFile >> reinterpret_cast<ushort&>(iSwapCurrentIndex);

  int iSize=0;
  SaveFile >> reinterpret_cast<ushort&>(iSize); DBG2(iSwapCurrentIndex,iSize);
  for(int i=0;i<iSize;i++){
    swapweaponcfg cfg;
    cfg.Load(SaveFile);
    vSWCfg.push_back(cfg);
  }
}
void clearInvalidSwapCfgs()
{
  for(int i=vSWCfg.size()-1;i>=0;i--) //from last to 1st has no problem erasing, wont break the indexes!
    if(!vSWCfg[i].IsValid())
      {vSWCfg.erase(vSWCfg.begin()+i);DBG2("removingInvalid",i);}
}
truth hasItem(itemvector& iv, item* it){
  //TODO why this wont compile? ->   if(std::find(iv.begin(),iv.end(),it)!=iv.end())return true;
  for(int i=0;i<iv.size();i++)
    if(iv[i]==it)
      return true;
  return false;
}
truth commandsystem::SwapWeaponsCfg(character* Char)
{DBG1(vSWCfg.size());
  if(!Char->IsHumanoid() || dynamic_cast<ghost*>(Char)){DBGLN;
    ADD_MESSAGE("This monster type cannot wield weapons.");
    return false;
  }

  humanoid* h = dynamic_cast<humanoid*>(Char);DBGLN;

  itemvector iv;
  h->GetStack()->FillItemVector(iv);

  item* wL = h->GetLeftWielded();
  item* wR = h->GetRightWielded();

  // less colors as possible to be less confusing
  static col16 colMaintOpts = LIGHT_GRAY; //MakeRGB16(255*0.5,0xFF*0.5,0xFF*0.75); //dark gray bluish
  //  static col16 colWieldedCfg = MakeRGB16(200,200,200); // a bit lighter gray
  static col16 colWieldedCfg = colMaintOpts; //MakeRGB16(255*0.5,0xFF*0.75,0xFF*0.5); // gray greenish
  static col16 colAlert = colMaintOpts; //MakeRGB16(255*0.75,0xFF*0.75,0xFF*0.5); //dark gray yellowish
  static col16 colNotOnInv = MakeRGB16(255*0.75,0xFF*0.5,0xFF*0.5);
  static col16 colAtPlayerSquare = MakeRGB16(255*0.5,0xFF*0.62,0xFF*0.5);
  static col16 colWielded = colMaintOpts;
//  static col16 colWieldedCfg = MakeRGB16(0,255*0.75,255);//cyan

  bool bMaintenanceMode=false;
  int iSelectAgain=-1;
  for(;;){DBGLN;
    felist Cfgs(CONST_S("How you want to change your swap weapons configurations?"));

    int iSelectableIndex=0;

    int iKeyAdd = iSelectableIndex++;
    Cfgs.AddEntry(festring("Add current wieldings as a new config"),WHITE,0,game::AddToItemDrawVector(itemvector()),true);

    int iKeyUndoLastRm = -1;
    if(bMaintenanceMode)
      if(vSWCfgRemoved.size()>0){
        iKeyUndoLastRm = iSelectableIndex++;
        Cfgs.AddEntry(festring("Undo previous removal"),colMaintOpts,0,game::AddToItemDrawVector(itemvector()),true);
      }

    int iKeyMaintenance=-1;
    if(!bMaintenanceMode){
      iKeyMaintenance = iSelectableIndex++;
      Cfgs.AddEntry(festring("Maintenance mode"),colMaintOpts,0,game::AddToItemDrawVector(itemvector()),true);
    }

    // each config
    bool bHasInvalid=false;
    for(int i=0;i<vSWCfg.size();i++){
      vSWCfg[i].ClearListControls();
      if(!vSWCfg[i].IsValid()){bHasInvalid=true;DBG2("skipInvalid",i);continue;}

//      if(iSwapCurrentIndex==i)
      if(h->GetLeftWielded()==vSWCfg[i].GetL() && h->GetRightWielded()==vSWCfg[i].GetR())
        Cfgs.AddEntry(festring()<<"   CURRENTLY WIELDED ----------------------",colWieldedCfg,0,game::AddToItemDrawVector(itemvector()),false);
      else{
        vSWCfg[i].iKeyActivate = iSelectableIndex++; DBG2(i,vSWCfg[i].iKeyActivate);
        Cfgs.AddEntry(festring()<<"Wield these now ---------------------------",WHITE,0,game::AddToItemDrawVector(itemvector()),true);
      }

      if(bMaintenanceMode){
        vSWCfg[i].iKeyRm = iSelectableIndex++; DBG2(i,vSWCfg[i].iKeyRm);
        Cfgs.AddEntry(festring()<<"Remove this config", colAlert,0,game::AddToItemDrawVector(itemvector()),true);

        vSWCfg[i].iKeySwapArms = iSelectableIndex++; DBG2(i,vSWCfg[i].iKeySwapArms);
        Cfgs.AddEntry(festring()<<"Swap arms", colMaintOpts,0,game::AddToItemDrawVector(itemvector()),true);

        if(i>0){DBGLN;
          vSWCfg[i].iKeyUp = iSelectableIndex++;
          Cfgs.AddEntry(festring()<<"Move up", colMaintOpts,0,game::AddToItemDrawVector(itemvector()),true);
        }DBGLN;
      }

      for(int j=0;j<2;j++){DBGLN;
        festring fs; fs<<"   ";
        item* it = NULL;
        item* w = NULL;
        col16 cW = DARK_GRAY; //LIGHT_GRAY;
//        static col16 colDarkW    = MakeRGB16(      0, 255*0.5, 255*0.75); // dark cyan
//        static col16 colDarkW    = MakeRGB16(200,200,200); // a bit lighter gray
//        static col16 colNotOnInv = MakeRGB16(255*0.75,0xFF*0.5,0xFF*0.5); //dark gray reddish

        switch(awRL[j]){
        case LEFT_WIELDED_INDEX:
          it = vSWCfg[i].GetL();
          w = wL;
          fs << "(L";
//          if(it==w)cW=colDarkW;//fs<<"!";
          fs << ") ";
          break;
        case RIGHT_WIELDED_INDEX:
          it = vSWCfg[i].GetR();
          w = wR;
          fs << "(R";
//          if(it==w)cW=colDarkW;//fs<<"!";
          fs << ") ";
          break;
        }

        if(it){
          if(it==wL || it==wR)
            cW = colWielded;//colDarkW;
          else if(hasItem(iv,it))
            cW = DARK_GRAY; // on inv
          else if(it->GetSquareUnder()==Char->GetSquareUnder())
            cW = colAtPlayerSquare;
          else
            cW = colNotOnInv; //inaccessible

//          if(it!=wL && it!=wR && !hasItem(iv,it))cW = colNotOnInv;

          if(!it->Exists())ABORT("item doesnt exist! %lu %s",it->GetID(),it->GetName(DEFINITE).CStr()); //this may segfault tho...
          it->AddInventoryEntry(Char, fs, 1, true);
          Cfgs.AddEntry(fs, cW, 0, game::AddToItemDrawVector(itemvector(1,it)), false);
        }
      }

      if(bMaintenanceMode)
        if(i<vSWCfg.size()-1){
          vSWCfg[i].iKeyDown = iSelectableIndex++;
          Cfgs.AddEntry(festring()<<"Move down", colMaintOpts,0,game::AddToItemDrawVector(itemvector()),true);
        }

    }

    if(bHasInvalid)clearInvalidSwapCfgs();

    game::SetStandardListAttributes(Cfgs);DBGLN;
    Cfgs.SetEntryDrawer(game::ItemEntryDrawer);
    Cfgs.AddFlags(SELECTABLE);
    game::DrawEverythingNoBlit(); // doesn't prevent mirage puppies
    if(iSelectAgain>-1)
      Cfgs.SetSelected(iSelectAgain);
    int Selected = Cfgs.Draw(); DBG1(Selected);
    game::ClearItemDrawVector();

    if(Selected & FELIST_ERROR_BIT)
      return false;

    if(Selected==iKeyAdd){DBGLN;
      swapweaponcfg cfg;
      if(h->GetLeftWielded())
        cfg.SetL(h->GetLeftWielded());
      if(h->GetRightWielded())
        cfg.SetR(h->GetRightWielded());
      vSWCfg.push_back(cfg);
    }else
    if(bMaintenanceMode && Selected==iKeyUndoLastRm){DBGLN;
      vSWCfg.push_back(vSWCfgRemoved[vSWCfgRemoved.size()-1]);
      //vSWCfgRemoved.erase(vSWCfgRemoved.end());//-1);
    }else
    if(Selected==iKeyMaintenance){
      bMaintenanceMode=true;
    }else{
      for(int i=0;i<vSWCfg.size();i++){
        if(bMaintenanceMode && Selected==vSWCfg[i].iKeyRm){ DBG3(i,Selected,vSWCfg[i].iKeyRm);
          vSWCfgRemoved.push_back(vSWCfg[i]);
          vSWCfg.erase(vSWCfg.begin()+i);
          if(iSwapCurrentIndex==i)
            iSwapCurrentIndex=vSWCfg.size(); //to be 0 next request TODO could just be next on list but may be unnecessarily complex to implement?
          break; //vector safe
        }else
        if(bMaintenanceMode && (Selected==vSWCfg[i].iKeyDown || Selected==vSWCfg[i].iKeyUp)){
          swapweaponcfg cfgMove = vSWCfg[i]; DBG2(cfgMove.GetR(),cfgMove.GetL());

          if(Selected==vSWCfg[i].iKeyDown){
            vSWCfg.insert(vSWCfg.begin()+i+2,cfgMove);
            vSWCfg.erase (vSWCfg.begin()+i);
          }else
          if(Selected==vSWCfg[i].iKeyUp){
            vSWCfg.insert(vSWCfg.begin()+i-1,cfgMove);
            vSWCfg.erase (vSWCfg.begin()+i+1);
          }

          break; //vector safe
        }else
        if(Selected==vSWCfg[i].iKeyActivate){DBGLN;
          SwapWeaponsWork(Char,i);
          return false; //to close it as gained experience with dexterity action!
        }else
        if(Selected==vSWCfg[i].iKeySwapArms){DBGLN;
          item* itL = vSWCfg[i].GetL();
          vSWCfg[i].SetL(vSWCfg[i].GetR());
          vSWCfg[i].SetR(itL);
          break;
        }
      }
    }

  } // list draw loop

  return false;
}

truth commandsystem::SwapWeapons(character* Char)
{
  return SwapWeaponsWork(Char);
}
truth commandsystem::SwapWeaponsWork(character* Char, int iIndexOverride)
{
  if(!Char->IsHumanoid()){DBGLN;
    ADD_MESSAGE("This monster type cannot wield weapons.");
    return false;
  }

  if(!Char->CanUseEquipment()){DBGLN;
    ADD_MESSAGE("You cannot wield anything.");
    return false;
  }

  if(vSWCfg.size()==0){DBGLN;
    ADD_MESSAGE("You should prepare some quick swap weapons first!");
    return false;
  }

  humanoid* h = dynamic_cast<humanoid*>(Char); DBG2(iSwapCurrentIndex,vSWCfg.size());

  item* wL = h->GetLeftWielded();
  item* wR = h->GetRightWielded();

  if(iIndexOverride==-1){
    //make it sure what is the current index
    for(int i=0;i<vSWCfg.size();i++)
      if(vSWCfg[i].GetL()==wL && vSWCfg[i].GetR()==wR){
        iSwapCurrentIndex=i;
        break;
      }

    iSwapCurrentIndex++;
  }else
    iSwapCurrentIndex=iIndexOverride;

  if(iSwapCurrentIndex >= vSWCfg.size())
    iSwapCurrentIndex=0;

  bool bDidSwap=false;

  stack* stk = h->GetStack();

  /**
   * important to work correctly if user asks for 2handed in both hands, or if it repeats on another cfg,
   * anyway, should be like the user configured and not guessed.
   */
  if(wL)wL->MoveTo(stk);
  if(wR)wR->MoveTo(stk);

  int iAlreadyWieldedCfg=0;
  for(int iArm=0;iArm<2;iArm++){DBGLN;
    item* it = NULL;
    arm* Arm = NULL;

    switch(awRL[iArm]){
    case LEFT_WIELDED_INDEX:
      Arm=h->GetLeftArm();
      it = vSWCfg[iSwapCurrentIndex].GetL();
      if(it==wL)iAlreadyWieldedCfg++;
      break;
    case RIGHT_WIELDED_INDEX:
      Arm=h->GetRightArm();
      it = vSWCfg[iSwapCurrentIndex].GetR();
      if(it==wR)iAlreadyWieldedCfg++;
      break;
    }

    if(Arm && it){
      std::vector<item*> iv;
      stk->FillItemVector(iv);
      if(hasItem(iv,it) || (it->GetSquareUnder()==Char->GetSquareUnder() && !it->GetRoom()) ) {
        it->RemoveFromSlot(); // w/o this line of code (TODO mem gets corrupted?), it will SEGFAULT when saving the game! extremelly hard to track!!! TODO it is hard to track right?
        h->SetEquipment(awRL[iArm],it);
        bDidSwap=true;
      }
    }

  }

//  static int iRecursiveRetry=0; //this is risky..
//  if(iAlreadyWieldedCfg==2 && vSWCfg.size()>1){
//    iRecursiveRetry++;
//    SwapWeaponsWork(Char,iIndexOverride);
//  }

  if(bDidSwap)
    Char->DexterityAction(5);

  return true;
}
