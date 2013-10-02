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

#ifndef __LIST_H__
#define __LIST_H__

#include <vector>

#include "v2.h"

class outputfile;
class inputfile;
class rawbitmap;
class bitmap;
class festring;
struct felistentry;
struct felistdescription;

typedef void (*entrydrawer)(bitmap*, v2, uint);

class felist
{
 public:
  felist(cfestring&, col16 = WHITE, uint = 0);
  ~felist();
  void AddEntry(cfestring&, col16, uint = 0,
		uint = NO_IMAGE, truth = true);
  void AddDescription(cfestring&, col16 = WHITE);
  uint Draw();
  void QuickDraw(bitmap*, uint) const;
  void Empty();
  void EmptyDescription();
  festring GetEntry(uint) const;
  col16 GetColor(uint) const;
  void SetColor(uint, col16);
  uint GetLength() const;
  uint GetLastEntryIndex() const;
  void Load(inputfile&);
  void Save(outputfile&) const;
  truth IsEmpty() const;
  uint GetSelected() const { return Selected; }
  void SetSelected(uint What) { Selected = What; }
  void EditSelected(int What) { Selected += What; }
  truth DrawPage(bitmap*) const;
  void Pop();
  static void CreateQuickDrawFontCaches(rawbitmap*, col16, uint);
  void PrintToFile(cfestring&);
  void SetPos(v2 What) { Pos = What; }
  void SetWidth(uint What) { Width = What; }
  void SetPageLength(uint What) { PageLength = What; }
  void SetBackColor(col16 What) { BackColor = What; }
  void SetFlags(uint What) { Flags = What; }
  void AddFlags(uint What) { Flags |= What; }
  void RemoveFlags(uint What) { Flags &= ~What; }
  void SetUpKey(uint What) { UpKey = What; }
  void SetDownKey(uint What) { DownKey = What; }
  void SetEntryDrawer(entrydrawer What) { EntryDrawer = What; }
 private:
  void DrawDescription(bitmap*) const;
  std::vector<felistentry*> Entry;
  std::vector<felistdescription*> Description;
  uint PageBegin;
  uint Maximum;
  uint Selected;
  v2 Pos;
  uint Width;
  uint PageLength;
  col16 BackColor;
  uint Flags;
  uint UpKey;
  uint DownKey;
  entrydrawer EntryDrawer;
};

#endif
