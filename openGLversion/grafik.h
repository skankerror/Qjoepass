/*
 *  grafik.h
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


#ifndef __GRAFIK__
#define __GRAFIK__

#include "vtypes.h"

#if defined(__cplusplus)
extern "C++"
{
#endif



#define ColorsInList	14
#define OrbitColorMax	13
extern sysColor orbitColorList[];

#define COLOR_MAX               (1.0f)
//                                       red                green               blue
#define DefWhite                sysColor(COLOR_MAX * 1.0f, COLOR_MAX *  1.0f, COLOR_MAX *  1.0f)
#define DefBlack                sysColor(COLOR_MAX * 0.0f, COLOR_MAX *  0.0f, COLOR_MAX *  0.0f)
#define DefRed                  sysColor(COLOR_MAX * 1.0f, COLOR_MAX *  0.0f, COLOR_MAX *  0.0f)
#define DefGreen                sysColor(COLOR_MAX * 0.0f, COLOR_MAX *  1.0f, COLOR_MAX *  0.0f)
#define DefBlue                 sysColor(COLOR_MAX * 0.0f, COLOR_MAX *  0.0f, COLOR_MAX *  1.0f)
#define DefYellow               sysColor(COLOR_MAX * 0.9f, COLOR_MAX *  0.9f, COLOR_MAX *  0.0f)
#define DefDarkGray             sysColor(COLOR_MAX * 0.2f, COLOR_MAX *  0.2f, COLOR_MAX *  0.2f)
#define DefLightGray            sysColor(COLOR_MAX * 0.8f, COLOR_MAX *  0.8f, COLOR_MAX *  0.8f)
#define DefBrown                sysColor(COLOR_MAX * 0.6f, COLOR_MAX *  0.4f, COLOR_MAX *  0.2f)
#define DefSpec                 sysColor(COLOR_MAX * 0.4f, COLOR_MAX *  0.4f, COLOR_MAX *  0.4f)

extern int              dLineHeigth;

extern sysColor lineColor;
extern sysColor textColor;
extern sysColor ballColor;
extern sysColor eventColor1;
extern sysColor eventColor2;
extern sysColor white;
extern sysColor black;
extern sysColor spec;
extern sysColor red;
extern sysColor green;
extern sysColor darkGray;
extern sysColor lightGray;
extern sysColor blue;
extern sysColor yellow;
extern sysColor brown;


void initGrafik( void );
void stroke_output(GLfloat x, GLfloat y, GLfloat z, char *format,...);
void setColorFromVector(sysColor c);
void drawLine3( punkt *a, punkt *b, float width, sysColor c);
void drawCylinderLine3(  punkt a, punkt b, float baseRadius, float topRadius,  sysColor c );
void drawBall3( punkt *a, float size, int parts, sysColor c);
void drawDisk3( punkt *a, float innerRadius, float outerRadius, int parts, sysColor *c);
sysColor getListColor( int c );
sysColor _getListColor( int c, sysColor *l, const int listEntries);

//void setColor(sysColor rgb, float shine);


#if defined(__cplusplus)
}
#endif

#endif

