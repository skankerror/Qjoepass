/*
 * cdewMenu.cpp
 * joe2
 *
 *  created by wolfgang on 02.12.06.
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "myutil.h"

#include "causal_editor.h"
#include "cdewClass.h"
#include "cdewDraw.h"
#include "cdewMenu.h"

#include "openglmenu.h"

#include "exitModules.h"

#include "mydefs.h"
#include "screenshot.h"
#include "applicationFolders.h"
#include "fileIO.h"

#include "fileaccess.h"
#include "myPrintf.h"
#include "animationWin.h"

#include "languageSupport.h"
#include "mem.h"


extern int		autofixByUserSelection;
extern oglwin	*firstCdewWindow;
wwwBoolean		currentWindowHasBeenCLosed;
wwwBoolean		changeCellSizeFlag = false;

int				defaultThickness = 1;
int				cdewDefaultThickness = 1, 
				cdewDefaultColor = 0,
				cdewLastColor = -1,
				cdewLastThickness = 1;

void			menuSelection( int r );
void			cdewCalcStart( int wwDummy );
void			toggleTimeCursor( int dummy );

static int
fileMenu,
psMenu,
headerMenu,
transferMenu,
editMenu,
fixMenu,
markerMenu,
markMenu,
markActionMenu,
unmarkMenu,
viewMenu,
viewThrowMenu,
viewPatternMenu,
//arrowStyleMenu,
arrowThicknessMenu,
backgroundColorMenu,
arrowColorMenu,
modifierMenu,
displayMenu,
contentMenu,
sizeMenu,
cycleMenu,
colorMenu,
repeatMenu,
generalMenu,
windowMenu;




void oglwin::cdewInitMenu( void ) {
	
    //myPrintf("init cdew menu \n");
	
    _menuList cdewList[] = {
		// hotkeys
		

		// hotkeys
		{"cdew window",									menuTitle},
		{" ",                                           noMenuHotkey,                  '0',             NULL,           cdewFixDelay},
		{languageStrings[languageCdewMenuPS],           newMenu,                        0,              &psMenu,        _cdewMenu},
		{languageStrings[languageCdewPSSaveAs],         menuEntry,                      'g',            NULL,           cdewSavePS},
		{languageStrings[languageCdewPSColor],          menuEntry,                      'G',            NULL,           NULL, &colorPostscript},
		
		{languageStrings[languageCdewMenuHeader],       newMenu,                        0,              &headerMenu,    _cdewMenu},
		{languageStrings[languageCdewHeaderOpen],       menuEntry,                      'A',			NULL,			cdewReadHeader},
		{languageStrings[languageCdewHeaderSaveAs],     menuEntry,                      0,              NULL,           cdewWriteHeader},
		{languageStrings[languageCdewHeaderClear],      menuEntry,                      0,              NULL,           cdewClearHeader},
		
		{languageStrings[languageCdewMenuTransfer],     newMenu,                        0,              &transferMenu,	_cdewMenu},
		{languageStrings[languageCdewStart],            menuEntry,                      '\003',         NULL,           cdewAnimate},
		{languageStrings[languageCdewAlwaysStart],      menuEntry,                      0,              NULL,           NULL,   &cdewAutoStart},
		{languageStrings[languageCdewOpenText],         menuEntry,                      'T',              NULL,           cdewTextEdit},
		{languageStrings[languageCdewAlwaysOpenText],   menuEntry,                      0,              NULL,           NULL,   &cdewAutoOpen},
		
		{languageStrings[languageCdewMenuFile],         newMenu,                        0,              &fileMenu,      _cdewMenu},
		{languageStrings[languageCdewFileNew],          menuEntry,                      'W',            NULL,           cdewNewWin},
		{languageStrings[languageCdewFileDuplicate],	menuEntry,                      'D',            NULL,           cdewDuplicateWin},
		{languageStrings[languageCdewFileOpen],         menuEntry,                      'o',            NULL,           cdewOpen},
		{languageStrings[languageCdewFileClose],        menuEntry,                      'w',            NULL,           cdewCloseWin},
		{languageStrings[languageCdewFileSave],         menuEntry,                      's',            NULL,           cdewSave},
		{languageStrings[languageCdewFileSaveAs],       menuEntry,                      'y',			NULL,           cdewSaveAs},
		{languageStrings[languageCdewFileSaveAutoName], menuEntry,                      'N',			NULL,           cdewSaveAuto},
		{languageStrings[languageCdewFileAdd],          menuEntry,                      'a',			NULL,           cdewAdd},
		{languageStrings[languageCdewFileMerge],        menuEntry,                      'm',			NULL,           cdewMerge},
		{languageStrings[languageCdewFileSaveTemplate], menuEntry,						0,				NULL,           cdewTemplate},
		{languageStrings[languageCdewFileScreenshot],   menuEntry,                      ' ',			NULL,           cdewScreenshot},
		{languageStrings[languageCdewFileSign],         menuEntry,                      '!',			NULL,           cdewSign},
		{languageStrings[languageCdewFileShow],         menuEntry,                      '=',			NULL,           toggleAnimationActive},
		{languageStrings[languageCdewMenuPS],           addAsSubMenu,                   0,              &psMenu,        _cdewMenu},
		{languageStrings[languageCdewMenuHeader],       addAsSubMenu,                   0,              &headerMenu,	_cdewMenu},
		{languageStrings[languageCdewMenuTransfer],     addAsSubMenu,                   0,              &transferMenu,	_cdewMenu},
		
		{languageStrings[languageCdewMenuFix],          newMenu,                        0,              &fixMenu,       _cdewMenu},
		{languageStrings[languageCdewFixNow],           menuEntry,                      'f',    		NULL,           cdewFixNow},
		{languageStrings[languageCdewFixAlways],        menuEntry,                      0,              NULL,           NULL, &autoFixConnections},
		{languageStrings[languageCdewFixNoHurries],     menuEntry,                      0,              NULL,           NULL, &unhurriedDiagram},
		
		{languageStrings[languageCdewMenuModifier],     newMenu,                        0,              &modifierMenu,	_cdewMenu},
		{languageStrings[languageCdewModifierBeep],     menuEntry,                      0,              NULL,           cdewToggleBeepOnMarks},
		{languageStrings[languageCdewModifierBounce],   menuEntry,                      0,              NULL,           cdewToggleBounceOnMarks},
		{languageStrings[languageCdewModifierNoBeep],   menuEntry,                      0,              NULL,           cdewBeepOff},
		{languageStrings[languageCdewModifierNoBounce], menuEntry,                      0,              NULL,           cdewTossAll},
		{languageStrings[languageCdewModifierHalfspin], menuEntry,                      0,              NULL,           cdewHalfSpinOnMarks},
		
		{languageStrings[languageCdewMenuMark],         newMenu,                        0,              &markMenu,      _cdewMenu},
		{languageStrings[languageCdewMarkPath],         menuEntry,                      'p',			NULL,			cdewMarkPath},
		{languageStrings[languageCdewMarkPass],		   menuEntry,                      'P',				NULL,			cdewMarkPass},
		
		{languageStrings[languageCdewMarkRight],        menuEntry,                      'R',			NULL,			cdewMarkRight},
		{languageStrings[languageCdewMarkBeat],         menuEntry,                      'b',			NULL,			cdewMarkBeat},
		{languageStrings[languageCdewMarkJuggler],      menuEntry,                      'j',			NULL,			cdewMarkJuggler},
		{languageStrings[languageCdewMarkHand],         menuEntry,                      'h',			NULL,			cdewMarkHands},
		{languageStrings[languageCdewMarkInvert],       menuEntry,                      'i',			NULL,			cdewInvertMarked},
		{languageStrings[languageCdewMarkShiftLeft],    menuEntry,                      '1',			NULL,			cdewShiftMarksLeft},
		{languageStrings[languageCdewMarkShiftRight],   menuEntry,                      '3',			NULL,			cdewShiftMarksRight},
		
		{languageStrings[languageCdewMenuAction],		newMenu,                        0,				&markActionMenu, _cdewMenu},
		{languageStrings[languageCdewMarkInvertHeight], menuEntry,                      'H',			NULL,			cdewInvertHeight},
		{languageStrings[languageCdewMarkXHands],       menuEntry,                      'X',			NULL,			cdewChangeHands},
		
		{languageStrings[languageCdewMenuUnmark],       newMenu,                        0,              &unmarkMenu,	_cdewMenu},
		{languageStrings[languageCdewUnmarkPath],       menuEntry,                      0,              NULL,           cdewUnmarkPath},
		{languageStrings[languageCdewUnmarkBeat],       menuEntry,                      0,              NULL,           cdewUnmarkBeat},
		{languageStrings[languageCdewUnmarkJuggler],    menuEntry,                      0,              NULL,           cdewUnmarkJuggler},
		{languageStrings[languageCdewUnmarkHand],       menuEntry,                      0,              NULL,           cdewUnmarkHands},
		{languageStrings[languageCdewUnmarkAll],        menuEntry,                      'u',			NULL,           cdewUnmarkAll},
		
		{languageStrings[languageCdewMenuCycle],        newMenu,                        0,              &cycleMenu,     _cdewMenu},
		{languageStrings[languageCdewCycleLeft],        menuEntry,                      '4',			NULL,           cdewCycleLeft},
		{languageStrings[languageCdewCycleRight],       menuEntry,                      '6',			NULL,           cdewCycleRight},
		{languageStrings[languageCdewCycleUp],          menuEntry,                      '8',			NULL,           cdewCycleUp},
		{languageStrings[languageCdewCycleDown],        menuEntry,                      '2',			NULL,			cdewCycleDown},
		{languageStrings[languageCdewSwap1And2],		   menuEntry,					'J',			NULL,			cdewSwap1And2},
		
		{languageStrings[languageCdewMenuContent],      newMenu,                        0,              &contentMenu,   _cdewMenu},
		{languageStrings[languageCdewContentClear],     menuEntry,                      0,              NULL,           cdewClear},
		{languageStrings[languageCdewContentDefault],   menuEntry,                      0,				NULL,           cdewSetDefault},
		{languageStrings[languageCdewHandsDistribute],   menuEntry,						0,				NULL,           cdewDistributePerHand},
		{languageStrings[languageCdewContentDistribute],menuEntry,						0,				NULL,			cdewDistribute},
		{languageStrings[languageCdewContentRotate],    menuEntry,                      'r',			NULL,           cdewRotate},
		{languageStrings[languageCdewContentResetArrows],  menuEntry,					0,              NULL,           cdewResetBows},
		
		{languageStrings[languageCdewMenuSize],         newMenu,                        0,              &sizeMenu,      _cdewMenu},
		{languageStrings[languageCdewSizeAddJuggler],   menuEntry,                      '*',			NULL,           cdewAddJuggler},
		{languageStrings[languageCdewSizeRemoveJuggler],menuEntry,                      '/',			NULL,           cdewRemoveJuggler},
		{languageStrings[languageCdewSizeAddBeat],      menuEntry,                      '+',			NULL,           cdewAddThrow},
		{languageStrings[languageCdewSizeRemoveBeat],   menuEntry,                      '-',			NULL,           cdewRemoveThrow},
		{languageStrings[languageCdewSizeAddReverse],   menuEntry,                      'I',			NULL,           cdewAddReverse},
		{languageStrings[languageCdewSizeAddSymetric],  menuEntry,                      '%',			NULL,           cdewAddSym},
		{languageStrings[languageCdewSizeAddSame],      menuEntry,                      '&',			NULL,           cdewAddSame},
		
		{languageStrings[languageCdewMenuEdit],         newMenu,                        0,              &editMenu,      _cdewMenu},
		{languageStrings[languageCdewEditUndo],         menuEntry,                      0,              NULL,           cdewUndo},
		{languageStrings[languageCdewEditRedo],         menuEntry,                      0,              NULL,           cdewRedo},              
		
		{languageStrings[languageCdewEditCut],		   menuEntry,						'x',			NULL,           cdewCut},
		{languageStrings[languageCdewEditCopy],		   menuEntry,						'c',			NULL,           cdewCopy},
		{languageStrings[languageCdewEditPaste],		   menuEntry,					'v',			NULL,           cdewPaste},
		
		
		{languageStrings[languageCdewMenuContent],      addAsSubMenu,                   0,              &contentMenu,   _cdewMenu},
		{languageStrings[languageCdewMenuSize],         addAsSubMenu,                   0,              &sizeMenu,      _cdewMenu},
		{languageStrings[languageCdewMenuCycle],        addAsSubMenu,                   0,              &cycleMenu,     _cdewMenu},
		{languageStrings[languageCdewMenuFix],          addAsSubMenu,                   0,              &fixMenu,       _cdewMenu},
		
		{languageStrings[languageCdewMarkBackgroundColor],       newMenu,					0,              &backgroundColorMenu,_cdewMenu},
		{languageStrings[languageCdewMarkBackgroundColorReset], menuEntry,					0,				NULL,			cdewBackgroundColorReset},
		{languageStrings[languageCdewMarkColor1],       menuEntry,					0,				NULL,           cdewBackgroundColor1},
		{languageStrings[languageCdewMarkColor2],       menuEntry,					0,				NULL,			cdewBackgroundColor2},
		{languageStrings[languageCdewMarkColor3],       menuEntry,					0,				NULL,			cdewBackgroundColor3},
		{languageStrings[languageCdewMarkColor4],       menuEntry,					0,				NULL,			cdewBackgroundColor4},
		{languageStrings[languageCdewMarkColor5],       menuEntry,					0,				NULL,			cdewBackgroundColor5},
		{languageStrings[languageCdewMarkColor6],       menuEntry,					0,				NULL,			cdewBackgroundColor6},
		{languageStrings[languageCdewMarkColor7],       menuEntry,					0,				NULL,			cdewBackgroundColor7},
		{languageStrings[languageCdewMarkColor8],       menuEntry,					0,				NULL,			cdewBackgroundColor8},
		
		{languageStrings[languageCdewMarkArrowColor],       newMenu,				0,              &arrowColorMenu,_cdewMenu},
		{languageStrings[languageCdewMarkArrowColorReset], menuEntry,				0,				NULL,			cdewArrowColorReset},
		{languageStrings[languageCdewMarkColor1],       menuEntry,					0,				NULL,           cdewArrowColor1},
		{languageStrings[languageCdewMarkColor2],       menuEntry,					0,				NULL,			cdewArrowColor2},
		{languageStrings[languageCdewMarkColor3],       menuEntry,					0,				NULL,			cdewArrowColor3},
		{languageStrings[languageCdewMarkColor4],       menuEntry,					0,				NULL,			cdewArrowColor4},
		{languageStrings[languageCdewMarkColor5],       menuEntry,					0,				NULL,			cdewArrowColor5},
		{languageStrings[languageCdewMarkColor6],       menuEntry,					0,				NULL,			cdewArrowColor6},
		{languageStrings[languageCdewMarkColor7],       menuEntry,					0,				NULL,			cdewArrowColor7},
		{languageStrings[languageCdewMarkColor8],       menuEntry,					0,				NULL,			cdewArrowColor8},
		{languageStrings[languageCdewMarkArrowThickness],   newMenu,					0,              &arrowThicknessMenu,_cdewMenu},
		{languageStrings[languageCdewMarkArrowThicknessReset],   menuEntry,				0,				NULL,			cdewThicknessReset},
		{languageStrings[languageCdewMarkArrowThickness1],   menuEntry,					0,				NULL,			cdewThickness1},
		{languageStrings[languageCdewMarkArrowThickness2],   menuEntry,					0,				NULL,			cdewThickness2},
		{languageStrings[languageCdewMarkArrowThickness3],   menuEntry,					0,				NULL,			cdewThickness3},
		{languageStrings[languageCdewMarkArrowThickness4],   menuEntry,					0,				NULL,			cdewThickness4},
		//                             {languageStrings[languageCdewMarkArrowDefaults], menuEntry,                      0,              NULL,           cdewArrowDefault},
		
		{languageStrings[languageCdewMarkRepeat],            newMenu,					0,				&repeatMenu,	_cdewMenu},
		{languageStrings[languageCdewMarkRepeatStart],       menuEntry,					0,				NULL,			cdewToggleRepeatIn},
		{languageStrings[languageCdewMarkRepeatEnd],         menuEntry,					0,				NULL,			cdewToggleRepeatOut},
		{languageStrings[languageCdewMarkRepeatDelete],      menuEntry,					0,				NULL,			cdewResetRepeat},
		
		
		{languageStrings[languageCdewMenuMarkers], 	  newMenu,							0,              &markerMenu,     _cdewMenu},
		// {languageStrings[languageCdewMenuMarkArrowStyle],   addAsSubMenu,                   0,              &arrowStyleMenu,      _cdewMenu},
		{languageStrings[languageCdewMarkBackgroundColor],   addAsSubMenu,				0,              &backgroundColorMenu,_cdewMenu},
		{languageStrings[languageCdewMarkArrowColor],        addAsSubMenu,				0,              &arrowColorMenu,_cdewMenu},
		{languageStrings[languageCdewMarkArrowThickness],    addAsSubMenu,				0,              &arrowThicknessMenu,_cdewMenu},
		{languageStrings[languageCdewMarkRepeat],            addAsSubMenu,				0,              &repeatMenu,	_cdewMenu},
		{languageStrings[languageCdewMenuModifier],          addAsSubMenu,				0,              &modifierMenu,  _cdewMenu},
		{languageStrings[languageCdewMenuMark],              addAsSubMenu,				0,              &markMenu,      _cdewMenu},
		{languageStrings[languageCdewMenuAction],			 addAsSubMenu,				0,              &markActionMenu,_cdewMenu},
		{languageStrings[languageCdewMenuUnmark],            addAsSubMenu,				0,              &unmarkMenu,	_cdewMenu},
		
		{languageStrings[languageCdewMenuDisplay ],          newMenu,					0,              &displayMenu,	_cdewMenu},
		{languageStrings[languageCdewDisplaySingle],         menuEntry,					0,              NULL,           cdewSingleLineMode},
		{languageStrings[languageCdewDisplaySync],           menuEntry,					0,              NULL,           cdewSyncLineMode},
		{languageStrings[languageCdewDisplayColor],          menuEntry,					0,              NULL,           cdewColorLineMode},
		{languageStrings[languageCdewDisplayNext],           menuEntry,					'd',			NULL,			cdewNextDisplayMode},
		{languageStrings[languageCdewDisplayModePrint],		 menuEntry,					0,              NULL,           NULL, &showDisplayMode},
		
		{languageStrings[languageCdewMenuColor],             newMenu,					0,              &colorMenu,     _cdewMenu},
		{languageStrings[languageCdewColorLength],           menuEntry,					0,              NULL,           cdewLengthColor},
		{languageStrings[languageCdewColorPath],             menuEntry,					0,              NULL,           cdewCausalColor},
		{languageStrings[languageCdewColorJuggler],          menuEntry,					0,              NULL,           cdewJugglerColor},
		{languageStrings[languageCdewColorReset],           menuEntry,					0,              NULL,           cdewColorResetAll},
		{languageStrings[languageCdewColorNext],             menuEntry,					'E',			NULL,           cdewNextColorMode},
		{languageStrings[languageCdewColorModePrint],        menuEntry,					0,              NULL,           NULL, &showColorMode},
		
		{languageStrings[languageCdewMenuViewThrow],         newMenu,					0,              &viewThrowMenu, _cdewMenu},
		{languageStrings[languageCdewViewHurries],           menuEntry,					'.',			NULL,           NULL, &showHurries},
		{languageStrings[languageCdewViewCrosses],           menuEntry,					0,				NULL,           NULL, &showCrosses},
		{languageStrings[languageCdewViewFaults],            menuEntry,					'F',			NULL,           NULL, &showErrors},
		
		{languageStrings[languageCdewMenuViewPattern],       newMenu,					0,              &viewPatternMenu, _cdewMenu},
		{languageStrings[languageCdewViewDelays],            menuEntry,					0,              NULL,           NULL, &showDelays},
		{languageStrings[languageCdewViewLines],             menuEntry,					'_',			NULL,           NULL, &jugglerLines},
		{languageStrings[languageCdewViewCellCol],           menuEntry,					'e',			NULL,           cdewCellColors},
		{languageStrings[languageCdewViewSiteswap],          menuEntry,					'S',			NULL,           cdewThrowDisplayMode},
		{languageStrings[languageCdewViewObjects],           menuEntry,					'n',			NULL,           NULL, &showObjects},
		{languageStrings[languageCdewViewBeats],             menuEntry,					'B',			NULL,           NULL, &showNumbers},
		{languageStrings[languageCdewViewAllHands],          noMenuHotkey,				0,				NULL,           NULL, &showUnusedHandsInSyncMode},
		{languageStrings[languageCdewViewStart],              menuEntry,				'<',			NULL,           NULL, &showStart},
		
		{languageStrings[languageCdewMenuView],              newMenu,					0,              &viewMenu,      _cdewMenu},
		{languageStrings[languageCdewMenuViewThrow],         addAsSubMenu,				0,              &viewThrowMenu,	_cdewMenu},
		{languageStrings[languageCdewMenuViewPattern],       addAsSubMenu,				0,              &viewPatternMenu,_cdewMenu},
		{languageStrings[languageCdewViewTimeC],             menuEntry,					't',			NULL,           toggleTimeCursor },
			
		{languageStrings[languageCdewMenuColor],             addAsSubMenu,				0,              &colorMenu,		_cdewMenu},
		{languageStrings[languageCdewMenuDisplay],           addAsSubMenu,				0,              &displayMenu,	_cdewMenu},
		
		
		{languageStrings[languageCdewMenuGeneral],			newMenu,					0,              &generalMenu,	_cdewMenu},
		{languageStrings[languageCdewGeneralBackground],	menuEntry,					'#',			NULL,           backgroundColorMode},
		{"cell background",									menuEntry,					'\'',			NULL,			NULL, &cellBackgroundFlag},

		{languageStrings[languageCdewGeneralSnap],			menuEntry,					'5',              NULL,			NULL, &cdewSnap},
			
#if MAC_OS_X_VERSION
		{languageStrings[languageCdewMenuWindow],			newMenu,					0,              &windowMenu,	_cdewMenu},
		{languageStrings[languageCdewWindowAnimation],		menuEntry,					'z',            NULL,           cdewSwitchToLastOrAnimationWindow},
		{languageStrings[languageCdewWindowNextCdew],		menuEntry,					'>',            NULL,           cdewCycleThroughCdewWindows},
#endif		
		{"mainmenu",										newMenu,					0,              NULL,			_cdewMenu },
		{languageStrings[languageCdewMenuFile],				addAsSubMenu,				0,              &fileMenu,		_cdewMenu},
		{languageStrings[languageCdewMenuEdit],				addAsSubMenu,				0,              &editMenu,		_cdewMenu},
		{languageStrings[languageCdewMenuMarkers],			addAsSubMenu,				0,             &markerMenu,		_cdewMenu},
		{languageStrings[languageCdewMenuView],				addAsSubMenu,				0,              &viewMenu,      _cdewMenu},
		{languageStrings[languageCdewMenuGeneral],			addAsSubMenu,				0,              &generalMenu,	_cdewMenu},
#if MAC_OS_X_VERSION
		{languageStrings[languageCdewMenuWindow],			addAsSubMenu,				0,              &windowMenu,	_cdewMenu},
#endif
		{languageStrings[languageAllMenusOpenDoc],			menuEntry,					'?',			NULL,			openDocFunc},
		{languageStrings[languageAllMenusQuit],				menuEntry,					'q',            NULL,           cdewQuit},

		{NULL,                                               menuListTerminator,		0,              NULL,           0}      // last entry
	};
	
	
	
	
	
	
	
	
    // alloc memory for menuList
    cdewMenuList = (_menuList *)malloc( sizeof( cdewList ));
    // copy content
    memcpy( cdewMenuList, cdewList, sizeof( cdewList ));
    initMenu( cdewMenuList, true );
	
    //      myPrintf("init cdew menu ...done \n");
	
}


void menuSelection( int r ) {
    oglwin *t;
	
    t = getOglwinFromID( glutGetWindow() );
    t->_menuSelection(r);
	
}

void oglwin::_menuSelection( int r ) {
    currentWindowHasBeenCLosed = false;
    _menuSelect(cdewMenuList, r );
    if( currentWindowHasBeenCLosed == false )
        postInputAction( );
}



void oglwin::stdInput( unsigned char key) {
    currentWindowHasBeenCLosed = false;
    _menuHotkeySelect( cdewMenuList, key );
    if( currentWindowHasBeenCLosed == false )
        postInputAction( );
}
void oglwin::postInputAction( void ) {
    redisplayAllCDEWWindows();
	transferToAnimation();
}
//#pragma mark ------ functions
void cdewNewWin( int wwDummy) {
//#pragma unused (wwDummy)
    new oglwin("untitled" );
}

#define COPIED_DIAGRAM_NAME "copiedDiagram"
//#pragma mark ------ functions
void cdewDuplicateWin( int wwDummy) {
//#pragma unused (wwDummy)
	char *p;
	int wx, wy, ww, wh;
	
    oglwin *w = getOglwinFromID( glutGetWindow() );
	w->saveDiagramWithName(COPIED_DIAGRAM_NAME);
	
	wx = glutGet( GLUT_WINDOW_X);
	wy = glutGet( GLUT_WINDOW_Y);
	ww = glutGet( GLUT_WINDOW_WIDTH);
	wh = glutGet( GLUT_WINDOW_HEIGHT);	
	staggerWindowPosition( &wx, &wy);
	setDefaultCdewWindowSize( wx, wy, ww, wh);
	oglwin *c = new oglwin("copy" );
	setDefaultCdewWindowSize( -1,-1,-1,-1);
	
	p = readFile( COPIED_DIAGRAM_NAME, "r");
	c->_readDiagram(p);
	free( p );
	remove( COPIED_DIAGRAM_NAME );
}

void cdewOpen( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow() );
    w->readDiagram();
#if !CDEW_STANDALONE
	
    if( cdewAutoStart )
        w->transferToAnimation();
    if( cdewAutoOpen )
        w->transferToTextEditor();
#endif
}
void cdewSave( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow() );
    w->fileChangedFlag = true;
    w->saveDiagram();
}

void cdewSaveAuto( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow() );
    w->fileChangedFlag = true;
    w->autoSaveFlag = true;
    w->saveDiagramAuto();
}

void cdewSaveAs( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow() );
    w->fileChangedFlag = true;
    w->autoSaveFlag = false;
    w->saveDiagramAs();
}
void cdewSavePS( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
	//    w->saveDiagramWithName(cdewTemplatePath);
	//    w->writeTemplatePrefs();
	//yyy
	//  pfad sichern
	//	ps file: pfad nicht übernehmen, besser: eigenen postscript pfad
    printPS = true;
    w->printThisWindowPS();
}
void cdewQuit( int wwDummy) {
//#pragma unused (wwDummy)
	
	exitModules();
}
void cdewTemplate( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->saveDiagramWithName(cdewTemplatePath);
    w->writeTemplatePrefs();
}
void cdewFixNow( int wwDummy) {
//#pragma unused (wwDummy)
	
    wwwBoolean fixState = autoFixConnections;
	oglwin *w = getOglwinFromID( glutGetWindow()) ;
    autoFixConnections = true;
    autofixByUserSelection = true;
    w->fixAllConnections();
    autofixByUserSelection = false;
    autoFixConnections = fixState;
}
void cdewNextDisplayMode( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->switchDisplayMode();
}

void cdewSingleLineMode( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->displayMode = singleLineMode;
}
void cdewSyncLineMode( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->displayMode = syncLineMode;
}
void cdewColorLineMode( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->displayMode = colorLineMode;
}

/*
 {"arrow length colors", menuEntry,                      0,              NULL,           cdewLengthColor},
 {"causal path colors",  menuEntry,                      0,              NULL,           cdewCausalColor},
 */
