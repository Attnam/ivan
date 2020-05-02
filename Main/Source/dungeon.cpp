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

#include "audio.h"
#include "bitmap.h"
#include "database.h"
#include "dbgmsgproj.h"
#include "dungeon.h"
#include "error.h"
#include "femath.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "message.h"
#include "save.h"
#include "script.h"


dungeon::dungeon(int Index) : Index(Index)
{
  Initialize();

  for(int c = 0; c < GetLevels(); ++c)
    Generated[c] = false;
}

dungeon::~dungeon()
{
  for(int c = 0; c < GetLevels(); ++c)
    delete Level[c];

  delete [] Level;
  delete [] Generated;
}

void dungeon::Initialize()
{
  std::map<int, dungeonscript>::const_iterator DungeonIterator = game::GetGameScript()->GetDungeon().find(Index);

  if(DungeonIterator != game::GetGameScript()->GetDungeon().end())
    DungeonScript = &DungeonIterator->second;
  else
  {
    ABORT("Unknown dungeon #%d requested!", Index);
    return;
  }

  Level = new level*[GetLevels()];
  Generated = new truth[GetLevels()];

  for(int c = 0; c < GetLevels(); ++c)
    Level[c] = 0;
}

const levelscript* dungeon::GetLevelScript(int I)
{
  std::map<int, levelscript>::const_iterator LevelIterator = DungeonScript->GetLevel().find(I);

  const levelscript* LevelScript;

  if(LevelIterator != DungeonScript->GetLevel().end())
    LevelScript = &LevelIterator->second;
  else
    LevelScript = DungeonScript->GetLevelDefault();

  return LevelScript;
}

/* Returns whether the level has been visited before */

truth dungeon::PrepareLevel(int Index, truth Visual)
{
  graphics::SetDenyStretchedBlit();

  if(Generated[Index])
  {
    level* NewLevel = LoadLevel(game::SaveName(), Index);
    game::SetCurrentArea(NewLevel);
    game::SetCurrentLevel(NewLevel);
    game::SetCurrentLSquareMap(NewLevel->GetMap());
    return true;
  }
  else
  {
    festring fsGenLoopDL;{const char* pc = std::getenv("IVAN_DebugGenDungeonLevelLoopID");if(pc!=NULL)fsGenLoopDL<<pc;}
    int iGenLoopMax=250;{const char* pc = std::getenv("IVAN_DebugGenDungeonLevelLoopMax");if(pc!=NULL)iGenLoopMax=atoi(pc);}
    
    festring fsDL;fsDL<<GetIndex()<<Index;
    int iRetryMax = fsGenLoopDL==fsDL ? iGenLoopMax : 10;
    
    DBG3("GeneratingDungeonLevel",fsDL.CStr(),fsGenLoopDL.CStr());
    
    level* NewLevel=NULL;
    cbitmap* EnterImage=NULL;
    for(int i=0;i<iRetryMax;i++){
      try{
        if(!genericException::ToggleGenNewLvl())ABORT("expecting gen lvl to become: true");
        
        NewLevel = Level[Index] = new level;
        NewLevel->SetDungeon(this);
        NewLevel->SetIndex(Index);
        const levelscript* LevelScript = GetLevelScript(Index);
        NewLevel->SetLevelScript(LevelScript);
        DBG7("GeneratingDungeonLevel",NewLevel->GetDungeon()->GetIndex(),Index,fsDL.CStr(),fsGenLoopDL.CStr(),i,NewLevel->GetDungeon()->GetLevelDescription(Index, true).CStr());

        if(Visual)
        {
          if(LevelScript->GetEnterImage())
          {
            EnterImage = new bitmap(game::GetDataDir() + "Graphics/" + *LevelScript->GetEnterImage());
            game::SetEnterImage(EnterImage);
            v2 Displacement = *LevelScript->GetEnterTextDisplacement();
            game::SetEnterTextDisplacement(Displacement);
            game::TextScreen(CONST_S("Entering ") + GetLevelDescription(Index)
                             + CONST_S("...\n\nThis may take some time, please wait."),
                             Displacement, WHITE, false, 
                             true, &game::BusyAnimation);
            game::TextScreen(CONST_S("Entering ") + GetLevelDescription(Index)
                             + CONST_S("...\n\nPress any key to continue."),
                             Displacement, WHITE, game::GetAutoPlayMode()<2, 
                             false, &game::BusyAnimation);
            game::SetEnterImage(0);
            delete EnterImage;
            EnterImage=NULL;
          }
          else
          {
            game::SetEnterTextDisplacement(ZERO_V2);
            game::TextScreen(CONST_S("Entering ") + GetLevelDescription(Index)
                             + CONST_S("...\n\nThis may take some time, please wait."),
                             ZERO_V2, WHITE, false, true, &game::BusyAnimation);
          }
        }

        NewLevel->Generate(Index);
        game::SetCurrentLSquareMap(NewLevel->GetMap());
        Generated[Index] = true;
        game::BusyAnimation();

        if(*NewLevel->GetLevelScript()->GenerateMonsters())
          NewLevel->GenerateNewMonsters(NewLevel->GetIdealPopulation(), false);

        if(genericException::ToggleGenNewLvl())ABORT("expecting gen lvl to become: false");
        if(fsGenLoopDL!=fsDL)
          return false; // new level is ok
      }catch(const genericException& e){
        // cleanup
        //TODO it is not working well, memory usage keeps increasing...
        if(NewLevel  ){delete NewLevel  ;NewLevel=NULL;}
        if(EnterImage){delete EnterImage;EnterImage=NULL;}
        
        //retry
      }
    } //for()
    if(fsGenLoopDL==fsDL)ABORT("Generating dungeon loop test completed.");
    ABORT("Generating new level failed after %d retries, aborting...",iRetryMax);
  }
}

