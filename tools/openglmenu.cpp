/**********************************************************************

c routines for glut menu helper

usage:
build a menu list (to be documented)
build a glut window
call menuInit to attatch the menuList to the window
if you want to use hotkeys, call _menuHotkeySelect from your key func

version:     0.0
date:        10.09.2001
author:      wolfgang westerboer
email:       westwolf@netcologne.de
***********************************************************************/
#include "systemswitch.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "openglmenu.h"
#include "mydefs.h"
#include "myutil.h"
#include "prefDef.h"
#include "cmdLine.h"
#include "applicationFolders.h"
#include "myPrintf.h"

#include "info.h"
#include "animationWin.h"
#include "listWindow.h"
#include "inputWindow.h"
#include "runaround.h"

#define PRINT_MENU_SELECTION 0
#define SHOW_MENU_SELECT_INFO 0
#define SHOW_MENU_BUILD 0
#define SHOW_ADD_MENU_ENTRY 0

// bool showMenuHotkeys = true,
//bool                showMenuSelectors = true;

#undef  printf

void    showMenuInfo( _menuList *m );
void    menuDummy( int c );

wwwBoolean globalOpenGLMenuInUse = false;
/**********************************************************************
add all characters to the list
that may not appear in menus under certain os
(for hotkey usage, see makeMenuString)
***********************************************************************/
static  char *MENUKEY_AVOID_LIST = "/\033";

static  const char	SelectedMenuChar	= '+', 
UnselectedMenuChar	= ' ';
int					menuMarkerFlag		= dontChangeMenu;


wwwBoolean _menuSelect( _menuList *m, int r )
{
#if PRINT_MENU_SELECTION
	int id = glutGetWindow();
	printf("menu id %d called in window %d\n", r, id );
#endif
	
	if( m == NULL )
	{       printf("NULL menu selected\n");
		return false;
	}
	
	while( m->name )
	{
		
		
		if( m->id != 0 && m->id == (void *)r )
		{
#if PRINT_MENU_SELECTION
			printf("menu function is %s\n", m->name );
#endif
			_doMenuFunction( m );
			return true;
		}
		m++;
	}
	_myBeep();
	return false;
}

wwwBoolean _menuHotkeySelect( _menuList *m, char c)
{
	
	if( m == NULL )
		return false;
	
	while( m->name )
	{
		if( m->hotkey == c)
		{
#if PRINT_MENU_SELECTION
			printf("menu function is %s\n", m->name );
#endif
			_doMenuFunction( m );
			return true;
		}
		m++;
	}
	
	
	return false;
}
 
// ------------------------------------
// menu action
// toggle marker flag
// set men according to marker flag
// call attached function
// ------------------------------------
void _doMenuFunction( _menuList *m )
{
	
#if PRINT_MENU_SELECTION
	printf("menu function for id %d:\n%s\n%lx\n", m->menuID, m->name, m->marker);
#endif
	
	// set markers according to vars / flags
	
	// does a marker exist? 
	if( m->marker )
	{
		
		// toggle entry
		*m->marker = (*m->marker == false );
		
		
#if SHOW_MENU_SELECT_INFO
		showMenuInfo( m );
#endif
		
		if( listID){
			reCreateMenu( listID, runaroundListMenuList);
			reCreateMenu( inputID, runaroundListMenuList);
		}
		
		if( infoWin)
			reCreateMenu( animationWin, worldMenuList);
		if( animationWin )
			reCreateMenu( infoWin, infoMenuList);

		// set Marker in Menu
		if( *m->marker )
			selectMenuTitle( m );
		else
			unselectMenuTitle( m );
		
	
	}
	
	
	// if a function is defined, call it
	if(m->function )
		m->function( 0 );
	
	
	// check if function fourced a change of menu state
	switch( menuMarkerFlag )
	{
		case unselectMenu:
			unselectMenuTitle( m );
			break;
		case selectMenu:
			selectMenuTitle( m );
			break;
		case dontChangeMenu:
		default:
			break;
	}
	
	// reset flag
	menuMarkerFlag = dontChangeMenu;
	
}


