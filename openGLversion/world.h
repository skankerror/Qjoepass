/*
 *  world.h
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


#ifndef __WORLD__
#define __WORLD__


#if defined(__cplusplus)
extern "C++"
{
#endif

#include "parseSiteswap.h"

#define			DefMag		(1.f/100)

// std sizes
#define         ObjectSize                   10.0
#define         ClubSize                    52
#define         FishSize                    52
#define         RingSize                    40
#define         RingSize2       (RingSize/2)
#define         RingPoints                  7           // DONT change!
	


// Beschreibt das Weltmodell von Joe Pass
enum{			followOnce = 1, 
				followAlways, 
				followDone};        // Positionen verndern

enum{			random_object = 0, 
				clubs,	
				fishes, 
				balls, 
				rings, 
				pancakes, 
				nothing, 
				endOfObjects};

		  
enum{			greyObj, 
				redObj, 
				multiColorObj, 
				destObj,
				destJugglerObj,
				orbitColorObj, 
				ColorObjEnd};

enum{			greyJugglers, 
				multiColorJugglers, 
				noJugglers, 
				ColorJugEnd};

enum{			hasFloorTextureIndex = 1,
                hasSkyTextureIndex,
                hasObjectTextureIndex,
                hasRingTextureIndex,
                hasClubTextureIndex,
                hasHeadTextureIndex,
                hasBodyTextureIndex,
				hasStageTextureIndex,
                noTextureIndex};


//      world.c
void			initMyWorld( void );
void			disposeMyWorld( void );


void			generateObjects( void );
bool			projectWorld( void );

punkt			getPosOfJugglerAtOrAfter( int nr,  float t);
punkt			getPoiOfJugglerAtOrAfter( int nr,  float t);

//extern bool		showObjectNumbers ;

// shader
extern bool		shaderMode;

// fog
extern float	fogEnd;
extern punkt 	center;
extern float	minDistanceValue;
extern int		tableHeigt;
extern int		tableSize;
extern char		*objInfoStr[];
	
#if defined(__cplusplus)
}
#endif



#endif
