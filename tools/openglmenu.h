/*
 *  openglmenu.h
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

#ifndef OPENGL_MENU
#define OPENGL_MENU

#include "mydefs.h"
#include "graphics.h"


/*      header for glut menu helper

        define menus for glut
        attach hotkeys
        set markers on selected menus depending on
        + state global switch (menuMarkerFlag)
        + argument passed to initMenu (->marker)

        routines are very basic stuff.
        dont remove entries from menus
        dont rename menus
        just initialize a list and let the routines do the rest of the work

        wolfgang westerboer, 10.09.2001
        westwolf@netcologne.de


_menuList demoList[]    = {
                                                        {"mainmenu",            newMenu,                0,              NULL,           worldMenuSelection},
                                                        {"quit",                        menuEntry,              'q',    NULL,           quit},
                                                        {NULL,                          menuListTerminator,     0, NULL, 0}     // last entry
                                                };
*/
//#define  wwwBoolean int

#if defined(__cplusplus)
extern "C" {
#endif


    enum        {menuEntry, newMenu, addAsSubMenu, noMenuHotkey, menuTitle, menuListTerminator};
    enum        {unselectMenu, selectMenu, dontChangeMenu};

    extern int          menuMarkerFlag;


    // all menu functions have to return a bool value
    // unselectMenu     -> menu will not be marked
    // selectMenu               -> menu will be marked
    // dontChangeMenu   -> menu will not change
    // remark:
    // as the mark is set as soon as the function connected to the menu returns
    // and the menu does only operate when the window is active,
    // interference between different menus and should not be a problem


    //
    typedef struct {
        char	*name;                             // name of menu entry (without placeholder for marker)
        // NULL == end of list
        // ""   == no menu entry, but hotkey
        int		type;                                       // what am I
        char	hotkey;										// hotkey
        int		*id;										// my id, if im a submenu (provided by programmer)
        /* if a submenu is defined,
           its id is saved in this placeholder.
           if the added to the menutree, well look for the menu id
           make sure that you have an own placeholder for every submenu.
        */
        void    (*function)(int i );    // function to be called if menu entry is selected
        // or
        // callback if entry is a (sub) menu

        // add a pointer to a wwwBoolean value if marker should be set automattically according to thes pointers content
        // NB: content will be toggled automatically
        // set it to zero if theres no such a value.
		bool	*marker;

        // this fields are for internal use only
        // initialize them with zero
        //                              int             menuID;                         // id of menu
        int     entry;                          // number of menuEntry (for internal use: set/remove markers)
        int		winID;
        int		menuID;
    }
    _menuList;

    void        initMenu( _menuList *menuList, bool generateHotkeyListing );
    wwwBoolean  _menuSelect( _menuList *m, int r );
    wwwBoolean  _menuHotkeySelect( _menuList *m, char c);
    void        _doMenuFunction( _menuList *m );
    char		*makeMenuString( _menuList *m, wwwBoolean selected );
    void		unselectMenuTitle( _menuList *m );
    void		selectMenuTitle( _menuList *m );
    void		checkHotkeys( _menuList *m );
    void		checkMenuStructure( _menuList *m );
    void		showMenuList(_menuList *m);
    void		reCreateMenu( int winID, _menuList *m );

    extern		wwwBoolean globalOpenGLMenuInUse;
    void		globalMenuStatusFunc( int, int, int );

#if defined(__cplusplus)
}
#endif

#endif