char *makeMenuString( _menuList *m, wwwBoolean selected )
{
	char *p;
	char c;
	char *t;
	
	// Here I allocate more mem than I acutally need
	// just in case I'll decide to increase number of spaces,
	// or make some other mistakes that causes bugs which are difficult to find
	int markerSpace = 10;
	int hotkeySpace = 10;
	
	//printf("name = %s, hotkey = %c (%d)\n", m->name, m->hotkey, m->hotkey);
	p = (char *)malloc( strlen(m->name) + markerSpace + hotkeySpace+ 1000); //wwwwww
	
	/*#if 0
#if TARGET_OS_MAC
        if( m->hotkey )
		sprintf(p, "%c %s /%c", (selected?SelectedMenuChar:UnselectedMenuChar), m->name, m->hotkey);
	else
		sprintf(p, "%c %s", (selected?SelectedMenuChar:UnselectedMenuChar), m->name);
#else
	if( m->hotkey)
		sprintf(p, "%c %s [%c]", (selected?SelectedMenuChar:UnselectedMenuChar), m->name, m->hotkey);
	else
		sprintf(p, "%c %s", (selected?SelectedMenuChar:UnselectedMenuChar), m->name);
#endif
	
#else
	*/
	if( m->hotkey == 0 )
		t = 0;
	else
		t = strchr(MENUKEY_AVOID_LIST, m->hotkey);
	
	if( 01 )
	{       int printedChars = 0;
		char *actualPos = p;
		
		if( jpPrefs.showMenuSelectors )
			printedChars = sprintf(actualPos, "%c ",(selected?SelectedMenuChar:UnselectedMenuChar));
		else
			printedChars = sprintf(actualPos, "  ");
		actualPos += printedChars;
		
		if( jpPrefs.showMenuHotkeys && (c = m->hotkey ) != 0 && t==0)
			if( c >= ' ')
				printedChars = sprintf(actualPos, "[%c] ",  m->hotkey);
			else
				printedChars = sprintf(actualPos, "[+%c] ",  m->hotkey+'a'-1);
		else
			printedChars = sprintf(actualPos, "    ");
		
		actualPos += printedChars;
		
		
		sprintf(actualPos, "%s", m->name);
		
	}
	return p;
}

void checkHotkeys( _menuList *mList )
{
	_menuList *a, *b;
	
	for( a = mList; a->name != NULL; a++ )
	{
		for( b = mList; b->name != NULL; b++ )
		{
			if( a != b && a->hotkey != 0 && a->hotkey == b->hotkey )
			{
				printf("can't add hotkey >%c< for menu >%s<\n",  b->hotkey,b->name );
				printf("already defined for menu >%s<\n", a->name );
				
				getchar();
				exit( 1 );
			}
		}
	}
	
}


// menu entry with same name exists
void checkMenuStructure( _menuList *m )
{
	_menuList *fistMenuPointer = m, *t;
	t = fistMenuPointer;
	
	
	for( t = fistMenuPointer; t != m; t++ )
	{
		if( *m->name && strcmp( m->name, t->name ) == 0)
		{
			printf("menu entry >%s< already exists\n", m->name );
			getchar();
			exit( 1 );
		}
	}
}

void showMenuList(_menuList *m)
{
	while( m->name )
	{
		
		if( m->id)
		{
			if( (intptr_t)m->id >= 0)     // submenu
				printf("m->id = %d\n", *m->id);
			else                    // menu entry
				printf("m->id = %d\n", (intptr_t)m->id);
		}
		m++;
	}
}

void menuDummy( int c )
{
#if MAC_CLASSIC_VERSION
#pragma unused (c)
#endif
	
	exit(0);
}

