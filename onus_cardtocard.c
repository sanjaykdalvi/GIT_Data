#include <stdio.h>

/* Added for decline report rectification requested by J&K bank */
 #define SUBRESULT_CODE_NEW     1

int lcheckCardBlock(t_paczpoau *ppaczpoau)
{
   if (
           (  memcmp(ppaczpoau->poau_cd_banque,"00003",5) == 0  )
        || (  memcmp(ppaczpoau->poau_cd_banque,"00005",5) == 0  )    
        || (  memcmp(ppaczpoau->poau_cd_banque,"00008",5) == 0  ) 
        || (  memcmp(ppaczpoau->poau_cd_banque,"00006",5) == 0  ) 
      )
   if ( ppaczpoau->poau_sanct_financiere[0] == '0' )
   {                                              
      f_printf(stderr, " RTH CARD IS BLOCKED " );
      return ( ECHEC );                          
   } 
   return ( SUCCESS );                        
}

int lCheckNegative ( t_paczpoau *ppaczpoau, char* SubResult )
{
	t_paczoppo	paczoppo;
	t_paczop00	paczop00;

	/* check negative date present in Card details */
	if ((  memcmp ( ppaczpoau->poau_date_opposition, "      ", 6 ) == 0 ) || 
       ( memcmp ( ppaczpoau->poau_date_opposition, "000000", 6 ) == 0 ) )
	{
		f_printf(stderr, "DEST CARD NOT HOTLISTED\n");
	}
	else
	{
		f_printf(stderr,"Negative date present in Online File[%.6s]\n",ppaczpoau->poau_date_opposition );
		if ( GetErrorCodeUsingCaptureCode ( ppaczpoau->poau_carte_a_capturer[0], SubResult ) == SUCCESS )
		{
			return ( SUCCESS );
		}
		else
		{
			GetErrorCodeUsingReferalCode ( paczpoau.poau_cd_appel[0], SubResult );
			return ( SUCCESS );
		}
		return ( SUCCESS );
	}
	/* Card Not Hotlisted  */
	return ( ECHEC );
}

