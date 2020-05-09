
/************************************************************************/
/*   J version 2.3               by Jack Boyce        12/91             */
/*                                  jboyce@tybalt.caltech.edu           */
/*                                                                      */
/*   This program finds all juggling site swap patterns for a given     */
/*   number of balls, maximum j2_throw value, and pattern length.          */
/*   A flow graph approach is used in order to speed up computation.    */
/*                                                                      */
/*   It is a complete rewrite of an earlier program written in 11/90    */
/*   which handled only non-multiplexed asynchronous solo site swaps.   */
/*   This version can generate multiplexed and nonmultiplexed tricks    */
/*   for an arbitrary number of people, number of hands, and throwing   */
/*   rhythm.  The built-in modes are asynchronous and synchronous solo  */
/*   juggling, and two person asynchronous passing.  Other setups can   */
/*   be read in as files.                                               */
/*                                                                      */
/*   See the documentation file for an explanation of site swap         */
/*   notation, program use, and how to create your own throwing rhythm  */
/*   file.                                                              */
/*                                                                      */
/*   Include flag modified and the -simple flag added on 2/92           */
/*   Extra check (for speed) added to gen_loops() on 01/19/98           */
/*   Bug fix in find_start_end() on 02/18/99                            */
/************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*************************  DEFINITIONS ********************************/

#define  ASYNCH_SOLO      0       /* different types of modes */
#define  SYNCH_SOLO       1
#define  ASYNCH_PASSING   2
#define  CUSTOM           3 

#define  EMPTY            0       /* types of multiplexing filter slots */
#define  THROW            1
#define  LOWER_BOUND      2

#define  BUFFER_SIZE      160     /* # of chars. in file input buffer */
#define  CHARS_PER_THROW   20     /* max. # of chars. printed per j2_throw */

/* added by ...ww */
    /* J2_STANDALONE is defined in the makefile
       typing make will build a standalone app
       call j2() from your code to use j2 as a subroutine ( see below )
    */   
	#ifdef J2_STANDALONE
		#ifdef TARGET_OS_MAC
		#include <SIOUX.h>
		#include <console.h>
		#endif
    #endif

#ifndef true
#define false (0)
#endif
	#include <time.h>

	time_t start_time, now_time;
	#define DefTimeLimit 	5
	int time_limit = DefTimeLimit;


	/************************************************************************
		made the program a subroutine, that is:
		memory is freed at end of main routine call
		therefore all memory calls were encapsulated
		
		globals are initialized with default values in a new subroutine

		all memory is requested by my routines, 
		all memory fragments are listed in an extra list
		at the end of the function call all requested memory is released
		
		added routines are at the end of this file
		
		output goes to a file, which is
			stdout for standalone version
			j2.out for subroutine version
		
		...ww
		westwolf@netcologne.de
		07.10.2001
	************************************************************************/	
	#include "j2.h"

	int _j2( int argc, char **argv);

	#define malloc	wwMalloc

	void *wwMalloc( size_t size);
	void freeWwMem( void );

	typedef struct wwMem{	struct wwMem *nextWwMem;
							void *memPointer;
						}_wwMem;
						
	_wwMem *firstWwMem = 0;
	FILE *fp;
/* end of ...ww additions */

						

// globals
int ***pattern_rhythm, ***pattern_state, **pattern_throwcount;
int ***pattern_holes;
struct j2_throw ***pattern_throw;
struct filter ***pattern_filter;
int hands, max_occupancy = 0;
int **rhythm_repunit, rhythm_period;
int *holdthrow, *person_number, *scratch1, *scratch2, leader_person = 1;
int **ground_state;
int n, l, ht, *xarray, *xparray, *iarray, numflag = 0, groundflag = 0;
int fullflag = 1, mp_filter = 1, delaytime = 0;
int sequence_flag = 1;
int mode = ASYNCH_SOLO;               /* default mode */
int people, slot_size;            /* number of people in pattern */
char *starting_seq, *pattern_buffer, *ending_seq;
const int solo_rhythm_repunit[1][1] =			{ { 1 } };
const int synch_rhythm_repunit[2][2] =     		{ { 1, 0 },
                                            	{ 1, 0 } };
const int asynch_passing_rhythm_repunit[2][1] =	{ { 1 },
                                            	  { 1 } };

/* added by ...ww */
	int rotflag = 0;
	int lameflag =  0;


	void initGlobals( void );
/* end of additions by ...ww */



struct j2_throw {                    /* records j2_throw information */
   int to;                            /* destination hand # */
   int value;                         /* total time ticks aloft */
};

struct filter {                   /* multiplexing filter slot entry */
   int type;                          /* type of entry */
   int from;                          /* source hand # */
   int value;                         /* total time ticks aloft */
};



int		**alloc_array(int height, int width);
void 	initialize(void);
int		custom_initialize(char *custom_file);
int 	valid_throw(int pos);
int 	valid_pattern(void);

int 	compare_rotations(int pos1, int pos2);
int 	compare_loops(int pos1, int pos2);
int 	compare_throws(int pos1, int pos2);


char 	*print_number(char *pos, int value);
char 	*print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm);
char 	*default_custom_print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm);
void 	print_pattern(void);
int 	compare_states(int **state1, int **state2);
int 	mp_addthrow(struct filter *dest_slot, int slot_hand, int type,
			int value, int from);
unsigned long gen_loops(int pos, int throws_made, int min_throw,
			int min_hand, unsigned long num);
void 	find_start_end(void);
unsigned long gen_patterns(int balls_placed, int min_value, int min_to,
			unsigned long num);
void 	find_ground(void);
int 	main(int argc, char **argv);
void 	die(void);
int 	custom_valid_throw(int pos);
int 	custom_valid_pattern(void);
char 	*custom_print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm);


int **alloc_array(int height, int width)
{
   int i, **ptr;

   if ((ptr = (int **)malloc(height * sizeof(int *))) == 0)
      die();
   for (i = 0; i < height; i++)
      if ((ptr[i] = (int *)malloc(width * sizeof(int))) == 0)
         die();

   return (ptr);
}


/* The following routine initializes stuff for the built-in rhythms. */
/* This involves allocating and initializing arrays.                 */

void initialize(void)
{
   int i, j;

   switch (mode) {
      case ASYNCH_SOLO:
         rhythm_repunit = alloc_array(1, 1);
	 rhythm_repunit[0][0] = solo_rhythm_repunit[0][0];
	 hands = 1;
	 rhythm_period = 1;
         people = 1;
         if ((holdthrow = (int *)malloc(sizeof(int))) == 0)
            die();
         holdthrow[0] = 2;
         if ((person_number = (int *)malloc(sizeof(int))) == 0)
            die();
         person_number[0] = 1;
	 break;
      case SYNCH_SOLO:
         rhythm_repunit = alloc_array(2, 2);
	 for (i = 0; i < 2; i++)
	    for (j = 0; j < 2; j++)
               rhythm_repunit[i][j] = synch_rhythm_repunit[i][j];
	 hands = 2;
	 rhythm_period = 2;
     people = 1;
     if ((holdthrow = (int *)malloc(2 * sizeof(int))) == 0)
        die();
         holdthrow[0] = holdthrow[1] = 2;
         if ((person_number = (int *)malloc(2 * sizeof(int))) == 0)
            die();
         person_number[0] = person_number[1] = 1;
	 break;
      case ASYNCH_PASSING:
	 rhythm_repunit = alloc_array(2, 1);
	 for (i = 0; i < 2; i++)
	    rhythm_repunit[i][0] = asynch_passing_rhythm_repunit[i][0];
	 hands = 2;
	 rhythm_period = 1;
         people = 2;
         if ((holdthrow = (int *)malloc(2 * sizeof(int))) == 0)
            die();
         holdthrow[0] = holdthrow[1] = 2;
         if ((person_number = (int *)malloc(2 * sizeof(int))) == 0)
            die();
         person_number[0] = 1;
         person_number[1] = 2;
	 break;
   }
}


