/*
 *  exitModules.cpp
 *  joe2
 *
 *  created by wolfgang on 28.07.05.
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
#include "exitModules.h"
#include "editor.h"
#include "causal_editor.h"
#include "runaround.h"
#include "workspace.h"

bool exitModulesFlag = false;

/*	mac os x seems to unload some modules early in casse of exit() calls, i.e. navigationServices. 
	This makes it impossible to leave the programm through the normal exit calls, 
	as save requests will cause the programm to crash.
   
	As a solution (a hack?) we close all editors before we call exit. 
	Normal atexit calls to this routines will cause no conflict as window content is marked as saved and
	windows are closed already.
*/   
void exitModules( void )
{
	extern bool editorContendChanged;
	// set flag to inform modules that files have to be saved 
	exitModulesFlag = true;
//	printf("editorContendChanged = %d\n", editorContendChanged);
// save workspace
	if( useCdewWorkspace )
		saveCdewWorkspace();

	// all editors
	exitCausalEditor();
	_closeTextEditor();
	_closeRunaround();
	
	exit( 0 );
}
	