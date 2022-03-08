
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_fnc.c                                          *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 06-Mar-2000                                         *
 * Description         : ONUS functions                                      *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   20-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   12-Apr-04 A K Azhakesh         CR-MXP-0401-0042                       *
 * 3   24-Feb-04 Veeresh       RTH-changes                                   *
 * 4   25-Feb-05  Michel    reviewed-OK                                      *
 *****************************************************************************/

/* Chowdesha/Preema added for IDBI BCHA EHOST  */
#define IDBI_BCHA_EHOST  1  
#define ADDON_PHASE_2

/* Chowdesha added for CBA */
#define CBA   1  

/*  Chowdesha added on 26-07-2011 for SYNDICATE bank act flag checking */
 #define SYNDICATE_ACT_FLAG  1  

/* Bharath added for APPLY JK*/
   #define JK_CMS_APPLY   1   
   #define SELECT_CITY    1
   #define PHOENIX_MALL   1 

 #define JK_CMS_APPLY_UPD 1  

  /* #define EXPIRY_CHECK_AVOID  1 */

   #define SHELL_PHASE1 1 
   #define SFLP 1

  #define SHELL_PHASE2 1    
  #define ADDON_CARDS_HOTLIST 1  
  #define INT_TRANS_LIMIT 1

 #define CNP_3ATTEMPT 1 
 #define CARD_PRODUCT_CODE  1
 #define MVISA  1

#define BOBL_VBV 1
#define VIJAYA_TRAVEL 1
#define CARD_CONTROL 1
#define  BFL_CUG_CARD     1
 #define  ADDON            1   
#define VISA_MCI_CNP_PARAMETER 1  

#define  LODHA_PALAV     1 
#define  BOBL_PREPAID    1
#define  BFL_PHASE2      1

/* #define  QR_ISSUANCE 1 */

/*changes done for ADVISE message as mutiple debit happening for card holder */
 #define DR_ADVICE 1

#define VIJAYA_PHASE_3 1

 #define RUPAY_ISSUANCE   1   /* Added by Amit for RUPAY Issuance on 13 july 2016 */

 #define SHELL_DECIMAL_RELOAD_ISSUE   1   /* Added by Amit for resolving reload decimal value issue on 27 Mar 2017*/

 #define EDENRED_MOBILE_TXN 1  /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */

#define LODHA_DB_26SEPT 1

#ifdef LODHA_DB_26SEPT
 #include <iso_visa.h>
 #include <reloadtransactions.h>
 t_reloadtransactions reloadtxn;
#endif



 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>

 #include <signal.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <memory.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <isam.h>
 #include <paczpa00.h>
 #include <paczpa02.h>
 #include <paczpa03.h>
 #include <paczpa05.h>
 #include <paczpa11.h>
 #include <paczpa56.h>
 #include <paczpa21.h>
 #include <paczpoau.h>
 #include <paczoppo.h>
 #include <paczop00.h>
 #include <pacznf00.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczeuro.h>
 #include <paczauto.h>
 #include <paczisoauto.h>
 #include <paczautr.h>
 #include <paczport.h>
 #include <pcrzbcli.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <visacq03.h>
 #include <visacq04.h>
 #include <define.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <bplinfo.h>
 #include <paczrescntry.h>
 #include <paczrescity.h>
 #include <onus.h>
 #include <paczmlist.h>
 #include "onus_fnc.h"

 #ifdef BFL_CUG_CARD
 #include <bflzbcom.h>
 #include <bflzport.h>
 #include <bflzbcli.h>
 #include <bflzparm.h>
 #include <bfl_rthauth.h>
 #include <bflauth00.h>
 #include <bflhistoryauth.h>
 #include <paczpa1b.h>
 #include <bfl_dealer_master.h>
 #include <bflacqchn.h>
 #include <bflzaddon.h>
 #include <bflcarddet.h>

t_bflzlpa03   bflzlpa03;
t_bflzlpa20   bflzlpa20;
t_paczpa1b    paczpa1b;
t_bflzlpa02   bflzlpa02;
t_lnaddport   lnaddport;
t_bflcarddet  bflcarddet;
extern t_bflauth00   bflauth00;
extern t_bflrthauth  bflrthauth;
extern t_bflzport    bflzport;
extern t_bflzbcom    bflzbcom;
extern t_bflzbcli    bflzbcli;
extern t_bflhistauth bflhistauth;
extern t_bfl_dealer_master bfl_dealer_master;
extern int lnbcli ; /*BFL CARD Client file */
extern int lnbcom ; /*BFL CARD ACCOUNT MASTER */
extern int lnporte ;/*BFL CARD MASTER */
extern int lnsauto; /*BFL AUTHORISATION */
extern int lnhauto; /*BFL AUTHORISATION HISTORY */
extern int lnparam; /*BFL PARAMETERS*/
extern int lnrth; /* BFL-RTH FILE*/
extern int Same_Merchant;
int lncrd;
int BFL_GetCardType( char* , char* , char* );
int CheckCoBrandCard();
int Card_Embo_Date_Checking();
int OpenReadACQCHN();
int ReadAddonCard();
int ReadAddOnFile(char *, char *);
int lnacqch = -1;
int lnaddonporte = -1;

extern char RRN_NO[];

#ifdef BFL_PHASE2
extern char   parent_rrn[];
extern char   Limit_Before_Void[];
extern char   Limit_After_Void[];
char   acquiring_channel[3];
#endif
#endif

#ifdef SHELL_PHASE2
 #include <cardmccparam.h>
 #endif

#ifdef  SHELL_PHASE1 
#include <onuspr05.h>
extern t_onuspr05              onuspr05;
extern int fdonusparam;

#endif


/* Chowdesha added on 26-05-2011 for HSBC DOP Phase 2*/
/* #define HSBC_DOP_PHASE2  1 */

/* Chowdesha added for the CSL Comments  */
#define CSL_PROJECT

/* Added for IDBI BCHAA */
  #define IDBI_BCHAA  1   


/* Chowdesha added for INSTALLMENT PAYMENT */
#define INSTALLPAYMENT    1  
 #define STIP_PARAM        1   

/* Added for decline report rectification requested by J&K bank */
 #define SUBRESULT_CODE_NEW     1


extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;

extern t_paczisoauto	paczisoauto;
extern t_paczpa05		paczpa05;
extern t_paczpoau		paczpoau;
extern t_pcrzbale		pcrzbale;
extern t_paczpa02		paczpa02;
extern t_rthauth		rthauth;

t_paczport paczport;
#ifdef JK_CMS_APPLY

 #include <pcrzbcom.h>
extern  t_pcrzbcom pcrzbcom;
 
char * cGetTimeYYYYMMDDHHMMSS(void );
#endif
 #ifdef SHELL_PHASE1
 
 t_paczoppo paczoppo_shell ;
  t_paczoppo paczoppo_sh ;
 #endif

/* SCENARIO CODES */
extern char	CODE_SCENARIO_I;
extern char	CODE_SCENARIO_A;
extern char	CODE_SCENARIO_O;

/* ISAM Data File Descriptors */
extern int		fdparam;	/* Parameter file */
/*******extern int		fdnf00;		 Common Negative file */

/* Function Prototypes */
extern char* GetDateYYMMDD ( void );
extern char* GetDateYYYYMMDD ( void );
extern char* GetTime ( void );
extern double atof ( char* );
extern int InternationalCard ( char CardUse );
#ifdef RUPAY_ISSUANCE   /* Added by Amit for RUPAY Issuance on 30 NOV 2016 */
extern int DomestiCard ( char CardUse );
#else
/*
extern int DomestiCard ( char CardUse );
*/
#endif
extern int IsLocalUsed ( char* CountryCode, char* CodePays );
extern int GetErrorCodeUsingCaptureCode ( char CaptureCode, char* SubResult );
extern int GetErrorCodeUsingBlockCode ( char CaptureCode, char* SubResult );
extern int GetErrorCodeUsingReferalCode ( char ReferalCode, char* SubResult );

int GetTypeOfCard ( char* CardNumber, char* CardType );


/* Chowdesha added for PREPAID CARD */
 #define  PREPAID_CARD  1  

  /* Added to Implement Reload Slab for SHELL on 13.09.2012 */
  #define SHELL_RELOAD_SLAB       1 

#ifdef SHELL_RELOAD_SLAB
#include <paczpaod.h>
extern double gcba_fees_amt;
#endif
 #define MCI_MONEY_SEND 1 

#ifdef VIJAYA_TRAVEL
char current_date [9];
char last_auth_date[9];
int GetJulianDate();
#endif


#ifdef CNP_3ATTEMPT
int Check_CNP_Attempts(char );
int Check_Card_Not_Present(char );
int iCNP_Max_Attempts;
t_paczpa11  paczpa11;
#endif

#define MRL_287  1

#ifdef VISA_MCI_CNP_PARAMETER

#include <cnp_parameter.h>
t_cnp_transaction cnp_transaction;
t_cnp_visa_mci_parameter        cnp_visa_mci_parameter;

int Card_Not_Present_Transaction_Check(char ,char *);

int RBI_SMS_FLAG;

int Get_Mci_Ecom_Value(char  * );
char Pos_Data_Info_cnp[40];
#endif

#ifdef VIJAYA_PHASE_3
extern int Ucaf ;
#endif

#define CARD_RENEWAL_ACT 1

#ifdef SUBRESULT_CODE_NEW
  int    gExpiry_validate_senario;        /*Values - 0 No error                          */
                                          /*       - 1 Expired Card                      */
                                          /*       - 2 Expiry Mismatch                   */
                                          /*       - 3 Error reading Record in FC-PORTE  */
                                          /*       - 4 Error updating Record in FC-PORTE */
                                          /*       - 5 Error updating Record in FC-POAU  */
                                          /*       - 6 Deactivated Card                  */
 #endif


#ifdef RUPAY_ISSUANCE   /* Added by Amit for RUPAY Issuance on 07 NOV 2016 */

void GetRupayIssCardType(char* IssCardType)
{
        memset ( IssCardType, 0, sizeof(IssCardType) );
        f_printf(stderr,"\nBank Code:[%.5s],Card Type[%.2s]",paczpa05.pa05_codbnq,paczpoau.poau_type_carte);
        t_paczpa11  RUPAY_paczpa11;
        memset(&RUPAY_paczpa11, ' ', sizeof(RUPAY_paczpa11));
        memcpy(RUPAY_paczpa11.pa11_notabl, "11", 2);
        memcpy(RUPAY_paczpa11.pa11_banque , paczpa05.pa05_codbnq, 5);
        memcpy(RUPAY_paczpa11.pa11_type_carte, paczpoau.poau_type_carte, 2);

        if(isread(fdparam, (char *) &RUPAY_paczpa11, ISEQUAL ) == 0)
        {
                memcpy ( IssCardType, RUPAY_paczpa11.pa11_card_issuer_code, 1 );
                f_printf(stderr, "\nCard 1st [%.1s] ,IssCardType [%.1s]\n",paczpoau.poau_num_carte,IssCardType);
                isrelease ( fdparam );
                isunlock ( fdparam );
        }
        else
        {
                f_printf(stderr, "\n RUPAY_paczpa11 - Record read Error [%d]",  iserrno);
                isrelease ( fdparam );
                isunlock ( fdparam );
        }
}
#endif

int LoadKeys ( char* BankCode, char CardFirstDigit, char pvkIndex )
{
    char   Index[3];
    int	   TempVal = 0;

    memset ( Index, 0, 3 );
    f_printf(stderr,"\n Keys Loading Of The Bank = {%.5s} (%c)\n", \
              BankCode, pvkIndex );
    TempVal = pvkIndex - '0';                                                   
    if ( memcmp ( BankCode, "00017", 5 ) == 0 )
    {
       TempVal = 0;                                                         
       sprintf ( Index , "%02d", TempVal );                                 
       f_printf(stderr,"\n d Index to be loaded is %.2s \n\n", Index );
		charger_cles(BankCode,Index,'V','I');                              
    }
    /******Saju . 21/01/04 for WE       *********/
    /* First digit is added by ACR, 27-10-2010 */
    else if ( CardFirstDigit == '4' || CardFirstDigit == '1' || CardFirstDigit == '7' 
	     #ifdef IDBI_BCHAA
	     || CardFirstDigit == '6'
	     #endif
             #ifdef LODHA_PALAV
             || (CardFirstDigit == '9')
            #endif 
		 ) /* Muthoot - Michel */
    {
#ifdef RUPAY_ISSUANCE   /* Added by Amit for RUPAY Issuance on 25 AUG 2016 */
       if( paczisoauto.message_format == 'R' )
                TempVal = 50;
#endif
       sprintf ( Index , "%02d", TempVal );                                 
       f_printf(stderr,"\n d Index to be loaded is %.2s \n\n", Index );
       charger_cles(BankCode,Index,'V','I');                              
    }
    else if ( CardFirstDigit == '5' )
    {
       TempVal = TempVal + 6;
       sprintf ( Index , "%02d", TempVal );                                 
       f_printf(stderr,"\n d Index to be loaded is %.2s \n\n", Index );
		charger_cles(BankCode,Index,'M','I');                              
    }
    else if ( CardFirstDigit == '3' )
    {
       TempVal = 40;
       sprintf ( Index , "%02d", TempVal );                                 
       f_printf(stderr,"\n d Index to be loaded is %.2s \n\n", Index );
       charger_cles(BankCode,Index,'A','I');                              
    }
    else 
    {
       TempVal = TempVal + 12;
       sprintf ( Index , "%02d", TempVal );                                 
       f_printf(stderr,"\n d Index to be loaded is %.2s \n\n", Index );
       charger_cles(BankCode,Index,'P','I');                              
    }
    return ( SUCCESS );
}




int GetCurrency ( char* TxnCurrency, char* Amount, char* CardholderCurrency )
{
   char	TempAmount[13];
   memset ( Amount, 13, 0 );
   memset ( TxnCurrency, 4, 0 );
   memset ( TempAmount, 13, 0 );
   /* check field 5 CARD HOLDER BILLING AMOUNT IS PRESENT in PACKET */
   if ( strlen ( paczisoauto.amount_cardholder_billing ) != 0 )
   {
      memcpy ( Amount, paczisoauto.amount_cardholder_billing, 12 );
      memcpy ( TxnCurrency, paczisoauto.currency_code_cardholder_billing, 3 );
   }
   else if ( memcmp ( paczisoauto.currency_code_transaction, \
  		      CardholderCurrency, 3 ) == 0 )
   {
      memcpy ( Amount, paczisoauto.amount_transaction, 12 );
      memcpy ( TxnCurrency, paczisoauto.currency_code_transaction, 3 );
   }
   else if ( memcmp ( paczisoauto.currency_code_settlement, \
			CardholderCurrency, 3 ) == 0 )
   {
      memcpy ( Amount, paczisoauto.amount_transaction, 12 );
      memcpy ( TxnCurrency, paczisoauto.currency_code_settlement, 3 );
   }
   else
   {
      f_printf(stderr, "\nAMCC: GET CURRENCY ERROR \n");
      return ( ECHEC );
   }
   return ( SUCCESS );
}

int CardUsageCheck ( char* ISOData, char Format, char* Result )
{
  
   #ifdef VIJAYA_TRAVEL
   extern char User[];
   #endif
 
   char	 CountryCode[4];	
   char	 POSData[26];
   char	 CardUse;

   memset ( CountryCode, 0, 4 );
   memset ( POSData, 0, 26 );
   memset ( Result, '0', 6 );
   memcpy ( CountryCode, paczisoauto.acquiring_institution_country_code, 3 );
   if ( Format == 'M' )
   {
      GetFieldMci ( 60, ISOData, POSData );
      memcpy ( CountryCode, POSData+13, 3 );
   } 


   /* Get the card Use value */
   CardUse = paczpa05.pa05_carte_locale[0];
   if ( ( InternationalCard ( CardUse ) == SUCCESS ) && 
	 ( IsLocalUsed ( CountryCode, "356" ) == SUCCESS ) )
   {
      memcpy ( Result, "120001", 6 );
      return ( ECHEC );
   }

  /* Commented for production movement */

  /*
   if( (memcmp(User, "MCI", 3 ) == 0) || (memcmp(User, "VISA", 4) == 0) )
   {
      if(( DomestiCard ( CardUse ) == SUCCESS ) &&
         ( IsLocalUsed ( CountryCode, "356" ) != SUCCESS ))
      {
         memcpy ( Result, "120001", 6 );
         return ( ECHEC );
      }
   }
 */
#ifdef RUPAY_ISSUANCE   /* Added by Amit for RUPAY Issuance on 30 NOV 2016 */
   if( memcmp(User, "RUPAY", 5 ) == 0 )
   {
      if(( DomestiCard ( CardUse ) == SUCCESS ) &&
         ( IsLocalUsed ( CountryCode, "356" ) != SUCCESS ))
      {
         memcpy ( Result, "120001", 6 );
         return ( ECHEC );
      }
   }
#endif
   #ifdef PREPAID_CARD 	
	 
          if (   (memcmp(paczpa05.pa05_prepaid_card, "1", 1) == 0)  && 		 
	             (memcmp ( paczpa05.pa05_codbnq, "00047", 5 ) != 0) && 
         	     ( memcmp ( paczpa05.pa05_codbnq, "00052", 5) != 0) &&
                      (DoPBank(paczpa05.pa05_codbnq) == FALSE)  &&  
				( memcmp ( paczpa05.pa05_codbnq, "00038", 5) != 0)  && 
				( ( memcmp(paczisoauto.merchant_type, "6011", 4) == 0 ) ||
					( memcmp(paczisoauto.merchant_type, "6010", 4) == 0 )  )  &&
                   ( memcmp(CountryCode, "356", 3) == 0)  
			 )			  
			{	
                             #ifdef BOBL_PREPAID
                             if(memcmp(paczisoauto.primary_account_number, "546775", 6) == 0)
                             f_printf(stderr, "\n  1. Transactions Allowed In india for  BOBL PREPAID TRAVEL CARDS \n");
                             else
                             #endif
                             {

                   		f_printf(stderr, "\n Transactions not Allowed In india for PREPAID CARDS \n");
                   		memcpy ( Result, "120001", 6 );
                   		return ( ECHEC );
                             }
			}
   #endif 

   return ( SUCCESS );
}

