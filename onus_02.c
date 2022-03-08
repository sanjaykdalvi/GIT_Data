
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_02.c                                           *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 16-Jun-2000                                         *
 * Description         : Processes Auth messages (0200,1200 etc)             *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   24-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   12-Apr-04 A K Azhakesh         CR-MXP-0401-0042                       *
 * 3   17-May-06 Veeresh              BR-MXP-0605-0019                       *
 *                                                                           *
 *****************************************************************************/

/*  Chowdesha added for CBA */
 #define   CBA   1  

/* #define QR_ISSUANCE  1 */


/* Added for CBA Amount issue 23.02.2012*/
#define DR_CBA_AMT 1

/* Added  by Chowdesha/Nagesh Patil on 04-01-2011 to fix the J&K ATM withdrawal transaction amount issue */
#define JK_TRNS_AMOUNT_ISSUE  1 

/* Ramya added on 09-10-2012 for VIJAYA BANK GIFT CARD */
  #define VIJAYA_GIFT_CARD  1   

 #define RUPAY_ISSUANCE   1    /* Added by Amit for RUPAY Issuance on 23 Nov 2016*/

 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>

 #include <signal.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <memory.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <isam.h>
 #include <iso_aegn.h>
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
 #include <paczautr.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <ath-trc.h>
 #include <define.h>
 #include <onus.h>
 #include <constantes.h>
 #include <iso_visa.h>
 #include <iso_mci.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <scenario.h>

#ifdef CBA
	#include <bankacnt.h>
	#include <cardbacc.h>
	#include <pacztrnslimit.h>
	#include <safparam02.h>
	extern 	t_bankacnt bankacnt;  
#endif 

#define BOI_FRM   1
#define SFLP_FRM 1
#define CARD_CONTROL     1


#define GIFT_CARD   1
#define SYNDICATE_PIN_RESET 1

/* preema added for enabling ACT flag  for multibank 20072011 */
#define MULTI_ACT  1    

/* Darshan added for card level limit checking for Shell phase2*/
  #define SHELL_PHASE2  1  

 /*Added by darshan on 29.08.2012 for Visa Mandate changes of Oct 2012  */
 #define VISA_MCW_OCT2012     1

/* Added for decline report rectification requested by J&K bank */
 #define SUBRESULT_CODE_NEW     1

/*
 #define ACT_CORP  1  
 #define ACT_UBI   1
*/

#ifdef GIFT_CARD 
	extern double	FltGiftFeesAmount; 
#endif 
#ifdef CARD_CONTROL
extern char card_con_res[];
#endif

#ifdef SFLP_FRM
#include <onuspr05.h>
extern t_onuspr05  onuspr05;
#endif

extern char			ServiceCode[3+1];
extern char			pvv[5+1];
extern char			cvv[3+1];
extern char			PinFormat[3];
extern char			EffectiveDate[5];
extern char			CardSecurity[6];
extern char			LanguageCode[4];
extern char			User[7];
extern char 		LOCAL_AMOUNT[13];
extern char			LocalAmount[13];
char				TypeOfCard[5];
char				track_type[2];
char act_flg[1];
char				CODE_BANQUE[5]; /* Used in FPRINTF function */

t_paczisoauto	paczisoauto;	/* Authorization(ISO data fields) Structure */
t_paczpa05		paczpa05;		/* Card Issuer details structure */
t_paczpoau		paczpoau;		/* Card holder details structure */
t_pcrzbale		pcrzbale;		/* Account deatils structure */
t_paczpa02		paczpa02;		/* Bank Details structure */
t_rthauth		rthauth;		/* RTH Authorization details */
t_paczau00		paczau00;		/* Authorization Details */
t_ath_trc		athtrc;			/* Authorization trace Details */

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;	/* DISPATCH PID */

extern 	char	CODE_SCENARIO_O;
extern 	char	CODE_SCENARIO_I; /* BR-MXP-0605-0019 */

/* ISAM Data File Descriptors */
extern	int		fdparam;	/* Parameter file */
extern 	int		fdseuro;	/* Euro Authorization movement file */
extern	int		fdtrans;	/* Transaction file */
extern	int		fdnf00;		/* Common Negative file */

/* Function prototypes */
void Request_0200 ( t_struct_iso* );
char* ProcessRequest_0200 ( t_struct_iso* );
extern int VerifyCVV ( char* SubResult, int MessageType );
extern int VerifyPVV ( char * SubResult );


extern void FormatRRNOForMCI ( char* user, t_paczisoauto* ); 
extern void DumpMessage ( t_struct_iso* Request, char Format );
extern int get_autorisation ( char* AuthId, int fdparam );
extern int InsertFieldVisa ( int field, char* text, char* data, int* len );
extern int InsertFieldMci ( int field, char* text, char* data, int* len );
extern double atof ( char* );
extern int  DumpAuthTrace ( t_ath_trc *athtrc );
extern int DumpISOAuto ( t_paczisoauto* isoautoRec );
extern int FillAuto ( char* ISOData, char Format, t_paczisoauto* AuthRecord );
extern int  DumpRthAuthRecord(t_rthauth   *rthauth);
extern int GetIssuerDetails ( t_paczpa05* pa05, char* CardNumber );
extern int GetBankDetails ( char* BankCode, t_paczpa02* pa02 );
extern int GetCurrency ( char* TxnCurrency, char* Amount, char* CardholderCurrency );
extern int CardUsageCheck ( char* ISOData, char Format, char* Result );
extern int CheckAccountType ( char* Result );
extern int CheckCIFBlockCodes ( char* SubResult );
extern int CheckNegative ( char* SubResult );
extern int GetCardType ( char* CardNumber, char* BankCode, char* CardType );
extern int OnusToDispatch_0110 ( char* Resp,char* SubResult, \
 t_struct_iso* ISOData );
extern int MCCLimitCheck ( char* SubResult );
extern int MCCProductBranchLimitCheck ( char* SubResult );
extern void LoadAuthTrace ( t_struct_iso* ISOStruct );
extern int GetErrorCodeUsingReferalCode ( char ReferalCode, char* SubResult );
extern int	GetActualResponseCode ( char* Bank, char* ProductCode, char* SubResult, char* ActualCode );
extern int CheckKeyEnterExpiry ( char* ExpiryDate, char* SubResult );
extern int GetApprovalCode ( char* ApprovalCode, char* SubResult, t_struct_iso* TxnData );
extern void LoadRthAuthRecord( t_struct_iso* ISOStruct );

#ifdef QR_ISSUANCE
extern int OnusToDispatch_0110_280022 ( char* Resp,char* SubResult, \
 t_struct_iso* ISOData );
#endif

#ifdef CBA
	extern  int iGet_TransLimit(char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, t_pacztrnslimit  *ptrpacztrnslimit);
	extern  int iCheck_TransactionLimit(char *ptrtrnsamt, char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, int istatus);
	extern  int get_account_type( char message_format, char *processing_code, int *account_type ) ;
	extern enum eAccountType 
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
	extern    double gcba_fees_amt; 
	extern char Cbs_auth_code[ 8 ]; 
   	extern char strcbs_bal_amt[ 40 ]; 
    extern char CBS_LOCAL_AMT [15]; 
    extern char CBS_CURR_CODE[5]; 
	extern  t_pacztrnslimit gpacztrnslimit; 
	extern int gFreeTxn; 
#endif 

#ifdef SHELL_PHASE2
        extern iCheckCardLevelLimit();
        extern int Shell_Pin_Act ;
#endif


#ifdef SUBRESULT_CODE_NEW
    extern char *Expiry_Decline_code_return();
#endif
