/*
 * causal_editor.h
 * joe2
 *
 * header to integrate cdew in your program
 
 * Wolfgang Westerboer, 24.09.2001
 * westwolf@netcologne.de
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
#ifndef __CDEW__
#define __CDEW__
#if 01
#define USE_CDEW 1

#endif 


#ifdef USE_CDEW
#include "graphics.h"
#include "vtypes.h"
#include "mydefs.h"
#include "cdewClass.h"


void setDefaultCdewWindowSize( int x, int y, int w, int h );
void getDefaultCdewWindowSize( int *x, int *y, int *w, int *h );
extern 	int					cdewWindowDefaultX,
							cdewWindowDefaultY,
							cdewWindowDefaultW,
							cdewWindowDefaultH;
void staggerWindowPosition( int *x, int *y);
extern int					cdewStaggerMode;
extern	oglwin				*firstCdewWindow;


#define CDEW_INFORMATION_STRING "hide cdew information / do not interprete as siteswap"

enum			{jugglerDelayDrag, jugglerSwapDrag, jugglerNoDrag};
extern int		dragJugglerFlag;



#if defined(__cplusplus)
extern "C++"
{
#endif
	
	enum {cdewBackgroundGrey, cdewBackgroundWhite, cdewBackgroundEnd};
	extern int				inactiveBackgroundMode;
	
	
	extern 	bool			showErrors;
	extern bool				cellBackgroundFlag;
	extern int              maxCellColors ;
	extern sysColor         inactiveColor;
	extern sysColor         activeColor;
	extern float            timeCursorWidth;
	extern float            timeCursorOffset;
	extern Vector3D         timeCursorColor;
	
	extern char				*ownerSignature;
	extern wwwBoolean		cdewStartedPattern;
	extern bool				cdewAutoStart;
	extern bool             diffHeightFlag;
	
	extern char				*cdewStartPath;
	extern char             *cdewTemplatePath;
	extern  bool			cdewAutoOpen,
							autoFixConnections,
							showUnusedHandsInSyncMode,
							unhurriedDiagram,
							drawAsBowFlag,
							showMarkFlag,
							dontSetMeToTrueUnlessYouAreWWFlag;
	
	extern char             *cdewSelectPath;
	extern char             *cdew1StdPath;
	extern char             *cdewPrefPath;
	extern int				defaultThickness;
	
	void                    generateCdewPathNames(const char *sysD);
	
	oglwin					*initCausalEditor( char *title);
	
	void					_cdewMenu( int id);
	
	void					reCreateCausalEditorMenus( void );
	
	void                    setAllWindowsInactive( void );
	int						getActiveOglwin(void );
	
	int						getLastUsedWindow( void );
	int						makeOglwin( char *title );
	wwwBoolean				writePatternToNewOglwin( char *name, bool allowSameFilename );
	wwwBoolean				writePatternToOglwin( int id, char *name, bool allowSameFilename);
	void					setOglwinSize( int id );
	oglwin					*activateCausalEditor( void );
	void					exitCausalEditor( void );
	void					cdewJugglerColor( int wwDummy);
	
	void					redisplayAllCDEWWindows( void );
	void					markCdewPatternSaved( int id );
	void					setPatternSavedByUser( GLint id, wwwBoolean value);
	
	void					transferOnePersonNotationToCdewAndAutoSave( char *rawName );
	int						getOglwinOnePersonNotationID( void );
	
	extern bool				cdewSnap;
	
	void					_drawTimeCursor( int dummy );

	
#if defined(__cplusplus)
}
#endif

#endif

#endif