void oglwin::setColorPattern( void ) {
    cdewCausalColor(0);
}


void cdewClear( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->clearDiagram();
}
void cdewCloseWin( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->closeWin();
    currentWindowHasBeenCLosed = true;
	
}

void cdewReadHeader( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->readHeaderFromFile();
	
	if(cdewAutoStart){
		w->autoStartRestartFlag = w->fileChangedFlag = true;
		w->transferToAnimation();
	}
}
void cdewWriteHeader( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->writeHeaderToFile();
}
void cdewClearHeader( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->clearHeader();
}
void cdewRedo( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->redo();
}
/*
 
 void cdewNextColor( int wwDummy) {
 //#pragma unused (wwDummy)
 oglwin *w = getOglwinFromID( glutGetWindow()) ;
 w->nextColor();
 }
 
 void cdewNextThickness( int wwDummy) {
 //#pragma unused (wwDummy)
 oglwin *w = getOglwinFromID( glutGetWindow()) ;
 w->nextThickness();
 }
 */

void cdewBackgroundColorReset( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, NO_COLOR);
}

void cdewBackgroundColor1( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells,  1);
}

void cdewBackgroundColor2( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 2);
}

void cdewBackgroundColor3( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 3);
}

void cdewBackgroundColor4( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 4);
}

