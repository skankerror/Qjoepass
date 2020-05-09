/*
 *  mem.cpp
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
#include "globals.h"

#include "mem.h"
#include "parseSiteswap.h"
#include "cameraControl.h"
#include "myutil.h"

#ifndef MAC_VERSION
#include "systemswitch.h"
#endif

#if 1
#include <assert.h>
#else
#define assert  listErr
#endif

//------------- Meine Listen --------------------
char                                            pattern[PATTERN_SIZE];                          // Hier wird das Muster reinkopiert
Juggler											jugglerListe[JUGGLER_MAX];                      // Jongleure
_object                                           ballListe[BALL_MAX];                            // ObjectBeschreibung
punkt                                           ballPositionListe[BALL_MAX];
sse                                             eventListe[EVENT_MAX];                          // Die Liste der SS-Ereignisse
sseLoop                                         loopListe[LOOP_MAX];                            // Kontrollstrukturen
Juggler											startPositionListe[JUGGLER_MAX];				// Anfangsposition
lmp                                             lmpListe[LMP_MAX];
lsp                                             lspListe[LSP_MAX];
lp												lpListe[LP_MAX];
spinElement                                     spinListe[SPIN_MAX];
_cameraMovementPointer							cmpListe[CMP_MAX];

void listErr( wwwBoolean  );

_cameraMovementPointer *getCmp( int i )
{       assert( i >= 0 && i < CMP_MAX );
	return cmpListe + i;
}

Juggler *getJuggler( int i )
{       assert( i >= 0 && i <JUGGLER_MAX );
	return jugglerListe + i;
}

sse *getEvent( int i )
{       assert( i >= 0 && i < EVENT_MAX );
	return eventListe + i;
}

int getEventIndex( sse* e )
{
	int c;
	
	for( c = 0; c < EVENT_MAX; c++ )
		if( eventListe+c == e )
			return c;
	
	return -1;
}

int getObjectIndex( _object* o)
{
	int c;
	
	for( c = 0; c < BALL_MAX; c++ )
		if( ballListe + c == o )
			return c;
	return -1;
}


spinElement *getSpin( int i )
{       if( i < 0)
	i = 0;
	if( i >= SPIN_MAX)
		i = 0;
	assert( i >= 0 && i < SPIN_MAX );
	return spinListe + i;
}

sseLoop *getLoop( int i )
{       assert( i >= 0 && i < LOOP_MAX );
	return loopListe + i;
}

lsp *getLsp( int i )
{       assert( i >= 0 && i < LSP_MAX );
	return lspListe + i;
}

lmp *getLmp( int i )
{       assert( i >= 0 && i < LMP_MAX );
	return lmpListe + i;
}

lp *getLp( int i )
{       assert( i >= 0 && i < LP_MAX );
	return lpListe + i;
}


Juggler *getStartPosition( int i )
{       assert( i >= 0 && i < JUGGLER_MAX );
	return startPositionListe + i;
}

_object *getObject( int i )
{       assert( i >= 0 && i < BALL_MAX );
	return ballListe + i;
}

punkt *getObjectPosition( int i )
{       assert( i >= 0 && i < BALL_MAX );
	return  ballPositionListe + i;
}

void listErr( wwwBoolean b )
{
	if( !b )
		beep();
}
