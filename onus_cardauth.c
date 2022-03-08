
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_cardauth.c                                     *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 14-Feb-2000                                         *
 * Description         : ONUS Card authorization related functions           *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   11-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2                                                                         *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

/* Chowdesha added on 05-Oct-2011 for Fees calculation */
 #define PREPAID_CARD_FEES  1     

 #define CORP_DR_FLD51 1

/* Added for JK CMS OTBL Updation  */
 #define JK_CMS_APPLY_UPD 1     

/* Chowdesha added for HSBC DOP POS acquiring */
/* #define HSBC_DOP_POS  1  */

   #define SELECT_CITY 1
   #define PHOENIX_MALL 1 
   #define LODHA_PALAV 1 
   #define CARDLEVEL_AUTHTIME 1
   #define KOTAK_UPDATION   1

/*  Chowdesha added for CBA  */
 #define  CBA   1  
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

 #include <isam.h>
 #include <define.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pcrzbale.h>
 #include <paczpoau.h>
 #include <paczisoauto.h>
 #include <paczpa05.h>
 #include <paczau00.h>
 #include <paczoppo.h>
 #include <paczop00.h>
 #include <ath-trc.h>
 #include <onus.h>
 #ifdef KOTAK_UPDATION
 #include <kotak_limit.h>
 #include <time.h>
 #include <errno.h>
 #endif

#define KOTAK_CONFIGURABLE_MID 1

#ifdef KOTAK_CONFIGURABLE_MID
        extern rtrim(char *str);
#endif

 #include "onus_cardauth.h"

 #ifdef LODHA_PALAV
 extern       int             fdparam;
 #include <paczmlist.h>
 t_cardlimit cardlimit;
 #include <paczpaod.h>
 t_paczpaod paczpaod1;
 #endif


int DoPBank(char *bank_code);

/* Chowdesha added on 26-05-2011 for HSBC DOP Phase 2*/
/* #define HSBC_DOP_PHASE2  1 */

/* Chowdesha added on 12-Apr-2010 for PREPAID CARD */
  #define PREPAID_CARD  1   

/* Chowdesha addedd for GIFT CARD  */
#define GIFT_CARD  1

 #define SHELL_PHASE1 1 
 #define MCI_MONEY_SEND 1 

/* Added for decline report rectification requested by J&K bank */
 #define SUBRESULT_CODE_NEW     1

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;

extern	char	CODE_SCENARIO_O;
extern	char	User[];

extern t_paczisoauto	paczisoauto;
extern t_paczpa05		paczpa05;
extern t_pcrzbale		pcrzbale;
extern t_paczpoau		paczpoau;
extern t_paczpoau		dstpaczpoau;
extern t_ath_trc		athtrc;	
extern t_paczau00		paczau00;		/* Authorization Details */

/* ISAM Data File Descriptors */
extern double FltConvPercent;
extern double FltConvPercent_amt; 
static 	double		FltCardAmount = 0.0;
extern int		snFlagAlarm ; 

/* Function Prototypes */
extern int ConvertAmount ( char* ProductCode, double amount1, char* Currency1, \
 double* amoun2, char* currency2, char* date1, double* CurrenctRate1, \
 char* date2, double* CurrencyRate2 );

extern double atof ( char* );
extern int GetErrorCodeUsingReferalCode2 ( char ReferalCode, char* SubResult );
extern int CardAvailRefresh ( void );
extern int CashAvailRefresh ( void );
extern char* GetDateYYMMDD ();
extern char LOCAL_AMOUNT[13]; /* Azhakesh */

int is_country_club_limit(void);

#ifdef SHELL_PHASE1 
 char fld_54_shell[21];
#endif

#ifdef PREPAID_CARD 
	extern double  FltFeesAmount;
	extern  double	FltConvPercent; 
	extern double FltConvPercent_amt; 
	extern	int iGetFees(char *Card_nember, char *fees_amt1, int fees_field, char *atm_amtlimit);
#endif

#ifdef GIFT_CARD 
	extern double	FltGiftFeesAmount; 
#endif 

/* #ifdef HSBC_DOP_PHASE2 */
#ifdef  HSBC_DOP_POS 
extern double gcba_fees_amt; 
extern int DoPBank(char *bank_code);
#endif
#ifdef CBA
	extern    double gcba_fees_amt; 
	   extern char CBS_LOCAL_AMT [ ]; 
#endif 

#ifdef JK_CMS_APPLY_UPD
#include <onuspr05.h>
int             fdonusparam = -1;
extern t_onuspr05              onuspr05;
extern int  iUpdate_cardlevel_cardcash_available_amount(char *card_number, double damt1, double damt2, double damt3, double damt4, int iflag) ;
#endif

#ifdef SUBRESULT_CODE_NEW
      extern char *Expiry_Decline_code_return();
#endif







