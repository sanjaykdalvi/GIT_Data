/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_account.c                                      *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 14-Feb-2000                                         *
 * Description         : ONUS account related functions                      *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #  Date      Programmer/Reviewer  Description                             *
 *---------------------------------------------------------------------------*
 * 1  13-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS   *
 * 2  12-Dec-04 Saju Joseoph         ICICI Cash at AC level                  *
 *    16-Mar-04 Aji Gopal            Reviewed                                *
 * 3  21-Jan-04 A K Azhakesh         Funds in clearing is added while        *
 * 4                                 calculating the OTBL                    *
 * 5  17-March-05 Veeresh            BR-MXP-0502-0024                        *
 * 6  03-Aug-05 Michel Alex CR-MXP-0411-0241 NTB Cash Lt checking- Developer *
 * 6  04-Aug-05 Avinash Rao CR-MXP-0411-0241 NTB Cash Lt checking- Reviewer  *
 * 7  11-Nov-05 Avinash Rao CR-MXP-0509-0579 Cash at Acc level     Developer *
 * 7  11-Nov-05 Gilu Alex   CR-MXP-0509-0579 Reviewed                        *
 * 8  20-Jan-05 Michel Alex BR-MXP-0601-0023 To avoid Negative cash available*
 * 8  20-Jan-05 Avinash Rao BR-MXP-0601-0023 Reviewed                        *
 * 9  24-Jan-06 Michel Alex CR-MXP-0510-0597ASyn Cash Lt checking- Developer *
 * 9  02-Feb-06 Veeresh E   CR-MXP-0510-0597ASyn Cash Lt checking- Reviewer  *
 * 10 10-Feb-06 Ranjith TV  CR-MXP-0602-0671 Barrista card changes           *
 * 10 10-Feb-06 Avinash Rao CR-MXP-0602-0671 Reviewed barrista card changes  *
 * 11 18-Apr-06 Ranjith TV  CR-MXP-0603-0694 Adlabs card changes and 
					     card type checking              * 
 *****************************************************************************/

/* Darshan Added to include New BOI Platinum Master Card bin for checking transaction amount greater the 1000.00 on 10.07.2013 */
#define BOI_ADD_NEW_BIN     1

/* Added on 07/11/2012 for Muthoot Card Closure */
  #define MUTHOOT_CARD_CLOSURE      1  

/*To remove DB call fro BFL*/
#define REMOVE_DB
/* Chowdesha added on 05-Oct-2011 for Fees calculation */
 #define PREPAID_CARD_FEES  1    

/* Chowdesha added for HSBC DOP POS acquiring */
/* #define HSBC_DOP_POS  1  */
/* Added for ING Vysya Bank Issuance on 29.05.2013 */
  #define ING_VYSYA_ISSUANCE 1   

/* Bharath Added For Retial CUG */
 #define RETAIL_CUG 1 
/* NAGESH BASWANTHARAO ADDDED FOR SHELL*/
   #define SHELL_PHASE1 1 
   #define SELECT_CITY  1
   #define PHOENIX_MALL 1 
/* NAGESH BASWANTHARAO ADDDED FOR MUTHOOT_ISS_ACQ2 */
  #define MUTHOOT_ISS_ACQ2 1   

/* NAGESH BASWANTHARAO ADDDED FOR VIJAYA_GIFT_CARD_FEES_DR */
  #define VIJAYA_GIFT_CARD_FEES_DR 1   
  #define BOBL_PREPAID 1
  #define BFL_PHASE2   1
  
  #define CORP_DR_FLD51 1

  #define LODHA_PALAV 1 
  #define CARDLEVEL_AUTHTIME 1
  #define MCI_MONEY_SEND 1  

  #define BOI_CRDIT_NEWBIN 1
  #define BFL_CUG_CARD  1
  #define EDEN_ACT   1

  #define MRL_383       1
  #define MRL_461       1
  #define MRL_483	1
  #define BOI_GPPC  1


 #define SHELL_RELOAD_ISSUE 1   /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */

 #define EDENRED_REPORT 1 /* Added by Amit on 7 APR 2017 for PREQ2968 Eden Red Report related Changes */

 #define EDENRED_NEG_BAL_FIX 1 /* Added by Amit on 3 MAY 2017 to resolve Eden Red Negative Balance issue */

/* Ramya Added for BOI BIN ....*/
/*  #define BOI_NEWBIN  1  */

/*  #define HSBC_DOP_PHASE2 1 */ 

/*  Chowdesha added for CBA  */
 #define  CBA   1 
 #define MVISA_ISSUER 1
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
 #include <paczpoau.h>
 #include <paczisoauto.h>
 #include <paczpa05.h>
 #include <pcrzbale.h>
 #include <paczpa02.h>
 #include <paczpa62.h>
 #include <rthauth.h>
 #include <ath-trc.h>
 #include <paczau00.h>
 #include <onus.h>
 #include <bflcarddet.h>
#include "onus_account.h"
 
 #include <paczpaod.h>
 t_paczpaod paczpaod;

#define ABN_MIN_AVL		500
#define ABN_REG_BARISTA_MIN_AVL	150	 /* Ranjith CR-MXP-0602-0671 */
#define ABN_REG_ADLABS_MIN_AVL	250	 /* Ranjith CR-MXP-0603-0694 */
#define BARISTABIN		"541538"
#define ADLABSCLASSIC		"541538" /* Ranjith CR-MXP-0603-0694 */
#define ADLABSGOLD		"542505" /* Ranjith CR-MXP-0603-0694 */

#define PROD_232

#define MUTHOOTGOLD  1

#define  SHELL_RELOAD_VOID_IS 1

/* Chowdesha/Preema added on 19-Mar-2009 to support partial reversal */
#define PARTIAL_REVERSAL   1

/* Chowdesha  PREPAID CARD 12-04-2010 */
  #define PREPAID_CARD  1    

/* Chowdesha added for GIFT CARD */
#define GIFT_CARD  1

/* Chowdesha added for IDBI DOP on 04-05-2011  */
#define IDBI_DOP  1 

/* Aded for JK CMS  acquiring */
  #define JK_CMS_APPLY 1    

#define MRL_288 
extern char rsa_flag[];
int UpdateRsaBcom();
extern char Limit_Check_Bypass_Flag[];
/* Added for JK CMS OTBL Updation  */
  #define JK_CMS_APPLY_UPD 1  

/* #define SHELL_SALE 1 */

/* Added for decline report rectification requested by J&K bank */
 #define SUBRESULT_CODE_NEW     1
#define VIJAYA_GIFT_CARD 1
#define VIJAYA_TRAVEL 1
	
#define MATCHED					0
#define UNMATCHED				1

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */ 
extern	int 	PIDROUT;
extern	int		snFlagAlarm ; 

extern t_paczisoauto	paczisoauto;
extern t_paczpa05		paczpa05;
extern t_pcrzbale		pcrzbale;
extern t_paczpoau		paczpoau;
extern t_paczpa02		paczpa02;
extern t_rthauth		rthauth;
extern t_ath_trc		athtrc;	
extern t_paczau00	paczau00;	
#ifdef BFL_CUG_CARD
#include <bflzbcom.h>
#include <bflzport.h>
#include <bflzbcli.h>
#include <bflzparm.h>
#include <BFLPROC.h>
#include <bflauth00.h>
#include <bflhistoryauth.h>

extern t_bflzport    bflzport;
extern t_bflzbcom    bflzbcom;
extern t_bflzbcli    bflzbcli;
extern t_procdata    procdata;
extern t_bflhistauth bflhistauth;
extern t_bflauth00   bflauth00;
extern t_bflzlpa03   bflzlpa03;
extern int lnbcli ; /*BFL CARD Client file */
extern int lnbcom ; /*BFL CARD ACCOUNT MASTER */
extern int lnporte ;/*BFL CARD MASTER */
extern int lnsauto; /*BFL AUTHORISATION */
extern int lnhauto; /*BFL AUTHORISATION HISTORY */
extern int lnparam; /*BFL PARAMETERS*/

extern t_bflcarddet  bflcarddet;
extern int lncrd;
char* BFL_GetBalance ( t_bflzbcom* Account );
char* BFL_GetRedeemBalance ( t_bflzbcom* Account );
char* BFL_GetOpenECSBalance( t_bflzbcom* Account );
char* BFL_CheckAccountAvailable ( int Type /* 1- Normal 2 - Advice Txn */ );
int BFL_CheckRedeemPointAvailable(char *);
int BFL_CheckOpenECSAvailable(char *);
extern char RedeemPointBalance[];
extern char Bfl_LocalAmount[];
extern char Bfl_AdvEmiAmount[];
extern char Bfl_AdvEcsAmount[];
extern int  Card_First_Trans;
extern int  chargeslip_first_fee;
extern int  Cibil_Done;
#ifdef MRL_383
	extern int rset_card_1st_fee;
#endif
int  Update0100RedeemPoints(char*);
int  Update0400RedeemPoints();
int  BFL_UpdateOpenEcsEmi ( char * Response );
int  BFL_Update0400OpenEcsEmi ();
int  BFL_UpdatePin_Count(char *);

#define MRL_207 1
#ifdef MRL_207
int  BFL_UpdateOtp_Count(char *);
#endif

void UpdateBFLCardFirstUse();
extern char Original_Amount[13];
extern       char    CODE_SAUTO_HAUTO;
#ifdef BFL_PHASE2
extern char Prefered_Limit[];
extern int Ecs_Waive_Off;
char  Limit_Before_Void[13];
char  Limit_After_Void[13];
#endif
#endif

#ifdef SHELL_RELOAD_VOID_IS

int iReloadPresentFile(char * reload_card, char * reload_rrn);
int iReloadUpdateFile(char *reload_card, char * reload_rrn);
#endif

static double			FltAccountAmount = 0.0;

/* ISAM Data File Descriptors */
extern int		fdparam;	/* Parameter file */


#ifdef MUTHOOT_CARD_CLOSURE
char szFld_54_Muthoot[13];
#endif


/* Function prototypes */
double CalculateTheRate ( char* BankCode, char* PaymentStatus );
extern double atof(char *);
extern int GetErrorCodeUsingReferalCode ( char ReferalCode, char* SubResult );
extern char* GetDateYYMMDD ( void );

double CalculateTheRate ( char* BankCode, char* PaymentStatus );

extern char LOCAL_AMOUNT[13];   /* Azhakesh*/
/* preema for Muthhot DR 100609 */
char Institution_Id_code[11];     
char orig_data_OD[24];            

int IsBOIBIN( char * BIN );	/* Added by ACR, 28-10-2005 */
/*
void UpdateCountryClubAccountBalance(int trnstype);
*/


#ifdef PREPAID_CARD 
	double	FltFeesAmount = 0.0;
    double	FltConvPercent = 0.0;
	double FltConvPercent_amt = 0.0;
	int iGetFees(char *Card_nember, char *fees_amt1, int fees_field, char *atm_amtlimit);
#endif 

#ifdef GIFT_CARD 
	double	FltGiftFeesAmount = 0.0;
#endif 

#include <paczpatm.h> 

/* #ifdef HSBC_DOP_PHASE2 */
#ifdef HSBC_DOP_POS
	extern    double gcba_fees_amt; 
	extern int DoPBank(char *bank_code);
	double dfee_amt= 0.0;
#endif 

#ifdef CBA
	extern    double gcba_fees_amt; 
	extern char strcbs_bal_amt[ ]; 
   extern char CBS_LOCAL_AMT [ ]; 
#endif

#ifdef CORP_DR_FLD51
extern char User[];
#endif

#ifdef LODHA_PALAV
extern t_paczpoau               dstpaczpoau;
#endif

/* Commented BY Nagesh baswantharao */
#ifdef NEVER 
extern int iUpdate_account_otbl_amount(char *card_number, double damt);
extern int iUpdate_Account_block_amount(char *card_number, double damt);
#endif

/* 
ACR:
Note: This function should be called only checking the account level OTBL 
For HSBC DoP, all the reload amount will be in Funds clearing field which will be
availlable to the cardholder only after the funds are received during clearing process
But for reload limit check this funds in clearing should also be considered hence this
functions can't be used.
*/


