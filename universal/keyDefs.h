/*
 *  keyDefs.h
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

#ifndef __KEY_CODES__
#define __KEY_DEFS__

#include "systemswitch.h"
#ifdef MAC_VERSION

#define AXIS			'#'
#define GRID			'\''
#define NIVEAUS			'Ô' // alt + #

#define LESSfloorLines	','
#define MOREfloorLines	';'

#define NUMBERS			'n'
#define	JUGGLERCOLOR	'J'
#define POWINFO			'b'
#define ZOOMOUT         's'
#define ZOOMIN			'S'
#define BALLINFO		'i'
#define PATTERNINFO 	'p'

#define DREIDE			'¶'	// alt + d
	
#define EYEMINUS		'š'
#define EYEPLUS			'…'
#define ZEROMINUS		'Š'
#define ZEROPLUS		'€'

// alt + NUMBER				   toggle output of
#define FONE			'Á'		// jugglers 
#define FTWO			'Ò'		// balls
#define FTHREE			'¦'		
#define FFOUR			'¢'		
#define FFIVE			'['		
#define FSIX			']'
#define FSEVEN			'|'
#define FEIGHT			'{'
#define FNINE			'}'
#define FZERO			'­'		

// shift + alt + number 	mark ball 1 - 9
#define FSZERO			'='		
#define FSONE			'!'
#define FSTWO			'"'
#define FSTHREE			'¤'		
#define FSFOUR			'$'	
#define FSFIVE			'%'		
#define FSSIX			'&'
#define FSSEVEN			'/'
#define FSEIGHT			'('
#define FSNINE			')'

#define MOVEMENT		'l'
#endif


// read files
#define READSTYLEFILE  'x'
#define READPROFILE    'X'

// to be done
#define NUMBERS			'n'

// info control
#define	JUGGLERCOLOR	'J'
#define POWINFO			'b'
#define BALLINFO		'i'
#define PATTERNINFO 	'p'
  
// animation control
#define GRAVPLUS		'h'
#define GRAVMINUS		'H'
#define FASTER			'+'
#define SLOWER			'-'
#define DROP			'\r'
 
  
// view control
#define SMALLERMODELL	'e'
#define BIGGERMODELL	'E'

#define SMALLERGRID		'.'
#define BIGGERGRID		':'
#define BIGGER			'S'
#define SMALLER			's'
#define AXIS			'#'
#define NIVEAUS			'Ô' // alt + #
#define LESSNIVEAULINES	'°' // alt + ,
#define MORENIVEAULINES	'þ' // alt + ;
#define SMALLERNIVEAU	'É' // alt + .
#define BIGGERNIVEAU	'Ö' // alt + :

#define SMALLEROBJECTS	'<'
#define BIGGEROBJECTS	'>'
#define COLORFIGS		'c'
#define COLOROBJ		'C'
#define NUMBERS_KEY		'n'
#define FATMAN			'k'
#define THINMAN			'K'

// juggler control
#define MOREDWELL		'D'
#define LESSDWELL		'd'
#define MOREHMOVEMENT	'B'
#define LESSHMOVEMENT	'b'
#define MOREVMOVEMENT	'V'
#define LESSVMOVEMENT	'v'
#define HIDEJUGGLERS    'j'
#define NEXTJUGGLERVIEW	'3'
#define CAMERA_MOTION	'5'
#define TOPVIEW			'7'
#define BIRDVIEW		'9'
#define FRONTVIEW		'1'

// sound control
#define THROWSOUND		'T'
#define CATCHSOUND		't'

#define RESETVIEW		'\033'
#define RESETMAG		'\033'
#define OPENWORK		' '
#define SCREENSHOT		'0'
#define SELECTFILE		'o'	
#define SAVEFILE		's'
#define SELECTTTOO		'O'	
#define SHADER			'g'

// movie control
#define MAKEMOVIE		'm'
#define MAKEMOVIEDOUBLE	'M'

#define MOREFRAMES		'F'
#define LESSFRAMES		'f'

#define WORLD_MAX		(65535)
#define FLOOR_MAX		(65535)


#endif