/* This routine reads a custom rhythm file and parses it to get the */
/* relevant information.  If there is an error it prints a message  */
/* and then exits.                                                  */

int custom_initialize(char *custom_file)   /* name of file */
{
   int i, j, k, left_delim, right_delim;
   int last_period, last_person, person, hold, second_pass;
   char ch, *file_buffer;
   FILE *fp;

   if ((fp = fopen(custom_file, "r")) == NULL) {
      fprintf( fp,"File error: cannot open '%s'\n", custom_file);
      return -1;//exit(0);
   }
   if ((file_buffer = (char *)malloc(BUFFER_SIZE * sizeof(char))) == 0)
      die();

   for (second_pass = 0; second_pass < 2; second_pass++) {
      hands = j = 0;
      people = last_person = 1;

      do {
         ch = (char)(i = fgetc(fp));

         if ((ch == (char)10) || (i == EOF)) {
            file_buffer[j] = (char)0;

            for (j = 0, k = 0; (ch = file_buffer[j]) != 0 && (ch != ';'); j++)
                if (ch == '|') {
                   if (++k == 1)
                      left_delim = j;
                   else if (k == 2)
                      right_delim = j;
                }
            if (ch == ';')
               file_buffer[j] = (char)0;        /* terminate at comment */

            if (k) {
               if (k != 2) {
                 fprintf( fp,"File error: need two rhythm delimiters per hand\n");
                  return -2;//exit(0);
               }
                      /* At this point the line checks out.  See if */
                      /* period is what we got last time.           */
               if (hands && ((right_delim-left_delim-1) != last_period)) {
                  fprintf( fp,"File error: rhythm period not constant\n");
                  return -3;//exit(0);
               }
               last_period = right_delim - left_delim - 1;

                      /* Now parse the line we've read in */

               file_buffer[left_delim] = (char)0;
               person = atoi(file_buffer);

               if (hands) {
                  if (person == (last_person + 1)) {
                     people++;
                     last_person = person;
                  } else if (person != last_person) {
                     fprintf( fp,"File error: person numbers goofed up\n");
                     return -4;//exit(0);
                  }
               } else if (person != 1) {
                  fprintf( fp,"File error: must start with person number 1\n");
                  return -5;//exit(0);
               }

                      /* Now put stuff in the allocated arrays */

               if (second_pass) {
                  person_number[hands] = person;
                  hold = atoi(file_buffer + right_delim + 1);
                  holdthrow[hands] = (hold ? hold : 2);

                          /* Fill the rhythm matrix */
                  for (j = 0; j < rhythm_period; j++) {
                     ch = file_buffer[j + left_delim + 1];
                     if (((ch < '0') || (ch > '9')) && (ch != ' ')) {
                        fprintf( fp,"File error: bad character in rhythm\n");
                        return -6;//exit(0);
                     }
                     if (ch == ' ')
                        ch = '0';
                     rhythm_repunit[hands][j] = (int)(ch - '0');
                  }
               }

               hands++;   /* got valid line, increment counter */
            }
            j = 0;    /* reset buffer pointer for next read */
         } else {
            file_buffer[j] = ch;
            if (++j >= BUFFER_SIZE) {
               fprintf( fp,"File error: input buffer overflow\n");
               return -7;//exit(0);
            }
         }
      } while (i != EOF);

      if (!hands) {
         fprintf( fp,"File error: must have at least one hand\n");
         return -8;//exit(0);
      }

      if (!second_pass) {        /* allocate space after first pass */
         rhythm_period = last_period;
         rhythm_repunit = alloc_array(hands, rhythm_period);
         if ((holdthrow = (int *)malloc(hands * sizeof(int))) == 0)
            return -9;//die();
         if ((person_number = (int *)malloc(hands * sizeof(int))) == 0)
            return -10;//die();
         rewind(fp);          /* go back to start of file */
      }
                  
   }

   (void)fclose(fp);        /* close file and free memory */
   free(file_buffer);
   
   return 0;
}


/*  valid_throw -- checks if a given j2_throw is valid.  Check for        */
/*                 excluded throws and a passing communication delay,  */
/*                 as well a custom filter (if in CUSTOM mode).        */

int valid_throw(int pos)            /*  1 = valid j2_throw, 0 = invalid  */ 
{
   int i, j, k, balls_left, balls_thrown;

   for (i = 0; i < hands; i++) {            /* check for excluded throws */
      if (pattern_rhythm[pos][i][0]) {      /* can we make a j2_throw here? */
         for (j = 0; (j < max_occupancy) &&
	                    (k = pattern_throw[pos][i][j].value) != 0; j++) {
            if ((people > 1) && (person_number[i] !=
                            person_number[pattern_throw[pos][i][j].to])) {
// printf("checking for pass %d\n", k);
               if (xparray[k])
                {
// printf("excluded\n");
                  return (0);
                }
            } else if (xarray[k])
	       return (0);
         }
         if (!j && xarray[0])
            return (0);
      }
   }
          /*  Now check if we are allowing for a sufficient  */
          /*  communication delay, if we are passing.        */

   if ((people > 1) && (pos < delaytime)) {      /* need to check? */
            /*  First count the number of balls being thrown,       */
            /*  assuming no multiplexing.  Also check if leader is  */
            /*  forcing others to multiplex or make no j2_throw.       */
      for (balls_thrown = 0, i = 0; i < hands; i++)
         if (pattern_rhythm[pos][i][0]) {
            balls_thrown++;
            if ((pattern_state[pos][i][0] != 1) && 
                           (person_number[i] != leader_person))
               return(0);
         }

      balls_left = n;
      for (i = 0; (i < ht) && balls_left; i++)
         for (j = 0; (j < hands) && balls_left; j++)
            if (pattern_rhythm[pos + 1][j][i])
               if (--balls_left < balls_thrown) {
                  scratch1[balls_left] = j;       /* dest hand # */
                  scratch2[balls_left] = i + 1;   /* dest value */
               }

      if (balls_left)
         return (0);       /* this shouldn't happen, but die anyway */

      for (i = 0; i < hands; i++)
         if (pattern_state[pos][i][0] && 
                            (person_number[i] != leader_person)) {
            for (j = 0, k = 1; (j < balls_thrown) && k; j++)
               if ((scratch1[j] == pattern_throw[pos][i][0].to) &&
                       (scratch2[j] == pattern_throw[pos][i][0].value))
                  scratch2[j] = k = 0;    /* can't j2_throw to spot again */
            if (k)
               return (0);        /* wasn't throwing to empty position */
         }
   }

   if (mode == CUSTOM)
      return (custom_valid_throw(pos));

           /****************************************************/
           /*  If you want to add extra j2_throw filters for the  */
           /*  built-in modes, do it here.                     */
           /****************************************************/

   return (1);
}