/*------------------------
/ build menus from list
------------------------*/
void initMenu( _menuList *m, bool generateHotkeyListing )
{
	//      static
	int i = 0;
	//      static
	int id = 0;
	char *newMenuString;
	FILE *fp = NULL;
	
	if( generateHotkeyListing)
		fp = fopen( hotkeyPath, "a" );
	
#if SHOW_MENU_BUILD
	printf("\n\n\nmenuList = %lx\n", m );
#endif
	while( m->name != NULL )
	{
		if( 1 )
			checkHotkeys(m);
		if( 0 )
			checkMenuStructure(m);
		// printf("menustring: %s\n", m->name);
		newMenuString = makeMenuString( m, m->marker?*m->marker:false );
		// printf("newMenuString: %s\n", newMenuString);
		
		m->menuID = glutGetMenu();
		
		switch (m->type )
		{
			case menuTitle:
				if( fp )
					fprintf(fp,
						"%c"
						"--------------------------------------------------\n"
						"%s - hotkey list\n"
						"values in brackets are hex values, use control key\n"
						"--------------------------------------------------\n", 12, m->name);
				break;		
			case menuEntry:
				m->id = (int *)--i;
				glutAddMenuEntry( newMenuString, (intptr_t )m->id);
#if SHOW_MENU_BUILD
#if SHOW_ADD_MENU_ENTRY
				
				printf("%s |%d = %d, flag = %lx\n", m->name, m->id, m->marker);
#endif
#endif
				if( fp ){
					char c = m->hotkey;
					
					if( c ){
						if( c < 32 )
							fprintf(fp," (%02x): %s\n", c, m->name);
						else
							fprintf(fp," %c   : %s\n", c, m->name);
					}
				}
				
				m->entry = glutGet(GLUT_MENU_NUM_ITEMS);
				//                              printf("m->entry = %d\n", m->entry);
				break;
			case newMenu:
				m->menuID = id = glutCreateMenu(m->function);
				if( m->id )
					*m->id = id;
#if SHOW_MENU_BUILD
					printf("new menu %s, id = %d\n", m->name, id);
#endif
				break;
			case addAsSubMenu:
				if( m->id )
				{
					glutAddSubMenu(newMenuString, *m->id);
#if SHOW_MENU_BUILD
					printf("addAsSubMenu %s, id = %d\n", m->name, *m->id);
#endif
				}
				else
					printf("initMenu: wrong submenu structure. m->id is NULL in %s\n", m->name);
				
				break;
			case noMenuHotkey:
				break;
			default:
				printf("initMenu: wrong menu parameter in %s\n", m->name);
				break;
				
		}
		free( newMenuString );
		m++;
	}
	if( fp ){
		fprintf(fp,"--------------------------------------------------\n\n");
		fclose( fp );
	}
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void reCreateMenu( int winID, _menuList *m )
{
	int id;
	
	if( winID <= 0 )
	{
		_myPrintf("reCreateMenu(): unknown window id %d\n", winID);
		return;
	}
	id = glutGetWindow();
	
	glutSetWindow( winID);
	glutDestroyMenu( winID);
	initMenu( m, false );
	glutSetWindow( id );
	
	
}

void unselectMenuTitle( _menuList *m )
{
	char *p;
	p = makeMenuString( m, false );
	
	glutSetMenu( m->menuID);
	glutChangeToMenuEntry( m->entry, p, (intptr_t)m->id);
	
	free( p );
}


void selectMenuTitle( _menuList *m )
{
	char *p;
	p = makeMenuString( m, true );
	
	glutSetMenu( m->menuID);
	glutChangeToMenuEntry( m->entry, p, (intptr_t)m->id);
	
	free( p );
	
}

void showMenuInfo( _menuList *m )
{
	int menuID = glutGetMenu();
	printf( "(un)select menu (entry = %d, id = %d, menuID = %d, saved = %d)\n", m->entry, (intptr_t)m->id, menuID, m->menuID);
	
}


void globalMenuStatusFunc( int status, int x, int y)
{
#if MAC_CLASSIC_VERSION
#pragma unused( x,y)
#endif
	
	if( status == GLUT_MENU_IN_USE )
		globalOpenGLMenuInUse = true;
	else
		globalOpenGLMenuInUse = false;
	
}
