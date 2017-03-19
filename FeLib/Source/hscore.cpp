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

#include <curl/curl.h>

#include "hscore.h"
#include "save.h"
#include "felist.h"
#include "feio.h"
#include "femath.h"

static truth RetrieveHighScoresFromServer(cfestring&,
                                          std::vector<festring>&,
                                          std::vector<long>&,
                                          std::vector<time_t>&);
static void SubmitHighScoreToServer(cfestring&, cfestring&, cfestring&,
                                    long, cfestring&, time_t, long);

/* Increment this if changes make highscores incompatible */
#define HIGH_SCORE_VERSION 128

highscoreview highscore::View = LOCAL;

cfestring& highscore::GetEntry(int I) const { return Entry[I]; }
long highscore::GetScore(int I) const { return Score[I]; }
long highscore::GetSize() const { return Entry.size(); }

highscore::highscore(cfestring& File) : LastAdd(0xFF), Version(HIGH_SCORE_VERSION) { Load(File); }

truth highscore::Add(long NewScore, cfestring& NewEntry, time_t NewTime,
                     long NewRandomID, cfestring& HighScoreServerURL,
                     cfestring& HighScoreServerUsername,
                     cfestring& HighScoreServerPassword)
{
  if (!HighScoreServerURL.IsEmpty())
    SubmitHighScoreToServer(HighScoreServerURL, HighScoreServerUsername,
                            HighScoreServerPassword, NewScore, NewEntry,
                            NewTime, NewRandomID);

  for(uint c = 0; c < Score.size(); ++c)
    if(Score[c] < NewScore)
    {
      Entry.insert(Entry.begin() + c, NewEntry);
      Score.insert(Score.begin() + c, NewScore);
      Time.insert(Time.begin() + c, NewTime);
      RandomID.insert(RandomID.begin() + c, NewRandomID);

      if(Score.size() > MAX_HIGHSCORES)
      {
        Entry.resize(MAX_HIGHSCORES, festring());
        Score.resize(MAX_HIGHSCORES);
        Time.resize(MAX_HIGHSCORES);
        RandomID.resize(MAX_HIGHSCORES);
      }

      LastAdd = c;
      return true;
    }

  if(Score.size() < MAX_HIGHSCORES)
  {
    LastAdd = Score.size();
    Entry.push_back(NewEntry);
    Score.push_back(NewScore);
    Time.push_back(NewTime);
    RandomID.push_back(NewRandomID);
    return true;
  }
  else
  {
    LastAdd = MAX_HIGHSCORES;
    return false;
  }
}

void highscore::Draw(cfestring& HighScoreServerURL)
{
  if(Score.empty())
  {
    iosystem::TextScreen(CONST_S("There are no entries yet. "
                                 "Play a game to correct this."));
    return;
  }

  if(GetVersion() != HIGH_SCORE_VERSION)
  {
    iosystem::TextScreen(CONST_S("The highscore file is for another version of IVAN."));
    return;
  }

  for(;;)
  {
    festring Title;
    if(View == LOCAL)
      Title = CONST_S("Adventurers' Hall of Fame                                "
                      "Press ENTER to view global highscores");
    else if(View == GLOBAL)
      Title = CONST_S("Global Adventurers' Hall of Fame                          "
                      "Press ENTER to view local highscores");

    felist List(Title);
    festring Desc;

    if(View == LOCAL)
      for(uint c = 0; c < Score.size(); ++c)
      {
        Desc.Empty();
        Desc << c + 1;
        Desc.Resize(5, ' ');
        Desc << Score[c];
        Desc.Resize(13, ' ');
        Desc << Entry[c];
        List.AddEntry(Desc, c == uint(LastAdd) ? WHITE : LIGHT_GRAY, 13);
      }
    else if(View == GLOBAL)
    {
      RetrieveHighScoresFromServer(HighScoreServerURL, GlobalEntry,
                                   GlobalScore, GlobalTime);

      for(uint c = 0; c < GlobalScore.size(); ++c)
      {
        Desc.Empty();
        Desc << c + 1;
        Desc.Resize(5, ' ');
        Desc << GlobalScore[c];
        Desc.Resize(13, ' ');
        Desc << GlobalEntry[c];
        List.AddEntry(Desc, LIGHT_GRAY, 13);
      }
    }

    List.SetFlags(FADE);
    List.SetPageLength(40);

    cuint DrawResult = List.Draw();

    if(DrawResult == UNSELECTABLE_SELECT) // Enter was pressed.
      ToggleBetweenLocalAndGlobalView();
    else if(DrawResult == LIST_WAS_EMPTY)
    {
      iosystem::TextScreen(CONST_S("Couldn't fetch global highscores from the server."));
      View = LOCAL;
    }
    else
      break;
  }
}

void highscore::Save(cfestring& File) const
{
  outputfile HighScore(File);
  long CheckSum = HIGH_SCORE_VERSION + LastAdd;
  for(ushort c = 0; c < Score.size(); ++c)
  {
    CheckSum += Score[c] + Entry[c].GetCheckSum() + RandomID[c];
  }

  HighScore << ushort(HIGH_SCORE_VERSION) << Score
            << Entry << Time << RandomID << LastAdd << CheckSum;
}

/* This function needs much more error handling */
void highscore::Load(cfestring& File)
{
  {
    inputfile HighScore(File, 0, false);

    if(!HighScore.IsOpen())
      return;

    HighScore.Get();

    if(HighScore.Eof())
      return;
  }

  inputfile HighScore(File, 0, false);
  HighScore >> Version;
  HighScore >> Score >> Entry >> Time >> RandomID >> LastAdd;
}

