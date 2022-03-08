
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_load.c                                         *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 16-Feb-2000                                         *
 * Description         : Loading of Issuer table and Bank table functions    *
 *                       using linked list data structure.                   *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   24-Apr-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   18-Jun-04 Saju Joseph          BR-MXP-0406-0027                       *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <isam.h>
 #include <paczpa05.h>
 #include <paczpa02.h>
 #include <onus.h>
 #include <define.h>
 #include <paczpaem.h>

 #include "onus_load.h"
 
t_paczpaem paczpaem;

 #define STIP_PARAM    1   

/* Chowdesha added on 21-10-2010 for VIJAYA BANK - VIP CARDS STIP LIMIT  */
 #define STIP_PARAM_VIP_LIMIT  1  


/* ISAM Data File Descriptors */
extern int		fdparam;	/* Parameter file */

#if 0
typedef struct	pa05_tag
{
	t_paczpa05			paczpa05;
	struct pa05_tag		*link;
}t_pa05list;


typedef struct pa02_tag
{
	t_paczpa02			paczpa02;
	struct pa02_tag		*link;
}t_pa02list;


t_pa05list	*Head05 = (t_pa05list*)NULL;	/* Head node for paczpa05 table */
t_pa02list	*Head02 = (t_pa02list*)NULL;	/* Head node for paczpa02 table */
#endif

int LoadIssuerTable( void )
{
	int				Result;
	t_paczpa05		pa05;
	t_pa05list		*Temp05 = NULL;
	t_pa05list		*TempNode = NULL;
	struct keydesc	skey;

	skey.k_flags = ISDUPS;
	skey.k_nparts = 1;
	skey.k_part[0].kp_start = 0;
	skey.k_part[0].kp_leng = 32;
	skey.k_part[0].kp_type = CHARTYPE;

	memset ( (char*)&pa05, ' ', sizeof ( t_paczpa05 ) );
	memcpy ( pa05.pa05_notabl, "05", 2 );
	memcpy ( pa05.pa05_codemet, "000000000", 9 );

	Result = isstart ( fdparam, &skey, 32, (char*)&pa05, ISGTEQ );
	if ( Result < 0 )
	{
		f_printf(stderr, "isstart ERROR While Loading Issuer Table[%d] \n", \
		 iserrno );
		exit ( 0 );
	}

	Result = isread ( fdparam, (char*)&pa05, ISCURR );
	if ( Result < 0 )
	{
		f_printf(stderr, "isread cur ERROR While Loading Issuer Table[%d]\n",\
		  iserrno );
		exit ( 0 );
	}
	
	for ( ;; )
	{
		Temp05 = ( t_pa05list* ) malloc ( sizeof ( t_pa05list ) );
		memcpy ( (char*)&(Temp05->paczpa05), (char*)&pa05, \
		 sizeof ( t_paczpa05 ) ); 
		Temp05->link = NULL;

		f_printf(stderr, "ISSUER [%.9s] Loaded \n", pa05.pa05_codemet );

		if ( Head05 == NULL )
		{
			Head05 = TempNode = Temp05;	
		}
		else
		{
			TempNode->link = Temp05;
			TempNode = TempNode->link;
		}

		/* Read the Next Record */
		Result = isread ( fdparam, (char*)&pa05, ISNEXT );

		if ( Result < 0 )
		{
            f_printf(stderr, "ERROR LOADING THE CARD ISSUER TABLE, errno [%d]\n", iserrno );
            exit(0);
		}
		if ( memcmp ( pa05.pa05_notabl, "05", 2 ) != 0 )
		{
			break;
		}
	}

	isrelease ( fdparam );
	isunlock ( fdparam );

	f_printf(stderr, "\nISSUER Table loaded successfully \n" );
	return ( SUCCESS );
}

int LoadBankDetails ( void )
{
	int				Result;
	t_paczpa02		pa02;
	t_pa02list		*Temp02 = NULL;
	t_pa02list		*TempNode = NULL;
	struct keydesc	skey;

	skey.k_flags = ISDUPS;
	skey.k_nparts = 1;
	skey.k_part[0].kp_start = 0;
	skey.k_part[0].kp_leng = 32;
	skey.k_part[0].kp_type = CHARTYPE;

	memset ( (char*)&pa02, ' ', sizeof ( t_paczpa02 ) );
	memcpy ( pa02.pa02_notabl, "02", 2 );
	memcpy ( pa02.pa02_bq, "00000", 5 );

	Result = isstart ( fdparam, &skey, 32, (char*)&pa02, ISGTEQ );
	if ( Result < 0 )
	{
		f_printf(stderr, "isstart ERROR While Loading Bank Table[%d] \n",\
		 iserrno );
		exit ( 0 );
	}

	Result = isread ( fdparam, (char*)&pa02, ISCURR );
	if ( Result < 0 )
	{
		f_printf(stderr, "isread cur ERROR While Loading Bank Table[%d]\n",\
		  iserrno );
		exit ( 0 );
	}
	
	for ( ;; )
	{

         /** saju.05042004.Start. BOI is not AMCC certified.It has to be reomoved after AMCC certification .BR-MXP-0406-0027.Start

                if(memcmp(pa02.pa02_bq,"00006",5) == 0)
                    pa02.pa02_amcc_member[0] = 'N';

          BR-MXP-0406-0027.End  *******/

		Temp02 = ( t_pa02list* ) malloc ( sizeof ( t_pa02list ) );
		memcpy ( (char*)&(Temp02->paczpa02), (char*)&pa02, \
		 sizeof ( t_paczpa02 ) ); 
		Temp02->link = NULL;

		f_printf(stderr, "Bank [%.5s] AMCC [%c] Loaded \n", pa02.pa02_bq,pa02.pa02_amcc_member[0] );

		if ( Head02 == NULL )
		{
			Head02 = TempNode = Temp02;	
		}
		else
		{
			TempNode->link = Temp02;
			TempNode = TempNode->link;
		}

		/* Read the Next Record */
		Result = isread ( fdparam, (char*)&pa02, ISNEXT );
		if ( Result < 0 )
		{

            f_printf(stderr, "ERROR LOADING THE BANK TABLE (02), errno [%d]\n", Result );
			exit(0);
		}
		if ( memcmp ( pa02.pa02_notabl, "02", 2 ) != 0 )
		{
			break;
		}
	}

	isrelease ( fdparam );
	isunlock ( fdparam );

	f_printf(stderr, "\nBANK details Table loaded successfully \n" );
	return ( SUCCESS );
}

