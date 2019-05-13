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
	stdio.h

@brief
	standard C header file for CdePkg
@todo
*/
#ifndef _CDE_STDDEF_H_
#define _CDE_STDDEF_H_

//#pragma warning( disable : 4996 4311 4312 4101)

#ifndef NULL
	#define NULL ((void *)0)
#endif//NULL

#pragma warning( disable : 4142 )
#if   defined(_M_AMD64)
	typedef   signed long long ptrdiff_t;
	typedef unsigned long long size_t;
#else
	typedef  signed  long ptrdiff_t;
	typedef unsigned long size_t;
#endif

typedef short wchar_t;

#define offsetof(s,m) ((size_t)&(((s*)0)->m))

#endif//_CDE_STDDEF_H_