int is_country_club_limit(void)
{
	int country_club_limit = FALSE ;
	
	/* Check if the card is used in Andhra Bank terminal or not */
	/* Additional limit is given only if card is used on a AB terminal and the location is 
		country club */
	if( memcmp( paczisoauto.acquiring_institution_identification_code ,"450623", 6 ) == 0 )
	{
		/* Check if the location is a country club outlet or not */
		if( memcmp( paczisoauto.card_acceptor_identification_code, "000010255341297", 15 ) == 0 )
		{
			country_club_limit = TRUE ;
		}
	}
	return country_club_limit ;
}



#ifdef PREPAID_CARD 



int lcheckCardBlock(t_paczpoau *ppaczpoau)
{
	/*
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
	*/
	if( ppaczpoau->poau_act_card[0]== '1' )
		return ( ECHEC );
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
			GetErrorCodeUsingReferalCode ( ppaczpoau->poau_cd_appel[0], SubResult );
			return ( SUCCESS );
		}
		return ( SUCCESS );
	}
	/* Card Not Hotlisted  */
	return ( ECHEC );
}


int DoPBank(char *bank_code)
{
	if( memcmp( bank_code, "00010", 5 ) == 0 )
		return TRUE ;
	if( memcmp( bank_code, "00060", 5 ) == 0 )
		return TRUE ;
	return FALSE ;
}
#endif 


#ifdef JK_CMS_APPLY_UPD

int iUpdate_card_otbl()
{
		char cCardno[20], loccardavl[22];
	    int iRet ;
        double FltCardAvail = 0.0f; 
        double FltCashAvail = 0.0f;
		double DblCashLimit = 0.0f, DblCardLimit = 0.0f;
		char CardLimit [13];

		memset( cCardno, 0x00, sizeof(cCardno) );
		memset( CardLimit, 0x00, sizeof(CardLimit) );
		memset(loccardavl, 0x00, sizeof(loccardavl));
		iRet = 0 ;

         f_printf(stderr, "\n In iUpdate_card_otbl  Update");
		memset(&onuspr05, ' ' , sizeof(t_onuspr05));
        memcpy(onuspr05.table_code, "R05",    3);
        memcpy(onuspr05.bank_code, paczpa05.pa05_codbnq,    5);
		 if (isread(fdonusparam,&onuspr05,ISEQUAL) ==  0)
          {
			         isrelease(fdonusparam);
					 isunlock(fdonusparam);

				FPRINTF ( stderr, "Read Success ONUS PARAM \n" );
				if	(memcmp(onuspr05.otblup, "1", 1 )==0)
				{				
					memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_residuel_carte, 12); 
					FltCardAvail = atof(loccardavl);                
					f_printf(stderr, "\n Card available =[%.12s]", loccardavl) ;

                     memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_residuel_cash, 12); 
					FltCashAvail = atof(loccardavl);
					
					memcpy(CardLimit, paczpoau.poau_plafond_carte, 12); 
					DblCardLimit = atof (CardLimit);

					memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_plafond_cash, 12);
					DblCashLimit = atof (loccardavl);

	    			/* Update OTBL amount */
					memcpy( cCardno, paczpoau.poau_num_carte, 16 );
				
					f_printf(stderr, "\n \n Card Number=[%s]", cCardno);			
					f_printf(stderr, "\n FltCardAvail=[%f]", FltCardAvail) ;
					f_printf(stderr, "\n FltCashAvail=[%f]", FltCashAvail) ;
					f_printf(stderr, "\n  DblCardLimit = [%f]", DblCardLimit);
					f_printf(stderr, "\n DblCashLimit=[%f] \n ", DblCashLimit) ;

					snFlagAlarm = 0 ;
					alarm( 3 );
					iRet = iUpdate_cardlevel_cardcash_available_amount( cCardno, FltCardAvail, FltCashAvail,DblCardLimit, DblCashLimit,3 );
					alarm( 0 );

					if( iRet != 1 || snFlagAlarm != 0 )
						f_printf(stderr, "ONEVIEW DATABASE OTBL AMT UPDATE FAILED FOR CARD=[%s] ERROR=%d\n", cCardno, iRet );
				}
          }
		  else
			FPRINTF ( stderr, "Read failed ONUS PARAM \n" );

		    return SUCCES;
}