int valid_pattern(void)      /*  1 = valid pattern, 0 = invalid  */
{
   int i, j, k, m, q, flag;


   for (i = 0; i <= ht; i++) {        /* check for included throws */
	  if (iarray[i]) {
		 flag = 1;
		 for (j = 0; (j < l) && flag; j++) {
			for (k = 0; (k < hands) && flag; k++) {
			   if (pattern_rhythm[j][k][0]) {
			      m = 0;
			      do {
				     q = pattern_throw[j][k][m].value;
				     if ((q == i) && (k == pattern_throw[j][k][m].to))
					    flag = 0;
                     m++;
                  } while ((m < max_occupancy) && q && flag);
               }
            }
         }
		 if (flag)
			return (0);
      }
   }

   if (mode == CUSTOM)
      return (custom_valid_pattern());
      
/* added from jugglingLab by ...ww */
      /* Check for '11' sequence: */
     if ((mode == ASYNCH_SOLO) && lameflag && (max_occupancy == 1)) {
      for (i = 0; i < (l - 1); i++)
         if ((pattern_throw[i][0][0].value == 1) &&
		          (pattern_throw[i+1][0][0].value == 1))
	    return (0);
   }


/* added from jugglingLab by ...ww */
 	// Added 12/4/2002
	// Is the pattern composite?  If so, ensure we only print one rotation of it
	if ((fullflag == 0) && (rotflag == 0)) {
		for (i = 1; i < l; i++) {
			if ((i % rhythm_period) == 0) {		// can we compare states?
				if (compare_states(pattern_state[0], pattern_state[i]) == 0) {
					if (compare_rotations(0, i) < 0)
					{
						return false;
					}	
				}
			}
		}
	}

          /********************************************************/
          /*  If you want to add an extra pattern filter for one  */
          /*  of the built-in modes, do it here.                  */
          /********************************************************/

   return (1);
}



/* added from jugglingLab by ...ww */
int compare_rotations(int pos1, int pos2) 
{
	int i;
	
		
	for (i = 0; i < l; ) 
	{
		int res = compare_loops((pos1+i)%l, (pos2+i)%l);
		if (res > 0)
			return 1;
		else if (res < 0)
			return -1;
		
		i++;
		for (; i < l; i++) {
			if (compare_states(pattern_state[pos1], pattern_state[(pos1+i)%l]) == 0)
				break;
		}
	}
	return 0;
}
	
	// Added 12/4/2002

/* added from jugglingLab by ...ww */
int compare_loops(int pos1, int pos2) 
{
	int **startstate = pattern_state[pos1];
	int result = 0;
	int i = 0;
	
	// Rule 1:  The longer loop is always greater
	// Rule 2:  For loops of equal length, use throw-by-throw comparison
    // Rule 3:  Loops are equal only if the respective throws are identical
	
	while (true) {
		i++;
		
		if ((i % rhythm_period) == 0) {
			int cs1 = compare_states(pattern_state[pos1+1], startstate);
			int cs2 = compare_states(pattern_state[pos2+1], startstate);
			
			if (cs1 == 0) {
				if (cs2 == 0)
					return result;
				return -1;
			}
			if (cs2 == 0)
				return 1;
		}
		
		if (result == 0)
            result = compare_throws(pos1, pos2);

		pos1++;
		pos2++;
	}	
}
        
/* added from jugglingLab by ...ww */
// Added 12/7/2002
// This method assumes the throws are comparable, i.e., that pos1 is congruent
// to pos2 mod rhythm_period
int compare_throws(int pos1, int pos2) 
{
        int **rhythm = pattern_rhythm[pos1];	// same as pos2 since throws comparable

 /*
        int **value1 = pattern_throw[pos1];
        int **to1 = pattern_throw[pos1].to;
        int **value2 = pattern_throw[pos2].value;
        int **to2 = pattern_throw[pos2].to;
*/

		struct j2_throw **p1 = pattern_throw[pos1], **p2 = pattern_throw[pos2]; 
        int i, j;
        
        
        // Rule 1:  Throws are equal only if identical

        for ( i = 0; i < hands; i++) {
            for (j = 0; j < rhythm[i][0]; j++) {
                if (p1[i][j].value > p2[i][j].value)
                    return 1;
                else if (p1[i][j].value < p2[i][j].value)
                    return -1;
                else if (p1[i][j].to > p2[i][j].to)
                    return 1;
                else if (p1[i][j].to < p2[i][j].to)
                    return -1;
            }
        }
        
        return 0;
}



/*  compare_states -- equality (0), lesser (-1), or greater (1)     */

int compare_states(int **state1, int **state2)
{
   int i, j, mo1 = 0, mo2 = 0;

   for (i = 0; i < hands; i++)
      for (j = 0; j < ht; j++) {
         if (state1[i][j] > mo1)
	    mo1 = state1[i][j];
         if (state2[i][j] > mo2)
	    mo2 = state2[i][j];
      }

   if (mo1 > mo2)
      return (1);
   if (mo1 < mo2)
      return (-1);

   for (j = (ht - 1); j >= 0; j--)
      for (i = (hands - 1); i >= 0; i--) {
         mo1 = state1[i][j];
	 mo2 = state2[i][j];
	 if (mo1 > mo2)
	    return (1);
         if (mo1 < mo2)
	    return (-1);
      }

   return (0);
}

char *print_number(char *pos, int value)     /* prints number as single character */
{
   if (value < 10)
      *pos = (char)value + '0';
   else
      *pos = (char)(value - 10) + 'A';

   return (++pos);
}


char *print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm)  /* prints single j2_throw */
{
   int i, j;

   for (i = 0, j = 1; (i < hands) && j; i++)
      if (rhythm[i][0])           /* supposed to make a j2_throw? */
         j = 0;
   if (j)
      return (pos);      /* can't make a j2_throw, skip out */

   switch (mode) {
      case ASYNCH_SOLO:
    	 if ((max_occupancy > 1) && j2_throw[0][1].value) {
            *pos++ = '[';
            for (i = 0; (i < max_occupancy) &&
	       		             (j = j2_throw[0][i].value)!=0; i++)
	           pos = print_number(pos, j);
	        *pos++ = ']';
	     } else
	        pos = print_number(pos, j2_throw[0][0].value);
         break;
      case SYNCH_SOLO:
         *pos++ = '(';
	 if ((max_occupancy > 1) && j2_throw[0][1].value) {
	    *pos++ = '[';
	    for (i = 0; (i<max_occupancy) && (j=j2_throw[0][i].value)!=0; i++) {
	       pos = print_number(pos, j);
	       if (j2_throw[0][i].to)
	          *pos++ = 'x';
	    }
	    *pos++ = ']';
	 } else {
	    pos = print_number(pos, j2_throw[0][0].value);
	    if (j2_throw[0][0].to)
	       *pos++ = 'x';
	 }
	 *pos++ = ',';
	 if ((max_occupancy > 1) && j2_throw[1][1].value) {
	    *pos++ = '[';
	    for (i = 0; (i<max_occupancy) && (j=j2_throw[1][i].value)!=0; i++) {
	       pos = print_number(pos, j);
	       if (!j2_throw[1][i].to)
	          *pos++ = 'x';
	    }
	    *pos++ = ']';
	 } else {
	    pos = print_number(pos, j2_throw[1][0].value);
	    if (!j2_throw[1][0].to)
	       *pos++ = 'x';
	 }
	 *pos++ = ')';
	 break;
      case ASYNCH_PASSING:
	 *pos++ = '<';
         if ((max_occupancy > 1) && j2_throw[0][1].value) {
	    *pos++ = '[';
	    for (i = 0; (i<max_occupancy) && (j=j2_throw[0][i].value)!=0; i++) {
	       pos = print_number(pos, j);
	       if (j2_throw[0][i].to)
	          *pos++ = 'p';
	    }
    	    *pos++ = ']';
	 } else {
	    pos = print_number(pos, j2_throw[0][0].value);
	    if (j2_throw[0][0].to)
	       *pos++ = 'p';
	 }
         *pos++ = '|';
	 if ((max_occupancy > 1) && j2_throw[1][1].value) {
	    *pos++ = '[';
	    for (i = 0; (i<max_occupancy) && (j=j2_throw[1][i].value)!=0; i++) {
	       pos = print_number(pos, j);
	       if (!j2_throw[1][i].to)
	          *pos++ = 'p';
	    }
	    *pos++ = ']';
	 } else {
	    pos = print_number(pos, j2_throw[1][0].value);
	    if (!j2_throw[1][0].to)
	       *pos++ = 'p';
	 }
	 *pos++ = '>';
	 break;
      case CUSTOM:
         pos = custom_print_throw(pos, j2_throw, rhythm);
         break;
   }

   return (pos);
}

