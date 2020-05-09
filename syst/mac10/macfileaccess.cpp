/*
 *  macfileaccess.cpp
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---2011>  Wolfgang Westerboer
 
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

#if MAC_OS_X_VERSION	
#include "MoreFilesX.h"


#include "fileaccess.h"
#include "macio.h"


#ifndef UseNSOpenSavePanel

char 	*getFileNameFromFSSpec( FSSpec *sfFile);




int		navService;

/*****************************************************************************/
void _initIO( void )
{
	RegisterAppearanceClient();
	navService = NavServicesAvailable();
	
	NavLoad();
}



bool fio( char *infoString, char *filetypes )
{
#pragma unused (filetypes)
	
	FSSpec		fio_fspec;
	int 		ret;
	Str255		i2 = "\p";

	//i2 = CFStringGetPascalStringPtr( infoString);
	
	CopyCStringToPascal(infoString, i2);
	ret = navOpenFile( &fio_fspec, (char *)i2);
	//	ret = navOpenFile( &fio_fspec, infoString);
	if( ret )
		getFileNameFromFSSpec( &fio_fspec);
	
	return ret;
	
}
 
bool fis( char *infoString, char *name )
{
	FSSpec		fis_fspec;
	int 		ret;
	Str255		macFilename = "\pfilename";
	Str255		i2 = "\p"; 
	
	CopyCStringToPascal(name, macFilename);
	CopyCStringToPascal(infoString, i2);
	ret = navSelectForSave( &fis_fspec, (char *)macFilename, ".pass", (char *)i2 );
	//	ret = navSelectForSave( &fis_fspec, name, ".pass", infoString);
	/* create a dummy file */
	if( ret )
		getFileNameFromFSSpec( &fis_fspec);
	
	return ret;
}


 
bool fid( char *infoString)
{
	FSSpec		fis_fspec;
	int 		ret;
	Str255		macFilename = "\pfilename";
	Str255		i2 = "\p"; 
	char		*name = "";
	CopyCStringToPascal(name, macFilename);
	CopyCStringToPascal(infoString, i2);
	ret = navSelectForSave( &fis_fspec, (char *)macFilename, ".pass", (char *)i2 );
	//	ret = navSelectForSave( &fis_fspec, name, ".pass", infoString);
	/* create a dummy file */
	if( ret )
		getFileNameFromFSSpec( &fis_fspec);
	
	return ret;
}



char *getFileNameFromFSSpec( FSSpec *sfFile)
{
	char *pd;
	short ret;
	
	ret = FSMakePath(sfFile->vRefNum, 
					 sfFile->parID, 
					 sfFile->name , 
					 (unsigned char *)fileSelectorPathAndName, 
					 MAX_FULL_FILE_NAME);
	
	pd = strrchr( fileSelectorPathAndName, PATH_DIVIDER);
	if( pd == NULL )
		return NULL;
	
	*pd = '\0';
	sprintf( thePatternTitle, "%s", thePatternTitle); 
	sprintf( fullPatternPathName, "%s", fileSelectorPathAndName); 
	*pd = PATH_DIVIDER;
	
	return thePatternTitle;
}
#endif

#endif