int iUpdate_card_otbl_addon_cards(t_paczpoau paczpoau)
{
		char cCardno[20], loccardavl[22];
	    int iRet ;
        double FltCardAvail = 0.0f; 
        double FltCashAvail = 0.0f;
		double DblCashLimit = 0.0f, DblCardLimit = 0.0f;
		char CardLimit [13];

		memset( cCardno, 0x00, sizeof(cCardno) );
		memset( CardLimit, 0x00, sizeof(CardLimit) );
		memset(loccardavl, 0x00, sizeof(loccardavl));
		iRet = 0 ;

         f_printf(stderr, "\n In iUpdate_card_otbl  Update");
		memset(&onuspr05, ' ' , sizeof(t_onuspr05));
        memcpy(onuspr05.table_code, "R05",    3);
        memcpy(onuspr05.bank_code, paczpa05.pa05_codbnq,    5);
		 if (isread(fdonusparam,&onuspr05,ISEQUAL) ==  0)
          {
			         isrelease(fdonusparam);
					 isunlock(fdonusparam);

				FPRINTF ( stderr, "Read Success ONUS PARAM \n" );
				if	(memcmp(onuspr05.otblup, "1", 1 )==0)
				{				
					memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_residuel_carte, 12); 
					FltCardAvail = atof(loccardavl);                
					f_printf(stderr, "\n Card available =[%.12s]", loccardavl) ;

                     memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_residuel_cash, 12); 
					FltCashAvail = atof(loccardavl);
					
					memcpy(CardLimit, paczpoau.poau_plafond_carte, 12); 
					DblCardLimit = atof (CardLimit);

					memset(loccardavl, 0x00, sizeof(loccardavl));
					memcpy(loccardavl, paczpoau.poau_plafond_cash, 12);
					DblCashLimit = atof (loccardavl);

	    			/* Update OTBL amount */
					memcpy( cCardno, paczpoau.poau_num_carte, 16 );
				
					f_printf(stderr, "\n \n Card Number=[%s]", cCardno);			
					f_printf(stderr, "\n FltCardAvail=[%f]", FltCardAvail) ;
					f_printf(stderr, "\n FltCashAvail=[%f]", FltCashAvail) ;
					f_printf(stderr, "\n  DblCardLimit = [%f]", DblCardLimit);
					f_printf(stderr, "\n DblCashLimit=[%f] \n ", DblCashLimit) ;

					snFlagAlarm = 0 ;
					alarm( 3 );
					iRet = iUpdate_cardlevel_cardcash_available_amount( cCardno, FltCardAvail, FltCashAvail,DblCardLimit, DblCashLimit,3 );
					alarm( 0 );

					if( iRet != 1 || snFlagAlarm != 0 )
						f_printf(stderr, "ONEVIEW DATABASE OTBL AMT UPDATE FAILED FOR CARD=[%s] ERROR=%d\n", cCardno, iRet );
				}
          }
		  else
			FPRINTF ( stderr, "Read failed ONUS PARAM \n" );

		    return SUCCES;
}
#endif 

