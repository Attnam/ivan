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

#define __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "char.h"
#include "database.h"

EXTENDED_SYSTEM_SPECIALIZATIONS(character)(0, 0, 0, "character");

#include "human.h"
#include "nonhuman.h"

#undef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include <algorithm>
#include <queue>
#include <cctype>

#include "actions.h"
#include "balance.h"
#include "bitmap.h"
#include "bodypart.h"
#include "command.h"
#include "confdef.h"
#include "curseddeveloper.h"
#include "error.h"
#include "felist.h"
#include "fluid.h"
#include "game.h"
#include "god.h"
#include "gods.h"
#include "graphics.h"
#include "hiteffect.h"
#include "hscore.h"
#include "iconf.h"
#include "iloops.h"
#include "ivandef.h"
#include "lterras.h"
#include "materias.h"
#include "message.h"
#include "miscitem.h"
#include "rawbit.h"
#include "room.h"
#include "save.h"
#include "stack.h"
#include "team.h"
#include "traps.h"
#include "whandler.h"
#include "wsquare.h"

#include "team.cpp"
#include "char.cpp"
#include "curseddeveloper.cpp"
