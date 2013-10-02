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

#include "team.h"
#include "error.h"
#include "game.h"
#include "message.h"
#include "save.h"
#include "stack.h"
#include "wsquare.h"
#include "actions.h"
#include "iconf.h"
#include "whandler.h"
#include "hscore.h"
#include "god.h"
#include "command.h"
#include "materias.h"
#include "room.h"
#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "rawbit.h"
#include "miscitem.h"
#include "confdef.h"
#include "traps.h"
#include "iloops.h"
#include "balance.h"

#include "team.cpp"
#include "char.cpp"
