// =======================================================================
// Glass Coder CYR ver. 1.1 (C, CPP)
// Troy computing, Inc.
// Copyright (c) 1994-'99
// -----------------------------------------------------------------------
// Transparent data coding. CYR version.
// Used library: memc, glassfun
// -----------------------------------------------------------------------
// File: glasscod.c
// -----------------------------------------------------------------------
//     Compiler: Borland C++ 5.02
//  Application: Win32 console
// -----------------------------------------------------------------------
// Made date: 22/VIII.1999
// Last edit: 04/ XII.1999
//     Build: 1
// =======================================================================
#include <io.h>			// filelength
#include <stdio.h>
#include <string.h>		// strcmp
// -----------------------------------------------------------------------
#include "memc.h"
#include "glassfun.h"
// =======================================================================
#define pYN(_x_)	_x_?"Yes":"No"
// =======================================================================
/*void PrintOptions(int CODEFLG)
{
printf ("Options:\n"
        	"enH (not 4 fido) = %s\n"
                "enU (u = \xa8)     = %s\n"
                "enN (n = \xaf)     = %s\n"
                "enB (b = \x9c)     = %s\n"
                "enR (r = \xa3)     = %s\n"
                "enY (y = \xe3)     = %s\n"
                "\n",
                pYN(CODEFLG&enH), pYN(CODEFLG&enU), pYN(CODEFLG&enN),
                pYN(CODEFLG&enB), pYN(CODEFLG&enR), pYN(CODEFLG&enY)
                );
}
// =======================================================================
int getflags(int argc, char* argv[])
{
int i, res=0;
for (i=0; i<argc; i++)
	{
        switch(*(argv[i]+1))
        	{
		case 'H': res|=enH; break;
		case 'U': res|=enU; break;
        	case 'N': res|=enN; break;
        	case 'B': res|=enB; break;
        	case 'R': res|=enR; break;
        	case 'Y': res|=enY; break;
                default: break;
        	}
        }
return (res);
}*/
// =======================================================================
int main(int argc, char* argv[])
{
// -----------------------------------------------------------------------
//int CODEFLG;
// -----------------------------------------------------------------------
printf ("\n"
	"Glass Coder CYR ver. 1.1\n"
        "Troy computing, Inc.\n"
        "Copyright (c) 1994-'99\n\n");
// -----------------------------------------------------------------------
if (argc==1)
	{
	printf ("Usage: GLASSCOD <command> <infile> <outfile> [-options]\n"
        	"       GLASSCOD /? - to help\n\n");
        }
// -----------------------------------------------------------------------
// Help
// -----------------------------------------------------------------------
else if (argc==2 && !strcmp(argv[1], "/?"))
	{
        printf ("Sorry, general help isn't available ... "
        	"supported CP866 only\n\n"
        	"GLASSCOD t <appfile> - test of capacity of given file\n"
                "GLASSCOD i <appfile> - get information of given file\n"
                "GLASSCOD e <infile> <appfile> <outfile> - encode\n"
                "GLASSCOD d <infile> <outfile>           - decode\n\n"
                "Options: <not available in this version>\n"
                "/p:<password>	- use password to protect data\n"
                "/x:[Hunbry]	- extended encoding (\x8d\xa8\xaf"
				                "\x9c\xa3\xe3)\n"
                "/w		- use CP1215 (win)\n\n"
                "SHAREWARE version. troyanets@mail.ru, 2:5020/1472.19,"
				                " 123:45/5.19\n\n"
                );
        }
// -----------------------------------------------------------------------
// Test of capacity
// -----------------------------------------------------------------------
else if (argc>=3 && (*argv[1]=='t' || *argv[1]=='T'))
	{
        // ---------------------------------------------------------------
        char* buffer;
        int len;
        int bytes, bits; // bytes readed, bits found
        // ---------------------------------------------------------------
        //CODEFLG=getflags(argc-3, argv+3);
	// ---------------------------------------------------------------
        {
        FILE* fp;
        if ((fp=fopen(argv[2], "rb"))==NULL)
        	{ perror("Can't open input file"); return(NULL); }
        // ---------------------------------------------------------------
        len=(unsigned)filelength(fileno(fp));
        // ---------------------------------------------------------------
        if ((buffer=newm(char, len))==NULL)
        	{ perror("Can't allocate memory for input file");
                fcloseall(); return(NULL); }
        // ---------------------------------------------------------------
        bytes=fread(buffer, 1, len, fp); fclose(fp);
        }
        // ---------------------------------------------------------------
        bits=gc_getcapacity(buffer, bytes, NULL); deletem(buffer);
        // ---------------------------------------------------------------
        printf ("Test results:\n"
        	"Filename        = %-12s (%d bytes, %d Kb)\n"
        	"bits available  = %-6d (%d bytes, %d Kb)\n"
                "GC header size  = %-6d (%d bytes)\n"
                "capacity ratio  = %.2f%%\n\n",
        	argv[2], len, len/1024,
                bits, bits/8,	 bits/8192,
                memhbGC(), memhGC(), (float)bits*100/(len*8));
        // ---------------------------------------------------------------
        }
// -----------------------------------------------------------------------
// Information
// -----------------------------------------------------------------------
else if (argc>=3 && (*argv[1]=='i' || *argv[1]=='I'))
        {
        // ---------------------------------------------------------------
        char* buffer;
        int len, bytes;
        headerGC h;
        // ---------------------------------------------------------------
        {
        FILE* fp;
        if ((fp=fopen(argv[2], "rb"))==NULL)
        	{ perror("Can't open input file"); return(NULL); }
        // ---------------------------------------------------------------
        len=(unsigned)filelength(fileno(fp));
        // ---------------------------------------------------------------
        if ((buffer=newm(char, len))==NULL)
        	{ perror("Can't allocate memory for input file");
                fcloseall(); return(NULL); }
        // ---------------------------------------------------------------
        bytes=fread(buffer, 1, len, fp); fclose(fp);
        }
        // ---------------------------------------------------------------
        h=gc_getinfo(buffer, bytes); deletem(buffer);
        // ---------------------------------------------------------------
        printf ("Info results:\n"
        	"Filename        = %s (%d bytes, %d Kb)\n"
                "is GC file      = %s\n"
	      	"h.initGC        = '%c' (%x)\n"
                "h.version       = %#x\n"
                "h.lod           = %u (%u bytes, %u Kb)\n\n",
        	argv[2], len, len/1024, pYN(!gc_error), h.initGC,
                h.initGC, h.version, h.lod*8, h.lod, h.lod/1024);
        // ---------------------------------------------------------------
        }
// -----------------------------------------------------------------------
// Encoding
// -----------------------------------------------------------------------
else if (argc>=5 && (*argv[1]=='e' || *argv[1]=='E'))
	{
        // ---------------------------------------------------------------
        char *inbuf, *outbuf, *appbuf;
        int len, alen;
        int bytes,abytes,bits; // bytes readed, bytes in afile, bits found
        FILE* fp;
        // ---------------------------------------------------------------
        // Reads input (binary) file
        // ---------------------------------------------------------------
        {
        if ((fp=fopen(argv[2], "rb"))==NULL)
        	{ perror("Can't open input file"); return(NULL); }
        // ---------------------------------------------------------------
        len=(unsigned)filelength(fileno(fp));
        // ---------------------------------------------------------------
        if ((inbuf=newm(char, len))==NULL)
        	{ perror("Can't allocate memory for input file");
                fcloseall(); return(NULL); }
        bytes=fread(inbuf, 1, len, fp); fclose(fp);
        }
        // ---------------------------------------------------------------
        // Reads application (text) file
        // ---------------------------------------------------------------
        {
        if ((fp=fopen(argv[3], "rb"))==NULL)
        	{
                deletem(inbuf);
                perror("Can't open application file"); return(NULL); }
        // ---------------------------------------------------------------
        alen=(unsigned)filelength(fileno(fp));
        // ---------------------------------------------------------------
        if ((appbuf=newm(char, alen))==NULL)
        	{
                deletem(inbuf);
                perror("Can't allocate memory for application file");
                fcloseall(); return(NULL); }
        abytes=fread(appbuf, 1, alen, fp); fclose(fp);
        }
        // ---------------------------------------------------------------
        bits=gc_getcapacity(appbuf, abytes, NULL);
        // ---------------------------------------------------------------
	printf ("Encoding info:\n"
        	"Infile          = %-12s (%d bytes, %d Kb)\n"
                "Appfile         = %-12s (%d bytes, %d Kb)\n"
                "Outfile         = %-12s (%d bytes, %d Kb)\n"
                "bits needed     = %-6d (%d bytes, %d Kb)\n"
        	"bits available  = %-6d (%d bytes, %d Kb)\n"
                "capacity ratio  = %.2f%%\n\n",
                argv[2], len, 	 len/1024,
                argv[3], alen, 	 alen/1024,
                argv[4], alen, 	 alen/1024,
                (len+memhGC())*8,
                (len+memhGC()),
                (len+memhGC())/1024,
                bits,	 bits/8, bits/8192,
                (float)bits*100/(alen*8));
        // ---------------------------------------------------------------
        printf("GLASSCDR: encoding starts ... ");
        outbuf=gc_encode(inbuf, bytes, appbuf, abytes, NULL);
        if (outbuf) printf ("ok\n");
	else   	{
                deletem(inbuf);
        	printf ("Error: %d.%s\n", gc_error, gc_errors[gc_error]);
                return(NULL);
                }
        // ---------------------------------------------------------------
        printf("GLASSCDR: writing starts ... ");
        if ((fp=fopen(argv[4], "wb"))==NULL)
        	{
                deletem(inbuf); deletem(appbuf);
                perror("Can't open application file"); return(NULL); }
        // ---------------------------------------------------------------
        printf ("%d bytes\n", fwrite(outbuf, 1, abytes, fp)); fclose(fp);
        // ---------------------------------------------------------------
        //deletem(outbuf);
        //deletem(inbuf);
        //deletem(appbuf);
        // ---------------------------------------------------------------
        }
// -----------------------------------------------------------------------
// Decoding
// -----------------------------------------------------------------------
else if (argc>=4 && (*argv[1]=='d' || *argv[1]=='D'))
	{
        // ---------------------------------------------------------------
        char *inbuf, *outbuf;
        int ilen, ibytes;	// len of input file, readed bytes
        headerGC h;
        FILE* fp;
        // ---------------------------------------------------------------
        // Reads input (binary) file
        // ---------------------------------------------------------------
        {
        if ((fp=fopen(argv[2], "rb"))==NULL)
        	{ perror("Can't open input file"); return(NULL); }
        // ---------------------------------------------------------------
        ilen=(unsigned)filelength(fileno(fp));
        // ---------------------------------------------------------------
        if ((inbuf=newm(char, ilen))==NULL)
        	{ perror("Can't allocate memory for input file");
                fcloseall(); return(NULL); }
        ibytes=fread(inbuf, 1, ilen, fp); fclose(fp);
        }
        // ---------------------------------------------------------------
        h=gc_getinfo(inbuf, ibytes);
        // ---------------------------------------------------------------
	printf ("Decoding info:\n"
        	"Infile          = %-12s (%d bytes, %d Kb)\n"
                "Outfile         = %-12s\n"
                "bits decoded    = %-6d (%d bytes, %d Kb)\n\n",
                argv[2], ilen, 	 ilen/1024,
                argv[3], h.lod*8,h.lod, h.lod/1024);
        // ---------------------------------------------------------------
        printf("GLASSCDR: decoding starts ... ");
        outbuf=gc_decode(inbuf, ilen, NULL);
        if (outbuf) printf ("ok\n");
        else   	{
                deletem(inbuf);
        	printf ("Error: %d.%s\n", gc_error, gc_errors[gc_error]);
                return(NULL);
                }
        // ---------------------------------------------------------------
        printf("GLASSCDR: writing starts ... ");
        if ((fp=fopen(argv[3], "wb"))==NULL)
        	{
                deletem(inbuf);
                perror("Can't open output file"); return(NULL); }
        // ---------------------------------------------------------------
        printf ("%d bytes\n", fwrite(outbuf, 1, h.lod, fp)); fclose(fp);
        // ---------------------------------------------------------------
        //deletem(outbuf);
        //deletem(inbuf);
        //deletem(appbuf);
        // ---------------------------------------------------------------
        }
// -----------------------------------------------------------------------
else
	{
        printf ("Error: Unknown command '%s'\n\n", argv[1]);
        }
return(NULL);
}
// =======================================================================
// -----------------------------------------------------------------------
// Description: <none>
//   Parametrs: <none>
//      Return: NULL - SUCCESS ...
// -----------------------------------------------------------------------