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

#include "char.h"
#include "message.h"
#include "stack.h"
#include "felist.h"
#include "confdef.h"
#include "room.h"
#include "game.h"
#include "materias.h"
#include "human.h"
#include "team.h"
#include "god.h"
#include "team.h"
#include "smoke.h"
#include "save.h"
#include "whandler.h"
#include "bitmap.h"
#include "fluid.h"
#include "rawbit.h"
#include "balance.h"

#include "item.cpp"
#include "bodypart.cpp"
#include "gear.cpp"
#include "miscitem.cpp"
