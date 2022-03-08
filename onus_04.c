/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_04.c                                           *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 14-Feb-2000                                         *
 * Description         : Processes reversal messages (0400,1400 etc)         *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   04-Apr-00 Gilu Alex            Modified to reverse STIP count and     *
 *                                    for MTI 0400 and 0420                  *
 * 2   10-Apr-00 Gilu Alex            Modified to reverse card/account       *
 *                                    for message type 0420                  *
 * 3   19-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 4   11-Feb-04 Saju Joseph          CR-MXP-0401-0045                       *
 * 5   13-Dec-04 Veeresh E            Country wise Risk Parameter logic      * 
 * 5   13-Dec-04 Michel Alex          Reviewed-Country wise Risk Param logic * 
 *---------------------------------------------------------------------------*
 * 6   07-Feb-05 Veeresh              BR-MXP-0501-0004                       *
 * 6   07-Feb-05 Michel Alex          Reviewed BR-MXP-0501-0004              * 
 * 7   02aug06   Sheela 	      BR-MXP-0606-0020, BR-MXP-0606-0019     *
 *****************************************************************************/

/* Chowdesha added on 05-Oct-2011 for Fees calculation */
  #define PREPAID_CARD_FEES  1   

/* Chowdesha   added for CBA  */
 #define  CBA  1 

 #define QR_ISSUANCE 1

/* Chowdesha/Preema  added for CBA  */
 #define IDBI_BCHA_EHOST  1  
 
/* NAGESH BASWANTHARAO ADDED FOR SHELL */
  #define SHELL_PHASE1 1 
  #define INT_TRANS_LIMIT 1
  #define BOI_FRM 1
  #define CARD_CONTROL 1
  #define BFL_CUG_CARD 1
  #define ADDON        1  

  #define LODHA_PALAV 1 
  #define SELECT_CITY 1 
  #define KOTAK_UPDATION 1

  #define SFLP_FRM   1

  #define BOI_GPPC  1

  #define  SHELL_RELOAD_VOID_IS 1
  #define  BOBL_PREPAID 1
  #define PHOENIX_MALL 1  

  #define  LODHA_DB_26SEPT 1


 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>

#define MRL_244_172 

 #include <signal.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <memory.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <isam.h>
 #include <paczpa02.h>
 #include <paczpa03.h>
 #include <paczpa05.h>
 #include <paczpa11.h>
 #include <paczpoau.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczeuro.h>
 #include <paczauto.h>
 #include <paczisoauto.h>
 #include <paczport.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <ath-trc.h>
 #include <define.h>
 #include <iso_visa.h>
 #include <iso_mci.h>

 #include <onus.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <iso_aegn.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <scenario.h>
 #include <bankacnt.h>
 #include "onus_04.h"
 #include <bflcarddet.h>
 #define RUPAY_ISSUANCE 1   /* Added by Amit for RUPAY ISSUANCE on 1 SEP 2016 */

 #define SHELL_RELOAD_ISSUE 1   /* Added by Amit on 17 Mar 2017 for SHELL Reload Reversal Issue */

#define BFL_REMI_TO_PL

#define MRL_383	1

#ifdef BFL_REMI_TO_PL

        void BFL_Daily_Auth_Reverse();

        extern int lnprod;
        extern int lnrules;
     /*   extern int lnamnt;*/


        #include <remi_pl_rules.h>
        #include <loan_product_master.h>
        #include <cust_prod_amount.h>


        char sale_req_date[16];
        char txn_emi_amount[12+1];
        char txn_product[16];
#endif



/* Chowdesha added for HSBC DOP POS  on 10-06-2011 */
/* #define HSBC_DOP_POS  1  */

#ifdef CBA
	extern 	t_bankacnt bankacnt;  
#endif 

/* Chowdesha added on 26-05-2011 for HSBC DOP Phase 2*/
/*  #define HSBC_DOP_PHASE2  1 */ 
/* Chowdesha/Preema added to send the reversal to IDBI EHOST - Needs to be removed later */
/* #define IDBI_EHOST_ROUTING_NEVER  1 */

/* Chowdesha added on 25-Feb-2009 for VISA MANDATE to send back 430 */

/* #define VISAMANDATE  1 */
 
/* Chowdesha/Preema added to send 430 to partial reversal */ 
   #define VISAMANDATE_420   1 
  #define SBI_L 1 

/* Chowdesha added for the PREPAID CARD */
   #define PREPAID_CARD  1    
      #define STIP_PARAM    1 

/*  Chowdesha added for GIFT CARD */
#define GIFT_CARD   1 

/* Darshan added on 03.04.2012 to send approval for reversal transactions for which original transaction not found in MOVEMENT/HISTORY file*/
#define VISA_MANDATE_2012	1

 /*Added by darshan on 29.08.2012 for Visa Mandate changes of Oct 2012  */
 #define VISA_MCW_OCT2012     1
 #define VIJAYA_GIFT_CARD 1
 #define SHELL_VOID 1



char			ServiceCode[3+1];
char			pvv[5+1];
char			cvv[3+1];
char			PinFormat[3];
char			EffectiveDate[5];
char			CardSecurity[6];
char			LanguageCode[4];
char			User[7];

char			LocalAmount[13];

t_paczisoauto	paczisoauto;	/* Authorization(ISO data fields) Structure */
t_paczpa05		paczpa05;		/* Card Issuer details structure */
t_paczpoau		paczpoau;		/* Card holder details structure */
t_pcrzbale		pcrzbale;		/* Account deatils structure */
t_paczpa02		paczpa02;		/* Bank Details structure */
t_rthauth		rthauth;		/* RTH Authorization details */
t_paczau00		paczau00;		/* Authorization Details */
t_paczauto		paczauto;		/* Authorization History Details */
t_ath_trc		athtrc;			/* Authorization trace Details */

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;	/* DISPATCH PID */

/* SCENARIO CODES */
extern	char	CODE_SAUTO_HAUTO; 
extern	char	CODE_SCENARIO_A;
extern	char	CODE_SCENARIO_O;

/* ISAM Data File Descriptors */
extern	int		fdparam;	/* Parameter file */
extern	int		fdseuro;	/* EURO Authorization movement file */
extern	int		fdtrans;	/* Transaction file */
extern	int		fdnf00;		/* Common Negative file */
#ifdef BFL_CUG_CARD
#include <bflzbcom.h>
#include <bflzport.h>
#include <bflzbcli.h>
#include <bflzparm.h>
#include <bflauth00.h>
#include <bflhistoryauth.h>
#include <bfl_rthauth.h>
#include <BFLPROC.h>
#include <bfl_dealer_master.h>
#include <bflzaddon.h>

extern t_bflzport    bflzport;
extern t_bflzbcom    bflzbcom;
extern t_bflzbcli    bflzbcli;
extern t_bflauth00   bflauth00;
extern t_bflrthauth  bflrthauth;
extern t_bflzlpa20   bflzlpa20;
extern t_procdata    procdata;
extern t_bfl_dealer_master bfl_dealer_master;
extern t_lnaddport   lnaddport;
t_bflhistauth        bflhistauth;
extern t_bflcarddet  bflcarddet;
extern int lnbcli ; /*BFL CARD Client file */
extern int lnbcom ; /*BFL CARD ACCOUNT MASTER */
extern int lnporte; /*BFL CARD MASTER */
extern int lnsauto; /*BFL AUTHORISATION */
extern int lnhauto; /*BFL AUTHORISATION HISTORY */
extern int lnparam; /*BFL PARAMETERS*/
extern int lncrd;
extern int BFL_Update0400Account ( t_entete_iso* IsoHeader );
extern int Update0400RedeemPoints( );
extern int BFL_Update0400OpenEcsEmi( );
extern int BFL_CheckRecordInMovementAuth ( char* Amount, t_bflauth00* AuthRec, int mti );
extern int BFL_CheckRecordInHistoryAuth  ( char* Amount, t_bflhistauth* AuthRec, int mti );
extern int BFL_UpdateSauto ( char* SubResult );
extern void BFL_ReverseRthAuthRecord ( char* RRNO ,int Message_Type);
extern int  ReverseLoanVelocLimit(char * ,char *,char * );
extern int ReadAddonCard();
char* BFL_ProcessRequest_0400 ( t_struct_iso* );
extern int Mobile_No_Tranx ;
char Bfl_LocalAmount[13];
char Bfl_AdvEmiAmount[13];
char Bfl_AdvEcsAmount[13];
#endif

#ifdef SFLP_FRM
#include <onuspr05.h>
extern t_onuspr05  onuspr05;
#endif

extern t_paczpoau	dstpaczpoau;	/* Destination card details */

/* Function prototypes */
void Request_0400 ( t_struct_iso* );
char* ProcessRequest_0400 ( t_struct_iso* );
extern int VerifyCVV ( char* SubResult, int MessageType );
extern int VerifyPVV ( char * SubResult, int MessageType );


extern void FormatRRNOForMCI ( char* user, t_paczisoauto* ); 
extern char Orginal_RRN[];
extern void DumpMessage ( t_struct_iso* Request, char Format );
extern int GetResponseMTI ( t_msg_bl* IntrfMsg );
extern double atof ( char* );
extern int  DumpAuthTrace ( t_ath_trc *athtrc );
extern int DumpISOAuto ( t_paczisoauto* isoautoRec );
extern int FillAuto ( char* ISOData, char Format, t_paczisoauto* AuthRecord );
extern int GetIssuerDetails ( t_paczpa05* pa05, char* CardNumber );
extern int GetBankDetails ( char* BankCode, t_paczpa02* pa02 );
extern int GetCurrency ( char* TxnCurrency, char* Amount, char* CardholderCurrency );
extern int GetCardType ( char* CardNumber, char* BankCode, char* CardType );
extern int ReverseMCCProductBranch ( );
extern void LoadAuthTrace ( t_struct_iso* ISOStruct );
extern int	GetActualResponseCode ( char* Bank, char* ProductCode, char* SubResult, char* ActualCode );
#ifdef CARD_CONTROL
#endif
int OnusToDispatch_0410 ( char* Resp, char* SubResult, t_struct_iso* TxnData );
int get_account_type( char message_format, char *processing_code, int *account_type ) ;