double CalculateTheRate ( char* BankCode, char* PaymentStatus )
{
	int 		i = 0;
	int			Index = 0;
	char		CharRate[6];
	t_paczpa62	paczpa62;

	for ( i = 0; i < 12 ; i++ )
	{
		if ( PaymentStatus[i] == '1' )
			Index++;
	}

	FPRINTF ( stderr, "INDEX = %d \n", Index );

	memset ( (char*)&paczpa62, ' ', sizeof ( t_paczpa62 ) );
	memcpy ( paczpa62.pa62_code_table, "62", 2 );
	memcpy ( paczpa62.pa62_code_banque, BankCode, 5 );

	if ( isread ( fdparam, (char*)&paczpa62, ISEQUAL ) < 0 )
	{
		FPRINTF ( stderr, "Payment statement parameters not found for bank \
		 = [%.5s] \n", BankCode );
 
                isunlock ( fdparam ); /* V5 theta migration Michel 09.02.2006*/
		return ( -1.0 );
	}
	isunlock ( fdparam );

	memset ( CharRate, 0, 6 );
	memcpy ( CharRate, paczpa62.pa62_rate_0 + ( Index * 5 ), 5 );

	for ( i = 0; i < 5 ; i++ )
	{
		if ( ( CharRate[i]  < '0' ) ||
		     ( CharRate[i]  > '9' ) )
		{
			FPRINTF ( stderr, "Rate historic payment contain garbage = \
			  [%.5s] \n", CharRate );
			return ( - 1.0 );
		}
	}

	return ( atof ( CharRate ) / 10000.00 );
}




int iTxnStatus()
{
	return UNMATCHED;
}


int iFindTxnType( char *proc_code )
{

       #ifdef VIJAYA_TRAVEL 
        if( memcmp( proc_code, "21", 2 ) == 0 )
                return DEBIT ;
       #endif

        #ifdef SHELL_RELOAD_VOID_IS
        if( memcmp( proc_code, "610000", 6 ) == 0 )
                return CREDIT ;
        #endif

	#ifdef MVISA_ISSUER
	if( memcmp( proc_code, "260000", 6 ) == 0 ) /* emPay 18122015 mVisa Iss Project */
		return DEBIT ;
	#endif

	if( proc_code[0] == '2' )
		return CREDIT ;
	else if( proc_code[0] == '0' )
		return DEBIT ;
	else if( proc_code[0] == '4' )
		return TRANSFER ;
	else if( memcmp( proc_code, "17", 2 ) == 0 || memcmp( proc_code, "09", 2 ) == 0 )	 /* Cash advance transactions */
		return DEBIT ;
        else if( memcmp( proc_code, "11", 2 ) == 0)
                return DEBIT ;
	else if( memcmp( proc_code, "83", 2 ) == 0 )
		return DEBIT ; 
        #ifdef EDEN_ACT
	else if( memcmp( proc_code, "84", 2 ) == 0 )
		return DEBIT ; 
        #endif

	return NON_FINANCIAL ;
}




/*
 This function checks whether the given bin belongs to BOI or not. If the
 input BIN belongs to BOI, returns SUCCESS, else returns ECHEC. This function
 is implemented for minimum amount check for the following BOI BINs. Here BOIs
 subsidaries are not considered.
*/
int IsBOIBIN( char * BIN )
{
	if( memcmp( BIN, "54203430", 8 ) == 0 || 
		memcmp( BIN, "55435130", 8 ) == 0 || 
		memcmp( BIN, "55435030", 8 ) == 0 ||
		memcmp( BIN, "43111930", 8 ) == 0 ||
	      #ifdef BOI_CRDIT_NEWBIN
		memcmp( BIN, "483827", 6 ) == 0 ||
		memcmp( BIN, "483828", 6 ) == 0 ||
		memcmp( BIN, "400147", 6 ) == 0 ||
	      #endif
		memcmp( BIN, "43407130", 8 ) == 0 ||
              #ifdef BOI_NEWBIN
		memcmp( BIN, "45762300", 8 ) == 0 ||
              #endif
		memcmp( BIN, "54117541", 8 ) == 0 ||
		memcmp( BIN, "54117540", 8 ) == 0 ||
		memcmp( BIN, "54117410", 8 ) == 0 ||
		memcmp( BIN, "54002107", 8 ) == 0 ||
           #ifdef BOI_ADD_NEW_BIN
                memcmp( BIN, "52775900", 8 ) == 0 ||
           #endif
		memcmp( BIN, "42606330", 8 ) == 0 
	  )
	{
		return( SUCCESS );
	}
	return (ECHEC) ;
}

#ifdef NEVER
/* Added by ACR for Andhra Bank restricted cards change */
/* Date : 29-09-2008 */
void UpdateCountryClubAccountBalance(int trnstype)
{
	int country_club_limit = FALSE ;
	char acc_cash_avail[13], acc_cash_limit[13] ;
	char szMID[16]  ;
	char TempAmt[13];
	double FltUsedAmt, FltAccUsedAmt ;

	memcpy( szMID, paczisoauto.card_acceptor_identification_code, 15 );
	szMID[15] = 0 ;

        f_printf(stderr, "\nCOUNTRY 400 trnstype   [%d]\n", trnstype );

	if( memcmp( paczisoauto.acquiring_institution_identification_code ,"450623", 6 ) == 0 )
	{
		if( memcmp( szMID, "000010255341297", 15 ) == 0 )
		{
			country_club_limit = TRUE ;
		}
	}

	if( country_club_limit == FALSE )
	{
		memcpy(acc_cash_avail, pcrzbale.bale_cash_available,12);
		f_printf(stderr, "BEFORE UPDATE \n" );
		f_printf(stderr, "ACC CASH AVAIL [%.12s]\n",pcrzbale.bale_cash_available  );
		acc_cash_avail[12] = 0;

		memcpy ( TempAmt, paczisoauto.replacement_amounts, 12 );
		TempAmt[12] = 0;
		FltUsedAmt = atof ( TempAmt );
        FltAccUsedAmt = FltUsedAmt ;
		
		if( FltAccUsedAmt == FltAccountAmount )
			FltAccUsedAmt = 0.0 ;
		
		if( trnstype == 0100 )
		{
			/* BR-MXP-0601-0023 Michel 20.01.2006 Begin */																	
			if ( atof(acc_cash_avail) - FltAccountAmount < 0.0 )
				memcpy(pcrzbale.bale_cash_available, "000000000000", 12);
			else
			{
				sprintf(acc_cash_avail,"%012.0f",atof(acc_cash_avail) - FltAccountAmount);
				memcpy(pcrzbale.bale_cash_available,acc_cash_avail,12);
			}
		}
		else if( trnstype == 0400 )
		{
			/* BR-MXP-0601-0023 Michel 20.01.2006 Begin */                    
			if (atof(acc_cash_avail)+(FltAccountAmount- FltAccUsedAmt) < 0.0 )
				memcpy(acc_cash_avail, "000000000000", 12);
			else
				sprintf(acc_cash_avail,"%012.0f",atof(acc_cash_avail) +(FltAccountAmount- FltAccUsedAmt ));                                                       
                                                                                
			/* BR-MXP-0601-0023 Michel 20.01.2006 End   */  
			memcpy(acc_cash_limit,pcrzbale.bale_plafond_cash,12);             
			acc_cash_limit[12] = 0;
			if(atof(acc_cash_limit) < atof(acc_cash_avail))
				memcpy(pcrzbale.bale_cash_available,acc_cash_limit,12);         
			else /* Added by Michel 10.10.08 */
				memcpy(pcrzbale.bale_cash_available,acc_cash_avail,12);         
		
			f_printf(stderr, "COUNTRY 400 FltAccountAmount [%f]\n", FltAccountAmount );
			f_printf(stderr, "COUNTRY 400 FltAccUsedAmt    [%f]\n", FltAccUsedAmt );
			f_printf(stderr, "COUNTRY 400 acc_cash_limit   [%s]\n", acc_cash_limit );
			f_printf(stderr, "COUNTRY 400 acc_cash_avail   [%s]\n\n", acc_cash_avail );
			f_printf(stderr, "COUNTRY 400 pcrzbale.bale_cash_available   [%.12s]\n\n", pcrzbale.bale_cash_available );
		}
		f_printf(stderr, "AFTER  UPDATE \n" );
		f_printf(stderr, "ACC CASH AVAIL [%.12s]\n",pcrzbale.bale_cash_available  );
	}
}
#endif

#ifdef PREPAID_CARD 
int iGetFees(char *Card_nember, char *fees_amt1, int fees_field, char *atm_amtlimit)
{
   int iret, i;
   char fees[13];
   char filename[100];
   char loc_fees_cod[3];
   t_paczpatm paczpatm;
  
   f_printf(stderr, "\n In iGetFees Function ........."); 
   memset(fees, 0x00, sizeof(fees));
   memset(&paczpatm, ' ', sizeof(t_paczpatm));
   f_printf(stderr, "CardNumber[%s]\n", Card_nember);
   for(i = 9;i > 5; --i)
   {
       memset(&paczpatm, ' ', sizeof(t_paczpatm));
	   memcpy(paczpatm.patm_notabl, "TM", 2); 
     /*  f_printf(stderr, "BIN[%s]\n",paczpatm.patm_bin); */
      memcpy(paczpatm.patm_bin,Card_nember,i);
      /* sprintf(paczpatm.patm_transaction_code, "%02s",  fees_field);  */
      memset(loc_fees_cod, 0x00, sizeof(loc_fees_cod));
      sprintf(loc_fees_cod, "%02d",  fees_field);
      memcpy(paczpatm.patm_transaction_code,loc_fees_cod, 2);
      /* f_printf(stderr, " FEES_STRUCT[%s]\n",&paczpatm); */
	  
      if(isread(fdparam,&paczpatm,ISEQUAL)==0)
      {

              memcpy(fees_amt1, paczpatm.patm_initial_one_time_fee, 12); 
			  memcpy(atm_amtlimit, paczpatm.patm_amount_limit, 12); 
  	  	      f_printf(stderr, "\n patm_atm_withdrawal_fee [%.12s]", fees_amt1); 	
              f_printf(stderr, "\n patm_atm_max_amt [%.12s]", atm_amtlimit); 	
 		      isunlock ( fdparam ); 
		  	  return SUCCESS; 


           /*  
		  switch (fees_field)
		  {
		      case  4: 
		          memcpy(fees_amt1, paczpatm.patm_atm_withdrawal_fee, 6); 
  				  f_printf(stderr, "\n patm_atm_withdrawal_fee [%.6s]", fees_amt1); 
		  	      return SUCCESS; 

               case 5:
		 		   memcpy(fees_amt1, paczpatm.patm_atm_balance_enquiry_fee, 6); 
   				   f_printf(stderr, "\n patm_atm_withdrawal_fee [%.6s]", fees_amt1); 
		  	       return SUCCESS; 
                
				default:
		   			 f_printf(stderr, "\n Invalid field value "); 
				     return ECHEC; 				  
			 }
		*/

      }
   }
   
   f_printf(stderr, "\n Fees Not defined "); 
   return ECHEC; 
}
#endif 



