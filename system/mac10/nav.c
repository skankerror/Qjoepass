#if 01
/*
 * nav.c
 *  joe2
 *
 * creatend by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright ( C) <1997---2011>  Wolfgang Westerboer
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 ( at your option) any later version.
 
 This program is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------*/

#include "systemswitch.h"

#if MAC_OS_X_VERSION	

#include <string.h>
#include <stdio.h>


#include "macio.h"

/* 
 | call file selector to open a file
*/

bool navOpenFile( FSSpec *fspec, char *infoString)
{
	NavDialogOptions	navOptions;
	NavReplyRecord		navResponse;
	AEDesc 				desc;

	
	NavGetDefaultDialogOptions( &navOptions);
	BlockMoveData( infoString, navOptions.message, infoString[0]+1);
	BlockMoveData( LMGetCurApName( ), navOptions.clientName, LMGetCurApName( )[0]+1);

	NavChooseFile( NULL, &navResponse, &navOptions, NULL, NULL, NULL, NULL, NULL);

	if ( !navResponse.validRecord) {
		NavDisposeReply( &navResponse);		
		return( false);
	} 
	
	desc.dataHandle=0L;
	AEGetNthDesc( &navResponse.selection, 1, typeFSS, NULL, &desc);
	BlockMoveData( *desc.dataHandle, fspec, sizeof( FSSpec));
	AEDisposeDesc( &desc);
	
	NavDisposeReply( &navResponse);	

	return( true);
}


/* 
 | call file selector to save a file
 */

bool navSelectForSave( FSSpec *thisFsspec, char *filename, char *extension, char *infoString)
{
	NavDialogOptions	navOptions;
	NavReplyRecord		navResponse;
	AEDesc 				desc;
	FSSpec				fspec;
		
	NavGetDefaultDialogOptions( &navOptions);
	if( infoString )
		BlockMoveData( infoString, navOptions.message, infoString[0]+1);
	BlockMoveData( LMGetCurApName( ), navOptions.clientName, LMGetCurApName( )[0]+1);
	BlockMoveData( filename, navOptions.savedFileName, filename[0]+1);

	NavPutFile( NULL, &navResponse, &navOptions, NULL, myType, myCreator, NULL);

	if ( !navResponse.validRecord) {
		NavDisposeReply( &navResponse);
		thisFsspec = NULL;
		return( false);
	}

	desc.dataHandle=0L;
	AEGetNthDesc( &navResponse.selection, 1, typeFSS, NULL, &desc);
	BlockMoveData( *desc.dataHandle, &fspec, sizeof( FSSpec));
	AEDisposeDesc( &desc);

	if ( navResponse.replacing) {					// write out the file
		;
	}else
		FSpCreate( &fspec, myCreator, myType, smSystemScript);    
	
	
	*thisFsspec = fspec;
	return true;
}

#endif

#endif