void cdewBackgroundColor5( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells,  5);
}

void cdewBackgroundColor6( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 6);
}

void cdewBackgroundColor7( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 7);
}

void cdewBackgroundColor8( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedBackgroundColor(w->causalCells, 8);
}


void cdewArrowColorReset( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = -1, markedColorMode);
}

void cdewArrowColor1( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 1, userColorMode);
}

void cdewArrowColor2( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 2, userColorMode);
}

void cdewArrowColor3( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 3, userColorMode);
}

void cdewArrowColor4( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 4, userColorMode);
}

void cdewArrowColor5( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 5, userColorMode);
}

void cdewArrowColor6( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 6, userColorMode);
}

void cdewArrowColor7( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 7, userColorMode);
}

void cdewArrowColor8( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = 8, userColorMode);
}

void cdewThicknessReset( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedWidth(defaultThickness);
}

void cdewThicknessMakeDefault( int wwDummy) {
//#pragma unused (wwDummy)
	cdewDefaultThickness = cdewLastThickness;
}


void cdewThickness1( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedWidth(cdewLastThickness = 1);
}

void cdewThickness2( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedWidth(cdewLastThickness = 2);
}

void cdewThickness3( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedWidth(cdewLastThickness = 3);
}

void cdewThickness4( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedWidth(cdewLastThickness = 4);
}

