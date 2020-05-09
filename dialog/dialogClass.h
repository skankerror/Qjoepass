/*
 *  dialogClass.h
 *  joe2
 *
 *  created by wolfgang on 02.12.06.
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

#define MAX_DIALOG_LINES		6

typedef class dialog{
	
public:
	dialog( char *m );
	~dialog( );

	void	draw( void );
	void	key(unsigned char theKey, int x, int y);
	void	mouse(int button, int state, int x, int y);

private:
	void	redisplayDialogWindow( void );
	void	clearDialogString( void );	
	void	setDialogString( char *s);
	void	showDialogString( void );
	void	reshapeDialog(int width, int height);
	void	createDialogWindow( void );
	void	initDialog( void );
	int		id;
	char	*dialogString;

	char	*dialogLineBuffers[MAX_DIALOG_LINES];
	char	*dialogStringBuffer;
	int		dialogLineCount;
	
	char	dialogExitKey;
}dialog;