#ifdef BFL_CUG_CARD
char *BFL_CheckAccountAvailable ( int Type )
{
	char            TempAmount[13];         
	char            TxnAmount[13];
	double          FltTxnAmount = 0.0;
	char            FundsClearing[10];       
	double          FltFundsClearing = 0.0;  
	char            CreditLimit[13];
	double          FltCreditLimit = 0.0;
	char            CurrentPurchase[13];
	double          FltCurrentPurchase = 0.0;
	char            CurrentCash[13];
	double          FltCurrentCash = 0.0;
	char            CurrentBalance[13];
	double          FltCurrentBalance = 0.0;
	char            CumulAuth[13];
	double          FltCumulAuth = 0.0;
	double          CalculatedAmount = 0.0;
	double          Rate = 0.0;
	double          FltExcessLimit = 0.0;
	char            SubResult[7];
	double loc_FltAccountAmount = 0.00;

	/* BFL_EXCESS_PAMYENT : below two variables added*/
	char            TotalUtlDebt[13];
	double          FltTotalUtlDebt = 0.0;
	
	#ifdef MRL_461
		char 	AdvEMIAmnt[13];
		double 	FltAdvEmiAmnt = 0.0;
		double	loc_NetAmnt = 0.0;
		memset(AdvEMIAmnt, 0, sizeof(AdvEMIAmnt));
	#endif

	memset ( TxnAmount, 0, 13 );
	memset ( FundsClearing, 0 , 10 );
	memset ( CreditLimit, 0, 13 );
	memset ( CurrentPurchase, 0, 13 );
	memset ( CurrentCash, 0, 13 );
	memset ( CurrentBalance, 0 ,13 );
	memset ( CumulAuth, 0, 13 );
	/* BFL_EXCESS_PAMYENT */
	memset ( TotalUtlDebt, 0, 13 );
	
	t_bflzpa21 bflzpa21;
	double ShadowLimit = 0.0;
	char Shadowperc[4];
	char ShadowCapLim[13];
	double FltShadowperc;
	double FltShadowCapLim;


	f_printf ( stderr, "\n BFL-ACCOUNT AVAILABLE FUNCTION () \n");


	memcpy ( TxnAmount, paczisoauto.amount_transaction, 12 );
	FltTxnAmount = atof ( TxnAmount );


	memcpy ( CreditLimit, bflzbcom.lbcom_acc_credit_limit, 12 );
	CreditLimit[12] = 0;
	FltCreditLimit = atof ( CreditLimit );



	memcpy ( CurrentBalance, bflzbcom.lbcom_acc_tot_util, 12 );
	CurrentBalance[12] = 0;
	FltCurrentBalance = atof ( CurrentBalance );


	memcpy ( CumulAuth, bflzbcom.lbcom_acc_tot_auth, 12 );
	CumulAuth[12] = 0;
	FltCumulAuth = atof ( CumulAuth );


	memcpy ( TempAmount, bflzbcom.lbcom_acc_temp_limit+1, 12 );
	TempAmount[12] = 0;
	FltExcessLimit = atof ( TempAmount );
	
	#ifdef MRL_461
		f_printf(stderr, "Advance EMI Amount from Structure = [%.12s]\n",procdata.adv_emiamount);
		memcpy(AdvEMIAmnt, procdata.adv_emiamount, 12);
		AdvEMIAmnt[12] = 0;
		FltAdvEmiAmnt = atof (AdvEMIAmnt);
		f_printf(stderr, "Advance EMI Amount = [%f]\n", FltAdvEmiAmnt);
	#endif

	/* BFL_EXCESS_PAMYENT */
	memcpy ( TotalUtlDebt, bflzbcom.lbcom_acc_total_utl_debt, 12 );
	TotalUtlDebt[12] = 0;
	FltTotalUtlDebt = atof ( TotalUtlDebt );

	f_printf ( stderr, "Account Number    = [%.19s] \n", bflzbcom.lbcom_customer_id);
	f_printf ( stderr, "Credit Limit      = [%f] \n", FltCreditLimit );
	f_printf ( stderr, "Current Balance   = [%f] \n", FltCurrentBalance );
	f_printf ( stderr, "Cumul Auth        = [%f] \n", FltCumulAuth );
	f_printf ( stderr, "Excess Limit      = [%f] \n", FltExcessLimit );
	f_printf ( stderr, "Total Util Debt/Excess Payment = [%f] \n", FltTotalUtlDebt );
	f_printf ( stderr, "paczisoauto.other_amounts   = [%.19s] \n", paczisoauto.other_amounts );

	if(bflzbcom.lbcom_acc_temp_limit[0] == '-')
	{
	#ifdef MRL_483	
		CalculatedAmount = FltCreditLimit - FltExcessLimit - FltCurrentBalance - FltCumulAuth;
	#else
		CalculatedAmount = FltCreditLimit - FltExcessLimit - FltCurrentBalance - FltCumulAuth + FltTotalUtlDebt;
	#endif
		f_printf(stderr, "1. Calculated Amount = [%.f]\n",CalculatedAmount);
	}
	else
	{
	#ifdef MRL_483
		CalculatedAmount = FltCreditLimit + FltExcessLimit - FltCurrentBalance - FltCumulAuth;
	#else
		CalculatedAmount = FltCreditLimit + FltExcessLimit - FltCurrentBalance - FltCumulAuth + FltTotalUtlDebt;
	#endif	
		f_printf(stderr, "2. Calculated Amount = [%.f]\n",CalculatedAmount);	
	}
	if ( CalculatedAmount < 0.0 )
		CalculatedAmount = 0.0;

	Rate = 1.0;
	CalculatedAmount = Rate * CalculatedAmount;
	f_printf ( stderr, "BFL-Calculated Amount = [%.f] \n", CalculatedAmount );

	sprintf ( TempAmount, "%012.0f", CalculatedAmount );

	loc_FltAccountAmount = FltTxnAmount;
	
	#ifdef MRL_461
	loc_NetAmnt = loc_FltAccountAmount - FltAdvEmiAmnt;	
	f_printf(stderr, "CalculatedAmount [%.f] loc_FltAccountAmount [%.f] FltAdvEmiAmnt [%.f] loc_NetAmnt [%.f]\n",CalculatedAmount,loc_FltAccountAmount,FltAdvEmiAmnt,loc_NetAmnt);	
	#endif
	
	f_printf(stderr,"Limit_Check_Bypass_Flag [%.1s] \n",Limit_Check_Bypass_Flag);
	if((memcmp(Limit_Check_Bypass_Flag,"Y",1) == 0) && (CalculatedAmount > 0) &&((memcmp(transaction_source,"SI",2) ==0) || (memcmp(transaction_source,"SP",2)==0)))
	{
		f_printf(stderr,"Limit_Check_Bypass_Flag is enabled \n");
	
	}
	else if ( CalculatedAmount < loc_NetAmnt)/* MRL_461 */
	{
		/* MRL_428 */
		{

			memset(&bflzpa21, ' ', sizeof(bflzpa21));
			memcpy(bflzpa21.lpa21_table_cd, "21", 2);
			memcpy(bflzpa21.lpa21_bank_code, "00059", 5);
			memcpy(bflzpa21.lpa21_customer_risk, bflzbcom.lbcom_cust_risk_clasif, 3);
			memcpy(bflzpa21.lpa21_asset_risk, procdata.asset_risk, 2);

			f_printf(stderr, "READING PARAMETER MASTER FOR SHADOW PERC [%.3s] [%.2s].\n",
				bflzbcom.lbcom_cust_risk_clasif, procdata.asset_risk);

			if(isread(lnparam, &bflzpa21, ISEQUAL)==0)
			{
				memcpy(Shadowperc, bflzpa21.lpa21_shadow_perc, 3);
				Shadowperc[3]='\0';
				FltShadowperc = atof(Shadowperc);
				FltShadowperc = FltShadowperc/100.0;
				
				memcpy(ShadowCapLim, bflzpa21.lpa21_cap_amount, 12);
				ShadowCapLim[12]='\0';
				FltShadowCapLim = atof(ShadowCapLim);

				f_printf(stderr, "SHADOW [%f], CAP [%f]\n", 
					FltShadowperc, FltShadowCapLim);

				if((FltShadowperc!=0) && (FltShadowCapLim!=0))
				{
					if((FltCreditLimit*FltShadowperc)
						<
					FltShadowCapLim)
						ShadowLimit = (FltCreditLimit*FltShadowperc);
					else
						ShadowLimit = (FltShadowCapLim);

					f_printf(stderr, "SHADOW [%f], CAP [%f]\n",
						(FltCreditLimit*FltShadowperc),
						FltShadowCapLim);

					f_printf(stderr, "FINAL SHADOW LIMIT: [%f]\n", ShadowLimit);

					if(bflzbcom.lbcom_acc_temp_limit[0] == '-')
					{
					#ifdef MRL_483
						CalculatedAmount = (FltCreditLimit + ShadowLimit) - FltExcessLimit - FltCurrentBalance - FltCumulAuth;
					#else
						CalculatedAmount = (FltCreditLimit + ShadowLimit) - FltExcessLimit - FltCurrentBalance - FltCumulAuth + FltTotalUtlDebt;
					#endif
						f_printf(stderr, "3. Calculated Amount = [%.f]\n",CalculatedAmount);
					}
					else
					{
					#ifdef MRL_483	
						CalculatedAmount = (FltCreditLimit + ShadowLimit) + FltExcessLimit - FltCurrentBalance - FltCumulAuth;
					#else
						CalculatedAmount = (FltCreditLimit + ShadowLimit) + FltExcessLimit - FltCurrentBalance - FltCumulAuth + FltTotalUtlDebt;
					#endif
						f_printf(stderr, "4. Calculated Amount = [%.f]\n",CalculatedAmount);
					}
					#ifdef MRL_461
					if ( CalculatedAmount >= loc_NetAmnt)
					{
						f_printf(stderr, "SUFFICIENT BALANCE AFTER SHADOW LIMI APPLIED.\n");
						f_printf(stderr, "[%f] > [%f]\n", CalculatedAmount, loc_NetAmnt);
						return ("000000");
					}
					#endif
					
				}
			}
		}
		f_printf(stderr, "Requested Amount Exceeded.NET AMOUNT [%f] > [%f] TRANSACTION AMT[%f]\n",
			loc_NetAmnt, CalculatedAmount,loc_FltAccountAmount );
		return("510051");
	}
	f_printf(stderr, "[%f] > [%f]\n", CalculatedAmount, loc_NetAmnt);
	return ( "000000" );
}

int BFL_Update0100Account ( char * Response )
{
	char		CumulAuth[13], strtemp[15];
	char            cbs_bal[25], strtemp1[15];      
	char		AdvEmiAmount[13];
        char            Update_Pref_Limit[13];
        char            customer_i[20];
	char*		TmpBal = NULL;
	int		Result = -1;
	int             igiftcard_flag = 0; 
	int		iTxnType;
	double		FltCumulAuth = 0.0;
	double          dfee_amt = 0.0f;
	double	        FltAccountAmount = 0.0;
        double          AdvEmiAccountAmount = 0.0;


         	FPRINTF ( stderr, "\nUPDATING LN-BCOM in BFL_Update0100Account\n" );

                if((memcmp (paczisoauto.processing_code,"30",2) == 0 ) || (memcmp (paczisoauto.processing_code,"99",2) == 0 )
                   ||(memcmp (paczisoauto.processing_code,"21",2) == 0 ))
                return ( 0);
                
                memset(CumulAuth,0x00,sizeof(CumulAuth)); 
                memset(AdvEmiAmount,0x00,sizeof(AdvEmiAmount)); 
                memset(LOCAL_AMOUNT,0x00,sizeof(LOCAL_AMOUNT)); 
                memcpy(LOCAL_AMOUNT,paczisoauto.amount_transaction,12);
                memset(Update_Pref_Limit,0x00,sizeof(Update_Pref_Limit)); 
                memset(customer_i,0x00,sizeof(customer_i)); 
                if((memcmp(procdata.adv_emiamount,"000000000000",12) != 0 ) && (memcmp(procdata.adv_emiamount,"            ",12) != 0))
                memcpy(AdvEmiAmount,procdata.adv_emiamount,12);
                else
                memcpy(AdvEmiAmount,"000000000000",12);

                FPRINTF(stderr, "\n LOCAL_AMOUNT [%.12s], FltAccountAmount [%f]", LOCAL_AMOUNT, FltAccountAmount);
  
	        FltAccountAmount = atof(LOCAL_AMOUNT); 
                memset(LOCAL_AMOUNT,0x00,sizeof(LOCAL_AMOUNT));
                AdvEmiAccountAmount = atof(AdvEmiAmount);
	
                iTxnType = iFindTxnType( paczisoauto.processing_code );
                f_printf(stderr,"RESPONSE.........[%s]\n",Response);

	if ( ( memcmp ( Response, "00", 2 ) == 0 ) ||
		 ( memcmp ( Response, "08", 2 ) == 0 ) )
	{
		memcpy ( CumulAuth, bflzbcom.lbcom_acc_tot_auth,12);
                CumulAuth[12] = 0 ;
		
		FPRINTF( stderr,"BEFUPD :Account [%.19s] CumulAuth[%.12s]AdvEmiAmount[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth,AdvEmiAmount);
		FltCumulAuth = atof ( CumulAuth );

		FPRINTF ( stderr, "BFL BEFORE UPDATE CURRENT AUTHORISATION\n" );
		FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );

		if( iTxnType == DEBIT )
			FltCumulAuth += (FltAccountAmount - AdvEmiAccountAmount);
		/*	FltCumulAuth += FltAccountAmount ;  */
   
		FPRINTF ( stderr, "BFL AFTER UPDATE CURRENT AUTHORISATION\n" );
		FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );
		sprintf ( CumulAuth, "%012.0f", FltCumulAuth );
		sprintf ( Original_Amount, "%012.0f", (FltAccountAmount - AdvEmiAccountAmount) );
		FPRINTF( stderr,"\n UPD :Account [%.19s] CumulAuth[%.12s]AdvEmiAmount[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth,AdvEmiAmount);

		memcpy ( bflzbcom.lbcom_acc_tot_auth, CumulAuth, 12 );
		memcpy ( bflzbcom.lbcom_last_auth_date, GetDateYYMMDD (), 6 );
		memcpy ( bflzbcom.lbcom_last_auth_time, GetTime (), 6);
		memcpy ( bflzbcom.lbcom_last_mod_date, GetDateYYMMDD (), 6 );
                #ifdef BFL_PHASE2
                memcpy(Update_Pref_Limit,bflzbcom.lbcom_acc_prefferd_limit,12);
                if((atof(Update_Pref_Limit) >  0.0)&&(memcmp(prefered_limit_flag,"Y",1) == 0 ))
                {
                   memcpy  (customer_i,bflzbcom.lbcom_customer_id, 19);
                   customer_i[19] = '\0';
                   FPRINTF ( stderr, "Prefered Limit  = [%f] \n", atof(bflzbcom.lbcom_acc_prefferd_limit) );
                   memcpy  ( Prefered_Limit,bflzbcom.lbcom_acc_prefferd_limit,12);
                   FPRINTF ( stderr, "RESETTING PREFERED LIMIT" );
                   memcpy  ( bflzbcom.lbcom_acc_prefferd_limit,"000000000000",12);
                   memcpy  ( Update_Pref_Limit,"000000000000",12);
                   #ifdef NEVER
                   Update_Prefered_Limit(customer_i,Update_Pref_Limit);
                   #endif

                }
                #endif
 


		Result = isrewrite ( lnbcom , (char*)&bflzbcom);
		isunlock ( lnbcom );

		if ( Result != 0 )
		{
			FPRINTF ( stderr, "ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
			iserrno, bflzbcom.lbcom_customer_id);
			return ( ECHEC );
		}
                UpdateBFLCardFirstUse(); /**Update First Use to LN-PORTE**/
	}
	TmpBal = (char*)BFL_GetBalance ( &bflzbcom);
	return ( SUCCESS );
}

