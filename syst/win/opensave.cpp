/*
 *  opensave.h
 *  joe2
 *
 * created by wolfgang, 02.09.2009
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

#ifdef WINDOWS_VERSION

#include <windows.h>
#include <stdio.h>

#include "myDefs.h"
#include "opensave.h"
#include "systemswitch.h"


bool  callOpenDialog( char *infoString, char * fileTypes )
{
	OPENFILENAME myOfn;
	char fileName[1000];   
	bool ret;
	
	// init myOfn
	ZeroMemory(&myOfn, sizeof(myOfn) );
	myOfn.lStructSize		= sizeof( myOfn );
	myOfn.hwndOwner			= NULL;
	myOfn.lpstrFile			= fileName;
	myOfn.lpstrFile[0]		= '\0';
	myOfn.nMaxFile			= sizeof( fileName );
	myOfn.lpstrFilter		= fileTypes;
	myOfn.nFilterIndex		= 1;
	myOfn.lpstrFileTitle	= NULL;
	myOfn.nMaxFileTitle		= 0;
	myOfn.lpstrInitialDir	= NULL;
	myOfn.lpstrTitle		= openInfoString;
	myOfn.Flags				= OFN_PATHMUSTEXIST 
							| OFN_FILEMUSTEXIST;

	ret = GetOpenFileName( &myOfn );
	
	// print name to buffer 
	if( ret == true)
		sprintf(fileSelectorPathAndName, "%s", myOfn.lpstrFile);	

	return ret;
}

bool  callSaveDialog( char *infoString )
{
	OPENFILENAME myOfn;

	char fileName[1000];   

	bool ret;
	
	// init myOfn
	// init myOfn
	ZeroMemory(&myOfn, sizeof(myOfn) );
	myOfn.lStructSize		= sizeof( myOfn );
	myOfn.hwndOwner			= NULL;
	myOfn.lpstrFile			= fileName;
	myOfn.lpstrFile[0]		= '\0';
	myOfn.nMaxFile			= sizeof( fileName );
	myOfn.lpstrFilter		= "JoePass\0*.pass\0Text\0*.TXT\0All\0*.*\0";
	myOfn.nFilterIndex		= 1;
	myOfn.lpstrFileTitle	= NULL;
	myOfn.nMaxFileTitle		= 0;
	myOfn.lpstrInitialDir	= NULL;
	myOfn.Flags				= OFN_PATHMUSTEXIST;	
	ret = GetSaveFileName( &myOfn );
	
	// print name to buffer 
	if( ret == true)
		sprintf(fileSelectorPathAndName, "%s", myOfn.lpstrFile);	
	
	return ret;
}
#endif
