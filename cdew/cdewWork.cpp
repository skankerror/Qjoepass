/*
 * cdewWork.cpp
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

#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>



#include "myutil.h"

#include "parseSiteswap.h" 
#include "causal_editor.h"
#include "globals.h"
#include "cdewClass.h"
#include "cdewDraw.h"
#include "openglmenu.h"
#include "grafik.h"
#include "myPrintf.h"
#include "vmath.h"



int		feedbackX = -1, feedbackY;
bool	diffHeightFlag = true;
bool	noArrowMFFlag = true;

#pragma mark -------- drawing routines


void oglwin::drawEditorContent( void )
{
	int j, e;
	_causalCell *p;
	
	wwwBoolean undoTempState = oglwinUndoState;
	oglwinUndoState = undoDeactivated;
	
	colorPattern( cdewColorMode == LineColorMode );
	
	oglwinUndoState = undoTempState;
	
	markFaults();
	
	initPostscript( windowWidth, windowHeight);
	
	if( jugglerLines )
	{       setColor (.7f, .7f, .7f);
		for( j = 1; j < cdewJugglers; j++ )
		{
			drawOneLine( 0, lineHeight*j, windowWidth,lineHeight*j);
		}
	}
	// draw all cells
	for( j = 0; j < cdewJugglers; j++ )
	{
		float v;
		int h;
		
		for( h = 0; h < 2; h++ )
		{
			v = *getDelayedJuggler( j, h);
			for( e = int(-v-1) ; e < cdewEntries-v+1; e++ ){
				drawCellBackground( j,h,e);
			}
		}
	}
	
	drawChristophesNotationInfo();
	
	
	// draw different modes
	if( theMouseInputMode)
		drawMimMode();
	
	if( showNumbers )
		drawNumberOfBeats();
	
	if( showObjects )
		drawNumberOfObjects();
	
	
	if( showStart )
		drawStartDistribution();
	
	if( userDoesJugglerDelayDrag || showDelays)
		drawDelayValue();
	
	// draw the causal arrows
	for( j = 0; j < cdewJugglers; j++ )
	{
		int v;
		int h;
		
		for( h = 0; h < 2; h++)
		{
			v = (int)*getDelayedJuggler( j, h);
			
			for( e = -v-1; e < cdewEntries +2 - v + 1; e++ )
			{       int i;
				p = getCausalCell(causalCells, j,h,e, cdewJugglers, cdewHands, cdewEntries);
				i = 0;
				do
				{
					
					drawCellContent( j,h,e-i);
					
					i += cdewEntries;
				}while( e+p->length-i >= -2 );
			}
		}
	}
	
	
	endPostscript();
	
	drawDisplayMode();
	drawColorMode();	
}





void oglwin::drawCellContent( int j, int hnd, int e)
{
	_causalCell *p;
	
	p = getCausalCell(causalCells, j,hnd,e, cdewJugglers, cdewHands, cdewEntries);
	
	_drawCellContent( j, hnd, e );
	
	
}



// used when a cdewMouse click is made, that is, a cell is selected
// dont check for marks, as we may want to add one
void oglwin::calcCenterOfCell( int j, int hnd, int e, float *centerX, float *centerY)
{
	wwwBoolean sync = false;
	_causalCell *p1, *p2;
	int     cellWidth = windowWidth/cdewEntries;
	float d;
	
	/*_calcCenterOfCell( j, hnd, e, centerX, centerY);
	 return;
	 */
	/*      if( e > cdewEntries / 2 )
	 j *= 2;
	 */
	p1 = getCausalCell(causalCells, j,leftHand,e, cdewJugglers, cdewHands, cdewEntries);
	p2 = getCausalCell(causalCells, j,rightHand,e, cdewJugglers, cdewHands, cdewEntries);
	/*#ifdef MultiLineMode
	 if( e > cdewEntries / 2 )
	 j *= 2;
	 #endif
	 */
	// heres the difference
	// would flicker, if checked for sync with subroutine isSyncThrow()
	sync = ((p1->length != absorb  ) && ( p2->length != absorb ) );
	d = cellWidth * *getDelayedJuggler(j, hnd);
	*centerX = windowWidth / cdewEntries * (e+.5) + d;
	if( displayMode == syncLineMode || sync )
		*centerY =  windowHeight - lineHeight2 * (j*2  +  (hnd == leftHand? 1:0)  +.5) ;
	else
		*centerY =  (float)windowHeight - lineHeight2 * (j*2  + 1);
	
	
}


// calculates adress of the cell at cdewMouse position
void oglwin::getCellFromMousePosition( int x, int y, int *j, int *hnd, int *e)
{
	_causalCell *p1, *p2;
	int cellWidth, cellHeightJ, cellHeightH;
	int hi;
	float delayOffset;
	// calculate cell size
	cellWidth = windowWidth/cdewEntries;
	cellHeightJ = lineHeight;
	cellHeightH =lineHeight2;
	*j =  y / lineHeight;
	hi =  y / lineHeight2;
	*hnd = hi%2?leftHand:rightHand;
	
	
	
#define FAKTOR ( 30)
	// if the cdewMouse moves leftHand or rightHand out of the window, jump to a new cell every few pixles
	if( x > windowWidth )
	{       int o;
		
		o = x;
		while( o > windowWidth )
			o -= windowWidth;
		o /= FAKTOR;
		
		x += o * cellWidth;
	}else if( x < 0 )
	{       int o;
		
		o = -x;
		o /= FAKTOR;
		
		x -= o * cellWidth;
	}
	delayOffset = *getDelayedJuggler(*j, *hnd) * cellWidth;
	// if the selected cell lies before the fist cell, the calculated index is 1 too high
	if( x < delayOffset )
		x -= cellWidth;
	x -= (int)delayOffset;
	
	*e =  x / cellWidth ;
	
	p1 = getCausalCell( causalCells, *j, *hnd, *e, cdewJugglers, cdewHands, cdewEntries);
	p2 = getCausalCell( causalCells, *j, switchHand(*hnd), *e, cdewJugglers, cdewHands, cdewEntries);
	
	if( p1->invertHandHeightOffset || p2->invertHandHeightOffset )
		*hnd = switchHand( *hnd );
	
}