int CheckAccountType ( char* Result )
{

   /* check Auth request from ATM and processing code is Withdrawal */
   if ( ( ( memcmp ( paczisoauto.processing_code, "01", 2 ) != 0 )&&
          ( memcmp ( paczisoauto.processing_code, "30", 2 ) != 0 )&&
          ( memcmp ( paczisoauto.processing_code, "31", 2 ) != 0 )
        ) ||
	( memcmp ( paczisoauto.merchant_type, "6011", 4 ) != 0 )
      )
   {
       return ( SUCCESS );
   }
   switch ( paczpa05.pa05_account_type_check[0] )
   {
      case '0':  /* No Checking Required */
                 f_printf(stderr, "No Account Checking Required\n");
                 break;
      case '1':	/* Saving Account */
		if ( memcmp ( paczisoauto.processing_code+2, "10", 2 ) != 0 )
		{
		   f_printf(stderr, "\nOTHER THAN SAVING A/C ON WITHDRAWAL NOT ALLOWED \n");	
                   if( memcmp( paczisoauto.processing_code+2, "00", 2 ) == 0 )
		      memcpy ( Result, "530053", 6 );
                   else if(memcmp( paczisoauto.processing_code+2, "20", 2 ) == 0 ) 
                      memcpy ( Result , "520052" , 6 );
                   else
                      memcpy ( Result , "050005" , 6 );
             
   		   return ( ECHEC );
		 }
		 break;

	case '2':	/* Checking account */
		   if ( memcmp ( paczisoauto.processing_code+2, "20", 2 ) != 0 )
		   {
		      f_printf(stderr, "\nOTHER THAN CHECKING A/C ON WITHDRAWAL NOT ALLOWED \n");	
                      if( memcmp( paczisoauto.processing_code+2, "00", 2 ) == 0 )
		          memcpy ( Result, "530054", 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "10", 2 ) == 0 ) 
                          memcpy ( Result , "530055" , 6 );
                      else
                          memcpy ( Result , "050006" , 6 );
		      return ( ECHEC );
		   }
		   break;

	case '3':	/* Credit Account */
		   if ( memcmp ( paczisoauto.processing_code+2, "30", 2 ) != 0 )
		   {
		      f_printf(stderr, "\nOTHER THAN CREDIT A/C ON WITHDRAWAL NOT ALLOWED \n");	
                      if( memcmp( paczisoauto.processing_code+2, "00", 2 ) == 0 )
		         memcpy ( Result, "050007", 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "10", 2 ) == 0 ) 
                         memcpy ( Result , "530056" , 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "20", 2 ) == 0 ) 
                         memcpy ( Result , "520057" , 6 );
                      else
                         memcpy ( Result , "050023" , 6 );
		      return ( ECHEC );
		    }
		    break;

	case '4':	/* Others Account */
		   if ( memcmp ( paczisoauto.processing_code+2, "40", 2 ) != 0 )
		   {
		      f_printf(stderr, "\nOTHER A/C TYPE 40 ON WITHDRAWAL NOT ALLOWED \n");	
                      if( memcmp( paczisoauto.processing_code+2, "00", 2 ) == 0 )
		         memcpy ( Result, "530058", 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "10", 2 ) == 0 ) 
                         memcpy ( Result , "530059" , 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "20", 2 ) == 0 ) 
                         memcpy ( Result , "520060" , 6 );
                      else
                         memcpy ( Result , "050024" , 6 );
		      return ( ECHEC );
		   }
		   break;

	case '5':	/* Credit or Default Account */ 
		   if ( memcmp ( paczisoauto.processing_code+2, "00", 2 ) != 0 &&
                        memcmp ( paczisoauto.processing_code+2, "30", 2 ) != 0
                      )
		   {
		      f_printf(stderr, "\nA/C TYPE OTHER THAN CREDIT OR DEFAULT ON WITHDRAWAL NOT ALLOWED \n");	
                      if(memcmp( paczisoauto.processing_code+2, "10", 2 ) == 0 ) 
                         memcpy ( Result , "530061" , 6 );
                      else if(memcmp( paczisoauto.processing_code+2, "20", 2 ) == 0 ) 
                         memcpy ( Result , "520062" , 6 );
                      else
                         memcpy ( Result , "050025" , 6 );
		      return ( ECHEC );
		   }
		   break;

   }
   return ( SUCCESS );
}


/* to check ICICI CIF block codes excluding for hot list reasons */
/* Gilu Alex - 24-08-2004 */
/* CR-MXP-0401-0042 Start */
int CheckCIFBlockCodes ( char* SubResult )
{
   if( memcmp ( paczpoau.poau_cd_banque, "00012", 5 ) == 0 )
   {
      if ( GetErrorCodeUsingBlockCode ( \
            paczpoau.poau_carte_a_capturer[0], SubResult ) == SUCCESS )
      {
         return ( SUCCESS );
      }
   }

   /* Card not blocked */
   return ( ECHEC );
}
/* CR-MXP-0401-0042 End   */


int CheckNegative ( char* SubResult )
{
   t_paczoppo	paczoppo;
   t_paczop00	paczop00;

   #ifdef HSBC_DOP_PHASE2 
   if (  (DoPBank(paczpa05.pa05_codbnq) == TRUE)  &&
         ( memcmp ( paczisoauto.processing_code, "96", 2 ) == 0) 
	   )
	{
          f_printf(stderr, "\n Card Closure and Full Refund - Hot List checking not required");
		  return ( ECHEC );
	}
   #endif
   #ifdef LODHA_PALAV
   if ((memcmp(paczpa05.pa05_codbnq,"00087",5 )== 0)&&
         (memcmp ( paczisoauto.processing_code, "40", 2 ) == 0)) 
	{
                  f_printf(stderr, "\n Card to Card Transfer - Hot List checking not required");
		  return ( ECHEC );
	}
   #endif
   #ifdef SELECT_CITY 
   if ((memcmp(paczpa05.pa05_codbnq,"00093",5 )== 0)&&
         (memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)) 
	{
                  f_printf(stderr, "\n SELECT CITY RELOAD - Hot List checking not required");
		  return ( ECHEC );
	}
   #endif
   #ifdef PHOENIX_MALL 
   if ((memcmp(paczpa05.pa05_codbnq,"00098",5 )== 0)&&
         (memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)) 
	{
                  f_printf(stderr, "\n SELECT CITY RELOAD - Hot List checking not required");
		  return ( ECHEC );
	}
   #endif

   /* check negative date present in Card details */
   if ((  memcmp ( paczpoau.poau_date_opposition, "      ", 6 ) == 0 ) || 
       ( memcmp ( paczpoau.poau_date_opposition, "000000", 6 ) == 0 ) )
       /******************&&
         ( memcmp ( paczpoau.poau_cd_banque, "00012", 5 ) != 0 ) )
        ***************************/
   {
      f_printf(stderr, "CARD NOT HOTLISTED\n");
   }
   else
   {
      f_printf(stderr,"Negative date present in Online File[%.6s]\n",paczpoau.poau_date_opposition );
      if ( GetErrorCodeUsingCaptureCode ( \
           paczpoau.poau_carte_a_capturer[0], SubResult ) == SUCCESS )
      {
         return ( SUCCESS );
      }
      else
      {
         GetErrorCodeUsingReferalCode ( paczpoau.poau_cd_appel[0], \
				  SubResult );
	 return ( SUCCESS );
      }
      return ( SUCCESS );
   }
   /* Card Not Hotlisted  */
   return ( ECHEC );
}



int GetCardType ( char* CardNumber, char* BankCode, char* CardType )
{
   if ( ( memcmp ( BankCode, "44444", 5 ) == 0 ) ||
        ( memcmp ( BankCode, "55555", 5 ) == 0 ) )
   {
       return ( ECHEC );
   }
   #ifdef CARD_PRODUCT_CODE
   if(CardTypeProductCode( CardType ) == SUCCESS )
   {
      return ( SUCCESS );
   }
   else
   #endif
   if ( GetTypeOfCard ( CardNumber, CardType ) != SUCCESS )
   {
      /* Card Type not Found */
      return ( ECHEC );
   }
   /* Card Type Found */
   return ( SUCCESS );
}

int GetTypeOfCard ( char* CardNumber, char* CardType )
{
   int	i = 0;
   int	cc = -1;
   int 	n[7];
   t_paczpa21	paczpa21;

   n[0] = 9; n[1] = 8; 
   n[2] = 7; n[3] = 6; 
   n[4] = 3; n[5] = 2; 
   n[6] = 1; 
   for ( i = 0; i < 7 ; i++ )
   {	
      memset ( (char*)&paczpa21, ' ', sizeof ( t_paczpa21 ) );
      memcpy ( paczpa21.pa21_code_table, "21", 2 );
      memcpy ( paczpa21.pa21_bin, CardNumber, n[i] );
      memset ( paczpa21.pa21_suite_cle, ' ', 21 ); 
      do
      {
         cc = isread ( fdparam, (char*)&paczpa21, ISEQUAL );
      }
      while ( ( iserrno == ELOCKED ) && ( cc != 0 ) );
      if ( cc == 0 )
      {
         isunlock ( fdparam );
	 memset ( CardType, 0, 5 );
	 memcpy ( CardType, paczpa21.pa21_type_of_card, 4 );
	 f_printf(stderr, "\nCard [%.19s] have has card type [%.4s] \n", \
			  CardNumber, CardType );
	 return ( SUCCESS );
      }
      isunlock ( fdparam );
   }
   f_printf(stderr, " \nCard %.19s have no corrosponding card type \n", \
             CardNumber );
   return ( ECHEC );
}

#ifdef CARD_PRODUCT_CODE
int CardTypeProductCode(char* CardType)
{

        memset(&paczpa11, ' ', sizeof(paczpa11));
        memcpy(paczpa11.pa11_notabl, "11", 2);
        memcpy(paczpa11.pa11_banque , paczpa05.pa05_codbnq, 5);
        memcpy(paczpa11.pa11_type_carte, paczpoau.poau_type_carte, 2);

        if(isread(fdparam, (char *) &paczpa11, ISEQUAL ) == 0)
        {
                isrelease ( fdparam );
                isunlock ( fdparam );
        }
        else
        {
                f_printf(stderr, "\n paczpa11 - Record read Error [%d]",  iserrno);
                return ( ECHEC );
        }

        if(memcmp(paczpa11.pa11_product_code,"1",1) == 0)
        {
                 memset ( CardType, 0, 5 );
                 memcpy ( CardType, paczpoau.poau_prod_code, 4 );
                 f_printf(stderr, "\nCard [%.19s] have has card type [%.4s]at card level \n", \
                          paczpoau.poau_num_carte, CardType );
                 if((memcmp(CardType,"    ",4) == 0)||(memcmp(CardType,"0000",4) == 0))
                 return ( ECHEC );

                 return ( SUCCESS );
        }
        else
         return ( ECHEC );


}
#endif


int MCCLimitCheck ( char* SubResult )
{
   if ( paczpa05.pa05_mcc_limit_check[0] != 'Y' )
   {
      /* MCC wise LIMIT check flag is not set - No need to check */
      return ( SUCCESS );
   }
   /* Check the MCC LIMIT */
   return ( SUCCESS );
}

int MCCProductBranchLimitCheck ( char* SubResult )
{
   if ( paczpa05.pa05_mcc_prod_branch_check[0] != 'Y' )
   {
      /* MCC Product and Branch LIMIT check flag is not set - No \
      need to check */
      return ( SUCCESS );
   }
   /* Check the MCC Product and Branch LIMIT */
   return ( SUCCESS );
}

int ReverseMCCProductBranch ( )
{
	if ( paczpa05.pa05_mcc_prod_branch_check[0] != 'Y' )
	{
		/* MCC Product and Branch LIMIT check flag is not set - No \
		  need to Reverse */
		return ( SUCCESS );
	}

	/* Reverse the MCC Product and Branch LIMIT */

	return ( SUCCESS );
}


#ifdef JK_CMS_APPLY

int iupdate_Limit_to_Addon_cards(char *card_number, char *trns_amt);




int iCheck_CardholderOnline( t_paczpoau* paczpoau,char* Result,int* Value)
{
	if((memcmp(paczpoau->poau_date_opposition ,"      " , 6)==0) ||
	(memcmp(paczpoau->poau_date_opposition ,"000000" , 6)==0))
	{
		f_printf(stderr,"\n Card Not Hotlisted");
	}
	else
	{
		if(*Value !=103)
		{
			f_printf(stderr,"\n Card is already hotlisted");/* Card Hotlisted..Reject Record */
			memcpy(Result , "830000" ,6);
			return ( ECHEC );
		}
	}
	f_printf(stderr,"Value is (%d)\n",*Value);
	if((*Value ==106)||(*Value ==105)||(*Value ==107)||(*Value == 125))
	{
		if(memcmp(paczpoau->poau_act_card ,"1" , 1)==0) /* Card Inactive..Reject Record */
		{
			f_printf(stderr," Card Inactive , Reject record\n");
			memcpy(Result , "750000" ,6);
			return ( ECHEC );
		}   
		f_printf(stderr," iCheck_CardholderOnline END\n");
	}
	return SUCCESS;
}



char * cGetTimeYYYYMMDDHHMMSS(void )
{
   struct tm *newtime;
   long ltime;
   static char theTime[25];

   time( &ltime );

   /* Obtain coordinated universal time: */
   newtime = localtime( &ltime );

   memset(theTime, 0x00, 25);
   sprintf( theTime, "%02d%02d%04d%02d:%02d:%02d",(newtime->tm_mon)+1, (newtime->tm_mday), (newtime->tm_year) + 1900, (newtime->tm_hour), (newtime->tm_min), (newtime->tm_sec));

   return(theTime);
}

