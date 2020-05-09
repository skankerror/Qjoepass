/*
 *  globals.h
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
#ifndef __JP_GLOBALS__
#define __JP_GLOBALS__

#include "systemswitch.h"
#include "mydefs.h"
#include "vtypes.h"


#ifdef __cplusplus
extern "C++"
{
#endif

enum{ initState, firstState, defaultState};// states for frameRate calculation
enum{ dontChangeMode = -1, dontCorrectPassValues, correctPassValues};

#ifdef DEF_VARS
#define extern
#endif

    // define global variables


extern wwwBoolean               hasSkyTexture,
                                hasFloorTexture,
                                hasObjectTexture,
                                hasRingTexture,
                                hasClubTexture,
                                hasHeadTexture,
                                hasBodyTexture,
								hasInfoTexture,
								hasStageTexture;

    extern  GLint				animationWin;
    extern  int                 gridLines;
    extern  int                 defStyleIndex;
    extern bool                 fullscreen;
    extern wwwBoolean           hasEnvModel;
    extern wwwBoolean           hasBodyModel;
    extern wwwBoolean           hasArmModel;
    extern float                objectBounceFrom;

#ifndef MAC_VERSION
    extern  int					worldWidth, worldHeight;        // was GLint
    extern  float				timeStep;
#endif
    extern  int                 startTime;

    // debug stuff
    extern float				debugValue1, debugValue0;
	extern float				*debugMatrix44;
    extern punkt				debugPoint, debugPoint2;

#undef extern

    // this ones are initialized in some modules
#ifdef OPEN_GL_VERSION


    extern float            fieldOfViewY;
    extern sysColor         inactiveColor;
    extern sysColor         activeColor;
    extern bool             joeHasCrashed;

    extern float            aTime;
    extern float            patternTime;
    extern wwwBoolean       skMode,
							ckMode,
							akMode,
							capsMode,
							cmdMode;                //Zustand der Sondertasten
#endif

    extern  int             infoWin;
    extern  int             editWin;

    extern float			runaroundInputWinSizeX,
							runaroundInputWinSizeY,
							runaroundInputWinSizeW,
							runaroundListWinSizeX,
							runaroundListWinSizeY,
							runaroundListWinSizeW,
							runaroundListWinSizeH,
							runaroundInputWinSizeH
							;

    extern wwwBoolean       mhnHurriedMode;
    extern punkt            center;
    extern  char            *thePattern;
    extern  Vector3D        schulter;
    extern  Vector3D        schritt;
    extern  Vector3D        fuss;
    extern  Vector3D        kopf;
    extern  sysColor        passColor;
    extern  sysColor        selfColor;
    extern  int             passCorrectionMode;

#ifdef __cplusplus
}
#endif

#endif
