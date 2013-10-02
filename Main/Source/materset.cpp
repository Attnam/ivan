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

#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "materia.h"
#include "database.h"

EXTENDED_SYSTEM_SPECIALIZATIONS(material)(0, 0, 0, "material");

#include "materias.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "char.h"
#include "confdef.h"
#include "message.h"
#include "save.h"
#include "fluid.h"
#include "smoke.h"
#include "bitmap.h"
#include "game.h"
#include "rawbit.h"
#include "whandler.h"
#include "rain.h"

#include "materia.cpp"
#include "materias.cpp"
#include "fluid.cpp"
#include "smoke.cpp"
#include "rain.cpp"
