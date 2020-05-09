/*
 *  dxf.cpp
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


#include "globals.h"
#include "graphics.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "grafik.h"

#include "dxf.h"
#include "farbwerte.h"


#include "vtypes.h"
#include "fileIO.h"

typedef float vec[4];

wwwBoolean              Is_DXF(char *p);
int                     read_DXF(char *p);

static void ReadLayers( void);
static void     ReadEntities( void);
static void     Read3DFace();

static void     saveCharacteristics( char *name, int colorIndex );
void setCharacteristics( char *name );

char*           parseWord(  char *p, char *delimeters );
int                     parseInt(  char *p, char *delimeters );
float           parseFloat(  char *p, char *delimeters );

char*           readDxfFile(char *filename );

typedef struct  {       char            name[20];
                                        sysColor        c;
                                }       _layerInfo;

#define MAX_ENTRIES     100
_layerInfo characteristics[MAX_ENTRIES];
int maxEntries;

char tokenList[] = " \r\n\t";

char *readDxfFile(char *filename )
{
        FILE *fp;
        size_t s;
        char *b;

//      freeLayerInfos();

        if( (fp = fopen(filename, "rb"))==NULL)
                return NULL;

        s = getFileSize( fp );
        if( (b = (char *)malloc( s +1))== NULL )
        {       fclose( fp );
                return NULL;
        }

        fread( b, s, 1, fp );
        fclose (fp );
        *(b+s) = '\0';
        return b;
}




wwwBoolean makeGlListFromDxfFile( int list, char *filename )
{
        char *p = NULL;
        GLUquadricObj*  quadObj = NULL;


        if( (p = readDxfFile( filename ) ) == NULL )
        {       return false;
        }

        if( p && !Is_DXF( p ) )
        {       free( p );
                return false;
        }
        /* Initialise display lists */
        glNewList(list, GL_COMPILE);

        quadObj = gluNewQuadric();
        gluQuadricDrawStyle(quadObj, GLU_LINE);// GLU_FILL);
        gluQuadricNormals(quadObj, GLU_SMOOTH);

        glEnable( GL_AUTO_NORMAL);

        glPushMatrix();
        if(p )
                read_DXF( p );

        glPopMatrix();
        glEndList();

        if( p )
                free( p );

        if( quadObj )
                gluDeleteQuadric(quadObj);

        return true;

}

wwwBoolean Is_DXF(char *p)
{
        while (isspace(*p))
        {
                ++p;
        }
        if ((p[0] == '9') && (p[1] == '9') && (p[2] == '9'))
                return true;

        return (*p == '0');
}

char *parseWord(  char *p, char *delimeters )
{
        char *q;

        assert( p == NULL );

        q = strtok( p, delimeters);

        return q;
}

int parseInt(  char *p, char *delimeters )
{
        char *q;

        assert( p == NULL );

        q = strtok( p, delimeters);

        return atoi(q);
}

float parseFloat(  char *p, char *delimeters )
{
        char *q;

        assert( p == NULL );

        q = strtok( p, delimeters);


        return atof( q );
}


int read_DXF(char *p)
{
        char *name;

        name = strtok( p, tokenList);

        // identify sections
        while (name)
        {
                if( 0 ==strcmp(name,"TABLES"))
                        ReadLayers();

                if(0 == strcmp(name,"TABLES"))
                        ;
                else if( 0 == strcmp(name,"BLOCKS"))
                        ;
                else if (0 == strcmp(name,"ENTITIES"))
                        ReadEntities();
                else if (0 == strcmp(name,"LAYER"))
                        ReadLayers();
                name = parseWord( NULL, tokenList);

        }

        return 0;
}

static void ReadLayers( void)
{
        char* name = "";
        char *layerName;

        while( 0 != strcmp( name, "0" ))
        {
                if (0 == strcmp(name,"2"))
                {       layerName = parseWord(  NULL, tokenList);
                }
                if (0 == strcmp(name,"62"))
                {       int                     colorIndex;

                        colorIndex = parseInt( NULL, tokenList);
                        saveCharacteristics( layerName, colorIndex );
                }
                name = parseWord( NULL, tokenList);
        }
}