#ifdef LODHA_PALAV
char *Card_Card_Limit(int Type)
{
         f_printf(stderr,"CARD TO CARD LIMIT\n");
         int 		fdlist = -1 ;
         char 		file_to_open[100];
         char 		Monthly_Balance[13];
         char 		Card_Per_txn[13];
         char 		Transaction_Amount[13];
         char 		Monthly_Otbl_Card[13];
         char 		Date_Month_Year[9];
         double 	Card_Per_Txn_Limit =  0.0 ;
         double 	Txn_Amount =  0.0 ;
         double 	Monthly_Otbl =  0.0 ;
         
         memset(file_to_open,0x00,sizeof(file_to_open));
         memset(Monthly_Balance,0x00,sizeof(Monthly_Balance));
         memset(Transaction_Amount,0x00,sizeof(Transaction_Amount));
         memset(Card_Per_txn,0x00,sizeof(Card_Per_txn));

         memset(Monthly_Otbl_Card,0x00,sizeof(Monthly_Otbl_Card));
         memset(Date_Month_Year,0x00,sizeof(Date_Month_Year));
         memcpy(Date_Month_Year,(char *)GetDateDDMMYYYY(),8);

         memcpy(Date_Month_Year+0,"01",2);
         sprintf( file_to_open, "%s/FC-CARDLIMIT", getenv("MultipacDat"));
         fdlist= isopen( file_to_open, ISINOUT+ISMANULOCK );
         if(fdlist < 0)
         {
                  f_printf(stderr,"File FC-CARDLIMIT Open Error , iserrno [%d]\n",iserrno);
                  isclose(fdlist);
                  return ( "961002" );
         }

         memset(&cardlimit, ' ', sizeof(t_cardlimit));
         memcpy(cardlimit.table_code, "04", 2);
         memcpy(cardlimit.mlist_bank_code,paczpoau.poau_cd_banque,   5);

         memcpy(cardlimit.card_number, paczpoau.poau_num_carte , 19);
         memcpy(cardlimit.processing_code,paczisoauto.processing_code , 2);
         memcpy(cardlimit.filler,"    ",4);

         if(isread(fdlist, (char *) &cardlimit, ISEQUAL) == 0)
         {
           	f_printf(stderr,"PER TXN LIMIT [%.12s]\n",cardlimit.card_per_txn_lmt);
           	f_printf(stderr,"MONTHLY TXN LIMIT [%.12s]\n",cardlimit.card_monthly_limit);
           	f_printf(stderr,"MONTHLY TXN OTBL  [%.12s]\n",cardlimit.card_monthly_otbl);

           	f_printf(stderr,"Card Record [%.150s]\n",&cardlimit);

           	if(memcmp(cardlimit.card_refrsh_date,Date_Month_Year,8) != 0 )
           	{
                        memset((char *)&paczpaod1,' ',             sizeof(t_paczpaod));
                        memcpy(paczpaod1.paod_notabl,     "OD",                     2);
                       /* memcpy(paczpaod1.paod_bin,paczisoauto.primary_account_number, 6);*/
                        memcpy(paczpaod1.paod_bin,paczpa05.pa05_codemet, 9);

                        memcpy(paczpaod1.paod_suitecle,"                     ",  21);
                        if(isread(fdparam,(char *)&paczpaod1,ISEQUAL)< 0 )
                        {
                              f_printf(stderr,"Read Error , iserrno [%d]\n",iserrno);
                              isclose(fdlist);
                              return ("961002");

                        }
                        isrelease(fdparam);
                        isunlock (fdparam);

                        if(memcmp(paczpaod1.paod_card_per_txn_lmt,cardlimit.card_per_txn_lmt,12) != 0 )
             			memcpy(cardlimit.card_per_txn_lmt,paczpaod1.paod_card_per_txn_lmt,12);

                        if(memcmp(paczpaod1.paod_montly_lmt,cardlimit.card_monthly_limit,12) == 0 ) 
             			memcpy(cardlimit.card_monthly_otbl,cardlimit.card_monthly_limit,12);
                        else
                        {
             			memcpy(cardlimit.card_monthly_limit,paczpaod1.paod_montly_lmt,12);
             			memcpy(cardlimit.card_monthly_otbl,cardlimit.card_monthly_limit,12);
                        }
                        
                        if( memcmp(cardlimit.card_per_txn_lmt,paczpaod1.paod_card_per_txn_lmt, 12) != 0)
             			memcpy(cardlimit.card_per_txn_lmt,paczpaod1.paod_card_per_txn_lmt,12);

             		memcpy(cardlimit.card_refrsh_date,Date_Month_Year,8);
             		isrewrite ( fdlist, (char*)&cardlimit);
           	}

           	f_printf(stderr,"REFRESH:MONTHLY TXN OTBL  [%.12s]\n",cardlimit.card_monthly_otbl);

           	memcpy(Card_Per_txn,cardlimit.card_per_txn_lmt,12);
           	memcpy(Transaction_Amount,paczisoauto.amount_transaction,12);
           	memcpy(Monthly_Otbl_Card,cardlimit.card_monthly_otbl,12);
         
           	Card_Per_Txn_Limit = atof(Card_Per_txn);
           	Txn_Amount         = atof(Transaction_Amount);
           	Monthly_Otbl       = atof(Monthly_Otbl_Card);

           	f_printf(stderr,"MONTHLY OTBL [%f]\n",Monthly_Otbl);
           	f_printf(stderr,"TXN AMOUNT  [%f]\n",Txn_Amount);
           	f_printf(stderr,"PER TXN LIMIT  [%f]\n",Card_Per_Txn_Limit);
           
           	if(Type == 1) /*Update OTBL Before Sending the Response */
           	{
             		sprintf(Monthly_Balance,"%012.0f",(Monthly_Otbl - Txn_Amount));
             		memcpy(cardlimit.card_monthly_otbl,Monthly_Balance,12);
            		isrewrite ( fdlist, (char*)&cardlimit);
             		isclose(fdlist);
             		return ("000000");

           	}

           	if(Type == 2) /* Update OTBL For Reversal Transaction */
           	{
             		sprintf(Monthly_Balance,"%012.0f",(Monthly_Otbl + Txn_Amount));
             		memcpy(cardlimit.card_monthly_otbl,Monthly_Balance,12);
            		isrewrite ( fdlist, (char*)&cardlimit);
             		isclose(fdlist);
             		return ("000000");

           	}
          
          	if(Txn_Amount > Card_Per_Txn_Limit)
           	{
             		f_printf(stderr,"EXCEEDED PER TXN LIMIT [%.12s],TXN AMOUNT [%.12s]\n",cardlimit.card_per_txn_lmt,paczisoauto.amount_transaction);
             		isclose(fdlist);
             		return ("121002");
           	}
           
          	if((Monthly_Otbl - Txn_Amount) < 0.0 )
           	{
             		f_printf(stderr,"EXCEEDED PER MONTH LIMIT [%.12s],TXN AMOUNT [%.12s]\n",cardlimit.card_monthly_otbl,paczisoauto.amount_transaction);
             		isclose(fdlist);
             		return ("121003");
           	}

         }
         else
         {
                f_printf(stderr,"Record not present in the Master File, iserrno [%d]\n",iserrno);
                if(iserrno == ENOREC)
                {
               	      	memset((char *)&paczpaod1,' ',             sizeof(t_paczpaod));
                    	memcpy(paczpaod1.paod_notabl,     "OD",                     2);
                     	memcpy(paczpaod1.paod_bin,paczpa05.pa05_codemet, 9);
              
                     	memcpy(paczpaod1.paod_suitecle,"                     ",  21);
                        f_printf(stderr,"Record [%.40s]\n",&paczpaod1);

                     	if(isread(fdparam,(char *)&paczpaod1,ISEQUAL)< 0 )
                     	{
                              f_printf(stderr,"Read Error , iserrno [%d]\n",iserrno);
                              isclose(fdlist);
                              return ("961002");

                     	}

                     	isrelease(fdparam);
                     	isunlock (fdparam);

                     	memset(&cardlimit, ' ', sizeof(t_cardlimit));
                     	memcpy(cardlimit.table_code, "04", 2);
                     	memcpy(cardlimit.mlist_bank_code,paczpoau.poau_cd_banque,   5);
                     	memcpy(cardlimit.card_number, paczpoau.poau_num_carte , 19);

                        memcpy(cardlimit.processing_code,paczisoauto.processing_code , 2);
                        memcpy(cardlimit.filler,"    ",4);
                        memcpy(cardlimit.filler1,"             ",13);

                        memcpy(cardlimit.card_per_txn_lmt,paczpaod1.paod_card_per_txn_lmt, 12);
                     	f_printf(stderr,"Per Trxn Limit  [%.12s]\n",paczpaod1.paod_card_per_txn_lmt);
                     	memcpy(cardlimit.card_daily_limit,paczpaod1.paod_card_daily_lmt, 12);
                     	memcpy(cardlimit.card_weekly_limit,paczpaod1.paod_card_week_lmt, 12);

                     	memcpy(cardlimit.card_monthly_limit,paczpaod1.paod_montly_lmt, 12);

                     	memcpy(cardlimit.card_daily_otbl,paczpaod1.paod_card_daily_lmt,12);
                     	f_printf(stderr,"Monthly Limit  [%.12s]\n",paczpaod1.paod_montly_lmt);
                     	memcpy(cardlimit.card_weekly_otbl,paczpaod1.paod_card_week_lmt,12);

                     	memcpy(cardlimit.card_monthly_otbl,paczpaod1.paod_montly_lmt, 12);
                     	memcpy(cardlimit.card_refrsh_date,(char *)GetDateDDMMYYYY(),8);
                     	memcpy(cardlimit.card_refrsh_date+0,"01",2);
                
                    	f_printf(stderr,"Card Record [%.150s]\n",&cardlimit);

                     	if(iswrite( fdlist, (char*)&cardlimit)< 0)
                     		f_printf(stderr,"write Error, iserrno [%d]\n",iserrno);
                     	else
                     		f_printf(stderr,"write Success\n");
                  }
                  else
                  {
                   	f_printf(stderr,"Record not present in the Master File, iserrno [%d]\n",iserrno);
                   	isclose(fdlist);
                   	return ("961002");
                  }
           }

   isclose(fdlist);
   return ("000000");
}
#endif