/* Function used to update the cash list, card limit, card avilable, cash avilable for Addon cards  */
int iupdate_Limit_to_Addon_cards(char *card_number, char *trns_amt)
{
	char stravi_limit[15], cash_limit[8], credit_limit[15], cashlimit_amt[15]; 
    char strtemp1[15], strtemp2[15], strcard_avl[15]; 
    char filename[100]; 
    int iRetval = 0, iMODE, iFirstcard = 0 ;

	int loc_porte = -1, loc_poau = -1; 

    double dtemp = 0.0f, dcardlimit_diff = 0.0f, dval2 = 0.0f; 
	double dcashlimit = 0.0f, dcashlimit_diff = 0.0; 
    
	struct keydesc pkey;   

	pkey.k_flags  = ISDUPS;
	pkey.k_nparts = 1;
	pkey.k_part[0].kp_start = 590;
	pkey.k_part[0].kp_leng  = 19;
	pkey.k_part[0].kp_type  = CHARTYPE;

   t_paczport  loc_paczport;
   t_paczpoau loc_paczpoau; 

	
	 memset(filename, 0x00, sizeof(filename));
	 sprintf(filename, "%s/FC-PORTE", getenv("MultipacDat"));
	 loc_porte = isopen(filename, ISAUTOLOCK+ISINOUT);
	 if(loc_porte < 0)
	{
         f_printf(stderr, "\n PORTE open error [%d]", iserrno);
		 return ECHEC; 
	}

	 memset(filename, 0x00, sizeof(filename));
	 sprintf(filename, "%s/FC-POAU", getenv("MultipacDat"));
	 loc_poau = isopen(filename, ISAUTOLOCK+ISINOUT);
	 if(loc_poau < 0)
	{
         f_printf(stderr, "\n PORTE open error [%d]", iserrno);
		 isclose(loc_porte);
		 return ECHEC; 
	}

	 memset(&loc_paczport, ' ', sizeof(t_paczport));
	 memset(&loc_paczpoau, ' ', sizeof(t_paczpoau));
   
    memcpy(loc_paczport.port_num_carte_primaire, card_number, strlen(card_number));
	if( isstart( loc_porte, &pkey, 19,  (char *)&loc_paczport, ISGTEQ ) < 0 )
	{
          f_printf(stderr, "\n Check Addon card - No records");
		  isclose(loc_poau);
		  isclose(loc_porte); 
		  return ECHEC; 
	}
  
   iMODE  = ISCURR; 
    f_printf(stderr,"\n\n New Card Updation ................\n");  
   while(iserrno != EENDFILE)
   {
	       dtemp = 0.0f; 

		   memset(credit_limit, 0x00, sizeof(credit_limit)); 
           memset(cashlimit_amt, 0x00, sizeof(cashlimit_amt));
		   memset(cash_limit, 0x00, sizeof(cash_limit));    
    	       
			if(isread( loc_porte, (char *)&loc_paczport ,iMODE) < 0)
			{
				  f_printf(stderr,"\n Addon card Read Addon card read [%d] ", iserrno);
                  break; 
			}

			if(memcmp(loc_paczport.port_num_carte_primaire, card_number, strlen(card_number)) != 0)
			{
				  f_printf(stderr,"\n Addon card  Read Addon card read Invalid card [%.16s]",loc_paczport.port_numero_porteur);
				  isrelease(loc_porte);
				  isunlock(loc_porte);
                
				  /*
					  iMODE = ISNEXT; 
					  continue;  
				 */
				  break;  
			}

			if(loc_paczport.port_cd_carte_primaire[0] != '1' )
			{
				  f_printf(stderr, "\n Not an addon  card[%.16s]",loc_paczport.port_numero_porteur);				  
  				  isrelease(loc_porte);
				  isunlock(loc_porte);
				  iMODE = ISNEXT; 
                  continue; 
			}
           /* f_printf(stderr,"\n In update Limit"); */
	      memcpy(cash_limit, loc_paczport.port_pourc_plafond_cash, 5);
          dtemp = atof(cash_limit);
		  if(dtemp < 0.0)
			 	dtemp = 0.0;
 
		  dcashlimit = 0.0f; 

          /* Added for Debug purpose needs to be remove later end  */
		  if( (memcmp(cash_limit, "     ", 5) != 0) &&  (dtemp != 0.0) )
		  {
				memcpy(credit_limit, trns_amt, 12);
                dcashlimit = (atof(credit_limit) / 10000.00) * dtemp;
				if(dcashlimit < 0.0)
						dcashlimit = 0.0; 
                sprintf(cashlimit_amt, "%012.0f",dcashlimit);  
			}
			else
                   memcpy(cashlimit_amt, "000000000000", 12); 

                   #ifdef ADDON_CARDS_HOTLIST
                     memset(&loc_paczpoau, ' ', sizeof(t_paczpoau));
                     memcpy(loc_paczpoau.poau_num_carte, loc_paczport.port_numero_porteur, 19);
		   if(isread(loc_poau, (char *) &loc_paczpoau, ISEQUAL) != 0) 
                      return (SUCCES);
                      
                     if((  memcmp ( loc_paczpoau.poau_date_opposition, "      ", 6 ) == 0 ) ||
                       (  memcmp ( loc_paczpoau.poau_date_opposition, "000000", 6 ) == 0 ) )
                        {
                          f_printf(stderr,"\n Not Hotlisted Card ");
                          isrelease(loc_poau);
                         isunlock(loc_poau);
                        }
                        else
                        {
                          f_printf(stderr,"\n  Hotlisted Card ");
                          isrelease(loc_poau);
                          isunlock(loc_poau);
                          isclose(loc_poau);
                          isclose(loc_porte);
                          return (SUCCES);
                        }

                   #endif


		   memcpy(loc_paczport.port_plafond_carte,trns_amt , 12);
                   memcpy(loc_paczport.port_plafond_cash, cashlimit_amt , 12);

		   if(isrewrite ( loc_porte, (char*)&loc_paczport )<0)
			{
					 f_printf(stderr,"\n Addon card  Error in Rewriting FC-PORTE, iserrno[%d]",iserrno);
					 isrelease(loc_porte);
					 isunlock(loc_porte);					
			}
	        else
			{

				  memset(&loc_paczpoau, ' ', sizeof(t_paczpoau)); 
				  memcpy(loc_paczpoau.poau_num_carte, loc_paczport.port_numero_porteur, 19); 
				   
				   if(isread(loc_poau, (char *) &loc_paczpoau, ISEQUAL) == 0)
					{		
						dcardlimit_diff = 0.0f;  dval2 = 0.0f; 

						memset(strtemp1, 0x00, sizeof(strtemp1));
						memset(strtemp2, 0x00, sizeof(strtemp2)); 					   
					        memset(strcard_avl, 0x00, sizeof(strcard_avl));

						/* Copying the new values to card credit limit and cash advance limit */
						 memcpy(strtemp1, loc_paczpoau.poau_plafond_carte, 12);
						 memcpy(strtemp2, loc_paczpoau.poau_residuel_cash, 12);
						 memcpy(strcard_avl, loc_paczpoau.poau_residuel_carte, 12); 
                                               #ifndef ADDON_CARDS_HOTLIST 
                                                 if((  memcmp ( loc_paczpoau.poau_date_opposition, "      ", 6 ) == 0 ) ||
                                                    (  memcmp ( loc_paczpoau.poau_date_opposition, "000000", 6 ) == 0 ) )
                                                    {
                                                      f_printf(stderr,"\n Not Hotlisted Card ");
                                                    }
                                                 else
                                                    {
                                                      f_printf(stderr,"\n  Hotlisted Card ");
                                                      isrelease(loc_poau);
                                                      isunlock(loc_poau);
                                                      isclose(loc_poau);
                          			      isclose(loc_porte);

                                                      return (SUCCES);
                                                    }
                                                 #endif 

                       
						 memcpy(loc_paczpoau.poau_plafond_carte , trns_amt , 12);
						 memcpy(loc_paczpoau.poau_plafond_cash, cashlimit_amt, 12);    
						
						f_printf(stderr,"\n Current avilable [%s], strtemp1 [%s],  Transaction Amount [%s] ", strcard_avl, strtemp1, trns_amt); 
						dcardlimit_diff  = atof(trns_amt) - atof(strtemp1);
                        f_printf(stderr,"\n Fidderence Amoutn [%f]", dcardlimit_diff);

						/* Difference limit updation */
						 dval2 = (dcardlimit_diff/10000.00) * dtemp;
						
						 f_printf(stderr,"\n Addon card The value of strtemp2 [%.12s], dcardlimit_diff [%f], dtemp [%f] Final val dval2 [%f] ", strtemp2, dcardlimit_diff, dtemp, dval2);  
                         dcashlimit_diff = 0.0f; 
						dcashlimit_diff  = atof(strtemp2) + dval2; 
							 
					   /* Difference cash updation */
					   f_printf(stderr,"\n Difference amount [%f]", dcardlimit_diff);
					   dcardlimit_diff += atof(strcard_avl);
                       f_printf(stderr,"\n Final Amount [%f]", dcardlimit_diff);

					   memset(strtemp1, 0x00, sizeof(strtemp1));
					   memset(strtemp2, 0x00, sizeof(strtemp2));

					   sprintf(strtemp1, "%012.0f", dcashlimit_diff);
					   sprintf(strtemp2, "%012.0f", dcardlimit_diff);

                      f_printf(stderr,"\n Addon card number [%.19s]", loc_paczpoau.poau_num_carte);
					  f_printf(stderr, ",  Updated cash limit [%.12s]", strtemp1);
					  f_printf(stderr,", Updated Card limit [%.12s] \n", strtemp1);

					  memcpy(loc_paczpoau.poau_residuel_cash, strtemp1, 12);	
					  memcpy(loc_paczpoau.poau_residuel_carte, strtemp2, 12);

					  if(isrewrite(loc_poau, (char *) &loc_paczpoau) < 0)
					  {
						   f_printf(stderr,"\n Limit updation write error iserrno[%d]", iserrno);              
					  }
					}
					else
						f_printf(stderr,"\n Card holder Online read error iserrno [%d] card number [%.19s] ", iserrno, loc_paczpoau.poau_num_carte);
					       /* OTBL Updation to oneview */
                                        #ifdef JK_CMS_APPLY_UPD
			             	iUpdate_card_otbl_addon_cards(loc_paczpoau);  
                                        #endif

					isrelease(loc_poau);
					isunlock(loc_poau); 

					 isrelease(loc_porte);
					 isunlock(loc_porte);
			   }
            memset(&loc_paczport, ' ', sizeof(t_paczport));          
		    iMODE = ISNEXT; 
	  }
                                        isrelease(loc_poau);
                                        isunlock(loc_poau);

                                         isrelease(loc_porte);
                                         isunlock(loc_porte);

										 isclose(loc_poau);
										 isclose(loc_porte);  

	  return SUCCES; 
}

#endif


#ifdef SHELL_PHASE1 
int  Read_ONUS_05( int  Temp_Code)
{

            if(Temp_Code == READ_PARAM) 
            {

              	memset(&onuspr05, ' ' , sizeof(t_onuspr05));
              	memcpy(onuspr05.table_code, "R05",    3);
              	memcpy(onuspr05.bank_code, paczpa05.pa05_codbnq,    5);
              	if(isread(fdonusparam,&onuspr05,ISEQUAL) < 0)
                    {
                                f_printf(stderr, "\n RTH Bank paramter error iserrno [%d]", iserrno);
                                isrelease(fdonusparam);
                                isunlock(fdonusparam);
                    }
                else
                   {
                        f_printf(stderr,"\n OPTION Read_ONUS_05 SUCCESS\n");
                        isrelease(fdonusparam);
                        isunlock(fdonusparam);
	                return SUCCESS;
                   }
             }
             else
             {
               f_printf(stderr,"\nINVALID OPTION Read_ONUS_05\n");
             }



}
#endif
#ifdef SHELL_RELOAD_SLAB
int iCheckReloadSlab(char *Bin, char *Amount)
{
    t_paczpaod paczpaod;
    char szRldSlb_Amt[13];
    char szRld_Amt[13];
    int RldSlb_Amt = 0;
    int Txn_Amt = 0;
    double Total_Amount = 0.0 ;

    memset(&paczpaod, ' ', sizeof(t_paczpaod));
    memcpy(paczpaod.paod_notabl,     "OD", 2);
    memcpy(paczpaod.paod_bin, Bin, strlen(Bin));
    memcpy(paczpaod.paod_suitecle,  "                     ",  21);

    if( isread(fdparam, (char *)&paczpaod, ISEQUAL) < 0 )
    {
        f_printf(stderr, "\n RELOAD SLAB NOT DEFINED FOR THIS BIN, BIN[%s]\n", Bin);
        return -2;
    }
    isrelease(fdparam);
    isunlock(fdparam);

    memset(szRldSlb_Amt, 0x00, sizeof(szRldSlb_Amt));
    memset(szRld_Amt, 0x00, sizeof(szRld_Amt));
    memcpy(szRldSlb_Amt, paczpaod.paod_reload_slab_amt, 12);
    memcpy(szRld_Amt,Amount,12);

#ifdef SHELL_DECIMAL_RELOAD_ISSUE
    if(memcmp(Amount+10,"00",2) != 0)
    {
	f_printf(stderr, "\n ERROR..RELOAD AMOUNT IS IN DECIMAL VALUES\n");
        return ECHEC;
    }
#endif

        f_printf(stderr, "\n OD SLAB AMOUNT [%.12s] \n",paczpaod.paod_reload_slab_amt);

    Total_Amount = (atof(szRld_Amt) - (gcba_fees_amt * 100));
        f_printf(stderr, "\n Fee [%.12f] \n",gcba_fees_amt);
        f_printf(stderr, "\n TOTAL AMOUNT [%.12f] \n",Total_Amount);

    memset(szRld_Amt, 0x00, sizeof(szRld_Amt));
    sprintf ( szRld_Amt, "%012.0f", Total_Amount);
    f_printf(stderr, "\n  AMOUNT....... [%.12s] \n",szRld_Amt);

    Txn_Amt = (atoi(szRld_Amt)/100);
    RldSlb_Amt = atoi(szRldSlb_Amt);

        f_printf(stderr, "\n OD SLAB = %ld,Txn_Amt = %ld \n",RldSlb_Amt,Txn_Amt);
    if( (Txn_Amt % RldSlb_Amt) != 0 )
    {
        f_printf(stderr, "\n RELOAD AMOUNT IS NOT MULTIPLE OF RELOAD SLAB AMOUNT \n");
        return ECHEC;
    }      

    return SUCCESS; 
}
#endif


#ifdef SUBRESULT_CODE_NEW
char *Expiry_Decline_code_return()
{
    switch(gExpiry_validate_senario)
    {
        case 1:
               f_printf(stderr, "\n EXPIRED CARD \n");
               return ("540201");

        case 2:
               f_printf(stderr, "\n EXPIRY MISMATCH \n");
               return ("540011");

        case 3:
               f_printf(stderr, "\n ERROR READING CARD MASTER DURING EXPIRY DATE VALIDATION \n");
               return ("540012");

        case 4:
               f_printf(stderr, "\n ERROR UPDATING RECORD IN CARD MASTER DURING EXPIRY DATE VALIDATION \n");
               return ("540013");

        case 5:
               f_printf(stderr, "\n ERROR UPDATING RECORD IN CARDHOLDER ONLINE DURING EXPIRY DATE VALIDATION \n");
               return ("540014");

        case 6:
               f_printf(stderr, "\n DEACTIVATED CARD FOUND DURING EXPIRY DATE VALIDATION \n");
               return ("540015");

        default:
               f_printf(stderr, "\n EXPIRED CARD/EXPIRY MISMATCH \n");
                return ("540010");

    }
}
#endif

#ifdef VIJAYA_TRAVEL


int GetJulianDate()
{
  int noday=0;
  int yr, i, leap;
  int day, month, year;
  char tempvar[5];

  static char daytab[2][13] = {
      {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
  };

  memset( tempvar,0 , 5 );
  memcpy( tempvar, current_date, 2 );
  day = atoi( tempvar);

  memset( tempvar,0 , 5 );
  memcpy( tempvar, current_date+2, 2 );
  month = atoi( tempvar);

  memset( tempvar,0 , 5 );
  memcpy( tempvar, current_date+4, 4 );
  year = atoi( tempvar);

  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  for( i = 1; i < month; i++ )
        noday += daytab[leap][i];
  noday += day;

  return( noday);
}
#endif


#ifdef CNP_3ATTEMPT
int Check_CNP_Attempts(char MessageFormat)
{
        char Current_CNP_Count[3];
        char MAX_CNP_Count[3];
	#ifdef EDENRED_MOBILE_TXN /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */
        extern char User[];
	#endif

        memset(&paczpa11, ' ', sizeof(paczpa11));
        memcpy(paczpa11.pa11_notabl, "11", 2);
        memcpy(paczpa11.pa11_banque , paczpa05.pa05_codbnq, 5);
        memcpy(paczpa11.pa11_type_carte, paczpoau.poau_type_carte, 2);

        if(isread(fdparam, (char *) &paczpa11, ISEQUAL ) == 0)
        {
                isrelease ( fdparam );
                isunlock ( fdparam );
        }
        else
        {
                f_printf(stderr, "\n paczpa11 - Record read Error [%d]",  iserrno);
                return SUCCESS;
        }

        memset(Current_CNP_Count,0x00,sizeof(Current_CNP_Count));
        memset(MAX_CNP_Count,0x00,sizeof(MAX_CNP_Count));

        iCNP_Max_Attempts = 0;
        f_printf(stderr,"\n Check_CNP_Attempts");

        memcpy(Current_CNP_Count,paczpoau.poau_cnp_count,2);
        memcpy(MAX_CNP_Count,paczpa11.pa11_cnp_max_count,2);

        #ifdef EDENRED_MOBILE_TXN /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */
        if( (memcmp(User,"APPLY",5) == 0) && (memcmp (paczisoauto.processing_code,"00",2 ) == 0) && (memcmp (paczisoauto.pos_entry_mode_code,"01",2 ) == 0) )
        {
                f_printf(stderr,"\nApply Mobile Txn: iCNP_Max_Attempts[%d]",iCNP_Max_Attempts);
                return SUCCESS;
        }
        #endif

        if (paczpa02.pa02_cnp_max_att[0] == 'Y')/* Bank Level Flag */
        {
                if(Check_Card_Not_Present(MessageFormat) == SUCCESS )
                {
                        f_printf(stderr,"\n CNP Transaction [%.2s]",paczpa11.pa11_cnp_max_count);

                        if ( atoi(MAX_CNP_Count) == 0 )
                        {
                                f_printf(stderr,"\n Max count for  CNP_Attempts at card holder set to Zero");
                                return SUCCESS;
                        }

                        f_printf(stderr,"\n Current CNP count [%.2s],Max CNP count [%.2s]",paczpoau.poau_cnp_count,paczpa11.pa11_cnp_max_count);
                        f_printf(stderr,"\n Current CNP count [%d],Max CNP count [%d]",atoi(Current_CNP_Count),atoi(MAX_CNP_Count));

                        if( paczpoau.poau_cnp_block[0] == '1') /***** CNP BLOCK FLAG AT CARD HOLDER ONLINE maximum attempts *****/
                        {

                                f_printf(stderr,"\n Max Failed CNP_Attempts Reached");
                                return ECHEC;
                        }

                        iCNP_Max_Attempts = CNP_BLOCK_COUNTER;
                }
                else
                iCNP_Max_Attempts = CNP_UNBLOCK;


        }

        f_printf(stderr,"\n iCNP_Max_Attempts[%d]",iCNP_Max_Attempts);

        return SUCCESS;

}



int Check_Card_Not_Present(char MessageFormat)
{
        f_printf(stderr,"\n Check_Card_Not_Present");

        if( MessageFormat == 'V' )
        {
                f_printf(stderr,"\n VISA Check_Card_Not_Present");


                if((( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) == 0 )&&
                    (memcmp(paczisoauto.pos_condition_code,"00",2 ) == 0))||
                    (memcmp (paczisoauto.pos_condition_code,"05",2 ) == 0)||
                    (memcmp (paczisoauto.pos_condition_code,"08",2 ) == 0)||
                    (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)||
                    (memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0))
                return ( SUCCESS );

        }

        if( MessageFormat == 'M' )
        {
             f_printf(stderr,"\n MASTER CARD Check_Card_Not_Present");

            /****************KEY ENTERED***************/
                if( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) == 0 )
                return ( SUCCESS );

            /****************ECOM/MOTO/SIRP***************/
                if( memcmp( paczisoauto.pos_entry_mode_code,"81",2 ) == 0 )
                return ( SUCCESS );

        }

        #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 31 AUG 2016 */
        if( MessageFormat == 'R' )
        {
                f_printf(stderr,"\n RUPAY Check_Card_Not_Present");


                if((( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) == 0 )&&
                    (memcmp(paczisoauto.pos_condition_code,"00",2 ) == 0))||
                    (memcmp (paczisoauto.pos_condition_code,"05",2 ) == 0)||
                    (memcmp (paczisoauto.pos_condition_code,"08",2 ) == 0)||
                    (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)||
                    ((memcmp (paczisoauto.pos_entry_mode_code,"06",2 ) == 0)&&
                    (memcmp (paczisoauto.pos_condition_code,"07",2 ) == 0)))
                return ( SUCCESS );

        }
        #endif

         return(ECHEC);
}
#endif

#ifdef VISA_MCI_CNP_PARAMETER

