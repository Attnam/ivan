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

#include "config.h"
#include "save.h"
#include "felist.h"
#include "feio.h"

configoption* configsystem::Option[MAX_CONFIG_OPTIONS];
festring configsystem::ConfigFileName;
int configsystem::Options;

void configsystem::AddOption(festring fsCategory, configoption* O) {
  for(int i=0;i<Options;i++)if(Option[i] == O)ABORT("Option already set '%s' '%s'", O->Name, O->Description); //help developers to prevent duplicated entries

  O->fsCategory=fsCategory;
  Option[Options++] = O;
}
void configsystem::NormalStringChanger(stringoption* O, cfestring& What)
{ O->Value = What; }
void configsystem::NormalNumberChanger(numberoption* O, long What)
{ O->Value = What; }
void configsystem::NormalTruthChanger(truthoption* O, truth What)
{ O->Value = What; }
void configsystem::NormalCycleChanger(cycleoption* O, long What)
{ O->Value = What; }

configoption::configoption(cchar* Name, cchar* Description, cchar* HelpInfo)
: Name(Name), Description(Description), HelpInfo(HelpInfo) { }

stringoption::stringoption(cchar* Name, cchar* Desc,
                           cchar* HelpInfo, cfestring& Value,
                           void (*ValueDisplayer)(const stringoption*,
                                                  festring&),
                           truth (*ChangeInterface)(stringoption*),
                           void (*ValueChanger)(stringoption*,
                                                cfestring&))
: configoption(Name, Desc, HelpInfo),
  Value(Value), ValueDisplayer(ValueDisplayer),
  ChangeInterface(ChangeInterface),
  ValueChanger(ValueChanger) { }

numberoption::numberoption(cchar* Name, cchar* Desc, cchar* HelpInfo, long Value,
                           void (*ValueDisplayer)(const numberoption*,
                                                  festring&),
                           truth (*ChangeInterface)(numberoption*),
                           void (*ValueChanger)(numberoption*, long))
: configoption(Name, Desc, HelpInfo),
  Value(Value), ValueDisplayer(ValueDisplayer),
  ChangeInterface(ChangeInterface),
  ValueChanger(ValueChanger) { }

scrollbaroption::scrollbaroption(cchar* Name, cchar* Desc, cchar* HelpInfo, long Value,
                                 void (*ValueDisplayer)(const numberoption*,
                                                        festring&),
                                 truth (*ChangeInterface)(numberoption*),
                                 void (*ValueChanger)(numberoption*, long),
                                 void (*BarHandler)(long))
: numberoption(Name, Desc, HelpInfo, Value, ValueDisplayer,
               ChangeInterface, ValueChanger),
  BarHandler(BarHandler) { }

truthoption::truthoption(cchar* Name, cchar* Desc, cchar* HelpInfo, truth Value,
                         void (*ValueDisplayer)(const truthoption*, festring&),
                         truth (*ChangeInterface)(truthoption*),
                         void (*ValueChanger)(truthoption*, truth))
: configoption(Name, Desc, HelpInfo),
  Value(Value), ValueDisplayer(ValueDisplayer),
  ChangeInterface(ChangeInterface),
  ValueChanger(ValueChanger) { }

cycleoption::cycleoption(cchar* Name, cchar* Desc, cchar* HelpInfo,
                           long Value, long CycleCount,
                           void (*ValueDisplayer)(const cycleoption*,
                                                  festring&),
                           truth (*ChangeInterface)(cycleoption*),
                           void (*ValueChanger)(cycleoption*,
                                                long))
: configoption(Name, Desc, HelpInfo),
  Value(Value), CycleCount(CycleCount),
  ValueDisplayer(ValueDisplayer),
  ChangeInterface(ChangeInterface),
  ValueChanger(ValueChanger) { }

truth configsystem::Save()
{
  std::ofstream SaveFile(ConfigFileName.CStr(), std::ios::out);

  if(!SaveFile.is_open())
    return false;

  for(int c = 0; c < Options; ++c)
  {
    SaveFile << Option[c]->Name << " = ";
    Option[c]->SaveValue(SaveFile);
    SaveFile << ";\n";
  }

  return true;
}

truth configsystem::Load()
{
  inputfile SaveFile(ConfigFileName, 0, false);

  if(!SaveFile.IsOpen())
    return false;

  festring Word;

  for(SaveFile.ReadWord(Word, false);
      !SaveFile.Eof();
      SaveFile.ReadWord(Word, false))
  {
    /* Inefficient, but speed is probably not an issue here */

    for(int c = 0; c < Options; ++c)
      if(Word == Option[c]->Name)
        Option[c]->LoadValue(SaveFile);
  }

  return true;
}