truth highscore::MergeToFile(highscore* To) const
{
  truth MergedSomething = false;

  for(uint c = 0; c < Score.size(); ++c)
    if(!To->Find(Score[c], Entry[c], Time[c], RandomID[c]))
    {
      To->Add(Score[c], Entry[c], Time[c], RandomID[c], "", "", "");
      MergedSomething = true;
    }

  return MergedSomething;
}

truth highscore::Add(long NewScore, cfestring& NewEntry,
                     cfestring& HighScoreServerURL,
                     cfestring& HighScoreServerUsername,
                     cfestring& HighScoreServerPassword)
{
  return Add(NewScore, NewEntry, time(0), RAND(), HighScoreServerURL,
             HighScoreServerUsername, HighScoreServerPassword);
}

/* Because of major stupidity this return the number of NEXT
   from the right entry, 0 = not found */

int highscore::Find(long AScore, cfestring& AEntry,
                    time_t ATime, long ARandomID)
{
  for(uint c = 0; c < Score.size(); ++c)
  {
    if(AScore == Score[c] && Entry[c] == AEntry
       && ATime == Time[c] && ARandomID == RandomID[c])
      return c + 1;
  }

  return 0;
}

truth highscore::LastAddFailed() const
{ return LastAdd == MAX_HIGHSCORES; }

void highscore::Clear()
{
  Entry.clear();
  Score.clear();
  Time.clear();
  RandomID.clear();
  Version = HIGH_SCORE_VERSION;
  LastAdd = 0xFF;
}

truth highscore::CheckVersion() const
{
  return Version == HIGH_SCORE_VERSION;
}

static void ParseHighScoresFromCSV(cfestring& CSV,
                                   std::vector<festring>& GlobalEntry,
                                   std::vector<long>& GlobalScore,
                                   std::vector<time_t>& GlobalTime)
{
  for(festring::sizetype LastIndex = 0;;)
  {
    festring::csizetype ScoreIndex = CSV.Find("\n", LastIndex) + 1;
    if(!ScoreIndex || ScoreIndex == festring::NPos) return;
    GlobalScore.push_back(atol(CSV.CStr() + ScoreIndex));
    festring::csizetype EntryIndex = CSV.Find("\"", ScoreIndex) + 1;
    festring::csizetype EntryLength = CSV.Find("\"", EntryIndex) - EntryIndex;
    GlobalEntry.push_back(festring(CSV.CStr() + EntryIndex, EntryLength) + '\0');
    GlobalTime.push_back(0); // TODO
    LastIndex = EntryIndex + EntryLength;
  }
}

static size_t WriteMemoryCallback(char* Contents, size_t Size, size_t Count, void* UserData)
{
  const size_t RealSize = Size * Count;
  *static_cast<festring*>(UserData) << cfestring(Contents, RealSize);
  return RealSize;
}

static truth RetrieveHighScoresFromServer(cfestring& HighScoreServerURL,
                                          std::vector<festring>& GlobalEntry,
                                          std::vector<long>& GlobalScore,
                                          std::vector<time_t>& GlobalTime)
{
  if(curl_global_init(CURL_GLOBAL_ALL) != 0)
    return false;

  truth Success = false;

  if(CURL* Curl = curl_easy_init())
  {
    festring RetrievedData;
    festring HighScoreRetrieveURL = HighScoreServerURL + "/highscores";

    curl_easy_setopt(Curl, CURLOPT_URL, HighScoreRetrieveURL.CStr());
    curl_easy_setopt(Curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, &WriteMemoryCallback);
    curl_easy_setopt(Curl, CURLOPT_WRITEDATA, &RetrievedData);
    CURLcode Res = curl_easy_perform(Curl);
    long ResponseCode;
    curl_easy_getinfo(Curl, CURLINFO_RESPONSE_CODE, &ResponseCode);

    if(Res == CURLE_OK && ResponseCode == 200)
    {
      GlobalEntry.clear();
      GlobalScore.clear();
      GlobalTime.clear();
      ParseHighScoresFromCSV(RetrievedData, GlobalEntry, GlobalScore, GlobalTime);
      Success = true;
    }

    curl_easy_cleanup(Curl);
  }

  curl_global_cleanup();
  return Success;
}

static void SubmitHighScoreToServer(cfestring& HighScoreServerURL,
                                    cfestring& HighScoreServerUsername,
                                    cfestring& HighScoreServerPassword,
                                    long NewScore, cfestring& NewEntry,
                                    time_t NewTime, long NewRandomID)
{
  if(curl_global_init(CURL_GLOBAL_ALL) != 0)
    return;

  if(CURL* Curl = curl_easy_init())
  {
    curl_slist* const Headers = curl_slist_append(nullptr, "Content-Type: application/json");

    festring Json;
    Json <<
    "{"
      "\"username\": \"" << HighScoreServerUsername << "\","
      "\"password\": \"" << HighScoreServerPassword << "\","
      "\"score\": " << NewScore << ","
      "\"entry\": \"" << NewEntry << "\""
    "}";

    festring HighScoreSubmitURL = HighScoreServerURL + "/submit_score";

    curl_easy_setopt(Curl, CURLOPT_URL, HighScoreSubmitURL.CStr());
    curl_easy_setopt(Curl, CURLOPT_POST, 1);
    curl_easy_setopt(Curl, CURLOPT_HTTPHEADER, Headers);
    curl_easy_setopt(Curl, CURLOPT_POSTFIELDS, Json.CStr());

    curl_easy_perform(Curl);

    curl_slist_free_all(Headers);
    curl_easy_cleanup(Curl);
  }

  curl_global_cleanup();
}