char LOCAL_AMOUNT[13];  /* Azhakesh */
char AUTH_MCC    [ 5]; /* Saju.15/12/2003 . For ICICI Acc Cash Check */ 

#ifdef PREPAID_CARD
extern double  FltFeesAmount; 
extern  double	FltConvPercent; 
extern double FltConvPercent_amt; 
#endif 

#ifdef GIFT_CARD 
	extern double	FltGiftFeesAmount; 
#endif 

#ifdef STIP_PARAM    
char org_txn_date[9];
#endif 

#ifdef HSBC_DOP_POS
    extern double gcba_fees_amt; 
#endif 

#ifdef SHELL_RELOAD_ISSUE /* Added by Amit on 17 Mar 2017 for SHELL Reload Reversal Issue */
extern int iReloadPresentFile(char *reload_card, char * reload_rrn);
#endif

/* MRL_401 */
t_bflzlpa06 bflzlpa06;
void BFL_GET_REFUND_DAYS(char *product_code,char *refund_days);


#ifdef CBA
enum eAccountType 
{
	DEFAULT_ACCOUNT,
	SAVINGS_ACCOUNT,
	CHECKING_ACCOUNT,
	CREDIT_ACCOUNT,
	CREDIT_LINE_ACCOUNT,
	CORPORATE_ACCOUNT,
	UNIVERSAL_ACCOUNT,
	MONEY_MARKET_INV_ACCOUNT,
	STORED_VALUE_ACCOUNT,
	REVOLVING_ACCOUNT,
	SPENDING_POWER_ACCOUNT
};
  extern char Cbs_auth_code[ 8]; 
  extern char strcbs_bal_amt[40]; 
  extern    double gcba_fees_amt;
#endif 

#ifdef LODHA_PALAV
extern int Update_Bank_Level_Reload_Limit(int);
#endif


#ifdef SHELL_VOID
extern char Invoice_Number[7];
#endif
void Request_0400 ( t_struct_iso* Request)
{
	char	Result[6+1];
	char	ActualResponseCode[6+1];
	#ifdef CBA
	char *IsoData; 
	#endif 
	#ifdef IDBI_BCHA_EHOST
	char resp_code[5], auth_code[8],cbs_fld54[50]; 	 
    char fld_12[12], fld_13[8], user[7], fld_37_rrn[18]; 
	int iRetval = 0;
	#endif
  #ifdef CBA
	  memset(&bankacnt, ' ', sizeof(t_bankacnt)); 
	  memset(strcbs_bal_amt, 0x00, sizeof(strcbs_bal_amt));
	  memset(Cbs_auth_code, 0x00, sizeof(Cbs_auth_code));
   #endif  
	memset ( Result, 0, 6+1 );
	/* Process the authorization request */

        if(memcmp(Request->entete.banque, "00059", 5) == 0 )
		memcpy ( Result, BFL_ProcessRequest_0400( Request ), 6 );

	Result[3] = '4';

	/* Get the Actual response code */
	GetActualResponseCode ( Request->entete.banque, \
	paczpa05.pa05_product_code,Result, ActualResponseCode );


	f_printf(stderr,"SUBRESULT:[%.6s] ACTUAL CODE:[%.3s] \n", \
		Result, ActualResponseCode );

	/* Build the response packet, log the transaction and send the response to DISPATCH */


	OnusToDispatch_0410 ( ActualResponseCode, Result, Request );	
	CODE_SAUTO_HAUTO = 'S';

	LoadAuthTrace ( Request ); /* BR-MXP-0405-0014 Gilu Alex */

	/* Moved the initialization after updating the ATH-MTRC file */

	Request->entete.type = 410 ;

        if(memcmp(Request->entete.banque, "00059", 5) == 0 )
        BFL_LoadRthAuthRecord(Request);

	DumpAuthTrace ( &athtrc );

	BFLEmailSMS(Request);
	
	return;
}



