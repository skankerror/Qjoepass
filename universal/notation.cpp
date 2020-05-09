/*
 *  notation.cpp
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

/* 
Note:
 this file uses slightly 2 different methods to transfer a notation to 
 a: the animation
 b: the causal editor
 
 technical background:
 jp uses 2 different ways to denote delayed patterns:
 a: airtime notation, manual method: 
#D-    ! manual calculation of airtime
#d2 0.5 ! amount of delay
 <3.5p|3.5px>  !pattern
 b: siteswap notation, automatic method: 
#D+    ! automatic calculation of airtime
#d2 0.5 ! amount of delay
 <3p|4p>  !pattern
 
 The animation uses a while the routines in the causal editor uses b. 
 
 Discussion:
 b: looks cleaner to me, as doesn´t need the x parameter to denote the destination hand. 
 This is done by the same rule as in vanilla siteswap, no matter if it´s a self or a pass.
 This method reaches it´s limits in extreme patterns like 0.5p, which will be expanded to 
 ...
 <0p|1p>
 0p or 0x never really worked.
 a: does not have this limitation
 
 the routines from the causal editor can read notation b even with these extreme patttern, so I didn´t change them.
 for the correctness of the animation I added support for method a as well.
 
 */ 


#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "notation.h"
#include "mainGL.h"
#include "parseSiteswap.h"
#include "fileIO.h"
#include "myPrintf.h"
#include "applicationFolders.h"
#include "pattern.h"

#include "cdewClass.h"
#include "causal_editor.h"
#include "cdewMenu.h"

#include "editor.h"
#include "prefDef.h"
#include "pathAccess.h"

#include "mydefs.h"
#include "cmdLine.h"
#include "globals.h"



#if 0
#define VERBOSE_PASSING_INFO
#endif


#define USE_DMINUS_NOTATION true

#define ASYNC_MINUS_HEADER    "#sx \n"\
"#D -\n"\
"#d 2 .5\n"

#define ASYNC_PLUS_HEADER    "#sb\n"\
"#D +\n"\
"#d 2 .5\n"

#define SYNC_HEADER     	"#sb\n"


void    	printThrowsToFile( char *buf, int start, int offset, int max, FILE *fp, int mode);
bool		_fourHandsToTwoHands( char *in, int mode);
void    	handleStdPattern( char *p );
void		getIndicesForThrow( int cnt, int j, int h, int l, int *ij, int *ih, int *ie);
char		*prepareNHandedBuffer( char *in, int h);
char		*makeSpacefreeCopyOfString( char *in);
char		*removeStuff( char *i );







void printThrowsToFile( char *buf, int start, int offset, int max, FILE *fp, int mode)
{
	int c;
	float f;
	
	
	for( c = start; c < max  ; c += offset )
	{
		char z = buf[c];
		
		if( z == 'p' )
			fprintf(fp,"p");
		else
		{
			if( z >= 'A' )
				f = z - 'A' + 10;
			else
				f = z - '0';
			
			f = f/2.;
			
			
			if( f == (int) f)
				fprintf(fp, "%d ", (int)f);
			else
			{
				
				if (mode == DPlusMode)
				{
					if( start == 0 )
						fprintf(fp, "%dp ", (int)f);
					else
						fprintf(fp, "%dp ", (int)f+1);
				}else
				{
					fprintf(fp, "%d.5p%s", (int)f, start != 0?"x ":" ");
				}       
				
				
			}
		}
	}
}

/* no syntax checking,
   pattern is asumed to be formatted propperly
*/
char *relToAbsPass( char *buf )
{
	int j = 0;
	char *p;
	
	// look for starting sequenz
	p = strchr( buf, '<' );
	if( p == NULL )
		return buf;
	j = 1;
	while( *p && *p != '>'){
		if( *p == '|' )
			j++;
		p++;
	}
	
	int jj = 1;
	int r, a;
	// replace all rn by p(n+j)%j
	p = strchr( buf, '<' );
	while( *p && *p != '>'){
		if(*p == 'r' ){
			*p++ = ':';
			if( *p != '0'){
				r = *p - '1'+1;
				a = (r+jj)%j;
				*p =a-1 + '1';
			}
		}else if(*p == '|' )
			jj++;
		p++;
	}
	
	myPrintf( buf );
	return buf;
}


