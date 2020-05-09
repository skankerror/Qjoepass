/*
 *  prefDefinitionValues.h
 *  joe2
 *
 *  created by wolfgang on 29.09.05.
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

#define defSpeed			2.0
#define defWinSpeed			2.0
#define defFpsMovie			10
#define defFrameRate		30
#define defGravity			98.1

#ifdef OPEN_GL_VERSION
#define DefBodyColor		DefLightGray
#define DefBgColor			sysColor(COLOR_MAX*.3f,COLOR_MAX*.3f,COLOR_MAX*.8f)
#define DefObjectColor		DefRed
#endif
#define DefLeftColor		sysColor( COLOR_MAX*.12f,COLOR_MAX*.33f,COLOR_MAX*.1f )
#define DefRightColor		sysColor( COLOR_MAX*.5f,COLOR_MAX*.01f,COLOR_MAX*.12f)

#define DefGridColor		sysColor(COLOR_MAX*.25f, COLOR_MAX*.25f, COLOR_MAX*.25f)
#define DefTextColor		sysColor( 0 , 0 , 0 )
#define DefSkyColor			sysColor( COLOR_MAX*0.1f,COLOR_MAX*0.1f,COLOR_MAX*.3f)
#define DefStageColor		sysColor(COLOR_MAX,COLOR_MAX,COLOR_MAX)
#define DefLineColor		sysColor(COLOR_MAX,COLOR_MAX,COLOR_MAX)
#define DefEvent1Color		sysColor(COLOR_MAX , 0 , 0)
#define DefEvent2Color		sysColor( 0 , 0 , COLOR_MAX )
#define	SkySize				5000
#define	DefLevelOfDetail	5

//new, resolution independend window information
#define	DefWorldWinSizeX	0.003516f
#define	DefWorldWinSizeY	0.034028f
#define	DefWorldWinSizeW	0.370313f
#define	DefWorldWinSizeH	0.625000f

#define	defInfoWinSizeX		0.003516f
#define	defInfoWinSizeY		0.675694f
#define	defInfoWinSizeW		0.369922f
#define	defInfoWinSizeH		0.213889f

#define	defPatternWinSizeX	0.377734f
#define	defPatternWinSizeY	0.031250f
#define	defPatternWinSizeW	0.269141f
#define	defPatternWinSizeH	0.620833f

#define	defScenesWinSizeX	0.377734f
#define	defScenesWinSizeY	0.031250f
#define	defScenesWinSizeW	0.269141f
#define	defScenesWinSizeH	0.620833f
#define	defPositionWinSizeX	0.377734f
#define	defPositionWinSizeY	0.031250f
#define	defPositionWinSizeW	0.269141f
#define	defPositionWinSizeH	0.620833f

#define SideOffset			2
#define UpOffset			-30
#define BackOffset			30

#define CatchSideOffset		0
#define CatchBackOffset		0
#define CatchUpOffset		0
