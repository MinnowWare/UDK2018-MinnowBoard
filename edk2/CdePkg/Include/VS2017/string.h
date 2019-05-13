/*!
@copyright

	Copyright (c) 2019, MinnowWare. All rights reserved.
	This program and the accompanying materials are licensed and made
	available under the terms and conditions of the BSD License
	which accompanies this distribution.  The full text of the license
	may be found at

	http://opensource.org/licenses/bsd-license.php

	THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
	WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

@file
	string.h

@brief
	standard C header file for CdePkg
@todo
*/
#ifndef _CDE_STRING_H_
#define _CDE_STRING_H_

#include <stddef.h>
//#pragma warning( disable : 4996 4311 4312 4101)

void *memcpy( void * s1, const void * s2, size_t n );
void *memmove( void *s1, const void *s2, size_t n );
char *strcpy( char * s1, const char * s2 );
char *strncpy( char * s1, const char * s2, size_t n );
char *strcat( char * s1, const char * s2 );
char *strncat( char * s1, const char * s2, size_t n );
int memcmp( const void *s1, const void *s2, size_t n );
int strcmp( const char *s1, const char *s2 );
int strcoll( const char *s1, const char *s2 );
int strncmp( const char *s1, const char *s2, size_t n );
size_t strxfrm( char * s1, const char * s2, size_t n );
void *memchr( const void *s, int c, size_t n );
char *strchr( const char *s, int c );
size_t strcspn( const char *s1, const char *s2 );
char *strpbrk( const char *s1, const char *s2 );
char *strrchr( const char *s, int c );
size_t strspn( const char *s1, const char *s2 );
char *strstr( const char *s1, const char *s2 );
char *strtok( char * s1, const char * s2 );
void *memset( void *s, int c, size_t n );
char *strerror( int errnum );
size_t strlen( const char *s );

#endif//_CDE_STRING_H_
