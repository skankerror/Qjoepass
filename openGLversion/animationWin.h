/*
 *  animationWin.h
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
#ifndef __ANIMWIN__
#define __ANIMWIN__

#include "mydefs.h"
#include "movie.h"
#include "openglmenu.h"

#if defined(__cplusplus)
extern "C++"
{
#endif
	
#define AnimationWindowTitle    "Joe's world"
	
    
	// material
	extern GLfloat					jugglerMaterialSpecular[];
	extern GLfloat					jugglerMaterialShininess[];
	extern GLfloat					jugglerMaterialEmission[];
	
	extern GLfloat					objMaterialSpecular[];
	extern GLfloat					objMaterialShininess[];
	extern GLfloat					objMaterialEmission[];		
	
	// light
	extern GLfloat					specular0[];
	extern GLfloat					diffuse0[];		
	extern GLfloat					ambient0[];		
	extern GLfloat					lmodel_ambient[];	
	
	extern GLint					animationWin;
	extern _menuList				*worldMenuList;
	
	extern int						viewMode;
	
	extern wwwBoolean               animationActive;
	extern  int                     jpAnimationFPS;
	
	extern wwwBoolean               showRate;
	extern wwwBoolean               redrawInfo;
	extern int                      theFrameRate;
	
	extern bool						jpAnimate;
	extern bool						jpUntilNextThrow;
	
	
	extern bool                     useTapAnimationValues;
	
	void	frontFunc( int dummy ),
	topFunc( int dummy ),
	birdFunc( int dummy ),
	nextPosFunc( int dummy ),
	moveCamFunc( int dummy ),
	openDocFunc( int dummy),
	animationQuit( int dummy);
	
	
	void	openPrechacthisFunc( int dummy);
	void	initAnimation( void );
	void	createAnimationWindow( void );
	void	toggleAnimationActive(int dummy);
	void	_toggleAnimationActive( bool flag  );
	
	void	switchToLastOrAnimationWindow( int wwDummy);
	
	void	drawScene(void);
	void	drawInfo( void );
	
	/* functions */
	void	visible(int vis);
	void	RedisplayAppWindows(void);
	void	recalcFrameRate( void );
	
	// animation and info share the same menus / keys
	void	worldKey(unsigned char key, int x, int y);
	void	worldSpecialKey(int theKey, int x, int y);
	
	void	setWorldTitle( char *t);
	
	extern	movie	*myMovie;
	
	
#if defined(__cplusplus)
}
#endif

#endif