void oglwin::setMotionFeedback( int x, int y)
{
	
	int j, h,e;
	_causalCell *p;
    static int lastJ = -1, lastH, lastE;
	
	
	
	feedbackX = x;
	feedbackY = windowHeight - y;
	return;
	
	allCells(
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 p->feedbackFlag = false;
			 )
	
	
	getCellFromMousePosition( x,y, &j, &h, &e);
	
    if(  j >= cdewJugglers )
        j = -1;
    if( h >= cdewHands )
        h = -1;
    if( e >= cdewEntries )
        e = -1;
	
    if( j < 0 || h < 0 || e < 0 )
        goto bail;
	
	p = getCausalCell( causalCells, j, h, e, cdewJugglers, cdewHands, cdewEntries);
	
	if( mouseState == false )
		return;
	
	p->feedbackFlag = true;
	
bail:
	if(1|| lastJ != j || lastH != h || lastE != e )
		glutPostRedisplay();
	
    lastJ = j;
    lastH = h;
    lastE = e;
	
}


void oglwin::_drawCellContent( int j, int hnd, int e)
{
	_causalCell *p, *pSync,*pDest,*pDestSync;
	int jDest,hDest,eDest;
	int ei;
	wwwBoolean sourceIsSync, destIsSync;
	GLfloat width;
	wwwBoolean printHand;
	sysColor col;
	int colIndex;
	char s[1000];
	float centerX, centerY, destCenterX, destCenterY;
	float originalX, originalY, destOriginalX, destOriginalY;
	int     cellWidth = windowWidth/cdewEntries;
	int cellHeight = windowHeight / cdewJugglers;
	float delay;
	float letterOffset, sourceArrowOffset, destArrowOffset;
	int ePrev;
	_causalCell *pPrev;
	float up;
	bool noArrowMFmode = false,
	mfBowFlag = false;
	
	
	//allow negative offsets
	ei = e;
	while( ei < 0 )
		ei+=cdewEntries;
	
	
	// get causal values
	ePrev = e - 1;
	while( ePrev < 0 )
		ePrev += cdewEntries;
	
	p = getCausalCell(causalCells, j,hnd,ei, cdewJugglers, cdewHands, cdewEntries);
	pSync = getCausalCell(causalCells, j,switchHand(hnd),ei, cdewJugglers, cdewHands, cdewEntries);
	pPrev = getCausalCell(causalCells, j,hnd,ePrev, cdewJugglers, cdewHands, cdewEntries);
	
	// get center of cell (this hand)
	_calcCenterOfCell( j, hnd, e, &originalX, &originalY);
	
	eDest = p->length + e;
	hDest = p->destHand;
	jDest = p->destJuggler;
	
	// get center of dest cell
	_calcCenterOfCell( jDest, hDest, eDest, &destOriginalX, &destOriginalY);
	
	
	// check if source or dest is a syncThrow
	pSync = getCausalCell(causalCells, j,switchHand(hnd),e, cdewJugglers, cdewHands, cdewEntries);
	sourceIsSync = isSyncThrow(p);
	
	pDest = getCausalCell(causalCells, jDest,hDest,eDest, cdewJugglers, cdewHands, cdewEntries);
	pDestSync = getCausalCell(causalCells, jDest,switchHand(hDest),eDest, cdewJugglers, cdewHands, cdewEntries);
	destIsSync = isSyncThrow( pDest );//(pDest->length != absorb && pDestSync->length != absorb);
	
	
	// print unused hands only in sync line mode
	if( displayMode != syncLineMode && p->length <= absorb && p->marked == false)
		return;
	
	
	// different height     in color line mode
	/*
	 if a hand is used in sync, it depends on the destination of the throw
	 a)      its to the same height:
	 height is moved up / down for both arrows and letters
	 b) its to a different height:
	 no change of height
	 */
	letterOffset = sourceArrowOffset = destArrowOffset = 0;
	
	if(displayMode == colorLineMode)
	{       
		int offset = cellHeight / 6;
		
		
		if( p->length > absorb && pSync->length > absorb )
		{
			letterOffset = (hnd==rightHand? offset:-offset);
			if( p->invertHandHeightOffset || pSync->invertHandHeightOffset )
				letterOffset *= -1;
			
			if(1){//colorLineMode ){
				
				if( sourceIsSync && destIsSync && p->destJuggler == pSync->destJuggler){//&&  && hnd == hDest  && originalY == destOriginalY )
					sourceArrowOffset =  offset;
					sourceArrowOffset *= (hnd==rightHand? 1:-1);
					
					destArrowOffset =  offset;
					destArrowOffset *= (hDest==rightHand? 1:-1);
					
				}
				
				if(pDest->invertHandHeightOffset || pDestSync->invertHandHeightOffset )
					destArrowOffset *= -1;
				if( p->invertHandHeightOffset || pSync->invertHandHeightOffset )
					sourceArrowOffset *= -1;
			}				
		}
		if(diffHeightFlag && sourceIsSync && destIsSync)//j == jDest && hnd == hDest && originalY == destOriginalY  )
		{
			if( j == jDest)
			{
				sourceArrowOffset =  (hnd==rightHand? offset:-offset);
				if( p->invertHandHeightOffset || pSync->invertHandHeightOffset )
					sourceArrowOffset *= -1;
				
				destArrowOffset =  (hDest==rightHand? offset:-offset);
				if( pDest->invertHandHeightOffset || pDestSync->invertHandHeightOffset )
					destArrowOffset *= -1;
			}
		}
	}
	
	// the center of a cell may be delayed
	centerX = originalX + cellWidth * p->delay ;
	centerY = originalY;
	
	destCenterX = destOriginalX + cellWidth * pDest->delay;
	destCenterY = destOriginalY;
	
	noArrowMFmode = false;
	if(  displayMode == colorLineMode ){ // MF mode
		mfBowFlag = false;
		if( sourceIsSync ){
			if(pSync->length != absorb && p->length == 0 && p->destJuggler == j && p->destHand != hnd){  // ich bin der zip
				letterOffset = sourceArrowOffset = destArrowOffset = 0;
				if( noArrowMFFlag )
					noArrowMFmode = true;
			}
			if( p->length != absorb && pSync->length == 0 && pSync->destJuggler == j && pSync->destHand == hnd){ // mein sync ist der zip
				letterOffset = sourceArrowOffset = destArrowOffset = 0;
				if( p->length >= 1 && p->destJuggler == j && p->destHand == hnd)
					mfBowFlag = true;
			}
		}
	} 
	
	
	// start of output
	// save openGL matrix
	pushMatrix();
	
	glGetFloatv( GL_LINE_WIDTH, &width);
	
	// repeating signs
	if( p->repeatIn )
		drawRepeatIn( centerX);
	if( p->repeatOut )
		drawRepeatOut( centerX);
	
	
	// draw extra flags
	if(1){	//showModifiers)
		
		float scale = min( cellWidth, cellHeight) / 100.f;
		setColor( 1,.0,.0);
		setLineWidth( 1. );
		
		if( p->bounce){
			sprintf(s, "%c", 'b');
			setPosition( centerX + cellWidth * .3f, centerY - cellHeight * .1f );
			openGLString( s, scale );
		}
		
		if( p->beep ){
			sprintf(s, "%c", 'm');
			setPosition( centerX + cellWidth * .2f, centerY - cellHeight * .1f );
			openGLString( s, scale );
		}

		if( p->halfspin ){
			sprintf(s, "%c", '@');
			setPosition( centerX + cellWidth * .1f, centerY - cellHeight * .1f );
			openGLString( s, scale );
		}
	}
	
	
	// debugging:
	// draw internal numbers
	if( 0 ){
		setColor( 0,0,0);
		setLineWidth( 1. );
		sprintf(s, "%d", p->nr);
		setPosition(  (originalX + centerX)/2., centerY - 10);
		drawString( s );
	}
	// draw delay
	if((centerX != originalX) && (showDelays || userDoesThrowDelayDrag)){
		float delayWidth = 3;
		
		setColor( .7,.7,.7);
		setLineWidth( delayWidth );
		drawOneLine( originalX, centerY + sourceArrowOffset, centerX, centerY + sourceArrowOffset);
		
		setColor( 0,0,0);
		setLineWidth( 1. );
		sprintf(s, "%.2f", p->delay);
		setPosition(  (originalX + centerX)/2., centerY + sourceArrowOffset - 10);
		drawString( s );
	}
	
	// letters, marks etc are drawn on modified height
	setGLPosition( centerX, centerY + letterOffset);
	
	// print marks
	// hide marks opDestSyncion made for certain patterns look nice in the kaskade
	// due to a bug sometimes marks are always hidden
	// function disabled
	if( p->marked ){//&& showMarkFlag ){
		setColor( 0.5,0.5,0.5 );
		drawDisk( 11,12);
	}
	
	
	
	setLineWidth(p->width );
	setColor( 0.,0.,0. );
	
	// set color of arrow
	colIndex = (p->length+2)%MAX_COLOR;
	delay = *getDelayedJuggler( jDest, p->destHand) - *getDelayedJuggler(j, hnd) + pDest->delay - p->delay;
	colIndex = (int)(colIndex + delay);
	
	
	col = colors[colIndex];
	
	if( cdewColorMode == JugglerColorMode )
		col =  getListColor( j );
	else if (cdewColorMode == EmphaseCrossColorMode )
		col = (((j != jDest) && (hnd == hDest))? green: red);
	
	// if a cell has a color specified, use it regardless of the display mode
	if( p->autoColor != NO_COLOR ){
		colIndex = p->autoColor;
		col = colors[colIndex];
	}
	
	
	// if a cell has a color specified, use it regardless of the display mode
	if(p->userDefArrowColor != NO_COLOR ){	
		colIndex = p->userDefArrowColor;
		col = colors[colIndex];
	}
	setColor(col.x,col.y,col.z);
	
	
	// print hand
	// in sync line mode and if both hands are used or marked
	printHand = (p->length > absorb || p->marked );
	if( displayMode == syncLineMode )
		if( showUnusedHandsInSyncMode )
			printHand = true;
	
	printHand = true;
	if( printHand){		
		
		GLfloat temp;
		float d, l;
		
		if( cdewColorMode != LineColorMode )
			setColor( .0,.0,.0);
		glGetFloatv( GL_LINE_WIDTH, &temp);
		
		if( displayMode != colorLineMode )
			setColor(0,0,0);
		
		if( p->startConflict )
			setColor(1.f,0,0);
		

		switch(   printSiteswap ){
			case LetterDisplayMode:
				sprintf(s, "%c", (hnd==rightHand) ?'R':'L');
				break;
				
			case MultiPersonNumberDisplayMode:
				d = *getDelayedJuggler(jDest, hDest) - *getDelayedJuggler(j, hnd);
				l = p->length + d + 2.;
				
				l *= cdewJugglers;
				
#define round( x ) (((x)/10)*10)
				l = round( l );
				
				if( l > 9 )
					sprintf(s, "%c", (int)l-9+'A');
				else
					sprintf(s, "%.0f", l);
				
				
				break;        
				
				case OnePersonNumberDisplayMode:  		
				d = *getDelayedJuggler(jDest, hDest) - *getDelayedJuggler(j, hnd);
				l = p->length + d + 2.;
				
				
				
				if(  l != (int )l )
					sprintf(s, "%.2f", l);
				else
					sprintf(s, "%d", (int)l);
				
				
				break;
				
            	case noHandDisplayMode:
				sprintf(s, "");
				break;
				
            	default:
				_myPrintf("wrong display mode \n");
				assert( 1 );
		}
		
		if( p->length != absorb){
			// print crosses
			if( showCrosses){
				wwwBoolean shouldCross, isCross;
				
				// odd number == throw should cross
				shouldCross = (int)(p->length+2)%2;
				
				// isross = should cross XOR sourceHand == destHand
				isCross = shouldCross ^ (hnd != p->destHand);
				
				
				if( isCross )
					strcat(s, "x");
			}
			
			// print hurries
			if( showHurries ){
				if( p->length > absorb && pPrev->length > absorb )
					strcat( s, "*");
			}
			// print passes
			if( 0 && p->length > absorb  &&  j != p->destJuggler )
			strcat( s, "p");
		}			 
		
		drawString( s );
		
		
	}
	
	setColor(col.x,col.y,col.z);
	
	
	// an arrow:
	if( p->length != absorb && noArrowMFmode == false){   
		bool drawBowMode;
		float delay;
		
		Vector3D v1, v2,v3,v4,v, vz, vd;
		float f;
		
		v1.x = centerX;
		v1.y = centerY + sourceArrowOffset;
		v2.x = destCenterX;
		v2.y = destCenterY + destArrowOffset;
		v1.z = v2.z = 0;
		
		v = v2 - v1;				// direction
		f = v.betrag();
		v = v.normalised();			// direction normalized
		v = v *  radius ;			// times circle size (marks etc)
		
		v3 = v1 + v;				// new start and end 
		v4 = v2 - v;
		
		// if resulting vector is too short (neg direction in relation to original vector)
		// => draw only the arrow in the center of the two points
		if( (v4 - v3) * (v2 - v1) < 0)
			v3 = v4 = v1 + (v2 - v1)*.5;
		
		
		
		colIndex = (p->length+2)%MAX_COLOR;
		delay = *getDelayedJuggler( jDest, p->destHand) - *getDelayedJuggler(j,hnd) + pDest->delay - p->delay;
		colIndex = (int)(colIndex + delay);
		
		
		
		drawBowMode = drawAsBow( p ) || drawAsBowFlag || mfBowFlag;
		
		// straight arrow
		if( !drawBowMode ){
			// make closed loopSync of 1 and -1 look nice
			if( p->length == -1){
				int o = 6;
				
				if( v3.y < v4.y ){
					v3.x += o;
					v3.y += o;
					v4.x += o;
					v4.y += o;
				}else if( v3.y > v4.y ){
					v3.x -= o;
					v3.y += o;
					v4.x -= o;
					v4.y += o;
				}
			}
			// make close loopSync of a pair of 0 look nice
			if( p->length == 0 && pDest->length == 0 ){
				int o = 6;
				if( v3.y < v4.y )
				{
					v3.x += o;
					v4.x += o;
				}else if( v3.y > v4.y )
				{
					v3.x -= o;
					v4.x -= o;
				}
			}
			// one of the many excepDestSyncions that are neccessary for Christophes Notation
			if( christophesNotation && p->c1x == true){
				setPosition( centerX, centerY );
				drawDisk( 0,5);
				
			}
			else{
				// draw a simple line
				drawOneLine(v3.x, v3.y, v4.x, v4.y);
			}
			
		}else{ 	// round arrow
			// draw bezier curves
			int l;
			Vector3D vUp1, vUpDest;
			
			// draw a curve
			l = p->length;
			vUp1 = v3;
			vUpDest = v4;
			
			up = ((float)lineHeight * (UpFactor + p->factor))* bowFactor ;
			
			
			// zero length (== hold)
			if(l == 0){
				vUp1.y += radius * up * .8;
				vUpDest.y += radius * up * .8;
				vUp1.x += radius * 1.6;
				vUpDest.x -= radius * 1.6;
			}else{ 
				// Throw
				v3 = v1;
				v4 = v2;
				
				if( l == -1 )
					l = 1;
				if( l == -2 )
					l = 1;
				vUp1.y += radius * (log(l+1.f)*up);
				vUpDest.y += radius * (log(l+1.f)*up);
			}
			if( p->length > -1 ){
				v3.x += radius;
				v4.x -= radius;
			}else{ 
				v3.x -= radius;
				v4.x += radius;
			}
			
			// bows are bit higher than the letters
			
			{
#define HightMax 1.3
				float hightOffset = (up*4);
				if( hightOffset > radius * HightMax )
					hightOffset = radius * HightMax ;
				if( hightOffset < -radius * HightMax  )
					hightOffset = -radius * HightMax ;
				
				
				v3.y += hightOffset;
				v4.y += hightOffset;
				vUp1.y += hightOffset;
				vUpDest.y += hightOffset;
			}
			
			drawCurve( v3, vUp1, vUpDest, v4);
		}
		
		// arrowhead
		{
			Vector3D dir;
			float angle;
			Vector3D z(0,0,1);
			Vector3D a1,a2,a3,a4;
			Vector3D b1,b2,b3,b4;
			dir = v4 - v3;
			dir = dir.normalised();
			
			if( drawBowMode ){
				if( p->length >= 0)
					angle = radToGrad( atan( up) );
				else
					angle = radToGrad( atan( -up) ) + 180;
			}
			else{
				float df;
				float dx,dy;
				
				dx = centerX - destCenterX;
				dy = centerY - destCenterY - destArrowOffset;
				
				angle = 180;
				
				if( dx != 0){       
					df = dy / dx;
					angle = 180 - radToGrad( atan ( df ) );
					
					if( dx < 0 )
						angle += 180;
					
				}else{
					angle = 90;
					if( dy < 0 )
						angle *= -1.;
				}
			}
			
			a1 = arrow1 * arrowheadFactor;
			a2 = arrow2 * arrowheadFactor;
			a3 = arrow3 * arrowheadFactor;
			a4 = arrow4 * arrowheadFactor;
			a1 = a1.rotateAroundVektor(  angle, z );
			a2 = a2.rotateAroundVektor(  angle, z );
			a3 = a3.rotateAroundVektor(  angle, z );
			a4 = a4.rotateAroundVektor(  angle, z );
			b1 = v4+a1;
			b2 = v4+a2;
			b3 = v4+a3;
			b4 = v4+a4;
			
			
			
			// one of the many excepDestSyncions that are neccessary for Christophes Notation
			if(!( christophesNotation && p->c1x == true) )
				drawQuad( b1.x,b1.y,b2.x,b2.y,b3.x,b3.y,b4.x,b4.y);
			
		}
	}		
	drawTimeCursor();
	
	
	// end of output
	// restore openGL matrix
	popMatrix();
	
}// end of output


