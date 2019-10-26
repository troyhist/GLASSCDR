// =======================================================================
// Glass coder FUNctuion library CYR ver. 1.1 (C, CPP)
// Troy computing, Inc.
// Copyright (c) 1994-'99
// -----------------------------------------------------------------------
// Transparent data coding. CYR version.
// Used library: memc.h
// -----------------------------------------------------------------------
// File: glassfun.h
// -----------------------------------------------------------------------
//     Compiler: Borland C++ 5.02
//  Application: Win32 console
// -----------------------------------------------------------------------
// Made date: 24/ XI.1999
// Last edit: 04/XII.1999
//     Build: 1
// =======================================================================
// Precompile defenitions ... CHANGEABLE !!!
// =======================================================================
#define _GF_CODER	// for CLASSCODer program only
// =======================================================================
// Constants ... DO NOT CHANGE !!!
// =======================================================================
#ifndef _HEADERGC
#define _HEADERGC
// -----------------------------------------------------------------------
// Options keys (enable ...)
// -----------------------------------------------------------------------
#define enH	1
#define enU	2
#define enN	4
#define enB	8
#define enR	16
#define enY	32
// -----------------------------------------------------------------------
// Header struct
// -----------------------------------------------------------------------
typedef struct
{
char initGC;		// Initialization of GlassCoder
char version;		// Format's version
short lod;		// Length of data in bytes
} headerGC;
// -----------------------------------------------------------------------
// GC error
// -----------------------------------------------------------------------
extern int gc_error;
// -----------------------------------------------------------------------
// Initializing constants
// -----------------------------------------------------------------------
#define initGCok	'G'	// correct initialization
#define gcVERSION	0x11	// current format version (1.1)
// -----------------------------------------------------------------------
// Important constants
// -----------------------------------------------------------------------
#define gcBIB		8	// bits in byte
#define gcON		1
#define gcOFF		NULL
// -----------------------------------------------------------------------
// CG errors
// -----------------------------------------------------------------------
#define gcERROR		-1	// global error
#define gcEOK		NULL	// no error
#define gcEENC		1	// global encode error
#define gcEDEC		2	// global decode error
#define gcENOMEM  	3	// not enough memory
#define gcENOTGC  	4	// not a GC file
#define gcEWRNGVER	5	// wrong version of GC
// -----------------------------------------------------------------------
#ifdef _GF_CODER
extern char* gc_errors[];
#endif
// -----------------------------------------------------------------------
// Important macros
// -----------------------------------------------------------------------
#define memhGC()  sizeof(headerGC) 	// Size of header in bytes
#define memhbGC() memhGC()*gcBIB	// Size of header in bits
// =======================================================================
int gc_getcapacity(void* buffer, int len, int options);
// -----------------------------------------------------------------------
// Description: Returns capacity of given buffer in bits
//   Parametrs: buffer	- text buffer
//		len	- size in bytes
//              options	- coding methods
//      Return: size in bits
// -----------------------------------------------------------------------
headerGC gc_getinfo(void* buffer, int len);
// -----------------------------------------------------------------------
// Description: Returns information about given data
//   Parametrs: buffer	- text buffer
//		len	- size in bytes
//      Return: headerGC struct
// -----------------------------------------------------------------------
char* gc_encode(void*inbuf, int len, char*appbuf, int alen, int options);
// -----------------------------------------------------------------------
// Description: Encodes 'inbuf' via 'appbuf'
//   Parametrs: inbuf	- input (binary) buffer
//              len     - size of 'inbuf' in bytes
//		appbuf	- aplicable (text) buffer
//		alen	- size of 'appbuf' in bytes
//              options - coding methods
//      Return: ptr to coded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
char* gc_decode(char* inbuf, int ilen, int options);
// -----------------------------------------------------------------------
// Description: Decodes 'inbuf' via 'appbuf' using GC header
//   Parametrs: inbuf	- input (text) buffer
//              ilen	- size of 'inbuf' (text) in bytes
//              options - coding methods
//      Return: ptr to decoded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
#endif
// =======================================================================
