/*
 *  runaroundMenu.cpp
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
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "languageSupport.h"

#include "runaroundClass.h"

#include "globals.h"
//
#include "runaroundMenu.h"

#include "runaround.h"
#include "listWindow.h"
#include "openglmenu.h"
#include "parseSiteswap.h"
#include "myutil.h"
#include "animationWin.h"
#include "runaround.h"
int		
runaroundFileMenu,
runaroundEditListMenu,
runaroundRotationMenu,
runaroundEditPositionMenu,
runaroundEditInputMenu,
runaroundViewMenu,
runaroundPositionMenu,
runaroundExpertsMenu;


#define LIST_FILE_MENU  "file..."
#define LIST_EDIT_MENU  "edit"
#define LIST_VIEW_MENU  "view"

_menuList *runaroundListMenuList;

void runaroundInitMenu( void )
{
	initMenu( runaroundListMenuList, true );
}

// xxxxx
//-----------------------------------------------------------------------
// runaround list window und 
// runaround input window 
// nutzen das gleiche Menu.
// aber: Die Menus gehören für glut zu Fenstern.
// also muss das menu des jeweils anderen Fensters auch upgedated werden.
// was nicht geht, da die menus über ihre IDs angesprochen werden.
// also bekommen die Fenster 2 identische Menus
// was auch nicht hilft, da dann die Marker des anderen Menus nicht angepasst werden.
// Also:
// Menu des anderen Fensters per Hand updaten
//-----------------------------------------------------------------------

void runaroundListKey(  unsigned char theKey, int x, int y )
{
#pragma unused( x,y)

	if( askForSaveInRunaround )
	{
		switch( theKey )
		{
			case 'S':
				saveRunaroundProject();
				closeRunaroundNow();
				askForSaveInRunaround = false;
				break;
			case 'D':
				setRunaroundPatternChanged( false );
				closeRunaroundNow();
			case 'R':
			case '\033':
				askForSaveInRunaround = false;
				break;			
		}
		return;
	}
	_menuHotkeySelect( runaroundListMenuList, theKey );
	

	redrawRunaroundWindows();
}

void initRunaroundList( void )
{
	new runaroundList();
}


void runaroundListMenuSelection( int id )
{
	
	_menuSelect( runaroundListMenuList, id );
	
	redrawRunaroundWindows();
}


runaroundList::~runaroundList( )
{
} 
runaroundList::runaroundList( void )
{
	
	_menuList *l, _runaroundListMenuList[] = {
		{"Runarond window",																menuTitle},
		
		
		
		{languageStrings[languageRunaroundMenuFile],                                    newMenu,                0,		&runaroundFileMenu,		runaroundListMenuSelection},
		 {languageStrings[languageRunaroundFileNew],                                     menuEntry,              'n',    NULL,                   newProject},
		 {languageStrings[languageRunaroundFileOpen],                                    menuEntry,              'o',    NULL,                   openRunaroundProject},
		 {languageStrings[languageRunaroundFileSave],                                    menuEntry,              's',    NULL,                   mSaveRunaroundProject},
		 {languageStrings[languageRunaroundFileSaveAs],                                  menuEntry,              'S',    NULL,                   mSaveRunaroundProjectAs},
		 {languageStrings[languageRunaroundFileClose],                                   menuEntry,              'w',    NULL,                   closeRunaround},
		 //{languageStrings[languageRunaroundFileDefault],								menuEntry,               0,		NULL,                   },
		 	
		
		
		{languageStrings[languageRunaroundMenuPositions],								newMenu,                0,		&runaroundPositionMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundPositionsSideToSide],							menuEntry,              0,		NULL,					runaroundSideToSidePosition},
		{languageStrings[languageRunaroundPositionsBackToBack],							menuEntry,              0,		NULL,					runaroundBackToBackPosition},
		{languageStrings[languageRunaroundPositionsLine],								menuEntry,              0,		NULL,					runaroundLinePosition},
		{languageStrings[languageRunaroundPositionsDropbackLine],						menuEntry,              0,		NULL,					runaroundDropbackLinePosition},
		{languageStrings[languageRunaroundPositionsCircle],								menuEntry,              0,		NULL,                   runaroundCirclePosition},
		{languageStrings[languageRunaroundPositionsFeed],								menuEntry,              0,		NULL,                   runaroundFeedPosition},
		{languageStrings[languageRunaroundPositionsGorillaFeed],						menuEntry,              't',    NULL,                   runaroundTerrorFeedPosition},
		{languageStrings[languageRunaroundPositionsRotateAllPositions],					menuEntry,              0,		NULL,                   NULL, &runaroundRotateAllScenesTogetherFlag},
		
	
		{languageStrings[languageRunaroundMenuEditScenesList],							newMenu,                0,		&runaroundEditListMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundFileSelect],                                  menuEntry,              'p',    NULL,                   runaroundSelectPattern},
		{languageStrings[languageRunaroundEditCopyNext],                                menuEntry,              ' ',    NULL,                   copyToNextField},
		{languageStrings[languageRunaroundEditAddPositionEnd],                  		menuEntry,              '+',    NULL,                   runaroundAddPosition},
		{languageStrings[languageRunaroundEditAddPositionSelection],    				menuEntry,              0,      NULL,                   runaroundAddBeforeSelectedPosition},
		{languageStrings[languageRunaroundEditDeletePositionEnd],               		menuEntry,              '-',    NULL,                   runaroundRemovePositionAtEnd},
		{languageStrings[languageRunaroundEditDeletePositionSelection], 				menuEntry,              0,      NULL,			        runaroundRemoveSelectedPosition},
		{languageStrings[languageRunaroundEditCycleClockwise],							menuEntry,              '.',    NULL,                   cycleCW},
		{languageStrings[languageRunaroundEditCycleCounterclockwise],					menuEntry,              ',',    NULL,                   cycleCCW},
		{languageStrings[languageRunaroundEditAddJuggler],                              menuEntry,              '*',    NULL,                   runaroundAddJuggler},
        {languageStrings[languageRunaroundEditRemoveJuggler],                   		menuEntry,              '/',   NULL,                   runaroundRemoveJuggler},
		{languageStrings[languageRunaroundEditNewline],                                 menuEntry,              '\r',	NULL,					toggleNewLine},
		{languageStrings[languageRunaroundEditDouble],                                  menuEntry,              'D',    NULL,                   doublePattern},
		{languageStrings[languageRunaroundEditHalve],									menuEntry,				0,		NULL,                   shrinkToHalve},


		
		{languageStrings[languageRunaroundMenuSelectScenes],							newMenu,                0,		&runaroundRotationMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundSelectScenesA],								menuEntry,              '1',	NULL,                   runaroundSelectSceneA},
		{languageStrings[languageRunaroundSelectScenesB],								menuEntry,              '2',	NULL,                   runaroundSelectSceneB},
		{languageStrings[languageRunaroundSelectScenesUnselect],						menuEntry,              'u',	NULL,					unselectAllScenes},
		{languageStrings[languageRunaroundEditRotateClockwise],							menuEntry,              '<',    NULL,                   runaroundRotateSceneR},
		{languageStrings[languageRunaroundEditRotateCounterClockwise],					menuEntry,              '>',    NULL,                   runaroundRotateSceneL},
		
		{languageStrings[languageRunaroundMenuEditCurrentScene],						newMenu,                0,		&runaroundEditPositionMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundEditHorizontal],                              menuEntry,              'h',    NULL,                   horizontal},
		{languageStrings[languageRunaroundEditVertical],                                menuEntry,              'v',    NULL,                   vertical},
//		{languageStrings[languageRunaroundEditXpositions],                              menuEntry,              '#',    NULL,                   exchange},
		{languageStrings[languageRunaroundFileEditCausal],                			    menuEntry,              'e',    NULL,                   editPatternInCDEW},
		{languageStrings[languageRunaroundFileEditText],                   			    menuEntry,              'E',    NULL,                   editPatternInTE},
		 
		{languageStrings[languageRunaroundMenuView],                                    newMenu,                0,      &runaroundViewMenu,		runaroundListMenuSelection},
		{languageStrings[languageRunaroundAnimatedScene],                               menuEntry,              'a',    NULL,                   runaroundShowAnimatedScene, &runaroundShowAnimatedSceneFlag},
		{languageStrings[languageRunaroundViewGrid],                                    menuEntry,              'g',    NULL,                   showGrid, &showGridFlag},
//xxxxx hier auch einfügen: Umschalten in mehreren Menus		
		{languageStrings[languageRunaroundViewPos],										menuEntry,              0,      NULL,					showPos, &showPosFlag},
		{languageStrings[languageRunaroundViewPoi],										menuEntry,              0,      NULL,					showPoi, &showPoiFlag}, 
		{languageStrings[languageRunaroundViewShowFrame],                               menuEntry,              'G',      NULL,                   showFrame, &showFrameFlag},
		{languageStrings[languageRunaroundViewShowIndex],                               menuEntry,              'i',    NULL,					toggleSceneIndex, &showSceneIndexFlag},
		{languageStrings[languageRunaroundViewMoreCorners],                             menuEntry,              'C',    NULL,                   moreCornersInFrame},
		{languageStrings[languageRunaroundViewLessCorners],                             menuEntry,              'c',    NULL,                   lessCornersInFrame},
		{languageStrings[languageRunaroundViewRotateFrameCW],							menuEntry,				'f',    NULL,					rotateFrameClockwise},
		{languageStrings[languageRunaroundViewRotateFrameCCW],							menuEntry,				'F',    NULL,					rotateFrameCounterclockwise},
		{languageStrings[languageRunaroundViewScaleUp],                                 menuEntry,              'X',    NULL,                   bigger},
		{languageStrings[languageRunaroundViewScaleDown],                               menuEntry,              'x',    NULL,                   smaller},
		
		{languageStrings[languageRunaroundMenuExperts],									newMenu,                0,      &runaroundExpertsMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuRemoveUnused],							menuEntry,				0,		NULL,					RemoveUnusedFilesOnFileList},
		{languageStrings[languageRunaroundMenuAttachDefault],							menuEntry,				0,		NULL,					attachDefaultPatternToAllPositions},
		
		{"mainmenu",																	newMenu,                0,		NULL,                   runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuFile],                                    addAsSubMenu,			0,		&runaroundFileMenu,		runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuEditScenesList],							addAsSubMenu,			0,		&runaroundEditListMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuSelectScenes],							addAsSubMenu,			0,		&runaroundRotationMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuEditCurrentScene],						addAsSubMenu,			0,		&runaroundEditPositionMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuPositions],								addAsSubMenu,			0,		&runaroundPositionMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuView],                                    addAsSubMenu,			0,		&runaroundViewMenu,		runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuExperts],									addAsSubMenu,			0,		&runaroundExpertsMenu,	runaroundListMenuSelection},
		{languageStrings[languageRunaroundMenuRun],                                     menuEntry,              'r',    NULL,                   startList},
		{languageStrings[languageAllMenusOpenDoc],										menuEntry,				'?',	NULL,					openDocFunc},
		{languageStrings[languageAllMenusQuit],											menuEntry,              'q',    NULL,					animationQuit},
		{NULL,																			menuListTerminator,     0,		NULL,					0}      // last entry
	};
;
	
	// alloc memory for menuList
	l = (_menuList *)malloc( sizeof( _runaroundListMenuList ));
	// copy content
	memcpy( l, _runaroundListMenuList, sizeof( _runaroundListMenuList ));
	
	runaroundListMenuList = l;
	
}