void oglwin::drawCellBackground( int j, int hnd, int e)
{       float centerX, centerY;
	float h,w;
	_causalCell *p, *p2;
	float vx, vy, vz;
	float shadeFactor = .7f;
	wwwBoolean showFaults = false;
	bool faultDetected = false;
	
	// get cell positions
	w = windowWidth / cdewEntries /2.*.9;
	h = lineHeight2 / 2  *.9 ;      // leftHand and rightHand are drawn
	/*      centerX = windowWidth / cdewEntries * (e+.5);
	 centerY =  lineHeight2 * (j*2  + (hnd == leftHand? 1:0)  +.5) ;
	 */
	_calcCenterOfCell( j, hnd, e, &centerX, &centerY);
	p = getCausalCell( causalCells, j, hnd, e, cdewJugglers, cdewHands, cdewEntries );
	p2 = getCausalCell( causalCells, j, switchHand(hnd), e, cdewJugglers, cdewHands, cdewEntries );
	
	// handle hands with inverted height
	if( displayMode == colorLineMode){
		h = h/2.;
		if( p->invertHandHeightOffset || p2->invertHandHeightOffset)
			h *= -1.;
		centerY -= (hnd == leftHand? h:-h);
	}
	
	faultDetected = false;
	if(showErrors && (p->notOutgoing || p->notReached || p->reached > 1 ||
					  p2->notOutgoing || p2->notReached || p2->reached > 1))
		faultDetected = true;
	
	
	// set color for inactive windows with / without field background
	if( inactiveBackgroundMode == cdewBackgroundGrey || faultDetected)
		vx = vy = vz = getCdewBackgroundColor().x * 0.8;
	else
		vx = vy = vz = getCellBackgroundColor( cellColor );
	
	{		
		float f = 3.f;
		// if a cell has a color specified, use it regardless of the display mode
		if(p->userDefBackgroundColor != NO_COLOR ){	
			int colIndex = p->userDefBackgroundColor;
			sysColor col = colors[colIndex];
			vx *= col.x * f;
			vy *= col.y * f;
			vz *= col.z * f;
		}
		if(p2->userDefBackgroundColor != NO_COLOR ){	
			int colIndex = p2->userDefBackgroundColor;
			sysColor col = colors[colIndex];
			vx *= col.x * f;
			vy *= col.y * f;
			vz *= col.z * f;
		}
	}
#if 0
	// intimeCursorMode make faults blink
	if( showTimeCursor )
	{
#define TC_FREQ 500
		static int startTime = -1;
		int now;
		int d;
		
		if( startTime == -1 )
			startTime = glutGet( GLUT_ELAPSED_TIME);
		
		now = glutGet( GLUT_ELAPSED_TIME);
		d = (now - startTime) % TC_FREQ;
		if( d == 0)
			glutPostRedisplay();
		
		if(d > TC_FREQ / 2 )
			showFaults = true;
		
	}
	
#endif
	
	// display faults
	if(showErrors && patternHasFaults)
		showFaults = true;
	
	// Faults
	if( showFaults){
		switch(displayMode){
			case syncLineMode:
				if(p->notOutgoing){
					vx *= shadeFactor;
					faultDetected = true;
				}
				if( p->notReached){
					vz *= shadeFactor;
					faultDetected = true;
				}
				if( p->reached > 1 ){
					vy *= shadeFactor;
					faultDetected = true;
				}
				break;
				case colorLineMode:
				if( p->length == absorb ){
					if( p2->notOutgoing){
						vx *= shadeFactor;
						faultDetected = true;
					}
					if( p2->notReached){
						vz *= shadeFactor;
						faultDetected = true;
					}
					if( p2->reached > 1 ){
						vy *= shadeFactor;
						faultDetected = true;
					}
				}else{
					if( p->notOutgoing){
						vx *= shadeFactor;
						faultDetected = true;
					}
					if( p->notReached){
						vz *= shadeFactor;
						faultDetected = true;
					}
					if( p->reached > 1 ){
						vy *= shadeFactor;
						faultDetected = true;
					}
				}
				break;
				
				case singleLineMode:
				if( p->notOutgoing || p2->notOutgoing){
					vx *= shadeFactor;
					faultDetected = true;
				}
				if( p->notReached || p2->notReached){
					vz *= shadeFactor;
					faultDetected = true;
				}
				if( p->reached > 1 || p2->reached>1){
					vy *= shadeFactor;
					faultDetected = true;
				}
				break;
		}
	}
	
	
	// don´t draw cell background
	if( cellBackgroundFlag || faultDetected){
		
		// draw to screen and postscript file
		setColor(vx, vy, vz);
		_postscriptColor(vx, vy, vz);
		drawQuad(centerX-w, centerY-h,
				 centerX+w, centerY-h,
				 centerX+w, centerY+h,
				 centerX-w, centerY+h);
	}
	// draw rect in siteswap input mode (input cursor)
	if(siteswapInputMode && j == siteswapInputJuggler && e == siteswapInputEntry && hnd == siteswapInputHand)
	{
		setColor (0,0,0);
		drawOneLine(centerX-w-1, centerY-h-1, centerX-w-1, centerY+h+1);
		drawOneLine(centerX-w-1, centerY+h+1, centerX+w+1, centerY+h+1);
		drawOneLine(centerX+w+1, centerY+h+1, centerX+w+1, centerY-h-1);
		drawOneLine(centerX+w+1, centerY-h-1, centerX-w-1, centerY-h-1);
	}
}