int Get_Mci_Ecom_Value(char  *data )
{
   int i = 1;
   char tag[3];
   char length[3];
   char Data1[8];

   f_printf(stderr, "FLD48 Value [%s]\n", data );
   memset(Data1,0x00 ,sizeof(Data1));

   while( strlen(data) > i+4 )
   {
      sprintf(tag,"%.2s",data+i);
      i = i+2;
      sprintf(length,"%.2s",data+i);
      i = i+2;

      if( atoi(length) >0 )
      {
          if( atoi(tag) == 42 )
          {
              memcpy(Data1,data+i,7);

              if(memcmp(Data1+6,"2",1) == 0 )
              return (2);
              else if(memcmp(Data1+6,"1",1) == 0 )
              return (1);
              else if(memcmp(Data1+6,"0",1) == 0 )
              return (0);

          }
          else
          i = i+atoi(length);
      }
      else
          return ECHEC;
   }
}

int Card_Not_Present_Transaction_Check(char MessageFormat,char *Pos_Data)/*Done*/
{
       extern char User[];
       f_printf(stderr, " Card_Not_Present_Transaction_Check..... ...\n");
	#ifndef MVISA	
       if(memcmp(User,"APPLY",5) == 0)
	#else 
	#ifdef QR_ISSUANCE
       if((memcmp(User,"APPLY",5) == 0 )  || (( (memcmp(User,"mVisa",5) == 0 ) || (memcmp(User,"QRISS", 5)== 0) )  && ( ( memcmp ( paczisoauto.processing_code, "26", 2 ) == 0 ) || ( memcmp ( paczisoauto.processing_code, "28", 2 ) == 0 ) )))
	#else
       if((memcmp(User,"APPLY",5) == 0 )  || ((memcmp(User,"mVisa",5) == 0 )  && ( memcmp ( paczisoauto.processing_code, "26", 2 ) == 0 )))
	#endif
	#endif
       return(SUCCESS);

         char Pos_Data_Info [40];
         memset(Pos_Data_Info,0x00,sizeof(Pos_Data_Info));

         if ( MessageFormat == 'M' )
         memcpy(Pos_Data_Info,Pos_Data,strlen(Pos_Data));

         memset((char *)&cnp_transaction ,0x20, sizeof(t_cnp_transaction));
         memcpy(cnp_transaction.cp01_notabl,"CT",2);
         memcpy(cnp_transaction.cp01_codemet,paczisoauto.primary_account_number,6);

         if ( pcrzbale.bale_vip[0] == '1' )
         memcpy(cnp_transaction.cp01_vip_flag,"1",1);
         else
         memcpy(cnp_transaction.cp01_vip_flag,"0",1);

         if( MessageFormat == 'V' )
         {
                 if( memcmp ( paczisoauto.acquiring_institution_country_code, "356", 3) != 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);

                 #ifdef SFLP
                 if(memcmp(paczisoauto.acquiring_institution_country_code, paczpoau.poau_cd_monnaie, 3) == 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #endif

         }
         else if ( MessageFormat == 'M' )
         {

                 if( memcmp( Pos_Data_Info + 13, "356", 3 ) == 0 )
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
         }
/* Added by Amit for RUPAY Issuance */
#ifdef RUPAY_ISSUANCE
         else if( MessageFormat == 'R' )
         {
                 if( memcmp ( paczisoauto.acquiring_institution_country_code, "356", 3) != 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);

                #ifdef SFLP
                 if(memcmp(paczisoauto.acquiring_institution_country_code, paczpoau.poau_cd_monnaie, 3) == 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #endif
         }
#endif

         if(isread(fdonusparam,(char*)&cnp_transaction,ISEQUAL)< 0 )
         {
          if((pcrzbale.bale_vip[0] == '1' )&& (iserrno == ENOREC))
          {
             memset((char *)&cnp_transaction ,0x20, sizeof(t_cnp_transaction));
             memcpy(cnp_transaction.cp01_notabl,"CT",2);
             memcpy(cnp_transaction.cp01_codemet,paczisoauto.primary_account_number,6);
             memcpy(cnp_transaction.cp01_vip_flag,"0",1);
             if( MessageFormat == 'V' )
             {
                 if( memcmp ( paczisoauto.acquiring_institution_country_code, "356", 3) != 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #ifdef SFLP
                 if(memcmp(paczisoauto.acquiring_institution_country_code, paczpoau.poau_cd_monnaie, 3) == 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #endif

             }
             else if ( MessageFormat == 'M' )
             {

                 if( memcmp( Pos_Data_Info + 13, "356", 3 ) == 0 )
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
             }
/* Added by Amit for RUPAY Issuance */
#ifdef RUPAY_ISSUANCE
             else if( MessageFormat == 'R' )
             {
                 if( memcmp ( paczisoauto.acquiring_institution_country_code, "356", 3) != 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"1",1);
                 else
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #ifdef SFLP
                 if(memcmp(paczisoauto.acquiring_institution_country_code, paczpoau.poau_cd_monnaie, 3) == 0)
                 memcpy(cnp_transaction.cp01_dom_int_flag,"0",1);
                 #endif
             }
#endif

             if(isread(fdonusparam,(char*)&cnp_transaction,ISEQUAL)< 0 )
             {
             f_printf(stderr,"Error[%d]..... in reading the CNP Parameters\n",iserrno);
             return ( SUCCESS );
             }
             else
             f_printf(stderr,"Read Sucess CNP Parameters As Non-VIP\n");

          }
          else
          {
          f_printf(stderr,"Error [%d]in reading the CNP Parameters\n",iserrno);
          return ( SUCCESS );
          }
         
         }
         else
          f_printf(stderr,"Read Sucess the CNP Parameters\n");

         isrelease(fdonusparam);
         isunlock(fdonusparam);


        f_printf(stderr,"%s CNP Parameters\n",&cnp_transaction);

        if( MessageFormat == 'V' )
        {
                f_printf(stderr," VISA CNP Parameters\n");

                if ((memcmp (paczisoauto.pos_condition_code,"01",2 ) == 0)&& (memcmp (cnp_transaction.cp01_cardholder_not_present,"N",1 ) == 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"02",2 ) == 0)&&(memcmp (cnp_transaction.cp01_unattended_device,"N",1 ) == 0)&&
                    (memcmp (paczisoauto.merchant_type, "6011", 4)  != 0 )&&(memcmp(paczisoauto.pos_entry_capability_code+0,"2",1)!= 0 ))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"05",2 ) == 0) &&  (memcmp (cnp_transaction.cp01_cardholder_present_card_not_present,"N",1 ) == 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"08",2 ) == 0)&& (memcmp (cnp_transaction.cp01_moto_sirp,"N",1 )== 0)&&
                    (memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) != 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) &&(memcmp (cnp_transaction.cp01_e_commerce,"N",1 ) == 0)
                    &&(memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) != 0) )
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"71",2 ) == 0)&&(cnp_transaction.cp01_card_present_magstripe_error[0] == 'N'))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0) && (cnp_transaction.cp01_ivr_transactions[0] == 'N'))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"03",2 ) == 0)&& (cnp_transaction.cp01_suspicious_transaction[0]== 'N'))
                 return ( ECHEC );

                if(( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) ==0 )&&(memcmp (paczisoauto.pos_condition_code,"00",2 ) == 0)
                   &&( cnp_transaction.cp01_pos_key_entry[0] == 'N'))
                 return ( ECHEC );
        }
        #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY CNP Parameter check */
        else if(MessageFormat == 'R')
        {
                f_printf(stderr," RUPAY CNP Parameters\n");

                if ((memcmp (paczisoauto.pos_condition_code,"01",2 ) == 0)&& (memcmp (cnp_transaction.cp01_cardholder_not_present,"N",1 ) == 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"02",2 ) == 0)&&(memcmp (cnp_transaction.cp01_unattended_device,"N",1 ) == 0)&&
                    (memcmp (paczisoauto.merchant_type, "6011", 4)  != 0 ))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"05",2 ) == 0) &&  (memcmp (cnp_transaction.cp01_cardholder_present_card_not_present,"N",1 ) == 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"08",2 ) == 0) && (memcmp (cnp_transaction.cp01_moto_sirp,"N",1 )== 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) &&(memcmp (cnp_transaction.cp01_e_commerce,"N",1 ) == 0))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"71",2 ) == 0)&&(cnp_transaction.cp01_card_present_magstripe_error[0] == 'N'))
                 return ( ECHEC );

                if (( memcmp( paczisoauto.pos_entry_mode_code,"06",2 ) ==0 ) && (memcmp (paczisoauto.pos_condition_code,"07",2 ) == 0) &&
                         (cnp_transaction.cp01_ivr_transactions[0] == 'N'))
                 return ( ECHEC );

                if ((memcmp (paczisoauto.pos_condition_code,"03",2 ) == 0)&& (cnp_transaction.cp01_suspicious_transaction[0]== 'N'))
                 return ( ECHEC );

                if(( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) ==0 )&&(memcmp (paczisoauto.pos_condition_code,"00",2 ) == 0)
                   &&( cnp_transaction.cp01_pos_key_entry[0] == 'N'))
                 return ( ECHEC );
        }
        #endif
        else if ( MessageFormat == 'M' )
        {
                f_printf(stderr," MCI CNP Parameters\n");

             /*   if (( memcmp( Pos_Data_Info + 3, "1", 1 ) == 0 )&&( cnp_transaction.cp01_cardholder_not_present[0] == 'N'))
                  return ( ECHEC );*/
         /*********KEY ENTRY TRANSACTION************/

                if(( memcmp( paczisoauto.pos_entry_mode_code,"01",2 ) ==0 )&&( cnp_transaction.cp01_pos_key_entry[0] == 'N')) 
                 return ( ECHEC );

         /****************IPG***************/
                if(( memcmp( paczisoauto.pos_entry_mode_code,"81",2 ) ==0 )&&( memcmp( Pos_Data_Info + 3, "5", 1 ) == 0 ) 
                   &&(cnp_transaction.cp01_e_commerce[0] == 'N'))
                 return ( ECHEC );

         /****************MOTO/SIRP***************/

               if(( memcmp( paczisoauto.pos_entry_mode_code,"81",2 ) ==0 )&&
                  (( memcmp( Pos_Data_Info + 3, "2", 1 ) == 0 )||( memcmp( Pos_Data_Info + 3, "3", 1 ) == 0 ) 
                  ||( memcmp( Pos_Data_Info + 3, "4", 1 ) == 0 ) )
                  &&(cnp_transaction.cp01_moto_sirp[0] == 'N'))
                return ( ECHEC );

        }

             f_printf(stderr,"Returning SUCCESS - Card_Not_Present_Transaction_Check \n");

  return ( SUCCESS );

}
#endif

