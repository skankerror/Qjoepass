/*
 *  inputWindow.h
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
 -----------------------------------------------------------------------*/#ifndef __INPUT_WINDOW__
#define __INPUT_WINDOW__


#include "vtypes.h"

#if defined(__cplusplus)
extern "C"
{
#endif
	
#include "graphics.h"
	
	
#define _point punkt
	
	typedef struct _runaroundJuggler{ _point pos, posIn, posOut, poi, poiIn, poiOut;}_runaroundJuggler;
	
	typedef struct _fieldContent{   
		int c;
		_runaroundJuggler *j;
		int showInNewLine;
		bool selected;
		char *pattern;
		int x, y, w, h;
		Vector3D center;
	}_fieldContent;
	
	void                            drawContentInRect( _fieldContent *f,  int x, int y, int w, int h);
	void                            inputWindow( void );
	void                            closeInputWindow( void );
	
	void							runaroundLinePosition( int d),
									runaroundDropbackLinePosition( int d),
									runaroundSideToSidePosition( int d),
									runaroundBackToBackPosition( int d),
									runaroundFeedPosition( int d ),
									runaroundTerrorFeedPosition( int d ),
									runaroundCirclePosition( int d ),
									runaroundRotateCurrentScene(int d),
									runaroundRotateAllScenes(int d),
									runaroundRotateCurrentLine(int d),
									runaroundRotateSceneR( int d ),
									runaroundRotateSceneL( int d );
	
	
	void							_runaroundCirclePosition( _fieldContent *f ),
									_runaroundBackToBackPosition( _fieldContent *f );
	
	void							_runaroundRotateScene(	_fieldContent *fc, int degrees );
	
	
	extern float            		metersOnScreen;
	extern GLint            		inputID ;
	extern int                      cornersInRunaroundView;
	
#if 0
#define drawPosInRunaroundEditorFlag	jpPrefs.showJugglerPos
#define drawPoiInRunaroundEditorFlag	jpPrefs.showJugglerPoi
#define UseSameViewSwitchesInAnimationAndRunaround
#endif
#ifndef UseSameViewSwitchesInAnimationAndRunaround
	extern  int		      			drawPosInRunaroundEditorFlag;
	extern  int		      			drawPoiInRunaroundEditorFlag;
#endif
	extern  int		      			drawModifiersInRunaroundEditorFlag;
	
	extern char						*selectedRunaroundPattern;
	extern int						selectedRunaroundIndex;
	extern int						startingAngleOfFrame;
	extern bool						runaroundRotateAllScenesTogetherFlag;
	extern bool						runaroundRotateAllScenesFlag;
	extern bool						runaroundRotateCurrentLineFlag;
	extern bool						runaroundRotateCurrentSceneFlag;
	extern bool						runaroundShowAnimatedSceneFlag;
	extern bool						showGridFlag;
	extern bool						showPosFlag;
	extern bool						showPoiFlag;
	extern bool						showFrameFlag;
	extern bool						showSceneIndexFlag;
// saved in prefs:
	extern int						runaroundShowSceneIndexFlag;
	extern int						runaroundShowFrameFlag;
	
	
#if defined(__cplusplus)
}
#endif


#endif
