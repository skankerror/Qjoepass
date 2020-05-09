/*
 * cdewDraw.cpp
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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


#include "fileaccess.h"
#include "fileIO.h"
#include "cdewDraw.h"
#include "cdewClass.h"

#include "cmdLine.h"
#include "cdewDefs.h"

// drawing routines for absolute positioning
// set position with setPosition
// drawing routines will push / pop position before transalting to new position

FILE			*psfp, *temp;
const char      *PSFileName     =       "causal.ps";
//char			*psFooter;
int             printThisWindowAsPostscript = 0;
wwwBoolean      printPS;
wwwBoolean      psSaved;
bool            colorPostscript = false;

static float    xPos = 0, yPos = 0, myWidth;


#define DOES_POSTSCRIPT                         01
#define PS_DISABLED_MESSAGE                     "postscipt output is not active"

const char * psHead =
"%!PS-Adobe-3.0 EPSF-3.0" NL;

const char *psClip =
"% clip to window borders" NL
"%%BoundingBox: ";

const char *psInfo =
"% cdew, version 1.8" NL
"% causal diagram editor" NL
"% by wolfgang westerboer, westwolf@gmx.de" NL;

const char * psHeader =
"% Units are in points, 72.27 to an inch, I think." NL
"/inch {1 mul} def" NL
"% Set a variable to hold the leftHand margin of .20 inch." NL
"/xmargin .20 inch def" NL
"% Some possible fonts in different sizes." NL
"/f20 {/Times-Bold findfont 20 inch scalefont setfont} def" NL
"/f12 {/Times-Roman findfont 12 inch scalefont setfont} def" NL
"% Define command to get to top of page location." NL
"/StartPage {xmargin 10 inch moveto} def" NL
"% Define carriage return effect to get to leftHand margin.  Maintains y position." NL
"/cr {xmargin currentpoint exch pop moveto} def" NL
"% Define linefeed effect (this spacing is independent of the font size!)" NL
"/lf {0 -25.00 rmoveto} def" NL
"StartPage" NL
"% Our small font (see above)." NL
"f12" NL;

const char *psFooter =
"% Following causes the stuff on the current page to actually be printed." NL
"showpage" NL
"% end of postscript output by cdew";





// postscript initialisation
void oglwin::initPostscript( int w, int h )
{
	time_t  t;
	temp = psfp;
	if( printThisWindowAsPostscript != glutGetWindow())
		return;
	
	printThisWindowAsPostscript = 0;
	
#if !DOES_POSTSCRIPT
	if( psfp )
		fprintf(psfp, "(%s) show" NL, PS_DISABLED_MESSAGE);
	psfp = NULL;
	return;
	
#endif
	
	if( printPS)
	{       psfp = fopen( PSFileName,"w");
		
		if( psfp )
			psSaved = true;
		printPS = false;
	}else
		psfp = 0;
	
	if( psfp ){
		fprintf(psfp, "%s" NL, psHead);
		fprintf(psfp, "%s 0 0 %d %d " NL, psClip, w, h);
		fprintf(psfp, "%s" NL, psInfo);
		time (&t);
		fprintf(psfp,"%%time is %s " NL NL, ctime(&t ));
		fprintf(psfp, "%s" NL, psHeader);
	}
}

void oglwin::endPostscript( void )
{
	if( psfp )
	{
		fprintf(psfp, "%s", psFooter);
		fclose (psfp );
		psfp = 0;
		if( !cmdLineMakePostscript){
			char *dot;
			char name[1000];
			char *buf;
			
			strcpy(name, cdewFileName);
			
			dot = strrchr( name, '.');
			if( dot )
				*dot = '\0';
			strcat(name, ".ps");
			
			buf = readFile( (char *)PSFileName, "r");
			if( buf ) {
				char tempTitle[1000];
#ifdef USE_CREATOR_TYPES
				// save as photoshop file
				OSType t = getCreator();
				setCreator( '8BIM' );
#endif
				
				strcpy(tempTitle, cdewFileName);            // save cdew filename
				saveTextFileAs( buf, name, "add file selector info here" );                // destroys cdew filename
				strcpy( cdewFileName, tempTitle);			// restore cdew filename
				
				free(buf);
#ifdef USE_CREATOR_TYPES
				// restore old creator
				setCreator( t );
#endif
			}
			
		}
		
        psfp = temp;
	}
}


void oglwin::printThisWindowPS( void )
{
	printThisWindowAsPostscript = glutGetWindow();
}

void oglwin::removePostscriptFile( void )
{
	if( psSaved )
		remove( PSFileName );
}


void pushMatrix( void )
{
	glPushMatrix( );
	// no postscrip equivalent
	
}

void popMatrix( void )
{
	glPopMatrix();
	
	// no postscrip equivalent
}


void setLineWidth( float w)
{
	myWidth = w;
	
	glLineWidth( w);
	if( psfp )
		fprintf(psfp, NL "%d setlinewidth" NL, (int)w);
}

float getLineWidth( void )
{
    return myWidth;
}


void smallFont( void )
{
	if( !psfp)
		return;
	fprintf(psfp, "f12" NL);
}

void bigFont( void )
{
	if( !psfp)
		return;
	fprintf(psfp, "f20" NL);
}

void setPosition( float x, float y)
{
	setGLPosition(x, y);
	setPSPosition( x, y );
}

void setGLPosition( float x, float y)
{
	
	xPos = x;
	yPos = y;
}

void setPSPosition( float x, float y)
{
	if( !psfp)
		return;
	fprintf(psfp, "%f inch %f inch moveto" NL, x,y);
	
}

void setColor( float x, float y, float z)
{
	
	openGLColor( x,y,z);
	postscriptColor( x,y,z);
}

void openGLColor(  float x, float y, float z)
{
	glColor3f (x,y,z);
}

void postscriptColor( float x, float y, float z)
{
	if( !psfp)
		return;
	
	if( colorPostscript )
		_postscriptColor(x,y,z);
	else
		_postscriptColor(0,0,0);
}

void _postscriptColor( float x, float y, float z)
{
	
	if( psfp)
		fprintf(psfp, "%f %f %f setrgbcolor" NL, x,y,z);
}


void drawOneLine( float x1, float y1, float x2, float y2)
{
	
	openGLLine( x1, y1, x2, y2);
	postscriptLine( x1,y1,x2,y2);
}

void openGLLine( float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f ((x1),(y1) );
	glVertex2f ((x2),(y2) );
	glEnd();
}

void postscriptLine( float x1, float y1, float x2, float y2)
{
	if( !psfp)
		return;
	
	fprintf(psfp, "%f inch  %f inch moveto  %f inch %f inch lineto stroke" NL,
			x1, y1, x2, y2);
}

void drawQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	
	openGLQuad(x1,y1,x2,y2, x3, y3, x4, y4);
	postscriptQuad(x1,y1,x2,y2, x3, y3, x4, y4);
}

void openGLQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	glBegin(GL_QUADS);
	glVertex2f( x1, y1 );
	glVertex2f( x2, y2 );
	glVertex2f( x3, y3 );
	glVertex2f( x4, y4 );
	glEnd();
}

void postscriptQuad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// postscript
	if( !psfp)
		return;
	fprintf(psfp, "%f inch %f inch moveto" NL, x1,y1);
	fprintf(psfp, "%f inch %f inch lineto" NL, x2,y2);
	fprintf(psfp, "%f inch %f inch lineto" NL, x3,y3);
	fprintf(psfp, "%f inch %f inch lineto" NL, x4,y4);
	fprintf(psfp, "fill" NL);
}




void drawString( char *s )
{
	openGLString( s, 1.f );
	postscriptString( s );
}

void openGLString( char *s, float size )
{
	size_t i;
	
	glPushMatrix();
	glTranslatef( xPos - 4, yPos - 4, 0);
	glScalef( .1, .1, .1);
	glScalef( size, size, size);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	
	glPopMatrix();
}

void postscriptString( char *s )
{
	
	if(! psfp)
		return;
	fprintf(psfp, "%d inch %d inch moveto" NL, (int) xPos - 4, (int) yPos - 3);
	
	fprintf(psfp, "(%s) show" NL, s);
}


void drawCurve(  Vector3D v1, Vector3D v2, Vector3D v3, Vector3D v4)
{
	Vector3D ctrlpoints[4] = {
        Vector3D(-4.0, -4.0, 1.0),
        Vector3D( -2.0, 4.0, 1.0),
        Vector3D(2.0, -4.0, 1.0),
	Vector3D(4.0, 4.0, 1.0)};
	int i;
	
	ctrlpoints[0] = v1;
	ctrlpoints[1] = v2;
	ctrlpoints[2] = v3;
	ctrlpoints[3] = v4;
	
	
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, (GLfloat *)ctrlpoints);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat) i/30.0);
	glEnd();
	glDisable(GL_MAP1_VERTEX_3);
	
	
	if( psfp)
	{       fprintf(psfp,   "%f inch %f inch moveto "
					"%f inch %f inch  %f inch %f inch %f inch %f inch "
					"curveto stroke" NL,
					v1.x, v1.y, v2.x, v2.y,  v3.x, v3.y, v4.x, v4.y);
	}
}


void drawDisk( double in, double out )
{
	GLUquadricObj   *theQuadric     = 0;
	
	
	glPushMatrix();
	glTranslatef( xPos, yPos, 0 );
	
	theQuadric = gluNewQuadric();
	gluDisk(theQuadric, in, out, 20, 20);
	gluDeleteQuadric( theQuadric );
	
	glPopMatrix();
	
	if( psfp)// %f inch  %f inch  , centerX, centerY+y1
		fprintf(psfp, "%d inch %d inch 10 inch 0 360 arc stroke" NL, (int)xPos, (int)yPos);
}

void drawChar( char c )
{
	glPushMatrix();
	
	glTranslatef( xPos, yPos, 0 );
	glutStrokeCharacter     (GLUT_STROKE_MONO_ROMAN, c );
	
	glPopMatrix();
}