/*  The following is the default routine that is used to print a  */
/*  j2_throw when the program is in CUSTOM mode.                     */

char *default_custom_print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm)
{
   int i, j, k, m, q, lo_hand, hi_hand, multiplex, parens;
   char ch;

   if (people > 1)
      *pos++ = '<';

   for (i = 1; i <= people; i++) {

           /* first find the hand numbers corresponding to person */
      for (lo_hand = 0; person_number[lo_hand] != i; lo_hand++)
         ;
      for (hi_hand = lo_hand; (hi_hand < hands) && 
                         (person_number[hi_hand] == i); hi_hand++)
         ;

           /* check rhythm to see if person is throwing this time */
      for (j = lo_hand, k = 0; j < hi_hand; j++)
         if (rhythm[j][0])
            k++;

      if (k) {        /* person should j2_throw */
         if (k > 1) {     /* more than one hand throwing? */
            *pos++ = '(';
            parens = 1;
         } else
            parens = 0;

         for (j = lo_hand; j < hi_hand; j++) {
            if (rhythm[j][0]) {      /* this hand supposed to j2_throw? */
               if ((max_occupancy > 1) && j2_throw[j][1].value) {
                  *pos++ = '[';        /* multiplexing? */
                  multiplex = 1;
               } else
                  multiplex = 0;

                  /* Now loop thru the throws coming out of this hand */

               for (k = 0; (k < max_occupancy) && 
                                         (m = j2_throw[j][k].value)!=0; k++) {
                  pos = print_number(pos, m);    /* print j2_throw value */

                  if (hands > 1) {    /* ambiguity about destination? */
                     if ((m = person_number[j2_throw[j][k].to]) != i) {
                        *pos++ = ':';
						pos = print_number(pos, m);
                     }
										/* person number */

                     for (q = j2_throw[j][k].to - 1, ch = 'a'; (q >= 0) &&
                              (person_number[q] == m); q--, ch++)
                        ;        /* find hand # of destination person */

                        /* destination person has 1 hand, don't print */
                     if ((ch != 'a') || ((q < (hands - 2)) &&
                                           (person_number[q + 2] == m)))
                        *pos++ = ch;             /* print it */
                  }

                  if (multiplex && (people > 1) &&
                        (k != (max_occupancy - 1)) && j2_throw[j][k + 1].value)
                     *pos++ = '/';
								   /* another multiplexed j2_throw? */
               }
               if (k == 0)
                  *pos++ = '0';

               if (multiplex)
                  *pos++ = ']';
            }

            if ((j < (hi_hand - 1)) && parens)  /* put comma between hands */
              *pos++ = ',';
         }
         if (parens)
            *pos++ = ')';
      }

      if (i < people)           /* another person throwing next? */
         *pos++ = '|';
   }

   if (people > 1)
      *pos++ = '>';

   return (pos);
}


void print_pattern(void)
{
   int i, excited = 0;
   char *pos;

   if (groundflag != 1) {
      if (sequence_flag) {
         if (mode == ASYNCH_SOLO)
            for (i = n - strlen(starting_seq); i > 0; i--)
               fprintf( fp," ");
         fprintf( fp,"%s  ", starting_seq);
      } else {
         excited = compare_states(ground_state, pattern_state[0]);
         if (excited)
            fprintf( fp,"* ");
         else
            fprintf( fp,"  ");
      }
   }
   
   pos = pattern_buffer;
   for (i = 0; i < l; i++)
      pos = print_throw(pos, pattern_throw[i], pattern_rhythm[i]);
   *pos = (char)0;         /* terminate the string */
   fprintf( fp,"%s", pattern_buffer);

   if (groundflag != 1) {
   	if (sequence_flag)
      	fprintf( fp,"  %s\n", ending_seq);
   	else {
      	if (excited)
         	fprintf( fp," *\n");
      	else
         	fprintf( fp,"\n");
   	}
   } else
      fprintf( fp,"\n");
   
}




   /*  The next function is part of the implementation of the   */
   /*  multiplexing filter.  It adds a j2_throw to a filter slot,  */
   /*  returning 1 if there is a collision, 0 otherwise.        */

int mp_addthrow(struct filter *dest_slot, int slot_hand, int type,
		int value, int from)
{
   switch (type) {
      case EMPTY:
         return (0);
         break;
      case LOWER_BOUND:
         if (dest_slot->type == EMPTY) {
            dest_slot->type = LOWER_BOUND;
            dest_slot->value = value;
            dest_slot->from = from;
            return (0);
         }
         return (0);
         break;
      case THROW:
         if ((from == slot_hand) && (value == holdthrow[slot_hand]))
            return (0);           /* j2_throw is a hold, so ignore it */

         switch (dest_slot->type) {
            case EMPTY:
               dest_slot->type = THROW;
               dest_slot->value = value;
               dest_slot->from = from;
               return (0);
               break;
            case LOWER_BOUND:
               if ((dest_slot->value <= value) || (dest_slot->value <=
                              holdthrow[slot_hand])) {
                  dest_slot->type = THROW;
                  dest_slot->value = value;
                  dest_slot->from = from;
                  return (0);
               }
               break;       /* this kills recursion */
            case THROW:
               if (( dest_slot->from == from) &&
                           (dest_slot->value == value))
                  return (0);        /* throws from same place (clump) */
               break;       /* kills recursion */
         }
         break;
   }

   return (1);
}


/*  gen_loops -- This recursively generates loops, given a particular   */
/*               starting state.                                        */

unsigned long gen_loops(int pos, int throws_made, int min_throw,
		int min_hand, unsigned long num)