// used when a cell is drawn
// check for marks, too
void oglwin::_calcCenterOfCell( int j, int hnd, int e, float *centerX, float *centerY)
{
	wwwBoolean sync;
	_causalCell *p1, *p2;
	int     cellWidth = windowWidth/cdewEntries;
	float d;
	int h = hnd;
	
	
	p1 = getCausalCell(causalCells, j,leftHand,e, cdewJugglers, cdewHands, cdewEntries);
	p2 = getCausalCell(causalCells, j,rightHand,e, cdewJugglers, cdewHands, cdewEntries);
#ifdef MultiLineMode
	if( e > cdewEntries / 2 )
		j *= 2;
#endif
	// heres the difference
	//((p1->length != absorb || p1->marked || p1->notOutgoing) && ( p2->length != absorb || p2->marked || p2->notOutgoing)  );
	if( diffHeightFlag)
		sync = isSyncThrow( p1 );
	else
		sync = false;
	
	d = cellWidth * *getDelayedJuggler(j,hnd);
	
	*centerX = windowWidth / cdewEntries * (e+.5) + d;
	
	
	//displayMode != syncLineMode )//d
	if(displayMode == colorLineMode || (displayMode == singleLineMode && !sync ) )
		*centerY =  windowHeight - lineHeight2 * (j*2  + 1);
	else
	{
		if( p1->invertHandHeightOffset )
			h = switchHand(h);
		
		*centerY =  windowHeight - lineHeight2 * (j*2  +  (h == leftHand? 1:0)  +.5) ;
	}
	if(userDoesJugglerDelayDrag && j == startJ && dragJugglerFlag == jugglerSwapDrag) 
		*centerY += startMouseY - mouseY;
	
	
	if( displayMode == colorLineMode ){ // MFmode
		float cxOffset = radius*2.f + 4.f;
		if(sync ){
			if( p1->length == 0 && p1->destJuggler == j && p1->destHand != hnd){  // ich bin der zip
				*centerX += cxOffset;
			}
			if( p2->length == 0 && p2->destJuggler == j && p2->destHand != hnd){ // mein sync ist der zip
				*centerX += cxOffset;
			}			
		}
	} 
}