void dungeon::PrepareMusic(int Index)
{
  const levelscript* LevelScript = GetLevelScript(Index);
  bool hasCurrentTrack = false;
  festring CurrentTrack = audio::GetCurrentlyPlayedFile();

  for( int i = 0; i < LevelScript->GetAudioPlayList()->Size; ++i  )
  {
     festring Music = LevelScript->GetAudioPlayList()->Data[i];
     if( CurrentTrack == Music )
     {
        hasCurrentTrack = true;
        break;
     }
  }

  if( hasCurrentTrack == true )
  {
     audio::ClearMIDIPlaylist(CurrentTrack);
     for( int i = 0; i < LevelScript->GetAudioPlayList()->Size; ++i  )
     {
        festring Music = LevelScript->GetAudioPlayList()->Data[i];
        if( CurrentTrack == Music )
        {
        }
        else
        {
           audio::LoadMIDIFile(Music, 0, 100);
        }
     }
  }

  if( hasCurrentTrack == false )
  {
     audio::SetPlaybackStatus(audio::STOPPED);
     audio::ClearMIDIPlaylist();
     for( int i = 0; i < LevelScript->GetAudioPlayList()->Size; ++i  )
     {
        festring Music = LevelScript->GetAudioPlayList()->Data[i];
        audio::LoadMIDIFile(Music, 0, 100);
     }
     audio::SetPlaybackStatus(audio::PLAYING);
  }




}

void dungeon::SaveLevel(cfestring& SaveName, int Number, truth DeleteAfterwards)
{
  outputfile SaveFile(SaveName + '.' + Index + Number);
  SaveFile << Level[Number];

  if(DeleteAfterwards)
  {
    delete Level[Number];
    Level[Number] = 0;
  }
}

level* dungeon::LoadLevel(cfestring& SaveName, int Number)
{
  inputfile SaveFile(SaveName + '.' + Index + Number);
  return LoadLevel(SaveFile, Number);
}

void dungeon::Save(outputfile& SaveFile) const
{
  SaveFile << Index << WorldMapPos;

  for(int c = 0; c < GetLevels(); ++c)
    SaveFile << Generated[c];
}

void dungeon::Load(inputfile& SaveFile)
{
  SaveFile >> Index >> WorldMapPos;
  Initialize();

  for(int c = 0; c < GetLevels(); ++c)
    SaveFile >> Generated[c];
}

int dungeon::GetLevels() const
{
  return *DungeonScript->GetLevels();
}

festring dungeon::GetLevelDescription(int I,bool bPretty)
{
  if(GetLevel(I)->GetLevelScript()->GetDescription()){
    return *GetLevel(I)->GetLevelScript()->GetDescription();
  }else{
    festring fs = *DungeonScript->GetDescription();
    int i = I+1;

    if(bPretty){
      festring fsRoman; // roman numbers
      const char* X[] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
      fsRoman << X[(i%100)/10];
      const char* I[] = {"","I","II","III","IV","V","VI","VII","VIII","IX"};
      fsRoman << I[(i%10)];

      return fs + " " + fsRoman;
    }else{
      return fs + " level " + i;
    }
  }
}

festring dungeon::GetShortLevelDescription(int I)
{
  if(GetLevel(I)->GetLevelScript()->GetShortDescription())
    return *GetLevel(I)->GetLevelScript()->GetShortDescription();
  else
    return *DungeonScript->GetShortDescription() + " lvl " + (I + 1);
}

outputfile& operator<<(outputfile& SaveFile, const dungeon* Dungeon)
{
  if(Dungeon)
  {
    SaveFile.Put(1);
    Dungeon->Save(SaveFile);
  }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dungeon*& Dungeon)
{
  if(SaveFile.Get())
  {
    Dungeon = new dungeon;
    Dungeon->Load(SaveFile);
  }

  return SaveFile;
}

level* dungeon::LoadLevel(inputfile& SaveFile, int Number)
{
  SaveFile >> Level[Number];
  Level[Number]->SetDungeon(this);
  Level[Number]->SetIndex(Number);
  Level[Number]->SetLevelScript(GetLevelScript(Number));
  PrepareMusic(Number);

  return Level[Number];
}

int dungeon::GetLevelTeleportDestination(int From) const
{
  int To;

  if(Index == ELPURI_CAVE)
  {
    if(RAND_2)
    {
      To = From + RAND_2 + RAND_2 + RAND_2 + RAND_2 + 1;

      if(To > DARK_LEVEL)
        To = From;
    }
    else
    {
      To = From - RAND_2 - RAND_2 - RAND_2 - RAND_2 - 1;

      if(To < 0)
        To = 0;
    }

    return To;
  }

  if(Index == UNDER_WATER_TUNNEL)
    return RAND_N(3);

  return From;
}