/* START Of Changes By PRAVINN For LTV Phase II 26122018 */
#include <cust_prod_amount.h>
int BFL_Updatelncust ( char * Response )
{
        int noRecordFound = 0;
        t_cust_prod_amount cust_prod_amount ;

	int lnamnt =0;
	char cust_open[100];

	memset(cust_open,0x00,sizeof(cust_open));
	sprintf(cust_open,"%s/LN-CUST",getenv("MultipacDat"));

	lnamnt = isopen(cust_open,ISINOUT+ISMANULOCK);
	if(lnamnt < 0 )
	{
		f_printf(stderr," LN-CUST ERROR File [%s] Open Error [%d]",cust_open,iserrno);
		isclose(lnamnt);
		return(ECHEC);
	}
	

	
	if (  memcmp ( Response, "00", 2 ) == 0 )
	{
		printf("IN LN_CUST FUNCTION \n");
		memset(&cust_prod_amount, ' ', sizeof(cust_prod_amount));
		memcpy(cust_prod_amount.customerid, bflzbcom.lbcom_customer_id, 19);

		if(isread(lnamnt, &cust_prod_amount, ISEQUAL)<0)
		{
			fprintf(stderr,"Error reading LN-CUST : [%d]\n",iserrno);
		}
	
		if (iserrno ==  ENOREC)
		{
			fprintf(stderr,"DATA not found in LN-CUST \n");
			noRecordFound =1;	
		}


		fprintf(stderr,"buisness_prod_code:[%.8s] procdata.asset_risk:[%.2s]cust id [%.19s] \n",buisness_prod_code,procdata.asset_risk,bflzbcom.lbcom_customer_id);

		memcpy(cust_prod_amount.lcust_last_auth_prod, "          ", 10);
		memcpy(cust_prod_amount.lcust_last_auth_prod,buisness_prod_code,strlen(buisness_prod_code));
		memcpy ( cust_prod_amount.last_date, GetDateYYMMDD (), 6 );

		fprintf(stderr,"cust_prod_amount.lcust_last_auth_prod:[%.8s] \n",cust_prod_amount.lcust_last_auth_prod);

		if (noRecordFound == 1 )
		{
			if(iswrite(lnamnt, &cust_prod_amount)<0)
			{
				fprintf(stderr, "Error in writing record in LN-CUST for Customer: [%.12s] iserrno [%d]\n",cust_prod_amount.customerid,iserrno);
			}
		}
		else if( isrewrite(lnamnt, (char *)&cust_prod_amount ) < 0 )
		{
			fprintf(stderr, "Error in rewriting in LN-CUST for customer: [%.12s] issernno[%d]\n ",cust_prod_amount.customerid,iserrno);
		}
	isunlock( lnamnt );
	isrelease( lnamnt );
	}

	isclose(lnamnt);
        return ( SUCCESS );

}
/* END Of Changes By PRAVINN For LTV Phase II 26122018 */


char* BFL_GetBalance ( t_bflzbcom* Account )
{
	char		CreditLimit[13];
	double		FltCreditLimit = 0.0;
	char		Temp_Limits[13];
	double		Temp_Limit = 0.0;
	char		CumulUtil[13];
	double		FltCumulUtil = 0.0;
	char		CumulAuth[13];
	double		FltCumulAuth = 0.0;
	static char	Balance[13];
	double		CalculatedAmt = 0.0;
	char            TxnAmount[13];
	double          FltTxnAmount = 0.0;

        /* BFL_EXCESS_PAMYENT */
        char            TotalUtlDebt[13];
        double          FltTotalUtlDebt = 0.0;

	memcpy ( TxnAmount, paczisoauto.amount_transaction, 12 );
	TxnAmount[12] = 0;
	FltTxnAmount = atof ( TxnAmount );

	memcpy ( CreditLimit, Account->lbcom_acc_credit_limit, 12);
	CreditLimit[12] = 0;
	FltCreditLimit = atof ( CreditLimit );

	memcpy ( Temp_Limits, Account->lbcom_acc_temp_limit+1, 12);
	Temp_Limits[12] = 0;
	Temp_Limit = atof ( Temp_Limits );

	memcpy ( CumulUtil, Account->lbcom_acc_tot_util, 12);
	CumulUtil[12] = 0;
	FltCumulUtil = atof ( CumulUtil );

	memcpy ( CumulAuth, Account->lbcom_acc_tot_auth, 12);
	CumulAuth[12] = 0;
	FltCumulAuth = atof ( CumulAuth );

        /* BFL_EXCESS_PAMYENT */
        memset ( TotalUtlDebt, 0, 13 );
        memcpy ( TotalUtlDebt, bflzbcom.lbcom_acc_total_utl_debt, 12 );
        TotalUtlDebt[12] = 0;
        FltTotalUtlDebt = atof ( TotalUtlDebt );
 
        if(Account->lbcom_acc_temp_limit[0] == '-')
	{
	#ifdef MRL_483
		CalculatedAmt = FltCreditLimit - Temp_Limit - FltCumulUtil - FltCumulAuth;
	#else
		CalculatedAmt = FltCreditLimit - Temp_Limit - FltCumulUtil - FltCumulAuth + FltTotalUtlDebt;
	#endif
		FPRINTF(stderr,"1. Calculated Amount [%.f]\n",CalculatedAmt);
	}
        else
	{
	#ifdef MRL_483
		CalculatedAmt = FltCreditLimit + Temp_Limit - FltCumulUtil - FltCumulAuth;
	#else
		CalculatedAmt = FltCreditLimit + Temp_Limit - FltCumulUtil - FltCumulAuth + FltTotalUtlDebt; 
	#endif
		FPRINTF(stderr,"2. Calculated Amount [%.f]\n",CalculatedAmt);
	}
	sprintf ( Balance, "%012.0f", CalculatedAmt );
	FPRINTF(stderr, "===========DISPLAY BALENCE=================");
	FPRINTF ( stderr,"\n BCOM AVAILE %012.f", CalculatedAmt );
	FPRINTF(stderr, "\n" ); 
	return ( Balance );
}

char* BFL_GetOpenECSBalance ( t_bflzbcom* Account )
{
        char            CreditLimit[13];
        double          FltCreditLimit = 0.0;
        char            CumulUtil[13];
        double          FltCumulUtil = 0.0;
        char            CumulAuth[13];
        double          FltCumulAuth = 0.0;
        static char     Balance[13];
        double          CalculatedAmt = 0.0;


        memcpy ( CreditLimit, Account->lbcom_ecs_avaialable_limit, 12);
        CreditLimit[12] = 0;
        FltCreditLimit = atof ( CreditLimit );

        memcpy ( CumulUtil, Account->lbcom_ecs_settled_amount, 12);
        CumulUtil[12] = 0;
        FltCumulUtil = atof ( CumulUtil );

        memcpy ( CumulAuth, Account->lbcom_ecs_auth_amount, 12);
        CumulAuth[12] = 0;
        FltCumulAuth = atof ( CumulAuth );

        CalculatedAmt = FltCreditLimit - FltCumulUtil - FltCumulAuth ;

        sprintf ( Balance, "%012.0f", CalculatedAmt );
        FPRINTF(stderr, "**************DISPLAY OPEN ECS BALENCE*****************");
        FPRINTF ( stderr,"\n OPEN ECS AVAILABLE %012.f", CalculatedAmt );
        FPRINTF(stderr, "\n" );

        return ( Balance );
}

int BFL_CheckOpenECSAvailable (char *subresult)
{
        char            Open_Ecs_Balance[13];
        double          FltOpen_Ecs_Balance = 0.0;
        char            New_Request_Emi[13];
        double          FltNew_Request_Emi = 0.0;

        memcpy ( Open_Ecs_Balance , (char*)BFL_GetOpenECSBalance( &bflzbcom), 12 );
        Open_Ecs_Balance[12] = 0;
        if((memcmp(procdata.emiamount,"000000000000",12) != 0 ) && (memcmp(procdata.emiamount,"            ",12) != 0 ))
        {
          memcpy ( New_Request_Emi, procdata.emiamount ,12 );
          New_Request_Emi[12] = 0;
          FltNew_Request_Emi = atof ( New_Request_Emi );
        }
        else
        FltNew_Request_Emi = 0.0;

        FltOpen_Ecs_Balance = atof ( Open_Ecs_Balance );
        FPRINTF ( stderr, "\nRequested Open Ecs EMI [%f] Available Open ECS Balance [%f] \n",   FltNew_Request_Emi ,FltOpen_Ecs_Balance);

        if( FltOpen_Ecs_Balance < FltNew_Request_Emi )
        {
           FPRINTF ( stderr, "\nRequested Open Ecs EMI [%f]  Exceeded Available Open ECS Balance [%f] \n",   FltNew_Request_Emi ,FltOpen_Ecs_Balance);
           memcpy(subresult,"J44351",6);
           return ( ECHEC );
        }
        memcpy(subresult,"000000",6);
        return ( SUCCESS );
}

