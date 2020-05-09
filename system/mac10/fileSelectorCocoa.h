#if 0
//
//  o.h
//  t
//
//  Created by Wolfgang on 23.02.13.
//  Copyright (c) 2013 Wolfgang. All rights reserved.
//

#ifndef t_o_h
#define t_o_h



#if defined(__cplusplus)
extern "C"
{
#endif
	
void initFileSelectorModule( void );
bool callFileSelectorOpen( char* infoString, char *filePathAndName );
bool callFileSelectorSave( char* infoString, char *filePathAndName);

#if defined(__cplusplus)
}
#endif



#endif

#endif
