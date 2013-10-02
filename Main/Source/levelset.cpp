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

#define __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "lterra.h"
#include "database.h"

EXTENDED_SYSTEM_SPECIALIZATIONS(glterrain)(0, 0, "glterrain");
EXTENDED_SYSTEM_SPECIALIZATIONS(olterrain)(0, 0, "olterrain");

#include "lterras.h"

#undef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

#include <algorithm>
#include <ctime>

#include "char.h"
#include "team.h"
#include "action.h"
#include "message.h"
#include "stack.h"
#include "iconf.h"
#include "miscitem.h"
#include "room.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "god.h"
#include "bitmap.h"
#include "materias.h"
#include "confdef.h"
#include "fluid.h"
#include "nonhuman.h"
#include "smoke.h"
#include "save.h"
#include "allocate.h"
#include "whandler.h"
#include "rain.h"
#include "balance.h"

#include "level.cpp"
#include "lsquare.cpp"
#include "lterra.cpp"
#include "lterras.cpp"
