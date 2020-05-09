/*
 *  grafik.cpp
 *  joe2
 *
 * created by wolfgang on 20.04.2009
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
/*
 calls to openGL grafik system
 */
#include "graphics.h"

#include "vtypes.h"
#include "headers.h"



#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include "prefDef.h"
#include "grafik.h"
//#include "vmath.h"
#include "vtypes.h"
#include "dxf.h"

#define gluQuadricTexture _gluQuadricTexture
void _gluQuadricTexture(GLUquadricObj* o, GLboolean t);

#define drawOneLine(x1,y1,z1,x2,y2,z2)  glBegin(GL_LINES);  \
glVertex3f ((x1),(y1),(z1) ); glVertex3f ((x2),(y2),(z2) ); glEnd();
int                     dLineHeigth;
//sysColor      farbListe[];
sysColor        white(DefWhite);
sysColor        black(DefBlack);
sysColor        spec(DefSpec);
sysColor        red(DefRed);
sysColor        green(DefGreen);
sysColor        darkGray(DefDarkGray);
sysColor        lightGray(DefLightGray);
sysColor        blue(DefBlue);
sysColor        yellow(DefYellow);
sysColor        brown(DefBrown);
//float luminance = .8f;
// first 20 colors are nice. rest calculated -> boring
sysColor colorList[BALL_MAX] = {
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 0.f, COLOR_MAX * 0.f),  // red
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 0.f, COLOR_MAX * 1.f),  // blue
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 1.f, COLOR_MAX * 0.f),  // green
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 1.f, COLOR_MAX * 0.f),  // yellow
sysColor(COLOR_MAX *.75f, COLOR_MAX * 0.f, COLOR_MAX * 1.f),  //lila
sysColor(COLOR_MAX *.30f, COLOR_MAX * .3f, COLOR_MAX * .8f),  // flieder
sysColor(COLOR_MAX *.55f, COLOR_MAX * 1.f, COLOR_MAX * 0.f),  // grasgrn
sysColor(COLOR_MAX *.80f, COLOR_MAX * 0.f, COLOR_MAX * 0.f),  // dunkelrot
sysColor(COLOR_MAX *.0f,  COLOR_MAX * .5f, COLOR_MAX * 0.f),  //dunkelgrn
sysColor(COLOR_MAX *.05f, COLOR_MAX *.05f, COLOR_MAX * .8f),  //dunkelblau
sysColor(COLOR_MAX *.50f, COLOR_MAX * 0.f, COLOR_MAX * 1.f),  // lila
sysColor(COLOR_MAX *1.0f, COLOR_MAX *.50f, 0),               // orange
sysColor(COLOR_MAX *0.0f, COLOR_MAX *.75f, COLOR_MAX *.75f), // trkis
sysColor(COLOR_MAX * .2f, COLOR_MAX * .2f, COLOR_MAX * .4f)   // blue
};

sysColor orbitColorList[OrbitColorMax] = {
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 1.f, COLOR_MAX * 1.f), 
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 0.f, COLOR_MAX * 0.f), 
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 1.f, COLOR_MAX * 0.f), 
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 0.f, COLOR_MAX * 1.f),
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 1.f, COLOR_MAX * 1.f),
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 0.f, COLOR_MAX * 1.f), 
sysColor(COLOR_MAX * 1.f, COLOR_MAX * 1.f, COLOR_MAX * 0.f),
sysColor(COLOR_MAX * 0.5f, COLOR_MAX * 0.f, COLOR_MAX * 0.f), 
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 0.5f, COLOR_MAX * 0.f), 
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 0.f, COLOR_MAX * 0.5f),
sysColor(COLOR_MAX * 0.f, COLOR_MAX * 0.5f, COLOR_MAX * 0.5f),
sysColor(COLOR_MAX * 0.5f, COLOR_MAX * 0.f, COLOR_MAX * 0.5f), 
sysColor(COLOR_MAX * 0.5f, COLOR_MAX * 0.5f, COLOR_MAX * 0.f)
};

void initGrafik( void )
{
	int c;
	
	for( c = 0; c < ColorsInList; c++)
		colorList[ c ] = colorList[ c ] * jpPrefs.luminance;
}

sysColor getListColor( int c ){
	
	return _getListColor( c, colorList, ColorsInList);
}	

sysColor _getListColor( int c, sysColor *l, const int listEntries)
{	sysColor r;
	if( c < 0 )
		r = white;
	else
		r = l[c % listEntries];
	
	return r;
}

