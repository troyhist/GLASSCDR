// =======================================================================
// Glass coder FUNctuion library CYR ver. 1.1 (C, CPP)
// Troy computing, Inc.
// Copyright (c) 1994-'99
// -----------------------------------------------------------------------
// Transparent data coding. CYR version.
// Used library: memc.h
// -----------------------------------------------------------------------
// File: glassfun.c
// -----------------------------------------------------------------------
//     Compiler: Borland C++ 5.02
//  Application: Win32 console
// -----------------------------------------------------------------------
// Made date: 24/ XI.1999
// Last edit: 04/XII.1999
//     Build: 1
// =======================================================================
#include <mem.h>		// memcpy
#include <string.h>		// strlen
// -----------------------------------------------------------------------
#include "memc.h"
#include "glassfun.h"
// =======================================================================
//#define _GF_DEBUG		// Remark in release
// -----------------------------------------------------------------------
// Error messages
// -----------------------------------------------------------------------
#ifdef _GF_CODER
char* gc_errors[]={
"no error",
"global encode error",
"global decode error",
"not enough memory",
"not a GC file",
"wrong version of GC"};
#endif
// -----------------------------------------------------------------------
// Coding letters/alphabet (all)
// -----------------------------------------------------------------------
char gc_abc[]={
'A',  'a',  'B',  'C',  'c',  'E',  'e',  'K',  'M',  'O',  'o',  'P',
'p',  'T',  'X',  'x',
0x80, 0xa0, 0x82, 0x91, 0xe1, 0x85, 0xa5, 0x8a, 0x8c, 0x8e, 0xae, 0x90,
0xe0, 0x92, 0x95, 0xe5};
// -----------------------------------------------------------------------
// Coding letters/alphabet (latin) 	- bit 0
// -----------------------------------------------------------------------
char gc_abclat[]={
'A',  'a',  'B',  'C',  'c',  'E',  'e',  'K',  'M',  'O',  'o',  'P',
'p',  'T',  'X',  'x'};
// -----------------------------------------------------------------------
// Coding letters/alphabet (cyrilic)	- bit 1
// -----------------------------------------------------------------------
char gc_abccyr[]={
0x80, 0xa0, 0x82, 0x91, 0xe1, 0x85, 0xa5, 0x8a, 0x8c, 0x8e, 0xae, 0x90,
0xe0, 0x92, 0x95, 0xe5};
// -----------------------------------------------------------------------
// GlassCoder's error
// -----------------------------------------------------------------------
int gc_error=gcEOK;
// -----------------------------------------------------------------------
#define sgc_abc		sizeof(gc_abc)
#define sgc_abclat	sizeof(gc_abclat)
#define sgc_abccyr	sizeof(gc_abccyr)
// -----------------------------------------------------------------------
// Bit replacements
// -----------------------------------------------------------------------
#define gcZERO(_i_)	gc_abclat[_i_]
#define gcONE(_i_)	gc_abccyr[_i_]
// =======================================================================
// Private prototypes
// =======================================================================
int _gc_getbit(char* buffer, int j);
int _gc_isconsist(char letter);
int _gc_letter2idx(char letter);
int _gc_letter2val(char letter);
void _gc_setbit(char* buffer, int j, int x);
// =======================================================================
// Private functions
// =======================================================================
char* _gc_encode(void* inbuf, int len, char* appbuf, int alen, int options)
{
// -----------------------------------------------------------------------
// Description: Encodes 'inbuf' via 'appbuf'
//   Parametrs: inbuf	- input (binary) buffer
//              len     - size of 'inbuf' in bytes
//		appbuf	- aplicable (text) buffer
//		alen	- size of 'appbuf' in bytes
//              options - coding methods
//      Return: ptr to coded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
char* outbuf;
int i;
int j=0;			// j - bit pointer
int maxbitnum=gcBIB*len;	// size of 'inbuf' in bits
gc_error=gcEOK;
// -----------------------------------------------------------------------
if ((outbuf=newm(char, alen))==NULL) { gc_error=gcEENC; return(NULL); };
// -----------------------------------------------------------------------
memcpy(outbuf, appbuf, alen);
// -----------------------------------------------------------------------
for (i=0; i<alen; i++)
	{
        if (_gc_isconsist(outbuf[i]) && j<maxbitnum)
                if (_gc_getbit(inbuf, j++))
                        outbuf[i]=gcONE(_gc_letter2idx(outbuf[i]));
                else
                	outbuf[i]=gcZERO(_gc_letter2idx(outbuf[i]));
        }
// -----------------------------------------------------------------------
// Not available in this version
// -----------------------------------------------------------------------
if (options) return(outbuf);
// -----------------------------------------------------------------------
return(outbuf);
// -----------------------------------------------------------------------
}
// =======================================================================
void* _gc_decode(char* inbuf, int ilen, int len, int options)
{
// -----------------------------------------------------------------------
// Description: Decodes 'inbuf'
//   Parametrs: inbuf	- input (text) buffer
//              ilen	- size of 'inbuf' (text) in bytes
//		len	- size of output (binary) buffer (bytes to decode)
//              options - coding methods
//      Return: ptr to decoded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
char* outbuf;
int i;
int j=0;			// j - bit pointer
int maxbitnum=gcBIB*len;	// size of 'outbuf' in bits
gc_error=gcEOK;
// -----------------------------------------------------------------------
if ((outbuf=newm(char, len))==NULL) { gc_error=gcEDEC; return(NULL); };
// -----------------------------------------------------------------------
for (i=0; i<ilen; i++)
        if (_gc_isconsist(inbuf[i]) && j<maxbitnum)
        	_gc_setbit(outbuf, j++, _gc_letter2val(inbuf[i]));
// -----------------------------------------------------------------------
// Not available in this version
// -----------------------------------------------------------------------
if (options) return(outbuf);
// -----------------------------------------------------------------------
return(outbuf);
// -----------------------------------------------------------------------
}
// =======================================================================
int _gc_getbit(char* buffer, int j)
{
// -----------------------------------------------------------------------
// Description: Returns value of given bit's number
//   Parametrs: buffer	- buffer with binary data
//		j	- bit's number
//      Return: NULL, !NULL
// -----------------------------------------------------------------------
return(buffer[j/gcBIB] & (1<<(j%gcBIB)));
}
// =======================================================================
int _gc_isconsist(char letter)
{
// -----------------------------------------------------------------------
// Description: Checks letter for possibility of coding
//   Parametrs: letter	- given letter
//      Return: !NULL - success, NULL - otherwise
// -----------------------------------------------------------------------
int i;
for (i=0; i<(int)sgc_abc; i++)
        if (gc_abc[i]==letter) return(!NULL);
return(NULL);
}
// =======================================================================
int _gc_letter2idx(char letter)
{
// -----------------------------------------------------------------------
// Description: Returns letter's index in coding alphabets
//   Parametrs: letter	- given letter
//      Return: idx - success, gcERROR - ERROR
// -----------------------------------------------------------------------
int i;
for (i=0; i<sgc_abclat; i++) if (gc_abclat[i]==letter) return(i);
for (i=0; i<sgc_abccyr; i++) if (gc_abccyr[i]==letter) return(i);
return(gcERROR);
}
// =======================================================================
int _gc_letter2val(char letter)
{
// -----------------------------------------------------------------------
// Description: Returns value of one bits using letter.
//   Parametrs: letter	- given letter
//      Return: NULL, 1
// -----------------------------------------------------------------------
int i;
for (i=0; i<sgc_abclat; i++) if (gc_abclat[i]==letter) return(gcOFF);
for (i=0; i<sgc_abccyr; i++) if (gc_abccyr[i]==letter) return(gcON);
return(gcERROR);
}
// =======================================================================
void _gc_setbit(char* buffer, int j, int x)
{
// -----------------------------------------------------------------------
// Description: Sets value of given bit's number
//   Parametrs: buffer	- buffer with binary data
//		j	- bit's number
//		x	- new value
//      Return: <none>
// -----------------------------------------------------------------------
#ifdef _GF_DEBUG
#include <stdio.h>
#endif
buffer[j/gcBIB]=x?
	buffer[j/gcBIB]|(char)(1<<(j%gcBIB)):
	buffer[j/gcBIB]&(char)~(1<<(j%gcBIB));
#ifdef _GF_DEBUG
if (j>memhbGC())
	{
        printf ("%d ",x);
        if (!(j%gcBIB)) printf (": %d = %c\n", (j/gcBIB)-memhGC(), buffer[(j/gcBIB)-1]);
        }
#endif
}
// =======================================================================
int gc_getcapacity(void* vbuffer, int len, int options)
{
// -----------------------------------------------------------------------
// Description: Returns capacity of given buffer in bits
//   Parametrs: buffer  - text buffer
//              len     - size in bytes
//              options - coding methods
//      Return: size in bits
// -----------------------------------------------------------------------
char* buffer=vbuffer;
int i, bits=0;
// -----------------------------------------------------------------------
for (i=0; i<len; i++)
        {
	// ---------------------------------------------------------------
        // Checks options
        // ---------------------------------------------------------------
        if ((buffer[i]=='H' || buffer[i]==(char)0x8d) && options&enH)
                        bits++;
        if ((buffer[i]=='u' || buffer[i]==(char)0xa8) && options&enU)
                        bits++;
        if ((buffer[i]=='n' || buffer[i]==(char)0xaf) && options&enN)
                        bits++;
        if ((buffer[i]=='b' || buffer[i]==(char)0x9c) && options&enB)
                        bits++;
        if ((buffer[i]=='r' || buffer[i]==(char)0xa3) && options&enR)
                        bits++;
        if ((buffer[i]=='y' || buffer[i]==(char)0xe3) && options&enY)
                        bits++;
        // ---------------------------------------------------------------
        // Standart replacement
        // ---------------------------------------------------------------
        if (_gc_isconsist(buffer[i])) bits++;
        }
bits-=memhbGC();
return(bits>0?bits:0);
}
// =======================================================================
headerGC gc_getinfo(void* buffer, int len)
{
// -----------------------------------------------------------------------
// Description: Returns information about given data
//   Parametrs: buffer	- text buffer
//		len	- size in bytes
//      Return: headerGC struct
// -----------------------------------------------------------------------
headerGC h, *header;	// decoded data: header
gc_error=gcEOK;
// -----------------------------------------------------------------------
// Temporary fills structure
// -----------------------------------------------------------------------
h.initGC=NULL;
h.version=0;
h.lod=0;
// -----------------------------------------------------------------------
if (len<memhbGC()) { gc_error=gcENOTGC; return(h); }
// -----------------------------------------------------------------------
// Decodes header (w/o) options
// -----------------------------------------------------------------------
if ((header=_gc_decode(buffer, len, memhGC(), NULL))==NULL)
	{ gc_error=gcENOMEM; return(h); };
// -----------------------------------------------------------------------
h=*header; deletem(header);
if (h.initGC!=initGCok) gc_error=gcENOTGC;
// -----------------------------------------------------------------------
return (h);
// -----------------------------------------------------------------------
}
// =======================================================================
char* gc_encode(void* inbuf, int len, char* appbuf, int alen, int options)
{
// -----------------------------------------------------------------------
// Description: Encodes 'inbuf' via 'appbuf' with header adding
//   Parametrs: inbuf	- input (binary) buffer
//              len     - size of 'inbuf' in bytes
//		appbuf	- aplicable (text) buffer
//		alen	- size of 'appbuf' in bytes
//              options - coding methods
//      Return: ptr to coded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
char* outbuf;
headerGC* header;
gc_error=gcEOK;
if ((header=(headerGC*)newm(char,len+memhGC()))==NULL)
	{ gc_error=gcENOMEM; return(NULL); }; 
// -----------------------------------------------------------------------
// Filling header
// -----------------------------------------------------------------------
header->initGC=initGCok;
header->version=gcVERSION;
header->lod=(short)len;
// -----------------------------------------------------------------------
memcpy((char*)header+memhGC(), inbuf, len);
// -----------------------------------------------------------------------
outbuf=_gc_encode(header, len+memhGC(), appbuf, alen, options);
deletem(header);
// -----------------------------------------------------------------------
return(outbuf);
}
// =======================================================================
char* gc_decode(char* inbuf, int ilen, int options)
{
// -----------------------------------------------------------------------
// Description: Decodes 'inbuf' using GC header
//   Parametrs: inbuf	- input (text) buffer
//              ilen	- size of 'inbuf' (text) in bytes
//              options - coding methods
//      Return: ptr to decoded text buffer - success, NULL - otherwise
// -----------------------------------------------------------------------
char* data;		// decoded data: header + binary data
void* outbuf;		// decoded data: binary data
headerGC* header;	// decoded data: header
gc_error=gcEOK;
// -----------------------------------------------------------------------
// Decodes header (w/o) options
// -----------------------------------------------------------------------
if ((header=_gc_decode(inbuf, ilen, memhGC(), NULL))==NULL)
	{ gc_error=gcENOMEM; return(NULL); };
// -----------------------------------------------------------------------
// Checks header
// -----------------------------------------------------------------------
if (header->initGC!=initGCok) { gc_error=gcENOTGC; return(NULL); };
if (header->version!=gcVERSION) { gc_error=gcEWRNGVER; return(NULL); };
// -----------------------------------------------------------------------
// Decodes all data
// -----------------------------------------------------------------------
if ((data=_gc_decode(inbuf, ilen, header->lod+memhGC(), options))==NULL)
	{ gc_error=gcENOMEM; return(NULL); };
// -----------------------------------------------------------------------
if ((outbuf=newm(char,header->lod))==NULL)
	{ gc_error=gcENOMEM; return(NULL); };
// -----------------------------------------------------------------------
memcpy (outbuf, data+memhGC(), header->lod); 
// -----------------------------------------------------------------------
deletem(header); deletem(data);
// -----------------------------------------------------------------------
return(outbuf);
}
// =======================================================================

