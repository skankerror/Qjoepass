/*
 *  listWindow.h
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

#include "runaround.h"

#if defined(__cplusplus)
extern "C"
{
#endif
	
	extern  _fieldContent					*fieldList;
	extern  int                             totalFields;
	extern  GLint                   		listID;
	extern int								selectIndex;
	
	
	void									listIdle( void );
	
	int										getIndexOfSelectedField( void );
	_fieldContent                   		*getPointerToSelectedField( void );
	_fieldContent                   		*getField(_fieldContent *list, int i);
	int                                     getIndexFromFieldPointer( _fieldContent *l, _fieldContent *p);
	void                                    disposeFieldList( _fieldContent *l, int count );
	_fieldContent                   		*initFieldList( int count, int fieldsInLine, bool addPatternToFieldList );
	void                                    setSelectedFieldByPointer( _fieldContent *p);
	_runaroundJuggler              		 	*getRunaroundJuggler( _fieldContent *f, int j);
	
	void									copyFieldToField( _fieldContent *dest, _fieldContent *source);
	
	void                                    initListModul( void );
	void                                    listWindow( void );
	void                                    closeListWindow( void );
	
	void									runaroundListSpecialKey(int runaroundListKey, int x, int y);
	
	void                                    listKey(  unsigned char theKey, int x, int y );
	
	void									_runaroundRemoveJuggler( void );
	void									_runaroundAddJuggler( void );
	void									_runaroundRemovePositionAtEnd( void );
	void									_runaroundRemoveSelectedPosition( void );
	void									_runaroundAddPosition( void );
	void									_runaroundAddBeforeSelectedPosition( void );
	
	void									runaroundSelectSceneA( int d);
	void									runaroundSelectSceneB( int d);
	
#define	RunaroundFilesCreated			"runaroundFilesCreated.txt"	
	void									removeListFile( void );
	void									addNameToFileList(char *name );
	
	void									RemoveUnusedFilesOnFileList( int dummy );
	void									attachDefaultPatternToAllPositions( int dummy);
	
	void									createDefaultRunaroundDelayFile( void );
	float									readRunaroundDelayFileValue( void );
	
#if defined(__cplusplus)
}
#endif