/*
 void setColor(sysColor rgb, float shine)
 {
 #ifdef MAC_CLASSIC_VERSION
 #pragma unused( shine )
 #endif
 GLfloat v[4];
 v[0]= rgb.x;
 v[1]= rgb.y;
 v[2]= rgb.z;
 v[3]= 1.;
 return;
 }
 */

void stroke_output(GLfloat x, GLfloat y, GLfloat z, char *format,...)
{
	va_list args;
	char buffer[200], *p;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	glPushMatrix();
	glTranslatef(x, y, z );
#define S .1f
	glScalef(S, S , S );
	for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
}

void setColorFromVector(sysColor c) {
    glColor3f (c.x, c.y, c.z);
}
/*
 scaling, to rezize all coordinates stuff, but NOT the size,
 I multiply all points with a given faktor (1/100)
 */
void drawLine3( punkt *a, punkt *b, float width, sysColor c)
{       punkt p1, p2;
	GLfloat         w;
	
    glGetFloatv( GL_LINE_WIDTH, &w);
	glLineWidth( width );
	p1 = *a;
	p2 = *b;
	
	glPushMatrix();
	setColorFromVector(c);
	
	glBegin(GL_LINES);
	glLineWidth( width);
	glVertex3f (p1.x, p1.y, p1.z);
	glVertex3f (p2.x, p2.y, p2.z);
	glEnd();
	
	glPopMatrix();
	glLineWidth( w );
}

/*
 keine Linien, sondern Konusse zeichnen.
 Konus in gewnschter Gre
 (steht immer im Ursprung, immer nach Z orientiert)
 transformieren:
 Richtungsvektor berechnen
 Konus zu diesem Richtungsvektor rotieren (skalarProdukt mit Koordinatenachsen) Orientierung beachten
 An Koordinatenursprung verschieben (hiermit als punkt a, erster Parameter ) definiert
 */
void drawBall3( punkt *a, float radius, int parts, sysColor c)
{       punkt p1;
	GLfloat w;
	GLUquadricObj*theQuadric;
	
	theQuadric = gluNewQuadric();
	gluQuadricTexture(theQuadric, GL_TRUE);
    glGetFloatv( GL_LINE_WIDTH, &w);
	p1 = *a;
	
	setColorFromVector(c);
	glLineWidth( 1 );
	
	glPushMatrix();
    glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(180,1,0,0);
	glRotatef(90,0,1,0);
	gluSphere(theQuadric, radius  , parts, parts);
    glPopMatrix();
	glLineWidth( w );
	gluDeleteQuadric( theQuadric);
}


void drawDisk3( punkt *a, float innerRadius, float outerRadius, int parts, sysColor *c)
{       punkt p1;
	GLfloat w;
	GLUquadricObj*theQuadric;
	
	theQuadric = gluNewQuadric();
	gluQuadricTexture(theQuadric, GL_TRUE);
    glGetFloatv( GL_LINE_WIDTH, &w);
	p1 = *a;
	
	setColorFromVector(*c);
	glLineWidth( 1 );
	
	glPushMatrix();
    glTranslatef(p1.x, p1.y, p1.z);
	glRotatef(180,1,0,0);
	glRotatef(90,0,1,0);
	gluDisk(theQuadric, innerRadius, outerRadius, parts, parts);
    glPopMatrix();
	glLineWidth( w );
	gluDeleteQuadric( theQuadric);
}

void drawCylinderLine3(  punkt a, punkt b, float baseRadius, float topRadius, sysColor c )
{
	Vector3D d;
	float l;
	GLUquadricObj*theQuadric;
	theQuadric = gluNewQuadric();
	gluQuadricTexture(theQuadric, GL_TRUE);
	gluQuadricOrientation(theQuadric,GLU_OUTSIDE);
	d = a - b;
	l = d.betrag();
	d = d.normalised();
	setColorFromVector(c);
	glPushMatrix();
	glTranslatef(a.x, a.y, a.z);
	d.translateZObjectOnVektor();
	glRotatef(180,0,1,0);
	
	gluCylinder(theQuadric,baseRadius,topRadius ,l , jpPrefs.levelOfDetail,1);
	//     drawOneLine( 0,0,0, 0,50,0);
	
	glPopMatrix();
	
	gluDeleteQuadric(theQuadric);
}

void _gluQuadricTexture(GLUquadricObj* o, GLboolean t)
{
	return;
}