int BFL_UpdateOpenEcsEmi ( char * Response )
{
        char            CumulAuth[13];
        char            TempAmount[13];
        char            Oneview_RedeemBalance[11];
        int             Result = -1;
        double          FltCumulAuth = 0.0;
        double          FltAccountAmount = 0.0;

                FPRINTF ( stderr, "\nUPDATING ECS AMOUNT LN-BCOM in BFL_UpdateOpenEcsEmi\n" );
                memcpy(TempAmount,procdata.emiamount,12);
                #ifdef BFL_PHASE2 
                if(Ecs_Waive_Off == 1)
                {
                        FPRINTF ( stderr, "\nECS WAIVE-OFF UPDATING WITH ZERO AMOUNT\n" );
                	memcpy(TempAmount,"000000000000",12);
                }
                #endif
                FltAccountAmount = atof(TempAmount); 
                FPRINTF(stderr, "\n TempAmount [%.12s], FltAccountAmount [%f]", TempAmount , FltAccountAmount);
                if(memcmp( paczisoauto.processing_code, "00", 2 ) != 0)
                return ( SUCCESS );
		FPRINTF(stderr, "\nprocessing code=%s response=%s rsa_flag=%s\n",paczisoauto.processing_code,Response,rsa_flag);
        if ( ( memcmp ( Response, "00", 2 ) == 0 ) ||
                 ( memcmp ( Response, "08", 2 ) == 0 ) )
        {
                memcpy ( CumulAuth, bflzbcom.lbcom_ecs_auth_amount,12);
                CumulAuth[12] = 0 ;

                FPRINTF( stderr,"2. BEFUPD :Account [%.19s] CumulAuth[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth);
                FltCumulAuth = atof ( CumulAuth );

                FPRINTF ( stderr, "--------------------ECS AUTHORISATION BEFORE UPDATE --------------------------\n" );
                FPRINTF ( stderr, "ECS Current Authorisation = [%f] \n", FltCumulAuth );

                        FltCumulAuth += FltAccountAmount ;

                FPRINTF ( stderr, "--------------------ECS AUTHORISATION AFTER UPDATE ----------------------------\n" );
                FPRINTF ( stderr, "ECS After Authorisation = [%f] \n", FltCumulAuth );
                sprintf ( CumulAuth, "%012.0f", FltCumulAuth );
                FPRINTF( stderr,"\n 2. UPD :Account [%.19s] CumulAuth[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth);
                memcpy ( bflzbcom.lbcom_ecs_auth_amount, CumulAuth, 12 );

                Result = isrewrite ( lnbcom , (char*)&bflzbcom);
                isunlock ( lnbcom );

                if ( Result != 0 )
                {
                        FPRINTF ( stderr, "OpenEcs ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
                        iserrno, bflzbcom.lbcom_customer_id);
                        return ( ECHEC );
                }
                memcpy ( Oneview_RedeemBalance, (char*)BFL_GetRedeemBalance( &bflzbcom), 10 );

		/* MRL_288  Rishabh Changes*/
		if( UpdateRsaBcom() == ECHEC )
		{
			FPRINTF(stderr,"Updation Of RSA Flag and Count Failed");
		}

		/* MRL_288 */
                BFL_UpdateAccountAmount(bflzbcom.lbcom_acc_tot_auth,bflzbcom.lbcom_ecs_auth_amount,
		Oneview_RedeemBalance,bflzbcom.lbcom_cibil_score,bflzbcom.lbcom_cibil_score_date,
		bflzbcom.lbcom_acc_prefferd_limit, bflzbcom.lbcom_fraud_block_type,
		bflzbcom.lbcom_rsa_flag_count,'C');

	}
        else if (( memcmp ( Response, "D3", 2 ) == 0 )&& (Cibil_Done == 1))
        {
                memcpy ( Oneview_RedeemBalance, (char*)BFL_GetRedeemBalance( &bflzbcom), 10 );

		/* MRL_288  Rishabh Changes*/
		if( UpdateRsaBcom() == ECHEC )
		{
			FPRINTF(stderr,"Updation Of RSA Flag and Count Failed");
		}

		/* MRL_288 */
                BFL_UpdateAccountAmount(bflzbcom.lbcom_acc_tot_auth,bflzbcom.lbcom_ecs_auth_amount,
		Oneview_RedeemBalance,bflzbcom.lbcom_cibil_score,bflzbcom.lbcom_cibil_score_date,
		bflzbcom.lbcom_acc_prefferd_limit, bflzbcom.lbcom_fraud_block_type,
		bflzbcom.lbcom_rsa_flag_count,'C');
        }

        return ( SUCCESS );
}

int BFL_Update0400OpenEcsEmi( )
{
        char            CumulAuth[13];
        char            TempAmount[13];
        int             Result = -1;
        double          FltCumulAuth = 0.0;
        double          FltAccountAmount = 0.0;
        char            Oneview_RedeemBalance[11];

                FPRINTF ( stderr, "\nUPDATING LN-BCOM OPEN ECS in BFL_Update0400OpenEcsEmi\n" );
                memcpy(TempAmount,Bfl_AdvEcsAmount,12); 
                #ifdef BFL_PHASE2
                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.ecs_bypass,"Y",1) == 0 ))
                {
                        FPRINTF ( stderr, "\nORIGINAL HISTORY AUTH  IS ECS WAIVE-OFF UPDATING WITH ZERO AMOUNT\n" );
                	memcpy(TempAmount,"000000000000",12);
                }
                if(( CODE_SAUTO_HAUTO == 'S' ) && (memcmp(bflauth00.ecs_bypass,"Y",1) == 0 ))
                {
                        FPRINTF ( stderr, "\nORIGINAL AUTH IS ECS WAIVE-OFF UPDATING WITH ZERO AMOUNT\n" );
                	memcpy(TempAmount,"000000000000",12);
                }
                #endif
                FltAccountAmount = atof(TempAmount);
                FPRINTF(stderr, "\n 3. TempAmount [%.12s], FltAccountAmount [%f]", TempAmount , FltAccountAmount);

                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.auth_match_flag,"1",1) == 0 ))
                memcpy ( CumulAuth, bflzbcom.lbcom_ecs_settled_amount,12);
                else if(( CODE_SAUTO_HAUTO == 'S' ) && (memcmp(bflauth00.auth_match_flag,"1",1) == 0 ))
                memcpy ( CumulAuth, bflzbcom.lbcom_ecs_settled_amount,12);
                else
                memcpy ( CumulAuth, bflzbcom.lbcom_ecs_auth_amount,12);
                CumulAuth[12] = 0 ;

                FPRINTF( stderr,"3. BEFUPD :Account [%.19s] CumulAuth[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth);
                FltCumulAuth = atof ( CumulAuth );

                FPRINTF ( stderr, "BEFORE UPDATE OPEN ECS\n" );
                FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );
                if((bflauth00.auth_source_rev[0]=='R')&&(memcmp(paczisoauto.processing_code,"03",2) == 0))
                        FltCumulAuth += FltAccountAmount ;
                else if((bflauth00.auth_source_rev[0]=='R')&&(memcmp(paczisoauto.processing_code,"23",2) == 0))
                        FltCumulAuth += FltAccountAmount ;
                else
                        FltCumulAuth -= FltAccountAmount ;

                FPRINTF ( stderr, "AFTER UPDATE OPEN ECS\n" );
                FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );

	#ifdef PROD_232
		if(FltCumulAuth<0.0)
		{
			f_printf(stderr, "CUMUL ECS NEGATIVE. INITIALIZING WITH ZERO.\n");
			FltCumulAuth=0.0;
		}
	#endif

                sprintf ( CumulAuth, "%012.0f", FltCumulAuth );
                FPRINTF( stderr,"\n 3. UPD :Account [%.19s] CumulAuth[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth);

                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.auth_match_flag,"1",1) == 0 ))
                memcpy ( bflzbcom.lbcom_ecs_settled_amount, CumulAuth, 12 );
                else if(( CODE_SAUTO_HAUTO == 'S' ) && (memcmp(bflauth00.auth_match_flag,"1",1) == 0 ))
                memcpy ( bflzbcom.lbcom_ecs_settled_amount, CumulAuth, 12 );
                else 
                memcpy ( bflzbcom.lbcom_ecs_auth_amount, CumulAuth, 12 );
                Result = isrewrite ( lnbcom , (char*)&bflzbcom);
                isunlock ( lnbcom );

                if ( Result != 0 )
                {
                        FPRINTF ( stderr, "OpenEcs ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
                        iserrno, bflzbcom.lbcom_customer_id);
                        return ( ECHEC );
                }
                memcpy ( Oneview_RedeemBalance, (char*)BFL_GetRedeemBalance( &bflzbcom),10);

		/* MRL_288 */
                BFL_UpdateAccountAmount(bflzbcom.lbcom_acc_tot_auth,bflzbcom.lbcom_ecs_auth_amount,
		Oneview_RedeemBalance,bflzbcom.lbcom_cibil_score,bflzbcom.lbcom_cibil_score_date,
		bflzbcom.lbcom_acc_prefferd_limit, bflzbcom.lbcom_fraud_block_type,
		bflzbcom.lbcom_rsa_flag_count,'C');

        return ( SUCCESS );
}

char* BFL_GetRedeemBalance( t_bflzbcom* Account )
{
        char            bfl_earned_redeem_points[11];
        char            mfg_earned_redeem_points[11];
        char            dlr_earned_redeem_points[11];
        char            bfl_earned_redeem_expired[11];
        char            bfl_adjust_redeem_points[11];
        char            CumulUtil[11];
        static char     Balance_Points[16];
        int             FltrBflRedPoints = 0;
        int             FltrmfgRedPoints = 0;
        int             FltrdlrRedPoints = 0;
        int             FltrExpRedPoints = 0;
        int             FltrAdjRedPoints = 0;
        int             FltCumulUtil = 0;
        int             Calculatedpoints = 0;


        memcpy ( bfl_earned_redeem_points, Account->lbcom_reward_bfl_earned, 10);
        bfl_earned_redeem_points[10] = 0;
        FltrBflRedPoints = atoi ( bfl_earned_redeem_points );

        memcpy ( mfg_earned_redeem_points , Account->lbcom_reward_mfg_earned, 10);
        mfg_earned_redeem_points[10] = 0;
        FltrmfgRedPoints = atoi ( mfg_earned_redeem_points );

        memcpy ( dlr_earned_redeem_points, Account->lbcom_reward_dlr_earned, 10);
        dlr_earned_redeem_points[10] = 0;
        FltrdlrRedPoints = atoi ( dlr_earned_redeem_points );

        memcpy ( CumulUtil, Account->lbcom_reward_redeem, 10);
        CumulUtil[10] = 0;
        FltCumulUtil = atoi ( CumulUtil );

        memcpy ( bfl_earned_redeem_expired , Account->lbcom_reward_expired, 10);
        bfl_earned_redeem_expired[10] = 0;
        FltrExpRedPoints = atoi (bfl_earned_redeem_expired);

        memcpy ( bfl_adjust_redeem_points , Account->lbcom_reward_adjustment+1, 10);
        bfl_adjust_redeem_points[10] = 0;
        FltrAdjRedPoints = atoi (bfl_adjust_redeem_points);

        if(memcmp(Account->lbcom_reward_adjustment+0,"-",1) == 0)
        Calculatedpoints = FltrBflRedPoints +  FltrmfgRedPoints + FltrdlrRedPoints - FltrAdjRedPoints - FltCumulUtil - FltrExpRedPoints;
        else
        Calculatedpoints = FltrBflRedPoints +  FltrmfgRedPoints + FltrdlrRedPoints + FltrAdjRedPoints - FltCumulUtil - FltrExpRedPoints;

        if(Calculatedpoints < 0 )
        Calculatedpoints = 0 ;

        sprintf ( Balance_Points , "%010d", Calculatedpoints );
        FPRINTF(stderr, "\n<<<<<<<<<<<DISPLAY REDEEM BALENCE>>>>>>>>>>>>>>>>>>");
        FPRINTF ( stderr,"\n REDEEM POINTS AVAILABLE %010d", Calculatedpoints );
        FPRINTF(stderr, "\n" );

        memcpy ( athtrc.calculated_available, Balance_Points , 10 );
        return ( Balance_Points );
}