#ifdef SELECT_CITY
char *Card_Reload_Limit(int Type
        #ifdef LODHA_DB_26SEPT
                , char *field57
        #endif
)
{
         t_cardlimit cardlimit ;
         t_paczpaod paczpaod1;
	 f_printf(stderr,"CARD RELOAD LIMIT\n");
         int            fdcardlimit = -1 ;
         char           file_to_open[100];
         char           Monthly_Balance[13];
         char           Card_Per_txn[13];
         char           Transaction_Amount[13];
         char           Monthly_Otbl_Card[13];
         char           Date_Month_Year[9];
         double         Card_Per_Txn_Limit =  0.0 ;
         double         Txn_Amount =  0.0 ;
         double         Monthly_Otbl =  0.0 ;


#ifdef LODHA_DB_26SEPT
        if(memcmp( paczisoauto.processing_code, "20", 2 ) == 0)
        {
                memset(paczisoauto.national_pos_geographic_data, 0x00, 14);
                field57[14]='\0';
                memcpy(paczisoauto.national_pos_geographic_data , field57, strlen(field57));
                f_printf(stderr, "Populating National POS Geographic data [%s].\n", paczisoauto.national_pos_geographic_data);
        }
#endif



         memset(file_to_open,0x00,sizeof(file_to_open));
         memset(Monthly_Balance,0x00,sizeof(Monthly_Balance));
         memset(Transaction_Amount,0x00,sizeof(Transaction_Amount));
         memset(Card_Per_txn,0x00,sizeof(Card_Per_txn));

         memset(Monthly_Otbl_Card,0x00,sizeof(Monthly_Otbl_Card));
         memset(Date_Month_Year,0x00,sizeof(Date_Month_Year));
         memcpy(Date_Month_Year,(char *)GetDateDDMMYYYY(),8);

         memcpy(Date_Month_Year+0,"01",2);
         sprintf( file_to_open, "%s/FC-CARDLIMIT", getenv("MultipacDat"));
         fdcardlimit = isopen( file_to_open, ISINOUT+ISMANULOCK );
         if(fdcardlimit < 0)
         {
                  f_printf(stderr,"File FC-CARDLIMIT Open Error , iserrno [%d]\n",iserrno);
                  isclose(fdcardlimit);
                  return ( "961002" );
         }


         memset(&cardlimit, ' ', sizeof(t_cardlimit));
         memcpy(cardlimit.table_code, "04", 2);
         memcpy(cardlimit.mlist_bank_code,paczpoau.poau_cd_banque,   5);

         memcpy(cardlimit.card_number, paczpoau.poau_num_carte , 19);
         memcpy(cardlimit.processing_code,paczisoauto.processing_code , 2);
         memcpy(cardlimit.filler,"    ",4);
         if(isread(fdcardlimit, (char *) &cardlimit, ISEQUAL) == 0)
         {
                f_printf(stderr,"MONTHLY TXN LIMIT [%.12s]\n",cardlimit.card_monthly_limit);
                f_printf(stderr,"MONTHLY TXN OTBL  [%.12s]\n",cardlimit.card_monthly_otbl);

                f_printf(stderr,"Card Record [%.150s]\n",&cardlimit);

                if(memcmp(cardlimit.card_refrsh_date,Date_Month_Year,8) != 0 )
                {


		        memset(&paczpa11, ' ', sizeof(paczpa11));
        		memcpy(paczpa11.pa11_notabl, "11", 2);
        		memcpy(paczpa11.pa11_banque , paczpa05.pa05_codbnq, 5);
        		memcpy(paczpa11.pa11_type_carte, paczpoau.poau_type_carte, 2);

        		if(isread(fdparam, (char *) &paczpa11, ISEQUAL ) == 0)
        		{
                		isrelease ( fdparam );
                		isunlock ( fdparam );
        		}
        		else
        		{
                		f_printf(stderr, "\n paczpa11 - Record read Error [%d]",  iserrno);
                                isclose(fdcardlimit);
                                return ("961002");
			}



                        if(memcmp(paczpa11.pa11_kyc_limit,cardlimit.card_monthly_limit,12) == 0 )
			{
                                memcpy(cardlimit.card_monthly_otbl,cardlimit.card_monthly_limit,12);
			}
                        else
                        {
                                memcpy(cardlimit.card_monthly_limit,paczpa11.pa11_kyc_limit,12);
                                memcpy(cardlimit.card_monthly_otbl,cardlimit.card_monthly_limit,12);
                        }

                        memcpy(cardlimit.card_refrsh_date,Date_Month_Year,8);
                        isrewrite ( fdcardlimit, (char*)&cardlimit);

                #ifdef LODHA_DB_26SEPT
                        memset(&reloadtxn, ' ', sizeof(reloadtxn));
                        memcpy(reloadtxn.bank, paczpoau.poau_cd_banque, 5);
                        memcpy(reloadtxn.card, paczpoau.poau_num_carte, 19);
                        memcpy(reloadtxn.processing_code, paczisoauto.processing_code, 2);
                        memcpy(reloadtxn.reference_number, paczisoauto.retrieval_reference_number, 12);
                        /*memcpy(reloadtxn.transaction_id, paczisoauto.national_pos_geographic_data, 14);*/
                        memcpy(reloadtxn.transaction_id, field57, 14);
                        memcpy(reloadtxn.transaction_date_time, cGetTimeYYYYMMDDHHMMSS(), 16);
                        memcpy(reloadtxn.monthly_transaction_limit, cardlimit.card_monthly_limit, 12);
                        memcpy(reloadtxn.monthly_transaction_otbl, cardlimit.card_monthly_otbl, 12);

                        f_printf(stderr, "ReloadTransaction Structure [%.*s]\n", sizeof(reloadtxn), &reloadtxn);

                        if(update_reload_transactions(reloadtxn)<0)
                        {
                                f_printf(stderr, "Database update failed.[0]\n");
                        }
                        else
                        {
                                f_printf(stderr, "Database update Success.[0]\n");
                        }
                #endif


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

                if(Type == 1) /*Update OTBL Before Sending the Response */
                {
                        sprintf(Monthly_Balance,"%012.0f",(Monthly_Otbl - Txn_Amount));
                        memcpy(cardlimit.card_monthly_otbl,Monthly_Balance,12);
                        isrewrite ( fdcardlimit, (char*)&cardlimit);


                #ifdef LODHA_DB_26SEPT
                        memset(&reloadtxn, ' ', sizeof(reloadtxn));
                        memcpy(reloadtxn.bank, paczpoau.poau_cd_banque, 5);
                        memcpy(reloadtxn.card, paczpoau.poau_num_carte, 19);
                        memcpy(reloadtxn.processing_code, paczisoauto.processing_code, 2);
                        memcpy(reloadtxn.reference_number, paczisoauto.retrieval_reference_number, 12);
                        memcpy(reloadtxn.transaction_id, field57, 14);
                        memcpy(reloadtxn.transaction_date_time, cGetTimeYYYYMMDDHHMMSS(), 16);
                        memcpy(reloadtxn.monthly_transaction_limit, cardlimit.card_monthly_limit, 12);
                        memcpy(reloadtxn.monthly_transaction_otbl, cardlimit.card_monthly_otbl, 12);

                        f_printf(stderr, "ReloadTransaction Structure [%.*s]\n", sizeof(reloadtxn), &reloadtxn);

                        if(update_reload_transactions(reloadtxn)<0)
                        {
                                f_printf(stderr, "Database update failed.[1]\n");
                        }
                        else
                        {
                                f_printf(stderr, "Database update Success.[1]\n");
                        }
                #endif



                        isclose(fdcardlimit);
                        return ("000000");

                }

                if(Type == 2) /* Update OTBL For Reversal Transaction */
                {
                        sprintf(Monthly_Balance,"%012.0f",(Monthly_Otbl + Txn_Amount));
                        memcpy(cardlimit.card_monthly_otbl,Monthly_Balance,12);
                        isrewrite ( fdcardlimit, (char*)&cardlimit);



                #ifdef LODHA_DB_26SEPT
                        memset(&reloadtxn, ' ', sizeof(reloadtxn));
                        memcpy(reloadtxn.bank, paczpoau.poau_cd_banque, 5);
                        memcpy(reloadtxn.card, paczpoau.poau_num_carte, 19);
                        memcpy(reloadtxn.processing_code, paczisoauto.processing_code, 2);
                        memcpy(reloadtxn.reference_number, paczisoauto.retrieval_reference_number, 12);
                        memcpy(reloadtxn.transaction_id, field57, 14);
                        memcpy(reloadtxn.transaction_date_time, cGetTimeYYYYMMDDHHMMSS(), 16);
                        memcpy(reloadtxn.monthly_transaction_limit, cardlimit.card_monthly_limit, 12);
                        memcpy(reloadtxn.monthly_transaction_otbl, cardlimit.card_monthly_otbl, 12);

                        f_printf(stderr, "ReloadTransaction Structure [%.*s]\n", sizeof(reloadtxn), &reloadtxn);

                        if(update_reload_transactions(reloadtxn)<0)
                        {
                                f_printf(stderr, "Database update failed.[2]\n");
                        }
                        else
                        {
                                f_printf(stderr, "Database update Success.[2]\n");
                        }
                #endif




                        isclose(fdcardlimit);
                        return ("000000");

                }
            /*    if(Txn_Amount > Card_Per_Txn_Limit)
                {
                        f_printf(stderr,"EXCEEDED PER TXN LIMIT [%.12s],TXN AMOUNT [%.12s]\n",cardlimit.card_per_txn_lmt,paczisoauto.amount_transaction);
                        isclose(fdcardlimit);
                        return ("121002");
                } */

                if((Monthly_Otbl - Txn_Amount) < 0.0 )
                {
                        f_printf(stderr,"EXCEEDED PER MONTH LIMIT [%.12s],TXN AMOUNT [%.12s]\n",cardlimit.card_monthly_otbl,paczisoauto.amount_transaction);
                        isclose(fdcardlimit);
                        return ("121003");
                }

         }
         else
         {
                f_printf(stderr,"Record not present in the Master File, iserrno [%d]\n",iserrno);
                if(iserrno == ENOREC)
                {


        memset(&paczpa11, ' ', sizeof(paczpa11));
        memcpy(paczpa11.pa11_notabl, "11", 2);
        memcpy(paczpa11.pa11_banque , paczpa05.pa05_codbnq, 5);
        memcpy(paczpa11.pa11_type_carte, paczpoau.poau_type_carte, 2);

        if(isread(fdparam, (char *) &paczpa11, ISEQUAL ) == 0)
        {
                isrelease ( fdparam );
                isunlock ( fdparam );
        }
        else
        {
                f_printf(stderr, "\n paczpa11 - Record read Error [%d]",  iserrno);
                isclose(fdcardlimit);
		return ("961002");
        }


			f_printf(stderr, "paczpa11.pa11_kyc_limit select city  %.12s \n",  paczpa11.pa11_kyc_limit );
                        memcpy(Transaction_Amount,paczisoauto.amount_transaction,12);
                        Txn_Amount         = atof(Transaction_Amount);
                        memcpy(Card_Per_txn,paczpa11.pa11_kyc_limit,12);

			f_printf(stderr, "paczpa11.pa11_kyc_limit %.12s, Card_Per_txn %12s \n",  paczpa11.pa11_kyc_limit , Card_Per_txn);
                        Card_Per_Txn_Limit = atof(Card_Per_txn);
                        f_printf(stderr,"TXN AMOUNT  [%f]\n",Txn_Amount);
                        f_printf(stderr,"RELOAD MONTHLY LIMIT AMOUNT  [%f]\n",Card_Per_Txn_Limit);
                        if(Txn_Amount > Card_Per_Txn_Limit)
                        {
                        f_printf(stderr,"EXCEEDED MONTHLY RELOAD LIMIT[%.12s],TXN AMOUNT [%.12s]\n",paczpa11.pa11_kyc_limit,paczisoauto.amount_transaction);
                        isclose(fdcardlimit);
                        return ("121002");
                        } 

                        memset(&cardlimit, ' ', sizeof(t_cardlimit));
                        memcpy(cardlimit.table_code, "04", 2);
                        memcpy(cardlimit.mlist_bank_code,paczpoau.poau_cd_banque,   5);
                        memcpy(cardlimit.card_number, paczpoau.poau_num_carte , 19);

                        memcpy(cardlimit.processing_code,paczisoauto.processing_code , 2);
                        memcpy(cardlimit.filler,"    ",4);
                        memcpy(cardlimit.filler1,"             ",13);



			memcpy(cardlimit.card_monthly_limit,paczpa11.pa11_kyc_limit , 12);
			f_printf(stderr,"Reload Monthly Limit  [%.12s]\n",paczpa11.pa11_kyc_limit);
                        memcpy(cardlimit.card_monthly_otbl,paczpa11.pa11_kyc_limit , 12);

                        memcpy(cardlimit.card_refrsh_date,(char *)GetDateDDMMYYYY(),8);
                        memcpy(cardlimit.card_refrsh_date+0,"01",2);

                        f_printf(stderr,"Card Record [%.150s]\n",&cardlimit);

                        if(iswrite( fdcardlimit, (char*)&cardlimit)< 0)
                                f_printf(stderr,"write Error, iserrno [%d]\n",iserrno);
                        else
                                f_printf(stderr,"write Success\n");


                #ifdef LODHA_DB_26SEPT
                        memset(&reloadtxn, ' ', sizeof(reloadtxn));
                        memcpy(reloadtxn.bank, paczpoau.poau_cd_banque, 5);
                        memcpy(reloadtxn.card, paczpoau.poau_num_carte, 19);
                        memcpy(reloadtxn.processing_code, paczisoauto.processing_code, 2);
                        memcpy(reloadtxn.reference_number, paczisoauto.retrieval_reference_number, 12);
                        memcpy(reloadtxn.transaction_id, field57, 14);
                        memcpy(reloadtxn.transaction_date_time, cGetTimeYYYYMMDDHHMMSS(), 16);
                        memcpy(reloadtxn.monthly_transaction_limit, cardlimit.card_monthly_limit, 12);
                        memcpy(reloadtxn.monthly_transaction_otbl, cardlimit.card_monthly_otbl, 12);

                        f_printf(stderr, "ReloadTransaction Structure [%.*s]\n", sizeof(reloadtxn), &reloadtxn);

                        if(insert_reload_transactions(reloadtxn)<0)
                        {
                                f_printf(stderr, "Database Insert failed[0].\n");
                        }
                        else
                        {
                                f_printf(stderr, "Database Insert Success.[0]\n");
                        }
                #endif


                  }
                  else
                  {
                        f_printf(stderr,"Record not present in the Master File, iserrno [%d]\n",iserrno);
                        isclose(fdcardlimit);
                        return ("961002");
                  }
           }

   isclose(fdcardlimit);
   return ("000000");
}
#endif

#ifdef BFL_CUG_CARD
int BFLCheckCardNumber( char* CardNo, t_bflzport* CardDetails)
{
   int Result = -1;
   int k = 0;

   memset ( CardDetails->lport_card_no, ' ' , 19 );
   memcpy ( CardDetails->lport_card_no, CardNo, strlen ( CardNo ) );
   /* Read the Card Holder details file */
   do
   {
      Result = isread ( lnporte, (char*)CardDetails, ISEQUAL );
      if(iserrno == ELOCKED)
      sleep(1);

      k++;
   }
   while( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
   if ( Result < 0 )
   {
     f_printf(stderr, "\n CARD NOT FOUND IN BFL CARDMASTER FILE %d [%.19s]\n", iserrno, CardNo ); 
     f_printf(stderr,  "\n 1. CARD NOT COUND IN CARD MASTER \n");
     return ( ECHEC );
   }
   CODE_SCENARIO_I = 1;
   memcpy(paczisoauto.primary_account_number,CardDetails->lport_card_no,19);
   return ( SUCCESS );
}

int BFLAccountNumber( char* Cust_id, t_bflzbcom* AccountDetails)
{
   int Result = -1;
   int k = 0;

   memcpy ( AccountDetails->lbcom_bank_code, "00059", 5 );
   memcpy ( AccountDetails->lbcom_customer_id, Cust_id, 19 );
   /* Read the Account Master  Holder details file */
   do
   {
      Result = isread ( lnbcom, (char*)AccountDetails, ISEQUAL );
      k++;
   }
   while( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
   if ( Result < 0 )
   {
     f_printf(stderr, "\n ACCOUNT  NOT FOUND IN BFL ACCOUNTMASTER FILE %d [%.19s]\n", iserrno, Cust_id); 
     f_printf(stderr,  "\n 1. ACCOUNT NOT COUND IN ACCOUNT MASTER \n");
     return ( ECHEC );
   }
   f_printf(stderr,  "\n Account Record [%.24s]\n", AccountDetails);
   return ( SUCCESS );
}

int BFLClientNumber( char* Cust_id, t_bflzbcli* ClientDetails)
{
   int Result = -1;
   int k = 0;

   memcpy ( ClientDetails->lbcli_bank_code, "00059", 5 );
   memcpy ( ClientDetails->lbcli_customer_id, Cust_id, 19 );
   /* Read the Client  Master  Holder details file */
   do
   {
      Result = isread ( lnbcli, (char*)ClientDetails, ISEQUAL );
      k++;
   }
   while( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
   if ( Result < 0 )
   {
     f_printf(stderr, "\n CLIENT   NOT FOUND IN BFL CLIENT MASTER FILE %d [%.19s]\n", iserrno, Cust_id); 
     f_printf(stderr,  "\n 1. CLIENT NOT COUND IN CLIENT MASTER \n");
     return ( ECHEC );
   }
   f_printf(stderr,  "\n Client Record [%.24s]\n", ClientDetails);
   return ( SUCCESS );
}

int BFLValidateExpiry()
{
  	char System_date[7];
  	char CardExpDate[7];


  	if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
  	return ( SUCCESS );

  	memset(System_date,0x00,sizeof(System_date));
  	memset(CardExpDate,0x00,sizeof(CardExpDate));
  	memcpy ( System_date, (char*)GetDateYYYYMMDD (), 6 );
  	memcpy ( CardExpDate, "20",2 );
  	memcpy ( CardExpDate+2, bflzport.lport_exp_dt,4 );
  	if ( memcmp ( System_date , CardExpDate, 6 ) > 0 )
  	{
     		f_printf(stderr, "\n  Sysdate [%.8s]> Actual Expiry Date [%.8s] \n",System_date,CardExpDate);
     		f_printf(stderr, "\n 10. System Date is more then the Card expiry date \n");
     		return ( ECHEC ) ;
  	}

  	return ( SUCCESS );


}

int BFL_GetCardType( char* CardNumber, char* BankCode, char* CardType )
{
/*
   if( memcmp ( BankCode, "00059", 5 ) != 0 ) 
   {
       return ( ECHEC );
   }
*/
   memcpy(CardType,bflzport.lport_mxp_card_type,2);
   f_printf(stderr, "\n BFL CARD TYPE [%.2s]\n",CardType);

   if(BFL_CardTypeProductCode() == SUCCESS )
   {
      return ( SUCCESS );
   }
   else
   return ( ECHEC );
}

int BFL_CardTypeProductCode()
{

        memset(&bflzlpa03, ' ', sizeof(t_bflzlpa03));
        memcpy(bflzlpa03.lpa03_table_cd, "03", 2);
        memcpy(bflzlpa03.lpa03_bank_code, paczpa05.pa05_codbnq , 5);
        memcpy(bflzlpa03.lpa03_card_type, bflzport.lport_mxp_card_type , 2);

        if(isread(lnparam, (char *) &bflzlpa03, ISEQUAL ) == 0)
        {
                isrelease ( lnparam );
                isunlock  ( lnparam );
        }
        else
        {
                f_printf(stderr, "\n Card Type bflzlpa03 - Record read Error [%d]",  iserrno);
                return ( ECHEC );
        }
   
        memset(&bflzlpa20,' ',sizeof(t_bflzlpa20));
        memcpy(bflzlpa20.lpa20_table_cd, "20", 2);
        memcpy(bflzlpa20.lpa20_bank_code, paczpa05.pa05_codbnq , 5);

        if(isread(lnparam, (char *) &bflzlpa20, ISEQUAL ) == 0)
        {
                isrelease ( lnparam );
                isunlock  ( lnparam );
        }
        else
        {
                f_printf(stderr, "\n BFL LOAN PARAMETERS bflzlpa20 - Record read Error [%d]",  iserrno);
                return ( ECHEC );
        }

        f_printf(stderr, " BFL MIN CIBIL SCORE [%.5s]\n",bflzlpa03.lpa03_min_cibil_score);
        f_printf(stderr, " BFL ECS EXPIRY DATE [%.8s]\n",bflzlpa20.lpa20_ecs_exp_date);
        f_printf(stderr, " BFL REDEEM RATE     [%.5s]\n",bflzlpa20.lpa20_redeem_rate);
        f_printf(stderr, " BFL REFUND DAYS     [%.2s]\n",bflzlpa20.lpa20_refund_days);

        return ( SUCCESS );

}

int GetActualCardNumber( char* CardNo,char *client_no,char *ActualCard_Number, char *SubResult)
{
    	char client_number[20]; 
    	char Bfl_Card_Number[20]; 

    	memset(client_number,0x00,sizeof(client_number));
    	memset(Bfl_Card_Number,0x00,sizeof(Bfl_Card_Number));

    	if(GetClientId(paczisoauto.primary_account_number,client_number,Bfl_Card_Number,SubResult) !=SUCCESS )
    	{
      		f_printf(stderr,"\n Error in getting the Client ID \n");
                #ifdef ADDON 
                if(memcmp(SubResult,"B1",2) == 0 )
                {
                    if(ReadAddOnFile(paczisoauto.primary_account_number,ActualCard_Number)==SUCCESS )
                    {
                       memcpy(SubResult,"000000",6);
                       return ( SUCCESS );
                    }
                }
                #endif
      		return ( ECHEC );
    	}

    	if(memcmp(Bfl_Card_Number,"203040",6) == 0 )
    	{
    		memcpy(ActualCard_Number,Bfl_Card_Number,19);
    		memcpy(client_no,client_number,19);
    		f_printf(stderr,"\n Actual  Card Number [%.19s] \n",ActualCard_Number);
    		f_printf(stderr,"\n Client ID [%.19s]\n",client_no);
        	return ( SUCCESS );
    	}

    	memset(Bfl_Card_Number,0x00,sizeof(Bfl_Card_Number));
    	f_printf(stderr,"\n .................Searching Oldest Card Number using the Client ID ...............\n");
    
    	if(GetOldCard(client_number,Bfl_Card_Number) !=SUCCESS )
    	{
      		f_printf(stderr,"\n Error in getting the Card Number using the Client ID \n");
		memcpy(SubResult,"B10000",6);
      		return ( ECHEC );
    	}
    		memcpy(ActualCard_Number,Bfl_Card_Number,19);
    		memcpy(client_no,client_number,19);
    		f_printf(stderr,"\n Actual  Card Number [%.19s] \n",ActualCard_Number);
    		f_printf(stderr,"\n Client ID [%.19s]\n",client_no);

   	return ( SUCCESS );
}

int GetClientId( char* CardNo,char *client_no,char *bfl_Card_Number, char *SbResult)
{
  	char Cell_No[15];
  	int  Multiple_Clients  = 0, k,Result = -1;
  	struct keydesc  skey;
  	    memset(Cell_No,0x20,sizeof(Cell_No));
  	    memcpy(Cell_No+0,CardNo+6,10);
  	    f_printf(stderr, "\n CELL NUMBER [%.15s]\n",Cell_No);

	    fprintf(stderr,"\"%s\",Function \"%s\" RRN[%.12s] \n",__FILE__, __FUNCTION__,RRN_NO);

            skey.k_flags = ISDUPS;
            skey.k_nparts = 1;
            skey.k_part[0].kp_start = 619;
            skey.k_part[0].kp_leng =   15;
            skey.k_part[0].kp_type = CHARTYPE;
            memset((char *)&bflzbcli, ' ', sizeof(t_bflzbcli));
            memcpy(bflzbcli.lbcli_residence_mobile,Cell_No, 15);
            f_printf(stderr, " Search Record Based On Cellnumber [%.15s]  \n", bflzbcli.lbcli_residence_mobile);

            k = 0;
            do
            {
               Result = isstart ( lnbcli, &skey, 15, (char*)&bflzbcli, ISGTEQ );
               k++;
            }
            while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );

            if(Result < 0)
            {
                     f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                     isclose(lnbcli);OpenLNBCLI();
		     memcpy(SbResult,"B10000",6);
                     return(ECHEC);
            }
		     f_printf(stderr, " [%.19s], ** Record\n", bflzbcli.lbcli_customer_id);
                     if(isread ( lnbcli, (char*)&bflzbcli, ISCURR ) == 0 )
                     {
                      	   	while(memcmp(Cell_No, bflzbcli.lbcli_residence_mobile, 15) == 0)
                           	{
                                	isrelease(lnbcli); isunlock(lnbcli);Multiple_Clients ++ ;
                                	if ( iserrno == EENDFILE ) break;
                                	f_printf(stderr, " [%.19s], Secondary Key  Record, \n", bflzbcli.lbcli_customer_id);
                                	memcpy(client_no,bflzbcli.lbcli_customer_id,19);
                                	memcpy(bfl_Card_Number,bflzbcli.lbcli_oldest_active_pri_card,19);
                                	if(isread ( lnbcli, (char*)&bflzbcli, ISNEXT ) < 0 )
                                	{
                                  		break;
                                	}
                           	}
                           	if(Multiple_Clients > 1)
                           	{
					memcpy(SbResult,"B20000",6);
                                	f_printf(stderr, " More then One Client Found \n");
                             		isrelease(lnbcli);isunlock(lnbcli);isclose(lnbcli);OpenLNBCLI();return(ECHEC);
                           	}
                           	if(Multiple_Clients == 0)
                           	{
					memcpy(SbResult,"B10000",6);
                                	f_printf(stderr, "No Client Found for entered Mobile.\n");
                             		isrelease(lnbcli);isunlock(lnbcli);isclose(lnbcli);OpenLNBCLI();return(ECHEC);
                           	}
                     }
                     else
                     {
                     		f_printf(stderr, "ERROR IN READING CURRENT RECORD , iserrno[%d]\n", iserrno);
                        	memcpy(SbResult,"B10000",6);
                        	isrelease(lnbcli);isunlock(lnbcli);isclose(lnbcli);OpenLNBCLI();return(ECHEC);
                     }
                        isrelease(lnbcli);isunlock(lnbcli);isclose(lnbcli);OpenLNBCLI();
                        return ( SUCCESS );
}

int OpenLNBCLI() 
{
         char filename[30];
         memset((char *)&bflzbcli, ' ', sizeof(t_bflzbcli));
         memset(filename, 0x00, sizeof(filename));
         sprintf(filename, "%s/LN-BCLI", getenv("MultipacDat"));
         lnbcli = isopen(filename, ISAUTOLOCK+ISINOUT);
         if(lnbcli < 0)
         {
          	f_printf(stderr, "\n FILE LN-BCLI  open error [%d]", iserrno);
          	return (ECHEC);
         }
         return(0);

}

int OpenLNPORTE()
{
    	char filename[30];
        memset((char *)&bflzport, ' ', sizeof(t_bflzport));
        memset(filename, 0x00, sizeof(filename));
        sprintf(filename, "%s/LN-PORTE", getenv("MultipacDat"));
        lnporte = isopen(filename, ISMANULOCK+ISINOUT);
        if(lnporte < 0)
        {
          	f_printf(stderr, "\n FILE LN-PORTE  open error [%d]", iserrno);
          	return (ECHEC);
        }
        return(0);

}

int OpenLNSAUTO()
{
         isclose(lnsauto);
         char filename[30];
         memset(filename, 0x00, sizeof(filename));
         sprintf(filename, "%s/LN-SAUTO", getenv("MultipacDat"));
         lnsauto = isopen(filename, ISAUTOLOCK+ISINOUT);
         if(lnsauto < 0)
         {
          	f_printf(stderr, "\n FILE LN-SAUTO  open error [%d]", iserrno);
          	return (ECHEC);
         }
         return(0);
}

int OpenReadACQCHN()
{
         isclose(lnacqch);
         char filename[30];
         t_bflacqchn bflacqchn;
         memset((char *)&bflacqchn, ' ' ,sizeof(t_bflacqchn));
         memset(filename, 0x00, sizeof(filename));
         memset(acquiring_channel, 0x00, sizeof(acquiring_channel));
         sprintf(filename, "%s/LN-ACQCHN", getenv("MultipacDat"));
         lnacqch = isopen(filename, ISMANULOCK+ISINPUT);
         if(lnacqch < 0)
         {
          	f_printf(stderr, "\n FILE LN-ACQCHN open error [%d]", iserrno);
          	return (ECHEC);
         }

          memcpy(bflacqchn.acq_table,"01",2);
          memcpy(bflacqchn.acq_mid,paczisoauto.card_acceptor_identification_code,15);
          memcpy(bflacqchn.acq_tid,paczisoauto.card_acceptor_terminal_identification,8);

          if(isread( lnacqch , (char *)&bflacqchn, ISEQUAL ) < 0 )
          {
          	f_printf(stderr, "\n RECORD READ ERROR[%.25s] [%d]", &bflacqchn,iserrno);
                isclose(lnacqch);
                memcpy(acquiring_channel,"  ",2);
                return(0);

          }

          memcpy(acquiring_channel,bflacqchn.acq,2);
          isrelease(lnacqch); 
          isclose(lnacqch);
          return(0);
}




int GetOldCard(char *client_no,char* CardNo)
{
  int  Multiple_Cards = 0, k,Result = -1;
  int  count  = 0,position = 0; ;
  char card_array[10][19];
  char act_array[10][6];
  struct keydesc  skey1;
  f_printf(stderr, "\n Client ID [%.19s]\n",client_no);
            skey1.k_flags = ISDUPS;
            skey1.k_nparts = 1;
            skey1.k_part[0].kp_start = 19;
            skey1.k_part[0].kp_leng =  19;
            skey1.k_part[0].kp_type = CHARTYPE;
            memset((char *)&bflzport, ' ', sizeof(t_bflzport));
            memcpy(bflzport.lport_cust_id,client_no, 19);
            f_printf(stderr, " Search Record Based On client id[%.19s]  \n",bflzport.lport_cust_id);

            k = 0;
            do
            {
               Result = isstart ( lnporte , &skey1, 19, (char*)&bflzport, ISGTEQ );
               k++;
            }
            while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );

            if(Result < 0)
            {
                     f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                     isclose(lnporte);
                     OpenLNPORTE();
                     return(ECHEC);
            }
                     f_printf(stderr, " [%.24s], ** Record\n", &bflzport);
                     if(isread ( lnporte , (char*)&bflzport, ISCURR ) == 0 )
                     {
                           while(memcmp(client_no , bflzport.lport_cust_id , 19) == 0)
                           {
                                isrelease(lnporte);
                                isunlock(lnporte);
                                Multiple_Cards++ ;
                                memcpy(CardNo,bflzport.lport_card_no,19);
                                if ( iserrno == EENDFILE ) break;

                                f_printf(stderr, " [%.24s], Secondary Key  Record, [%.6s]\n",
					 &bflzport,bflzport.lport_activation_dt);

                                if(memcmp(bflzport.lport_activation_flag,"Y",1) == 0)
                                {
                                	memcpy(card_array[count],bflzport.lport_card_no,19);
                                	memcpy(act_array[count],bflzport.lport_activation_dt,6);
                                	count++;
                                }
                                if(isread ( lnporte , (char*)&bflzport, ISNEXT ) < 0 )
                                break;
          
                           }

			   if(Multiple_Cards > 1)
                           {
                                f_printf(stderr, " More then One Cards Found \n");
                                isrelease(lnporte);
                                isunlock(lnporte);
                           }
                     }
                          isrelease(lnporte);
                          isunlock(lnporte);
                          isclose(lnporte);

                          if(count == 2)
                          {
                           	f_printf(stderr, "  Two Cards Found \n");
                           	if(memcmp(act_array[0],act_array[1],6)  > 0 )
				{
					memcpy(CardNo,card_array[0],19);

					f_printf(stderr, " Card With Active Status[%.19s] Card Activation Date[%.6s]\n",
						 CardNo,act_array[0]);
				}
                           	else
				{
					memcpy(CardNo,card_array[1],19);

					f_printf(stderr, " Card With Active Status[%.19s] Card Activation Date[%.6s]\n",
						 CardNo,act_array[1]);
				}

                          }
                          else if (count > 2)
                          {
                                f_printf(stderr, " More then Two Cards Found [%d]\n",count);
                          	for(int k  = 0 ;k < count;k++)
                          	{
        				for(int l  = k+1 ;l < count;l++)
        				{
                                        	if(memcmp(act_array[k],act_array[l],6) < 0 )
          					{
             						if((l+1) == count)
             						{
                						f_printf(stderr,"Success ..%d\n", k);
                                                        	position = k ;
                						k  = count;
             						}
          					}
          					else
          					l = count;
					}
                                        if((k+1) == count)
                                        position = k ;
				}
                           memcpy(CardNo,card_array[position],19);
                           f_printf(stderr, "Oldest Created Card With Active Status[%.19s] Created Date[%.6s]\n", CardNo,act_array[position]);
                          }
                          OpenLNPORTE();
                          return ( SUCCESS );
}

/* BFL_DELIVERY_CHANGE - created new function */
int Card_Embo_Date_Checking()
{

        char    Port_Embo_Dt[9];
        char    Current_Date[9];
        int     Embo_Date_Diff;


        if((memcmp(bflzport.lport_embo_dt,"000000",6) == 0 ) || (memcmp(bflzport.lport_embo_dt,"      ",6) == 0 ) )
        {
              f_printf(stderr,"Invalid Embossing date \n");
              return ( ECHEC );
        }
        
       
        memcpy(Port_Embo_Dt, "20", 2);
        memcpy(Port_Embo_Dt+2, bflzport.lport_embo_dt, 6);
        Port_Embo_Dt[8]='\0';

        f_printf(stderr, "Embossing Date : [%s]\n",Port_Embo_Dt);


        memcpy(Current_Date,GetDateYYYYMMDD(), 8);
        Current_Date[8]='\0';

        f_printf(stderr, "Current Date : [%s]\n",Current_Date);


        Embo_Date_Diff = DateDifferenceInDays(Current_Date, Port_Embo_Dt);

        f_printf(stderr, "Difference in days : [%d]\n", Embo_Date_Diff);


        if(Embo_Date_Diff >= 0)
        {
                f_printf(stderr,"Embossing date is Equal/less than the current date \n");
                return (SUCCESS);
        }
        else
        {
                f_printf(stderr, "Delivery status fail. "
                        "Embossing date is Future  date [%s] \n",Port_Embo_Dt);
                f_printf(stderr, "CARD DELIVERY STATUS = NO\n" );
                return ( ECHEC );
        }
}

int BFL_CheckRecordInMovementAuth( char* Amount, t_bflauth00* AuthRec, int mti )
{
   struct keydesc	skey1;
   int k, Result;
   int     Cibil_Count = 0 ;
   char	   MerchantId[16];
   char	   RRNO[13];
   char	   CardNumber[20];
   char    Subsequent_product[101];
   char    Subsequent_product_1[10];
   char    Subsequent_product_2[10];
   int     PARENT_EXIST = 0;   
   extern char User[];
   t_bflauth00  AuthRec_child;
  
   memset ( MerchantId, 0, 16 );
   memset ( RRNO, 0, 13 );
   memset ( RRNO, 0x20, 12 ); 
   memset ( CardNumber, 0, 20 );
   memset ( Subsequent_product , 0, sizeof(Subsequent_product) );
   memset ( Subsequent_product_1 , 0, sizeof(Subsequent_product_1) );
   memset ( Subsequent_product_2 , 0, sizeof(Subsequent_product_2) );
   memset ( CardNumber, 0x20, 19 );
   memset ( (char*)AuthRec, ' ', sizeof ( t_bflauth00 ) );
   memcpy ( MerchantId, paczisoauto.card_acceptor_identification_code, 15 );
   memcpy ( RRNO, paczisoauto.retrieval_reference_number, 12 );
   memcpy ( CardNumber, paczisoauto.primary_account_number, \
   strlen ( paczisoauto.primary_account_number ) ); 
   #ifdef BFL_PHASE2
   if(( mti == MSG_0101)&&(strlen(parent_rrn) == 12))         
   memcpy ( RRNO, parent_rrn, 12 );
   #endif

   CODE_SCENARIO_A = 'X';

   memset ( (char*)AuthRec, ' ', sizeof ( t_bflauth00 ) );
   memcpy ( AuthRec->reference_number, RRNO, 12 );
   k = 0;
   f_printf(stderr, " ISSTART Status [%d]\n", Result);

   Result = isread ( lnsauto, (char*)AuthRec, ISEQUAL );
   if ( Result == 0 )
   {
      f_printf(stderr, "Read Error iserrno [%d] \n", iserrno);
   
   f_printf(stderr," AuthRec [%s] \n",AuthRec->reference_number);
   f_printf(stderr," RRNO[%.12s]\n",RRNO);
  
                 #ifdef BFL_PHASE2
                 if(   ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) &&
                             ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) <  0 ) &&
                             ( memcmp ( AuthRec->relationship_type, "P",1) == 0)&&
                             ( memcmp ( AuthRec->auth_source_rev, "A",1) == 0)&&
                             ( memcmp ( AuthRec->terminal_no,paczisoauto.card_acceptor_terminal_identification,8) == 0 )&&
                             ( memcmp ( AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 )&&
                             ( memcmp ( AuthRec->response_code,"00",2) == 0 )&&
                             ( memcmp ( AuthRec->auth_req_date,GetDateYYMMDD (),6) == 0 )&&
                             ( mti == MSG_0101) )
                         {
                                         f_printf(stderr,"1.PARENT TRANSACTION FOUND \n");
                                         isunlock ( lnsauto );
                                         return ( PARENT);
                         }
                 #endif

		if ( strlen ( paczisoauto.card_acceptor_identification_code ) == 0) 
			memcpy ( MerchantId, AuthRec->merchant_no, 15 );
		if ( memcmp ( MerchantId+10, "     ", 5 ) == 0 )
			memcpy ( MerchantId+10, "00000", 5 );
		
		f_printf(stderr, "Record with the same RRN located\n");
		f_printf(stderr, "Checking Card Number, Amount & Txn date n Time \n");
		f_printf(stderr, "Request packet Amount = [%.12s] LN-SAUTO Amount = [%.12s] \n", paczisoauto.amount_transaction,	AuthRec->transaction_amount);
		f_printf(stderr, "Request packet TransDateTime = [%.10s] LN-SAUTO TransDateTime = [%.10s]  \n", paczisoauto.original_data_elements+10,	AuthRec->auth_req_time);
		f_printf(stderr, "Request packet Merchant [%.15s], Auth merchant [%.15s] \n", paczisoauto.card_acceptor_identification_code, AuthRec->merchant_no);
		f_printf(stderr, "Today's date [%.6s], Auth Date [%.6s] \n", GetDateYYMMDD(), AuthRec->auth_req_date);
		f_printf(stderr, "Auth Rev source: [%.1s] \n", AuthRec->auth_source_rev);
		f_printf(stderr, "Card Number: [%.19s],%d, Auth Card : [%.19s] \n",CardNumber, strlen(CardNumber), AuthRec->card_number);
		f_printf(stderr, "Txn RRN: [%.12s], Auth RRN : [%.12s] \n", paczisoauto.retrieval_reference_number, AuthRec->reference_number);
               if( (memcmp(paczisoauto.original_data_elements+10,"0000000000" ,10) != 0) ) 
	       {
                     
                           if( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) &&
                           ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 ) && 
                           ( ( memcmp(AuthRec->trace_audit_no,paczisoauto.system_trace_audit_number,6) == 0 ) 
				 || ( ( memcmp(paczisoauto.original_data_elements+4, AuthRec->trace_audit_no+4, 6) == 0) && (memcmp(AuthRec->auth_match_flag, "A",1) == 0)  )
				) &&
                             ( memcmp(AuthRec->terminal_no,paczisoauto.card_acceptor_terminal_identification,8) == 0 )&&
                            (( mti == MSG_0100 ) ))
			 {
					 f_printf(stderr,"1.Reauthorisation/Original Record Found\n");
					 isunlock ( lnsauto );
					 return ( SUCCESS );
			 }

                   if(memcmp(paczisoauto.processing_code,"20",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) 
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) 
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                        {
                                        f_printf(stderr, "R.Original Transaction located in LN-SAUTO \n");
                                        isunlock ( lnsauto );
					f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                        else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "H",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                        {
                                        f_printf(stderr, "RH.Original Transaction located in LN-SAUTO \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) 
                             && (memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "E.Original Transaction Already Refunded Reject the Transaction\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "Q",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                       {
                                        f_printf(stderr, "Refund after one round of Refund then reversal \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                       }
                       else
                       {
                                        f_printf(stderr, "F.Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                   } 
                   if(memcmp(paczisoauto.processing_code,"23",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {
                                        f_printf(stderr, "RR.Original Transaction located in LN-SAUTO \n");
                                        f_printf(stderr, "RR.Original Transaction is in REFUND STATE \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "RE.Original Transaction is in Authorisation state\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "RF.Refund Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                   }
                   if(memcmp(paczisoauto.processing_code,"03",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {
                                        f_printf(stderr, "VSR.Original Transaction located in LN-SAUTO \n");
                                        f_printf(stderr, "VSR.Original Transaction is in REFUND STATE \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "VSE.Original Transaction is in Authorisation state\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "VSF.Refund Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                   }
                   if(memcmp(paczisoauto.processing_code,"39",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {
                                        f_printf(stderr, "VRR.Original Transaction located in LN-SAUTO \n");
                                        f_printf(stderr, "VRR.Original Transaction is in REFUND STATE \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        memcpy (Requested_reward_point,bflauth00.redeem_point,6);
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "VRE.Original Transaction is in Authorisation state\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "VRF.Refund Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                   }
                   if((memcmp(paczisoauto.processing_code,"38",2) == 0) || (memcmp(paczisoauto.processing_code,"99",2) == 0))
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {
                                        f_printf(stderr, "VR.Original Transaction located in LN-SAUTO \n");
                                        isunlock ( lnsauto );
                                        if(memcmp(paczisoauto.processing_code,"99",2) == 0)
                                        memcpy (Requested_reward_point,bflauth00.redeem_point,6);
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "E.Original Transaction Already Reversed/Voided Reject the Transaction\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "F.Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                   }
                   if(memcmp(paczisoauto.processing_code,"02",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {

                                        if((memcmp(buisness_prod_code,"REMI",4) == 0)
                                           &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0))
                                        {
                                        	f_printf(stderr, "VS.Original Transaction located in LN-SAUTO \n");
                                        	isunlock ( lnsauto );
                                        	return ( SUCCESS );
                                        }
                                        else if (memcmp(buisness_prod_code,"REMI",4) != 0)
                                        {
                                                f_printf(stderr, "VS.Original Transaction located in LN-SAUTO \n");
                                                isunlock ( lnsauto );
                                                return ( SUCCESS );
                                        }
                                        else
                                        {
                                        	f_printf(stderr, "F.Original Transaction Is Not Located\n");
                                        	isunlock ( lnsauto );
                                        	return ( ECHEC );
                                        }

                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "H",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                        {
                                        f_printf(stderr, "VH.Original Transaction located in LN-SAUTO \n");
                                        isunlock ( lnsauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "E.Original Transaction Already Refunded Reject the Transaction\n");
                                        isunlock ( lnsauto);
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "F.Original Transaction Is Not Located\n");
                                        isunlock ( lnsauto );
                                        return ( ECHEC );
                       }

                  }
                  else
                  {
                   
		   if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) && 
			 ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 ) && 
			 ( ( memcmp ( paczisoauto.original_data_elements+10, AuthRec->auth_req_time, 6) == 0 )  /* Txn Date and Time */
 			     && (memcmp(AuthRec->auth_source_rev, "A",1) == 0) ))
			{ 
					f_printf(stderr, "2.Original Transaction located in LN-SAUTO \n");
					isunlock ( lnsauto );
					return ( SUCCESS );
			}
		   else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) && 
			 ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 )	)
			{
			f_printf(stderr, "3. Original Transaction located in LN-SAUTO \n");
			isunlock ( lnsauto );
			return ( SUCCESS );
			}

                  }
	       }

    }
    else
    {

       #ifdef BFL_PHASE2
       if( mti == MSG_0101)
       {
       	f_printf(stderr, "PARENT  Transaction Is Not Located\n");
        isunlock ( lnsauto );
        return ( ECHEC );
       }
       #endif
		
/************Search Record based On the Secondary Key*******************/
   skey1.k_flags = ISDUPS;
   skey1.k_nparts = 1;
   skey1.k_part[0].kp_start = 12;
   skey1.k_part[0].kp_leng = 19;
   skey1.k_part[0].kp_type = CHARTYPE;

   f_printf(stderr, "\n LN-SAUTO Searching the record based on the card number ");
   memset ( (char*)AuthRec, ' ', sizeof ( t_bflauth00 ) );
   memcpy ( AuthRec->card_number, CardNumber, 19 );
   if( mti == MSG_0102)
   memcpy(Subsequent_product,bflzlpa20.lpa20_product_codes,100);
   k = 0;
   do
   {
      Result = isstart ( lnsauto, &skey1, 19, (char*)AuthRec, ISGTEQ );
      k++;
   }
   while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );

   f_printf(stderr, " Locate original Record (Card) Result [%d]\n", Result);
   if(isread ( lnsauto, (char*)AuthRec, ISCURR  ) < 0 )
   {
      f_printf(stderr,"ErrorReading[%d]",iserrno);
      OpenLNSAUTO();
      return( ECHEC );
   }
   f_printf(stderr, " record[%s]",(char*)(AuthRec->card_number));
   	while ( memcmp ( AuthRec->card_number, CardNumber, 19) == 0 )
   	{
     	 	f_printf(stderr, " Card [%.19s]\n", AuthRec->card_number);
      		if ( iserrno == EENDFILE ) break;
                   /*************************/
      		f_printf(stderr, " Packet Card Number            [%s]  \n",   paczisoauto.primary_account_number);
      		f_printf(stderr, " Packet Original Data Elements [%s] \n",    paczisoauto.original_data_elements);
      		f_printf(stderr, " File   Card Number            [%.19s]  \n",AuthRec->card_number );
      		f_printf(stderr, " File   System Trace Audit     [%.6s] \n" , AuthRec->trace_audit_no);
      		f_printf(stderr, " File   Date and Time          [%.10s] \n" ,AuthRec->auth_req_time);
      		f_printf(stderr, " RRN                           [%.12s] \n" ,AuthRec->reference_number);
      		f_printf(stderr, " auth_source_rev               [%.1s] \n" , AuthRec->auth_source_rev);
      		f_printf(stderr, " relationship_type             [%.1s] \n" , AuthRec->relationship_type);
      		f_printf(stderr, " MTI                           [%.d] \n" ,  mti);
    		if( memcmp( paczisoauto.original_data_elements + 10 ,"0000000000",10) != 0) 
		{

                       if(   ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) &&
                             ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) <  0 ) &&
                             ( memcmp ( AuthRec->relationship_type, "P",1) == 0)&&
                             ( memcmp ( AuthRec->auth_source_rev, "A",1) == 0)&&
                             ( memcmp ( AuthRec->terminal_no,paczisoauto.card_acceptor_terminal_identification,8) == 0 )&&
                             ( memcmp ( AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 )&&
                             ( memcmp ( AuthRec->response_code,"00",2) == 0 )&&
                             ( memcmp ( AuthRec->auth_req_date,GetDateYYMMDD (),6) == 0 )&&
                             ( mti == MSG_0101) )
                         {
                                         memset ( &AuthRec_child, 0x20, sizeof ( t_bflauth00 ) );
                                         memcpy ( (char *)&AuthRec_child,(char *)AuthRec,sizeof ( t_bflauth00 ));
                                         f_printf(stderr,"5.Parent Transaction is found in  LN-SAUTO\n");
                                         f_printf(stderr, " Parent RRN [%.12s]  \n",   AuthRec->reference_number);
                                         isunlock ( lnsauto );
                                         PARENT_EXIST = 5;
                         }


			 if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) && 
			 ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 ) && 
			 ( ( memcmp ( paczisoauto.original_data_elements+10, AuthRec->auth_req_time, 6) == 0 )  /* Txn Date and Time */
 			     && (memcmp(AuthRec->auth_source_rev, "A",1) == 0) )
			    ) 	
			{ 
					f_printf(stderr, "6.Original Transaction located in LN-SAUTO \n");
					isunlock ( lnsauto );
                                        OpenLNSAUTO();
					return ( SUCCESS );
			}
		 }
		if(isread ( lnsauto, (char*)AuthRec, ISNEXT ) < 0 )
		{
			f_printf(stderr,"Error_in_read[%d]",iserrno );
                        OpenLNSAUTO();
			return( ECHEC );
		}
		isunlock ( lnsauto );
  	}
  	isunlock ( lnsauto );
        if(PARENT_EXIST == 5)
        {
           memset ( (char*)AuthRec, ' ', sizeof ( t_bflauth00 ) );
           memcpy ((char *)AuthRec, (char *)&AuthRec_child,sizeof ( t_bflauth00 ));
           f_printf(stderr, " Last Record Parent RRN [%.12s]  \n",   AuthRec->reference_number);
           OpenLNSAUTO();
           return ( PARENT );
        }

  	f_printf(stderr, "6:Record not found in Auth movemnt file \n" );
        OpenLNSAUTO();
  	return ( ECHEC );
     }
  }
void BFL_ReverseRthAuthRecord ( char* RRNO ,int MTI)
{
   t_bflrthauth AuthRec;

   memset ( (char*)&AuthRec, ' ', sizeof ( t_bflrthauth) );
   memcpy ( AuthRec.reference_number, RRNO, 12 );
   if ( isread ( lnrth, (char*)&AuthRec, ISEQUAL ) == 0 )
   {
      memcpy(AuthRec.transmission_date_time,(char *)GetDateDDMMYYYY(),8);
      memcpy(AuthRec.transmission_date_time+8,(char *)GetTime(),6);

      memcpy(AuthRec.txn_date_local, "20",2 );
      memcpy(AuthRec.txn_date_local+2,(char *)GetDateYYMMDD(),6 );
      if(strlen(Uniq_Identifier_Code) > 0 )
      memcpy(AuthRec.uniq_identifier,Uniq_Identifier_Code,strlen(Uniq_Identifier_Code));
      
      if( (MTI == 400) &&(memcmp(paczisoauto.processing_code,"20",2) == 0  ))
      {
          memcpy(AuthRec.processing_code, paczisoauto.processing_code, 6);
          memcpy(AuthRec.loanstatus ,"CANCELLED ",10);
          if(strlen(Otp_Code) >  0 )
	  {
		if(strlen(Otp_Code) > 6)
			memcpy(AuthRec.otp_code,Otp_Code+(strlen(Otp_Code)-6),6);
		else
			memcpy(AuthRec.otp_code,Otp_Code,strlen(Otp_Code));
	  }
          else
          memcpy(AuthRec.otp_code,"      ",6);

	  if(strlen(Otp_Request_id) > 0 )
          memcpy(AuthRec.otp_request_id,Otp_Request_id,strlen(Otp_Request_id));
          else
          memcpy(AuthRec.otp_request_id,"                       " ,23);
      }
      else if( (MTI == 400) &&(memcmp(paczisoauto.processing_code,"02",2) == 0  ))
      {
                memcpy(AuthRec.processing_code ,paczisoauto.processing_code,2);
          	memcpy(AuthRec.loanstatus ,"VOID      ",10);
                if(strlen(Limit_Before_Void) == 12)
                memcpy(AuthRec.available_limit_before_void,Limit_Before_Void,12);
                if(strlen(Limit_After_Void) == 12)
                memcpy(AuthRec.available_limit_after_void,Limit_After_Void,12);
                memset(Limit_Before_Void,0x00,13);
                memset(Limit_After_Void,0x00,13);
                memcpy(AuthRec.preferredcardlimit,bflzbcom.lbcom_acc_prefferd_limit, 12 );
                if(strlen(Otp_Code) >  0 )
		{
			if(strlen(Otp_Code) > 6)
				memcpy(AuthRec.otp_code,Otp_Code+(strlen(Otp_Code)-6),6);
			else
				memcpy(AuthRec.otp_code,Otp_Code,strlen(Otp_Code));
		}
          	else
          	memcpy(AuthRec.otp_code,"      ",6);

          	if(strlen(Otp_Request_id) > 0 )
          	memcpy(AuthRec.otp_request_id,Otp_Request_id,strlen(Otp_Request_id));
          	else
          	memcpy(AuthRec.otp_request_id,"                       " ,23);

      }
      else if( (MTI == 400) &&(memcmp(paczisoauto.processing_code,"38",2) == 0  ))
      {
                memcpy(AuthRec.processing_code ,paczisoauto.processing_code,2);
          	memcpy(AuthRec.loanstatus ,"VOID      ",10);
      }
      else if (MTI == 400)
      {
                memcpy(AuthRec.processing_code ,paczisoauto.processing_code,2);
          	memcpy(AuthRec.loanstatus ,"REVERSAL  ",10);
      }

      if ((MTI == 400) && ((memcmp(paczisoauto.card_acceptor_identification_code,"000594444444444",15) == 0 )
          	       &&  (memcmp(paczisoauto.card_acceptor_terminal_identification,"01010101",8) == 0 )) )
      memcpy(AuthRec.manual_reverse,"Y",1);
     

#ifdef MRL_287	
      if((strlen(Otp_Request_id) > 0 )&&(strlen(Otp_Code) > 0 ))
      {
        memcpy(AuthRec.transaction_type_otp_pin, "O", 1);
        f_printf(stderr,"updating transaction_type [%.1s]",AuthRec.transaction_type_otp_pin);
      }
      else
      {
        memcpy(AuthRec.transaction_type_otp_pin, "P", 1);
        f_printf(stderr,"updating transaction_type [%.1s]",AuthRec.transaction_type_otp_pin);
      }
#endif    
 
      if ( isrewrite ( lnrth, (char*)&AuthRec ) < 0 )
      {
         f_printf(stderr, "REWRITE ERROR BFL-RTH-AUTH   [%d] \n", iserrno );
      }

      BFL_UpdateToLogFile(&AuthRec, 'C');
   }
   else
   {
      f_printf(stderr, "RECORD NOT FOUND IN LOAN RTH-AUTH [%.12s] \n", RRNO );
   }
    isunlock ( lnrth);
}
int BFL_CheckRecordInHistoryAuth( char* Amount, t_bflhistauth* AuthRec, int mti )
{
   int     k, Result;
   char    MerchantId[16];
   char    RRNO[13];
   char    CardNumber[20];

   memset ( MerchantId, 0, 16 );
   memset ( RRNO, 0, 13 );
   memset ( CardNumber, 0, 20 );
   memset ( (char*)AuthRec, ' ', sizeof ( t_bflhistauth ) );
   memcpy ( MerchantId, paczisoauto.card_acceptor_identification_code, 15 );
   memcpy ( RRNO, paczisoauto.retrieval_reference_number, 12 );
   memcpy ( CardNumber, paczisoauto.primary_account_number, \
   strlen ( paczisoauto.primary_account_number ) );

   CODE_SCENARIO_A = 'X';

   
   memset ( (char*)AuthRec, ' ', sizeof ( t_bflhistauth ) );
   memcpy ( AuthRec->reference_number, RRNO, 12 );

   Result = isread ( lnhauto , (char*)AuthRec, ISEQUAL );
   if ( Result == 0 )
   {
      f_printf(stderr," AuthRec [%s] \n",AuthRec->reference_number);
      f_printf(stderr," RRNO[%.12s]\n",RRNO);
   
                if ( strlen ( paczisoauto.card_acceptor_identification_code ) == 0)
                        memcpy ( MerchantId, AuthRec->merchant_no, 15 );
                if ( memcmp ( MerchantId+10, "     ", 5 ) == 0 )
                        memcpy ( MerchantId+10, "00000", 5 );

                f_printf(stderr, "Record with the same RRN located\n");
                f_printf(stderr, "Checking Card Number, Amount & Txn date n Time \n");
                f_printf(stderr, "Request packet Amount = [%.12s] LN-SAUTO Amount = [%.12s] \n", paczisoauto.amount_transaction,       AuthRec->transaction_amount);
                f_printf(stderr, "Request packet TransDateTime = [%.10s] LN-SAUTO TransDateTime = [%.10s]  \n", paczisoauto.original_data_elements+10, AuthRec->auth_req_time);
                f_printf(stderr, "Request packet Merchant [%.15s], Auth merchant [%.15s] \n", paczisoauto.card_acceptor_identification_code, AuthRec->merchant_no);
                f_printf(stderr, "Today's date [%.6s], Auth Date [%.6s] \n", GetDateYYMMDD(), AuthRec->auth_req_date);
                f_printf(stderr, "Auth Rev source: [%.1s] \n", AuthRec->auth_source_rev);
                f_printf(stderr, "Card Number: [%.19s],%d, Auth Card : [%.19s] \n",CardNumber, strlen(CardNumber), AuthRec->card_number);
                f_printf(stderr, "Txn RRN: [%.12s], Auth RRN : [%.12s] \n", paczisoauto.retrieval_reference_number, AuthRec->reference_number);
                if((memcmp(paczisoauto.processing_code,"38",2) == 0) || (memcmp(paczisoauto.processing_code,"39",2) == 0)
                   ||(memcmp(paczisoauto.processing_code,"03",2) == 0))
                {
                                        f_printf(stderr,"\n VOID/VOID REVERSAL  SHOULD NOT BE CHECKED IN HISTORY FILE\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC );
                }

                   if(memcmp(paczisoauto.processing_code,"20",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->auth_match_flag, "2",1) != 0)
			     && (memcmp(AuthRec->auth_match_flag, "5",1) != 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                        {
                                        f_printf(stderr, "HR.Original Transaction located in LN-AUTP \n");
                                        isunlock ( lnhauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 )
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "HR.Original Transaction Already Refunded Reject the Transaction\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC);
                       }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "Q",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0)
                             && (memcmp(AuthRec->merchant_no,paczisoauto.card_acceptor_identification_code,15) == 0 ))
                       {
                                        f_printf(stderr, "Refund after one round of Refund then reversal \n");
                                        isunlock ( lnhauto );
                                        f_printf(stderr,"\n HR.Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                       }
                       else
                       {
                                        f_printf(stderr, "HF.Original Transaction Is Not Located\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC );
                       }

                   }
                   if(memcmp(paczisoauto.processing_code,"23",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {
                                        f_printf(stderr, "HR.Original Transaction located in LN-AUTP \n");
                                        f_printf(stderr, "HR.Original Transaction is in REFUND STATE \n");
                                        isunlock ( lnhauto );
                                        f_printf(stderr,"\n Copying Original Amount\n");
                                        memcpy ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 );
                                        return ( SUCCESS );
                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "HR.Original Transaction is in Authorisation state\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "HR.Refund Original Transaction Is Not Located\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC );
                       }

                   }
                   if(memcmp(paczisoauto.processing_code,"02",2) == 0)
                   {
                       if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             &&(memcmp(AuthRec->auth_source_rev, "A",1) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                        {

                                        if((memcmp(buisness_prod_code,"REMI",4) == 0)
                                           &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0))
                                        {
                                                f_printf(stderr, "HS.Original Transaction located in LN-SAUTO \n");
                                                isunlock ( lnhauto );
                                                return ( SUCCESS );
                                        }
                                        else if (memcmp(buisness_prod_code,"REMI",4) != 0)
                                        {
                                                f_printf(stderr, "HS.Original Transaction located in LN-SAUTO \n");
                                                isunlock ( lnhauto );
                                                return ( SUCCESS );
                                        }
                                        else
                                        {
                                                f_printf(stderr, "HF.Original Transaction Is Not Located\n");
                                                isunlock ( lnhauto );
                                                return ( ECHEC );
                                        }

                        }
                       else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 )
                             && (memcmp(AuthRec->auth_source_rev, "R",1) == 0)
                             &&(memcmp(AuthRec->auth_req_date, GetDateYYMMDD (),6) == 0)
                             && (memcmp(AuthRec->reference_number, paczisoauto.retrieval_reference_number,12) == 0) )
                       {
                                        f_printf(stderr, "HE.Original Transaction Already Refunded Reject the Transaction\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC);
                       }
                       else
                       {
                                        f_printf(stderr, "HF.Original Transaction Is Not Located\n");
                                        isunlock ( lnhauto );
                                        return ( ECHEC );
                       }

                  }
                  else
                  {

                   if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) &&
                         ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 ) &&
                         ( ( memcmp ( paczisoauto.original_data_elements+10, AuthRec->auth_req_time, 6) == 0 )  /* Txn Date and Time */
                             && (memcmp(AuthRec->auth_source_rev, "A",1) == 0) ))
                        {
                                        f_printf(stderr, "HR.Original Transaction located in LN-AUTP  \n");
                                        isunlock ( lnhauto );
                                        return ( SUCCESS );
                        }
                   else if ( ( memcmp ( CardNumber, AuthRec->card_number, strlen ( CardNumber ) ) == 0 ) &&
                               (memcmp(AuthRec->auth_source_rev, "A",1) == 0)&&
                         ( memcmp ( paczisoauto.amount_transaction, AuthRec->transaction_amount, 12 ) == 0 )   )
                        {
                        	f_printf(stderr, "HR. Original Transaction located in LN-AUTP \n");
                        	isunlock ( lnhauto );
                        	return ( SUCCESS );
                        }

                  }
    }
    else
      f_printf(stderr, "Read Error iserrno [%d] \n", iserrno);
   return ( ECHEC);
}

int GetBranchId(char *branch)
{
   char city_code[25];
   char city_name[51];
   char city_name_1[51];
   char dea_mas_ser_bra[205];

   memset(city_code,0x00,sizeof(city_code));
   memset(city_name,0x00,sizeof(city_name));
   memset(city_name_1,0x00,sizeof(city_name_1));
   memset(dea_mas_ser_bra,0x00,sizeof(dea_mas_ser_bra));
   memset(branch,0x00,sizeof(branch));
   memset((char *)&paczpa1b , ' ' , sizeof(t_paczpa1b));
   memcpy(paczpa1b.pa1b_notabl      , "1B",    2);
   memcpy(paczpa1b.pa1b_code_banque , "00059", 5);
   memcpy(paczpa1b.pa1b_code_agence ,bflzbcom.lbcom_branch_id ,   20);
   if(isread(fdparam,&paczpa1b,ISEQUAL) < 0)
   {
      f_printf(stderr,"Error In reading Branch Master %d\n",iserrno);
      memcpy(branch,bfl_dealer_master.supplier_branch,8);
      f_printf(stderr,"1.Branch [%s]\n",branch);
      return ( ECHEC );
   }
   else
   f_printf(stderr,"\nSUCCESS IN READING BRANCH MASTER\n");
   f_printf(stderr,"\nCITY ID [%.24s]\n",paczpa1b.pa1b_city);
   isunlock(fdparam);
   isrelease(fdparam);
   memcpy(city_code,paczpa1b.pa1b_city,24);

   memset((char *)&bflzlpa02, ' ' , sizeof(t_bflzlpa02));
   memcpy(bflzlpa02.lpa02_table_cd,"02",2);
   memcpy(bflzlpa02.lpa02_bank_code,"00059",5);
   memcpy(bflzlpa02.lpa02_city_id,city_code,8);
   memset(bflzlpa02.lpa02_filler,' ',17);
   if(isread(lnparam,&bflzlpa02,ISEQUAL) < 0)
   {
      f_printf(stderr,"Error In reading City Master %d\n",iserrno);
      memcpy(branch,bfl_dealer_master.supplier_branch,8);
      f_printf(stderr,"2.Branch [%s]\n",branch);
      return ( ECHEC );
   }
   else
   f_printf(stderr,"SUCCESS IN READING CITY MASTER IN LN-PARAM\n");
   isunlock(lnparam);
   isrelease(lnparam);

   memcpy(city_name,bflzlpa02.lpa02_city_name,50);
   memcpy(dea_mas_ser_bra,"    ",4);
   memcpy(dea_mas_ser_bra+4,bfl_dealer_master.serving_cities,200);

   for(int i = 0 ;i <50;i++)
   {
     if(city_name[i] == ' ')
     {
       city_name_1[i]= '\0';
       i = 50;
     }
     else
     city_name_1[i] = city_name[i];
   }
   if(strlen(city_name_1) > 0 )
   {
   	f_printf(stderr,"CITY NAME [%s]\n",city_name_1);
   	if(strstr(dea_mas_ser_bra,city_name_1) == NULL)
   	{
        	f_printf(stderr,"CITY NOT FOUND IN DELAER SERVING CITIES [%s]\n",city_name_1);
        	memcpy(branch,bfl_dealer_master.supplier_branch,8);
        	f_printf(stderr,"3.Branch [%s]\n",branch);
   		return ( ECHEC );
   	}
   	else
        {
   	f_printf(stderr,"CITY FOUND IN DELAER SERVING CITIES [%.50s]\n",dea_mas_ser_bra);
      	memcpy(branch,bflzbcom.lbcom_branch_id,8);
       	f_printf(stderr,"4.Branch [%s]\n",branch);
        }
   }
   else
   memcpy(branch,bfl_dealer_master.supplier_branch,8);
   return ( SUCCESS );
}

int CheckCoBrandCard()
{
  int  Multiple_Cards = 0, k,Result = -1,value_new = 0 ;
  struct keydesc  skey1;
  t_bflzport newbflzport;
  char New_Dealer_Code[11];
  char New_Dealer_Code1[11];
  
            skey1.k_flags = ISDUPS;
            skey1.k_nparts = 1;
            skey1.k_part[0].kp_start = 19;
            skey1.k_part[0].kp_leng =  19;
            skey1.k_part[0].kp_type = CHARTYPE;
            memset((char *)&newbflzport, ' ', sizeof(t_bflzport));
            memset(New_Dealer_Code,0x00,sizeof(New_Dealer_Code));
            memset(New_Dealer_Code1,0x00,sizeof(New_Dealer_Code1));
            memcpy(newbflzport.lport_cust_id,bflzport.lport_cust_id, 19);
            f_printf(stderr, " Search Record Based On client id[%.19s]  \n",newbflzport.lport_cust_id);

            k = 0;
            do
            {
               Result = isstart ( lnporte , &skey1, 19, (char*)&newbflzport, ISGTEQ );
               k++;
            }
            while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );

            if(Result < 0)
            {
                     f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                     isclose(lnporte);
                     ReOpenLNPORTE1();
                     return (0);
            }
                     f_printf(stderr, " [%.24s], ** Record\n", &newbflzport);
		 if(isread ( lnporte , (char*)&newbflzport, ISCURR ) == 0 )
                     {
                           while(memcmp(bflzport.lport_cust_id , newbflzport.lport_cust_id , 19) == 0)
                           {
                                isrelease(lnporte);
                                isunlock(lnporte);
                         
                                if ( iserrno == EENDFILE ) break;
                                f_printf(stderr, " [%.24s], Secondary Key  Record, \n", &newbflzport);
                                if((memcmp(newbflzport.lport_activation_flag,"Y",1) == 0)&&
				   ((memcmp(newbflzport.lport_card_sub_type,"11",2) == 0)
				     ||(memcmp(newbflzport.lport_card_sub_type,"18",2) == 0)||(memcmp(newbflzport.lport_card_sub_type,"19",2) == 0)
				     ||(memcmp(newbflzport.lport_card_sub_type,"10",2) == 0)))
                                {
                                         memcpy(New_Dealer_Code,newbflzport.lport_dealer_cd,10);
                                         f_printf(stderr,"\nDealer Code... [%s]",New_Dealer_Code);

                                         Trim_string(New_Dealer_Code,New_Dealer_Code1,10);
                                         f_printf(stderr,"\nAfter Trim Dealer Code... [%s]",New_Dealer_Code1);
					 if(memcmp(New_Dealer_Code1,bfl_dealer_master.mid+(15-strlen(New_Dealer_Code1)),strlen(New_Dealer_Code1)) == 0)
					 {
					        f_printf(stderr, "SAME DEALER CO-BRAND CARD EXIST");
						isrelease(lnporte);
						isunlock(lnporte);
						isclose(lnporte);
						ReOpenLNPORTE1();
						return ( 2 );
					  }
                                          if((memcmp(newbflzport.lport_card_sub_type,"10",2) == 0) || (memcmp(newbflzport.lport_card_sub_type,"11",2) == 0))
					  Multiple_Cards++ ;
                                }
                                if(isread ( lnporte , (char*)&newbflzport, ISNEXT ) < 0 )
                                break;
                           }

			   if(Multiple_Cards >= 3)
                           {
			        f_printf(stderr, "MORE THAN or EQUAL TO THREE CO-BRAND CARDS EXIST");
                                isrelease(lnporte);
				isunlock(lnporte);
				isclose(lnporte);
				ReOpenLNPORTE1();
				return ( 2 );
                           }
                     }
                          isrelease(lnporte);
                          isunlock(lnporte);
                          isclose(lnporte);

                          ReOpenLNPORTE1();
                          return (0);
}
int ReOpenLNPORTE1()
{
         char filename[30];
         memset(filename, 0x00, sizeof(filename));
         sprintf(filename, "%s/LN-PORTE", getenv("MultipacDat"));
         lnporte = isopen(filename, ISMANULOCK+ISINOUT);
         if(lnporte < 0)
         {
          f_printf(stderr, "\n FILE LN-PORTE  open error [%d]", iserrno);
          return (ECHEC);
         }
         return(0);
}