// int pos;              /* slot number in pattern that we're constructing */
// int throws_made;      /* number of throws made out of current slot      */
// int min_throw;        /* lowest we can j2_throw this time                  */
// int min_hand;         /* lowest hand we can j2_throw to this time          */
// unsigned long num;    /* number of valid patterns counted               */
{
   int i, j, k, m, o;
   static float dt = 0;
   static float printWarning = true;


/* added by ...ww */
time( &now_time);
dt = difftime( now_time, start_time);
if( dt > time_limit )
{
	if( printWarning )
		fprintf(fp, "\ntime limit exeeded (%d sec)\n", time_limit);
		
	printWarning = false;
	return 0;
}

   if (pos == l) {
      if ((compare_states(pattern_state[0], pattern_state[l]) == 0) &&
	                         valid_pattern()) {
         if (numflag != 2)
            print_pattern();
	 num++;
      }
      return (num);
   }

   if (!throws_made) 
      for (i = 0; i < hands; i++) {
         pattern_throwcount[pos][i] = pattern_state[pos][i][0];
	 for (j = 0; j < ht; j++) {
	    pattern_holes[pos][i][j] = pattern_rhythm[pos + 1][i][j];
   	    if (j != (ht - 1))
	       pattern_holes[pos][i][j] -= pattern_state[pos][i][j + 1];
	 }
	 for (j = 0; j < max_occupancy; j++) {
            pattern_throw[pos][i][j].to = i;      /* clear j2_throw matrix */
	    pattern_throw[pos][i][j].value = 0;
	 }
      }

   for (i = 0; (i < hands) && (pattern_throwcount[pos][i] == 0); i++)
      ;

   if (i == hands) {  /* done with current slot, move to next */ 
      if (!valid_throw(pos))              /* is the j2_throw ok? */
         return (num);

           /* first calculate the next state in ptrn, given last j2_throw */
      for (j = 0; j < hands; j++)      /* shift state to the left */
         for (k = 0; k < ht; k++)
	    pattern_state[pos + 1][j][k] = 
		           ( (k == (ht-1)) ? 0 : pattern_state[pos][j][k+1] );

                  /* add on the last j2_throw */
      for (j = 0; j < hands; j++)
         for (k = 0; (k < max_occupancy) && 
	                      (m = pattern_throw[pos][j][k].value)!=0; k++) 
     	    pattern_state[pos + 1][pattern_throw[pos][j][k].to][m - 1]++;
     	    
			/* Check if this is a valid state for a period-L pattern */
			/* This check added 01/19/98.                            */
		for (j = 0; j < hands; j++) {
			for (k = 0; k < ht; k++) {
				m = pattern_state[pos + 1][j][k];
				o = k;
				while ((o += l) < ht)
					if (pattern_state[pos + 1][j][o] > m)
						return num;     /* die (invalid state for this L) */
			}
		}
			/* end of new section */
     if (((pos + 1) % rhythm_period) == 0) {   
                 /* can we compare states? (rhythms must be same) */
         j = compare_states(pattern_state[0], pattern_state[pos + 1]);
         if (fullflag && (pos != (l - 1)) && (j == 0))  /* intersection */
	    return (num);
	    
		if ((rotflag == 0) && (j == 1)) /* prevents rotations from being printed */
				return num;
      }

	  if (fullflag == 2) {            /* list only simple loops? */
		 for (j = 1; j <= pos; j++)
			if (((pos + 1 - j) % rhythm_period) == 0) { 
			   if (compare_states(pattern_state[j],
						 pattern_state[pos + 1]) == 0) 
                  return (num); 
            } 
      } 
 
          /*  Now do the multiplexing filter.  This ensures that,  */
          /*  other than holds, objects from only one source are   */
          /*  landing in any given hand (for example, a clump of   */
          /*  3's).  The implementation is a little complicated,   */
          /*  since I want to cut off the recursion as quickly as  */
          /*  possible to get speed on big searches.  This         */
          /*  precludes simply generating all patterns and then    */
          /*  throwing out the unwanted ones.                      */

      if (mp_filter) {
         for (j = 0; j < hands; j++) {    /* shift filter frame to left */
	    for (k = 0; k < (slot_size - 1); k++) {
	       pattern_filter[pos + 1][j][k].type = 
		              pattern_filter[pos][j][k + 1].type;
	       pattern_filter[pos + 1][j][k].from =
		              pattern_filter[pos][j][k + 1].from;
	       pattern_filter[pos + 1][j][k].value =
		              pattern_filter[pos][j][k + 1].value;
	    }
	    pattern_filter[pos + 1][j][slot_size - 1].type = EMPTY;
                                  /* empty slots shift in */

            if (mp_addthrow( &(pattern_filter[pos + 1][j][l - 1]),
                      j, pattern_filter[pos][j][0].type,
                      pattern_filter[pos][j][0].value,
                      pattern_filter[pos][j][0].from))
               return (num);
	 }

         for (j = 0; j < hands; j++)           /* add on last j2_throw */
            for (k = 0; (k < max_occupancy) &&
       			       (m = pattern_throw[pos][j][k].value)!=0; k++)
               if (mp_addthrow( &(pattern_filter[pos + 1]
                      [pattern_throw[pos][j][k].to][m - 1]), 
                       pattern_throw[pos][j][k].to, THROW, m, j))
                  return (num);        /* problem, so end recursion */
      }

      num = gen_loops(pos + 1, 0, 1, 0, num);       /* go to next slot */ 
   } else {
      m = --pattern_throwcount[pos][i];     /* record j2_throw */
      k = min_hand;

      for (j = min_throw; j <= ht; j++) {
         for ( ; k < hands; k++) {
            if (pattern_holes[pos][k][j - 1]) {/*can we j2_throw to position?*/
   	       pattern_holes[pos][k][j - 1]--;
	       pattern_throw[pos][i][m].to = k;
	       pattern_throw[pos][i][m].value = j;
	       if (m)
	          num = gen_loops(pos, throws_made + 1, j, k, num);
	       else
	          num = gen_loops(pos, throws_made + 1, 1, 0, num);
	       pattern_holes[pos][k][j - 1]++;
	    }
	 }
	 k = 0;
      }
      pattern_throwcount[pos][i]++;
   }

   return (num);
}


/*  The next routine finds valid starting and ending sequences for      */
/*  excited state patterns.  Note that these sequences are not unique.  */

