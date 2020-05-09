/*
 *  sdInit.cpp
 *  joe2
 *
 * created by wolfgang 
 */
/*-----------------------------------------------------------------------
 JoePass! is a program that animates juggling patterns. 
 It also provides some editors that let you write your own patterns.
 Copyright (C) <1997---TODAY>  Wolfgang Westerboer
 
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
#include "globals.h"
#include "systemswitch.h"


#ifdef MAC_CLASSIC_VERSION

#if 0
// nothing to be done
#include <Files.h>
#include <Processes.h>
FSSpec    myAppFSSpec;

/*
system dependend initialisation
do whatever is neccessary to make your OS feel well
*/

// example: get process id and FsSpec for MacOS
wwwBoolean sdInit( void )
{
OSErr                  theErr;
ProcessSerialNumber    psn;
ProcessInfoRec         pi;

    theErr = GetCurrentProcess(&psn);

    if (noErr != theErr)
        return (theErr);

    pi.processInfoLength = sizeof (pi);
    pi.processName = NULL;
    pi.processAppSpec = &myAppFSSpec;

    theErr = GetProcessInformation(&psn, &pi);

    return (theErr);
}
#endif
// end of example
#endif
// end of MacOS