int OnusToDispatch_0410 ( char* Response, char* SubResult, t_struct_iso* TxnData )
{
	t_entete_iso	IsoHeader;
	t_msg_bl		SendMsg;
	char			*IsoData;
	char			ResponseCode[4];
        #ifdef BFL_CUG_CARD
        char                    Response_Bfl[3];
        #endif
	int				AdditionalDataLen = 0;
	int				TempLen = 0;
	int				Result = ECHEC;

	FPRINTF ( stderr, "ONUSTODISPATCH (0410) \n" );

	IsoData = TxnData->data;
	EnteteCpy ( &(IsoHeader), &(TxnData->entete) ); 
	memset ( ResponseCode, 0, 4 );
        #ifdef BFL_CUG_CARD
        memset ( Response_Bfl,0x00,3);
        #endif

        #ifdef SFLP_FRM
        if(memcmp(onuspr05.frm_server, "Y", 1) == 0)
        {
                memcpy(TxnData->entete.banque, paczpa05.pa05_codbnq, 5);
                f_printf(stderr," FRM 0410... After Mapping bank code ..[%.5s]\n", TxnData->entete.banque);
        }
        #endif


	 /* MRL_207 */
        BFL_Update_count(Response);

 #ifdef CBA 
	if ( (memcmp(TxnData->entete.banque, "00011", 5) == 0) || (memcmp(TxnData->entete.banque, "00054", 5) == 0)
             #ifdef SFLP_FRM
             ||(memcmp(onuspr05.frm_server, "Y", 1) == 0)
            #else
             #ifdef BOI_FRM 
             ||(memcmp(TxnData->entete.banque, "00006", 5) == 0) 
             #endif
             #endif
           )
	{
 
		  f_printf(stderr, "CBA - Response, [%s], SubResult [%s] ", Response, SubResult); 
    	  if(IsoHeader.systeme == 'V')
				  analyse_visa(IsoData);
  		  else if(IsoHeader.systeme == 'M')
  				  analyse_mci(IsoData);
	}
	 #endif 

   

	if ( memcmp ( Response, "RR", 2 ) == 0 )
	{
		switch ( IsoHeader.systeme )
		{
		case 'V':	/* VISA/PLUS */
		case 'P':
                #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 1 SEP 2016 */
                case 'R':
                #endif
			if ( ( memcmp ( paczau00.au00_cd_raison_refus, "00", 2 ) == 0 ) ||
				( memcmp ( paczau00.au00_cd_raison_refus, "08", 2 ) == 0 ) ||
				( memcmp ( paczau00.au00_cd_raison_refus, "11", 2 ) == 0 ) )
			{
				memcpy(ResponseCode, "00", 2); /* DR-MXP-0911-0031 - 20.11.09 - Michel */
				InsertFieldVisa ( RESPONSE_CODE, IsoData, "00", &TempLen );
				AdditionalDataLen += TempLen;
			}
			else
			{
				memcpy(ResponseCode, "05", 2); /* DR-MXP-0911-0031 - 20.11.09 - Michel */ 
				InsertFieldVisa ( RESPONSE_CODE, IsoData, "05", &TempLen );
				AdditionalDataLen += TempLen;
			}
			break;

		case 'E':	/* EURO */
			if ( ( memcmp ( paczau00.au00_cd_raison_refus, "00", 2 ) == 0 ) ||
				( memcmp ( paczau00.au00_cd_raison_refus, "08", 2 ) == 0 ) ||
				( memcmp ( paczau00.au00_cd_raison_refus, "11", 2 ) == 0 ) )
			{
				memcpy(ResponseCode, "00", 2); /* DR-MXP-0911-0031 - 20.11.09 - Michel */
				InsertFieldEuro ( RESPONSE_CODE, IsoData, "00", &TempLen );
				AdditionalDataLen += TempLen;
			}
			else
			{
				memcpy(ResponseCode, "95", 2); /* DR-MXP-0911-0031 - 20.11.09 - Michel */
				InsertFieldEuro ( RESPONSE_CODE, IsoData, "95", &TempLen );
				AdditionalDataLen += TempLen;
			}
			break;
		}
	}
	else
	{
		switch ( IsoHeader.systeme )
		{
		case 'V':	/* VISA/PLUS */
		case 'P':
                #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 1 SEP 2016 */
                case 'R':
                #endif
			{
				/* Michel - VISA April 08 mandate 12.02.08 begin */
				char buff[15];
				char temp[5] ;
				char *pBuf ;
				char field_63_len ;

				memset( buff, 0x00, sizeof( buff ) );	
				pBuf = &buff[3] ;

				memset( temp, 0x00, 5 );
				memcpy( temp, paczisoauto.remaining_open_to_use, 4 );
				if( memcmp( temp, "\x00\x00\x00\x00", 4 ) != 0 )
				{
					/*
					AddSubField63( 1, &pBuf, temp,buff);
					*/
					AddSubField63( 0, &pBuf, temp,buff);
				}
				field_63_len = pBuf - buff ;

				InsertFieldVisa ( DEBIT_PRIVATE_USE, IsoData, buff, &field_63_len);
				AdditionalDataLen += field_63_len;


				memcpy ( ResponseCode, Response, 2 );
				if ( InsertFieldVisa ( RESPONSE_CODE, IsoData, ResponseCode, &TempLen ) == ECHEC )
				{
					ReplaceFieldVisa ( RESPONSE_CODE, IsoData, ResponseCode );
				}
				else
				{
					AdditionalDataLen += TempLen;
				}

                                #ifdef BFL_CUG_CARD
                                if((memcmp(IsoHeader.banque, "00059", 5) == 0 )&&(memcmp(Response, "00", 2) == 0)&&( memcmp ( paczisoauto.processing_code, "38", 2 ) == 0))
                                {
                                   char            Redeem_Balance[90];
                                   memset(Redeem_Balance,0x00,sizeof(Redeem_Balance));
                                   TempLen = 0;
                                   AddAdditionalTags(Redeem_Balance,REDEEMVOID_RESPONSE);
                                   f_printf(stderr, "\n 4. Redeem Response DE57 [%.45s]", Redeem_Balance);
                                   InsertFieldVisa ( RESERVERD_NATIONAL_57, IsoData , Redeem_Balance , &TempLen );
                                   AdditionalDataLen += TempLen;

                                }
                                if((memcmp(IsoHeader.banque, "00059", 5) == 0 )&&(memcmp(Response, "00", 2) == 0)&&( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0))
                                {
                               /*    char            Refund_Amount[20];
				
                                   char            Redeem_Balance[90];
                                   char            Redeem_Balance1[110];*/
                                   
				   char            Redeem_Balance[120];
                                   char            Redeem_Balance1[150];

                                   memset(Redeem_Balance,0x00,sizeof(Redeem_Balance));
                                   memset(Redeem_Balance1,0x00,sizeof(Redeem_Balance1));
                                   AddAdditionalTags(Redeem_Balance,SALEREFUND_RESPONSE);
                                   
                                   f_printf(stderr, "\n 6. Refund Amount in Response DE57 [%.150s]", Redeem_Balance);

				/*memset(Refund_Amount,0x00,sizeof(Refund_Amount));*/
                                   memcpy(Redeem_Balance1,paczisoauto.amount_transaction,12);
                                   memcpy(Redeem_Balance1+12,Redeem_Balance,strlen(Redeem_Balance));
                                 /*  memcpy(Refund_Amount,paczisoauto.amount_transaction,12);*/
                                   TempLen = 0;
                                   f_printf(stderr, "\n 5. Refund Amount in Response DE57 [%.12s]", Redeem_Balance1);
                                   f_printf(stderr, "\n 5. Refund Amount in Response DE57 [%.150s]", Redeem_Balance1+12);
                                   
                                   InsertFieldVisa ( RESERVERD_NATIONAL_57, IsoData , Redeem_Balance1, &TempLen );
                                   AdditionalDataLen += TempLen;

                                }
                                if((memcmp(IsoHeader.banque, "00059", 5) == 0 )&&(memcmp(Response, "00", 2) == 0)&&( memcmp ( paczisoauto.processing_code, "02", 2 ) == 0))
                                {
                                   char            Redeem_Balance[90];
                                   memset(Redeem_Balance,0x00,sizeof(Redeem_Balance));
                                   TempLen = 0;
                                   AddAdditionalTags(Redeem_Balance,SALEVOID_RESPONSE);
                                   f_printf(stderr, "\n 6. Void Of Sale  Response DE57 [%.45s]", Redeem_Balance);
                                   InsertFieldVisa ( RESERVERD_NATIONAL_57, IsoData , Redeem_Balance , &TempLen );
                                   AdditionalDataLen += TempLen;

                                }
                                if((memcmp(IsoHeader.banque, "00059", 5) == 0 )&&(memcmp(Response, "00", 2) == 0)&&( memcmp ( paczisoauto.processing_code, "00", 2 ) == 0))
                                {
                                   char            Redeem_Balance[90];
                                   memset(Redeem_Balance,0x00,sizeof(Redeem_Balance));
                                   TempLen = 0;
                                   AddAdditionalTags(Redeem_Balance,SALEVOID_RESPONSE);
                                   f_printf(stderr, "\n 6. Sale Reversal Response DE57 [%.45s]", Redeem_Balance);
                                   InsertFieldVisa ( RESERVERD_NATIONAL_57, IsoData , Redeem_Balance , &TempLen );
                                   AdditionalDataLen += TempLen;

                                }
                                #endif
			}
			break;

		case 'E':	/* EURO/MCI */
		case 'M':
			memcpy ( ResponseCode, Response, 2 );
			if ( InsertFieldMci ( RESPONSE_CODE, IsoData, ResponseCode, &TempLen ) == ECHEC )
			{
				ReplaceFieldMci ( RESPONSE_CODE, IsoData, ResponseCode );
			}
			else
			{
				AdditionalDataLen += TempLen;
			}

			/* preema add field 126 start */
			InsertFieldMci( PRIVATE_DATA, IsoData, \
			paczisoauto.reserved_private_126, &TempLen );
			AdditionalDataLen += TempLen;

			/* preema add field 126 end */
			break;

		case 'A':	/* AMEX */
			memcpy ( ResponseCode, Response, 3 );
			/* Veeresh */
			f_printf(stderr,"ResponseCode[%.3s]",ResponseCode);
			analyse_aegn ( IsoData );
			InsertFieldAegn ( ACTION_CODE, IsoData, ResponseCode, &TempLen );
			AdditionalDataLen += TempLen;
			break;
		}
	}

	EnteteCpy ( &(SendMsg.mtexte.entete), &(TxnData->entete) );
	SendMsg.mtype = (long) PIDROUT;
	SendMsg.mtexte.entete.source = getpid ();
	SendMsg.mtexte.entete.lng = AdditionalDataLen + TxnData->entete.lng;

	f_printf(stderr, " \n Response [%.2s] ", Response); 
	f_printf(stderr, "\n paczau00.au00_cd_raison_refus [%.2s] ", paczau00.au00_cd_raison_refus); 
        #ifdef BFL_CUG_CARD
        memcpy(Response_Bfl,Response,2);
        #endif

	/* COPY THE RESPONSE CODE TO ISO HEADER */
	if ( memcmp ( Response, "RR", 2 ) == 0 )
	{
		Cd_reponseToHeader ( &(SendMsg.mtexte.entete), ResponseCode );
	}
        #ifdef BFL_CUG_CARD
        else if((memcmp(IsoHeader.banque, "00059", 5) == 0 )&&(memcmp(Response, "00", 2) == 0))
		 Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "00" );
        #endif
#ifdef SHELL_RELOAD_VOID_IS
        else if ((memcmp(IsoHeader.banque, "00062", 5) == 0 )&&(memcmp(paczisoauto.national_pos_geographic_data, "     ", 6) != 0 ))
                Cd_reponseToHeader ( &(SendMsg.mtexte.entete), ResponseCode );
#endif
	else
	{
		if ( ( memcmp ( paczau00.au00_cd_raison_refus, "00", 2 ) == 0 ) ||
			( memcmp ( paczau00.au00_cd_raison_refus, "08", 2 ) == 0 ) ||
			( memcmp ( paczau00.au00_cd_raison_refus, "11", 2 ) == 0 ) )
		{
			Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "00" );
		}
		else
		{
			if ( memcmp ( IsoHeader.user, "EUROPY", 6 ) == 0 )
			{
				Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "95" );
			}
			else
			{
			#ifdef VISA_MANDATE_2012
				if( (CODE_SAUTO_HAUTO == 'N') && ( memcmp ( IsoHeader.user, "VISA", 4 ) == 0 ) )
						Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "00" );
				else 
               #endif
				if(memcmp(Response, "21", 2) == 0)
					 Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "21" );
                                #ifdef BFL_CUG_CARD
                                else if (memcmp(IsoHeader.banque, "00059", 5) == 0 )
					 Cd_reponseToHeader ( &(SendMsg.mtexte.entete), Response_Bfl );
                                #endif
				else
					Cd_reponseToHeader ( &(SendMsg.mtexte.entete), "05" );
			}
		}
	}

	memcpy ( athtrc.sub_result, SubResult, 6 );

	/* Copy the Data to Send MSg */
	memcpy ( SendMsg.mtexte.data, IsoData, SendMsg.mtexte.entete.lng );

	GetResponseMTI ( &SendMsg );
	EnteteCpy ( &( TxnData->entete ), &(SendMsg.mtexte.entete));
	/* Write the Response message to trace */
	DumpMessage ( &( SendMsg.mtexte ), TxnData->entete.systeme );

	/* Send Response Message to DISPATCH */
	Result = SendMsgToInterf ( &SendMsg, bal1 ); 


	memcpy( bflrthauth.response_code, SubResult, 2);
	if ( 0 == memcmp(bflrthauth.response_code, "00", 2 ) )
	{
		memcpy(bflrthauth.sub_result, SubResult, 6 );
		if( CODE_SAUTO_HAUTO == 'S' )
			memcpy(bflrthauth.approval_code,bflauth00.auth_code, 6);
		if( CODE_SAUTO_HAUTO == 'H' )
			memcpy(bflrthauth.approval_code,bflhistauth.auth_code, 6);
	}
	else
	{
		memcpy(bflrthauth.approval_code, "000000" , 6);
		memcpy(bflrthauth.sub_result, SubResult,2);
		memcpy(bflrthauth.sub_result+2, SubResult+2,4);
	}



        isunlock(lnbcli);
        isunlock(lnporte);
        isunlock(lnbcom);
        isunlock(lnsauto);
        isunlock(lnhauto);
        isunlock(fdparam);
        isunlock(lnparam);


	if ( Result < 0 )
	{
		FPRINTF ( stderr, "ONUS: Unable to send response msg to DISPATCH \n" );
		return ( ECHEC );
	}
	else
	{
		return ( SUCCESS );
	}
}




