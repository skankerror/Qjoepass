/*
 *  runaroundMenu.h
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


#if defined(__cplusplus)
extern "C"
{
#endif
	void			initRunaroundList( void );
	void			generateDefaultPattern( int dummy );
	void			_generateDefaultPattern( int dummy );
	
	void 			runaroundInitMenu( void );
	void 			runaroundListKey(  unsigned char theKey, int x, int y );
	void 			runaroundListMenuSelection( int id );
	
	void 			mSaveRunaroundProject( int dummy );
	void 			mSaveRunaroundProjectAs( int dummy );
	
	void 			moveOneFieldUp( void );
	void 			moveOneFieldDown( void );
	void 			moveOneFieldLeft( void );
	void 			moveOneFieldRight( void );
    void            unselectAllScenes( int dummy );

	void 			getMaxFieldsOnScreen( int *rMax, int *cMax);
	
	void 			newProject( int dummy );
	void 			closeRunaround( int dummy );
	void 			startList( int dummy );
	void 			copyToNextField( int dummy );
	void 			runaroundRemoveJuggler( int dummy );
	void 			runaroundAddJuggler( int dummy );
	void 			runaroundRemoveJuggler( int dummy );
	void 			runaroundAddPosition( int dummy );
	void 			runaroundAddBeforeSelectedPosition( int dummy );
	void 			runaroundRemovePositionAtEnd( int dummy );
	void 			runaroundRemoveSelectedPosition( int dummy );
	void 			runaroundSelectPattern( int dummy );
	void 			toggleNewLine( int dummy );
	void 			doublePattern( int dummy );
	void			shrinkToHalve( int dummy );
	
	void 			editPatternInCDEW( int dummy );
	void 			editPatternInTE( int dummy );
	void			runaroundShowAnimatedScene( int dummy ); 
	void 			showGrid( int dummy );
	void 			showPos( int dummy );
	void 			showPoi( int dummy );
	void 			showModifiers( int dummy );
	void 			horizontal( int dummy );
	void 			vertical( int dummy );
	void 			bigger( int dummy);
	void 			smaller( int dummy);
	void 			showFrame( int dummy );
	void			toggleSceneIndex( int dummy );
	void			rotateFrameClockwise( int dummy );
	void			rotateFrameCounterclockwise( int dummy );
	void 			moreCornersInFrame( int dummy );
	void 			lessCornersInFrame( int dummy );
	void 			exchange( int dummy );
	void			cycleCW( int dummy );
	void			cycleCCW( int dummy );
	
	
#if defined(__cplusplus)
}
#endif

