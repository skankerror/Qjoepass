// unix.cpp
// gebastelt von joe@informatik.uni-leipzig.de 
// Wed Aug  2 18:01:42 MET DST 2000
// geaedert von Wolfgang Westerboer, westwolf@gmx.de 18.12.2000
// geaendert von Tom Fieseler, tom@seffe.de, 02.02.2003 fuer Linux
// file io.c
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

#ifdef UNIX_VERSION

#include <forms.h>
#include "fileaccess.h"

const char *callFileSelector( char *info, char *name );



void exitIO( void )
{
	fl_finish();
}




/* initialize file selector */
void _initIO( void )
// init FileIO - Services
{
 	int i = 1;
 	char *p = ".";
	fl_initialize(&i,&p, "", 0, 0);
	atexit(exitIO);
  // brauchen wir nicht?
  return;
}



/* 
call file selector to open file
give back full path to file
*/

const char *fio( char *infoString )
{
	const char *fname = NULL;
	fname =  callFileSelector("load file", "");
   
   	if(	fname )
		sprintf(fileSelectorPathAndName, "%s", fname);

	return fname;

}


/* 
call file selector to save file
pass back full path to file
*/

const char *fis( char *infoString, char *name )	// call file selector to save file
{
	const char *fname = NULL;
	fname =  callFileSelector("load file", name);
   
   	if(	fname )
		sprintf(fileSelectorPathAndName, "%s", fname);

	return fname;

}



const char *callFileSelector( char *info, char *name )
{
   const char *fname;

   fname = fl_show_file_selector(info,"","*.*",name);

   return fname;
}


#endif