int BFL_Update0400Account ( t_entete_iso* IsoHeader )
{
        char            CumulAuth[13];
        char            AdvEmiAmount[13];
        char            Update_Pref_Limit[13];
        char            customer_i[20];
        char*           TmpBal = NULL;
        int             Result = -1;
        int             iTxnType;
        double          FltCumulAuth = 0.0;
        double          FltAccountAmount = 0.0;
        double          AdvEmiAccountAmount = 0.0;


                FPRINTF ( stderr, "\nUPDATING LN-BCOM in BFL_Update0400Account \n" );

                memset(Limit_Before_Void,0x00,sizeof(Limit_Before_Void));
                memset(Limit_After_Void,0x00,sizeof(Limit_After_Void));

                if((memcmp(paczisoauto.processing_code,"02",2) == 0) || (memcmp(paczisoauto.processing_code,"20",2) == 0) )
                {
                	TmpBal = (char*)BFL_GetBalance ( &bflzbcom);
                	memcpy   (Limit_Before_Void+0,TmpBal,12);
                	TmpBal = NULL;
                }
                memset(CumulAuth,0x00,sizeof(CumulAuth));
                memset(AdvEmiAmount,0x00,sizeof(AdvEmiAmount));
                memset(LOCAL_AMOUNT,0x00,sizeof(LOCAL_AMOUNT));
                memset(Update_Pref_Limit,0x00,sizeof(Update_Pref_Limit));
                memset(customer_i,0x00,sizeof(customer_i));
                if((memcmp(paczisoauto.processing_code,"20",2) == 0) || (memcmp(paczisoauto.processing_code,"02",2) == 0))
                {
                     if(strlen(Bfl_LocalAmount) >  0 )
                     memcpy(LOCAL_AMOUNT,Bfl_LocalAmount,12);
                }
                else
                memcpy(LOCAL_AMOUNT,paczisoauto.amount_transaction,12);
                memcpy(AdvEmiAmount,Bfl_AdvEmiAmount,12);
                AdvEmiAccountAmount = atof(AdvEmiAmount);
                FPRINTF(stderr, "\n LOCAL_AMOUNT [%.12s], FltAccountAmount [%f]AdvEmiAccountAmount[%f]", LOCAL_AMOUNT, FltAccountAmount,AdvEmiAccountAmount);

                FltAccountAmount = atof(LOCAL_AMOUNT); 
                memset(LOCAL_AMOUNT,0x00,sizeof(LOCAL_AMOUNT));
                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.auth_match_flag,"1",1) == 0 ))
                memcpy ( CumulAuth, bflzbcom.lbcom_acc_tot_util,12);
                else if(( CODE_SAUTO_HAUTO == 'S' ) && (memcmp(bflauth00.auth_match_flag,"1",1) == 0 ))
                memcpy ( CumulAuth, bflzbcom.lbcom_acc_tot_util,12);
                else
                memcpy ( CumulAuth, bflzbcom.lbcom_acc_tot_auth,12);
                CumulAuth[12] = 0 ;

                FPRINTF( stderr,"BEFUPD :Account [%.19s] CumulAuth[%.12s]AdvEmiAmount[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth,AdvEmiAmount);
                FltCumulAuth = atof ( CumulAuth );

                FPRINTF ( stderr, "BEFORE UPDATE \n" );
                FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );
    		iTxnType = iFindTxnType( paczisoauto.processing_code );

                if((bflauth00.auth_source_rev[0]=='R')&&(memcmp(paczisoauto.processing_code,"03",2) == 0))
                        FltCumulAuth += (FltAccountAmount - AdvEmiAccountAmount);
                else if((bflauth00.auth_source_rev[0]=='R')&&(memcmp(paczisoauto.processing_code,"23",2) == 0))
                        FltCumulAuth += (FltAccountAmount - AdvEmiAccountAmount);
                else
                {
                 	if( iTxnType == DEBIT )
                        	FltCumulAuth -= (FltAccountAmount - AdvEmiAccountAmount);
                	else                                                              /*************Added to Support BFL  Refund Transaction*************/
                        	FltCumulAuth -= (FltAccountAmount - AdvEmiAccountAmount);
                }

                FPRINTF ( stderr, "AFTER UPDATE \n" );
                FPRINTF ( stderr, "Cumul Auth = [%f] \n", FltCumulAuth );
                

	#ifdef PROD_232
		if(FltCumulAuth<0.0)
		{
			f_printf(stderr, "CUMUL AUTH NEGATIVE. INITIALIZING WITH ZERO.\n");
			FltCumulAuth=0.0;
		}
	#endif

                sprintf ( CumulAuth, "%012.0f", FltCumulAuth );
		FPRINTF ( stderr, "Cumul Auth = [%.12s] \n", CumulAuth );
		FPRINTF( stderr,"\n UPD :Account [%.19s] CumulAuth[%.12s]AdvEmiAmount[%.12s]\n",bflzbcom.lbcom_customer_id,CumulAuth,AdvEmiAmount);

                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.auth_match_flag,"1",1) == 0 ))
                memcpy (bflzbcom.lbcom_acc_tot_util,CumulAuth,12);
                else if(( CODE_SAUTO_HAUTO == 'S' ) && (memcmp(bflauth00.auth_match_flag,"1",1) == 0 ))
                memcpy (bflzbcom.lbcom_acc_tot_util,CumulAuth,12);
                else
                memcpy ( bflzbcom.lbcom_acc_tot_auth, CumulAuth, 12 );
              
                memcpy ( bflzbcom.lbcom_last_auth_date, GetDateYYMMDD (), 6 );
                memcpy ( bflzbcom.lbcom_last_auth_time, GetTime (), 6);
                memcpy ( bflzbcom.lbcom_last_mod_date, GetDateYYMMDD (), 6 );

                #ifdef BFL_PHASE2 
                if(( CODE_SAUTO_HAUTO == 'H' ) && (memcmp(bflhistauth.prefered_limit_flag,"Y",1) == 0 )&&(memcmp(bflzbcom.lbcom_acc_prefferd_limit,"000000000000",12) == 0 ))
                {
                        
			memcpy  (customer_i,bflzbcom.lbcom_customer_id, 19);
                   	customer_i[19] = '\0';
                        memcpy(Update_Pref_Limit,bflzbcom.lbcom_acc_prefferd_limit,12);
                }
                else if (( CODE_SAUTO_HAUTO == 'S' )&&(memcmp(bflauth00.prefered_limit_flag,"Y",1) == 0 )&&(memcmp(bflzbcom.lbcom_acc_prefferd_limit,"000000000000",12) == 0 ))
                {
                        memcpy  (customer_i,bflzbcom.lbcom_customer_id, 19);
                        customer_i[19] = '\0';
                        memcpy(Update_Pref_Limit,bflzbcom.lbcom_acc_prefferd_limit,12);
                }
                #endif


                Result = isrewrite ( lnbcom , (char*)&bflzbcom);
                isunlock ( lnbcom );

                if ( Result != 0 )
                {
                        FPRINTF ( stderr, "ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
                        iserrno, bflzbcom.lbcom_customer_id);
                        return ( ECHEC );
                }
        TmpBal = (char*)BFL_GetBalance ( &bflzbcom);
        if((memcmp(paczisoauto.processing_code,"02",2) == 0) || (memcmp(paczisoauto.processing_code,"20",2) == 0) )
        memcpy   (Limit_After_Void+0,TmpBal,12);
        return ( SUCCESS );
}

int BFL_CheckRedeemPointAvailable (char *subresult)
{
        char            Avl_Redeem_Points[11];
        int             Avl_Redeem_Pts = 0;
        char            Req_Redeem_Points[7];
        int             FltRedeemPoints = 0;

        memcpy ( RedeemPointBalance, (char*)BFL_GetRedeemBalance( &bflzbcom), 10 );
        if(strlen(Requested_reward_point) > 0)
        {
          memcpy ( Req_Redeem_Points , Requested_reward_point ,6 );
          Req_Redeem_Points[6] = 0;
          FltRedeemPoints = atoi ( Req_Redeem_Points );
        }
        else
        FltRedeemPoints = 0;

        memcpy ( Avl_Redeem_Points, RedeemPointBalance, 10);
        Avl_Redeem_Points[10] = 0;
        Avl_Redeem_Pts = atoi ( Avl_Redeem_Points );
        FPRINTF ( stderr, "Requested Redeem Points [%d] Available Redeem Points [%d] \n",   FltRedeemPoints ,Avl_Redeem_Pts);

        if( Avl_Redeem_Pts < FltRedeemPoints )
        {
           FPRINTF ( stderr, "Requested Redeem Points [%d]  Exceeded the Available Redeem Points [%d] \n",   FltRedeemPoints ,Avl_Redeem_Pts);
           memcpy(subresult,"513351",6);
           return ( ECHEC );
        }
        memcpy(subresult,"000000",6);
        return ( SUCCESS );
}

int Update0100RedeemPoints ( char * Response )
{
        char            Redeem_CumulAuth[11];
        char            Redeem_Points[7];
        char            Oneview_RedeemBalance[11];
        int             Result = -1;
        int             FltCumulAuth = 0;
        int             FltRedeemPoint = 0;

                FPRINTF ( stderr, "\nREDEEM POINTS UPDATION TO LN-BCOM in Update0100RedeemPoints \n" );

                memset(Redeem_CumulAuth,0x00,sizeof(Redeem_CumulAuth));
                memset(Redeem_Points,0x00,sizeof(Redeem_Points));
                if(strlen(Requested_reward_point) > 0)
                {
                        memcpy(Redeem_Points,Requested_reward_point,6); /********************************/
                        FPRINTF(stderr, "\n REQUESTED REDEEM POINTS [%.6s]", Redeem_Points);
                        FltRedeemPoint = atoi(Redeem_Points);
                }
                else
                FltRedeemPoint = 0;

        f_printf(stderr,"RESPONSE.........[%s]\n",Response);

        if ( memcmp ( Response, "00", 2 ) == 0 )
        {
                memcpy ( Redeem_CumulAuth , bflzbcom.lbcom_reward_redeem,10);
                Redeem_CumulAuth[10] = 0 ;

                FPRINTF( stderr,  "Account [%.19s] Redeem_CumulAuth [%.10s]\n",bflzbcom.lbcom_customer_id,Redeem_CumulAuth);
                FltCumulAuth = atoi ( Redeem_CumulAuth );
                FPRINTF ( stderr, "BEFORE  REDEEM POINTS UPDATE \n" );
                FPRINTF ( stderr, "Redeem_CumulAuth  = [%d] \n", FltCumulAuth );

                FltCumulAuth += FltRedeemPoint ;

                FPRINTF ( stderr, "AFTER REDEEM POINTS UPDATE \n" );
                FPRINTF ( stderr, "Redeem_CumulAuth  = [%d] \n", FltCumulAuth );
                sprintf ( Redeem_CumulAuth , "%010d", FltCumulAuth );
                FPRINTF( stderr,"\n UPD :Account [%.19s] Redeem_CumulAuth [%.10s]\n",bflzbcom.lbcom_customer_id,Redeem_CumulAuth);

                memcpy ( bflzbcom.lbcom_reward_redeem, Redeem_CumulAuth , 10 );
                memcpy ( bflzbcom.lbcom_last_auth_date, GetDateYYMMDD (), 6 );
                memcpy ( bflzbcom.lbcom_last_auth_time, GetTime (), 6);
                memcpy ( bflzbcom.lbcom_last_mod_date, GetDateYYMMDD (), 6 );

                Result = isrewrite ( lnbcom , (char*)&bflzbcom);
                isunlock ( lnbcom );
        
                if ( Result != 0 )
                {
                        FPRINTF ( stderr, "ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
                        iserrno, bflzbcom.lbcom_customer_id);
                        return ( ECHEC );
                }
                memcpy ( Oneview_RedeemBalance, (char*)BFL_GetRedeemBalance( &bflzbcom),10);
		/* MRL_288 */
                BFL_UpdateAccountAmount(bflzbcom.lbcom_acc_tot_auth,bflzbcom.lbcom_ecs_auth_amount,
		Oneview_RedeemBalance,bflzbcom.lbcom_cibil_score,bflzbcom.lbcom_cibil_score_date,
		bflzbcom.lbcom_acc_prefferd_limit, bflzbcom.lbcom_fraud_block_type,
		bflzbcom.lbcom_rsa_flag_count,'C');
         }
        return ( SUCCESS );
}


int Update0400RedeemPoints ( )
{
        char            Redeem_CumulAuth[11];
        char            Redeem_Points[7];
        int             Result = -1;
        int             FltCumulAuth = 0;
        int             FltRedeemPoint = 0;
        char            Oneview_RedeemBalance[11];

                FPRINTF ( stderr, "\nREDEEM POINTS UPDATION TO LN-BCOM in Update0400RedeemPoints \n" );

                memset(Redeem_CumulAuth,0x00,sizeof(Redeem_CumulAuth));
                memset(Redeem_Points,0x00,sizeof(Redeem_Points));
                if(strlen(Requested_reward_point) > 0)
                {
                        memcpy(Redeem_Points,Requested_reward_point,6); /********************************/
                        FPRINTF(stderr, "\n REQUESTED REDEEM POINTS [%.6s]", Redeem_Points);

                        FltRedeemPoint = atoi(Redeem_Points);

                }
                else
                FltRedeemPoint = 0;

                memcpy ( Redeem_CumulAuth , bflzbcom.lbcom_reward_redeem,10);
                Redeem_CumulAuth[10] = 0 ;

                FPRINTF( stderr,  "Account [%.19s] Redeem_CumulAuth [%.10s]\n",bflzbcom.lbcom_customer_id,Redeem_CumulAuth);
                FltCumulAuth = atoi ( Redeem_CumulAuth );
                FPRINTF ( stderr, "REVERSAL -BEFORE  REDEEM UPDATE \n" );
                FPRINTF ( stderr, "Redeem_CumulAuth  = [%d] \n", FltCumulAuth );

                if((bflauth00.auth_source_rev[0]=='R')&&(memcmp(paczisoauto.processing_code,"39",2) == 0))
                FltCumulAuth += FltRedeemPoint ;
                else
                FltCumulAuth -= FltRedeemPoint ;

                FPRINTF ( stderr, "REVERSAL -AFTER REDEEM UPDATE \n" );
                FPRINTF ( stderr, "Redeem_CumulAuth  = [%d] \n", FltCumulAuth );
                sprintf ( Redeem_CumulAuth , "%010d", FltCumulAuth );
                FPRINTF( stderr,"\n UPD :Account [%.19s] Redeem_CumulAuth [%.10s]\n",bflzbcom.lbcom_customer_id,Redeem_CumulAuth);

                memcpy ( bflzbcom.lbcom_reward_redeem, Redeem_CumulAuth , 10 );
                memcpy ( bflzbcom.lbcom_last_auth_date, GetDateYYMMDD (), 6 );
                memcpy ( bflzbcom.lbcom_last_auth_time, GetTime (), 6);
                memcpy ( bflzbcom.lbcom_last_mod_date, GetDateYYMMDD (), 6 );

                Result = isrewrite ( lnbcom , (char*)&bflzbcom);
                isunlock ( lnbcom );
                
                if ( Result != 0 )
                {
                        FPRINTF ( stderr, "ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
                        iserrno, bflzbcom.lbcom_customer_id);
                        return ( ECHEC );
                }
                memcpy ( Oneview_RedeemBalance, (char*)BFL_GetRedeemBalance( &bflzbcom),10);
		/* MRL_288 */
                BFL_UpdateAccountAmount(bflzbcom.lbcom_acc_tot_auth,bflzbcom.lbcom_ecs_auth_amount,
		Oneview_RedeemBalance,bflzbcom.lbcom_cibil_score,bflzbcom.lbcom_cibil_score_date,
		bflzbcom.lbcom_acc_prefferd_limit, bflzbcom.lbcom_fraud_block_type,
		bflzbcom.lbcom_rsa_flag_count,'C');

        return ( SUCCESS );
}