void handleStdPattern( char *p )
{
	
	if( jpPrefs.generateCausalFromTextEditorOnStart)
	{
 		oglwin *w;
		size_t l = strlen( p );
		char *p2 = (char *)malloc( l + 1);
		sprintf(p2, "%s", p);
		
		// change relative passes to absolute passes
		relToAbsPass( p2 );
		
 		w = getOrCreateOglwinOnePersonNotation( p2 );
		
		glutSetWindow( w->id );
		//cdewAnimate( 0 );
		if( jpPrefs.autosaveCausalFromTextEditor )
			cdewSaveAuto(0);
		
		free( p2 );
	}
}



bool startOnePersonPassNotation( char *p, wwwBoolean checkForPassingPattern)
{
	
	startOnePersonPassNotationFlag( p, checkForPassingPattern, startAnimationMode);
	return startOnePersonPassNotationFlag( p, checkForPassingPattern, generateCausalMode); //yyy geht nicht richtigww
}	


/*
yyy aufräumen!
Eine Funktion, die 2 verschiedene Ergebnisse liefert, abhängig von Eingabe!
=> D+ und D- werden unterschiedlich behandelt, was grundsätzlich ok ist.
ABER: Die Ausgaben werden vollständig unabhngig voneinander erzeugt => sehr Fehleranfällig!
Ausgabe vereinheitlichen
//enum	{DPlusMode = generateCausalMode, DMinusMode = startAnimationMode};
// definiert in notation.h
 */