#ifdef LODHA_PALAV
#ifdef KOTAK_UPDATION
int GetBankAccountNumber(char *Bank_Account,char *crn_no,char *checksum)
{
         t_kotakaccount kotakaccount ;
         f_printf(stderr,"GET KOTAK ACCOUNT\n");
         int            fdcardlimit = -1 ;
         char           file_to_open[100];

         memset(file_to_open,0x00,sizeof(file_to_open));

         sprintf( file_to_open, "%s/FC-CARDLIMIT", getenv("MultipacDat"));
         fdcardlimit = isopen( file_to_open, ISINOUT+ISMANULOCK );
         if(fdcardlimit < 0)
         {
                  f_printf(stderr,"File FC-CARDLIMIT Open Error , iserrno [%d]\n",iserrno);
                  isclose(fdcardlimit);
                  return(ECHEC);
         }
         memset(&kotakaccount, ' ', sizeof(t_kotakaccount));
         memcpy(kotakaccount.table_code, "05", 2);
         memcpy(kotakaccount.bank_code,paczpoau.poau_cd_banque,   5);
         memcpy(kotakaccount.card_type, paczpoau.poau_type_carte, 2);
         memcpy(kotakaccount.filler,"                       ",23);
         if(isread(fdcardlimit, (char *) &kotakaccount, ISEQUAL) == 0)
         {
            f_printf(stderr,"ACCOUNT NUMBER [%.20s]\n",kotakaccount.Account_number);
            memcpy(Bank_Account,kotakaccount.Account_number,20);
            memcpy(crn_no,kotakaccount.Crn_No,9);
            memcpy(checksum,kotakaccount.Checksum_Key,20);
            f_printf(stderr,"CRN NO[%.11s]\n",kotakaccount.Crn_No);
            f_printf(stderr,"CHECK SUM [%.20s]\n",kotakaccount.Checksum_Key);
            f_printf(stderr,"CRN NO[%.11s]\n",crn_no);
            f_printf(stderr,"CHECK SUM [%.20s]\n",checksum);
         }
         else
         {
           isclose(fdcardlimit);
           return(ECHEC);
         }
        isrelease(fdcardlimit);
        isunlock(fdcardlimit);
        isclose(fdcardlimit);
        return(SUCCESS);
}

