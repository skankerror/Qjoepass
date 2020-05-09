/*
 *  workspace.h
 *  joe2
 *
 *  created by Wolfgang Westerboer on 14.09.08.
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
#if defined(__cplusplus)
extern "C"
{
#endif
	
	
#define WORKSPACE_MAGIC_TEXT							"JoePass! Workspace File" 
#define ANIMATION_WORKSPACE_WINPOS_ENTRY				"animationWindowPosition:="
#define CDEW_WORKSPACE_FILENAME_ENTRY					"cdew_filename:="
#define CDEW_WORKSPACE_REL_FILENAME_ENTRY				"cdew_relative_filename:="
#define CDEW_WORKSPACE_WINPOS_ENTRY						"windowPosition:="
#define	TEXTEDITOR_WORKSPACE_FILENAME_ENTRY				"texteditor_filename:="
#define	TEXTEDITOR_WORKSPACE_REL_FILENAME_ENTRY			"texteditor_relative_filename:="
#define TEXTEDITOR_WORKSPACE_WINPOS_ENTRY				"texteditor_windowPosition:="
#define CDEW_WORKSPACE_END_ENTRY						";"
#define RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY	"runaround_relative_filename:="
#define RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY2	"runaround_relative_filename2:="
#define RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY_P	"runaround_relative_path:="
#define RUNAROUNDEDITOR_WORKSPACE_REL_FILENAME_ENTRY_N	"runaround_relative_file:="
#define RUNAROUNDEDITOR_WORKSPACE_FILENAME_ENTRY		"runaround_filename:="
	
extern bool useCdewWorkspace;
	
void saveCdewWorkspace( void );
void _saveCdewWorkspace( char *pathToWorkspaceFolder, char *worskpaceFilename);
void _saveWorkspaceAs( char *t );
void loadDefaultWorkspace( void );
void _loadCdewWorkspace( char *pathToWorkspaceFolder );
void openFileInInternalAndExternalEditor( char *pathname);
void openFileInExternalEditor( char *pathname );


#if defined(__cplusplus)
}
#endif