#ifdef CBA 
int get_account_type( char message_format, char *processing_code, int *account_type ) 
{
	char szAccountType[3] ;

	memset( szAccountType, 0x00, sizeof( szAccountType ) );
	memcpy( szAccountType, processing_code, 2 );
	if( message_format == MCI_SYSTEM )
	{
		switch (atoi( szAccountType ) )
		{
		case 0:
			*account_type = DEFAULT_ACCOUNT ;
			break ;
		
		case 10:
			*account_type = SAVINGS_ACCOUNT ;
			break ;
		
		case 20:
			*account_type = CHECKING_ACCOUNT ;
			break ;
		
		case 30:
			*account_type = CREDIT_ACCOUNT;
			break ;
		
		case 38:
			*account_type = CREDIT_LINE_ACCOUNT;
			break ;
		
		case 39:
			*account_type = CORPORATE_ACCOUNT ;
			break ;
		
		case 40:
			*account_type = UNIVERSAL_ACCOUNT ;
			break ;
		
		case 50:
			*account_type = MONEY_MARKET_INV_ACCOUNT;
			break ;
		
		case 60:
			*account_type = STORED_VALUE_ACCOUNT;
			break ;
		
		case 90:
			*account_type = REVOLVING_ACCOUNT ;
			break ;
		
		default:
			return ECHEC;
		}
		return SUCCESS ;

	}
	else if( message_format == VISA_SYSTEM )
	{
		switch (atoi( szAccountType ) )
		{
		case 0:
			*account_type = DEFAULT_ACCOUNT ;
			break ;
		
		case 10:
			*account_type = SAVINGS_ACCOUNT ;
			break ;
		
		case 20:
			*account_type = CHECKING_ACCOUNT ;
			break ;
		
		case 30:
			*account_type = CREDIT_ACCOUNT;
			break ;
		
		case 40:
			*account_type = UNIVERSAL_ACCOUNT ;
			break ;
		
		case 64:
			*account_type = SPENDING_POWER_ACCOUNT;
			break ;
		
		default:
			return ECHEC;
		}
		return SUCCESS ;
	}
	return ECHEC ;
}
#endif 