void cdewArrowDefault( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->arrowDefault();
}

void cdewUndo( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->undo();
}
void cdewAnimate( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    wwwBoolean t;
    w = getOglwinFromID( glutGetWindow() );
    t = w->autoStartRestartFlag;
    w->autoStartRestartFlag = true;
    w->transferToAnimation();
    w->autoStartRestartFlag = t;
}
void cdewTextEdit( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->transferToTextEditor();
}
void cdewAdd( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->addDiagram();
}
void cdewMerge( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->mergeDiagram();
}
void cdewMarkPath( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->markPath();
}
void cdewMarkPass( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->markPass();
}
void cdewMarkBeat( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->markBeat();
}
void cdewMarkJuggler( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->markJuggler();
}
void cdewMarkHands( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->markJugglersHand();
}
void cdewInvertMarks( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->invertMarked();
}
void cdewShiftMarksRight( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cycleMarksRight();
}
void cdewShiftMarksLeft( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cycleMarksLeft();
}
void cdewUnmarkPath( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->unmarkPath();
}
void cdewUnmarkBeat( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->unmarkBeat();
}
void cdewUnmarkJuggler( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->unmarkJuggler();
}
void cdewUnmarkHands( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->unmarkJugglersHand();
}
void cdewUnmarkAll( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->unmarkAll();
}
void cdewSetDefault( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->setDefaultHand();
}
void cdewAddJuggler( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->addJuggler();
}
void cdewRemoveJuggler( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->removeJuggler();
}
void cdewAddThrow( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->addThrow();
}
void cdewRemoveThrow( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->removeThrow();
}
void cdewAddReverse( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->invertDiagram();
}
void cdewAddSym( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    changeCellSizeFlag = true;
    w->addSymPattern();
    changeCellSizeFlag = false;
	
}
void cdewAddSame( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    changeCellSizeFlag = true;
    w->addSamePattern();
    changeCellSizeFlag = false;
}
void cdewDistribute( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->distributeThrows();
}
void cdewDistributePerHand( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->distributeThrowsPerHand();
}
void cdewRedrawAll( int wwDummy) {
//#pragma unused (wwDummy)
    redisplayAllCDEWWindows();
}
void cdewResetBows( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->resetBowFactor();
}
void cdewCycleLeft( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cyclePatternLeft();
}
void cdewCycleRight( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cyclePatternRight();
}
void cdewCycleUp( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cyclePatternUp();
}
void cdewCycleDown( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cyclePatternDown();
}
void cdewSwap1And2( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->swap1And2();
}
void cdewInvertMarked( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->invertMarked();
}
void cdewRotate( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->rotateDiagram();
}