bool startOnePersonPassNotationFlag( char *p, wwwBoolean checkForPassingPattern, int modeFlag)
{
#if 01
	char *dot = 0;
	size_t l, c;
	float delay = -1, thisDelay;
	size_t beats;
	float values[1000];
	char buf[5000];
	wwwBoolean hasPassTag;
	
	
	l = strlen( p );
	if( l == 0 )
		return false;

	if( strchr( p, '#')) {
		validPattern = false;
		return false;
	}
		
	if( strchr(p, '<')){
		handleStdPattern( p);

		if( modeFlag == startAnimationMode){
			return startPattern( p );
		}
		return true;
	}

	
//	erzeuge causals von nicht sync mustern
	hasPassTag = false;
	
	beats = 0;
	
	for( c = 0; c < l; c++ ){
		char t = p[c];
		
		if( t == '\0')
			break;

		if( t == '\0' || isdigit( t ) || t == '.' || t == 'p' ||  t == ' ' || t =='\r' || t == '\n')
			; // ok, valid
		else{
#ifdef  VERBOSE_PASSING_INFO
			//		printf("wrong character %c (=%03d)\n", t,t);
#endif
	}
		
		// frictional value, preceeds passing tag
		if( t == '.' ){
			if( dot ){
#ifdef VERBOSE_PASSING_INFO
				printf("more than one '.' char before 'p'\n");
#endif
				return false;
			}
			else
				dot = &p[c];
		}
		
		// count beats
		// dont count delays, i.e. dont count while dot != 0
		if(!dot && isdigit( t )){
			values[beats] = t -'0';
			// next beat
			beats ++;
		}
		
		
		// passing tag
		if( t == 'p' ){
			if( beats < 1 ){
				
#ifdef VERBOSE_PASSING_INFO
				printf("no value given before pass\n");
#endif
				return false;
			}
			
			hasPassTag = true;
			
			// calculate delay
			if( dot ){
				p[c] = '\0';
				thisDelay = atof( dot );
				p[c] = 'p';
				
				if( delay < 0 ){
					delay = thisDelay;
#ifdef VERBOSE_PASSING_INFO
					printf("delay set to %.2f\n", delay);
#endif
				}else{
					if( delay != thisDelay ){
#ifdef VERBOSE_PASSING_INFO
						printf("cant handle different delays\n");
#endif
						return false;
					}
				}
			}else
				thisDelay = delay = 0;
			
			dot = 0;
			
			values[beats-1] += delay;
			values[beats-1] *= -1;			
		}
		
		
	}
	if( dot ){
#ifdef VERBOSE_PASSING_INFO
		printf("'p' is missing - cant handle delayed selfs\n");
#endif
		return false;
	}
	
	if( beats == 0){
		printf("empty pattern\n");
		return false;
	}
	
	
	
	if(checkForPassingPattern && !hasPassTag ){
#ifdef VERBOSE_PASSING_INFO
		printf("no passing pattern\n");
#endif
		return false;
	}
	
	/*
	 print siteswap to string
	 */
	{
		int cnt = 0;
		float tmp, sum;
		wwwBoolean syncPattern;
		int     *validSiteswap;
		wwwBoolean evenNumber;
		
		validSiteswap = (int*)malloc( sizeof(int) * beats);
		for( c = 0; c < beats; c++ )
 			*(validSiteswap+c)=0;
		
		sum = 0;
		
		/* calc sum for values
			frictional -> j2 does diagonal passes
			*/
		for( c = 0; c < beats; c++ ){
			int i;
			
			tmp = values[c];
			
			// add numbers
			if( tmp < 0 )
				tmp *= -1;
			sum += tmp;
			
			//check integrity
			i = (c+(int)tmp)%beats;
			validSiteswap[i] ++;
		}
		
		syncPattern = true;
		
		if( delay )
			syncPattern = false;
		
		for( c = 0; c < beats; c++ ){
			if( validSiteswap[c ] != 1 ){
				syncPattern = false;
				break;
			}
		}
		
		free( validSiteswap);
		
		
#ifdef VERBOSE_PASSING_INFO
		if( !syncPattern )
			printf("solo pattern is not a valid siteswap\n");
#endif
		
		evenNumber =    sum/beats == (int)(sum/beats);
		
		if( modeFlag == DMinusMode){
			cnt = 0;
			
			
			if( delay )
				cnt += sprintf(buf+cnt,"#sx\n #D - \n #d 1 %.2f\n", delay);
			
			
			
			
			if( hasPassTag )
				cnt += sprintf(buf+cnt,"%s", "< ");
			
			// juggler a
			for( c = 0; c < beats; c++ ){
				float v = values[c];
				
				if( v >= 0 )
					cnt += sprintf(buf+cnt, "%d ", (int)v);
				else{
					if( delay)
						cnt += sprintf(buf+cnt, "%.2fpx ", -v);
					else	
						cnt += sprintf(buf+cnt, "%.2fp ", -v);
				}
			}
			
			cnt += sprintf(buf+cnt,"%s", " | ");
			
#ifdef VERBOSE_PASSING_INFO
			printf("sum = %.2f\n", sum);
			printf("beats =mai %d\n", beats);
			printf("evenNumber = %d\n", evenNumber);
			printf("syncPattern = %d\n", syncPattern);
			printf("delay = %.2f\n", delay);
#endif
			
			// juggler b
			for( c = 0; c < beats; c++ ){
				float v;
				long offset;
				
				if( evenNumber ){
#ifdef VERBOSE_PASSING_INFO
					printf("even number\n");
#endif
					if( delay == 0){
						if( syncPattern)///sum / beats == (int)sum/beats)
								//sum /2 == (int)(sum/2) 	)
							offset = c;
						else
							offset =  (c  + beats/2 )%beats;
					}else

#ifdef VERBOSE_PASSING_INFO
						printf("odd number - vers 1.0\n");
#endif
					if( sum != (int)(sum) )
							offset =  (c + beats/2 )%beats;
						else
							offset =  (c + beats/2 )%beats;
				}else{
#ifdef VERBOSE_PASSING_INFO
						printf("odd number - vers 2.0\n");
#endif

				if( sum != (int)(sum) )
							offset =  (c + beats/2 )%beats;
						else
							offset =  (c + beats/2 )%beats;
					}
					v = values[ offset ];
					
					if( v >= 0 )
						cnt += sprintf(buf+cnt, "%d ", (int)v);
					else
						cnt += sprintf(buf+cnt, "%.2fp ", -v);
			}
			cnt += sprintf(buf+cnt,"%s", " >");
			
		}else{
			if( delay )
				cnt += sprintf(buf+cnt,"#sb\n #D + \n #d 1 %.2f\n", delay);
			
			if( hasPassTag )
				cnt += sprintf(buf+cnt,"%s", "< ");
			
			// juggler a
			for( c = 0; c < beats; c++ ){
				float v = values[c];
				
				if( v >= 0 )
					cnt += sprintf(buf+cnt, "%d ", (int)v);
				else{
					if( v == (int)(v) )
						cnt += sprintf(buf+cnt, "%dp ", -(int)v);
					else
						cnt += sprintf(buf+cnt, "%dp ", (int)-(v-delay));
				}
			}
			
			cnt += sprintf(buf+cnt,"%s", " | ");
			
	#ifdef VERBOSE_PASSING_INFO
			printf("sum = %.2f\n", sum);
			printf("beats = %d\n", beats);
			printf("evenNumber = %d\n", evenNumber);
			printf("syncPattern = %d\n", syncPattern);
			printf("delay = %.2f\n", delay);
	#endif
			
			// juggler b
			for( c = 0; c < beats; c++ ){
				float v;
				int offset;
				
				if( evenNumber ){
	#ifdef VERBOSE_PASSING_INFO
					printf("even number\n");
	#endif
					if( delay == 0){
						if( syncPattern)///sum / beats == (int)sum/beats)
								//sum /2 == (int)(sum/2) 	)
							offset = c;
						else
							offset =  (c  + beats/2 )%beats;
					}else
						if( sum != (int)(sum) )
							offset =  (c + beats/2 )%beats;
					else
						offset =  (c + beats/2 )%beats;
					
				}else{
	#ifdef VERBOSE_PASSING_INFO
					printf("odd number\n");
	#endif
					if( sum != (int)(sum) )
						offset =  (c + beats/2 )%beats;
					else
						offset =  (c + beats/2 )%beats;
				}
				v = values[ offset ];
				
				if( v >= 0 )
					cnt += sprintf(buf+cnt, "%d ", (int)v);
				else{
					if( v == (int)(v) )
						cnt += sprintf(buf+cnt, "%dp ", -(int)v);
					else
						cnt += sprintf(buf+cnt, "%dp ", (int)-(v+delay));
				}
			}
			cnt += sprintf(buf+cnt,"%s", " >");
		}
	}
#ifdef VERBOSE_PASSING_INFO
	printf("buf = %s \n", buf);
#endif
	if(commandLineTriesToStartPattern ){
_myPrintf("CmdLine starts symmetric pattern: \n\n%s\n\n", 		buf);
		saveFile( buf, editPath );
		validPattern = true;
		
	}
	
    if( modeFlag == DMinusMode){
		startPattern( buf);
    }else{
        if( jpPrefs.generateCausalFromTextEditorOnStart || generateCausalNow ){
			if(!startingUp)
				transferOnePersonNotationToCdewAndAutoSave( buf );
			else
				textEditorStartedPattern = true;
        }	}
	redrawTextEditor();

#endif
	
#ifdef VERBOSE_PASSING_INFO
	printf("validPattern = %d\n", validPattern    );
#endif
	
	
	return validPattern;
}