void configsystem::Show(void (*BackGroundDrawer)(),
                        void (*ListAttributeInitializer)(felist&),
                        truth SlaveScreen)
{
  int Chosen;
  truth TruthChange = false;

  felist List(CONST_S("Which setting do you wish to configure? (* requires restart)"));

  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Setting                                                        Value"));

  for(;;)
  {
    if(SlaveScreen)
      BackGroundDrawer();

    List.Empty();

    festring fsLastCategory;
    for(int c = 0; c < Options; ++c)
    {
      festring Entry = Option[c]->Description;
      Entry.Capitalize();
      int iLim=60;
      if(Entry.GetSize()>iLim-1){
        Entry.Resize(iLim-4);
        Entry<<"...";
      }else
        Entry.Resize(iLim-1);
      Entry<<" "; //space between "columns"
      Option[c]->DisplayValue(Entry);
      Entry.Resize(iLim+30);

      if(fsLastCategory!=Option[c]->fsCategory){
        List.AddEntry(Option[c]->fsCategory, WHITE, 0, NO_IMAGE, false);
        fsLastCategory=Option[c]->fsCategory;
      }

      List.AddEntry(Entry, LIGHT_GRAY);
      // TODO: help should show all possible values with details, may require cycling thru them
      List.SetLastEntryHelp(festring() << Option[c]->Description << "\n\n" << Option[c]->HelpInfo);
    }

    if(SlaveScreen && ListAttributeInitializer)
      ListAttributeInitializer(List);

//    List.SetFlags(SELECTABLE|(SlaveScreen ? DRAW_BACKGROUND_AFTERWARDS : 0)
//                  |(!SlaveScreen && !TruthChange ? FADE : 0));
    List.SetFlags(SELECTABLE|(SlaveScreen ? DRAW_BACKGROUND_AFTERWARDS : 0)|(!SlaveScreen ? FADE : 0));
    List.SetFirstDrawNoFade(TruthChange); //ignored if not fading
    Chosen = List.Draw();
    festring String;

    if(Chosen < Options)
      TruthChange = Option[Chosen]->ActivateChangeInterface();
    else
    {
      TruthChange=false;
      Save();
      return;
    }
  }
}

void configsystem::NormalStringDisplayer(const stringoption* O,
                                         festring& Entry)
{
  if(!O->Value.IsEmpty())
    Entry << O->Value;
  else
    Entry << '-';
}

void configsystem::NormalNumberDisplayer(const numberoption* O,
                                         festring& Entry)
{
  Entry << O->Value;
}

void configsystem::NormalTruthDisplayer(const truthoption* O,
                                        festring& Entry)
{
  Entry << (O->Value ? "yes" : "no");
}

void configsystem::NormalCycleDisplayer(const cycleoption* O,
                                         festring& Entry)
{
  Entry << O->Value;
}

truth configsystem::NormalTruthChangeInterface(truthoption* O)
{
  O->ChangeValue(!O->Value);
  return true;
}

truth configsystem::NormalStringChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new ")
                              + O->Description + ':',
                              v2(30, 30), WHITE, 0, 80,
                              true, true) == NORMAL_EXIT)
    O->ChangeValue(String);

  return false;
}

truth configsystem::NormalNumberChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new ")
                                          + O->Description + ':',
                                          v2(30, 30), WHITE, true));
  return false;
}

truth configsystem::NormalCycleChangeInterface(cycleoption* O)
{
  O->ChangeValue((O->Value + 1) % O->CycleCount);
  return true;
}
void stringoption::SaveValue(std::ofstream& SaveFile) const
{
  festring String;
  String << Value;
  SEARCH_N_REPLACE(String, "\"", "\\\"");
  SaveFile << '\"' << String.CStr() << '\"';
}

void stringoption::LoadValue(inputfile& SaveFile)
{
  SaveFile.ReadWord();
  SaveFile.ReadWord(Value);
}

/* ??? */

void numberoption::SaveValue(std::ofstream& SaveFile) const
{ SaveFile << Value; }
void numberoption::LoadValue(inputfile& SaveFile)
{ Value = SaveFile.ReadNumber(); }
void truthoption::SaveValue(std::ofstream& SaveFile) const
{ SaveFile << Value; }
void truthoption::LoadValue(inputfile& SaveFile)
{ Value = SaveFile.ReadNumber(); }

void cycleoption::SaveValue(std::ofstream& SaveFile) const
{ SaveFile << Value; }
void cycleoption::LoadValue(inputfile& SaveFile)
{ Value = SaveFile.ReadNumber(); }
