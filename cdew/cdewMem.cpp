/*
 * cdewMem.cpp
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
#include <assert.h>
#include <stdlib.h>

#include "cdewClass.h"
#include "myPrintf.h"


void oglwin::freeCausalCells( _causalCell *p)
{
	if( p )
		free( p );
}
_causalCell *oglwin::allocCausalCells( int j, int h, int e)
{
	_causalCell *p;
	
	p = (_causalCell*)malloc( sizeof(_causalCell )*j*h*e);
	return p;
}

_causalCell *oglwin::getCausalCell( _causalCell *p, int j, int h, int e, int maxJ, int maxH, int maxE)
{
	int ei;
	_causalCell *o;

	while(j >= maxJ )
		j--;
	while(h >= maxH )
		h--;
#if 0	
	// allow negative offsets
	ei = e;
	while( ei < 0)
		ei+=cdewEntries;
	while( ei >= cdewEntries )
		ei -= cdewEntries;			
#else
	// allow negative offsets
	ei = e;
	while( ei < 0)
		ei+=maxE;
	while( ei >= maxE )
		ei -= maxE;			
#endif
		
	o = _getCausalCell( p, j, h, ei, maxJ, maxH, maxE);
	
	return o;
	
}
_causalCell *oglwin::_getCausalCell( _causalCell *p, int j, int h, int e, int maxJ, int maxH, int maxE)
{
	j = j%maxJ;
		return p + j * maxH*maxE  + h *maxE + e ;
}

wwwBoolean oglwin::getIndicesFromCellPointer( _causalCell *in, int *j, int *h, int *e )
{	_causalCell *p;
	wwwBoolean found = false;
	
	if( in == NULL )
		return false;
		
	allCells(
				p = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
				if( in == p )
				{	
					*j = jAll;
					*h = hAll;
					*e = eAll;
					found = true;
				}	
			)
	return found;
		
}



void oglwin::checkmem( void )
{

	_causalCell *r, *s, *d;
	static int c;

_myPrintf("checkmem %4d\n", c++);
if(causalCells ==NULL )
{_myPrintf("memory not defined\n");
return;
}
		  
	r =(_causalCell *)malloc(sizeof( _causalCell ) * cdewJugglers *cdewHands  *cdewEntries);
	allCells(
		s = getCausalCell( causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
		d = getCausalCell( r, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries);
		*d = *s;
			)
	free( causalCells );
_myPrintf("checkmem successful\n");
	causalCells = r;
}




float *oglwin::allocDelayList( int j )
{
	int i, h;
	float *p;
		
	if( (p = (float*)malloc(sizeof(float)*j* 2) ) != NULL )
	{	
		for( i = 0; i < j; i++ )
			for( h = 0; h < 2; h++ )
				*(p+2*i+h) = 0.;
			
	}	
	return p;
}

float *oglwin::getDelayedJuggler( int j, int h )
{

	while( j < 0 )
		j+= cdewJugglers;
	while( j >= cdewJugglers )
		j-=cdewJugglers;
	assert( j < delayableJugglers && j >= 0);
	return jugglerDelayList+2*j+h;
}


_causalCell *oglwin::getCellToCell( _causalCell *p )
{
	_causalCell *tOld, *tDestOld;

	if( p == NULL )
		return 0;
		
	allCells(	tOld = getCausalCell(causalCells, jAll, hAll, eAll, cdewJugglers, cdewHands, cdewEntries) ;
				if( tOld->length != absorb ){
					tDestOld =  getCausalCell(causalCells, tOld->destJuggler, tOld->destHand, eAll + tOld->length, cdewJugglers, cdewHands, cdewEntries) ;
			
					if( tDestOld == p )
						return 	tOld;
				}
			)
			
	return 0;
}					


_causalCell *oglwin::getDestinationCell( _causalCell *p )
{
	_causalCell *d;
	int j,h,e;

	if(p == 0 || p->length == absorb)
		return NULL;
		
	getIndicesFromCellPointer(p, &j, &h, &e);
	 	
	d = getCausalCell(causalCells, p->destJuggler, p->destHand, e + p->length, cdewJugglers, cdewHands, cdewEntries) ;


	return d;
}



_causalCell *oglwin::getCellWithNumber( _causalCell *list, int mj, int mh, int me, int i )
{
	int j,h,e;
	_causalCell *p;
	
	
	for( j = 0; j < mj; j++ )
	{
		for( h = 0; h < mh; h++ )
		{
			for( e = 0; e < me; e++ )
			{
				p = getCausalCell( list, j,h,e,mj,mh,me);
				if( p->nr == i )
					return p;
			}
		}		
	}

	return NULL;

}		   


#if 0
void setCellNumber( _causalCell *p, int i)
{

	p->nr = i;
}
#endif



int *oglwin::allocStartHandList( int j )
{
	int i, h;
	int *p;
		
	if( (p = (int *)malloc(sizeof(int)*j* 2 * 2) ) != NULL )
	{	
		for( i = 0; i < j; i++ )
			for( h = 0; h < 2; h++ )
                *(p+2*j+h) = 0;
   	}	
	return p;
}

int *oglwin::getStartHandFromList( int j, int h, int *p )
{
if( h < 0 || h > 1 || j < 0 || j > cdewJugglers)
    myPrintf("Son n mist!\n");
    



	return p+2*j+h;


}