/* MRL_300 */
void UpdateBFLCardFirstUse()
{
	int isrewrite_porte=0;
	if(
		((memcmp(buisness_prod_code,"REMI",4)!=0)&&
		(memcmp(buisness_prod_code,"ECF",3)!=0)&&
		(memcmp(buisness_prod_code,"WLT",3)!=0)&&
		(memcmp(buisness_prod_code,"ERF",3)!=0))
		&&
		((memcmp(transaction_source,"EP",2) == 0)||
		(memcmp(transaction_source,"WP",2) == 0) ||
		(memcmp(transaction_source,"MP",2) == 0)||
		(memcmp(transaction_source,"SP",2) == 0)||
		(memcmp(transaction_source,"AP",2) == 0))
		&&
		(memcmp(bflcarddet.lcrd_new_rrn, "            ", 12)==0)
	)
	{
		fprintf(stderr,"First RRN       [%.12s]\n",paczisoauto.retrieval_reference_number);
		memcpy(bflcarddet.lcrd_new_rrn ,paczisoauto.retrieval_reference_number,12);
		chargeslip_first_fee = 3;
		isrewrite_porte=1;
		
	}
	 #ifdef MRL_383
	 if(
		 (memcmp(bflcarddet.lcrd_new_rrn, "            ",12)== 0) &&
		 (memcmp(buisness_prod_code,"REMI",4)== 0) &&
		 (memcmp(bflzport.lport_mxp_card_type,"58",2) == 0 )
	 )
	 {
		 fprintf(stderr,"First RRN       [%.12s] for REMI_EMI_CARD_CHARGES\n",paczisoauto.retrieval_reference_number);
		 memcpy(bflcarddet.lcrd_new_rrn ,paczisoauto.retrieval_reference_number,12);
		 rset_card_1st_fee = 2;
		 isrewrite_porte=1;
	 }
	 #endif

   	if(memcmp(bflcarddet.lcrd_last_auth_dt,"      ",6) == 0) 
   	{
                if(Card_First_Trans == 0)
			Card_First_Trans = 1;

		memcpy(bflcarddet.lcrd_last_auth_dt ,GetDateYYMMDD(),6);
                /*
		if( bflcarddet.lcrd_wrong_pin_attempt[0] > '0')
                 bflcarddet.lcrd_wrong_pin_attempt[0] = '0';
                if( bflcarddet.lcrd_wrong_pin_attempt[0] > '0')
                 bflcarddet.lcrd_wrong_pin_attempt[0] = '0';
        	*/	
		f_printf(stderr,"\n Updating First Usage to LN-CRDDET\n");
		isrewrite_porte=1;
   	}

	if(1 == isrewrite_porte)
	{
		fprintf(stderr,"\"%s\",Function \"%s\"  \n",__FILE__, __FUNCTION__);	
		UpdateLNCrdDet();
	}
}


#ifdef MRL_207
int BFL_UpdateOtp_Count(char *Respons)
{
           int Result = -1;
	   int writeFlag = -1;
	char CurrRRN[13]={0};
	char CurrCardNo[17]={0};
	
	memcpy(CurrRRN,paczisoauto.retrieval_reference_number,12);
	memcpy(CurrCardNo,bflzport.lport_card_no,16);
	
        if ( ( memcmp ( Respons, "L3", 2 ) == 0 ) || ( memcmp ( Respons, "N0", 2 ) == 0 ) )
	{

		if( memcmp(bflcarddet.lcrd_wrong_otp_attempt," ",1) == 0 )
			memcpy(bflcarddet.lcrd_wrong_otp_attempt,"0",1);
		if(( memcmp(bflcarddet.lcrd_wrong_otp_attempt,"9",1) > 0 )||( memcmp(bflcarddet.lcrd_wrong_otp_attempt,"0",1) < 0 ))
			memcpy(bflcarddet.lcrd_wrong_otp_attempt,"0",1);

		if(bflcarddet.lcrd_wrong_otp_attempt[0] < bflzlpa03.lpa03_max_wrng_otp_allowed[0]) 
			bflcarddet.lcrd_wrong_otp_attempt[0] += 1;

		f_printf(stderr,"BFL-Updating the Wrong OTP attempt[%.1s]",bflcarddet.lcrd_wrong_otp_attempt);
		f_printf(stderr,"BFL-Max OTP  attempt Allowed [%.1s]",bflzlpa03.lpa03_max_wrng_otp_allowed);

		if(bflcarddet.lcrd_wrong_otp_attempt[0] >=  bflzlpa03.lpa03_max_wrng_otp_allowed[0])
		{
			/********UPDATE CARD STATUS AS BLOCKED AT CARD MASTER************/
			if(memcmp(bflzport.lport_cs_block_flag,"P",1) == 0)
			{
				f_printf(stderr,"Countg exceed making card block as [B]");
				BflCardBlock_Api(CurrRRN, CurrCardNo,"B");
				
				memcpy(bflzport.lport_cs_block_flag,"B",1);
				memcpy(bflzport.lport_cs_block_reason,"Max otp and pin attempts exceeded                                               ",80);
			}
			else
			{
				/* TODO */
				f_printf(stderr,"Count exceed making card block as [O]");
				BflCardBlock_Api(CurrRRN, CurrCardNo, "O");
				
				memcpy(bflzport.lport_cs_block_flag,"O",1);
				memcpy(bflzport.lport_cs_block_reason,"Max otp attempts exceeded                                                       ",80);
			}

			memcpy(bflzport.lport_cs_block_dt,GetDateYYMMDD(),6);
			memcpy(bflzport.lport_cs_block_time, GetTime (), 6);
			memcpy(bflzport.lport_last_modify_dt,GetDateYYMMDD(),6);
			memcpy(bflzport.lport_user_name, "ONUS    ", 8);
			f_printf(stderr,"Blocking card in the database [%.19s].\n",bflzport.lport_card_no);

			Result = isrewrite ( lnporte , (char*)&bflzport);
			isunlock ( lnporte );
			if ( Result != 0 )
			{
				FPRINTF ( stderr, "BFL ONUS: REWRITE ERROR  %d\n", iserrno );
				return ( ECHEC );
			}
		}

		writeFlag = 0;	
	}

        if(bflzport.lport_wrong_otp_attempt[0] >=  bflzlpa03.lpa03_max_wrng_otp_allowed[0])
        {
                if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) || (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
                return ( SUCCESS );
        }

        if( memcmp ( Respons, "00", 2 ) == 0 )
        {
                if( bflcarddet.lcrd_wrong_otp_attempt[0] > '0' )
                {
                        f_printf(stderr,"BFL OTP  attempt setting to ZERO ");
                        bflcarddet.lcrd_wrong_otp_attempt[0] = '0';
                }
	writeFlag =0;
       }
	fprintf(stderr,"\"%s\",Function \"%s\" RRN \n",__FILE__, __FUNCTION__);
	if(writeFlag == 0)
	UpdateLNCrdDet();

        return ( SUCCESS );

}
#endif

