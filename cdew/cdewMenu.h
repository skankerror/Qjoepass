/*
 * cdewMenu.h
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

#ifndef __CDEW_MENU__
#define __CDEW_MENU__


#if defined(__cplusplus)
extern "C++"
{
#endif

void 	cdewSavePS(int wwDummy),
		cdewNewWin(int wwDummy),		
		cdewDuplicateWin(int wwDummy),
		cdewOpen(int wwDummy),
		cdewSave(int wwDummy),
		cdewSaveAs(int wwDummy),
		cdewSaveAuto(int wwDummy),
		cdewCloseWin(int wwDummy),
		cdewTemplate(int wwDummy),
		cdewFixNow(int wwDummy),
		cdewSingleLineMode(int wwDummy),
		cdewSyncLineMode(int wwDummy),
		cdewColorLineMode(int wwDummy),
		cdewColorResetAll(int wwDummy),
		cdewLengthColor(int wwDummy),
		cdewuserDefArrowColor(int wwDummy),
		cdewCausalColor(int wwDummy),
		cdewJugglerColor( int wwDummy),
		cdewCellColors(int wwDummy),
		cdewReadHeader(int wwDummy),
		cdewWriteHeader(int wwDummy),
		cdewClearHeader(int wwDummy),
		cdewClear(int wwDummy),
		cdewTextEdit(int wwDummy),
		cdewAnimate(int wwDummy),
		cdewUndo(int wwDummy),
		cdewRedo(int wwDummy),
//		cdewNextColor(int wwDummy),
//		cdewNextThickness(int wwDummy),
		cdewBackgroundColorReset( int wwDummy),
		cdewBackgroundColor1( int wwDummy),
		cdewBackgroundColor2( int wwDummy),
		cdewBackgroundColor3( int wwDummy),
		cdewBackgroundColor4( int wwDummy),
		cdewBackgroundColor5( int wwDummy),
		cdewBackgroundColor6( int wwDummy),
		cdewBackgroundColor7( int wwDummy),
		cdewBackgroundColor8( int wwDummy),	
		cdewArrowColorReset( int wwDummy),
		cdewArrowColor1( int wwDummy),
		cdewArrowColor2( int wwDummy),
		cdewArrowColor3( int wwDummy),
		cdewArrowColor4( int wwDummy),
		cdewArrowColor5( int wwDummy),
		cdewArrowColor6( int wwDummy),
		cdewArrowColor7( int wwDummy),
		cdewArrowColor8( int wwDummy),
		cdewThicknessReset(int wwDummy),
		cdewThickness1(int wwDummy),
		cdewThickness2(int wwDummy),
		cdewThickness3(int wwDummy),
		cdewThickness4(int wwDummy),
		cdewArrowDefault(int wwDummy),
		cdewAdd(int wwDummy),
		cdewMerge(int wwDummy),
		cdewMarkPath(int wwDummy),
		cdewMarkPass(int wwDummy),
		cdewMarkBeat(int wwDummy),
		cdewMarkJuggler(int wwDummy),
		cdewMarkHands(int wwDummy),
		cdewInvertMarks(int wwDummy),
		cdewShiftMarksLeft(int wwDummy),
		cdewShiftMarksRight(int wwDummy),
		cdewUnmarkPath(int wwDummy),
		cdewUnmarkBeat(int wwDummy),
		cdewUnmarkJuggler(int wwDummy),
		cdewUnmarkHands(int wwDummy),
		cdewUnmarkAll(int wwDummy),
		cdewSetDefault(int wwDummy),
		cdewAddJuggler(int wwDummy),		    
		cdewRemoveJuggler(int wwDummy ),
		cdewAddThrow(int wwDummy ),
		cdewRemoveThrow(int wwDummy ),
		cdewAddReverse(int wwDummy ),
		cdewAddSym(int wwDummy ),
		cdewAddSame(int wwDummy ),
		cdewDistributePerHand(int wwDummy ),
		cdewDistribute(int wwDummy ),
		cdewRedrawAll(int wwDummy ),
		cdewResetBows(int wwDummy ),
		cdewCycleLeft(int wwDummy ),
		cdewCycleRight(int wwDummy ),
		cdewCycleUp(int wwDummy ),
		cdewCycleDown(int wwDummy ),
		cdewSwap1And2( int wwDummy),
		cdewInvertMarked(int wwDummy ),
		cdewRotate(int wwDummy ),
		cdewInvertHeight(int wwDummy ),
		cdewFixDelay(int wwDummy ),
		cdewMarkRight(int wwDummy ),
		cdewChangeHands(int wwDummy ),
		cdewToggleBeepOnMarks(int wwDummy ),
		cdewBeepOff(int wwDummy ),
		cdewToggleBounceOnMarks(int wwDummy ),
		cdewHalfSpinOnMarks(int wwDummy ),
		cdewTossAll(int wwDummy ),
		cdewThrowDisplayMode( int dummy ),
		cdewNextDisplayMode(int wwDummy ),
		cdewNextColorMode(int wwDummy ),
		cdewSign(int wwDummy ),
		cdewQuit(int wwDummy),
		cdewScreenshot(int wwDummy),
        cdewToggleRepeatIn( int wwDummy),
        cdewToggleRepeatOut( int wwDummy),
        cdewResetRepeat( int wwDummy),
		cdewSwitchToLastOrAnimationWindow( int wwDummy),
		cdewCycleThroughCdewWindows( int wwDummy),
		cdewCut( int dummy ),
		cdewCopy( int dummy ),
		cdewPaste( int dummy ),
		backgroundColorMode( int dummy)
	//		cdewAnimationActive(int wwDummy)
				
		;


#if defined(__cplusplus)
}


#endif
#endif