int Check_Bank_Level_Reload_Limit()
{
  int Return_Value = -1;
  char resp_code[3];
  char Bank_Account[21];
  char CRN_NO[20];
  char Checksum_Key[20];
  if( memcmp ( paczisoauto.processing_code, "20", 2 ) != 0)
  return(SUCCESS);

  if(( memcmp( paczpa05.pa05_codbnq,"00087",5) == 0 )
     #ifdef SELECT_CITY
     ||( memcmp( paczpa05.pa05_codbnq,"00093",5) == 0 )
     ||( memcmp( paczpa05.pa05_codbnq,"00098",5) == 0 )
     #endif
    )
  f_printf(stderr,"\n Bank_Level_Reload_Limit Check Allowed");
  else
  return(SUCCESS);

  memset(Bank_Account,0x00,sizeof(Bank_Account));
  memset(CRN_NO,0x00,sizeof(CRN_NO));
  memset(Checksum_Key,0x00,sizeof(Checksum_Key));
  Return_Value = GetBankAccountNumber(Bank_Account,CRN_NO,Checksum_Key);
  if(Return_Value == ECHEC)
  {
        f_printf(stderr,"\n  UNABLE TO GET THE KOTAK ACCOUNT NUMBER");
        return(ECHEC);
  }


        char kotak_bank[6];
        char kotak_card_type[3];
        char kotak_merchant[16];

        memcpy(kotak_bank, paczpoau.poau_cd_banque, 5); kotak_bank[5]='\0';
        memcpy(kotak_card_type, paczpoau.poau_type_carte, 2); kotak_card_type[2]='\0';

        if(get_merchant_id(kotak_bank, kotak_card_type, kotak_merchant)<0)
        {
                f_printf(stderr, "Error in getting MerchantID for Bank: [%s], Type: [%s]\n",
                        kotak_bank, kotak_card_type);
                        return(ECHEC);
        }


   f_printf(stderr,"Response code from kotak  \"%.2s\"\n",resp_code);
  if(memcmp(resp_code,"91",2)==0)
  {
        f_printf(stderr,"\n  Network Issue Unable To Connect  Kotak API");
        return(ECHEC);
  }
  else if (memcmp(resp_code,"92",2)==0)
  {
        f_printf(stderr,"\n  Time Out Case---- Response Not Received from Kotak API");
        Update_Bank_Level_Reload_Limit(400);
        return(ECHEC);
  }
  else if (memcmp(resp_code,"93",2)==0)
  {
        f_printf(stderr,"\n PROBLEM IN HTTPS CONNECTION  ESTABLISHMENT WITH   Kotak API");
        return(ECHEC);
  }
  else if (memcmp(resp_code,"05",2)==0)
  {
        f_printf(stderr,"\n Reload Request Rejected by Kotak Server");
        return(ECHEC);
  }
  else if (memcmp(resp_code,"00",2)==0)
  {
        f_printf(stderr,"\n Reload Request Accepted by Kotak Server");
        return(SUCCESS);
  }

return(ECHEC);
}
#endif


#ifdef KOTAK_CONFIGURABLE_MID
int get_merchant_id(char *bank, char *type, char *mid)
{
	int fdcardlimit;
	char filename[1204];
	t_kotakaccount kotakaccount;

	sprintf(filename, "%s/FC-CARDLIMIT", getenv("MultipacDat"));
	if((fdcardlimit = isopen(filename, ISINOUT+ISMANULOCK))<0)
	{
		f_printf(stderr, "Error in opening [%s]\n", filename);
		return -1;
	}

	memset(&kotakaccount, ' ', sizeof(kotakaccount));
	memcpy(kotakaccount.table_code, "05", 2);
	memcpy(kotakaccount.bank_code, bank, 5);
	memcpy(kotakaccount.card_type, type, 2);

	if(isread(fdcardlimit, &kotakaccount, ISEQUAL)<0)
	{
		f_printf(stderr, "Error in reading record for bank:[%s], cardtype:[%s]\n", bank, type);
		isclose(fdcardlimit);
		return -1;
	}

	memcpy(mid, kotakaccount.Merchant_Code, 15);
	mid[15]='\0';
	rtrim(mid);

	isclose(fdcardlimit);
	return 0;
}
#endif