bool generateCausalDiagramFromBuffer( char *p, int jugglers )
{
#if 01
	extern bool jugglerNumberForNHandedPatternsDefined;
	int h;
	
	if (jugglerNumberForNHandedPatternsDefined)
		h = jugglers*2;
	else {
		h = 2;
	}


	generateCausalNow = true;
	if(  h != 2 )		
		nHandedToCausal( p, h);
	else if(  !startOnePersonPassNotationFlag( p, true, generateCausalMode))
		nHandedToCausal( p, h);
	generateCausalNow = false;
#else
	extern bool jugglerNumberForNHandedPatternsDefined;
	extern int jugglerNumberForNHandedPatterns;
	bool ret;

	//nHandedToCausal( p, jugglers*2);
	//return true;
	
	generateCausalNow = true;	
	if( jugglerNumberForNHandedPatternsDefined )
		//	ret = nHandedSiteswap( p, jugglerNumberForNHandedPatterns);
		//ret = 
		nHandedToCausal( p, jugglers*2);
	else	
		nHandedToCausal( p, 2);
//		startOnePersonPassNotationFlag( p, false, generateCausalMode);
	generateCausalNow = false;		
#endif
	

	return true;
}

bool nHandedSiteswap( char *in, int h)
{	
	bool ret;
	
	ret = nHandedToAnimation( in, h);
	if(jpPrefs.generateCausalFromTextEditorOnStart)
		nHandedToCausal( in, h);
	return ret;
}	


