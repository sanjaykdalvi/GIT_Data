#include <stdio.h>
#include <errno.h>
#include <isam.h>
#include <define.h>
#include <onus.h>
#include <constantes.h>
#include <paczpoau.h>

int fdporau ;
t_paczpoau               paczpoau;

main()
{
	char file[50] ;
	char cardno[30];
	char LastAuthDate[10] ;
	char CurrAuthDate[10] ;
	char	*DataFilePath = NULL;

	/* Get the MultiXPAC data files path */
	DataFilePath = getenv ( "MultipacDat" );
	if ( DataFilePath == NULL )
	{
		fprintf ( stderr, "ONUS: ERROR getting Data files path\n");
		exit ( 1 );
	}

	/* Open Card Holder online file */
	memset( file, 0x00, 50 );
	sprintf ( file, "%s/FC-POAU", DataFilePath );
	fdporau = isopen ( file, ISAUTOLOCK+ISINOUT );
	if ( fdporau < 0 )
	{
		fprintf ( stderr, "ONUS: ERROR OPEN FC-POAU: %d \n", iserrno );
		exit ( 1 );
	}

	memset( cardno, 0x00, 30 );
	printf( "Enter Card Number:\n" );
	scanf ("%s", cardno);
	if( strlen( cardno ) > 19 || strlen( cardno ) < 12 )
	{
		fprintf ( stderr, "Invalid Cardno %s \n", cardno );
		exit ( 1 );
	}
	memset( LastAuthDate, 0x00, 10 );
	memset( CurrAuthDate, 0x00, 10 );
	memset( &paczpoau, 0x00, sizeof(t_paczpoau) );
	if( CheckCardNumber( cardno, &paczpoau ) != SUCCESS )
	{
		fprintf( stderr, "Card Number not found\n" );
		exit( 1 );
	}
	memset( LastAuthDate, 0x00, 10 );
	memset( CurrAuthDate, 0x00, 10 );
	memcpy( CurrAuthDate, paczpoau.poau_dat_dern_auto, 6);
	printf( "CurrentAuthDate=%s\n", CurrAuthDate );
	
	printf( "Enter New Last Auth date\n" );
	scanf( "%s", LastAuthDate );
	printf("New Last Auth date=%s\n", LastAuthDate );

	memcpy( paczpoau.poau_dat_dern_auto, LastAuthDate, 6 );
	if ( iswrite( fdporau, &paczpoau ) < 0 )
		isrewrite( fdporau, &paczpoau );
	isunlock( fdporau );
	isclose ( fdporau );
}

int CheckCardNumber( char* CardNo, t_paczpoau* CardDetails)
{
   int Result = -1;
   int k = 0;

   memset ( CardDetails->poau_num_carte, ' ' , 19 );
   memcpy ( CardDetails->poau_num_carte, CardNo, strlen ( CardNo ) );
   /* Read the Card Holder details file */
   do
   {
      Result = isread ( fdporau, (char *)CardDetails, ISEQUAL );
      k++;
   }
   while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
   /*
   isunlock( fdporau );
   */

   if ( Result < 0 )
   {
      fprintf ( stderr, "\nCARD NOT FOUND IN CARDHOLDER ONLINE \
		 FILE %d [%.19s]\n", iserrno, CardNo );
      return ( ECHEC );
   }
   return ( SUCCESS );
}

