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

#ifndef __COLORBIT_H__
#define __COLORBIT_H__

#include <map>
#include <vector>

#include "v2.h"

class outputfile;
class inputfile;
class bitmap;
class cachedfont;
class festring;

typedef std::map<col16, std::pair<cachedfont*, cachedfont*> > fontcache;

class rawbitmap
{
 public:
  rawbitmap(cfestring&);
  rawbitmap(v2);
  ~rawbitmap();
  void Save(cfestring&);
  void MaskedBlit(bitmap*, v2, v2,
		  v2, packcol16*) const;
  void MaskedBlit(bitmap*, packcol16*) const;

  void LIKE_PRINTF(5, 6) Printf(bitmap*, v2, packcol16,
				cchar*, ...) const;
  void LIKE_PRINTF(5, 6) PrintfUnshaded(bitmap*, v2, packcol16,
					cchar*, ...) const;
  cachedfont* Colorize(cpackcol16*, alpha = 255,
		       cpackalpha* = 0) const;
  bitmap* Colorize(v2, v2, v2,
		   cpackcol16*, alpha = 255,
		   cpackalpha* = 0,
		   cuchar* = 0, truth = true) const;
  v2 GetSize() const { return Size; }

  void AlterGradient(v2, v2, int, int, truth);
  void SwapColors(v2, v2, int, int);
  void Roll(v2, v2, v2, paletteindex*);

  void CreateFontCache(packcol16);
  static truth IsMaterialColor(int Color) { return Color >= 192; }
  static int GetMaterialColorIndex(int Color) { return Color - 192 >> 4; }
  int GetMaterialColorIndex(int X, int Y) const
  { return PaletteBuffer[Y][X] - 192 >> 4; }
  truth IsTransparent(v2) const;
  truth IsMaterialColor1(v2) const;
  v2 RandomizeSparklePos(cv2*, v2*, v2, v2, int, int) const;
  void CopyPaletteFrom(rawbitmap*);
  void PutPixel(v2 Pos, paletteindex Color)
  { PaletteBuffer[Pos.Y][Pos.X] = Color; }
  paletteindex GetPixel(v2 Pos) const
  { return PaletteBuffer[Pos.Y][Pos.X]; }
  void Clear();
  void NormalBlit(rawbitmap*, v2, v2, v2, int = 0) const;
 protected:
  v2 Size;
  uchar* Palette;
  paletteindex** PaletteBuffer;
  fontcache FontCache;
};

#endif