void find_start_end(void)
{
	int i, j, k, m, q, flag;
	char *pos;

	*starting_seq = (char)0;    /* first set to null strings (in case */
	*ending_seq = (char)0;      /* we have a ground state trick)      */
              
			  /* first find the starting sequence */
	i = slot_size;       /* j2_throw position to start at (work back to gnd) */
	for (j = 0; j < hands; j++)
		for (k = 0; k < ht; k++)
			pattern_state[i][j][k] = pattern_state[0][j][k];   /* copy state */

	while ((i % rhythm_period) || compare_states(pattern_state[i],
                                                  ground_state)) {
		m = ht;            /* pointers to current ball we're pulling down */
		q = hands - 1;

		for (j = 0; j < hands; j++) {
			for (k = 0; k < max_occupancy; k++) {     /* clear j2_throw matrix */
				pattern_throw[i - 1][j][k].value = 0;
				pattern_throw[i - 1][j][k].to = j;
			}

			pattern_state[i - 1][j][0] = 0;
			if (pattern_rhythm[i - 1][j][0]) {
				while (pattern_state[i][q][m - 1] == 0) {
					if (q-- == 0) {     /* go to next position to pull down */
						q = hands - 1;
						if (!(--m))
							goto skip1;
					}
				}
				pattern_throw[i - 1][j][0].value = m;
				pattern_throw[i - 1][j][0].to = q;
				pattern_state[i][q][m - 1]--;
				pattern_state[i - 1][j][0]++;
			}
		}

skip1:
		for (j = 0; j < hands; j++) {
			if (pattern_state[i][j][ht - 1]) {
				*starting_seq = '?';
				starting_seq[1] = (char)0;
				goto skip2;       /* skip to where ending seq. is found */
			}
			for (k = 1; k < ht; k++)
				pattern_state[i - 1][j][k] = pattern_state[i][j][k - 1];
		}

		i--;
		if ((i == 0) && compare_states(*pattern_state, ground_state)) {
			*starting_seq = '?';
			starting_seq[1] = (char)0;
			goto skip2;
		}
	}

   pos = starting_seq;            /* write starting seq. to buffer */
   for ( ; i < slot_size; i++)
      pos = print_throw(pos, pattern_throw[i], pattern_rhythm[i]);
   *pos = (char)0;      /* terminate string */

         /*  Now construct an ending sequence.  Unlike the starting  */
         /*  sequence above, this time work forward to ground state. */

skip2:
   i = 0;
   while ((i % rhythm_period) || compare_states(pattern_state[i],
                             ground_state)) {
      m = 1;
      q = 0;
      for (j = 0; j < hands; j++) {
         for (k = 0; k < max_occupancy; k++) {
            pattern_throw[i][j][k].value = 0;
            pattern_throw[i][j][k].to = j;
         }
         for (k = pattern_state[i][j][0] - 1; k >= 0; k--) {
            flag = 1;
            while (flag) {
               for ( ; (q < hands) && flag; q++) {
                  if (pattern_rhythm[i+1][q][m-1] && ((m >= ht) || 
                                  (pattern_state[i][q][m] == 0))) {
                     if (m > ht) {
                        *ending_seq = '?';
                        ending_seq[1] = (char)0;
                        return;          /* no place to put ball */
                     }
                     pattern_throw[i][j][k].value = m;
                     pattern_throw[i][j][k].to = q;
                     flag = 0;
                  }
               }
               if (q == hands) {
                  q = 0;
                  m++;
               }
            }
         }
      }
      for (j = 0; j < hands; j++) {       /* shift the state left */
         for (k = 0; k < (ht - 1); k++)
            pattern_state[i+1][j][k] = pattern_state[i][j][k+1];
         pattern_state[i+1][j][ht-1] = 0;
      }
      for (j = 0; j < hands; j++)         /* add on the last throws */
         for (k = 0; (k < max_occupancy) && 
                    (m = pattern_throw[i][j][k].value)!=0; k++)
            pattern_state[i+1][pattern_throw[i][j][k].to][m-1] = 1;
      if (++i > ht) {
         *ending_seq = '?';
         ending_seq[1] = (char)0;
         return;
      }
   }

   pos = ending_seq;            /* write ending seq. to buffer */
   for (j = 0; j < i; j++)
      pos = print_throw(pos, pattern_throw[j], pattern_rhythm[j]);
   *pos = (char)0;           /* terminate starting string */
}


/*  gen_patterns -- Recursively generates all possible starting        */
/*                  states, calling gen_loops above to find the loops  */
/*                  for each one.                                      */

unsigned long gen_patterns(int balls_placed, int min_value, int min_to,
		unsigned long num)
{
   int i, j, k, m, q;

   if ((balls_placed == n) || (groundflag == 1)) {
      if (groundflag == 1) {    /* find only ground state patterns? */
         for (i = 0; i < hands; i++)
            for (j = 0; j < ht; j++)
               pattern_state[0][i][j] = ground_state[i][j];
      } else if ((groundflag == 2) &&
                     !compare_states(pattern_state[0], ground_state))
         return(num);         /* don't find ground state patterns */

           /*  At this point our state is completed.  Check to see      */
           /*  if it's valid.  (Position X must be at least as large    */
           /*  as position X+L, where L = pattern length.)  Also set    */
           /*  up the initial multiplexing filter frame, if we need it. */

      for (i = 0; i < hands; i++) {
         for (j = 0; j < ht; j++) {

            k = pattern_state[0][i][j];
            if (mp_filter && !k)
               pattern_filter[0][i][j].type = EMPTY;
            else {
               if (mp_filter) {
                  pattern_filter[0][i][j].value = j + 1;
                  pattern_filter[0][i][j].from = i;
                  pattern_filter[0][i][j].type = LOWER_BOUND;
               }

               m = j;
               while ((m += l) < ht) {
                  if ((q = pattern_state[0][i][m]) > k)
                     return (num);     /* die (invalid state for this L) */
                  if (mp_filter && q) {
                     if ((q < k) && (j > holdthrow[i]))
                        return (num);  /* different throws into same hand */
                     pattern_filter[0][i][j].value = m + 1;  /* new bound */
                  }
               }
            }
         }
        
         if (mp_filter)
            for ( ; j < slot_size; j++)
               pattern_filter[0][i][j].type = EMPTY; /* clear rest of slot */
      }

	  if ((numflag != 2) && sequence_flag) 
         find_start_end();/* find starting and ending sequences for state */

      return (gen_loops(0, 0, 1, 0, num));   /* find patterns thru state */
   }

   if (!balls_placed) {        /* startup, clear state */
      for (i = 0; i < hands; i++)
         for (j = 0; j < ht; j++)
	    pattern_state[0][i][j] = 0;
   }

   j = min_to;       /* ensures each state is generated only once */
   for (i = min_value; i < ht; i++) {
      for ( ; j < hands; j++) {
         if (pattern_state[0][j][i] < pattern_rhythm[0][j][i]) {
	    pattern_state[0][j][i]++;
  	    num = gen_patterns(balls_placed + 1, i, j, num);   /* recursion */
	    pattern_state[0][j][i]--;
	 }
      }
      j = 0;
   }

   return (num);
}


/*  find_ground -- Find the ground state for our rhythm.  Just put  */
/*                 the balls in the lowest possible slots, with no  */
/*                 multiplexing.                                    */

// juggleLab check: passed
void find_ground(void)
{
	int i, j, balls_left;

	balls_left = n;

	for (i = 0; i < hands; i++)       /* clear ground state array */
		for (j = 0; j < ht; j++)
			ground_state[i][j] = 0;

	for (i = 0; (i < ht) && balls_left; i++) 
		for (j = 0; (j < hands) && balls_left; j++) 
			if (pattern_rhythm[0][j][i]) {      /* available slots */
				ground_state[j][i] = 1;
				balls_left--;
			}

	if (balls_left) {
		fprintf( fp,"Maximum j2_throw value is too small\n");
		return;//exit(0);
	}
}



/*  This is the entry point of the program.  It decodes the command  */
/*  line, allocates the necessary memory space, and then calls       */
/*  gen_patterns above to find the loops.                            */
#ifdef J2_STANDALONE
int main(int argc, char **argv)
{
		
#ifdef TARGET_OS_MAC
   argc = ccommand(&argv);
   SIOUXSettings.autocloseonquit = (char)0;
   SIOUXSettings.asktosaveonclose = (char)0;
#ifdef PROFILE
   if (!ProfilerInit(collectDetailed, bestTimeBase, 15, 5)) {
      ProfilerSetStatus(1);
#endif
#endif

	fp = stdout;
	_j2( argc, argv);

#ifdef TARGET_OS_MAC
#ifdef PROFILE
      ProfilerSetStatus(0);
      ProfilerDump("\pJ2.prof");
      ProfilerTerm();
   }
#endif
#endif


   exit(0);        /* frees memory */

   return 0;

}
#endif
	

