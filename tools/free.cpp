#include <stdlib.h>


#include "myPrintf.h"

#if defined(__cplusplus)
extern "C"
{
#endif

void _myFree( void *p );
void *_myMalloc( size_t l);


void _myFree( void *p )
{
	static int i;
	_myPrintf("freeing                  memory at %8lx (count = %4d)\n", p, i++);
	free( p );
}

void *_myMalloc( size_t l)
{
	char *r;
	static int i;



	
	_myPrintf("allocating %4lx bytes of memory at %8lx (count = %4d)\n", l, r = (char *)malloc( l ), i++);


	return r;
}	
	
	


#if defined(__cplusplus)
}
#endif