int BFL_UpdatePin_Count(char *Respons)
{
        int             Result = -1;
	int 		writeFlag = -1;
	char CurrRRN[13]={0};
	char CurrCardNo[17]={0};
	
	memcpy(CurrRRN,paczisoauto.retrieval_reference_number,12);
	memcpy(CurrCardNo,bflzport.lport_card_no,16);

 	if ( ( memcmp ( Respons, "55", 2 ) == 0 ) || ( memcmp ( Respons, "75", 2 ) == 0 ) )
	{
		if( memcmp(bflcarddet.lcrd_wrong_pin_attempt," ",1) == 0)
			memcpy(bflcarddet.lcrd_wrong_pin_attempt,"0",1); 
		if((memcmp(bflcarddet.lcrd_wrong_pin_attempt,"9",1) > 0 ) ||(memcmp(bflcarddet.lcrd_wrong_pin_attempt,"0",1) < 0 ))
			memcpy(bflcarddet.lcrd_wrong_pin_attempt,"0",1); 
	
	
	 	if(bflcarddet.lcrd_wrong_pin_attempt[0] < bflzlpa03.lpa03_max_wrng_pin_allowed[0] )
			bflcarddet.lcrd_wrong_pin_attempt[0] += 1;

		f_printf(stderr,"BFL-Updating the Wrong pin attempt[%.1s]",bflzport.lport_wrong_pin_attempt);
		f_printf(stderr,"BFL-Max PIN  attempt Allowed [%.1s]",bflzlpa03.lpa03_max_wrng_pin_allowed);

		if(bflcarddet.lcrd_wrong_pin_attempt[0] >=  bflzlpa03.lpa03_max_wrng_pin_allowed[0])
		{
			/********UPDATE CARD STATUS AS BLOCKED AT CARD MASTER************/
			/* MRL_207 */
			if(memcmp(bflzport.lport_cs_block_flag,"O",1) == 0)
			{
				BflCardBlock_Api(CurrRRN , CurrCardNo,"B");
			
				memcpy(bflzport.lport_cs_block_flag,"B",1);
                                memcpy(bflzport.lport_cs_block_reason,"Max otp and pin attempts exceeded                                               ",80);
			}
			else
			{
				BflCardBlock_Api( CurrRRN ,CurrCardNo,"P");
	
				memcpy(bflzport.lport_cs_block_flag,"P",1);
			        memcpy(bflzport.lport_cs_block_reason,"Max pin attempts exceeded                                                       ",80);
			}
			memcpy(bflzport.lport_cs_block_dt,GetDateYYMMDD(),6);
			memcpy(bflzport.lport_cs_block_time, GetTime (), 6);
			memcpy(bflzport.lport_last_modify_dt,GetDateYYMMDD(),6);
			memcpy(bflzport.lport_user_name, "ONUS    ",8);
			f_printf(stderr,"Blocking card in the database [%.19s].\n",bflzport.lport_card_no);
		


			Result = isrewrite ( lnporte , (char*)&bflzport);
			if ( Result != 0 )
			{
				FPRINTF ( stderr, "BFL ONUS: REWRITE ERROR  %d\n", iserrno );
				return ( ECHEC );
			}
			isunlock ( lnporte );
			isrelease (lnporte);

		}
		writeFlag = 0;
	}

        if(bflcarddet.lcrd_wrong_pin_attempt[0] >=  bflzlpa03.lpa03_max_wrng_pin_allowed[0])
        {
        	if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 	(  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
        	return ( SUCCESS );
        }

 	if( memcmp ( Respons, "00", 2 ) == 0 )               
 	{
     		if( bflcarddet.lcrd_wrong_pin_attempt[0] > '0' )
     		{
                        f_printf(stderr,"BFL PIN  attempt setting to ZERO ");
         		bflcarddet.lcrd_wrong_pin_attempt[0] = '0'; 
		}
 		writeFlag = 0;	
	}
	
	fprintf(stderr,"\"%s\",Function \"%s\" RRN[] \n",__FILE__, __FUNCTION__);

	if(writeFlag == 0)	
	UpdateLNCrdDet();

	return ( SUCCESS );
}
#endif

#include<reloadinfo.h>
t_reloadinfo  reloadinfo;
int UpdateTempFile(char * reload_amt_to_temp)
{

        int fdrel = -1;
        char temp_file_open [80];

        memset(temp_file_open , 0x00, sizeof(temp_file_open));
        sprintf(temp_file_open, "%s/FCR-RELOAD",getenv("MultipacDat"));
        if((fdrel = isopen(temp_file_open,ISINOUT + ISMANULOCK))<0)
        {
                f_printf(stderr,"ONUS: ERROR OPEN FCR-RELOAD:%d\n",iserrno);
                return -1;
        }

        memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
        memcpy (reloadinfo.card_number, paczpoau.poau_num_carte, 19);
        memcpy (reloadinfo.rrn, paczisoauto.retrieval_reference_number, 12);

        memcpy ( reloadinfo.process_flag, "1", 1 );
        memcpy ( reloadinfo.reload_amount, reload_amt_to_temp, 12);
        /*
        need to update the date and time
        */
        if( iswrite( fdrel, (char *)&reloadinfo ) < 0 )
        {
                f_printf(stderr, " Error in writing into the temp file , iserrno %d \n", iserrno);
                isclose(fdrel);
                return 0;
        }
                isclose(fdrel);
        return -1;
}

int iReloadPresentFile(char *reload_card, char * reload_rrn)
{
        int fdrel = -1;
        char temp_file_open [80];
#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */
	struct keydesc  rlkey;
	int reload_result = -1;
#endif

        memset(temp_file_open , 0x00, sizeof(temp_file_open));
        sprintf(temp_file_open, "%s/FCR-RELOAD",getenv("MultipacDat"));
        if((fdrel = isopen(temp_file_open,ISINOUT + ISMANULOCK))<0)
        {
                f_printf(stderr,"ONUS: ERROR OPEN FCR-RELOAD:%d\n",iserrno);
                return -1;
        }
f_printf(stderr, " reload_rrn %s , paczpoau.poau_num_carte [%.19s]\n", reload_rrn, paczpoau.poau_num_carte);
        memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
        memcpy (reloadinfo.card_number, reload_card, 19);
        memcpy (reloadinfo.rrn, reload_rrn, 12);


f_printf(stderr, " REC details [%s]\n", &reloadinfo);
        if(isread (fdrel, (char *)&reloadinfo, ISEQUAL) < 0)
        {
#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */
		f_printf(stderr, " REC details not found,finding with RRN only...\n");
		rlkey.k_flags = ISNODUPS;
		rlkey.k_nparts = 1;
		rlkey.k_part[0].kp_start = 0;
		rlkey.k_part[0].kp_leng = 32;
		rlkey.k_part[0].kp_type = CHARTYPE;

		memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
		memcpy (reloadinfo.card_number, reload_card, 7);
		/*memcpy (reloadinfo.rrn, reload_rrn, 12);*/
		reload_result = isstart( fdrel, &rlkey , 7 , (char *)&reloadinfo , ISEQUAL  );
		if(reload_result == 0)
		{
			if(isread (fdrel, (char *)&reloadinfo, ISGTEQ) == 0)
			{
				while(iserrno != EENDFILE)
				{
					if(memcmp(reloadinfo.rrn,reload_rrn,12) == 0)
					{
						f_printf(stderr, "reload_card [%.19s],RRN [%.12s]\n",reloadinfo.card_number,reloadinfo.rrn);
						memcpy(reload_card,reloadinfo.card_number,19);
						if(memcmp(reloadinfo.process_flag, "1" , 1 ) == 0 )
						{
							isclose(fdrel);
							return 1;
						}
						else if(memcmp(reloadinfo.process_flag,"2" ,1 ) == 0 )
						{
							isclose(fdrel);
							f_printf(stderr, " Already processed \n");
							return 0;
						}
						else if(memcmp(reloadinfo.process_flag,"3" ,1 ) == 0 )
						{
							isclose(fdrel);
							f_printf(stderr, " Second leg Came after Void of Reload. \n");
							return 3;
						}
						else
						{
							isclose(fdrel);
							return -1;
						}
					}
					else
					{
						isread(fdrel, (char *) &reloadinfo, ISNEXT);
					}
				}
			}
			f_printf(stderr, " Read error in RRN based search , error no is %d\n", iserrno);
                        isclose(fdrel);
                        return -1;
		}
		else
#endif
		{
			f_printf(stderr, " Read error , error no is %d\n", iserrno);
			isclose(fdrel);
			return -1;
		}
        }
        else
        {

                if(memcmp(reloadinfo.process_flag, "1" , 1 ) == 0 )
                {
                        isclose(fdrel);
                        return 1;
                }
                else if(memcmp(reloadinfo.process_flag,"2" ,1 ) == 0 )
                {
                #ifdef NEVER
                memcpy(reloadinfo.process_flag,"2" ,1 ) ;
                if( isrewrite(fdrel, (char *)&reloadinfo ) < 0 )
                {
                        f_printf(stderr, " Rewrite failed , error no is %d\n", iserrno);
                        isclose(fdrel);
                        return -1;
                }
                #endif
                isclose(fdrel);
                f_printf(stderr, " Already processed \n");
                return 0;
                }
		#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 3 APR 2017 for SHELL Reload second leg not received but void received */
		else if(memcmp(reloadinfo.process_flag,"3" ,1 ) == 0 )
		{
			isclose(fdrel);
			f_printf(stderr, " Second leg Came after Void of Reload. \n");
			return 3;
		}
		#endif
                isclose(fdrel);
                return -1;

        }
}

int iReloadUpdateFile(char *reload_card, char * reload_rrn)
{
        int fdrel = -1;
        char temp_file_open [80];
#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */
        struct keydesc  rlkey;
        int reload_result = -1;
#endif

        memset(temp_file_open , 0x00, sizeof(temp_file_open));
        sprintf(temp_file_open, "%s/FCR-RELOAD",getenv("MultipacDat"));
        if((fdrel = isopen(temp_file_open,ISINOUT + ISMANULOCK))<0)
        {
                f_printf(stderr,"ONUS: ERROR OPEN FCR-RELOAD:%d\n",iserrno);
                return -1;
        }
f_printf(stderr, " reload_rrn %s , paczpoau.poau_num_carte [%.19s]\n", reload_rrn, paczpoau.poau_num_carte);
        memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
        memcpy (reloadinfo.card_number, reload_card, 19);
        memcpy (reloadinfo.rrn, reload_rrn, 12);


f_printf(stderr, " REC details [%s]\n", &reloadinfo);
        if(isread (fdrel, (char *)&reloadinfo, ISEQUAL) < 0)
        {
#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */
                f_printf(stderr, " REC details not found,finding with RRN only...\n");
                rlkey.k_flags = ISNODUPS;
                rlkey.k_nparts = 1;
                rlkey.k_part[0].kp_start = 0;
                rlkey.k_part[0].kp_leng = 32;
                rlkey.k_part[0].kp_type = CHARTYPE;

                memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
		memcpy (reloadinfo.card_number, reload_card, 7);
                /*memcpy (reloadinfo.rrn, reload_rrn, 12);*/
                reload_result = isstart( fdrel, &rlkey , 7 , (char *)&reloadinfo , ISEQUAL  );
                if(reload_result == 0)
                {
                        if(isread (fdrel, (char *)&reloadinfo, ISGTEQ) == 0)
                        {
                                while(iserrno != EENDFILE)
                                {
					if(memcmp(reloadinfo.rrn,reload_rrn,12) == 0)
					{
						f_printf(stderr, "reload_card [%.19s],RRN [%.12s]\n",reloadinfo.card_number,reloadinfo.rrn);
						memcpy(reload_card,reloadinfo.card_number,19);
						/*if(memcmp(reloadinfo.process_flag, "1" , 1 ) == 0 )
						{
							isclose(fdrel);
							return 1;
						}
						else if(memcmp(reloadinfo.process_flag,"2" ,1 ) == 0 )
						{
							isclose(fdrel);
							f_printf(stderr, " Already processed \n");
							return 0;
						}
						else
						{
							isclose(fdrel);
							return -1;
						}*/
						memcpy(reloadinfo.process_flag,"2" ,1 );
						if( isrewrite(fdrel, (char *)&reloadinfo ) < 0 )
						{
							f_printf(stderr, " Rewrite failed , error no is %d\n", iserrno);
							isclose(fdrel);
							return -1;
						}
						isclose(fdrel);
						return 1;
					}
					else
					{
                                        	isread(fdrel, (char *) &reloadinfo, ISNEXT);
					}
                                }
                        }
			f_printf(stderr, " Read error in RRN based search , error no is %d\n", iserrno);
                        isclose(fdrel);
                        return -1;
                }
                else
#endif
		{
			f_printf(stderr, " Read error , error no is %d\n", iserrno);
			isclose(fdrel);
			return -1;
		}
        }
        else
        {
                        memcpy(reloadinfo.process_flag,"2" ,1 );
                        if( isrewrite(fdrel, (char *)&reloadinfo ) < 0 )
                        {
                                f_printf(stderr, " Rewrite failed , error no is %d\n", iserrno);
                                isclose(fdrel);
                                return -1;
                        }
                        isclose(fdrel);
                        return 1;
        }
}

#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 3 APR 2017 for SHELL Reload second leg not received but void received */
int iReload_Void_UpdateFile(char *reload_card, char * reload_rrn)
{
        int fdrel = -1;
        char temp_file_open [80];
        struct keydesc  rlkey;
        int reload_result = -1;

        memset(temp_file_open , 0x00, sizeof(temp_file_open));
        sprintf(temp_file_open, "%s/FCR-RELOAD",getenv("MultipacDat"));
        if((fdrel = isopen(temp_file_open,ISINOUT + ISMANULOCK))<0)
        {
                f_printf(stderr,"ONUS: ERROR OPEN FCR-RELOAD:%d\n",iserrno);
                return -1;
        }
	f_printf(stderr, " reload_rrn %s , paczpoau.poau_num_carte [%.19s]\n", reload_rrn, paczpoau.poau_num_carte);
        memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
        memcpy (reloadinfo.card_number, reload_card, 19);
        memcpy (reloadinfo.rrn, reload_rrn, 12);


	f_printf(stderr, " REC details [%s]\n", &reloadinfo);
        if(isread (fdrel, (char *)&reloadinfo, ISEQUAL) < 0)
        {
                f_printf(stderr, " REC details not found,finding with RRN only...\n");
                rlkey.k_flags = ISNODUPS;
                rlkey.k_nparts = 1;
                rlkey.k_part[0].kp_start = 0;
                rlkey.k_part[0].kp_leng = 32;
                rlkey.k_part[0].kp_type = CHARTYPE;

                memset ((char *)&reloadinfo, ' ' , sizeof(t_reloadinfo));
		memcpy (reloadinfo.card_number, reload_card, 7);
                /*memcpy (reloadinfo.rrn, reload_rrn, 12);*/
                reload_result = isstart( fdrel, &rlkey , 7 , (char *)&reloadinfo , ISEQUAL  );
                if(reload_result == 0)
                {
                        if(isread (fdrel, (char *)&reloadinfo, ISGTEQ) == 0)
                        {
                                while(iserrno != EENDFILE)
                                {
					if(memcmp(reloadinfo.rrn,reload_rrn,12) == 0)
					{
						f_printf(stderr, "reload_card [%.19s],RRN [%.12s]\n",reloadinfo.card_number,reloadinfo.rrn);
						memcpy(reload_card,reloadinfo.card_number,19);
						memcpy(reloadinfo.process_flag,"3" ,1 );
						if( isrewrite(fdrel, (char *)&reloadinfo ) < 0 )
						{
							f_printf(stderr, " Rewrite failed , error no is %d\n", iserrno);
							isclose(fdrel);
							return -1;
						}
						isclose(fdrel);
						return 1;
					}
					else
					{
                                        	isread(fdrel, (char *) &reloadinfo, ISNEXT);
					}
                                }
                        }
			f_printf(stderr, " Read error in RRN based search , error no is %d\n", iserrno);
                        isclose(fdrel);
                        return -1;
                }
                else
		{
			f_printf(stderr, " Read error , error no is %d\n", iserrno);
			isclose(fdrel);
			return -1;
		}
        }
        else
        {
                        memcpy(reloadinfo.process_flag,"3" ,1 );
                        if( isrewrite(fdrel, (char *)&reloadinfo ) < 0 )
                        {
                                f_printf(stderr, " Rewrite failed , error no is %d\n", iserrno);
                                isclose(fdrel);
                                return -1;
                        }
                        isclose(fdrel);
                        return 1;
        }
}
#endif

 /* MRL_288  Rishabh Changes*/
int UpdateRsaBcom()
{
	if((memcmp(rsa_flag,"Y",1) == 0) || (memcmp(rsa_flag,"B",1) == 0))
	{
		char count[3];
		int icount,Result;

		memcpy(count,bflzbcom.lbcom_rsa_flag_count,2) ;
		count[2]='\0';

		icount = (atoi(count)+1)%100;
		sprintf(count, "%02d", icount);

		memcpy (bflzbcom.lbcom_rsa_flag_count,count,2);
		f_printf(stderr, "\n Rsa_Flag_Count [%.2s] \n" ,count);

		/* TB: Tempararyly Blocked */
		if((memcmp(rsa_flag,"B",1) == 0))
		{
			f_printf(stderr, "Fraud Blocking. Flag [%s]\n", rsa_flag);
			memcpy (bflzbcom.lbcom_fraud_block_type,"TB",2 );
			memcpy (bflzbcom.lbcom_fraud_block_reason,"TEMPORARY_RCU_BLOCK",19 );
			memcpy(bflzbcom.lbcom_fraud_block_date, (char *)GetDateYYMMDD(), 6);
			memcpy(bflzbcom.lbcom_fraud_block_time,(char *)GetTime (),6);
		}

		f_printf(stderr, "\nACOUNT IS UNDER SYSTEM FRAUD BLOCK STATUS [%.2s] \n"
			,bflzbcom.lbcom_fraud_block_type);

		Result = isrewrite ( lnbcom , (char*)&bflzbcom);
		isunlock ( lnbcom );

		if ( Result != 0 )
		{
			FPRINTF ( stderr, "ONUS: REWRITE ERROR ACCOUNT %d %.19s \n", \
			iserrno, bflzbcom.lbcom_customer_id);
			return ( ECHEC );
		}
	}
	return (SUCCESS);
}
	