int ReadAddonCard()
{
   	 char filename[30];
         memset(filename, 0x00, sizeof(filename));
         f_printf(stderr, "\n Reading Add On Card File\n");
         sprintf(filename, "%s/LN-ADDON", getenv("MultipacDat"));
         lnaddonporte = isopen(filename, ISMANULOCK+ISINOUT);
         if(lnaddonporte < 0)
         {
          	f_printf(stderr, "\n FILE LN-ADDON open error [%d]", iserrno);
          	return (ECHEC);
         }
    	 memcpy(lnaddport.laddon_card_no, paczisoauto.primary_account_number, 16);
         if(isread( lnaddonporte, (char *)&lnaddport, ISEQUAL ) < 0 )
         {
          	f_printf(stderr, "\n ADDON CARD [%.16s]READ ERROR [%d]",paczisoauto.primary_account_number, iserrno);
                isclose(lnaddonporte);
          	return (ECHEC);
         }
         isclose( lnaddonporte );
         return(SUCCESS);
}
#endif

int ReadAddOnFile(char *Cell_Num,char *AddOnCard)
{
         char filename[30];
         char Org_CellNumber[15];
         int Result = -1,CellNumber_Found = 0;
   	 int k = 0;
         f_printf(stderr, "\n Searching Cell Number in  Add On Card File\n");
         memset(filename, 0x00, sizeof(filename));
         memset(Org_CellNumber, 0x20, sizeof(Org_CellNumber));
         sprintf(filename, "%s/LN-ADDON", getenv("MultipacDat"));
         lnaddonporte = isopen(filename, ISMANULOCK+ISINOUT);
         if(lnaddonporte < 0)
         {
                f_printf(stderr, "\n FILE LN-ADDON open error [%d]", iserrno);
                return (ECHEC);
         }
        	struct keydesc  s1key;
		s1key.k_flags = ISDUPS;
		s1key.k_nparts = 1;
		s1key.k_part[0].kp_start = 38;
		s1key.k_part[0].kp_leng =  15;
		s1key.k_part[0].kp_type = CHARTYPE;
		memset((char *)&lnaddport, ' ', sizeof(t_lnaddport));
                memcpy(Org_CellNumber,Cell_Num+6, 10);
         	memcpy(lnaddport.laddon_mobile, Org_CellNumber, 15);

                k = 0;
            	do
            	{
               		Result = isstart ( lnaddonporte, &s1key, 15, (char*)&lnaddport, ISEQUAL );
               		k++;
            	}
            	while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );

	    	if(Result < 0)
            	{
                     	f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                     	isclose(lnaddonporte);
                     	return(ECHEC);
            	}
	     	f_printf(stderr, " CARD NUMBER[%.19s], ** Record\n", lnaddport.laddon_card_no);
	     	f_printf(stderr, " CELL NUMBER[%.10s], ** Record\n", lnaddport.laddon_mobile);
	     	f_printf(stderr, " OriginCell [%.10s], ** Record\n", Org_CellNumber);
             	if(isread ( lnaddonporte, (char*)&lnaddport, ISCURR ) == 0 )
             	{
                          while(iserrno != EENDFILE)
                          {  
				if(memcmp(Org_CellNumber, lnaddport.laddon_mobile, 10) == 0)
                                {
                                        isrelease(lnaddonporte); isunlock(lnaddonporte);
                                        f_printf(stderr, " [%.19s], Secondary Key  Record, \n", lnaddport.laddon_card_no);
                                        memcpy(AddOnCard,lnaddport.laddon_card_no,16);
                                        CellNumber_Found = 1;
                                        break;
                                }
                                else
                                {
                                        if(isread ( lnbcli, (char*)&lnaddport, ISNEXT ) < 0 )
                                        {
                     				f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                                                break;
                                        }
                                }
                          }

                          if (CellNumber_Found == 1)
                          {
                                isclose(lnaddonporte);
                          	return(SUCCESS);
                          }
                          else 
                          {
                                isclose(lnaddonporte);
                          	return(ECHEC);
                          }

	     	}
                else
                {
                     				f_printf(stderr, "RECORD NOT FOUND, iserrno[%d]\n", iserrno);
                     				isclose(lnaddonporte);
                     				return(ECHEC);
                }
         isclose( lnaddonporte );
         return(ECHEC);
}