bool nHandedToAnimation( char *in, int h)
{
	size_t l;
	int j;
	int i;
	char *buf, *p;
	FILE *fp;
	char d[2000];
	char *i2, *i3;
	bool ret = false;
	
	
	// works only with 2 handed subjects
	if( h/2 != h/2.)
		return false;
	
	// remove all comments and commands here
	if( (i3 = removeStuff( in )) == NULL)
		return false;
	
	if( (i2 = makeSpacefreeCopyOfString( i3 )) == NULL ){
		free( i3 );
		return false;
	}
	
	l = strlen( i2 );
	if( l/2 != l/2.)
		l *= 2;
	
	if( l == 0 ){
		if( i2 )
			free( i2 );
		if( i3 )
			free( i3 );
		return false;
	}

	
	// ----------------------
	// prepare buffer
	// ----------------------
	if( (buf = prepareNHandedBuffer( i2, h ))==NULL){
		free( i2);
		return false;
	}
	
	// ----------------------
	// calc parameters
	// ----------------------	
	j = h/2;
	
	getcwd(d, 2000);
	chdir( sysDir);
	
	// ----------------------
	// create output file
	// ----------------------	
	if( (fp = fopen( tmpNotationPath, "w")) == NULL ){
		ret = false;
		goto bail;
	}
	// ----------------------
	// print header
	// ----------------------
	fprintf(fp, "!%s\n", i2 );
	free( i2 );
	
	
	fprintf(fp, "!%d hands\n", h );
	fprintf(fp, "#s x\n#D -\n");
	for( i = 0; i < j; i++ )
		fprintf(fp, "#jugglerDelay %d %.2f\n", i+1, ((float)i) / j);
	
	fprintf(fp, "%s", "\n\n");
	
	// ----------------------
	// transform siteswap
	// print result to file
	// ----------------------
	p = buf;
	while( *p)
	{
		if( j != 1 )
			fprintf(fp, "%s", "<");
		
		for( i = 0; i < j; i++)	{
			char c;
			int v;
			float vn;
			
			// ----------------------
			// next character
			// ----------------------
			c = *p++;
			if( isdigit( c ) ){
				if( c == '0')
					v = 0;
				else
					v = c-'1'+1;
			}else
				v = c - 'A'+10;
			
			// ----------------------
			// n-handed value
			// ----------------------
			vn = (float)v/j;
			
			// ----------------------
			// self
			// ----------------------
			if( vn == (int)vn) // a self
				fprintf(fp, " %.2f   ", vn);
			else{
				// ----------------------
				// pass
				// ----------------------				
				int relSS;
				bool changeDestHand;
				
				relSS = (int)v%j;				
				changeDestHand = (relSS+i>=j);
				
				fprintf(fp, " %.2f", vn);					// throw value
				fprintf(fp, "r%d", v%j);					// dest juggler
				fprintf(fp, "%c", changeDestHand?'x':' ');	// dest hand
			}
			
			if( j != 1){					
				if( i != j-1)
					fprintf(fp, "%s", " |");
				else
					fprintf(fp, "%s", " >\n");
			}
		}
	}
	// ----------------------
	// close output file
	// ----------------------
	fclose( fp );	
	ret = readAndStartPattern( tmpNotationPath );	
bail:
	// ----------------------
	// clean up
	// ----------------------
	free( buf );
	chdir( d );
	
	return ret;
}

// skip unwanted stuff 
char *removeStuff( char *i )
{
	char *outStart;
	char *out;
	
	outStart = out = (char *)malloc( strlen( i )+1);
	
	while( *i ){
		switch( *i ){
			// comments
			case '!':
				do{
					i++;
					
				}while( *i && *i!='\n' && *i != '\r');
				break;
			case '#':
				do{
					i++;
					
				}while( *i && *i!='\n' && *i != '\r' && *i != ';');
				break;
			case '"':
				do{
					i++;
					
				}while( *i && *i != '"');
				break;
				
			default:
				*out++ = *i++;
				break;
				
				
		}
		
	}
	*out = '\0';
	
	return outStart;
}