void cdewInvertHeight( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->invertHeightForChristophesVisualisationModeFunc();
}
void cdewFixDelay( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->fixDelay();
}
void cdewMarkRight( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->markRight();
}
void cdewChangeHands( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->exchangeMarkedHands();
}
void cdewToggleBeepOnMarks( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->toggleBeepOnMarks();
}
void cdewBeepOff( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->beepOff();
}

void cdewToggleBounceOnMarks( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->toggleBounceOnMarks();
}

void cdewHalfSpinOnMarks( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->toggleHalfspinOnMarks();
}
void cdewTossAll( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->tossAll();
}

void cdewSign( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->setSignature(ownerSignature);
}


#if 01
void cdewScreenshot( int wwDummy) {
//#pragma unused (wwDummy)
	oglwin *w;
	w = getOglwinFromID( glutGetWindow() );
	w->screenshot();
}

void oglwin::screenshot( void ){	
    GLint w,h;
    char *buf;
    wwwBoolean r;
	
    char *p = (char *)malloc(strlen( mediaPath) + strlen(ScreenShotFileName) + 10 );
    if( p == 0 )
        return;
	
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);
	
    sprintf(p, "%s%s", mediaPath, ScreenShotFileName);
    printf("%s\n", p);
	
    r = screenShot (p, w, h);
    free( p);
	
    buf = readFile(getLastScreenshotFilename(), "rb");
	
    if(     buf != NULL) {
        char tempTitle[1000];
        char tempName[1000];
		
        char *t, nn[] = "untitled";
		char *tmpName;
		bool ret;
		
		
		
        sprintf(tempName, "%s", cdewFileName);
        t = strrchr( tempName, '.');
        if( t ) {
            *t = '\0';
            sprintf(tempName, "%s%s", tempName, fType);
        } else {
            sprintf(tempName, "%s%s", nn, fType);
			
        }
		
        if( cdewPathAndFileName == NULL )
            getcwd( cdewPathAndFileName, MAX_FULL_FILE_NAME);
		
		tmpName = getLastScreenshotFilename();
        if( tmpName == NULL ) {
            _myPrintf("1. %s\n2. %s\n", cdewPathAndFileName, tmpName);
            exit(0);
        }
		
		
        strcpy(tempTitle, cdewFileName);             // save pattern title
        ret = saveTextFileAs( buf, tempName, "save screenshot as..." );             // destroys pattern title
		
		if( ret )
			copyFile ( fullPatternPathName, tmpName);
		
        strcpy( cdewFileName, tempTitle);			// restore pattern title
		remove( tmpName );		
        free( buf );
		
    } else
        printf("can't make screenshot\n");
	
	
}
#endif