void FreeIssuerTable ( void )
{
	t_pa05list*		TempNode;

	TempNode = Head05;
	while ( TempNode != NULL )
	{
		Head05 = Head05->link; 
		free ( TempNode );
		TempNode = Head05;
	}
	f_printf(stderr, "Issuer table memory released \n" );
	return ;
}

void FreeBankTable ( void )
{
	t_pa02list*		TempNode;

	TempNode = Head02;
	while ( TempNode != NULL )
	{
		Head02 = Head02->link; 
		free ( TempNode );
		TempNode = Head02;
	}
	f_printf(stderr, "Bank table memory released \n" );
	return ;
}

void PrintBankTable ( void )
{
	t_pa02list		*TempNode;

	TempNode = Head02;

	while ( TempNode != NULL )
	{
		f_printf(stderr, "BANK [%.5s] \n", TempNode->paczpa02.pa02_bq );
		TempNode = TempNode->link;
	}

	return;
}

void PrintIssuerTable ( void )
{
	t_pa05list		*TempNode;

	TempNode = Head05;

	while ( TempNode != NULL )
	{
		f_printf(stderr, "ISSUER [%.9s] \n", TempNode->paczpa05.pa05_codemet);
		TempNode = TempNode->link;
	}
	return;
}

int GetIssuerDetails ( t_paczpa05* pa05, char* CardNumber )
{
	int			i;
	int			n[7];
	char		ProductCode[10];
	t_pa05list	*TempNode;

	n[0] = 9;
	n[1] = 8;
	n[2] = 7;
	n[3] = 6;
	n[4] = 3;
	n[5] = 2;
	n[6] = 1;
	for ( i = 0; i < 7; i++ )
	{
		TempNode = Head05;
		memset ( ProductCode, ' ', 10 ); 
		memcpy ( ProductCode, CardNumber, n[i] );
		while ( TempNode != NULL )
		{
			if ( memcmp ( TempNode->paczpa05.pa05_codemet, ProductCode, \
			 9 ) == 0 )
			{
				memcpy ( (char*)pa05, (char*)&(TempNode->paczpa05), \
				 sizeof ( t_paczpa05 ) ); 
				return ( SUCCESS );
			}
			TempNode = TempNode->link;
		} 
	}

	f_printf(stderr, "ISSUER DETAILS NOT FOUND FOR CARD[%.19s]\n",CardNumber );
	return ( ECHEC );
}

int GetBankDetails ( char* BankCode, t_paczpa02* pa02 )
{
	t_pa02list		*TempNode;

	TempNode = Head02;

	while ( TempNode != NULL )
	{
		if ( memcmp ( TempNode->paczpa02.pa02_bq, BankCode, 5 ) == 0 ) 
		{
			memcpy ( (char*)pa02, (char*)&(TempNode->paczpa02), \
			 sizeof ( t_paczpa02 ) );
			return ( SUCCESS );
		}
		TempNode = TempNode->link;
	}
	
	f_printf(stderr, "BANK DETAILS NOT FOUND FOR BANK[%.5s]\n", BankCode );
	return ( ECHEC );;
}

 #ifdef STIP_PARAM                                                              
int     iCheck_CardIssuer(porteur)
char    *porteur;
{
        int   i,cc, iRetval ;
        int  n[7];
        t_paczpaem  paczpaem;

        n[0] = 9; n[1] = 8;
        n[2] = 7; n[3] = 6;
        n[4] = 3; n[5] = 2;
                                                                                
        f_printf(stderr,"\n porteur = %.19s ", porteur );


       #ifdef STIP_PARAM_VIP_LIMIT
       iRetval = iCheck_VIP();
	   if( iRetval == SUCCESS) 
	   {
           f_printf(stderr, "\n VIP Card, ....... Limit not supported .....\n");
		   return (ECHEC);
	   }
      #endif 

        for (i=0;i<6;i++) 
        {
                memset((char *)&paczpaem,' ',             sizeof(t_paczpaem));
                memcpy(paczpaem.paem_notabl,     "EM",                     2); 
                memcpy(paczpaem.paem_codemet,    porteur,               n[i]);
                memcpy(paczpaem.paem_suitecle,  "                     ",  21);  

                do
                {
                        cc=isread(fdparam,(char *)&paczpaem,ISEQUAL);
                } while((cc != 0) && (iserrno == ELOCKED));
                if (cc == 0)
                {
                        f_printf(stderr," bin = %.9s",paczpaem.paem_codemet  );
                        isunlock(fdparam);
			return ( SUCCESS );
                }
               isunlock(fdparam);
        }
        return( ECHEC );
}
 #endif        