int j2( int argc, char **argv)
{
	int ret;


	fp = fopen( J2_OUTPUT_FIlE, "w");
	if( fp == 0 )
	{	fp = stdout;
		printf("cant create file %s\n", J2_OUTPUT_FIlE);
	}	
	ret = _j2(  argc, argv);
	fclose( fp );
	
	return ret;
}


int _j2( int argc, char **argv)
{
	int i, j, k, multiplex = 1;
	unsigned long num;
	
	int c;
	
	time( &start_time);
	fprintf( fp, "j2 ");
	for( c = 1; c < argc; c++ )
		fprintf( fp,"%s ", argv[c]);
		
	fprintf( fp,"\n");	
	initGlobals();


   if (argc < 4) {
      fprintf( fp,
"USAGE: j2 <number of objects> <max. j2_throw> <pattern length> [-options]\n\n");
      fprintf( fp,
"Version 2.3 (02/18/99), by Jack Boyce (jboyce@physics.berkeley.edu)\n\n");
      fprintf( fp,
"This program finds juggling patterns in a generalized form of site swap\n");
      fprintf( fp,
"notation.  For a full description of this notation and the program's\n");
      fprintf( fp,
"operation, consult the accompanying documentation files.  All patterns\n");
      fprintf( fp,
"satisfying the given constraints are listed by the program.  Solo asynch-\n");
      fprintf( fp,"ronous juggling is the default mode.\n\n");
      fprintf( fp,"Command line options:\n");
      fprintf( fp,
" -s  solo synchronous mode     -m <number>  multiplexing with at most the\n");
      fprintf( fp,
" -p  2 person passing mode          given number of simultaneous throws\n");
      fprintf( fp,
" -c <file>  custom mode        -mf  turn off multiplexing filter\n");
      fprintf( fp,
" -n  show number of patterns   -d <number>  passing communication delay\n");
      fprintf( fp,
" -no print number only         -l <number>  passing leader person number\n");
      fprintf( fp,
" -g  ground state patterns     -x <j2_throw> ..  exclude listed self-throws\n");
      fprintf( fp,
" -ng excited state patterns    -xp <j2_throw> .. exclude listed passes\n");
      fprintf( fp,
" -f  full listing (decompos-   -i <j2_throw> ..  must include listed self-\n");
      fprintf( fp,
"     able patterns too)             throws\n");
      fprintf( fp,
" -se disable starting/ending\n");
	  fprintf( fp,
" -simple  list only non-decomposable patterns\n\n");
/* added by ...ww*/ 
	  fprintf( fp,
" -rot  show all rotated versions of patterns\n"); 
	  fprintf( fp,
" -lame  remove '11' sequence in asynch mode\n"); 
/* end of additions by ...ww*/ 


      goto bail;
   }

   n = atoi(argv[1]);                    /* get the number of objects */
   if (n < 1) {
      fprintf( fp,"Must have at least 1 object\n");
      goto bail;
   }
   ht = atoi(argv[2]);                    /* get the max. j2_throw j2_throw */
   l = atoi(argv[3]);
   if (l < 1) {
      fprintf( fp,"Pattern length must be at least 1\n");
      goto bail;
   }

   if ((xarray = (int *)malloc((ht + 1) * sizeof(int))) == 0)
      goto bail;//die();                 /* excluded self-throws */
   if ((xparray = (int *)malloc((ht + 1) * sizeof(int))) == 0)
      goto bail;//die();                 /* excluded passes */
   if ((iarray = (int *)malloc((ht + 1) * sizeof(int))) == 0)
	  goto bail;//die();
   for (i = 0; i <= ht; i++) {      /* initialize to default */
      xarray[i] = 0;
      xparray[i] = 0;
	  iarray[i] = 0;
   }

   for (i = 4; i < argc; i++) {
      if (!strcmp(argv[i], "-n"))
         numflag = 1;
      else if (!strcmp(argv[i], "-no"))
         numflag = 2;
      else if (!strcmp(argv[i], "-g"))
         groundflag = 1;
      else if (!strcmp(argv[i], "-ng"))
         groundflag = 2;
      else if (!strcmp(argv[i], "-f"))
         fullflag = 0; 
     else if (!strcmp(argv[i], "-simple"))
		 fullflag = 2; 
      else if (!strcmp(argv[i], "-se"))
         sequence_flag = 0;
      else if (!strcmp(argv[i], "-s"))
         mode = SYNCH_SOLO;
      else if (!strcmp(argv[i], "-p"))
         mode = ASYNCH_PASSING;
 /* added by ...ww*/
      else if (!strcmp(argv[i], "-rot"))
         rotflag = 1;        
 	  else if (!strcmp(argv[i], "-lame"))
         lameflag = 1; 
/* end of additions by ...ww */
      else if (!strcmp(argv[i], "-c"))
     {
          mode = CUSTOM;
         if (i != (argc - 1))
            if( custom_initialize(argv[++i]) < 0 )
            	goto bail;
         else {
            fprintf( fp,"No custom rhythm file given\n");
           	goto bail;
         }
      } else if (!strcmp(argv[i], "-mf"))
         mp_filter = 0;
      else if (!strcmp(argv[i], "-m")) { 
         if ((i < (argc - 1)) && (argv[i + 1][0] != '-')) {
	    multiplex = atoi(argv[i + 1]);
   	    i++;
	 }
      }
      else if (!strcmp(argv[i], "-d")) {
         if ((i < (argc - 1)) && (argv[i + 1][0] != '-')) {
            delaytime = atoi(argv[i + 1]);
            groundflag = 1;        /* find only ground state tricks */
            i++;
         }
      }
      else if (!strcmp(argv[i], "-l")) {
         if ((i < (argc - 1)) && (argv[i + 1][0] != '-')) {
            leader_person = atoi(argv[i + 1]);
            i++;
         }
      }
      else if (!strcmp(argv[i], "-xp")) {
         i++;
         while ((i < argc) && (argv[i][0] != '-')) {
            j = atoi(argv[i]);
printf("exclude pass %d\n", j );
           	
            if ((j >= 0) && (j <= ht))
               xparray[j] = 1;
            i++;
         }
         i--;
      }
      else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "-xs")) {
         i++;
         while ((i < argc) && (argv[i][0] != '-')) {
	    j = atoi(argv[i]);
	    if ((j >= 0) && (j <= ht))
	       xarray[j] = 1;
	    i++;
       	 }
	 i--;
      }
      else if (!strcmp(argv[i], "-i")) {
         i++;
	 while ((i < argc) && (argv[i][0] != '-')) {
	    j = atoi(argv[i]);
	    if ((j >= 0) && (j <= ht))
	       iarray[j] = 1;
	    i++;
       	 }
	 i--;
      } else {
		 fprintf( fp,"Unrecognized command line option '%s'\n", argv[i]); 
		 goto bail;//exit(0);
      }
   }

   for (i = 0; i <= ht; i++)        /* include and exclude flags clash? */
	  if (iarray[i] && xarray[i])
	      goto bail;

   if (mode != CUSTOM)
      initialize();

   if (l % rhythm_period) {
      fprintf( fp,"Pattern length must be a multiple of %d\n", rhythm_period);
      goto bail;//exit(0);
   }

       /*  The following variable slot_size serves two functions.  It  */
       /*  is the size of a slot used in the multiplexing filter, and  */
       /*  it is the number of throws allocated in memory.  The number */
       /*  of throws needs to be larger than L sometimes since these   */
       /*  same structures are used to find starting and ending        */
       /*  sequences (containing as many as HT elements).              */

   slot_size = ((ht > l) ? ht : l);
   slot_size += rhythm_period - (slot_size % rhythm_period);

   for (i = 0; i < hands; i++)
      for (j = 0; j < rhythm_period; j++)
         if ((k = rhythm_repunit[i][j]) > max_occupancy)
	    max_occupancy = k;
   max_occupancy *= multiplex;
   if (max_occupancy == 1)       /* no multiplexing, turn off filter */
      mp_filter = 0;

     /*  Now allocate the memory space for the states, rhythms, and  */
     /*  throws in the pattern, plus other incidental variables.     */

   if ((pattern_state = (int ***)
                   malloc((slot_size + 1) * sizeof(int **))) == 0)
      goto bail;//die();
   for (i = 0; i < (slot_size + 1); i++) 
      pattern_state[i] = alloc_array(hands, ht);

   if ((pattern_rhythm = (int ***)
                   malloc((slot_size + 1) * sizeof(int **))) == 0)
      goto bail;//die();
   for (i = 0; i < (slot_size + 1); i++) {
      pattern_rhythm[i] = alloc_array(hands, ht);
      for (j = 0; j < hands; j++) 
         for (k = 0; k < ht; k++)
	    pattern_rhythm[i][j][k] = 
  	         multiplex * rhythm_repunit[j][(k + i) % rhythm_period];
   }

   if ((pattern_holes = (int ***)
                   malloc(l * sizeof(int **))) == 0)
      goto bail;//die();
   for (i = 0; i < l; i++) 
      pattern_holes[i] = alloc_array(hands, ht);

   if ((pattern_throw = (struct j2_throw ***)
                   malloc(slot_size * sizeof(struct j2_throw **))) == 0)
      goto bail;//die();
   for (i = 0; i < slot_size; i++) {
      if ((pattern_throw[i] = (struct j2_throw **)
	               malloc(hands * sizeof(struct j2_throw *))) == 0)
	 goto bail;//die();
      for (j = 0; j < hands; j++)
         if ((pattern_throw[i][j] = (struct j2_throw *)
	           malloc(max_occupancy * sizeof(struct j2_throw))) == 0)
  	    goto bail;//die();
   }

   if (mp_filter) {         /* allocate space for filter variables */
      if ((pattern_filter = (struct filter ***)
                  malloc((l + 1) * sizeof(struct filter **))) == 0)
	 goto bail;//die();
      for (i = 0; i <= l; i++) {
         if ((pattern_filter[i] = (struct filter **)
	              malloc(hands * sizeof(struct filter *))) == 0)
	    goto bail;//die();
	 for (j = 0; j < hands; j++)
	    if ((pattern_filter[i][j] = (struct filter *)
		      malloc(slot_size * sizeof(struct filter))) == 0)
	       goto bail;//die();
      }
   }

   pattern_throwcount = alloc_array(l, hands);
   ground_state = alloc_array(hands, ht);

   if (people > 1) {       /* passing communication delay variables */
      if ((scratch1 = (int *)malloc(hands * sizeof(int))) == 0)
         goto bail;//die();
      if ((scratch2 = (int *)malloc(hands * sizeof(int))) == 0)
         goto bail;//die();
   }

   if ((starting_seq=(char *)malloc(hands*ht*CHARS_PER_THROW*sizeof(char)))==0)
      goto bail;//die();
   if ((ending_seq = (char *)malloc(hands*ht*CHARS_PER_THROW*sizeof(char)))==0)
      goto bail;//die();
   if ((pattern_buffer = (char *)malloc(hands*l*CHARS_PER_THROW*
                              sizeof(char)))==0)
      goto bail;//die();

       /*  Now that all the storage space is allocated, generate  */
       /*  and print out the loops.  */

   find_ground();                /* find ground state */

   num = gen_patterns(0, 0, 0, 0L); 

   if (numflag) {
      if (num == 1)
         fprintf( fp,"1 pattern found\n");
      else
         fprintf( fp,"%ld patterns found\n", num);
   }
   
