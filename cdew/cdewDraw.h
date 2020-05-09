/*
 * cdewWork.cpp
 * joe2
 * drawing routines for cdew
 * both postscript and openGL
 * for internal use of cdew routines

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
#if defined(__cplusplus)
extern "C"
{
#endif

#include "vtypes.h"
#include "mydefs.h"




void drawOneLine( float x1, float y1, float x2, float y2);
void drawQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)  ;
void drawString( char *s );
void drawCurve(  Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4);
void drawDisk( double in, double out );
void drawChar( char c );

void setLineWidth( float w);
float getLineWidth( void);
void setColor( float x, float y, float z);
void setPosition( float x, float y);

void setGLPosition( float x, float y);
void setPSPosition( float x, float y);
	
	
void bigFont( void );
void smallFont( void );


void openGLLine( float x1, float y1, float x2, float y2);
void postscriptLine( float x1, float y1, float x2, float y2);
void openGLColor(  float x, float y, float z);
void postscriptColor( float x, float y, float z);
void _postscriptColor( float x, float y, float z);
void openGLQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void postscriptQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void openGLString( char *s, float size );
void postscriptString( char *s );

void pushMatrix( void );
void popMatrix( void );


extern  wwwBoolean      psSaved;
extern  wwwBoolean      printPS;
extern  bool            colorPostscript;



#if defined(__cplusplus)
}
#endif