#ifdef ADDON_PHASE_2

int Check_First_Txn()
{
	char filename[30];
	memset(filename, 0x00, sizeof(filename));
	f_printf(stderr, "\n Reading Add On Card File\n");

	sprintf(filename, "%s/LN-ADDON", getenv("MultipacDat"));
	lnaddonporte = isopen(filename, ISMANULOCK+ISINOUT);
	if(lnaddonporte < 0)
	{
		f_printf(stderr, "\n FILE LN-ADDON open error [%d]", iserrno);
		return (ECHEC);
	}

	f_printf(stderr, "CARD NUMBER IN STRUCTURE - [%.16s]\n",lnaddport.laddon_card_no);

	memset((char *)&lnaddport, ' ', sizeof(t_lnaddport));
	memcpy(lnaddport.laddon_card_no, paczisoauto.primary_account_number, 16);
	if(isread(lnaddonporte, &lnaddport, ISEQUAL) < 0)
	{
		f_printf(stderr, "Card number [%.19s] is not present in LN-ADDON file iserrno [%d]\n",
		paczisoauto.primary_account_number, iserrno);
		isclose(lnaddonporte);
		return ECHEC ;
	}
	

	f_printf(stderr,"FIRST TXN FLAG  [%.1s]\n", lnaddport.laddon_first_txn);
	f_printf(stderr,"OUTPUT of IF ELSE - > %d\n",memcmp(lnaddport.laddon_first_txn,"Y",1));

	if(memcmp(lnaddport.laddon_first_txn,"Y",1) != 0)
	{
		f_printf(stderr, "1st Transaction for ADD-on Card [%.19s]\n",paczisoauto.primary_account_number);
		memcpy(lnaddport.laddon_first_txn,"Y",1);
		if(isrewrite(lnaddonporte,&lnaddport) < 0)
		{
			f_printf(stderr,"Unable to update lnaddonporte is errno %d\n", iserrno);
			isclose(lnaddonporte);
			return ECHEC ;
		}
		else
		{
			f_printf(stderr, "LN-ADDON updated succesfully..\n");
			isclose(lnaddonporte);
			return SUCCESS ;
		}
	}

	isclose(lnaddonporte);
	return ECHEC ;
}
#endif