// ----------------------
// causal stuff
// ----------------------
void nHandedToCausal( char *in, int h)
{
	size_t l;
	int j;
	int i;
	char *buf, *p;
	int cnt;
	char d[2000];
	char *i2 = 0, *i3 = 0;
	
	
	// works only with 2 handed beings
	if( h/2 != h/2.)
		return;
	
	// remove all comments and commands here
	if( (i3 = removeStuff( in )) == NULL)
		return;
	
	if( (i2 = makeSpacefreeCopyOfString( i3 )) == NULL ){
		free( i3 );
		return;
	}
	
	l = strlen( i2 );
	if( l/2 != l/2.)
		l *= 2;
	
	if( l == 0 ){
		if( i2 )
			free( i2 );
		if( i3 )
			free( i3 );
		return;
	}
	// ----------------------
	// prepare buffer
	// ----------------------
	if( (buf = prepareNHandedBuffer( i2, h ))==NULL){
		if( i2 )
			free( i2);
		return;
	}
	if( i2 )
		free( i2 );
	
	// ----------------------
	// calc parameters
	// ----------------------	
	j = h/2;
	
	
	getcwd(d, 2000);
	chdir( sysDir);
	
	
	//	if(jpPrefs.generateCausalFromTextEditorOnStart )
	{
		int id;
		oglwin *cd;
		char name[2000];
		
		sprintf(name, "%s-%d_hands", theEditorTitle, h);
		id = getOglwinOnePersonNotationID();
		if(id == 0 || !generateTextToCausalInSameWindow)
			id = makeOglwin( name );
		else{
			glutSetWindow( id );
			glutSetWindowTitle( name );
		}
		cd = getOglwinFromID( id );
		cd->cdewJugglers = j;
		cd->cdewHands = 2;
		cd->cdewEntries = l;
		cd->freeCausalCells( cd->causalCells);
		cd->initCL();
		cd->imTheOnePersonNotationWin = true;
		cd->imTheAnimatedWindow = true;
		cd->setPatternSavedByUser(false);
		cd->fileChangedFlag = true;
		sprintf(cd->cdewFileName, "%s", name);
		if( jpPrefs.autosaveCausalFromTextEditor )
			cd->saveDiagramAuto( );
				
		for( i = 0; i < j; i++ ){
			float d = ((float)(i))/j;
			
			*cd->getDelayedJuggler(i, rightHand) = d;
			*cd->getDelayedJuggler(i, leftHand) = d;
		}
		
		p = buf;
		cnt = 0;
		while( *p)
		{
			char c;
			int v;
			int ch = 2;
			_causalCell *cc;
			int ij, ih, ie, dj, dh, de;
			
			c = *p++;
			if( isdigit( c ) ){
				if( c == '0')
					v = 0;
				else
					v = c-'1'+1;
			}else
				v = c - 'A'+10;
			
			
			getIndicesForThrow( cnt, j, ch, l, &ij, &ih, &ie);
			getIndicesForThrow( cnt + v, j, ch, l, &dj, &dh, &de);
			
			cc = cd->getCausalCell( cd->causalCells, ij, ih, ie, j, ch, l);
			cc->length = de-ie-2;
			cc->destJuggler = dj;
			cc->destHand =dh;
			
			cnt++;
		}
		glutPostRedisplay();

	}
bail:
		// ----------------------
		// clean up
		// ----------------------
		free( buf );
	chdir( d );	
}


void getIndicesForThrow( int cnt, int j, int h, int l, int *ij, int *ih, int *ie){
	
	*ie = cnt / j;
	*ij = cnt % j;
	*ih = *ie%h?leftHand:rightHand;
	
}


char *prepareNHandedBuffer( char *in, int h)
{
	size_t l;
	int j;
	int i;
	char *buf, *p;
	
	
	// ----------------------
	// calc parameters
	// ----------------------
	l = strlen( in );
	if( l/2 != l/2.)
		l *= 2;
	j = h/2.+0.5;
	buf = (char *)malloc( 2 * l * j * 2);
	
	
	// ----------------------
	// copy to buffer
	// ----------------------
	sprintf(buf, "%s", in);
	for( i = 1; i < j*2; i++)
		strcat( buf, in);
	
	// ----------------------
	// syntax check
	// anything but digits 
	// and letters is a fault
	// transform to upper dase 
	// ----------------------
	p = buf;
	while( *p){
		if( !isalnum( *p ) ){
			free( buf );
			buf = NULL;
			break;
		}
		*p = toupper(*p);
		p++;
	}
	
	return buf;
	
}

char *makeSpacefreeCopyOfString( char *in)
{
	char *r, *p;
	size_t t;
	
	t = strlen(in)+1;
	r = p = (char *)malloc( t);
	sprintf(p, "%s",  in);
	
	while( *p){
		if( isspace( *p ) || *p == crCode || *p ==  newlineCode || *p ==  3  )
			sprintf(p, "%s", p+1);
		else
			p++;
	}
	*p = '\0';
	return r;
}
