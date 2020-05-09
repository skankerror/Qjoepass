#ifndef __GRAPHICS__
#define __GRAPHICS__

#include "systemswitch.h"

#ifdef MAC_OS_X_VERSION
#include <GLUT/glut.h>
#elif MAC
#include <glut.h>
#else
#include <GL/glut.h>
#endif

#include "opengltools.h"


#endif
