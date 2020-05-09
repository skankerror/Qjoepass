/*
 *  myUtil.cpp
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

#include "systemswitch.h"
#include "graphics.h"

#if  MAC_OS_X_VERSION
#include <Carbon/Carbon.h>
#include <QuickTime/Movies.h>

#endif



#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "myutil.h"
#include "prefDef.h"
#include "mydefs.h"

#include "playSound.h"


#if MAC_CLASSIC_VERSION
#define USE_MAC_KEYBOARD_ACCESS
wwwBoolean              skMacMode, ckMacMode, akMacMode, capsMacMode, cmdMacMode;               //Zustand der Sondertasten
#endif



wwwBoolean              skMode, ckMode, akMode, capsMode, cmdMode;

//Zustand der Sondertasten

//wwwBoolean soundOutput = true;

void beep( void )
{
        if( !jpPrefs.soundOutput )
                return;
        _myBeep();
}

enum {singleThrowSound, singleCatchSound, multiThrowSound, multiCatchSound, dropSound, errorSound, soundCount}; //bgSound, soundCount};
#if 0 //XXXXX
#if MAC_OS_X_VERSION
extern Movie	theSound[soundCount];
#endif
#endif


void _myBeep( void )
{
#if 0 //xxxxx
#if MAC_OS_X_VERSION
	SysBeep(1);
#else
    ;
#endif
#endif
    
#if 01
    Hand dummy;
	//~ doSound(131, dummy, 3);
//      printf("%c", 7 );
//        putchar( '\a');

#endif

}

// macOS version
#ifdef USE_MAC_KEYBOARD_ACCESS
void errorHandler(int strID, int alrtID, wwwBoolean fatal);
void errorHandler(int strID, int alrtID, wwwBoolean fatal)
{
#pragma unused( strID, alrtID)
        if( fatal )
                exit(strID);
}
#endif

#ifdef USE_MAC_KEYBOARD_ACCESS
/* get state of modifier-keys (shift, caps, alt, cntrl, command) */
void getMacModifiers()
{
        KeyMap theKeyMap;

        GetKeys(theKeyMap);                     // Keyboard-Zustand

        skMacMode = theKeyMap[1] & 0x1;
        capsMacMode = theKeyMap[1] & 0x2;
        akMacMode = theKeyMap[1] & 0x4;
        ckMacMode = theKeyMap[1] & 0x8;
        cmdMacMode = (theKeyMap[1] & 0x8000) != 0;
#if 0
        // remap cmd to cntrl key
        if( cmdMacMode )
                ckMacMode = cmdMacMode;

#endif
//printf("%d\n", cmdMacMode);
}

#endif
//-------------------------------------
// never call glutGetModifiers directly
// some remapping is done here
//-------------------------------------
int getModifiers( void )
{
        int theModifiers = 0;

#ifdef USE_MAC_KEYBOARD_ACCESS

	// for some reason, I dont get the desired information on my (old) mac
        getMacModifiers();
        ckMode = ckMacMode;
        skMode = skMacMode;
        akMode = akMacMode; 
        cmdMode = cmdMacMode;

        if( skMode )
                theModifiers |= GLUT_ACTIVE_SHIFT;              //      Set if the Shift modifier or Caps Lock is active.
        if( akMode )
                theModifiers |= GLUT_ACTIVE_ALT ;               //      Set if the Alt modifier is active.
#if 01
        if( cmdMacMode )
                theModifiers |= GLUT_ACTIVE_CTRL;               //      Set if the Ctrl modifier is active.
#else
        if( ckMacMode )
                theModifiers |= GLUT_ACTIVE_CTRL;               //      Set if the Ctrl modifier is active.
#endif

#else

#if  MAC_OS_X_VERSION
{
	int tm = GetCurrentEventKeyModifiers();
		int s, c, a;

        s = (tm & 0x200);				//      Set if the Shift modifier or Caps Lock is active.
        c = (tm & 0x100);               //      Set if the Ctrl modifier is active.
        a = (tm & 0x800);               //      Set if the Alt modifier is active.

		akMode = a != 0;
		skMode = s != 0;
		ckMode = c != 0;
//		printf("theModifiers = %x\nsk = %d\nak = %d\nck = %d\n", theModifiers, skMode, akMode, ckMode);
		if(skMode)
				theModifiers = GLUT_ACTIVE_SHIFT;
		if( ckMode)
				theModifiers |= GLUT_ACTIVE_CTRL;
		if( akMode)
				theModifiers |= GLUT_ACTIVE_ALT;
}
#else
		theModifiers = glutGetModifiers();
        skMode = (theModifiers & GLUT_ACTIVE_SHIFT);    //      Set if the Shift modifier or Caps Lock is active.
        ckMode = (theModifiers & GLUT_ACTIVE_CTRL);             //      Set if the Ctrl modifier is active.
        akMode = (theModifiers & GLUT_ACTIVE_ALT);              //      Set if the Alt modifier is active.
#endif

#endif
        return theModifiers;

}

void checkApp( void )
{
        printf("control point reached\n");
        getchar();
}