bail:
	freeWwMem( );


	return 0;
}


void die(void)       /* just like the name sounds */
{
   fprintf( fp,"Insufficient memory\n");
	exit( -1);
}


/***********************************************************************/
/*                                                                     */
/*  The following are the customization functions.                     */
/*  See the documentation for an explanation of these routines.        */
/*                                                                     */
/***********************************************************************/

int custom_valid_throw(int pos)    /* excluded throws already checked for */
{
#ifdef TARGET_OS_MAC
#pragma unused (pos)
#endif

   return 1;           /* return j2_throw ok */
}

int custom_valid_pattern(void)
{
   return 1;           /* return pattern ok */
}

char *custom_print_throw(char *pos, struct j2_throw **j2_throw, int **rhythm)
//  char *pos;                      /* buffer pointer we're writing to */
{
   return default_custom_print_throw(pos, j2_throw, rhythm);
                              /* just do the default for now */
}

/***********************************************************************/

#undef malloc
void *wwMalloc( size_t size)
{	void *p;
	_wwMem *thisP, *t;

	p = malloc( size );
	if( p == NULL )
		return NULL;
//	fprintf( fp, "malloc(%ld) => %lx\n", size, p);

	// add to my memory list
	thisP = (_wwMem *)malloc( sizeof( _wwMem ) );
	if( thisP == 0 )
		return NULL;	
	
	// fill in values
	thisP->memPointer = p;
	thisP->nextWwMem = NULL;
	
	// add to chain
	if( firstWwMem == NULL )
		firstWwMem = thisP;
	else			
	{
		// initialize
		t = firstWwMem;
		// search last entry
		while( t->nextWwMem )
			t = t->nextWwMem;
		// add new entry
		t->nextWwMem = thisP;	
	}
	return p;
}	


void freeWwMem( void )
{

	_wwMem *p, *t;

//fprintf( fp,"freeing memory \n");	
	p = firstWwMem;
	
	while( p )
	{
		t = p->nextWwMem;
		// free memory
//fprintf( fp,"freing %lx\n", p->memPointer);		
		free( p->memPointer );
		// free helper structure
		free( p );
		// next helper structure
		p = t;
	}
	firstWwMem = 0;
//fprintf( fp,"freeing memory  ... done\n");	
}		

void initGlobals( void )
{
	pattern_rhythm = pattern_state = 0;
	pattern_throwcount = 0;
	pattern_holes = 0;
	pattern_throw = 0;
	pattern_filter = 0;
	max_occupancy = 0;
	rhythm_repunit = 0;
	rhythm_period = 0;
	holdthrow = person_number = scratch1 = scratch2 = 0;
	leader_person = 1;
	ground_state = 0;
	n = l = ht = 0;
	xarray = xparray = iarray = NULL;
	numflag = groundflag = 0;
	fullflag = 1;
	mp_filter = 1, delaytime = 0;
	sequence_flag = 1;
	mode = ASYNCH_SOLO;      	       
	people = 0, slot_size = 0;            
	starting_seq = pattern_buffer = ending_seq = NULL;
	/* added by ...ww*/
	rotflag = 0;
	lameflag = 0;
}