#ifdef BFL_CUG_CARD
char* BFL_ProcessRequest_0400 ( t_struct_iso* Request )
{
	int			MessageType;	/* MTI */
	double			TxnAmount = 0.0;
	double			TmpAmt = 0.0;
	char			LocalCurrency[4];
	char			Amount[13];
	char			MessageFormat;
	static char		SubResult[6+1];
	t_entete_iso		ISOHeader;		/* ISO HEADER */	
	char*			ISOData;		/* Bit MAP + DATA */

	char			ExpiryDate[4+1];
        char                    Customer_id[20];
        char                    Actual_Card_No[20];
	char			BankCode[6];
	char			AccountNumber[25];
	char			CardProduct[5];
        char                    visa_mci_country_code[4];
	char			temp_auto_reference[13]; /** Veeresh 141204 **/
        char                    AccessCode[7];
        char                    Refund_Days[4];
	char 			trans_amount[15];
	char  			FLD63_REV      [25];
        char                    Current_Refund_Day[9];
        char                    Current_Auth_Day[9];
        int                     Refund_Diff_Days = 0 ;
        int                     Return_Velocity  = -1 ;
	memset(trans_amount, 0x00, sizeof(trans_amount));
        memset( AccessCode , 0x00, sizeof(AccessCode));
        memset(Bfl_LocalAmount,0x00,sizeof(Bfl_LocalAmount));
        memset(Bfl_AdvEmiAmount,0x00,sizeof(Bfl_AdvEmiAmount));
        memset(Bfl_AdvEcsAmount,0x00,sizeof(Bfl_AdvEcsAmount));
        memset(Current_Refund_Day,0x00,sizeof(Current_Refund_Day));
        memset(Current_Auth_Day,0x00,sizeof(Current_Auth_Day));
	FPRINTF ( stderr, "BFL_ProcessRequest_0400 () \n" ); 
	memset(AUTH_MCC,0,5);
 
	/* MTI */
	MessageType = Request->entete.type;

	ISOData = Request->data;	/* Copy the ISO DATA to local variable */
	/* Copy the Header */
	EnteteCpy( &(ISOHeader), &(Request->entete) );
	/* Get the Message Format */
	MessageFormat = ISOHeader.systeme;

	/* Dump the request Data to Trace FIle */
	DumpMessage ( Request, MessageFormat );

	/*----------------------------------------------------------------------*/
	/* READ THE ISO MESSAGE FROM INPUT, CONSTRUCT THE PACZISOAUTO STRUCTURE */
	/*----------------------------------------------------------------------*/
	FillAuto ( ISOData, MessageFormat, &paczisoauto );
	paczisoauto.message_format = MessageFormat ;
        memset(&bflzport, ' ', sizeof( t_bflzport));
        memset(&bflzbcom, ' ', sizeof( t_bflzbcom));
        memset(&bflzbcli, ' ', sizeof( t_bflzbcli));
        memset(&lnaddport,' ', sizeof( t_lnaddport));
        memset((char *)&procdata,0x20,sizeof(t_procdata));
        memset(&bfl_dealer_master,0x20,sizeof(t_bfl_dealer_master));


	/* Dump the ISO auto struct */
	DumpISOAuto ( &paczisoauto );
        Refresh_strings();
        if(( memcmp ( ISOHeader.banque,"00059",5) == 0 )&& ( MessageFormat ==  'V' ))
        {
              memset(FLD63_REV,0x00,sizeof(FLD63_REV));
              GetFieldVisa ( NATIONAL_POS_GEOGRAPHIC_DATA, ISOData,FLD63_REV); 
              f_printf(stderr, "\n Field 59 data [%s]", FLD63_REV);
        }

	/***********************************************************************/
	/* IF THE REQUEST COME FROM EURO/MASTER, GET THE RRNO BASED ON
	   ACQ INSTITUTION CODE AND STAN */
	/***********************************************************************/
	TxnAmount = atof ( paczisoauto.amount_transaction );

	memset ( Amount, 0, 13 );
	memcpy ( Amount, paczisoauto.amount_transaction, 12 );

	
	f_printf(stderr,"packet amount[%.12s], Processing Code [%.2s]\n"
		,paczisoauto.amount_transaction,paczisoauto.processing_code);

	/* Below code changes to handle reversals of Void.
	 * We do not need to check the transaction amount for reversals of void.
	 * Ideally, Void of a sale transaction do not contain transaction amount.
	 * Since reversal fields are same as the original request, we do not expect
	 * the transaction amount for reversal of Voids.
	 */
	if((memcmp(paczisoauto.amount_transaction,"000000000000",12)==0) ||
		(strlen(paczisoauto.amount_transaction)==0))
	{
		FPRINTF ( stderr, "\nBFL Reversal request with zero's in Amount field");
                if((memcmp(paczisoauto.processing_code,"38",2) != 0)
                &&(memcmp(paczisoauto.processing_code,"20",2) != 0)
                &&(memcmp(paczisoauto.processing_code,"99",2) != 0)
                &&(memcmp(paczisoauto.processing_code,"03",2) != 0)
                &&(memcmp(paczisoauto.processing_code,"39",2) != 0)/* Reversal of Void */
                &&(memcmp(paczisoauto.processing_code,"02",2) != 0)
                &&(memcmp(paczisoauto.processing_code,"23",2) != 0))
		return ( "950403" );
	}

        if((memcmp(paczisoauto.processing_code,"38",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"39",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"20",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"02",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"00",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"99",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
        if((memcmp(paczisoauto.processing_code,"23",2) == 0)&& strlen(FLD63_REV) > 0 )
        taglenvalue(FLD63_REV,1);
	
	memset ( SubResult, '0', 6 );
       	memset ( Customer_id, 0, 20 );
       	memset ( Actual_Card_No, 0, 20 );

	/**************************************************************************/
	/*          CHECK THE CARD ISSUER (BANK)                                  */
	/**************************************************************************/
	f_printf(stderr,"Card Number [%.19s]\n",paczisoauto.primary_account_number);/* V5 theta migration Michel 09.02.2006 */ 
        if(memcmp( paczisoauto.primary_account_number,"222222",6) != 0 )
        {
		if ( GetIssuerDetails ( &paczpa05, paczisoauto.primary_account_number )  != SUCCESS )
		{
			/* RETURN ERROR CODE */
			return ( "950010" );
		}

		/* copy the Bank code */
		memset ( BankCode, 0, 6 );
		memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
        	/**********************************************************************/
        	/*        GET THE BANK DETAILS                                        */
        	/**********************************************************************/
        	if ( GetBankDetails ( BankCode, &paczpa02 ) == ECHEC )
        	{
                	return ( "950014" );
        	}
        	#ifdef NEVER
        	if( Read_ONUS_05(READ_PARAM) != SUCCESS)
        	{
                   	f_printf(stderr, " Error In Reading ONUS_05\n");
                   	return ( "050029");

        	}
        	#endif
        }

        /**************************************************************************/
        /*           CHECK CARD  MASTER                                           */
        /**************************************************************************/
        if(memcmp( paczisoauto.primary_account_number,"222222",6) == 0 )
        {
                 Mobile_No_Tranx = 0;
                 if( GetActualCardNumber(paczisoauto.primary_account_number,Customer_id,Actual_Card_No,SubResult)  != SUCCESS )
                  {
                        f_printf(stderr, "\nCARDHOLDER [%.19s]does not exist  Aginst this Mobile Number\n", paczisoauto.primary_account_number );
                        return ( "K50015" );

                  }
                 if ( BFLCheckCardNumber ( Actual_Card_No, &bflzport) != SUCCESS )
                  {
                        f_printf(stderr, "\nCARDHOLDER [%.19s]does not exist \n", paczisoauto.primary_account_number );
                        if ( iserrno == ELOCKED )
                                return ( "961002" );
                        else
                                return ( "K50001" );
                  }
                memcpy(Customer_id,bflzport.lport_cust_id,19);
                f_printf(stderr, "\n Customer_id [%s]", Customer_id);
                memcpy(paczisoauto.primary_account_number,bflzport.lport_card_no,19);
                if ( GetIssuerDetails ( &paczpa05, paczisoauto.primary_account_number )  != SUCCESS )
                {
                        f_printf(stderr, "\nERROR : CARD ISSUER [%.9s]does not exist \n", \
                        paczisoauto.primary_account_number );
                        return ( "961001" );
                }

                /* copy the Bank code */
                memset ( BankCode, 0, 6 );
                memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
                /**********************************************************************/
                /*        GET THE BANK DETAILS                                        */
                /**********************************************************************/
                if ( GetBankDetails ( BankCode, &paczpa02 ) == ECHEC )
                {
                        return ( "950014" );
                }
                Mobile_No_Tranx = 1;

        }
        else
        {
                Mobile_No_Tranx = 0;
                if ( BFLCheckCardNumber ( paczisoauto.primary_account_number, &bflzport) != SUCCESS )
                {
                        f_printf(stderr, "\nCARDHOLDER [%.19s]does not exist \n", paczisoauto.primary_account_number );
                        if ( iserrno == ELOCKED )
                                return ( "961002" );
                        else
                                return ( "K50001" );
                }
                #ifdef ADDON 
                if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0 )
                {
                        if( ReadAddonCard() != SUCCESS )
                        {
                            if ( iserrno == ELOCKED )
                                return ( "961002" );
                            else
                                return ( "B30001" );
                        }
                }
                #endif
                memcpy(Customer_id,bflzport.lport_cust_id,19);
                f_printf(stderr, "\n Customer_id [%s]", Customer_id);
        }


        /**************************************************************************/
        /*           CHECK ACCOUNT  MASTER                                        */
        /**************************************************************************/
        if ( BFLAccountNumber (Customer_id, &bflzbcom) != SUCCESS )
        {
                f_printf(stderr, "\nAccount Master  [%.19s]does not exist \n", Customer_id);
                        if ( iserrno == ELOCKED )
                                return ( "961013" );
                        else
                                return ( "K60013" );
        }
        f_printf(stderr, "\nBefore   BFLAccountNumber \n" );

        /**************************************************************************/
        /*           CHECK CLIENT RECORD                                          */
        /**************************************************************************/
        if ( BFLClientNumber (Customer_id, &bflzbcli) != SUCCESS )
        {
                f_printf(stderr, "\nClient  Master  [%.19s]does not exist \n", Customer_id);
                        if ( iserrno == ELOCKED )
                                return ( "961013" );
                        else
                                return ( "K70014" );
        }

        /**************************************************************************/
        /*          CHECK CARD SYSTEM BLOCK STATUS FOR ALL NON-REVERSALS TXNS.    */
        /**************************************************************************/
	/******PIN VERIFICATION NOT REQUIRED FOR REVERSALS*******/
	if(
		(memcmp(paczisoauto.processing_code,"00",2) != 0)
		&& (memcmp(paczisoauto.processing_code,"99",2) != 0)
		&&(memcmp(paczisoauto.processing_code,"23",2) != 0)
		&&(memcmp(paczisoauto.processing_code,"03",2) != 0)
		&&(memcmp(paczisoauto.processing_code,"39",2) != 0)
		) 
	{
		/******PIN VERIFICATION NOT REQUIRED FOR VOID&REFUND FOR E-COMM*******/
		if (
			(memcmp(transaction_source, "CI", 2)!=0) &&
			(memcmp(transaction_source, "CP", 2)!=0) &&
			(memcmp(transaction_source, "XI", 2)!=0) &&
			(memcmp(transaction_source, "XP", 2)!=0) &&
			(memcmp(transaction_source, "AI", 2)!=0) &&
			(memcmp(transaction_source, "AP", 2)!=0) &&
			(memcmp(transaction_source, "RI", 2)!=0) &&
			(memcmp(transaction_source, "RP", 2)!=0) &&
			(memcmp(transaction_source, "SI", 2)!=0) &&
			(memcmp(transaction_source, "SP", 2)!=0) 
		)
		{
			if ( checkOtpPinBlock_BFL(SubResult) != SUCCESS)
			{
				return ( SubResult );
			}
		}
		else
		{
			f_printf(stderr, "Bypassing PIN-OTP check for VOID-REFUND for E-COMM.\n");
		}
	}
	else
	{
		f_printf(stderr, "Bypassing PIN-OTP check for reversals.\n");
	}
         
	#ifdef NEVER
        /**************************************************************************/
        /*          CHECK ACCOUNT SYSTEM BLOCK STATUS                             */
        /**************************************************************************/
         if ( checkAccountSystemBlock_BFL() != SUCCESS)
         {
                        return ( "K40015" );
         }
         #endif
        
        /**************************************************************************/
        /*          CHECK CARD EXPIRY DATE                                        */
        /**************************************************************************/
         if (BFLValidateExpiry()  != SUCCESS )
         {
                     f_printf(stderr, "EXPIRED CARD\n" );
                     memcpy(SubResult,"C30006",6);
                     return ( SubResult );
         }
        /**************************************************************************/
        /*          CHECK CARD EXPIRY DATE                                        */
        /**************************************************************************/
          	if(BFL_CardTypeProductCode() != SUCCESS )
          	{
               		f_printf(stderr, "ERROR IN READING REFUND DAYS-CARD PRODUCT PARAMETERS\n" );
                        memcpy(SubResult,"961006",6);
                        return ( SubResult );
   	  	}
         if(memcmp(paczisoauto.processing_code,"20",2) == 0)
         {
                memset(Refund_Days,0,sizeof(Refund_Days));
                memcpy(Refund_Days,bflzlpa20.lpa20_refund_days,3);
         }
        /**************************************************************************/
        /*          CHECK CARD ACCESS CODE - PIN                                  */
        /**************************************************************************/
	f_printf(stderr,"Product Source : [%.2s]\n",transaction_source);
	f_printf(stderr,"Processing Code : [%.2s]\n",paczisoauto.processing_code);
         if(
		(memcmp(paczisoauto.processing_code,"00",2) != 0) 
		&& (memcmp(paczisoauto.processing_code,"99",2) != 0)
            	&&(memcmp(paczisoauto.processing_code,"23",2) != 0)
		&&(memcmp(paczisoauto.processing_code,"03",2) != 0)
            	&&(memcmp(paczisoauto.processing_code,"39",2) != 0) 
	  ) /******PIN VERIFICATION NOT REQUIRED FOR REVERSALS*******/
         {
	 	if(!( (( memcmp( transaction_source,         "CI", 2 ) == 0 )||( memcmp( transaction_source,"AP",2)==0)) 
		   && (( memcmp( paczisoauto.processing_code,"02", 2 ) == 0 )||( memcmp( paczisoauto.processing_code,"20",2) == 0))) 
		 
		 /****** MRL-445: OTP validation for Cancellation for REMI ECOM API ******/
		   
		   || (( memcmp( buisness_prod_code, "REMI", 4 ) == 0 )
		   && (( memcmp( transaction_source, "RI",   2 ) == 0 )||( memcmp( transaction_source, "RP", 2 ) == 0 ))
		   && (( memcmp( paczisoauto.processing_code,"02", 2 )==0)||( memcmp( paczisoauto.processing_code,"20", 2 )==0 )))
	    	  )
		/* No PIN check for REFUND & VOID */
		{
			f_printf(stderr, "PIN VERIFICATION \n" );
			if ( VerifyPIN_BFL ( AccessCode ,SubResult) != SUCCESS )
			{
				f_printf(stderr, "PIN VERIFICATION FAILURE\n" );
				return ( SubResult );
			}
		}
		else
		{
			f_printf(stderr, "Refund/VOID transaction. ACCESSCODE BYPASS.\n");
		}
         }
        /**********************************************************************/
	/* CHECK IF AUTHORIZATION IS IN AUTH MOVEMENT FILE - LN-SAUTO  	      */
	/**********************************************************************/
	FPRINTF ( stderr, " LN-SAUTO CHECK \n" );
	FPRINTF ( stderr, " Acquirer Refrence number = %.12s \n", paczisoauto.retrieval_reference_number );
	FPRINTF ( stderr, " Primary Account Number = %.19s \n", paczisoauto.primary_account_number );
	FPRINTF ( stderr, " Merchant Id = %.15s \n", ISOHeader.commercant );
	FPRINTF ( stderr, " Transaction Amount = %.12s \n", Amount );

	memset ( (char*)&bflauth00, 0x20, sizeof ( t_bflauth00) );
	memset ( (char*)&bflhistauth, 0x20, sizeof ( t_bflhistauth ) );

#ifdef BFL_REMI_TO_PL
        memset(sale_req_date, ' ', sizeof(sale_req_date));
        memset(txn_emi_amount, ' ', sizeof(txn_emi_amount));
        memset(txn_product, ' ', sizeof(txn_product));
#endif

	if ( BFL_CheckRecordInMovementAuth( paczisoauto.amount_transaction, &bflauth00, MessageType ) == SUCCESS )
	{
	#ifdef BFL_REMI_TO_PL
		memcpy(sale_req_date, bflauth00.auth_req_date, 6);
		memcpy(txn_emi_amount, bflauth00.emi_amount, 12);
		txn_emi_amount[12]='\0';
		memcpy(txn_product, bflauth00.product_id, 10);
		txn_product[10]='\0';
	#endif
		f_printf(stderr, "RECORD EXISTS IN LN-SAUTO \n" );
		FPRINTF ( stderr, "merchant_no [%.15s]\n",bflauth00.merchant_no);
		FPRINTF ( stderr, "terminal_no[%.8s]\n",bflauth00.terminal_no);
		FPRINTF ( stderr, "Amount [%.12s]\n",bflauth00.transaction_amount);
		FPRINTF ( stderr, "Response Code[%.2s]\n",bflauth00.response_code);
                f_printf(stderr, "RECORD EXISTS IN BFL-SAUTO \n" );
                if ( ( memcmp ( paczisoauto.amount_transaction, bflauth00.transaction_amount, 12 ) != 0 ) ||
                        ( memcmp (bflauth00.response_code, "00", 2 ) != 0 ) )
                {
                    if(bflauth00.auth_source_rev[0]!='R' && (( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0) || ( memcmp ( paczisoauto.processing_code, "02", 2 ) == 0)))
                    return ( "120000" );
                    #ifdef NEVER
                        else if(bflauth00.auth_source_rev[0]=='R')
			{
			  /* Reversal of Void */
			  BFL_UpdateSauto ( SubResult );
                          return ( "000000" );
			}
                    #endif
			else
                         return ( "000000" );
                }
                else
                {
                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "99", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "02", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "02", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "20", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "02", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "99", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "38", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "02", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "38", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "20", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "99", 2 ) == 0)&&( memcmp(bflauth00.auth_processing_code, "99", 2 ) != 0))
                         return ( "120000" );
                      
                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&
                           (( bflauth00.auth_source_rev[0] == 'R' ) || ( bflauth00.auth_source_rev[0] == 'H' ) ))
                         return ( "K30000" );
                        else if(( memcmp ( paczisoauto.processing_code, "02", 2 ) == 0)&&
                           (( bflauth00.auth_source_rev[0] == 'R' ) || ( bflauth00.auth_source_rev[0] == 'H' ) ))
                         return ( "K60000" );
                        else if(( memcmp ( paczisoauto.processing_code, "38", 2 ) == 0)&&
                           (( bflauth00.auth_source_rev[0] == 'R' ) || ( bflauth00.auth_source_rev[0] == 'H' ) ))
                         return ( "K60000" );
                        else if(( memcmp ( paczisoauto.processing_code, "99", 2 ) == 0)&&
                           (( bflauth00.auth_source_rev[0] == 'R' ) || ( bflauth00.auth_source_rev[0] == 'H' ) ))
                         return ( "K60000" );
                        else
                        memcpy ( SubResult, "000000", 6 );
                }

                if( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)
                {
			/* MRL_401 */

			char product_code[11];
			memcpy(product_code, bflauth00.product_id,10);
			product_code[10]='\0';

			/* Get product-level Refund days if defined */
			BFL_GET_REFUND_DAYS(product_code,Refund_Days);

                  	if(atoi(Refund_Days)!=0)
                  	{
                                f_printf(stderr,"Refund date {%.6s} ,Auth Date {%.6s}\n",GetDateYYMMDD (),bflauth00.auth_req_date);
                                memcpy(Current_Refund_Day,"20",2);
                                memcpy(Current_Refund_Day+2, (char*)GetDateYYMMDD (),6);
                                memcpy(Current_Auth_Day,"20",2);
                                memcpy(Current_Auth_Day+2,bflauth00.auth_req_date,6);
                                Refund_Diff_Days = DateDifferenceInDays(Current_Refund_Day,Current_Auth_Day);
                                f_printf(stderr,"Refund date {%.8s} ,Auth Date {%.8s}\n",Current_Refund_Day,Current_Auth_Day);
                                f_printf(stderr,"Refund Days Difference {%d} \n",Refund_Diff_Days);
                          	if(Refund_Diff_Days  > atoi(Refund_Days) )
                          	{
                                        f_printf(stderr,"Refund [%d]is Not allowed after {%d} Days\n",Refund_Diff_Days,atoi(Refund_Days));
                         		return ( "L10000" );
                          	}
                  	}
                }
		CODE_SAUTO_HAUTO = 'S';
		f_printf(stderr,"Transacion amount [%.12s]\n",paczisoauto.amount_transaction);
		memcpy( Bfl_LocalAmount , bflauth00.transaction_amount, 12 );
		memcpy( Bfl_AdvEmiAmount, bflauth00.advance_emi_amount, 12 );
		memcpy( Bfl_AdvEcsAmount, bflauth00.emi_amount, 12 );
		Bfl_LocalAmount[12] = 0;
                Bfl_AdvEmiAmount[12] = 0 ;
                Bfl_AdvEcsAmount[12] = 0 ;
		sprintf(temp_auto_reference,"%.12s",bflauth00.reference_number);
	}
        /* SEARCH IN HISTORY AUTH FILE - LN-HAUTO */ /***********************************************************************/
        else if ( BFL_CheckRecordInHistoryAuth( paczisoauto.amount_transaction, &bflhistauth, MessageType ) == SUCCESS )
	{
	#ifdef BFL_REMI_TO_PL
		memcpy(sale_req_date, bflhistauth.auth_req_date, 6);
		memcpy(txn_emi_amount, bflhistauth.emi_amount, 12);
		txn_emi_amount[12]='\0';
		memcpy(txn_product, bflhistauth.product_id, 10);
		txn_product[10]='\0';
	#endif
                f_printf(stderr, "RECORD EXISTS IN LN-AUTP \n" );
                FPRINTF ( stderr, "merchant_no [%.15s]\n",bflhistauth.merchant_no);
                FPRINTF ( stderr, "terminal_no[%.8s]\n",bflhistauth.terminal_no);
                FPRINTF ( stderr, "Amount [%.12s]\n",bflhistauth.auth_amount);
                FPRINTF ( stderr, "Response Code[%.2s]\n",bflhistauth.response_code);
                f_printf(stderr, "RECORD EXISTS IN  LN-AUTP \n" );
                memset ( (char*)&bflauth00, 0x20, sizeof ( t_bflauth00) );
                if ( ( memcmp ( paczisoauto.amount_transaction, bflhistauth.transaction_amount, 12 ) != 0 ) ||
                        ( memcmp (bflhistauth.response_code, "00", 2 ) != 0 ) )
                {
                        if( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)
                         return ( "L20000" );
                        else
                         return ( "000000" );
                }
                else
                {
                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&( memcmp(bflhistauth.auth_processing_code, "99", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&( memcmp(bflhistauth.auth_processing_code, "02", 2 ) == 0))
                         return ( "120000" );
                        if(( memcmp ( paczisoauto.processing_code, "99", 2 ) == 0)&&( memcmp(bflhistauth.auth_processing_code, "99", 2 ) != 0))
                         return ( "120000" );

                        if(( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)&&
                           (( bflhistauth.auth_source_rev[0] == 'R' ) || ( bflhistauth.auth_source_rev[0] == 'H' ) ))
                          return ( "K30000" );
                        else if(( memcmp ( paczisoauto.processing_code, "99", 2 ) == 0)&&
                           (( bflhistauth.auth_source_rev[0] == 'R' ) || ( bflhistauth.auth_source_rev[0] == 'H' ) ))
                          return ( "K60000" );
                        else if(( memcmp ( paczisoauto.processing_code, "23", 2 ) == 0)&&
                           ( bflhistauth.auth_source_rev[0] == 'A' ))
                          return ( "K60000" );
                        else
                        memcpy ( SubResult, "000000", 6 );
                 }

                if( memcmp ( paczisoauto.processing_code, "20", 2 ) == 0)
                {
			/* MRL_401 */

			char product_code[11];
			memcpy(product_code, bflhistauth.product_id,10);
			product_code[10]='\0';

			/* Get product-level Refund days if defined */
			BFL_GET_REFUND_DAYS(product_code,Refund_Days);

                  	if(atoi(Refund_Days)!=0)
                  	{
                                f_printf(stderr,"Refund date {%.6s} ,Auth Date {%.6s}\n",GetDateYYMMDD (),bflhistauth.auth_req_date);
                                memcpy(Current_Refund_Day,"20",2);
                                memcpy(Current_Refund_Day+2,(char*)GetDateYYMMDD (),6);
                                memcpy(Current_Auth_Day,"20",2);
                                memcpy(Current_Auth_Day+2,bflhistauth.auth_req_date,6);
                                Refund_Diff_Days = DateDifferenceInDays(Current_Refund_Day,Current_Auth_Day);
                                f_printf(stderr,"Refund date {%.8s} ,Auth Date {%.8s}\n",Current_Refund_Day,Current_Auth_Day);
                                f_printf(stderr,"Refund Days Difference {%d} \n",Refund_Diff_Days);
                                if(Refund_Diff_Days  > atoi(Refund_Days) )
                                {
                                        f_printf(stderr,"Refund [%d]is Not allowed after {%d} Days\n",Refund_Diff_Days,atoi(Refund_Days));
                                        return ( "L10000" );
                                }
                        }
                }
                CODE_SAUTO_HAUTO = 'H';
                f_printf(stderr,"Transacion amount [%.12s]\n",paczisoauto.amount_transaction);
                memcpy( Bfl_LocalAmount , bflhistauth.transaction_amount, 12 );
                memcpy( Bfl_AdvEmiAmount, bflhistauth.advance_emi_amount, 12 );
                memcpy( Bfl_AdvEcsAmount, bflhistauth.emi_amount, 12 );
                Bfl_LocalAmount[12] = 0;
                Bfl_AdvEmiAmount[12] = 0 ;
                Bfl_AdvEcsAmount[12] = 0 ;
                sprintf(temp_auto_reference,"%.12s",bflhistauth.reference_number);
	}
	else
	{
		FPRINTF ( stderr, "ORIGINAL TXN NOT IN LN-SAUTO/LN-AUTP AUTH FILE \n");
		memcpy ( SubResult, "K80000", 6 );
		CODE_SAUTO_HAUTO = 'N';
	}

        /**************************************************************************/
	/*   	    AMOUNT CONVERSION 											  */
	/**************************************************************************/
	memset ( Amount, 0, 13 );
	memset ( LOCAL_AMOUNT, 0, 13 );

	sprintf( LOCAL_AMOUNT, "%.12s", LocalAmount );   /* Azhakesh */
	LOCAL_AMOUNT[12] = 0 ;
	f_printf(stderr,"LOCAL AMOUNT [%s]\n",LOCAL_AMOUNT); /* V5 theta migration Michel 09.02.2006 */

	
	if ( memcmp ( SubResult, "00", 2 ) == 0 )
	{
		/* Check if the transaction is already reversed or not */
                if((memcmp(paczisoauto.processing_code,"23",2) == 0) || (memcmp(paczisoauto.processing_code,"03",2) == 0)|| (memcmp(paczisoauto.processing_code,"39",2) == 0))
                {
                   f_printf(stderr,"Refund Reversal/Void Sale Reversal/Void Redeem Reversal");
                   if( bflauth00.auth_source_rev[0] == 'A' )
                   return( SubResult );
                }
		else if( CODE_SAUTO_HAUTO == 'S' )
		{
			if( bflauth00.auth_source_rev[0] == 'R' )
				return( SubResult );
		}
		else if( CODE_SAUTO_HAUTO == 'H' )
		{
			if(( bflhistauth.auth_source_rev[0] == 'H' ) || ( bflhistauth.auth_source_rev[0] == 'R' ))
				return( SubResult );
		}
	
	}
	if( 	CODE_SAUTO_HAUTO == 'N' )
	{
		return( SubResult );
	}

	
	/**************************************************************************/
	/* IF ORIGINAL TRANSACTION PRESENT AND APPROVED, REVERSE TRANSACTION      */
	/**************************************************************************/
	if ( memcmp ( SubResult, "00", 2 ) == 0 )
	{
		char replace_amount[13] ;
		char amount_trans[13] ;

		memset( replace_amount, 0x00, 13 );
		memset( amount_trans, 0x00, 13 );

		memcpy( replace_amount,paczisoauto.replacement_amounts, 12);
		memcpy( amount_trans, paczisoauto.amount_transaction, 12) ;
                
                /****************API-------CALL*****************/
             if((memcmp(paczisoauto.processing_code,"00",2) == 0)||(memcmp(paczisoauto.processing_code,"02",2) == 0)||(memcmp(paczisoauto.processing_code,"20",2) == 0))
                {
                   if(((CODE_SAUTO_HAUTO == 'H')&&(memcmp(bflhistauth.product_id,"REMI",4) !=  0 ))||((CODE_SAUTO_HAUTO == 'S')&&(memcmp(bflauth00.product_id,"REMI",4) !=  0 )))
                   {
                  if(((CODE_SAUTO_HAUTO == 'H')&&(memcmp(bflhistauth.product_id,"ECF",3) !=  0 ))||((CODE_SAUTO_HAUTO == 'S')&&(memcmp(bflauth00.product_id,"ECF",3) !=  0 )))
                     {
			if(((CODE_SAUTO_HAUTO == 'H')&&(memcmp(bflhistauth.product_id,"ERF",3) !=  0 ))||((CODE_SAUTO_HAUTO == 'S')&&(memcmp(bflauth00.product_id,"ERF",3) !=  0 )))
			{	
			   if(((CODE_SAUTO_HAUTO == 'H')&&(memcmp(bflhistauth.product_id,"WLT",3) !=  0 ))||((CODE_SAUTO_HAUTO == 'S')&&(memcmp(bflauth00.product_id,"WLT",3) !=  0 )))
			   {	
					#ifdef MRL_244_172
					if(memcmp(paczisoauto.processing_code, "200006", 6) != 0)
					{
						#endif
						if(Update_Reversal() != 0 )
						{
							f_printf(stderr,"REAL TIME VOID/REFUND IS NOT APPROVED FROM BFL");
							return ( "L20000" );
						}
						#ifdef MRL_244_172
					}
					#endif

					if(memcmp(paczisoauto.processing_code,"20",2) == 0)
						BFL_Update0400( &ISOHeader );
			   }
			}
                     }
                   }
                }
                /****************API-------CALL*****************/

                if(memcmp(paczisoauto.processing_code,"00",2) == 0)
			BFL_Update0400( &ISOHeader );
                else if(memcmp(paczisoauto.processing_code,"02",2) == 0)
			BFL_Update0400( &ISOHeader );
                else if(memcmp(paczisoauto.processing_code,"03",2) == 0)
			BFL_Update0400( &ISOHeader );
                else if(memcmp(paczisoauto.processing_code,"38",2) == 0)
			Update0400RedeemPoints(); 
                else if(memcmp(paczisoauto.processing_code,"39",2) == 0)
			Update0400RedeemPoints(); 
                else if(memcmp(paczisoauto.processing_code,"99",2) == 0)
			Update0400RedeemPoints(); 

	#ifdef BFL_REMI_TO_PL
		if(
			(memcmp(paczisoauto.processing_code,"20",2) == 0) ||
			(memcmp(paczisoauto.processing_code,"00",2) == 0) ||
			(memcmp(paczisoauto.processing_code,"02",2) == 0) ||
			(memcmp(paczisoauto.processing_code,"03",2) == 0)
		)
			BFL_Daily_Auth_Reverse();
	#endif
	
             
		FPRINTF ( stderr, "REVERSE VELOCITY COUNT \n" );
		FPRINTF ( stderr, "Local Amount[%.12s] \n", LocalAmount );

		if (( memcmp ( ISOHeader.user, "PosISO", 6 ) == 0 ) && 
                    ((memcmp(paczisoauto.processing_code,"00",2) == 0)||(memcmp(paczisoauto.processing_code,"02",2) == 0)||(memcmp(paczisoauto.processing_code,"20",2) == 0)))
		{
			/*MRL_208*/
                        if( CODE_SAUTO_HAUTO == 'S' )
                        Return_Velocity = ReverseLoanVelocLimit_ltv(bflauth00.auth_req_date,bflauth00.reference_number);
                        else if ( CODE_SAUTO_HAUTO == 'H' ) /************************/
                        Return_Velocity = ReverseLoanVelocLimit_ltv(bflhistauth.auth_req_date, bflhistauth.reference_number);
                        FPRINTF ( stderr, "Return Value [%d] \n" ,Return_Velocity);

		}
		f_printf(stderr, "Going to update Charge ID in SAUTO for Refund\n");	
		#ifdef MRL_383
		f_printf(stderr,"Writing Charge ID and REMI_EMI_CARD_CHARGES in LN-SAUTO for Refund\n");
		memcpy ( bflauth00.charge_id1, "00601635", 8 );
		memcpy ( bflauth00.charge_id1_amount, bflzport.lport_emi_card_charges, 12 );
		fprintf(stderr,"Charge ID1 [%.8s] and CARD_ID1_AMOUNT [%.12s] in RTH-AUTH\n",bflauth00.charge_id1,bflauth00.charge_id1_amount);
		#endif

		/* Update the Auth Movement file */
		BFL_UpdateSauto ( SubResult );

		FPRINTF ( stderr, "UPDATE RECORD STATUS IN BFL-RTH-AUTH \n" );
                if( CODE_SAUTO_HAUTO == 'S' )
		BFL_ReverseRthAuthRecord ( bflauth00.reference_number ,MessageType);
                if( CODE_SAUTO_HAUTO == 'H' )
		BFL_ReverseRthAuthRecord ( bflhistauth.reference_number ,MessageType);

		/* CARD TYPE 58 and PRODUCT REMI  DELETE RRN*/
		#ifdef MRL_383
		f_printf(stderr, "txn_product [%.4s] & bflzport.lport_mxp_card_type [%.2s]\n",txn_product,bflzport.lport_mxp_card_type);
		if(memcmp(txn_product,"REMI",4) ==  0 && memcmp(bflzport.lport_mxp_card_type, "58", 2) == 0)
		{
			f_printf(stderr, "Refund RRN Deletion\n");
			f_printf(stderr, "Current Reference Number -: [%.12s]\n",paczisoauto.retrieval_reference_number);
                	f_printf(stderr, "Reference Number [%.12s] for Card[%.19s]\n",bflcarddet.lcrd_new_rrn,bflzport.lport_card_no);
        		if(memcmp(bflcarddet.lcrd_new_rrn, paczisoauto.retrieval_reference_number, 12)==0)
        		{
                		f_printf(stderr, "REFUND of First Sale transaction.\n");
                		memcpy(bflcarddet.lcrd_new_rrn, "            ", 12);
				memcpy(bflcarddet.lcrd_last_auth_dt,"      ",6);
				fprintf(stderr,"\"%s\",Function \"%s\" \n",__FILE__, __FUNCTION__);
				UpdateLNCrdDet();
        		}

		}
		#endif
	}
                memcpy( bflrthauth.response_code, SubResult, 2);
                if ( 0 == memcmp(bflrthauth.response_code, "00", 2 ) )
                {
                        memcpy(bflrthauth.sub_result, SubResult, 6 );
                        if( CODE_SAUTO_HAUTO == 'S' )
                        memcpy(bflrthauth.approval_code,bflauth00.auth_code, 6);
                        if( CODE_SAUTO_HAUTO == 'H' )
                        memcpy(bflrthauth.approval_code,bflhistauth.auth_code, 6);
                }
                else
                {
                        memcpy(bflrthauth.approval_code, "000000" , 6);
                        memcpy(bflrthauth.sub_result, SubResult,2);
                        memcpy(bflrthauth.sub_result+2, SubResult+2,4);
                }

	return ( SubResult );
}