#ifndef KOTAK_UPDATION
int Check_Bank_Level_Reload_Limit()
{

  char Daily_Reload_Limit[13];
  char Daily_Reload_Otbl [13];
  char transaction_amount [13];
  char Date_Month_Year[9];
  

  double   int_TxnAmount = 0.0;
  double   Reload_Avail_Otbl = 0.0;
  double   Reload_Limit = 0.0;
  f_printf(stderr,"\n Check_Bank_Level_Reload_Limit");

  memset(Daily_Reload_Limit,0x00,sizeof(Daily_Reload_Limit));
  memset(Daily_Reload_Otbl,0x00,sizeof(Daily_Reload_Otbl));
  memset(transaction_amount,0x00,sizeof(transaction_amount));

  memset(Date_Month_Year,0x00,sizeof(Date_Month_Year));
  memcpy(Date_Month_Year,(char *)GetDateDDMMYYYY(),8);

  if( memcmp ( paczisoauto.processing_code, "20", 2 ) != 0)
  return(SUCCESS);

  if(( memcmp( paczpa05.pa05_codbnq,"00087",5) == 0 )
     #ifdef SELECT_CITY
     ||( memcmp( paczpa05.pa05_codbnq,"00093",5) == 0 )
     #endif
     #ifdef  PHOENIX_MALL
     ||(memcmp ( paczpa05.pa05_codbnq, "00098", 5 ) == 0)
     #endif
    )
  f_printf(stderr,"\n Bank_Level_Reload_Limit Check Allowed");
  else
  return(SUCCESS);
  
/*
  if(memcmp(Date_Month_Year,onuspr05.refresh_date,8) != 0 )
   {
     	f_printf(stderr,"\n 1 . Reload OTBL [%.12s]and Refresh Date [%.8s] Before Refreshed at Bank level ",onuspr05.reload_daily_otbl,onuspr05.refresh_date);
     	memcpy(onuspr05.refresh_date,Date_Month_Year,8);
     	memcpy(onuspr05.reload_daily_otbl,onuspr05.reload_daily_limit ,12);
     	f_printf(stderr,"\n 2 .Reload OTBL [%.12s]and Refresh Date [%.8s] After Refreshed at Bank level ",onuspr05.reload_daily_otbl,onuspr05.refresh_date);
     	if( isrewrite(fdonusparam,(char *)&onuspr05) < 0 )
     	{
       		f_printf(stderr,"\n Error In rewriting the  onuspr05 Record");
       		return(ECHEC);
     	}
     	else
     	f_printf(stderr,"\n RELOAD OTBL and REFRESH DATE REFRESHED\n");

      isrelease(fdonusparam);
      isunlock(fdonusparam);

   }

*/

     

  memcpy( Daily_Reload_Otbl,onuspr05.reload_daily_otbl ,12);
  memcpy( Daily_Reload_Limit,onuspr05.reload_daily_limit ,12);
  memcpy( transaction_amount,paczisoauto.amount_transaction ,12);
 

 int_TxnAmount = atof(transaction_amount);

 Reload_Avail_Otbl = atof(Daily_Reload_Otbl);
 Reload_Limit      = atof(Daily_Reload_Limit);

        f_printf(stderr,"\n Transaction Amount [%f]",int_TxnAmount);
        f_printf(stderr,"\n Reload OTBL Avialable [%f]",Reload_Avail_Otbl);
        f_printf(stderr,"\n Reload Limit  [%f]",Reload_Limit);

 if(int_TxnAmount > Reload_Avail_Otbl)
 {
        f_printf(stderr,"\n Transaction Amount is Greater then the Available Reload OTBL");
        return(ECHEC);


 }

 return(SUCCESS);

}
#endif

#ifndef KOTAK_UPDATION

int Update_Bank_Level_Reload_Limit(int Messagetype)
{
  char Daily_Reload_Limit[13];
  char Daily_Reload_Otbl [13];
  char transaction_amount[13];

  double   int_TxnAmount = 0.0;
  double   Reload_Avail_Otbl = 0.0;
  double   Reload_Limit = 0.0;
  f_printf(stderr,"\n Update_Bank_Level_Reload_Limit");

  memset(Daily_Reload_Limit,0x00,sizeof(Daily_Reload_Limit));
  memset(Daily_Reload_Otbl,0x00,sizeof(Daily_Reload_Otbl));
  memset(transaction_amount,0x00,sizeof(transaction_amount));
  
  memcpy( Daily_Reload_Otbl,onuspr05.reload_daily_otbl,12);
  memcpy( Daily_Reload_Limit,onuspr05.reload_daily_limit ,12);
  memcpy( transaction_amount,paczisoauto.amount_transaction ,12);
 

 if( memcmp ( paczisoauto.processing_code, "20", 2 ) != 0)
 return(SUCCESS);

  if(( memcmp( paczpa05.pa05_codbnq,"00087",5) == 0 )
     #ifdef SELECT_CITY
     ||( memcmp( paczpa05.pa05_codbnq,"00093",5) == 0 )
     #endif
     #ifdef  PHOENIX_MALL
     ||(memcmp ( paczpa05.pa05_codbnq, "00098", 5 ) == 0)
     #endif
    )
  f_printf(stderr,"\n Bank_Level_Reload_Limit Update Allowed");
  else
  return(SUCCESS);

 f_printf(stderr,"\n Update_Bank_Level_Reload_Limit [MTI : %d  AMOUNT [%.12s]]\n",Messagetype,transaction_amount);

 int_TxnAmount = atof(transaction_amount);

 Reload_Avail_Otbl = atof(Daily_Reload_Otbl);

 if(Messagetype == 400)
 {
   Reload_Avail_Otbl = Reload_Avail_Otbl + int_TxnAmount;
   f_printf(stderr,"\n REVERSAL RELOAD OTBL UPDATION\n");
 }
 else
 Reload_Avail_Otbl = Reload_Avail_Otbl - int_TxnAmount;

 sprintf(Daily_Reload_Otbl,"%012.0f",Reload_Avail_Otbl );

 memcpy(onuspr05.reload_daily_otbl,Daily_Reload_Otbl,12);

 if( isrewrite(fdonusparam,(char *)&onuspr05) < 0 )
 {
        f_printf(stderr,"\n Error In rewriting the Record");
        return(ECHEC);
 }
 else
        f_printf(stderr,"\n RELOAD OTBL UPDATED SUCCESSFULLY\n");


 isrelease(fdonusparam);
 isunlock(fdonusparam);

 return(SUCCESS);

}
#endif