/*****
 #pragma mark ---------------------drawing routines
 *****/



void oglwin::drawNumberOfBeats( void )
{       float centerX, centerY = -4;
	int e;
	char s[10];
	
	for( e= 0; e < cdewEntries; e++ )
	{
		
		centerX = windowWidth / cdewEntries * (e+.5)-7;
		// draw the number
		sprintf(s, "%d", e+1);
		
		setColor(0,0,0);
		
		pushMatrix();
		setPosition(centerX, centerY+10);
		
		drawString( s );
		
		popMatrix();
		
		
	}
}
void oglwin::drawNumberOfObjects( void )
{       float centerX;
	float sc = min(lineHeight / 800., .07);
	char s[20];
	
	size_t i;
	centerX = windowWidth / 2;
	// draw the number
	sprintf(s, "%f", calcNumberOfObjects());
	glColor3i(0,0,0);
	pushMatrix();
	glTranslatef(centerX-30, windowHeight-10,0);
	glScalef(sc,sc,sc);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	popMatrix();
}

void oglwin::drawDisplayMode( void )
{
	float centerX, centerY = windowHeight/2;
	float sc = min(lineHeight / 800., .07);
	char s[100];
	size_t i;
	
	if(!showDisplayMode )
		return;
	
	centerX = 10;
	
	// draw the number
	switch(displayMode )
	{       case syncLineMode:
			sprintf(s, "syncLineMode");
			break;
		case singleLineMode:
			sprintf(s, "singleLineMode");
			break;
		case colorLineMode:
			sprintf(s, "colorLineMode");
			break;
	}
	glColor3i(0,0,0);
	pushMatrix();
	glTranslatef(centerX, centerY,0);
	glScalef(sc,sc,sc);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	popMatrix();
	
}

