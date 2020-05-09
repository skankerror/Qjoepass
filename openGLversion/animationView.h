/*
 animationView.h
 
 Navigation in 3d-space
 move in and out
 shift left and right
 rotate around center
 
 created by wolfgang, 04.05.2009
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

#ifndef __ANIMATION_VIEW__
#define __ANIMATION_VIEW__

#if defined(__cplusplus)
extern "C"
{
#endif

	
#define DefMatrix {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}

	enum { TOP_VIEW, FRONT_VIEW, BIRD_VIEW, JUGGLER_VIEW, OBJECT_VIEW, VOMIT_VIEW, TESS_VIEW, MOVE_CAMERA_VIEW};
	
	
	void animationViewInit(GLint button);
	void animationViewSet( int );
	void animationViewReshape(int width, int height);
	void animationResetMagnification( void );
	void animationSetMatrix(GLfloat matrix[4][4]);
	void animationGetMatrix(GLfloat matrix[4][4]);
	void animationViewMouse(int button, int state, int x, int y);
	void animationViewMotion(int x, int y);
	void animationMatrix( void );	

	void animationCameraView( void );
	
	
#if defined(__cplusplus)
}
#endif


#endif