#ifdef BFL_REMI_TO_PL
void BFL_Daily_Auth_Reverse()
{
        t_cust_prod_amount cust_prod_amount;
        t_product_master product_master;
	t_remi_pl_rules remi_pl_rules;

        char today_amount[12+1];
        int itoday_amount;

	char prod_cap_limit[12+1];
	int iprod_cap_limit;

        int itxn_emi_amount;
        char offset[16];
        char sysdate[16];
	char cust_open[100];
	int lnamnt =0;

        memcpy(sysdate, (char*)cGetTimeYYYYMMDDHHMMSS()+6, 2); /* YY */
        memcpy(sysdate+2, (char*)cGetTimeYYYYMMDDHHMMSS(), 2); /* MM */
        memcpy(sysdate+4, (char*)cGetTimeYYYYMMDDHHMMSS()+2, 2); /* DD */
        sysdate[6]='\0';

        f_printf(stderr, "Entered BFL_Daily_Auth\n");
        memset(&product_master, ' ', sizeof(product_master));
        memcpy(product_master.product_id, txn_product, strlen(txn_product));

        if(isread(lnprod, &product_master, ISEQUAL)<0)
        {
                f_printf(stderr, "W:Error in reading the Product Master. [%s]. Error no[%d]\n", txn_product,iserrno);
                return;
        }
        memcpy(offset, product_master.product_offset, 2);
        offset[2]='\0';



	memset(&remi_pl_rules, ' ', sizeof(remi_pl_rules));
	memcpy(remi_pl_rules.pl_tag, bflzbcom.lbcom_pl_tag, 5);
	memcpy(remi_pl_rules.product, txn_product, strlen(txn_product));

	
	f_printf(stderr, "pl_tag [%.5s] & product [%.*s]\n",bflzbcom.lbcom_pl_tag,strlen(txn_product), txn_product);

	if(isread(lnrules, &remi_pl_rules, ISEQUAL)<0)
	{
		f_printf(stderr, "No setup found for Product Code [%s] and Customer Risk [%.3s]\n",
		buisness_prod_code, bflzbcom.lbcom_cust_risk_clasif);
		return;
	}
	f_printf(stderr, "Rules Master read in onus_04.c\n");

	memcpy(prod_cap_limit, remi_pl_rules.limit, 12);
	prod_cap_limit[12]='\0';
	iprod_cap_limit = atoi(prod_cap_limit);
	f_printf(stderr, "Product Cap Limit:        [%s]/[%d]\n", prod_cap_limit, iprod_cap_limit);

	 memset(cust_open,0x00,sizeof(cust_open));
	 sprintf(cust_open,"%s/LN-CUST",getenv("MultipacDat"));

	 lnamnt = isopen(cust_open,ISINOUT+ISMANULOCK);
	 if(lnamnt < 0 )
	 {
		 f_printf(stderr," LN-CUST ERROR File [%s] Open Error [%d]",cust_open,iserrno);
		 isclose(lnamnt);
		 return;
	 }


        memset(&cust_prod_amount, ' ', sizeof(cust_prod_amount));
        memcpy(cust_prod_amount.customerid, bflzbcom.lbcom_customer_id, 19);

        if(isread(lnamnt, &cust_prod_amount, ISEQUAL)==0)
        {
                /*memcpy(today_amount, cust_prod_amount.product_amount[atoi(offset)].total_amount, 12);*/
                
		GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,1,1);
		today_amount[12]='\0';
                itoday_amount = atoi(today_amount);
		
		/*
                f_printf(stderr, "Today's Amount:           [%.12s]/[%d]\n",
                cust_prod_amount.product_amount[atoi(offset)].total_amount, itoday_amount);
		*/
	
                itxn_emi_amount = atoi(txn_emi_amount);

                f_printf(stderr, "Transaction's EMI Amount: [%.12s]/[%d]\n",
                txn_emi_amount, itxn_emi_amount);

                itoday_amount -= itxn_emi_amount;
		if(itoday_amount<0)
			memcpy(today_amount, "000000000000", 12);
		else
			sprintf(today_amount, "%012d", itoday_amount);
        		
		GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,2,1);	
	        /* memcpy(cust_prod_amount.product_amount[atoi(offset)].total_amount, today_amount, 12); */
		
		/*
                f_printf(stderr, "Today's updated Amount:   [%.12s]/[%d]\n",
                cust_prod_amount.product_amount[atoi(offset)].total_amount, itoday_amount);
		*/
		
		/*if('Y' == cust_prod_amount.product_amount[atoi(offset)].sms_flag[0])*/
	
		GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,1,2);
		
		if(today_amount[0]== 'Y')
		{
		fprintf(stderr,"Checking Prod amnt in reverse \n");
			if(itoday_amount < iprod_cap_limit)
			{
				memset(today_amount,' ',12);
				GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,2,2);

				/*cust_prod_amount.product_amount[atoi(offset)].sms_flag[0]=' ';*/
				f_printf(stderr, "Transaction amount unbreached. Do not send EOD block SMS.\n");
			}
			else
			{
				f_printf(stderr, "Customer still stands blocked for [%s]\n", txn_product);
			}
		}


                if(isrewrite(lnamnt, &cust_prod_amount)<0)
                {
                        f_printf(stderr, "Error in rewriting transaction Amount. ");
                        f_printf(stderr, "Customer: [%.12s]\n", cust_prod_amount.customerid);
                }
        }
	else
	{
		f_printf(stderr, "Error in reading LN-CUST. Cust: [%.*s], Errno: [%d]\n", 
			sizeof(cust_prod_amount), &cust_prod_amount, iserrno);
	}
	 isunlock( lnamnt );
	 isrelease( lnamnt );
	
	isclose(lnamnt);

}
#endif