int UpdateLNCrdDet()
{
        int Result =0;

        f_printf(stderr,"UPDATING LN-CRDDET \n");

        memcpy(bflcarddet.lcrd_last_modify_date,GetDateYYMMDD(),6);
        memcpy(bflcarddet.lcrd_user_name,"ONUS    ",8);

	f_printf(stderr,"[%s]",&bflcarddet);
        
	Result = isrewrite ( lncrd , (char*)&bflcarddet);
        if ( Result != 0 )
        {
		f_printf(stderr,"Update LN-CRDDET error [%d]\n ",iserrno);	
                if(iswrite( lncrd , (char*)&bflcarddet) < 0)
                {
                        f_printf ( stderr, "BFL CRDDET: WRITE ERROR  %d\n", iserrno );
                        return ( ECHEC );
                }
        }
	else if(Result == 0)
	f_printf("update success \n");

        return ( SUCCESS );
}

int ReadCRDDET()
{
        char filename[100];
        f_printf(stderr,"READING LN-CARDCET \n");
        memset((char *)&bflcarddet,0x00, sizeof(t_bflcarddet));
        memset(filename, 0x00, sizeof(filename));
        sprintf(filename, "%s/LN-CRDDET", getenv("MultipacDat"));
        lncrd = isopen(filename, ISMANULOCK+ISINOUT);
        if(lncrd < 0)
        {
                f_printf(stderr, "\n FILE LN-CRDDET  open error [%d]", iserrno);
                return (ECHEC);
        }

        memcpy(bflcarddet.lcrd_card_no,bflzport.lport_card_no ,19);

        if(isread(lncrd,(char *)&bflcarddet,ISEQUAL) != 0)
        {

                if(iserrno = ENOREC)
                {
                        memcpy(bflcarddet.lcrd_cust_id,bflzport.lport_cust_id, 19);
                        memcpy(bflcarddet.lcrd_wrong_pin_attempt, bflzport.lport_wrong_pin_attempt, 1);
                        memcpy(bflcarddet.lcrd_wrong_otp_attempt, bflzport.lport_wrong_otp_attempt, 1);
                        memcpy(bflcarddet.lcrd_new_rrn,bflzport.lport_new_rrn, 12);
                        memcpy(bflcarddet.lcrd_last_auth_dt,bflzport.lport_last_auth_dt, 6);
                        
			f_printf(stderr,"DATA NOT FOUND IN LN-CRDDET, POPULATING STRUCTURE \n");
                        f_printf(stderr,"[%s]",&bflcarddet);
                }
        }
        else
	{
        	f_printf(stderr,"DATA FOUND IN LN-CRDDET\n");
		f_printf(stderr,"[%s] \n",&bflcarddet);	
	}
        return 0;
}