void cdewCellColors( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
	w->cellColor++;
    w->cellColor = w->cellColor % MaxCellColors;
}

void cdewuserDefArrowColor( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->cdewColorMode = LengthColorMode;
}
void cdewThrowDisplayMode( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->printSiteswap++;
    w->printSiteswap %= EndThrowDisplayMode;
}


void reCreateCausalEditorMenus( void ) {
    extern oglwin *firstCdewWindow;
    oglwin *p = firstCdewWindow;
    int id = glutGetWindow();
	
    while( p ) {
        glutSetWindow( p->id );
        p->reCreateCausalMenu( );
		
		
        p = p->next;
		
		
    }
    if( id )
        glutSetWindow( id );
	
}
void oglwin::reCreateCausalMenu( void ) {
    reCreateMenu( id, cdewMenuList);
	
}




void cdewColorResetAll( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->setMarkedArrowColor(w->causalCells, cdewLastColor = -1, allColorMode);
    w->setMarkedBackgroundColor(w->causalCells, cdewLastColor = -1);
	
}



void cdewLengthColor( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->cdewColorMode = LengthColorMode;
}
void cdewCausalColor( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w = getOglwinFromID( glutGetWindow()) ;
    w->colorPattern(true);
    w->cdewColorMode = LineColorMode;
}