#ifdef KOTAK_UPDATION
int Update_Bank_Level_Reload_Limit(int Messagetype)
{
   int Return_Value = -1;
   char Bank_Account[21];
   int klimit = -1;
   char file_name[100];
   char Time_Gmt[15];
   char CRN_NO[20];
   char Checksum_Key[20];

  f_printf(stderr,"\n Update_Bank_Level_Reload_Limit");
  if( memcmp ( paczisoauto.processing_code, "20", 2 ) != 0)
  return(SUCCESS);
 if(( memcmp( paczpa05.pa05_codbnq,"00087",5) == 0 )
     #ifdef SELECT_CITY
     ||( memcmp( paczpa05.pa05_codbnq,"00093",5) == 0 )
     #endif
     #ifdef  PHOENIX_MALL
     ||(memcmp ( paczpa05.pa05_codbnq, "00098", 5 ) == 0)
     #endif
    )
  f_printf(stderr,"\n Bank_Level_Reload_Limit Update Allowed");
  else
  return(SUCCESS);


  if(Messagetype  == 400)
  {
         t_kotaklimit kotaklimit;
         memset(Bank_Account,0x00,sizeof(Bank_Account));
         memset(CRN_NO,0x00,sizeof(CRN_NO));
         memset(Checksum_Key,0x00,sizeof(Checksum_Key));
         Return_Value = GetBankAccountNumber(Bank_Account,CRN_NO,Checksum_Key);
         if(Return_Value == ECHEC)
         {
                f_printf(stderr,"\n  UNABLE TO GET THE KOTAK ACCOUNT NUMBER");
                return(ECHEC);
         }

         memset(file_name,0x00,sizeof(file_name));
         memset(Time_Gmt,0x00,sizeof(Time_Gmt));
         memset((char *)&kotaklimit,0x20,sizeof(t_kotaklimit));
         sprintf(file_name,"%s/KOTAK-LIMIT",getenv("MultipacDat"));
         klimit = isopen(file_name,ISAUTOLOCK+ISINOUT);
         if (klimit < 0)
         {
                  f_printf(stderr,"MULTIXPAC : ERROR OPENING \'%s\' ",klimit);
                  f_printf(stderr,"ISERRNO : %d \n",iserrno);
                  return (ECHEC);
         }

         memcpy(kotaklimit.Bank_code,paczpoau.poau_cd_banque,5);
         memcpy(kotaklimit.Card_Number,paczpoau.poau_num_carte,16);
         memcpy(kotaklimit.Reference,paczisoauto.retrieval_reference_number,12);
         memcpy(kotaklimit.Processing_Code,paczisoauto.processing_code,2);
         memcpy(kotaklimit.filler_1,"            ",12);
         memcpy(kotaklimit.Message_Code,"9950",4);
         memcpy(kotaklimit.Transaction_Amount,paczisoauto.amount_transaction,12);
         memcpy(kotaklimit.Gmt_Date_Time,Time_Gmt,14);
         memcpy(kotaklimit.Bank_Account_No,Bank_Account,strlen(Bank_Account));
         memcpy(kotaklimit.Currency,paczpoau.poau_cd_monnaie,3);
         memcpy(kotaklimit.Merchant_Id,paczisoauto.card_acceptor_identification_code,15);
         memcpy(kotaklimit.Processed_Count,"0",1);
         memcpy(kotaklimit.Checksum_Key,Checksum_Key,sizeof(Checksum_Key));
         memcpy(kotaklimit.Crn_No,CRN_NO,sizeof(CRN_NO));

         if( iswrite( klimit, &kotaklimit) < 0 )
         {
                f_printf(stderr, "Unable to write to KOTAK-LIMIT file iserrno=%d \n", iserrno ) ;
                isclose(klimit);
                return (ECHEC);
         }

         isrelease(klimit);
         isunlock(klimit);
         isclose(klimit);
 }

 return(SUCCESS);

}
#endif
#endif

