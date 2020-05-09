/*
 *  pathAccess.h
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

#ifndef __PATHACCESS__
#define __PATHACCESS__

#if defined(__cplusplus)
extern "C"
{
#endif

#include "mydefs.h"

#define         PATH_SIZE 2000

const char	*getSysDirName();
const char	*getAppDirName();
char		*getAppFolder(const char *);
char		*makeAbsoluteFilename(const char *, const char *);
char		*makeAbsolutePath(const char *, const char *);
void		printcwd();
int			createFolder(char *);
void		restorePath();
void		fixDirName( char *d);

extern char	applicationDir[];
extern char	*appDir, *sysDir;
#if defined(__cplusplus)
}
#endif


#endif
