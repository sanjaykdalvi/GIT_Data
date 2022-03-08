#include <stdio.h>
#include <stdarg.h>
#include <define.h>
extern char flag_trace;

void f_printf(FILE * stream, char *fmt, ...);
void f_printf_Begin();
void f_printf_end();

static char szPrintBuf[250000]; 
static int iPos = 0 ;

void FPRINTF(FILE * stream, char *fmt, ...)
{
	char szTemp[1024*4];
	int nStrLen ;

	va_list   argptr;

	if ( flag_trace == 'Y' )
	{
			va_start( argptr, fmt );	
			szTemp[0] = 0 ;
			vsprintf( szTemp, fmt, argptr );
			nStrLen = strlen( szTemp );
			if( (iPos+nStrLen) < (sizeof( szPrintBuf )-1) )
			{
				strcpy( szPrintBuf+iPos, szTemp );
				iPos += nStrLen;
			}
			va_end( argptr );
	}
	return;
}

void f_printf_Begin()
{
	iPos = 0 ;
	memset( szPrintBuf, 0x00, sizeof( szPrintBuf ) );
}
void f_printf(FILE * stream, char *fmt, ...)
{
	char szTemp[1024*16];
	va_list argptr ;
	int nStrLen ;



	va_start( argptr, fmt );	
	szTemp[0] = 0 ;
	vsprintf( szTemp, fmt, argptr );
	nStrLen = strlen( szTemp );

	/*DELETE*/

  	fprintf(stderr, "%.*s", nStrLen, szTemp);
    	return;


	if( (iPos+nStrLen) < (sizeof( szPrintBuf )-1) )
	{
		strcpy( szPrintBuf+iPos, szTemp );
		iPos += nStrLen;
	}
	va_end( argptr );
}
void f_printf_end()
{
	if( iPos > 2 )
	{
		fprintf( stderr, "%s", szPrintBuf );
		fflush(stderr);
		iPos = 0 ;
		szPrintBuf[0] = 0 ;
	}
}
