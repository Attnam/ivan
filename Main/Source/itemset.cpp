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

#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "item.h"
#include "database.h"

EXTENDED_SYSTEM_SPECIALIZATIONS(item)(0, 0, 0, "item");

#include "bodypart.h"
#include "gear.h"
#include "miscitem.h"

#undef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include <algorithm>
#include <ctime>
#include <cmath>

#include "balance.h"
#include "bitmap.h"
#include "char.h"
#include "confdef.h"
#include "felist.h"
#include "fluid.h"
#include "game.h"
#include "god.h"
#include "human.h"
#include "iconf.h"
#include "lterras.h"
#include "materias.h"
#include "message.h"
#include "nonhuman.h"
#include "rawbit.h"
#include "room.h"
#include "save.h"
#include "smoke.h"
#include "stack.h"
#include "team.h"
#include "whandler.h"
#include "wizautoplay.h"
#include "worldmap.h"
#include "wterras.h"

#include "item.cpp"
#include "bodypart.cpp"
#include "gear.cpp"
#include "miscitem.cpp"