static void ReadEntities( void)
{
        char* name = "";


        while( 0 != strcmp( name, "ENDSEC" ))
        {

        /*      if (name == "LINE")                             ReadLine(pCurr, pEnd, parser);
                else if (name == "POLYLINE")    ReadPolyLine(pCurr, pEnd, parser);
                else if (name == "INSERT")              ReadInsert(pCurr, pEnd, parser);
        */
                if (0 == strcmp(name,"3DFACE"))
                        Read3DFace( );
                if (0 == strcmp(name,"62"))
                {       //int                   colorIndex;
/*
                        colorIndex = parseInt( NULL, tokenList);
                        s = dxfColorIndices[colorIndex];
                        glColor3f( s.x, s.y, s.z);

*/              }
                name = parseWord( NULL, tokenList);
        }
}

static void Read3DFace()
{
        char*           name;
        int             code;
        wwwBoolean      done = false;
        vec             v1,v2,v3,v4;

        while (!done)
        {
                name = parseWord( NULL, tokenList);
                code = atoi( name );
                switch (code)
                {
                        case 0:
                                done = true;
                                break;

                        case 8:                                         // layer name
                                name = parseWord( NULL, tokenList);
                                setCharacteristics( name );
                                break;

                        case 10:                                        // X1
                                v1[0] = parseFloat( NULL, tokenList);
                                break;

                        case 11:                                        // X2
                                v2[0] = parseFloat( NULL, tokenList);
                                break;

                        case 12:                                        // X3
                                v3[0] = parseFloat( NULL, tokenList);
                                break;

                        case 13:                                        // X4
                                v4[0] = parseFloat( NULL, tokenList);
                                break;

                        case 20:                                        // Y1
                                v1[1] = parseFloat( NULL, tokenList);
                                break;

                        case 21:                                        // Y2
                                v2[1] = parseFloat( NULL, tokenList);
                                break;

                        case 22:                                        // Y1
                                v3[1] = parseFloat( NULL, tokenList);
                                break;

                        case 23:                                        // Y2
                                v4[1] = parseFloat( NULL, tokenList);
                                break;

                        case 30:                                        // Z1
                                v1[2] = parseFloat( NULL, tokenList);
                                break;

                        case 31:                                        // Z2
                                v2[2] = parseFloat( NULL, tokenList);
                                break;

                        case 32:                                        // Z1
                                v3[2] = parseFloat( NULL, tokenList);
                                break;

                        case 33:                                        // Z2
                                v4[2] = parseFloat( NULL, tokenList);
                                break;

                        case 62:                                        // color index
                                {       int                     colorIndex;
                                        sysColor        s(1,1,1);

                                        colorIndex = parseInt( NULL, tokenList);
                                        s = dxfColorIndices[colorIndex];
                                        glColor3f( s.x, s.y, s.z);
                                        break;
                                }
                        case 70:                                        // group index
                                parseInt( NULL, tokenList);
                                break;

                        default:                                        // ignore this char*
                                break;
                }
        }

//      glBegin( GL_QUADS);
        glBegin( GL_QUAD_STRIP );

// calculate normals vector
{
        Vector3D n1,n2,s;
        Vector3D va, vb, vc;
        va.x = v1[0];
        va.y = v1[1];
        va.z = v1[2];
        vb.x = v2[0];
        vb.y = v2[1];
        vb.z = v2[2];
        vc.x = v3[0];
        vc.y = v3[1];
        vc.z = v3[2];

        n1 = va - vb;
        n2 = va - vc;
        s = n1.vektorProdukt(n2).normalised();
        glNormal3f( s.x, s.y, s.z);
}

        glVertex3f(v1[0], v1[1],v1[2]);
        glVertex3f(v2[0], v2[1],v2[2]);
        glVertex3f(v3[0], v3[1],v3[2]);
        glVertex3f(v4[0], v4[1],v4[2]);

        glEnd();
}

void saveCharacteristics( char *name, int colorIndex )
{
        sysColor s = dxfColorIndices[colorIndex];

        if( maxEntries == MAX_ENTRIES )
                return;

        strcpy( characteristics[maxEntries].name, name );
        characteristics[maxEntries].c = s;

        maxEntries++;
}

void setCharacteristics( char *name )
{
        int c;
        sysColor s;

        _layerInfo *p = characteristics;

        for( c = 0; c < maxEntries && c < MAX_ENTRIES; c++ )
        {       if( strcmp( p[c].name, name) == 0)
                {       s = p[c].c;
                        glColor3f(s.x,s.y,s.z);
                        break;
                }
        }
}
