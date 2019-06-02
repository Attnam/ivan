// ****************************************************************************
// * This file is part of the HqMAME project. It is distributed under         *
// * GNU General Public License: http://www.gnu.org/licenses/gpl-3.0          *
// * Copyright (C) Zenju (zenju AT gmx DOT de) - All Rights Reserved          *
// ****************************************************************************

#ifndef XBRZ_CONFIG_HEADER_284578425345
#define XBRZ_CONFIG_HEADER_284578425345

//do NOT include any headers here! used by xBRZ_dll!!!

namespace xbrz
{
struct ScalerCfg
{
    double luminanceWeight            = 1;
    double equalColorTolerance        = 30;
    double dominantDirectionThreshold = 3.6;
    double steepDirectionThreshold    = 2.2;
    double newTestAttribute           = 0; //unused; test new parameters
};
}

#endif
