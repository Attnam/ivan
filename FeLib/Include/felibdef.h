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

#ifndef __FELIBDEF_H__
#define __FELIBDEF_H__

/*
 * Global defines for the project FeLib.
 * This file is created to decrease the need of including headers in
 * other headers just for the sake of some silly macros, because it
 * decreases compilation efficiency and may cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple .cpp or .h files
 * 2. all inline functions used in multiple .cpp or .h files
 *    and independent enough (do not require other headers)
 * 3. class construction macros used in multiple .h files
 */

#include "typedef.h"

cint MapMoveX[9] = { -1, 0, 1, -1, 1, -1, 0, 1, 0 };
cint MapMoveY[9] = { -1, -1, -1, 0, 0, 1, 1, 1, 0 };

culong SquarePartTickMask[4] = { 0xFF, 0xFF00, 0xFF0000, 0xFF000000 };

#define FPI 3.1415926535897932384626433832795

/* Btw, both __attribute__ ((regparm(3))) and __fastcall SUCK! */

#ifdef GCC
#define NO_ALIGNMENT __attribute__ ((packed))
#define NO_RETURN __attribute__ ((noreturn))
#define LIKE_PRINTF(p1, p2) __attribute__ ((format(printf, p1, p2)))
#else
#define NO_ALIGNMENT
#define NO_RETURN
#define LIKE_PRINTF(p1, p2)
#endif

template <class type>
inline type Max(type X, type Y) { return X >= Y ? X : Y; }

template <class type>
inline type Max(type X, type Y, type Z)
{ return X >= Y ? (X >= Z ? X : Z) : (Y >= Z ? Y : Z); }

template <class type>
inline type Min(type X, type Y) { return X <= Y ? X : Y; }

template <class type>
inline type Min(type X, type Y, type Z)
{ return X <= Y ? (X <= Z ? X : Z) : (Y <= Z ? Y : Z); }

template <class type>
inline type HypotSquare(type X, type Y) { return X * X + Y * Y; }

template <class type>
inline type Limit(type Value, type Minimum, type Maximum)
{ return Value >= Minimum ? Value <= Maximum ? Value : Maximum : Minimum; }

template <class type>
inline void LimitRef(type& Value, type Minimum, type Maximum)
{
  if(Value <= Minimum)
    Value = Minimum;
  else if(Value >= Maximum)
    Value = Maximum;
}

template <class type>
inline void Swap(type& X, type& Y)
{
  const type T = X;
  X = Y;
  Y = T;
}

inline col16 GetRed16(col16 Color) { return Color >> 8 & 0xF8; }
inline col16 GetGreen16(col16 Color) { return Color >> 3 & 0xFC; }
inline col16 GetBlue16(col16 Color) { return Color << 3 & 0xF8; }

inline col16 MakeRGB16(int Red, int Green, int Blue)
{
  return (Red << 8 & 0xF800) | (Green << 3 & 0x7E0) | (Blue >> 3 & 0x1F);
}

inline col16 MakeShadeColor(col16 Color)
{
  return MakeRGB16(GetRed16(Color) / 3,
		   GetGreen16(Color) / 3,
		   GetBlue16(Color) / 3);
}

inline col24 GetRed24(col24 Color) { return Color >> 16 & 0xFF; }
inline col24 GetGreen24(col24 Color) { return Color >> 8 & 0xFF; }
inline col24 GetBlue24(col24 Color) { return Color & 0xFF; }

inline col24 MakeRGB24(int Red, int Green, int Blue)
{
  return (Red << 16 & 0xFF0000) | (Green << 8 & 0xFF00) | (Blue & 0xFF);
}

inline int GetMaxColor24(col24 Color)
{
  return Max(GetRed24(Color), GetGreen24(Color), GetBlue24(Color));
}

inline int GetMinColor24(col24 Color)
{
  return Min(GetRed24(Color), GetGreen24(Color), GetBlue24(Color));
}

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE 4

#define TRANSPARENT_COLOR 0xF81F

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F

#define YELLOW 0xFFE0
#define PINK 0xF01E

#define WHITE 0xFFFF
#define LIGHT_GRAY 0x94B2
#define DARK_GRAY 0x528A
#define BLACK 0x0000

#define NORMAL_LUMINANCE 0x808080

#define KEY_BACK_SPACE 0x08
#define KEY_ESC 0x1B
#define KEY_ENTER 0x0D
#define KEY_UP 0x148
#define KEY_DOWN 0x150
#define KEY_RIGHT 0x14D
#define KEY_LEFT 0x14B
#define KEY_HOME 0x147
#define KEY_END 0x14F
#define KEY_PAGE_DOWN 0x151
#define KEY_PAGE_UP 0x149
#define KEY_SPACE ' '
#define KEY_NUMPAD_5 2

#define NO_FLAME 0xFFFF

#define SELECTABLE 1
#define INVERSE_MODE 2
#define BLIT_AFTERWARDS 4
#define DRAW_BACKGROUND_AFTERWARDS 8
#define FADE 16

/* felist errors */

#define FELIST_ERROR_BIT 0x8000
#define LIST_WAS_EMPTY 0xFFFF
#define ESCAPED 0xFFFE
#define NOTHING_SELECTED 0xFFFD

#define NO_LIMIT 0xFFFF

#define MAX_CONTROLS 0x10

#define HIGHEST 0xFF

#define NORMAL_EXIT 0
#define ABORTED 1

#define MAX_CONFIG_OPTIONS 0x100

#define FLY_PRIORITY ((10 << 4) + 10)
#define SPARKLE_PRIORITY ((12 << 4) + 12)
#define LIGHTNING_PRIORITY ((14 << 4) + 14)
#define AVERAGE_PRIORITY ((8 << 4) + 8)

#define NO_IMAGE 0xFFFF

#define ZERO_POOLS 1
#define RAND_ALLOC 2

#define REFS(ptr) reinterpret_cast<ulong*>(ptr)[-1]

#define SKIP_FIRST 1
#define ALLOW_END_FAILURE 2

#define MAX_RAND 0x7FFFFFFF

#define TRANSPARENT_PALETTE_INDEX 191

#define MAX_HIGHSCORES 100

/* sparkling flags */

#define SPARKLING_A 1
#define SPARKLING_B 2
#define SPARKLING_C 4
#define SPARKLING_D 8

#endif
