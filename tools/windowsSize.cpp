/*
 *  windowSize.cpp
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---today>  Wolfgang Westerboer
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------*/
#include "globals.h"


#if 0
		#define         defRunaroundListWinSizeX        500.f
        #define         defRunaroundListWinSizeY        50.f
        #define         defRunaroundListWinSizeW        200.f
        #define         defRunaroundListWinSizeH        200.f



        #define         defRunaroundInputWinSizeX       500.f
        #define         defRunaroundInputWinSizeY       300.f
        #define         defRunaroundInputWinSizeW       200.f
        #define         defRunaroundInputWinSizeH       200.f
#else
		#define         defRunaroundListWinSizeX        0.65f
        #define         defRunaroundListWinSizeY		0.03f
        #define         defRunaroundListWinSizeW        0.17f
        #define         defRunaroundListWinSizeH        0.30f



        #define         defRunaroundInputWinSizeX       0.65f
        #define         defRunaroundInputWinSizeY       0.35f
        #define         defRunaroundInputWinSizeW       0.17f
        #define         defRunaroundInputWinSizeH       0.30f
#endif

float					runaroundListWinSizeX = defRunaroundListWinSizeX,
                        runaroundListWinSizeY = defRunaroundListWinSizeY,
                        runaroundListWinSizeW = defRunaroundListWinSizeW,
                        runaroundListWinSizeH = defRunaroundListWinSizeH;


float					runaroundInputWinSizeX = defRunaroundInputWinSizeX,
                        runaroundInputWinSizeY = defRunaroundInputWinSizeY,
                        runaroundInputWinSizeW = defRunaroundInputWinSizeW,
                        runaroundInputWinSizeH = defRunaroundInputWinSizeH;