void oglwin::drawColorMode( void )
{  
    float centerX = windowWidth/2, centerY = windowHeight/2;
	float sc = min(lineHeight / 800., .07);
	char s[100];
	size_t i;
	
	if(!showColorMode )
		return;
	// draw the number
	switch(cdewColorMode )
	{    
		case syncLineMode:
			sprintf(s, "LineColorMode");
			break;
		case LengthColorMode:
			sprintf(s, "LengthColorMode");
			break;
		case JugglerColorMode:
			sprintf(s, "JugglerColorMode");
			break;
		case EmphaseCrossColorMode:	
			sprintf(s, "EmphaseCrossColorMode");
			break;
			
	}
	glColor3i(0,0,0);
	pushMatrix();
	glTranslatef(centerX, centerY,0);
	glScalef(sc,sc,sc);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	popMatrix();
}

void oglwin::drawMimMode( void )
{       float centerX, centerY = windowHeight/2;
	
	float sc = min(lineHeight / 800., .07);
	char s[100];	
	size_t i;
	centerX = 10;
	
	switch(theMouseInputMode )
	{       case mimDrag:
			sprintf(s, "drag");
			break;
		case mimMark:
			sprintf(s, "mark");
			break;
		case mimDragMark:
			sprintf(s, "drag marked");
			break;
		case mimDelay:
			sprintf(s, "delay");
			break;
		case mimHandDelay:
			sprintf(s, "hand delay");
			break;
		case mimDelayMark:
			sprintf(s, "delay marked");
			break;
		case mimFactor:
			sprintf(s, "bow factor");
			break;
		default:
			sprintf(s, "oups, undefined input mode string");
			break;
	}
	glColor3i(0,0,0);
	pushMatrix();
	glTranslatef(centerX, centerY,0);
	glScalef(sc,sc,sc);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	popMatrix();
}


