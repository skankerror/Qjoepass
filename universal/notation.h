/*
 *  notation.h
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
#ifndef __NOTATION__
#define __NOTATION__

#include "mydefs.h"


#if defined(__cplusplus)
extern "C"
{
#endif
	
#define NOTATION_FILE "notation.pass"
	
enum	{generateCausalMode, startAnimationMode};	
/*
Dies ist unsauber; enums sind eingabewerte für Funktion:
	generateCausalMode generiert nur ein Causal
	startAnimationMode startet nur die Animation
 besser:
	funtion1 generiert causal, wenn gewünscht
	funktion2 startet animation, wenn gewünscht

*/
	
enum	{DPlusMode = generateCausalMode, DMinusMode = startAnimationMode};

	
	
bool	startOnePersonPassNotation( char *p, wwwBoolean startOnePersonPassNotation );
bool	nHandedSiteswap( char *in, int j);
bool	nHandedToAnimation( char *in, int h);
void	nHandedToCausal( char *in, int h);
bool	startOnePersonPassNotationFlag( char *p, wwwBoolean checkForPassingPattern, int flg);
bool	generateCausalDiagramFromBuffer( char*p, int jugglers);	
#if defined(__cplusplus)
}
#endif

#endif