int BFL_Update0400 ( t_entete_iso* IsoHeader )
{
        int             CardResponse = ECHEC;
        int                     AccountResponse = ECHEC;
        int                     Result = -1;

        FPRINTF ( stderr, "\n BFL_Update0400 () \n" );

        CODE_SCENARIO_O = SCENARIO_REFUS;
        CODE_SCENARIO_A = SCENARIO_BALE;
        switch ( CODE_SCENARIO_A )
        {
        case SCENARIO_BALE:     /* ACCOUNT UPDATION */
                AccountResponse = BFL_Update0400Account ( IsoHeader );
                if ( AccountResponse == SUCCESS )
                        CODE_SCENARIO_O = SCENARIO_BALE;
                Result = AccountResponse;
                BFL_Update0400OpenEcsEmi( );
                break;

        default:
                FPRINTF ( stderr, " SCENARIO_A NOT DEFINED: |%c|  \n", CODE_SCENARIO_A );
                Result = ECHEC;
                break;
        }

	/* MRL_300 */
		f_printf(stderr, "Current Reference Number -: [%.12s]\n",paczisoauto.retrieval_reference_number);
		f_printf(stderr, "Reference Number [%.12s] for Card[%.19s]\n",bflzport.lport_new_rrn,bflzport.lport_card_no);
	if(memcmp(bflcarddet.lcrd_new_rrn, paczisoauto.retrieval_reference_number, 12)==0)
	{
		f_printf(stderr, "REVERSAL of First Sale transaction.\n");
		memcpy(bflcarddet.lcrd_new_rrn, "            ", 12);
		memcpy(bflcarddet.lcrd_last_auth_dt,"      ",6);
		fprintf(stderr,"\"%s\",Function \"%s\"  \n",__FILE__, __FUNCTION__);
		UpdateLNCrdDet();
	}

        isunlock(lnporte);
        isunlock(lnbcom);
        isunlock(lnsauto);
        isunlock(lnhauto);
        return ( Result );
}
#endif