// draw the cursor (a circle)
// the arrow length
void oglwin::cdewDrawCursor(void )
{
	_causalCell *p, *p2, *p3;
	char s[10];
	float x,y;
	int d;
	float length;
	
	// get arrow length.
	// It will start at source cell and points to dest cell
	// (dest cell position calculated below)
	p = getCausalCell( causalCells, startJ, startH, startE, cdewJugglers, cdewHands, cdewEntries);
	p2 = getCausalCell( causalCells, p->destJuggler, p->destHand, p->length + startE, cdewJugglers, cdewHands, cdewEntries);
	p3 = getCausalCell( causalCells, p->destJuggler, switchHand(p->destHand), p->length + startE, cdewJugglers, cdewHands, cdewEntries);
	if( p2->length == absorb && p3->length != absorb )
		p2 = p3;
	
	// no throw
	if( p->length == absorb )
	{       return;
	}
	
	// fit to screen
	d = (startE+p->length);
	if( p->length < 0 )
		while( d < 0 )
			d+=cdewEntries;
	else
		while( d >=cdewEntries )
			d-=cdewEntries;
	
	// get screen position
	_calcCenterOfCell( p->destJuggler, p->destHand, d, &x, &y);
	// draw the number
	length = *getDelayedJuggler( p->destJuggler, p->destHand) - *getDelayedJuggler( startJ, startH) + p2->delay - p->delay;
	length = p->length + length ;
	
	sprintf(s, "%2.2f", length);
	setColor(1,0,0);
	pushMatrix();
	glTranslatef(x,y,0);
	glTranslatef(0,10,0);
	glScalef(.1f,.1f,.1f);
	{       size_t i;
		for( i = 0; i <strlen(s);i++)
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	}
	popMatrix();
	
}
// print delay value
void oglwin::drawDelayValue(void )
{
	int j,h;
	char s[20];
	float v;
	for( j = 0; j < cdewJugglers; j++ )
	{       size_t i;
		
		for( h = 0; h < 2; h++)
		{
			// draw the number
			sprintf(s, "%.2f", v = *getDelayedJuggler(j,h));
			if( v >= 0 && v < 1 )
				setColor(.0f,.5f,.0f);
			else
				setColor(.7f,.5f,.5f);
			
			pushMatrix();
			glTranslatef(windowWidth/2 -100 + 200 * h, lineHeight * (cdewJugglers-j - 1),0);
			glTranslatef(0,10,0);
			glScalef(.1f,.1f,.1f);
			for( i = 0; i <strlen(s);i++)
				glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
			popMatrix();
		}
	}
}


// print info for cmhn
void oglwin::drawChristophesNotationInfo( void )
{       unsigned int i;
	char s[10];
	float sc = min(lineHeight / 800., .07);
	
	if( !christophesNotation )
		return;
	
	sprintf(s, "%s", "cmhn");
	glColor3i(0,0,0);
	pushMatrix();
	glTranslatef(10, windowHeight - 10,0);
	glScalef(sc,sc,sc);
	for( i = 0; i <strlen(s);i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
	popMatrix();
}


float oglwin::getCellBackgroundColor( int i )
{
	int mcc = MaxCellColors;
	
	i = i % mcc;
	return (1. - 1./MaxCellColors * i)*1.8;
}



oglwin *cursorWin = NULL;

// called by animationIdle
// time cursor is updated in the same freqence and moment as animation
// forces the diagram to be redrawn, if time cursor is active
void _drawTimeCursor( int dummy ){
	if( cursorWin){
		glutSetWindow(cursorWin->id);
		glutPostRedisplay();
	}
}


extern int ash;

void oglwin::drawTimeCursor( void ){
#if !CDEW_STANDALONE
	GLfloat w;
	
	glutSetWindow( id );
	
	float x = (windowWidth) * (aTime+timeCursorOffset)/ rawPatternTime;
	if( x >= windowWidth )
		x -= windowWidth;
	
	if( !showTimeCursor || !imTheAnimatedWindow || patternHasOpenLoops )
		return;
	
	glGetFloatv( GL_LINE_WIDTH, &w);
	glLineWidth( timeCursorWidth );
	
	setColor( timeCursorColor.x, timeCursorColor.y, timeCursorColor.z);
	drawOneLine( x,0,x,windowHeight);
	
	glLineWidth( w );
	
#endif
}

void oglwin::setTimeCursorDiagram( bool mode )
{
	if( mode == true )
		cursorWin = this;
	else
		cursorWin = NULL;
}


void oglwin::invertHeightForChristophesVisualisationModeFunc ( void )
{
	_causalCell *p, *pSync;
	wwwBoolean m;
	
	int j,e;
	
	for( j = 0; j < cdewJugglers; j++ )
	{
		for(e = 0; e < cdewEntries; e++ )
		{
			
			p = getCausalCell( causalCells, j, leftHand, e, cdewJugglers, cdewHands, cdewEntries);
			pSync = getCausalCell( causalCells, j, rightHand, e, cdewJugglers, cdewHands, cdewEntries);
			if( p->marked || pSync->marked)
			{       m = (p->invertHandHeightOffset || pSync->invertHandHeightOffset);
				toggle( m );
				pSync->invertHandHeightOffset = p->invertHandHeightOffset = m;
			}
		}
	}
}

void oglwin::drawOnscreenHelp( void )
{
	float fontSize = min((float)lineHeight/800., .1f);
	char s[100];
	
#if 01
	return;
#endif
	
	sprintf(s, "%x", theModifiers);
	setPosition( 10,10);
	glScalef( fontSize, fontSize, fontSize);
	drawString( s );
	//      stringToScreen( 10,10, fontSize, s );
	
	glutPostRedisplay();
}


typedef struct {int i; 
	float dl, dr;
} _jlist;



#if 01
int cmpDelays(const void *l1,const void *l2);
#else
int cmpDelays(_jlist *l1,_jlist *l2);
#endif

void oglwin::drawStart( void )
{  
	
	
	int j, h, e;
	char s[20];
	int y;
	int lh = lineHeight / 2;
	int o =  calcNumberOfObjects();
	int t;
	int *hv = (int *)malloc(cdewJugglers * cdewHands * sizeof( int ));  // number of objects in hand
	int c;
	_causalCell *p,*pp;
	_jlist *jlist = (_jlist *)malloc(cdewJugglers * sizeof( _jlist ));  // number of objects in hand
	
	
	
    if( hv == 0 || jlist == 0)
        assert( 1 );
	
	
    t = 0;
	for( c = 0; c < cdewJugglers * cdewHands; c++ )
	{	hv[c] = 1;
	    t++;
	    jlist[c].i = c;
	    jlist[c].dl = *getDelayedJuggler(c, leftHand);
	    jlist[c].dr = *getDelayedJuggler(c, rightHand);
	    
	}	
	
	setColor(0,0,0);
	_myPrintf("check for objects on start \n");
	
	allCells( 
			 p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
			 p->switched = false;
			 );
	
	qsort( jlist, cdewJugglers, sizeof(_jlist), cmpDelays);         
	
	
	// don´t change order of loops!
    for( e = 0; e < cdewEntries; e++ )
    {
       	for( j = 0; j < cdewJugglers; j++ )
	    { 
            for( h = 0; h < cdewHands; h++)
            {
                
                // nothing more to do
				
                p = getCausalCell( causalCells, j,h,e, cdewJugglers, cdewHands, cdewEntries);
                p->test = false;
                
                if( t >= o )
                    continue;
				
                if( p->length == absorb )
                    continue;
				
                // nothing to do here    
                if(p->length == -2)
                    continue;
				
                if( p->switched == true )
                    continue;
				
				
				
                pp = getCellToCell( p);
                
				
				
#if 01
				{
                    int th = h;
                    
                    _myPrintf("checking %d %d %d ", j,h,e);
                    // we don´t like zipps in starting phase
                    // start with object in the other hand
                    // only in starting phase!
                    
                    // throw enters the pattern with positive value, i.e. from the left
                    if( pp->length > 0 )
                    {    
                        
                        hv[j*2+th] ++;
                        t ++;
                    }    
                    
					//    p->test = true;
                    
                    _myPrintf("\n");
                    
                }    
#else
				// throw enters the pattern with positive value, i.e. from the left
				hv[j*2+h] ++;
				t ++;
				if( p->length < 0 )
					p->test = true;
#endif                        
				
                
            }
        }
    }   
    
    
	for( j = 0; j < cdewJugglers; j++ )
	{ 
        size_t i;
        for( h = 0; h < cdewHands; h++)
        {
        	y = windowHeight - lh * (2*j + h -.5);
			
			
            // draw the number
            sprintf(s, "%c = %d", h==leftHand?'L':'R',hv[j*2+h]);
			
            pushMatrix();
            glTranslatef(20, y + 10,0);
			glScalef(.08f,.08f,.08f);
            for( i = 0; i <strlen(s);i++)
				glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, s[i]);
            popMatrix();
        }
	}
	
	free( hv );
}


