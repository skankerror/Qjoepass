//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//                  switches and definitions
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
 * cdewDefs.cpp
 * joe2
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
//global definititons
#define 	DefaultThrow 	1
#define 	FSIZE 			FILENAME_MAX	// file definitions
#define 	READ_OR_WRITE	0x0 			// fake a UNIX mode 
#define 	dwellTime		0.5					// for JoePass patterns
#define 	minTime		 	.3


#define		CDEW_1_FILE_NAME 	"causal1.pass"
#define		CDEW_TEMPLATE_FILE	"causalTemplate.pass"
#define 	CDEW_PREF_NAME 		"cdew_prefs.txt"
#define		CDEW_START_NAME		"cdewStart.pass"
#define		CDEW_CRASH_NAME		"cdewLast.pass"
#define		CDEW_SELECT_FILE 	"causalSelect.pass"


#define	CausalContentTitle "causal editor"
#define cdewVersionString	"cdew Version "
#define cdewMagicString		"#" cdewVersionString
#define	MhnTag				"#mhn*"
#define	CMhnTag				"#cmhn*"
#define	ObjectCountTag		"#objectCount "
#define DelayMode			"#D - "
#define	DelayTag			"#jugglerDelay "
#define symTag				'%'
#define NL					"\n"
#define STARTCDEWSTRING		"start of cdew output"
#define CDEWSIZE			"cdewsize"
#define CDEWDISPLAYMODE		"cdewDisplayMode"
#define CDEWCOLORMODE		"cdewColorMode"
#define CDEWPRINTSITESWAPMODE	"cdewPrintSiteswapMode"
#define CDEWLINECOLORMODE	"cdewLineMode"
#define CDEWCELLCOLOR		"cdewCellBackground"
#define CDEWJUGGLERLINES	"cdewLineSeperators"
#define	CDEWHANDDELAY		"cdewHandDelay"
#define	CDEWSHOWNUMBERS		"cdewShowNumbers"
#define	CDEWSHOWHURRIES		"cdewShowHurries"
#define	CDEWSHOWCROSSES		"cdewShowCrosses"
#define	CDEWHIDECROSSES		"cdewHideCrosses"
#define	CDEWHURRIESFORBIDDEN "cdewUnhurriedDiagram"
#define CDEWCELLWIDTH		"cdewCellWidth"
#define CDEWCELLHEIGHT		"cdewCellHeight"
#define	TEMPWINDOWX			"tempWindowX"
#define	TEMPWINDOWY			"tempWindowY"
#define	TEMPWINDOWW			"tempWindowW"
#define	TEMPWINDOWH			"tempWindowH"
#define CELLNAME			"cell"
#define CELLDIVIDER			"="
#define cdeDelayTag			"cdeDelay"
#define cde2HandDelayTag	"cdeTwoHandDelay"
#define UNDO_FILE_NAME 		"Temp"
#define MaxCellColors		2
#define BOW_FACTOR			.99f
#define CDEWCREATOR         "cdewCreator"

#define headerEnding		".head"
#define HeaderTag			"!+Header"
#define EndHeaderTag		"!-Header"