void cdewNextColorMode( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
	
	if( ++w->cdewColorMode == EndOfColorMode){
		w->cdewColorMode = 0;
	}
	
/*	
    switch( w->cdewColorMode ) {
		default:
			
		case LengthColorMode:
			w->cdewColorMode = LineColorMode;
			break;
		case LineColorMode:
			w->cdewColorMode= JugglerColorMode;
			break;
		case JugglerColorMode:
			w->cdewColorMode = LengthColorMode;
			break;
    }
*/	
}


void cdewJugglerColor( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cdewColorMode = JugglerColorMode;
	
	
}

void cdewResetRepeat( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->resetRepeat();
	
}


void cdewToggleRepeatIn( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->toggleRepeatIn();
	
}

void cdewToggleRepeatOut( int wwDummy) {
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->toggleRepeatOut();
	
	
}


void cdewCalcStart( int wwDummy ){
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->calcStart();
}



void cdewSwitchToLastOrAnimationWindow( int wwDummy) {
//#pragma unused (wwDummy)
	switchToLastOrAnimationWindow(0);
	glutHideWindow();
	glutShowWindow();
}	


void cdewCycleThroughCdewWindows( int wwDummy) {
//#pragma unused (wwDummy)
    oglwin *w,*n;
	
    w = getOglwinFromID( glutGetWindow() );
    n = w->next;
	if( n == NULL )
		n = firstCdewWindow;
	_myPrintf("%03d\n", n->id );
	glutSetWindow(n->id);
	glutHideWindow();
	glutShowWindow();
}	

void cdewCut( int wwDummy ){
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->cutMarks();
}
void cdewCopy( int wwDummy ){
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->copyMarks();
}

void cdewPaste( int wwDummy ){
//#pragma unused (wwDummy)
	
    oglwin *w;
    w = getOglwinFromID( glutGetWindow() );
    w->startPaste();
}

void backgroundColorMode(  int wwDummy ){
//#pragma unused (wwDummy)
	
	inactiveBackgroundMode++;
	if( inactiveBackgroundMode >= cdewBackgroundEnd )	
		inactiveBackgroundMode = cdewBackgroundGrey;
	
	//	cdewRedrawAll( 0 );
}

void toggleTimeCursor( int wwDummy ){
//#pragma unused (wwDummy)
	
	oglwin *w;
	w = getOglwinFromID( glutGetWindow() );
	if( w->imTheAnimatedWindow){
		toggle(w->showTimeCursor);
		w->setTimeCursorDiagram(w->showTimeCursor);
	}
}


//muster speichern => flag gespeichert setzen und nicht noch einmal nachfragen