int cmpDelays(const void *l1,const void *l2)
{
#if 0
    _jlist p1 = *(_jlist*)l1, p2 = *(_jlist*)l2;
    
    return p1.dl - p2.dl;
#else
    _myPrintf("comparing %lx %lx\n", l1, l2);
    
    return 0;
#endif 
}



#define cellWidth           (windowWidth/cdewEntries)

#define RepeatLineWidth     2
#define RepeatLineOffset   (cellWidth/15.)
#define RepeatDotDiameter   2
#define RepeatDotHeight     10
#define RepeatLineHeight    (lineHeight*1.5/4)

void oglwin::drawRepeatIn( int x )
{
    int h = windowHeight;
    int x2;
    int w;
	
    
    w = getLineWidth( );
    setLineWidth( RepeatLineWidth );
    setColor( 0,0,0);
    
    x2 = x - RepeatLineOffset*1.;
    setPosition( x2, windowHeight/2 - RepeatDotHeight);
    drawDisk(0, RepeatDotDiameter );
    setPosition( x2, windowHeight/2 + RepeatDotHeight);
    drawDisk(0, RepeatDotDiameter );
    
    x2 = x - RepeatLineOffset*2.;
    drawOneLine( x2, RepeatLineHeight,x2, h - RepeatLineHeight);
    x2 = x - RepeatLineOffset*3.;
    drawOneLine( x2, RepeatLineHeight,x2, h - RepeatLineHeight);
	
	
    setLineWidth( w );
}


void oglwin::drawRepeatOut( int x )
{
    int h = windowHeight;
    int x2;
    int w;
    
    
    w = getLineWidth( );
    setLineWidth( RepeatLineWidth );
    setColor( 0,0,0);
    
    x2 = x + RepeatLineOffset*1.;
    setPosition( x2, windowHeight/2 - RepeatDotHeight);
    drawDisk(0, RepeatDotDiameter );
    setPosition( x2, windowHeight/2 + RepeatDotHeight);
    drawDisk(0, RepeatDotDiameter );
    
    x2 = x + RepeatLineOffset*2.;
    drawOneLine( x2, RepeatLineHeight,x2, h - RepeatLineHeight);
    x2 = x + RepeatLineOffset*3.;
    drawOneLine( x2, RepeatLineHeight,x2, h - RepeatLineHeight);
	
	
	setLineWidth( w );
}


void oglwin::drawStartDistribution( void )
{
    char s[1000];
    int j;
	int lh = lineHeight / 2;
	int y;
	
	
	
    if( !allLoopsClosed() )
        return;
	
	
    
    if( startHandList )
        free( startHandList );
    startHandList = 0;
	
    if( (startHandList = allocStartHandList( cdewJugglers)) == NULL)
        return;
	
    calcStart();
    
    setColor( 0,0,0);
	
	for( j = 0; j < cdewJugglers; j++ ){ 
		
    	y = windowHeight - lh * (2*j + 2) ;
		
		
        // draw the number
		sprintf(s, "%dL %dR", *getStartHandFromList( j, leftHand, startHandList) , *getStartHandFromList( j, rightHand, startHandList) );
		setPosition( 20, y+10);
		drawString( s );
	}
	
}