/* MRL_382 MRL_401 */
void BFL_GET_REFUND_DAYS(char *product_code,char *refund_days)
{
	char refund_days_06[4];
	memset(&bflzlpa06,' ',sizeof(t_bflzlpa06));
	memcpy(bflzlpa06.lpa06_table_cd, "06", 2);
	memcpy(bflzlpa06.lpa06_bank_code, "00059" , 5);
	memcpy(bflzlpa06.lpa06_product_id, product_code, strlen(product_code));
	f_printf(stderr,"Inside BFL_GET_REFUND_DAYS \n");
	if(isread(lnparam, (char *) &bflzlpa06, ISEQUAL ) == 0)
	{
		memcpy(refund_days_06,bflzlpa06.lpa06_refund_days,3);
		refund_days_06[3]='\0';

		f_printf(stderr,"Refund days in param [%d]",atoi(refund_days_06));
		if (atoi(refund_days_06) > 0)
		{
			memcpy(refund_days,refund_days_06,3);
			refund_days[3]='\0';
			f_printf(stderr, "BFL REFUND DAYS 06 [%.3s]\n",bflzlpa06.lpa06_refund_days);
			f_printf(stderr, "BFL REFUND DAYS has been overwritten by Master\n");
		}
		isrelease ( lnparam );
		isunlock  ( lnparam );
	}
	else
	{
		f_printf(stderr, "\n BFL LOAN PARAMETERS bflzlpa06 - Record read Error [%d]", iserrno);
	}
}
