/*
 *  editor.h
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

#ifndef __LINEEDIT__
#define __LINEEDIT__

#include "headers.h"
#include "vtypes.h"
#include "openglmenu.h"


#if defined(__cplusplus)
extern "C"
{
#endif
	
	
	
// key definitions
#define ESC						27
#define DEL						8

#define macnewlineCode			3

#define crCode					'\r'
#define newlineCode				'\n'
#define skipCode				'\0'
#define backspaceCode			8
#define delCode					127
	

extern wwwBoolean               textEditorStartedPattern;
extern wwwBoolean               generateCausalFromTextEditorNow;
extern bool						generateTextToCausalInSameWindow;
	
// I really should write documentation for what things are good for...
extern bool						dontSaveTextEditorContentThisTimeWithName;
extern char						modeSwitchString[];
extern int						editorDisplaySwitch;
extern char						textEditorFullPathAndName[];
extern char						theEditorTitle[];
extern char						textEditorPath[];
extern bool						editorPathValid;	
	
void                            initTextEditor( void );

/* input */
void                            resetTextEditor( void );
void                            _resetTextEditor( void );
void							openFileInTextEditor( char *n );
void							saveTextEditorContent(void);
char                            *_readTextEditorPattern( char *fileName );
void                            createTextEditor( void );
void                            closeTextEditor( void );
void                            _closeTextEditor( void );
void                            redrawTextEditor( void );
wwwBoolean                      writeToTextEditor( char *path );
void                            edit2Cdew( int dummy );
void                            edit2NewCdew( int dummy );
int                             _edit2Cdew( char *buf );
int                             _edit2NewCdew( char *buf );
int                             getErrPos( void );
void							startMusic( char *name);
wwwBoolean                      saveFile( char *buf, char *path );
void							setEditorWindowName( void );
void							setTextEditorNames(  char *p );
void							_editStartFunc( char *pattern );


extern sysColor                 editorCursorTextColor;
extern wwwBoolean               disableTransferToTextEditor;
extern wwwBoolean               skipAnimateCdewPattern;
extern wwwBoolean               generateCausalNow;

extern char                     *textEditorContent;
extern int                      inputPosition;
extern _menuList                *editMenuList;




#if defined(__cplusplus)
}
#endif
#endif
