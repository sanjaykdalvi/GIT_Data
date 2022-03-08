
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_01.c                                           *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 14-Feb-2000                                         *
 * Description         : Processes Auth messages (0100,1100 etc)             *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer Description                             *
 *---------------------------------------------------------------------------*
 * 1   14-Apr-00 Gilu Alex           Modified to update STIP count and       *
 *                                   limits for MTI 0120                     *
 * 2   24-Feb-03 Shadakappa S        Project CINNAMON : re-structured ONUS   *
 * 3   01-Oct-03 Bhargav/Saju        FLD54 is disabled for Syndicate bank as *
 *                                   the bank is opted for Balance Enquiry   *
 * 4   03-Dec-03 Saju Joseph         BR-MXP-0312-0082                        *
 * 5   08-Dec-03 Saju Joseph         BR-MXP-0312-0090                        *
 * 6   03-Mar-04 Saju Joseph         BRAVO - Track data handling             *
 * 7   12-Apr-04 Azhakesh A K        CR-MXP-0401-0042                        *
 *               Saju Joseph         Reviewed                                *
 * 8  CVC2 Extraction&Verification : Reviewed by Saju Joseph : 10052004      *
 * 9   16-Nov-04  Veeresh                                                    *
 * 10  17-Dec-04  Veeresh          FLD54 is disabled for BOI & UBI Visa Cards*
 * 11  03-Feb-05  Veeresh         Update0100 function not called for Pin Change*
 *                                req.( Proc code 05) -BR-MXP-0502-0002      * 
 * 12  22-Aug-05  Veeresh         Canara Bank CVV2 verification              *
 * 13   17-May-06 Veeresh              BR-MXP-0605-0019                       *
 * 14   29-May-06 Veeresh              BR-MXP-0605-0033                       *
 * 15   30-May-06 Avinash Rao          BR-MXP-0605-0033 Reviewed              *
 * 16   01-June-06 Avinash Rao         BR-MXP-0605-0019 Reviewed              *
 * 16   01-June-06 Avinash Rao         BR-MXP-0605-0014 Reviewed              *
 * 17   04-July-06 Sheela N A		BR-MXP-0606-0020, BR-MXP-0606-0019    *
 *****************************************************************************/

/* Added on 06-06-2014 for IDBI International Credit card International usage handling at card level */ 
/* #define IDBI_INTL_USAGE		1 */

/* Chowdesha added on 28-Jan-2013 for emv early issuance  */
 #define EMV_EARLY_ISSUANCE  1 

/* Added on 07.11.2012 for Muthoot Card Closure with Refund */
  #define MUTHOOT_CARD_CLOSURE     1 

/* Chowdesha added on 02-Oct-2011 for Fees calculation */
 #define PREPAID_CARD_FEES  1   

/* Added for ING Vysya Bank Issuance on 29.05.2013 */
 #define ING_VYSYA_ISSUANCE 1  

/* NAGESH BASWANTHARAO ADDED FOR BOI_PREPAID_TRAVEL */
#define BOI_PREPAID_TRAVEL 1

  #define BOI_TRAVEL_BALANCE 1  
  #define SHELL_B2B_ACT 1

#define EDEN_ACT   1

#define SHELL_NEFT 1

/* #define  QR_ISSUANCE  1 */

#define MRL_383	1

#define SHELL_RELOAD_VOID_IS 1

/* NAGESH BASWANTHARAO ADDED FOR MUTHOOT_ISS_ACQ2 */ /*12th Feb 2012 */
    #define MUTHOOT_ISS_ACQ2   1      

/* Added for RBI Mandates */
 #define RBI_CAVV_MANDATE 1 

/* Added for UBI Platinum Card Issuance on 26.07.12 */
#define  UBI_PLATINUM_CARD 1

/* Ramya added on 09-10-2012 for VIJAYA BANK GIFT CARD */
  #define VIJAYA_GIFT_CARD  1 

/* Bharath added for J&K CMS APPLY */
   #define JK_CMS_APPLY   1   
/* Bharath added for RETAIL_CUG */
    #define RETAIL_CUG  1 

/* Bharath added for BOI PAHSE II changes */
/* #define BOI_PHASE2 1  */
/*  Chowdesha added for NRI/NRO/NRE checking */
  #define NRI_NEW  1

/* Added to decline International CNP Transactions same as Domestic Transaction validation 25.10.2012*/
  #define CNP_INTL 1 

/* Chowdesha added for HSBC DOP POS */
/* #define HSBC_DOP_POS  1 */

/* Chowdesha added on 26-05-2011 for HSBC DOP Phase 2*/
/* #define HSBC_DOP_PHASE2  1 */

#define HSBC_DOP_LIMIT 1

/* Bharath added for DR  CBA local amount issue 17.02.12 */
#define DR_CBA_AMT 1

 /*  Chowdesha added for CBA  */
 #define  CBA   1 

 #define LODHA_DB_26SEPT 1

 #define REMOVE_DB 1

 #ifdef LODHA_DB_26SEPT
 #include <string.h>
 extern char *rtrim(char* string);
 #endif


/* MRL_332 */
extern int pos_sms_applicable;


#define BFL_TIMEOUT_ISSUE 1

#ifdef BFL_TIMEOUT_ISSUE
	extern int ibfl_txn_threshold;
#endif

#define MRL_204 1
#define MRL_207 1

#define BFL_REMI_TO_PL

#ifdef BFL_REMI_TO_PL

        void BFL_Daily_Auth();

        #include <remi_pl_rules.h>
        #include <loan_product_master.h>
        #include <cust_prod_amount.h>

        extern int lnprod;
        extern int lnrules;
       /* extern int lnamnt;*/

        extern int remi_to_pl_sms;

        extern char remi_to_pl_breach_amount[];
	extern t_cust_prod_amount cust_prod_amount;

	char VL_Rule_Done [100][7];
	int  VL_Count;
#endif


	char 				CARD_NO[17]={0};
	char 				RRN_NO[13]={0};

/* Chowdesha/Preema added for IDBI BCHA EHOST */
 #define IDBI_BCHA_EHOST  1  
 #define BOI_FRM          1
 #define CARD_CONTROL     1
 #define BFL_CUG_CARD     1
 #define ADDON            1  
 #define BOBL_PREPAID     1
 #define BFL_PHASE2       1
 #define RUPAY_ISSUANCE   1   /* Added by Amit for RUPAY Issuance on 4 july 2016*/ 
 #define SHELL_RELOAD_ISSUE 1   /* Added by Amit on 17 Mar 2017 for SHELL Reload second leg card number differ Issue */
 #define VISA_APR_2017_MANDATE 1  /* Added by Amit for Visa Apr 2017 Mandates */
 #define EDENRED_MOBILE_TXN 1  /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */

/* Chowdesha added for IVR on 30-Dec-2010 */
 #define IVR_TRNS  1  

/* Bharath added for UBI  Changes*/
#define  UBI_BIN_BLOCK  1

#define IVR_TRNS_FIX 1

/*  Chowdesha added on 09-March-2012 for Shell phase 2 development */
  #define SHELL_PHASE2  1  

/*  Added on 21-07-2011 for IVR transaction with merchant name validation */
#define IVR_MER_NAME  1 

 #define  MCI_SIRP 1    /* Nagesh for MCI SIRP test transaction mandate */

 #define  MCI_ASIS 1    /* Nagesh for MCI ASIS  transaction mandate */
 #define  SELECT_CITY     1
 #define  PHOENIX_MALL    1 

 #define DR_ADVICE 1

/*   Chowdesha added for IDBI DOP */
#define IDBI_DOP  1

#define HSBC 1  
#define CARD_RENEWAL_ACT 1  

#define DEBUG_1 1
 #define SYND_TRNS_AMOUNT_ISSUE  1

  #define SHELL_PHASE1   1   /* card sale trxn */

#define	VISA_MANDATE_2012	1 	/* Added by darshan on 09.03.2012 for Visa Mandate changes */

#define SHELL_IPG_TXN_FEE_CAL	1	/* Added by darshan on 26.07.2012 for Shell card to calculate fees for IPG Txns */

/* Added to Implement Reload Slab for SHELL on 13.09.2012 */
  #define SHELL_RELOAD_SLAB       1 
/*#define SHELL_SALE 1 */ 

/* Preema added on 30-05-2013 for Master card EMV issuance */
#define MCI_EMV_ISS  1
#define EMV_VISA_PIN 1

#define MVISA_ISSUER 1

#define EDENRED_NEW 1
#define EDEN_MCC_CHECK 1 

#define VIJAYA_TRAVEL 1
#define INT_TRANS_LIMIT 1

 #define CNP_3ATTEMPT  1 
 #define VISA_MCI_CNP_PARAMETER 1 
 #define ING_VYSYA_DR 1

 #define LODHA_PALAV 1 
 #define KOTAK_UPDATION 1
 #define LODHABUS  1 
 #define DECLINE_SMS 1 
 #define MCI_MONEY_SEND 1 

 #define BOI_CRDIT_NEWBIN 1

 #define SFLP_FRM 1

 #define BOI_GPPC  1

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
 #include <struct_iso.h>
 #include <iso_aegn.h>
 #include <paczpa02.h>
 #include <paczpa03.h>
 #include <paczpa05.h>
 #include <paczpaem.h>  /*  for card issuer */
 #include <paczpa56.h>
 #include <paczpa11.h>
 #include <paczpoau.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczauto.h>
 #include <paczisoauto.h>
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
 #include <saftrns.h>
 #include "onus_01.h"     /* Added by Pradnya */
 #include <paczmlist.h>
 #include <onuspr05.h>
 #include <posparam03.h>
 #include <emvparameter.h>

 #ifdef BFL_CUG_CARD
 #include <bflzbcom.h>
 #include <bflzport.h>
 #include <bflzbcli.h>
 #include <bflzparm.h>
 #include <bfl_rthauth.h>
 #include <BFLPROC.h>
 #include <bfl_dealer_master.h>
 #include <bflzaddon.h>
 #include <bflauth00.h>

 extern t_bflzport    bflzport;
 extern t_bflzbcom    bflzbcom;
 extern t_bflzbcli    bflzbcli;
 extern t_bflrthauth  bflrthauth;
 extern t_procdata    procdata;
 extern t_bfl_dealer_master bfl_dealer_master;
 extern t_lnaddport   lnaddport;


 extern int lnbcli ; /*BFL CARD Client file */
 extern int lnbcom ; /*BFL CARD ACCOUNT MASTER */
 extern int lnporte ;/*BFL CARD MASTER */
 extern int lnsauto; /*BFL AUTHORISATION */
 extern int lnhauto; /*BFL AUTHORISATION HISTORY */
 extern int lnparam; /*BFL PARAMETERS*/
 extern int lnrth;   /*BFL RTH*/
 extern int taglenvalue(char *,int);
 extern int AddAdditionalTags(char *,int );
 extern char Bfl_Card_Type[];
 extern int BFL_GetCardType( char* CardNumber, char* BankCode, char* CardType );
 extern int BFL_Update0100Account ( char *  );
 extern int BFL_UpdateOpenEcsEmi( char *  );
 extern int Update0100RedeemPoints( char *  );
 extern int BFL_UpdatePin_Count( char *  );
 extern int Open_Dealer_Master(char * );
 char       Original_Cell_Number[17];
 char       Original_Amount[13];
 #ifdef BFL_PHASE2
 char       Prefered_Limit[13];
 int        Ecs_Waive_Off;
 int        Velocity_Check;
 int        Same_Merchant;
 int        NXTALLD;
 #endif
 int BFL_OnusToDispatch_0110 ( char* Resp, char* SubResult, t_struct_iso* ISOData );
 char *BFL_ProcessRequest_0100 ( t_struct_iso *Request , t_entete_iso     ISOHeader);
 int Mobile_No_Tranx ;
 extern int Card_First_Trans;
 extern int chargeslip_first_fee;
 #ifdef MRL_383
 	extern int rset_card_1st_fee;
 #endif
 extern int Card_Embo_Date_Checking();
 extern int UpdateLoanVelocLimit_ltv();
 extern int Validate_Velocity_Rules(char *);
 extern int ReadAddonCard();
 extern t_bflauth00   bflauth00; 
 #endif

#define RULE_ENGINE 1

#ifdef RULE_ENGINE
	#include "ruleEngineMacros.h"
	#include "readAndPopulateRule.h"
	#include "ruleFldDtls.h"
	#include <paczpa1b.h>
	extern t_paczpa1b paczpa1b;

	char   Current_Cibil_Score[7];
	char   Cibil_Score_Min[6];
	char   New_Cibil_Score[6];
	char   New_Cibil_Score_Date[9];
	char   failedRule[7];
	int    ruleStatus = -2;

	extern t_bflzlpa03   bflzlpa03;
	extern t_fldDtls ruleFldDtls[]; /* ARRAY OF FIELD DETAILS IN RULE ENGINE */
	extern t_fldDtls cibilFldDtls[];

	extern t_ruleTypeRuleMaster * authRuleMaster[];
	extern t_ruleTypeRuleMaster * cibilRuleMaster[];
	extern t_ruleTypeRuleMaster * ltvRuleMaster[];
	extern t_ruleTypeRuleMaster * velocityRuleMaster[];

	extern int  authRuleCount;
	extern int  cibilRuleCount;
	extern int  ltvRuleCount;
	extern int  velocityRuleCount;

        char ltv_cumul_auth[12+1];
        char ltv_cumul_ecs[12];
        char ltv_customer_id[19+1];

	extern int runRuleEngine( char *ruleType,t_fldDtls ruleFldDtls[],
				t_ruleTypeRuleMaster *ruleTypeRuleMaster[], 
					char *failedRuleId, char* SubResult);
	extern int BFL_Updatelncust ( char *  );
#endif
 
#ifdef CBA
	#include <bankacnt.h>

	#include <cardbacc.h>
	#include <pacztrnslimit.h>
	#include <paczcardlimit.h>
	#include <safparam02.h>

	t_bankacnt bankacnt;  

#endif 
#ifdef LODHABUS 
#include <card_hotlist.h>
#endif
extern t_onuspr05  onuspr05;
extern int fdonusparam;

#ifdef LODHA_PALAV
extern int ReadFromBusPassMaster(char *);
extern int Check_Bank_Level_Reload_Limit();
extern int Update_Bank_Level_Reload_Limit(int);
int ReadMasterFlag ;
char Lodha_Balance[29];
#endif



#ifdef MUTHOOT_CARD_CLOSURE
extern char szFld_54_Muthoot[];
#endif

   #ifdef ING_VYSYA_DR
   int fld48_len = 0;
   int itvr_len = 0;
   char mci_fld48data[500];
   #endif

#ifdef SHELL_PHASE1
extern char fld_54_shell[ ];
int Card_Activation_Flag = 0;
#include <paczport.h>
int Shell_Pin_Act;
#endif

 #define TEST_CARD    1   

#define  EDEN_ACT_DR  1

#ifdef EDEN_ACT_DR
int active_stage = 0;
#endif


/* Added by chowdesha for ANDHRA BANK VBV */
 #define AND_VBV 
 #define JK_BANK_CHANGES 1

/* preema added for enabling ACT flag  for multibank 20072011 */
 #define MULTI_ACT  1    

 #define SYNDICATE_VBV  1  
 #define VBV_NEW 1

/* Chowdesha added for UBI VBV */
 #define  UBI_VBV   

/* Bharath added for muthoot amt limit 31.08.10*/
 #define MUTHOOT_LIMIT 1

#define MUTHOOTGOLD
#define CSL_PROJECT

 #define SBI_L 1 

 #define M_SECURE 1 
 #define JKBANK   1 

/* #define ACT_CORP  1   */

/* Chowdesha added for VISA MANDATES  */
#include <paczprea.h>
/* #define VISAMANDATE   1 */  

/* Chowdesha added  for RPI transaction Expiry date should not validate */ 
#define RPIEXPVAL     1

#define VIJAYA_PHASE_3 1

/*  Chowdesha added for the PIN RESET transaction for the IGL */
 #define CMS_EHOST  1   

/* Chowdesha added on 12-Apr-2010 for PREPAID CARD */
  #define PREPAID_CARD  1    
  #define PREPAID_CARD_CURR 1  
  #define STIP_PARAM  1    

/* Chowdesha added for GIFT cards */
#define GIFT_CARD   1  

 #define CVC2_VER  1  

 /*Added by darshan on 29.08.2012 for Visa Mandate changes of Oct 2012  */
 #define VISA_MCW_OCT2012     1

#ifdef SHELL_PHASE2 
   int gUpdate_mcc_card_veloc = 0;
   int iCheckCardLevelLimit();
#endif

#ifdef GIFT_CARD 
	extern double	FltGiftFeesAmount; 
#endif 

#ifdef DR_ADVICE 
extern t_paczauto paczauto;
#endif

#ifdef CMS_EHOST
	int OnusToDispatch_0610 ( char* Response, char* SubResult, t_struct_iso* TxnData ); 
	int iGetBonus_Points( char *Cardnumber, char *Bonus_Points); 
#endif 

#ifdef CBA
	t_bankacnt bankacnt;  
	t_pacztrnslimit gpacztrnslimit; 
	t_paczcardlimit  paczcardlimit; 
	extern char CBS_LOCAL_AMT [15]; 
	extern char CBS_CURR_CODE[5]; 
	int iGet_TransLimit(char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, t_pacztrnslimit  *ptrpacztrnslimit);
	int iCheck_TransactionLimit(char *ptrtrnsamt, char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, int istatus);
	int Insert_Replace_BankAccNum(t_struct_iso *Request, t_bankacnt bankacnt, int *ilen);
	extern int get_account_type( char message_format, char *processing_code, int *account_type ) ; 
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
int gFreeTxn = 0; 
double gcba_fees_amt = 0.0; 
char gstr_feesdet[250]; 
char strcbs_bal_amt[40]; 
#endif 

#ifdef SHELL_PHASE1/*HSBC_DOP_POS*/
double gcba_service_tax = 0.0; 
 
#endif
#ifdef BOI_FRM
char       Orginal_RRN[13];
#endif

#ifdef EMV_EARLY_ISSUANCE
	extern t_emvparameter emvparameter; 
#endif

#ifdef EDENRED_NEW
        t_acqbankid acqbankid;
#endif
#ifdef EDEN_MCC_CHECK
        extern int WHITELIST_MATCHED;
        extern int MCC_MATCHED;
#endif


#ifdef SHELL_RELOAD_VOID_IS
extern int iReloadPresentFile(char *reload_card, char * reload_rrn);
extern int iReloadUpdateFile(char *reload_card, char * reload_rrn);
#endif

#ifdef CARD_CONTROL
char   visa_mci_data[18];
char   card_con_res[3];
#endif


#ifdef IDBI_BCHA_EHOST
  extern char Cbs_auth_code[8]; 
#endif 
extern char			ServiceCode[3+1];
extern char			pvv[5+1];
extern char			cvv[3+1];
extern char			cvc2[3+1];
extern char			PinFormat[3];
extern char			EffectiveDate[5];
extern char			CardSecurity[6];
extern char			LanguageCode[4];
extern char			User[7];

extern char 		LOCAL_AMOUNT[];
extern char			LocalAmount[13];
char				TypeOfCard[5];
char                            track_type[2];

int                             velocity_flag ;

char card_no[17];
char act_flg[1];

/* MRL_359: Made the below field global */
char          			field_59data[200];


char				CODE_BANQUE[5]; /* Used in fprintf function */

t_paczisoauto	paczisoauto;	/* Authorization(ISO data fields) Structure */
t_paczpa05		paczpa05;		/* Card Issuer details structure */
#ifdef STIP_PARAM
/* #include<paczpaem.h> */
t_paczpaem		paczpaem;		/* Card Issuer details structure - II */
#endif

#ifdef MUTHOOT_LIMIT 
#include<paczpaod.h>
t_paczpaod              paczpaod; 
#endif

extern t_paczpoau	paczpoau;		/* Card holder details structure */
extern t_pcrzbale		pcrzbale;		/* Account deatils structure */
extern t_paczpa02	paczpa02;		/* Bank Details structure */
extern t_rthauth		rthauth;		/* RTH Authorization details */
extern t_paczau00	paczau00;		/* Authorization Details */
extern t_ath_trc		athtrc;			/* Authorization trace Details */
extern t_paczpoau	dstpaczpoau;	/* Destination card details */

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;	/* DISPATCH PID */

extern 	char	CODE_SCENARIO_O;
extern 	char	CODE_SCENARIO_I; /* BR-MXP-0605-0019 */

/* ISAM Data File Descriptors */
extern	int		fdparam;	/* Parameter file */
extern	int		fdtrans;	/* Transaction file */

extern t_paczport paczport;
#ifdef JK_CMS_APPLY
	char *ProcessRequest_0100_06 ( t_struct_iso *Request , t_entete_iso	ISOHeader); 
    extern int iCheck_CardholderOnline( t_paczpoau* ,char* Result,int* Value);
	extern char * cGetTimeYYYYMMDDHHMMSS(void );
#endif


/* Function prototypes */
void Request_0100 ( t_struct_iso* );
char* ProcessRequest_0100 ( t_struct_iso* );
char* ProcessRequest_0120 ( t_struct_iso* );

extern void FormatRRNOForMCI ( char* user, t_paczisoauto* ); 
extern void DumpMessage ( t_struct_iso* Request, char Format );
extern int VerifyCVV ( char* SubResult, int MessageType );
extern int VerifyPVV ( char * SubResult );
extern int get_autorisation ( char* AuthId, int fdparam );
extern int InsertFieldVisa ( int field, char* text, char* data, int* len );
extern int InsertFieldMci ( int field, char* text, char* data, int* len );
extern int InsertFieldAegn ( int field, char* text, char* data, int* len );
extern int GetActualResponseCode ( char* Bank, char* ProductCode, char* SubResult, char* ActualCode );
extern int DumpAuthTrace ( t_ath_trc *athtrc );
extern int DumpISOAuto ( t_paczisoauto* isoautoRec );
extern int FillAuto ( char* ISOData, char Format, t_paczisoauto* AuthRecord );
extern int DumpRthAuthRecord(t_rthauth   *rthauth);
extern int GetIssuerDetails ( t_paczpa05* pa05, char* CardNumber );
extern int GetBankDetails ( char* BankCode, t_paczpa02* pa02 );
extern int GetCurrency ( char* TxnCurrency, char* Amount, char* CardholderCurrency );
extern int CardUsageCheck ( char* ISOData, char Format, char* Result );

extern int CheckAccountType ( char* Result );
extern int CheckCIFBlockCodes ( char* SubResult );
extern int CheckNegative ( char* SubResult );
extern int GetCardType ( char* CardNumber, char* BankCode, char* CardType );
extern int MCCLimitCheck ( char* SubResult );
extern int MCCProductBranchLimitCheck ( char* SubResult );
extern int WriteToSeuro ( t_entete_iso* IsoHeader, char* TxnType );
extern void LoadAuthTrace ( t_struct_iso* ISOStruct );
extern int CheckSuppressResult(char* Bank, char* ProductCode,char* SubResult);
extern int GetResponseMTI ( t_msg_bl* IntrfMsg );
extern int CheckKeyEnterExpiry ( char* ExpiryDate, char* SubResult );
extern int GetApprovalCode ( char* ApprovalCode, char* SubResult,t_struct_iso* TxnData ); 
extern void LoadRthAuthRecord( t_struct_iso* ISOStruct );
extern double atof ( char* );

#ifdef LODHA_PALAV
extern char *Card_Card_Limit(int);
#endif


#ifdef STIP_PARAM
extern	char	CODE_SCENARIO_A;
extern int iCheck_CardIssuer(char  *porteur);
#endif


int iVerifyCAVVData(char *SubResult, int *iVerCVV2Flag);
#ifdef VBV_NEW
int Get_Ecom_Ind(char *data, char status[] , char *mci_cntr_code);
#else
int Get_Ecom_Ind(char *data, char status[]);  /* preema for M_SECURE */  
#endif

#ifdef MCI_EMV_ISS
 int TVR_CVR_check_result(char *data, char *crpt_status, int fld48_len, int *itvr_len );
#endif

#ifdef IVR_TRNS 
	int iVerify_IVR_CAVVData(char *SubResult, int *iVerCVV2Flag); 
#endif

/* Chowdesha added on 02-Mar-2009 for VISAMANDATES */
void LoadPreatthTrace(char *preauthtimelt);


extern int GetTCCValue( char* , char* );
extern int GetTrack1CardNumber_Expiry( char*, char*, char *);
extern int GetTrack2CardNumber_Expiry( char*, char*, char *);
extern int iFindTxnType( char *proc_code );
extern char* ProcessRequest_0420 ( t_struct_iso* Request );

char TempCheck[5];                                           

extern int DoPBank(char *bank_code);

#ifdef PREPAID_CARD 
	extern double  FltFeesAmount;
	extern double	FltConvPercent; 
	extern double FltConvPercent_amt; 

	extern	int iGetFees(char *Card_nember, char *fees_amt1, int fees_field, char *atm_amtlimit);

#endif


extern char *Expiry_Decline_code_return();



#ifdef CNP_3ATTEMPT
extern int Check_CNP_Attempts(char );
#endif

#ifdef VISA_MCI_CNP_PARAMETER
extern int Card_Not_Present_Transaction_Check(char ,char *);
#endif

#ifdef VIJAYA_PHASE_3
int Ucaf ;
int decline_sms_balance;
#endif



#ifdef CNP_INTL
int iCheck_CNP_bank(char *BankCode)
{
	if (  ( memcmp( BankCode, "00032", 5 ) == 0 ) || 
	      /*( memcmp( BankCode, "00005", 5 ) == 0 ) || */ /* Added  VIJAYA Bank on 06.12.12 */
	       /* ( memcmp( BankCode, "00034", 5 ) == 0 ) ||  */  /* Commemted for UBI on 06.12.12 by Chowdesha */
	      /* ( memcmp( BankCode, "00035", 5 ) == 0 ) ||  */ /* Commemted for UBI on 06.12.12 by Bharath */
	       ( memcmp( BankCode, "00071", 5 ) == 0 ) || 
	       ( memcmp( BankCode, "00072", 5 ) == 0 ) || 
	      /*( memcmp( BankCode, "00006", 5 ) == 0 ) || */ /*commented for BOI Credit card on 08-05-2013*/
             #ifdef ING_VYSYA_ISSUANCE
	        ( memcmp( BankCode, "00079", 5 ) == 0 ) ||  /*Added VYSYA Bank  on 29-05-2013*/
             #endif
	       ( memcmp( BankCode, "00053", 5 ) == 0 )  ) 
	{
      return SUCCES;
	}

   return ECHEC;

}

#endif

#ifdef SHELL_IPG_TXN_FEE_CAL
 int igetReload_Limit(char *ptrcardnum, t_paczpaod *ptrpaczpaod)
{
		 int cc = 0; 
		 int iCount = 0;
		 int iErrNo = 0;
		memcpy(ptrpaczpaod->paod_notabl,     "OD",                     2);
		memcpy(ptrpaczpaod->paod_bin,ptrcardnum, 7);
		memcpy(ptrpaczpaod->paod_suitecle,  "                     ",  21);
		do
		{
			cc= isread(fdparam,(char *)ptrpaczpaod,ISEQUAL);
			 iErrNo = iserrno ;
			isrelease(fdparam);
			isunlock (fdparam); 
			 if(cc == 0)
				break; 
			iCount++;
		} while(( iCount < 5 )  && (iserrno == ELOCKED) );

	 if(cc < 0)
	 {
			f_printf(stderr, "\n Error reading the record iserrno [%s]", iErrNo);
			return (ECHEC);
	 }

	  if( (iCount >= 5) || (iErrNo == ELOCKED) )
	  {
			f_printf(stderr, "\n Error reading the record - Rec locked/Count reached iserrno [%s]", iErrNo);
			return (ECHEC);
	   }
	   return SUCCES; 
}
 #endif

void Request_0100 ( t_struct_iso* Request)
{

	int iRetval  = 0, iRet = 0;
	char	Result[6+1];
	char	ActualResponseCode[6+1], acc_num[30];
	char *IsoData; 


	memset ( Result, 0, 6+1 );
	memset ( ActualResponseCode, '0', 7 );



	/* Process the authorization request */

	memcpy ( Result, ProcessRequest_0100 ( Request ), 6 );

	f_printf(stderr, "ProcessRequest_0100 Return Value: [%.6s] \n", Result );


	/* Get the Actual response code */
	GetActualResponseCode ( Request->entete.banque, \
		paczpa05.pa05_product_code, Result, ActualResponseCode );


	f_printf(stderr, "Response Code [%.5s] SubResult [%.6s] \n", ActualResponseCode, Result );


	if(memcmp(Request->entete.banque, "00059", 5) == 0 )
	{
		BFL_OnusToDispatch_0110(ActualResponseCode, Result, Request );
	}

	f_printf(stderr, "\nActualResponseCode [%.6s]\n", ActualResponseCode); 

	f_printf(stderr, "Before LoadRthAuthRecord\n");

	LoadRthAuthRecord ( Request );

	f_printf(stderr, "\n After LoadRthAuthRecord\n");

	DumpRthAuthRecord ( &rthauth );


	Request->entete.type = 110 ; 

	LoadAuthTrace ( Request );
	DumpAuthTrace ( &athtrc );

	if ( memcmp ( ActualResponseCode, "RR", 2 ) != 0 )
	{
		/* Write the authorization details to sauto file */
		BFL_WriteToSauto ( &(Request->entete), ActualResponseCode );
	}

	BFLEmailSMS(Request);
	


	/*LN-CUST NEW CALL AFTER BFL_WriteToSauto*/
	f_printf(stderr,"CALLING LN-CUST write for NEW record");
	BFL_Updatelncust(ActualResponseCode);	
	BFL_Daily_Auth(ActualResponseCode);
	
	return;
}

char *ProcessRequest_0100 ( t_struct_iso* Request )
{

	int                     MessageType;    /* MTI */
        char                    Pos_Data_Info[30];
        char                    Pos_Data_Info_61[30];
        t_entete_iso            ISOHeader;              /* ISO HEADER */
        char*                   ISOData;                /* Bit MAP + DATA */
        static char             SubResult[6+1];
        char                    MessageFormat;



   	/* MTI */
	MessageType = Request->entete.type;

        memset(Pos_Data_Info, 0x00, sizeof(Pos_Data_Info));
        memset(Pos_Data_Info_61, 0x00, sizeof(Pos_Data_Info_61));



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

        f_printf( stderr,"\n User :[%.6s],DE3 :[%.6s],DE2[%.1s]"
		,ISOHeader.user,paczisoauto.processing_code,ISOHeader.porteur);


	/* Dump the ISO auto struct */
	DumpISOAuto ( &paczisoauto );


        /****************************************************************************/
        /*  BFL CMS New Function BFLProcessRequest_0100   */
        /****************************************************************************/

        #ifdef BFL_CUG_CARD 
        if(memcmp(Request->entete.banque, "00059", 5) == 0)
	{ 
	  f_printf(stderr,"In Request_100 Inside BFL\n");
	  memset(SubResult, 0x00, sizeof(SubResult));
	  memcpy(SubResult, (char *) BFL_ProcessRequest_0100( Request , ISOHeader), 6);
	  return SubResult; 
	}
	#endif

	/* return SUCCESS  */
	memcpy(SubResult, "000000", 6);
	return (SubResult);
}


#ifdef BFL_CUG_CARD
int BFL_Update0100 ( int iTxnType, char card_type,char * Response )
{

        if(memcmp( paczisoauto.processing_code, "99", 2 ) == 0)
        {
         	f_printf(stderr,"UPDATE REDEEM POINTS TO BCOM FILE\n");
         	Update0100RedeemPoints ( Response );; 
                return ( SUCCESS );
        }
        if((memcmp( paczisoauto.processing_code, "30", 2 ) == 0) || (memcmp( paczisoauto.processing_code, "21", 2 ) == 0))
        return ( SUCCESS );
    

        f_printf(stderr,"CODE_SCENARIO_O[%c]\n",CODE_SCENARIO_O);
        CODE_SCENARIO_O = SCENARIO_BALE ;
        switch ( CODE_SCENARIO_O )
        {
        case SCENARIO_POAU:
		f_printf(stderr, "BFL Incorrect Case statement. *ERROR*\n");
                break;

        case SCENARIO_BALE:
	/*	BFL_Updatelncust ( Response );*/ /*MRL_130*/
                BFL_Update0100Account ( Response );
                BFL_UpdateOpenEcsEmi  ( Response );
                break;

	default:
		f_printf(stderr, "ERROR: SCENARIO other than BALE.\n");
        }
        return ( SUCCESS );
}
#endif



#ifdef AND_VBV

/* Added by chowdesha To Validate the CAVV Result for AndhraBank VBV  */
int iVerifyCAVVData(char *SubResult, int *iVerCVV2Flag)
{
	char country_code[ 5];
	memset (country_code, 0x00, sizeof(country_code));
        memcpy(rthauth.ECIstatus,paczisoauto.pos_entry_capability_code+9,1);

        f_printf(stderr,"chowd: paczisoauto.pos_entry_capability_code :: %s\n",paczisoauto.pos_entry_capability_code);
        f_printf(stderr,"chowd1: paczisoauto.pos_entry_capability_code :: %x\n",paczisoauto.pos_entry_capability_code[0]);

	if(strlen(paczisoauto.acquiring_institution_country_code) != 0)
		memcpy(country_code, paczisoauto.acquiring_institution_country_code, 3); 
        else
	        memcpy(country_code, paczisoauto.currency_code_transaction, 3);
	
     if(memcmp (paczisoauto.pos_entry_capability_code+9,"5",1)==0)
      {
         f_printf(stderr,"chowd:paczisoauto.pos_entry_capability_code = 5\n");  
         memcpy(rthauth.CAVVstatus,&paczisoauto.additional_response_data[14],1);
         if (paczisoauto.additional_response_data[14] == '2')
         {
              #ifndef  CVC2_VER
  	      *iVerCVV2Flag = 0;/* 29112010 */
              #else 
  	      *iVerCVV2Flag = 1;
              FPRINTF(stderr,"\nCAVV Verification Passed......., CVV2 Not Verified\n");
			  /* Added for ER on 29112010  */
			   if (VerifyCVV2Data(SubResult) == SUCCESS)
		       {              
			      f_printf(stderr, "1.Need to verify CVV2 DATA ...\n" );
				  return SUCCESS;   
	           }		
			  /* Added for ER on 29112010  */
              #endif
              return SUCCESS;                                
         } 
         else  
         {
              f_printf(stderr,"\n INVALID CAVV RESULT VALUE .......\n");
                 memcpy(SubResult, "050327" , 6);
              return ECHEC;                           
         }
       }                                                           
     else if(memcmp (paczisoauto.pos_entry_capability_code+9,"6",1)==0)  
       {
		 /* Decline CNP cases transaction for Local as well as International Transactions for ECI Indicator '7'  */
	 #ifdef CNP_INTL
         if ( (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)
	    &&(iCheck_CNP_bank( paczpa05.pa05_codbnq ) == SUCCES))  
	 {
	 	 f_printf(stderr, "\n CNP -  IPG with ECI 06 transactions not allowed \n"); 
                 memcpy(SubResult, "050318" , 6);
                 return ECHEC; 
         }
	#endif
	
  #ifdef VBV_NEW
	 if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) && (memcmp ( country_code, "356", 3) == 0) )
	 {
		 f_printf(stderr,"\n ECOM - 6. Transaction not allowed for the transactions with ECI indicator 6.\n"); 
                 memcpy(SubResult, "050318" , 6);
                 return ECHEC; 
	 }
   #else
	 #ifdef SYNDICATE_VBV  
         if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) && (memcmp(paczpa05.pa05_codbnq,"00032",5) == 0))
	 {
                f_printf(stderr,"\n ECOM - 6. Transaction not allowed for Syndicate bank..\n"); 
                 memcpy(SubResult, "050319" , 6);
                 return ECHEC; 
         }
         #endif
  #endif


       
         f_printf(stderr,"chowd:paczisoauto.pos_entry_capability_code = 6\n");  
         memcpy(rthauth.CAVVstatus,&paczisoauto.additional_response_data[14],1);
 	 if ((paczisoauto.additional_response_data[14] == '3') ||    
             (paczisoauto.additional_response_data[14] == '8') ||
             (paczisoauto.additional_response_data[14] == 'A'))     
          {
		#ifndef  CVC2_VER
	         *iVerCVV2Flag = 0;/* 29112010 */
		#else
	        *iVerCVV2Flag = 1;
			/* Added for ER on 29112010  */
			   if (VerifyCVV2Data(SubResult) == SUCCESS)
		       {              
			      f_printf(stderr, "1.Need to verify CVV2 DATA ...\n" );
				  return SUCCESS;   
	           }		
			  /* Added for ER on 29112010  */
		#endif	
                return SUCCESS;                                  
          }                                     
          else if (VerifyCVV2Data(SubResult) == SUCCESS)
           {              
              f_printf(stderr, "1.Need to verify CVV2 DATA ...\n" );
              return SUCCESS;   
           }		
           return ECHEC;
        }                                            
       else if(memcmp (paczisoauto.pos_entry_capability_code+9,"7",1)==0)  
        { 

		 /* Decline CNP cases transaction for Local as well as International Transactions for ECI Indicator '7' */
		 #ifdef CNP_INTL
         if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)
				 &&    ( iCheck_CNP_bank( paczpa05.pa05_codbnq ) == SUCCES)  
			 )
		{
				f_printf(stderr, "\n CNP -  IPG txn with ECI 07 transactions not allowed \n"); 
                 memcpy(SubResult, "050318" , 6);
                    return ECHEC; 
         }
       #endif 

             /* SYNDICATE BANK CONDITION */ 
	   #ifdef VBV_NEW
	   if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) && (memcmp ( country_code, "356", 3) == 0) )
			 {
				  f_printf(stderr,"\n ECOM - 7. Transaction not allowed\n"); 
                      memcpy(SubResult, "050318" , 6);
                  return ECHEC; 
		   }
	#else
     #ifdef SYNDICATE_VBV  
             if ((memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) && (memcmp(paczpa05.pa05_codbnq,"00032",5) == 0))
              {
                  f_printf(stderr,"\n ECOM - 7. Transaction not allowed for Syndicate bank..\n"); 
                  memcpy(SubResult, "050319" , 6);
                  return ECHEC; 
              }
      #endif 
 	 #endif
	
          f_printf(stderr,"chowd11:paczisoauto.pos_entry_capability_code = 7\n");
            if (VerifyCVV2Data(SubResult) == SUCCESS)
             {                    
                 f_printf(stderr, "2.Need to verify CVV2 DATA ....\n" );
                 return SUCCESS;  
             }  
             return ECHEC;
         }
     return SUCCESS;

}
#endif

/* Added by chowdesha To Validate the IVR CAVV Result    */
#ifdef IVR_TRNS 
int iVerify_IVR_CAVVData(char *SubResult, int *iVerCVV2Flag)
{
     char country_code[5];

     memcpy(rthauth.ECIstatus,paczisoauto.pos_entry_capability_code+9,1);
     f_printf(stderr,"IVR paczisoauto.pos_entry_capability_code :: %s\n",paczisoauto.pos_entry_capability_code);
     f_printf(stderr,"IVR  paczisoauto.pos_entry_capability_code :: %x\n",paczisoauto.pos_entry_capability_code[0]);

    #ifdef CNP_INTL
	memset (country_code, 0x00, sizeof(country_code));
	if(strlen(paczisoauto.acquiring_institution_country_code) != 0)
           memcpy(country_code, paczisoauto.acquiring_institution_country_code, 3);
     else
           memcpy(country_code, paczisoauto.currency_code_transaction, 3);

	  if(  (memcmp (paczisoauto.pos_condition_code,"08",2 ) == 0)  && 
		    (memcmp (paczisoauto.pos_entry_capability_code+8,"02",2)==0) && 
			(memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0) &&
		    (memcmp(country_code, "356", 3 ) == 0)
		  )
		{
				  f_printf(stderr,"\n SIRP - IVR transactions are not allowed for IVR merchant  \n");
							  memcpy(SubResult, "050321" , 6);
					  return ECHEC;                
		}
	#endif

		#ifdef CNP_INTL_SIRP_IVR
			if(  (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0) && 
			      ( memcmp ( country_code, "356", 3) != 0)  &&
				   (memcmp (paczisoauto.pos_entry_capability_code+8,"02",2)==0) &&  
                  (memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0)  && 
			      ( iCheck_CNP_bank(BankCode) == SUCCES)  )
			{
                  f_printf(stderr, "\n CNP - International SIRP transaction not allowed allowed in IVR merchant \n"); 
				  memcpy( SubResult, "050317", 6 );
				  return ( SubResult );
			}
		#endif


    if ((memcmp (paczisoauto.pos_entry_capability_code+8,"01",2)==0)
		 #ifdef IVR_MER_NAME
			|| (  (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)  && 
					(memcmp (paczisoauto.pos_entry_capability_code+8,"07",2)==0) && 
					(memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0) )
		 #endif
		)
	{
#ifdef IVR_TRNS_FIX
		memset (country_code, 0x00, sizeof(country_code));
		 if(strlen(paczisoauto.acquiring_institution_country_code) != 0)
                                memcpy(country_code, paczisoauto.acquiring_institution_country_code, 3);
                        else
                                memcpy(country_code, paczisoauto.currency_code_transaction, 3);

       #ifdef CNP_INTL
         if( ( memcmp(country_code, "356", 3) != 0)	 && ( iCheck_CNP_bank( paczpa05.pa05_codbnq ) == SUCCES) )
		 {		 
			   f_printf(stderr, "\n CNP - International IVR  transaction with entry mode 01 or ECI 07 with IVR in merchant name not allowed \n"); 
               memcpy(SubResult, "050321" , 6);
			   return ECHEC;
		 }
	   #endif		 

		 if(( memcmp ( country_code, "356", 3) == 0 )&& (memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0))

#else
		 if( (memcmp ( paczisoauto.acquiring_institution_country_code, "356", 3) == 0)  || ( memcmp ( paczisoauto.currency_code_transaction, "356", 3) == 0) )
#endif
		{
		  f_printf(stderr,"\n IVR - Non Participating NPC -issuer and Acquirer not participating Domestics trans not allowed  \n");
                  memcpy(SubResult, "050321" , 6);
		  return ECHEC;                
		}
		else
		{
  		   *iVerCVV2Flag = 1;
		    if (VerifyCVV2Data(SubResult) == SUCCESS)
		    {              
			       f_printf(stderr, "1. IVR - Need to verify CVV2 DATA ...\n" );
			       return SUCCESS;   
	        }		
			 return SUCCESS; 
		}
	}    
    else if(memcmp (paczisoauto.pos_entry_capability_code+8,"05",2)==0)
    {
         f_printf(stderr," IVR - paczisoauto.pos_entry_capability_code = 5\n");  
         memcpy(rthauth.CAVVstatus,&paczisoauto.additional_response_data[14],1);
         /* if (paczisoauto.additional_response_data[12] == '2') */
          if (paczisoauto.additional_response_data[14] == '2')
          {
     	      *iVerCVV2Flag = 1;
              FPRINTF(stderr,"\n IVR - CAVV Verification Passed......., CVV2 Not Verified\n");
			   if (VerifyCVV2Data(SubResult) == SUCCESS)
		       {              
			       f_printf(stderr, "2. IVR - Need to verify CVV2 DATA ...\n" );
			       return SUCCESS;   
	           }		
              return SUCCESS;                                
          } 
         else  
          {
                 f_printf(stderr,"\n IVR - INVALID CAVV RESULT VALUE .......\n");
     	         /* memcpy(SubResult,"050226",6); */
                 memcpy(SubResult, "050326" , 6);
                 return ECHEC;                           
          }
    }
	else  if ( (memcmp (paczisoauto.pos_entry_capability_code+8,"06",2) == 0) || 
		          (memcmp (paczisoauto.pos_entry_capability_code+8,"07",2) == 0) ||
             	  (memcmp (paczisoauto.pos_entry_capability_code+8,"08",2) == 0) )
	{
		         if((memcmp(paczisoauto.pos_entry_capability_code+8,"08",2) == 0)
					  #ifdef IVR_MER_NAME
						 || (  (memcmp (paczisoauto.pos_condition_code,"59",2 ) == 0)  && 
						         (memcmp (paczisoauto.pos_entry_capability_code+8,"06",2)==0) && 
			 					(memcmp (paczisoauto.card_acceptor_name_location,"IVR",3 ) == 0) )
					  #endif  
                   )
						f_printf(stderr,"\n IVR -  Authenticated NPC issuer cardholder not participating and merchant participating  \n");
                  else
						f_printf(stderr,"\n IVR -  Non Secure transaction - ECI VALUE is 06 or 07  \n");
                 memcpy(SubResult, "050322" , 6);
                 return ECHEC;                           
	}
     return SUCCESS;

}
#endif


void LoadPreatthTrace(char *preauthtimelt)
{
   int fdpaczprea;
   char filename[100];
   t_paczprea paczprea;

   memset(filename, 0x00, sizeof(filename));
   memset(&paczprea, ' ', sizeof(t_paczprea));
 
   /* memcpy(paczprea.prea_reference_auto,paczau00.au00_reference_auto, 12);
   memcpy(paczprea.prea_reference_issuer,paczau00.au00_reference_issuer, 12);
   */
   memcpy(paczprea.prea_reference_auto,paczau00.au00_reference_issuer, 12);
   memcpy(paczprea.prea_reference_issuer,paczau00.au00_reference_auto, 12);

   memcpy(paczprea.prea_num_auto_centre,paczau00.au00_num_auto_centre, 6);
   memcpy(paczprea.prea_num_carte,paczau00.au00_num_carte, 19);
  
   memcpy(paczprea.prea_num_commercant, paczisoauto.card_acceptor_identification_code , 15);
   memcpy(paczprea.prea_terminal_id, paczau00.au00_terminal_id, 8);
   memcpy(paczprea.prea_montant_demande, paczau00.au00_montant_demande, 12);
   memcpy(paczprea.prea_montant_distribue, paczau00.au00_montant_distribue, 12);
   memcpy(paczprea.prea_montant_mon_loc, paczau00.au00_montant_mon_loc, 12);

   memcpy(paczprea.prea_date_demande, paczau00.au00_date_demande, 6);
   memcpy(paczprea.prea_date_validite, paczau00.au00_date_validite, 4);
   memcpy(paczprea.prea_heure_demande, paczau00.au00_heure_demande, 4);
   memcpy(paczprea.prea_preauth_time_limit, preauthtimelt, 4);    
  
   memcpy(paczprea.prea_transmission_date_time, paczau00.au00_transmission_date_time, 10);
 
    
   /* f_printf(stderr, "paczprea  data [%.215s] \n", &paczprea);   */

   sprintf(filename, "%s/FC-PREA", getenv("MultipacDat"));
   if ( (fdpaczprea = isopen(filename, ISINOUT+ISAUTOLOCK)) < 0)
    {
         f_printf(stderr, "File open ERROR [%.50s] \n", filename);
         isclose(fdpaczprea);
         return;
     }
    if( iswrite(fdpaczprea,&paczprea) < SUCCESS ) 
    {
       f_printf(stderr, "Write failed FC-PREA [%.215s], iserrno : %d \n", &paczprea,iserrno);
    } 

   isclose(fdpaczprea);
   return;
}


int iCheckPreauth(char *strData)
{
  char acq_rrn[13], iss_rrn[13];
  char strmid[16], strtid[9];
  char datetime[12], time1[13], time2[13];
  char systemdate[9], txndate[9]; 
  char year_yyyy[5], t11[3];
  int year, t1, t2, idiff; 

  char filename[100];
  int fdpaczprea;

  t_paczprea paczprea;  
 
  memset(&paczprea, ' ', sizeof(t_paczprea)); 
  memset(acq_rrn, 0x00, sizeof(acq_rrn));
  memset(iss_rrn, 0x00, sizeof(iss_rrn));
  memset(strmid, 0x00, sizeof(strmid));
  memset(strtid, 0x00, sizeof(strtid));
  memset(datetime, 0x00, sizeof(datetime));
 
  GetFieldVisa( 36, strData ,acq_rrn); 
  GetFieldVisa( 40, strData, strtid);
  GetFieldVisa( 41, strData, strmid); 
  GetFieldVisa (6, strData, datetime);
 
  f_printf(stderr, "\n In Preauth check condition ......\n"); 
  memset(filename, 0x00, sizeof(filename));
  sprintf(filename, "%s/FC-PREA",getenv("MultipacDat")); 

  if( (fdpaczprea = isopen(filename,ISINOUT+ISAUTOLOCK)) < 0)
  {
     f_printf(stderr, "\n File not present [%s] \n", filename);
     return ECHEC; 
  }
  
    f_printf(stderr, " acq_rrn [%.15s] \n", acq_rrn);
    f_printf(stderr, " strtid  [%.8s] \n", strtid);
    f_printf(stderr, " strmid  [%.15s] \n", strmid);
    f_printf(stderr, "datetime [%.10s] \n", datetime);   
 
    memcpy(paczprea.prea_reference_auto, acq_rrn, 12);   
   /* memcpy(paczprea.prea_reference_auto,"721305001178",12); */  

    memcpy(paczprea.prea_terminal_id, strtid, 8);  
    memcpy(paczprea.prea_num_commercant, strmid, 15);  
 
    if(isread(fdpaczprea, &paczprea, ISEQUAL) != SUCCESS)  
     {
       f_printf(stderr, "\n Rec-not found in FC-PREA: iserrno :%d\n",iserrno); 
       isclose(fdpaczprea); 
       return ECHEC;  
     } 
   isrelease(fdpaczprea);
   isunlock(fdpaczprea);
 
   isclose(fdpaczprea); 
   
   memset(time1, 0x00, sizeof(time1));
   memset(time2, 0x00, sizeof(time2));

   if(memcmp(paczprea.prea_transmission_date_time, datetime, 4) == 0) 
   {
       int t1, t2;
       char t11[3];
       memcpy(t11, paczprea.prea_transmission_date_time+4,2);
       t11[2] = '\0';
       t1 = atol(t11) + 2;
       memset(t11, 0x00, sizeof(t11));
       sprintf(t11, "%02ld", t1);

       memcpy(time1, paczprea.prea_transmission_date_time+4, 6);
       memcpy(time1, t11, 2);
       memcpy(time2, datetime+4, 6);     
       
       f_printf(stderr, "\n time1   [%.6s]", time1);
       f_printf(stderr, "\n time2   [%.6s] \n", time2);   
 
       if (memcmp(time2, time1,6) > 0)
          return ECHEC;       
   }
   else
   {
      memset(systemdate, 0x00, sizeof(systemdate));
      memset(txndate, 0x00, sizeof(txndate));
      
      memcpy(txndate+4, paczprea.prea_transmission_date_time, 4);
      memcpy(systemdate, (char *)GetDateYYYYMMDD(), 8);
      if (memcmp (datetime,paczprea.prea_transmission_date_time,4) >= 0)
      {      
        memcpy(txndate,systemdate ,4);
      }           
      else
       {
          memset(year_yyyy, 0x00, sizeof(year_yyyy));
          memcpy( year_yyyy, systemdate, 4 );
          year = atoi( year_yyyy )-1;
          sprintf( txndate, "%.04d", year );
          memcpy( txndate+4,paczprea.prea_transmission_date_time, 4);
       } 
      memcpy(systemdate+4,datetime, 4);
      f_printf(stderr, " systemdate [%.8s] \n", systemdate);
      f_printf(stderr, " txndate    [%.8s] \n", txndate);     
   
      if ((idiff = DiffOfDatesYYYYMMDD(systemdate, txndate)) > 1)
         return ECHEC; 
      
      if ((idiff == 1) && (memcmp(datetime+4, "0210", 4) > 0))
      {
          f_printf(stderr, "\n MORE THAN 2 hrs ......\n");  
          return ECHEC;  
      } 
      memcpy(t11, paczprea.prea_transmission_date_time+4,2);   
      t11[2] = '\0';                                            
      t1 = atol(t11) + 2;
      if (t1 >= 24)
       {
         t2 = 24 - t1;
         sprintf(t11, "%02d", t2); 
       }
      else
        sprintf(t11, "%02d", t1);

      memset(time1, 0x00, sizeof(time1));
      memcpy(time1, paczprea.prea_transmission_date_time+4, 6);
      memcpy(time1, t11, 2); 
      memcpy(time2, datetime+4, 6);

      f_printf(stderr, "\n Original time1   [%.6s]", time1);
      f_printf(stderr, "\n Trans time2      [%.6s] \n", time2);

      if (memcmp(time2, time1,6) > 0)
         return ECHEC;
      
   }
   f_printf(stderr, "\n Date and Time original trans [%.10s] \n", paczprea.prea_transmission_date_time);
   f_printf(stderr, "\n 0120 Trans date and time     [%.10s] \n", datetime);  
   
   return SUCCESS; 
}

/* preema for M Secure 300409 */
#ifdef M_SECURE 
#ifdef VBV_NEW
int Get_Ecom_Ind(char *data, char status[] , char *mci_cntr_code)
#else
int Get_Ecom_Ind(char *data, char status[] )
#endif
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
      /*      printf("Tag [%s] Length [%s]\n",tag, length);          */
      if( atoi(length) >0 ) 
      {
          if( atoi(tag) == 42 ) 
          {
	      memcpy(Data1,data+i,7);
	      f_printf(stderr,"DATA48.42[%.7s]\n",Data1);
              memcpy(status, Data1+6, 1);
		f_printf(stderr, " mci_cntr_code[%.3s]\n",mci_cntr_code);
#ifdef VBV_NEW
		  if( (memcmp(Data1+6,"2",1) == 0 ) || ( (memcmp(Data1+6,"0",1) == 0 ) && (memcmp ( mci_cntr_code, "356", 3) != 0) ) )
		   {
	           #ifdef CNP_INTL
			  if(  (memcmp(Data1+6,"2",1) != 0 )  && ( memcmp(mci_cntr_code, "356", 3) != 0) && ( iCheck_CNP_bank( paczpa05.pa05_codbnq ) == SUCCES) )
			  {		 
					   f_printf(stderr, "\n CNP - International Msecure transactions with capalility code 0 not allowed \n"); 
					    return ECHEC;
			  }
			  #endif	
			   return SUCCESS;
		   }
#else
	      if((memcmp(Data1+6,"0",1) == 0 ) || (memcmp(Data1+6,"2",1) == 0 ))
	      {
		  return SUCCESS;
	      }
#endif
              else /*  if(memcmp(Data1+6,"1",1) == 0 ) */
		   return ECHEC;
          } 
          else
	      i = i+atoi(length); 
      }
      else 
          return ECHEC;
   }
}

#endif

#ifdef CMS_EHOST 



#include <pcrzbpfl.h>
int iGetBonus_Points( char *Cardnumber, char *Bonus_Points)
{ 
    char points[8];
    char file_to_open[100]; 
    int fd_bonus = -1;
	
    t_pcrzbpfl pcrzbpfl; 

  	memset(file_to_open, 0x00, sizeof(file_to_open));
    memset(points, 0x00, sizeof(points));

    sprintf( file_to_open, "%s/FCR-BONUS", getenv("MultipacDat"));
    fd_bonus  =  isopen( file_to_open, ISINPUT+ISAUTOLOCK );
    if (fd_bonus  <  0)
    {
            f_printf(stderr, "File open error FCR-BONUS [%.19s], iserrno %d\n", pcrzbpfl.bpfl_card_number, iserrno);
			 memcpy(Bonus_Points, "000000", 6);
			 return ECHEC;
     }
  
    memset( &pcrzbpfl, ' ', sizeof(t_pcrzbpfl));
    memcpy( pcrzbpfl.bpfl_card_number, Cardnumber, 16);
    if ( isread(fd_bonus, &pcrzbpfl, ISEQUAL) == 0 )
     {
            isrelease(fd_bonus);
             isunlock(fd_bonus);
             memcpy(points, pcrzbpfl.bpfl_closing_points + 4, 6);
             /*tmp_field_63[6] = 0;*/
             sprintf(Bonus_Points, "%.6s", points); 
             Bonus_Points[6] = '\0';
     }
     else
           memcpy(Bonus_Points, "00000000", 8);
   isclose(	  fd_bonus);

   return SUCCESS;
}
#endif 

#ifdef CBA

#ifdef CBA_OLD
int iGet_TransLimit(char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, t_pacztrnslimit  *ptrpacztrnslimit)
{
    int fdauth = -1; 
	int iRetval = 0, iloop = 0, lErrNo , iCount = 0;
	char filename[100], strloop[15];   
 
	memset(filename, 0x00, sizeof(filename));
	memset(strloop, 0x00, sizeof(strloop));

   sprintf(filename, "%s/FC-AUTHPARAM", getenv("MultipacDat"));
   fdauth = isopen(filename, ISAUTOLOCK+ISINOUT);
   if( fdauth < 0)
	{
         f_printf(stderr, "\n File Open Error [%s] ", filename); 
          return (ECHEC);  
	}
      
  strloop[0] = 6; strloop[1] = 2; strloop[2] = 3;  strloop[3] = 5; 
  strloop[4] = 7;  strloop[5] = 4;  strloop[6] = 8;                    
                                                                       
   for (iloop = 0; iloop < 7; iloop++)                                                    
   {          
	   memcpy(ptrpacztrnslimit->table_code, "01", 2);
	   memcpy(ptrpacztrnslimit->bin_prod, ptrbin, strloop[iloop]);
	   memcpy(ptrpacztrnslimit->bin_prod+strloop[iloop], ptrcardtype, 4);
	   memcpy(ptrpacztrnslimit->mode, ptrmode, 1);
	   memcpy(ptrpacztrnslimit->trns_type,ptrtrnstype, 2);
       iCount = 0;
	  do 
      {
          iRetval  = isread(fdauth,(char *)ptrpacztrnslimit,ISEQUAL);                  
          lErrNo = iserrno ;
          isunlock(fdauth);                                             
          if (iRetval == 0)                                                     
          {
			  break; 
		  }
		  iCount++;
       }while ( (lErrNo == ELOCKED) && ( iCount < NB_FOIS ) ); 
   }                                         
 
 isclose(fdauth);

  if( (iRetval < 0) || (iCount >= NB_FOIS) )
	{
           f_printf(stderr, "\n Record Not Found in the FC-AUTHPARAM  [%d] ", lErrNo); 
           return (ECHEC);  
	}

   return SUCCESS; 
}
#endif 

int iGet_TransLimit(char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, t_pacztrnslimit  *ptrpacztrnslimit)
{
    int fdauth = -1; 
	int iRetval = 0, iloop = 0, lErrNo , iCount = 0, ipos = 0;
	char filename[100], strloop[15], strbin[15];   
 
	memset(filename, 0x00, sizeof(filename));
	memset(strloop, 0x00, sizeof(strloop));
    memset(strbin, 0x00, sizeof(strbin));

   sprintf(filename, "%s/FC-AUTHPARAM", getenv("MultipacDat"));
   fdauth = isopen(filename, ISAUTOLOCK+ISINOUT);
   if( fdauth < 0)
	{
         f_printf(stderr, "\n File Open Error [%s] ", filename); 
          return (ECHEC);  
	}

   for(ipos = 0; ipos < 8; ipos++)
   {
	   if(ptrbin[ipos] == ' ')
		   break;
   }
  
  /*  ipos++;  */
	memset(ptrpacztrnslimit, ' ', sizeof(t_pacztrnslimit));
    memcpy(ptrpacztrnslimit->table_code, "01", 2);
    memcpy(ptrpacztrnslimit->bin_prod, ptrbin, ipos);
    memcpy(ptrpacztrnslimit->bin_prod+ipos, ptrcardtype, 4);
	memcpy(ptrpacztrnslimit->mode, ptrmode, 1);
	memcpy(ptrpacztrnslimit->trns_type,ptrtrnstype, 2);
	iCount = 0;


	do 
     {
          iRetval  = isread(fdauth,(char *)ptrpacztrnslimit,ISEQUAL);                  
          lErrNo = iserrno ;
          isunlock(fdauth);                                             
          if (iRetval == 0)                                                     
          {
			  break; 
		  }
		  iCount++;
       }while ( (lErrNo == ELOCKED) && ( iCount < NB_FOIS ) ); 

 isclose(fdauth);

 if(lErrNo == ELOCKED)
 {
	 f_printf(stderr, "\n Record Locked - FC-AUTHPARAM [%d]", lErrNo);
	 return(SYS_ECHEC);  
 } 


  if( iRetval < 0)
	{
          f_printf(stderr, "\n in Trans Limit - Record Not Found in the FC-AUTHPARAM  [%d] ", lErrNo); 
           return (ECHEC);  
	}


   return SUCCESS; 
}
 
int iCheck_TransactionLimit(char *ptrtrnsamt, char *ptrbin, char *ptrcardtype, char *ptrmode, char *ptrtrnstype, int istatus)
{
 	int iRetval = 0;
	char stramt_min[15], stramt_max[15], trns_amt[15]; 
    t_pacztrnslimit pacztrnslimit; 

    memset(&pacztrnslimit, ' ', sizeof(t_pacztrnslimit)); 
	memset(stramt_min, 0x00, sizeof(stramt_min));
	memset(stramt_max, 0x00, sizeof(stramt_max));
	memset(trns_amt, 0x00, sizeof(trns_amt));
    
	/* f_printf(stderr, "\n CBADEBUG In Txn Limit Checking ............"); */
	 f_printf(stderr, "\n CBS   ptrtrnsamt [%s], ptrbin [%.8s] ptrcardtype [%s] ptrmode [%s] ptrtrnstype [%s], istatus [%d] \n", ptrtrnsamt, ptrbin, ptrcardtype,ptrmode, ptrtrnstype, istatus); 
	#ifdef SHELL_PHASE1
	/* if( (memcmp(paczpa05.pa05_codbnq, "00062", 5) == 0 ) && ((memcmp(paczisoauto.processing_code, "27", 2) == 0)||*/
	if(((memcmp(paczisoauto.processing_code, "27", 2) == 0)||(memcmp( paczisoauto.processing_code, "86", 2 ) == 0 )||
            (memcmp( paczisoauto.processing_code, "08", 2 ) == 0 )||(memcmp( paczisoauto.processing_code, "07", 2 ) == 0 ) 
                  ||(memcmp( paczisoauto.processing_code, "81", 2 ) == 0 )))
	{
		return(SUCCESS);
	}
	#endif
        #ifdef LODHA_PALAV
        if (memcmp(paczpa05.pa05_codbnq, "00087", 5) == 0 )
        return(SUCCESS);
        #endif
   memcpy(trns_amt, ptrtrnsamt, 12);
 
    iRetval = iGet_TransLimit(ptrbin, ptrcardtype, ptrmode, ptrtrnstype, &pacztrnslimit); 
	if(iRetval == SYS_ECHEC)
		return SYS_ECHEC; 

	if(iRetval != SUCCESS)
	{
		f_printf(stderr, "\n Transaction Limit not defined "); 
		return SUCCESS; 
		/* return (ECHEC);  */
	}

   switch(istatus)
	{
           case 0:
			  f_printf(stderr, "\n ONUS  Transaction min and Max limit ");
              memcpy(stramt_min, pacztrnslimit.onus_dom_min_trnslimit, 12);
              memcpy(stramt_max, pacztrnslimit.onus_dom_max_trnslimit, 12);
              break;

		   case 1:
   			  f_printf(stderr, "\n Offus transaction min and Max limit ");
              memcpy(stramt_min, pacztrnslimit.offus_dom_min_trnslimit, 12);
              memcpy(stramt_max, pacztrnslimit.offus_dom_max_trnslimit, 12);
			  break;
          
		   case 2: 
    		   f_printf(stderr, "\n International Transaction min and Max limit ");
               memcpy(stramt_min, pacztrnslimit.intl_min_trnslimit, 12);
               memcpy(stramt_max, pacztrnslimit.intl_max_trnslimit, 12);
			   break;
       
	       default:
               f_printf(stderr, "\n Invalid Transaction min and Max limit "); 
               break; 
	}

	memset(&gpacztrnslimit, ' ', sizeof(t_pacztrnslimit));
   memcpy(&gpacztrnslimit, &pacztrnslimit, sizeof(t_pacztrnslimit));

	if( (strlen(stramt_min) != 12) && (strlen(stramt_max) != 12) )
	{
           f_printf(stderr, "\n Invalid Limit Amount stramt_min [%s] , stramt_max [%s]", stramt_min, stramt_max); 
            return (ECHEC);
	}

    if( (atof(trns_amt) < atof(stramt_min) )  ||  (atof(trns_amt) > atof(stramt_max) ) )
	{
           f_printf(stderr, "\n Transaction Limit Mismatch trns_amt [%.12s], stramt_min [%.12s], stramt_max [%.12s]", trns_amt, stramt_min, stramt_max); 
            return (ECHEC);
	}

  
	/* f_printf(stderr,"\n CBADEBUG Txn Limit Success ptrtrnsamt [%s], Min [%s], Max [%s] ", ptrtrnsamt, stramt_min, stramt_max );   */
	return SUCCESS;
}

#endif

extern int ReplaceFieldMci_1(int field, char *text, char *data);

#ifdef IDBI_BCHA_EHOST 

#ifdef BOI_FRM
int  ReadResponseFromDispatch( char  p_accord[ ], char  p_no_accord[ ] , char  p_rrn[ ], char p_no_porteur[ ], char *fld54, char DestId,  t_struct_iso *pack_resp,char *Frm_Bankcode,int Message_Type_FRMT)
#else
int  ReadResponseFromDispatch( char  p_accord[ ], char  p_no_accord[ ] , char  p_rrn[ ], char p_no_porteur[ ], char *fld54, char DestId,  t_struct_iso *pack_resp)
#endif
{

  char t_p_rrn[13],   strFLD_54[25];
  int  ERRNO, i, resultat, ilen = 0;
  #ifdef BOI_FRM
  int  MessageType_Frm  = 0 ;
  #endif
  t_msg_bl balbuf;
  t_struct_iso paquet;

   extern   int  RecuSIGALRM; 
  long process_id;  
  /* 
  extern int  RecuSIGUSR1;
  extern int  RecuSIGUSR2;
  extern int  RecuSIGALRM;
*/
  int  RecuSIGUSR1;
  int  RecuSIGUSR2;
/*  int  RecuSIGALRM; */



  f_printf(stderr,"RECEIVING FROM DISPATCH\n");
  f_printf(stderr,"BANKCODE .....[%.5s]\n",Frm_Bankcode);

  memset(t_p_rrn, 0, 13);
  memcpy(t_p_rrn, p_rrn, 12);

  RecuSIGUSR1 = 0;
  RecuSIGUSR2 = 0;
  RecuSIGALRM = 0; 

  #ifdef BOI_FRM
  if(( DestId == EHOST )&&( memcmp(Frm_Bankcode, "00006", 5) == 0 )&& (( Message_Type_FRMT == MSG_0100) ||( Message_Type_FRMT == MSG_0101)|| (  Message_Type_FRMT == MSG_0200))) 
  alarm(1);
  else if((DestId == EHOST)&&( memcmp(Frm_Bankcode, "00006", 5) == 0 )&&(( Message_Type_FRMT == MSG_0120)||( Message_Type_FRMT == MSG_0400)||( Message_Type_FRMT == MSG_0420))) 
  alarm(2);
  else
  #endif
  if(( DestId == EHOST || DestId == STAND_IN ) && ( memcmp(Frm_Bankcode, "00006", 5) != 0 ) )
    alarm(15);
  else
    alarm(24);  

	  do
	  {
			process_id=(long)getpid();
			resultat = msgrcv(bal1,&balbuf,LGCONTMAX,process_id,!IPC_NOWAIT);
			ERRNO = errno;
			if (ERRNO == EINTR) 
				break; 
	  } while ( (resultat <= 0) && (RecuSIGUSR2 <= 0) && (RecuSIGALRM <= 0) );

	  alarm(0);
	  fflush(stdin);

	  if (RecuSIGUSR2 > 0)
	  {
			f_printf(stderr,"REQUEST TO STOP\n");
			RecuSIGUSR2 = 0;  
			return( ECHEC );
	  }

	  if (resultat < 0)
	  {
			f_printf(stderr,"msgrcv() RESPONSE IMPOSSIBLE; ERRNO == %d\n",ERRNO);
			RecuSIGUSR2 = 0; 
			return( ECHEC );
	  }

    StructIsoCpy(&paquet,&(balbuf.mtexte)); 
    memcpy(pack_resp, &paquet, sizeof(t_struct_iso));

   switch ( paquet.entete.systeme )
   {  
     case 'V' : 
     #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 31 AUG 2016 */
     case 'R' :
     #endif
		affiche_message_iso_VISA(&paquet);
               break;

     case 'M' : affiche_message_iso_MCI(&paquet);
                break;

     case 'A' : affiche_message_iso_AEGN(&paquet);
                break;

     default : 
		f_printf(stderr, "\n Unsupported mesage format "); 
		 break;
   }

    f_printf(stderr,"REQUEST/RESPONSE VERIFICATION \n");
  
    if  (       ( memcmp(t_p_rrn,        paquet.entete.reference,  12 ) != 0 )
		    || ( memcmp(p_no_porteur, paquet.entete.porteur,    19 ) != 0 )		    
        )
    {
       return( -1 );     /* Response 91 */
    }

        memcpy( p_accord , paquet.entete.cd_reponse , 2 );
	memcpy( p_no_accord, paquet.entete.accord    , 6 );
	memset(strFLD_54, 0x00, sizeof(strFLD_54));
					 
   if(  (memcmp( p_accord , "00"    , 2 ) == 0 ) || (memcmp( p_accord , "08"    , 2 ) == 0 ) )
     {
			 switch( paquet.entete.systeme )
			  {
				  case 'V' : 
					  if( GetFieldVisa(53, paquet.data,strFLD_54) == 0)
				 	  {
				 		   f_printf(stderr , "Field54  value [%s]  Length[%d]\n", strFLD_54, strlen(strFLD_54));
						   ilen = strlen(strFLD_54);
						   memcpy(fld54, strFLD_54, ilen);
					  }
					  break;
				 case 'M':
                      f_printf(stderr, "Field 54 Data \n");
  				      if( GetFieldMci(53, paquet.data,strFLD_54) == 0)
				 	  {
						   ilen = strlen(strFLD_54);
						   memcpy(fld54, strFLD_54, ilen);
  				 		   f_printf(stderr , "CBS - Field54  value [%s]  Length[%d]\n", strFLD_54, strlen(strFLD_54));
					  }
					  else
							f_printf(stderr , "\n CBS - Field 54 Not present in response ");

                      break;                                        

	    		 default:
					  f_printf(stderr, "\n Unsupported format ");
					  break; 
			}
	 }

    return( SUCCESS );
}

#endif 

#ifdef CBA 
int Insert_FeesAmt(t_struct_iso *Request, char *strtot_feesamt, int *ilen)
{

    int TempLen = 0, iRetval = 0;
	char *ISOData;

    ISOData =  Request->data;
	f_printf(stderr, "\n CBS Fees Amount [%s]", strtot_feesamt);
	switch ( Request->entete.systeme )
	{
			case 'V':
				f_printf(stderr, "V - BEFORE inserting Fees Amount \n");
				iRetval = InsertFieldVisa ( ADDITIONAL_AMOUNTS, ISOData,  strtot_feesamt, &TempLen );
				if(iRetval != 0)
				{
                        f_printf(stderr, "\n V - Insert Fees Amt Failed "); 
				}

				*ilen = TempLen; 
				break;

			case 'M':
				f_printf(stderr, "M - BEFORE inserting Fees Amount \n");
				iRetval = InsertFieldMci ( ADDITIONAL_AMOUNTS, ISOData,  strtot_feesamt, &TempLen );
				if(iRetval != 0)
				{
                        f_printf(stderr, "\n M - Insert Fees Amt Failed "); 
				}
				*ilen = TempLen;		
				break;

			default :
				f_printf(stderr, "\n Format not supported by the system ");
				break; 
	}
	return SUCCESS;
}


int Insert_Replace_BankAccNum(t_struct_iso *Request, t_bankacnt bankacnt, int *ilen)
{

    int TempLen = 0;
	char bank_acc_num[30];
 	char *ISOData;

	memset(bank_acc_num, 0x00, sizeof(bank_acc_num));
	*ilen = 0;
	
	if(memcmp(bankacnt.bank_acc,  "          ", 10)== 0)
		return SUCCESS;
    
	memcpy(bank_acc_num, bankacnt.bank_acc, 24);
    f_printf(stderr, "\n CBADEBUG bank_acc_num - [%s]", bank_acc_num);

    ISOData =  Request->data;
	switch ( Request->entete.systeme )
	{
			case 'V':
				f_printf(stderr, "V - BEFORE inserting Bank Acc Num \n");
			    /* 
				if( InsertFieldVisa ( DEBIT_PRIVATE_USE, ISOData,  bank_acc_num, &TempLen ) == ECHEC)
				{
					ReplaceFieldVisa ( DEBIT_PRIVATE_USE, ISOData,  bank_acc_num, &TempLen) ;
				}
				*ilen = TempLen; 
				*/
				break;

			case 'M':
				f_printf(stderr, "M - BEFORE inserting Bank Acc Num  \n");
				if(InsertFieldMci ( BANKNET_DATA, ISOData,  bank_acc_num, &TempLen ) == ECHEC)
				{
					/* ReplaceFieldMci( BANKNET_DATA, ISOData,  bank_acc_num); */
						*ilen = 0;		
				}
				else
					*ilen = TempLen;		
				break;

			default :
				f_printf(stderr, "\n Format not supported by the system ");
				break; 
	}
	return SUCCESS;
}


#endif 





#ifdef MCI_EMV_ISS
int TVR_CVR_check_result(char *data, char *crpt_status, int fld48_len, int *itvr_len)
{
   int i = 1;
   char tag[3];
   char length[5];
   char Data1[100];
   char sub_01[5];
   char loc_crstat[10];
   int ilen = 0;
   int itag = 0;
   char temp_EMV[100];

   while(fld48_len > i+4 )
   {
       memset(tag, 0x00, sizeof(tag));
       sprintf(tag,"%.2s",data+i );
       itag = atoi(tag);
       f_printf(stderr, "\n Tag %d ", atoi(tag));
       i = i+2;

       memset(length, 0x00, sizeof(length));
       sprintf(length,"%.2s",data+i);
       ilen = atoi(length);
       f_printf(stderr, "\n Length : %.2s", length);
       i = i+2;

       if( ilen  > 0 )
       {
          f_printf(stderr, "in Data Tag is [%d] and Len is [%d]\n", itag, ilen);
          if((  atoi(tag) == 71 )  )
          {
                f_printf(stderr, "in 71 Tag is [%d] and Len is [%d] i is [%d] \n", itag, ilen, i);
                memset(temp_EMV, 0x00, sizeof(temp_EMV));
                sprintf(temp_EMV, "%s",data+i);
                f_printf(stderr, "temp_EMV[%s]\n",temp_EMV);
                memcpy(crpt_status, temp_EMV, ilen);
		*itvr_len = ilen;
                return SUCCESS;
          }
          i = i+ ilen;
       }
   }
   return ECHEC;
}
#endif

#ifdef BFL_CUG_CARD
char *BFL_ProcessRequest_0100( t_struct_iso *Request , t_entete_iso     ISOHeader)
{
        int                             MessageType;    /* MTI */
        int                             ecom_security;
        int                             i, iRetval = -1;
        int                             iVerCVV2Flag;
        int                             itagval = 0;
        int                             iFlag = 0;
        double                          TxnAmount = 0.0;

        char                    	TxnCurrency[4];
        char                    	LocalCurrency[4];
        char                    	Amount[13];
        char                    	MessageFormat;
        static char 		  	SubResult[6+1];
        char                            VerificationResult[3], strtemp[50];
        char        			*ISOData;   /* Bit MAP + DATA */
        char                    	ExpiryDate[4+1];
        char                    	BankCode[6];
        char                    	AccountNumber[25];
        char                    	Customer_id[20];
        char                    	Actual_Card_No[20];
        char                            AccessCode[7];
        char         			Track_Card_Number[20];
        char         			Track_Expiry_Date[5];
        char         			Pos_Data_Info[30];
        char         			TCC_Value[2];
        char         			reset_flag [2];
        char         			cAccount_no[25];
        char         			Cardno[20],Value[20],Amt[13];
        char                            Asset_Category_Risk[3];
        int          			type = 0 ;
	char           			branchid[10];
	char                            systemdate[9];

	char 				cust_open[100];
	int 				lnamnt =0;
     

	   /*  MTI  */
        MessageType = Request->entete.type;

        ISOData = Request->data;        /* Copy the ISO DATA to local variable */

        MessageFormat = ISOHeader.systeme;

        /***********************************************************************/
        /* IF THE REQUEST COME FROM EURO/MASTER, GET THE RRNO BASED ON             */
        /*  ACQ INSTITUTION CODE AND STAN                                                                          */
        /***********************************************************************/
        memset ( User, 0, 7 );
        memcpy ( User, ISOHeader.user, 6 );
        FormatRRNOForMCI ( ISOHeader.user, &paczisoauto );

        f_printf(stderr,"\n In BFLProcessRequest_0100 [%s]", cGetTimeYYYYMMDDHHMMSS());
        if (memcmp(Request->entete.banque, "00059", 5) != 0) 
        {
                return ( "300000" );
        }
        Refresh_strings();
        Card_First_Trans = 0;
        chargeslip_first_fee = 0;
	#ifdef MRL_383
		fprintf(stderr,"Value of rset_card_1st_fee = %d\n",rset_card_1st_fee);
		rset_card_1st_fee = 0;
	#endif

        memset ( ExpiryDate, 'X', 5 );
        memset ( ServiceCode, 0, 4 );
        memset ( pvv, 0, 6 );
        memset ( cvv, 0, 4 );
        memset ( cvc2, 0, 4 );
        memset ( PinFormat, 0, 3 );
        memset ( SubResult, '0', 6 );
        memset( field_59data, 0x00, sizeof(field_59data));
        memset( cAccount_no, 0x00, sizeof(cAccount_no));
        memset( Cardno, 0x00, sizeof(Cardno));
        memset( Value, 0x00, sizeof(Value));
        memset( AccessCode , 0x00, sizeof(AccessCode));
        memset( Asset_Category_Risk , 0x00, sizeof(Asset_Category_Risk));
        memset( Original_Amount , 0x00, sizeof(Original_Amount));
        #ifdef BFL_PHASE2
        memset( Prefered_Limit,0x00, sizeof(Prefered_Limit));
        Ecs_Waive_Off = 0;
        Velocity_Check = 0;
        Same_Merchant = 0 ;
        NXTALLD  = 0 ;
        #endif
        memset(&bflzport, ' ', sizeof( t_bflzport));
        memset(&bflzbcom, ' ', sizeof( t_bflzbcom));
        memset(&bflzbcli, ' ', sizeof( t_bflzbcli));
        memset(&lnaddport,' ', sizeof( t_lnaddport));
        
        memset((char *)&procdata,0x20,sizeof(t_procdata));
        memset(&bfl_dealer_master,0x20,sizeof(t_bfl_dealer_master));

#ifdef BFL_TIMEOUT_ISSUE
	int time_before = 0;
	int time_after = 0;
	time_before = time(0);
	f_printf(stderr, "Time_Before: %d\n", time_before);
#endif



   /*  GET CARDNUMBER FROM TRACK2 IF POS ENTRY MODE IS NOT KEY ENTRY */
        if ( strlen ( paczisoauto.primary_account_number ) == 0 )
        {
             GetTrackCardNumber ( paczisoauto.primary_account_number,   paczisoauto.track1_data, paczisoauto.track2_data );
        }

	memcpy(CARD_NO, paczisoauto.primary_account_number, 16);
	memcpy(RRN_NO,paczisoauto.retrieval_reference_number,12);
	
	fprintf(stderr,"\"%s\",Function \"%s\" RRN[%.12s] \n",__FILE__, __FUNCTION__,RRN_NO);


	/**************************************************************************/
        /*          CHECK THE CARD ISSUER (BANK)                                  */
        /**************************************************************************/
        if(memcmp( paczisoauto.primary_account_number,"222222",6) != 0 )
        {
        	if ( GetIssuerDetails ( &paczpa05, paczisoauto.primary_account_number )  != SUCCESS )
        	{
                	f_printf(stderr, "\nERROR : CARD ISSUER [%.9s]does not exist \n",  paczisoauto.primary_account_number );
                	return ( "961001" );
        	}

        /* copy the Bank code */
        memset ( BankCode, 0, 6 );
        memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
        memcpy ( CODE_BANQUE, paczpa05.pa05_codbnq, 5 );

        /**************************************************************************/
        /*        GET THE BANK DETAILS                                            */
        /**************************************************************************/
        	if ( GetBankDetails ( BankCode, &paczpa02 ) == ECHEC )
        	{
                	f_printf(stderr, "ERROR : Bank [%.5s] DOES NOT EXIST \n", BankCode );
                	return ( "961004" );
        	}
        }

        /**************************************************************************/
        /*           CHECK CARDHOLDER MASTER                                      */
        /**************************************************************************/
        f_printf(stderr, "\n BankCode [%s][%s]", BankCode, cGetTimeYYYYMMDDHHMMSS());


        memset ( ServiceCode, 0, 4 );
        memset ( pvv, 0, 6 );
        memset ( cvv, 0, 4 );
        memset ( cvc2, 0, 4 );
        memset ( PinFormat, 0, 3 );
        memset ( Customer_id, 0, 20 );
        memset ( Actual_Card_No, 0, 20 );
        /**************************************************************************/
        /*           CHECK CARD  MASTER                                           */
        /**************************************************************************/
	f_printf(stderr, "\n Time: %s\n", cGetTimeYYYYMMDDHHMMSS());
        if(memcmp( paczisoauto.primary_account_number,"222222",6) == 0 )
        {
                  Mobile_No_Tranx = 0;
                  memset(Original_Cell_Number,0x00,sizeof(Original_Cell_Number));
                  memcpy(Original_Cell_Number,paczisoauto.primary_account_number,16);
                 if( GetActualCardNumber(paczisoauto.primary_account_number,Customer_id,Actual_Card_No,SubResult)  != SUCCESS )
                  {
                        f_printf(stderr, "\nCARDHOLDER [%.19s]does not exist  Aginst this Mobile Number\n",\ 
                                  paczisoauto.primary_account_number );
                        return ( SubResult );
                  }
                 if ( BFLCheckCardNumber ( Actual_Card_No, &bflzport) != SUCCESS )
                  {
                        f_printf(stderr, "\nCARDHOLDER [%.19s]does not exist \n", paczisoauto.primary_account_number );
                        if ( iserrno == ELOCKED )
                                return ( "961002" );
                        else
                                return ( "B30000" );
                  }
                	memcpy(paczisoauto.primary_account_number,bflzport.lport_card_no,19);
                	memcpy(Customer_id,bflzport.lport_cust_id,19);
                	f_printf(stderr, "\n Customer_id [%s]", Customer_id);
                	if ( GetIssuerDetails ( &paczpa05, paczisoauto.primary_account_number )  != SUCCESS )
                	{
                        	f_printf(stderr, "\nERROR : CARD ISSUER [%.9s]does not exist \n", \
                        	paczisoauto.primary_account_number );
                        	return ( "961001" );
                	}

                        /* copy the Bank code */
        		memset ( BankCode, 0, 6 );
        		memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
        		memcpy ( CODE_BANQUE, paczpa05.pa05_codbnq, 5 );

        		/**************************************************************************/
        		/*        GET THE BANK DETAILS                                            */
        		/**************************************************************************/
                	if ( GetBankDetails ( BankCode, &paczpa02 ) == ECHEC )
                	{
                        	f_printf(stderr, "ERROR : Bank [%.5s] DOES NOT EXIST \n", BankCode );
                        	return ( "961004" );
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
                                return ( "B30001" );
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
	f_printf(stderr, "TIME2: %s\n", cGetTimeYYYYMMDDHHMMSS());
        if ( BFLAccountNumber (Customer_id, &bflzbcom) != SUCCESS )
        {
                f_printf(stderr, "\nAccount Master  [%.19s]does not exist \n", Customer_id);
                        if ( iserrno == ELOCKED )
			{
				f_printf(stderr, "TIME3: %s\n", cGetTimeYYYYMMDDHHMMSS());
                                return ( "961013" );
			}
                        else
                                return ( "B10003" );
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
                                return ( "B10004" );
        }
        /**************************************************************************/
        /*          READ DEALER MASTER                                            */
        /**************************************************************************/
         if ( Open_Dealer_Master(SubResult) != SUCCESS )
         {
                 f_printf(stderr, "ERROR IN READING DEALER MASTER\n" );
                 return ( SubResult );
         }
        /**************************************************************************/
        /*           VALIDATE INPUT DATA                                          */
        /**************************************************************************/

         f_printf(stderr, "\nBefore   BFLAccountNumber \n" );
       	if( MessageFormat ==  'V' )
        {
                    if (GetFieldVisa ( NATIONAL_POS_GEOGRAPHIC_DATA, ISOData,    field_59data ) != 0 )
                    {
                                        f_printf(stderr, "\n Field 59 Not present in the request  ");
                                        return ( "960306" );
                    }
                    f_printf(stderr, "\n Field 59 data [%s]", field_59data);
                    taglenvalue(field_59data,1);
        }
        else
        {
                    f_printf(stderr, "\n Message Format Not supported");
                    return ( "960307" );
       	}
        if(  memcmp( paczisoauto.processing_code, "00", 2 ) == 0 )
        {
                if(VerifytheSchemeData() != SUCCESS )
                {
                    f_printf(stderr, "\n Scheme Validation Failed");
                    return ( "960307" );
                }

		/**************************************************************************/
		/*          SCHEME - BRANCH MAPPING CHECK                                 */
		/**************************************************************************/

                if(
			(memcmp(buisness_prod_code,"REMI",4) != 0)&&
			(memcmp(buisness_prod_code,"ECF",3) != 0) &&
			(memcmp(buisness_prod_code,"WLT",3) != 0) &&
			(memcmp(buisness_prod_code,"ERF",3) != 0)
		)
                {
			f_printf(stderr, "\nPROCDATA SOURCE: [%.2s].\n", procdata.source);
			f_printf(stderr, "buisness_prod_code: [%.4s].\n", buisness_prod_code);
			f_printf(stderr, "Checking Scheme-Branch Mapping\n");

			memset(branchid,0x00,sizeof(branchid));
			GetBranchId(branchid);
		#ifdef BFL_TIMEOUT_ISSUE
			if((memcmp(procdata.source, "WP", 2)!=0) && (memcmp(procdata.source, "SP", 2)!=0) && (memcmp(procdata.source, "SI", 2)!=0) )
			{
		#endif
			#ifdef REMOVE_DB
				if(Check_Scheme_Branch_Api(RRN_NO , CARD_NO, scheme_id,branchid) != 0)
			#else
				if ( check_scheme_branch(scheme_id,branchid) != 0 )
			#endif
				{
					f_printf(stderr, " *\tScheme-Branch mapping ERROR\n" );
					return ( "H10001" );
				}
		#ifdef BFL_TIMEOUT_ISSUE
			}
		#endif
                }
        }

        /**************************************************************************/
        /*          TYPE OF CARD                                                  */
        /**************************************************************************/
        if ( BFL_GetCardType ( paczisoauto.primary_account_number, \
          BankCode, Bfl_Card_Type) != SUCCESS )
         {
                /* Card Type not found */
                f_printf(stderr, "CARD PRODUCT NOT FOUND FOR CARD[%.19s]\n", \
                 paczisoauto.primary_account_number );
                return ( "961018" );
         }

         /***********************************************************************/
         /*       BALANCE ENQ CHECK                             */
         /***********************************************************************/
        if( (  (  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
               (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 )
            ) &&
            ( paczpa05.pa05_balance_inquiry[0] != 'Y' )
          )
          {
           f_printf(stderr, "BFL-Balance Enq. flag disabled, hence not supported\n");
           return( "530002" );
          }
        /**************************************************************************/
        /*          CHECK CARD DELIVERY STATUS                                    */
        /**************************************************************************/
         if ( Check_Card_Delivery_Status( ) != SUCCESS )
         {
                if(Card_Embo_Date_Checking()!=SUCCESS)
                {
                        f_printf(stderr, "Check_Card_Delivery_Status/Card_Embo_Date_Checking Failed\n" );
                        memcpy(SubResult,"C50002",6);
                        return ( SubResult );
                }
         }
        /**************************************************************************/
        /*          CHECK CARD ACTIVATION STATUS                                  */
        /**************************************************************************/
         if ( checkCardActivation_BFL(SubResult) != SUCCESS)
         {
                        return ( SubResult );
         }
        /**************************************************************************/
        /*          CHECK CARD MANUAL BLOCK STATUS                                */
        /**************************************************************************/
         #ifdef NEVER
         if ( checkCardManualBlock_BFL() != SUCCESS)
         {
                        return( "B90001");
         }
         #endif
        /**************************************************************************/
        /*          CHECK CARD SYSTEM BLOCK STATUS                                */
        /**************************************************************************/
         if ( checkCardSystemBlock_BFL(SubResult) != SUCCESS)
         {
                        return ( SubResult );
         }
        /**************************************************************************/
        /*          CHECK ACCOUNT SYSTEM BLOCK STATUS                             */
        /**************************************************************************/
         if ( checkAccountSystemBlock_BFL() != SUCCESS)
         {
                        return ( "K40015" );
         }
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
        /*          CHECK CARD ACCESS CODE - PIN                                  */
        /**************************************************************************/
	if ( VerifyPIN_BFL ( AccessCode ,SubResult) != SUCCESS )
	{
		f_printf(stderr, "PIN VERIFICATION FAILURE\n" );
		return ( SubResult );
	}
        /**************************************************************************/
        /*          CARD SOURCE TYPE VALIDATIONS                                  */
        /**************************************************************************/
         if ( Validate_Card_Source(SubResult) != SUCCESS )
         {
                 f_printf(stderr, "Validate_Card_Source Failure\n" );
                 return ( SubResult );
         }
        /**************************************************************************/
        /*          BALANCE INQUIRY VALIDATIONS                                   */
        /**************************************************************************/
              if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
                {
                  f_printf(stderr, "BALANCE INQUIRY\n" );
                  memcpy(LocalAmount,paczisoauto.amount_transaction,12);

	#ifdef MRL_204
		BFL_RemiCheck(SubResult);
		if(memcmp(SubResult, "00", 2)!=0)
			return (SubResult);
	#endif
		
                  BFL_AuthScenarioCheck( SubResult, 1 );
                  BFL_BalanceInquiry( BankCode, SubResult ,LocalAmount);
                  return (SubResult);
                }
        /**************************************************************************/
        /*          BALANCE INQUIRY REDEEM POINTS                                 */
        /**************************************************************************/
               if(  memcmp( paczisoauto.processing_code, "21", 2 ) == 0 )
               {
                  memcpy(LocalAmount,paczisoauto.amount_transaction,12);
                  f_printf(stderr, "REDEEM POINT BALANCE INQUIRY\n" );
                  BFL_AuthScenarioCheck( SubResult, 1 );
                  BFL_BalanceInquiry( BankCode, SubResult ,LocalAmount);
                  return (SubResult);
               }
        /**************************************************************************/
        /*          OPEN ECS VALIDATIONS                                          */
        /**************************************************************************/
         if ( OpenECSValidations(SubResult ) != SUCCESS )
         {
                 f_printf(stderr, " ECS VALIDATION FAILURE\n" );
                 return ( SubResult );
         }
        /**************************************************************************/
        /*          SCENARIO CHECK                                                */
        /**************************************************************************/
         if(BFL_AuthScenarioCheck( SubResult, 1 ) != SUCCESS )
         {
                  f_printf(stderr, "BFL_AuthScenarioCheck\n" );
                  return (SubResult);
         }

        /**************************************************************************/
        /*           CHECK  REQUESTED REDEEM POINTS AVAILABLE                     */
        /**************************************************************************/
             if(memcmp( paczisoauto.processing_code, "99", 2 ) == 0 )
              { 
               		BFL_BalanceInquiry( BankCode, SubResult ,LocalAmount);
               		if(BFL_CheckRedeemPointAvailable( SubResult ) != SUCCESS )
                	{
                  		f_printf(stderr, "BFL_CheckRedeemPointAvailable\n" );
                  		return (SubResult);
                	}
                return (SubResult); /* No Need to Check Velocity Risk for Reward Redeem Transaction */
              }

        /**************************************************************************/
        /*           CHECK  PARENT TRANSACTION                                    */
        /**************************************************************************/
               if(relation_type[0] == 'C')
               {
			/* MRL_233 */
                        if(CheckParentRecord(SubResult) !=  SUCCESS )
                        {
                                f_printf(stderr, "\n PARENT TRANSACTION NOT FOUND");
                                return (SubResult);
                        }
               }

        /**************************************************************************/
        /*       TRANSACTION RISK AND VELOCITY   CHECK                            */
        /**************************************************************************/
          /*
         if(Get_Transaction_Risk(Customer_Risk,Asset_Category_Risk,Transaction_Risk) != SUCCESS )
         {
                  f_printf(stderr, "Get_Transaction_Risk\n" );
                  memcpy(SubResult,"60011",6);
                  return (SubResult);
         }
          */
        

         /* START OF CHANGES  MRL-130  FOR LTV RULE ENGINE */

	 memset(cust_open,0x00,sizeof(cust_open));
 	sprintf(cust_open,"%s/LN-CUST",getenv("MultipacDat"));

 	lnamnt = isopen(cust_open,ISINOUT+ISMANULOCK);
	 if(lnamnt < 0 )
	 {
		 f_printf(stderr," LN-CUST ERROR File [%s] Open Error [%d]",cust_open,iserrno);
		 isclose(lnamnt);
		 memcpy(SubResult,"960002",6);
		 return(SubResult);
	 }

	

	memset(&cust_prod_amount, ' ', sizeof(cust_prod_amount));
	memcpy(cust_prod_amount.customerid, bflzbcom.lbcom_customer_id, 19);
	
	if(isread(lnamnt, &cust_prod_amount, ISEQUAL)!=0)
	{
		f_printf(stderr, "ERROR IN reading LN-CUST. ERROR: %d\n", iserrno);
	}
	
	isclose(lnamnt);


         #ifdef RULE_ENGINE
         f_printf(stderr, "\n Before Rule Engine processing \n");
         f_printf(stderr, "\n Rule Count For AUTH :[%d]....\n", authRuleCount );
         f_printf(stderr, "\n Rule Count For LTV :[%d]....\n",  ltvRuleCount );
         f_printf(stderr, "\n Rule Count For VELOCITY :[%d]....\n", velocityRuleCount );
         f_printf(stderr, "\n Rule Count For CIBIL :[%d]....\n", cibilRuleCount );

        /**************************************************************************/
        /*       LTV RULE ENGINE CHECK                                            */
        /**************************************************************************/

        if( (ltvRuleCount > 0) ||
            ( authRuleCount > 0) ||
            ( velocityRuleCount > 0) ||
            ( cibilRuleCount > 0)
          )
	{
		memcpy(ltv_customer_id, bflzbcom.lbcom_customer_id, 19);
		ltv_customer_id[19]='\0';

		f_printf(stderr, "Customer: [%.12s]\n", bflzbcom.lbcom_customer_id);
		memset(ltv_cumul_auth,0,sizeof(ltv_cumul_auth));
		memset(ltv_cumul_ecs,0,sizeof(ltv_cumul_ecs));
		memcpy(ltv_cumul_auth, "000000000000", 12);
		ltv_cumul_auth[12]='\0';
		/* Start Of - Added For Handling "tier classification based on cities" issue */

		/*******Check urban flag in branch master********/

		memset((char *)&paczpa1b , ' ' , sizeof(t_paczpa1b));
		memcpy(paczpa1b.pa1b_notabl, "1B", 2);
		memcpy(paczpa1b.pa1b_code_banque, "00059", 5);
		memcpy(paczpa1b.pa1b_code_agence, bflzbcom.lbcom_branch_id, 20);


		if(isread(fdparam,&paczpa1b,ISEQUAL) < 0)
		{
		f_printf(stderr,"Error In reading Branch Master BFL_RemiCheck, [%d]\n",iserrno);
		memset((char *)&paczpa1b , ' ' , sizeof(t_paczpa1b));
		}
		isunlock(fdparam);
		isrelease(fdparam);

		/* End Of - Added For Handling "tier classification based on cities" issue */

	}

	if( ( ltvRuleCount > 0 ) && ( memcmp( bflzbcom.lbcom_vip_nonvip, "Y", 1 ) != 0 ) ) /*MRL_353*/        
	{
		memset( failedRule, '\0', sizeof( failedRule ) );
		f_printf(stderr, "\n Going To execute Rule For LTV....\n" );
		/*if( RULE_MET <= ( ruleStatus = runRuleEngine( "LT", ruleFldDtls, ltvRuleMaster, failedRule,SubResult) ) )*/
		ruleStatus = runRuleEngine( "LT", ruleFldDtls, ltvRuleMaster, failedRule,SubResult);
		if( ruleStatus <= 0 )
		{
			f_printf(stderr, "\n After runRuleEngine processing for LTV ruleStatus : [%d]\n", ruleStatus );
			f_printf(stderr, "\n Transaction has been failed because of Rule :[%s]\n", failedRule );
			f_printf(stderr, "\n Returning with error - M9-LTV rejects \n" );
			memcpy(SubResult, "M90000", 6);
			return (SubResult);
		}
	}
        else
	{
		if(memcmp( bflzbcom.lbcom_vip_nonvip, "Y", 1 ) == 0 )
			f_printf(stderr, "\n VIP FLAG active for LTV \n");
		else 
			f_printf(stderr, "\n No LTV Rule Defined \n");
	}
        #endif


	if( ( velocityRuleCount > 0 ) && ( memcmp( bflzbcom.lbcom_vip_nonvip, "Y", 1 ) != 0 ) ) /*MRL_353*/
	{
		f_printf( stderr, "\n Going To execute Rule For VELOCITY....\n" );
		memset( failedRule, '\0', sizeof( failedRule ) );
		ruleStatus = runRuleEngine( "VL", ruleFldDtls, velocityRuleMaster, failedRule,SubResult);
		if( ruleStatus <= 0 )
		{
			f_printf(stderr, "\n After runRuleEngine processing for VELOCITY ruleStatus : [%d]\n", ruleStatus );
			f_printf(stderr, "\n Transaction has been failed because of Rule :[%s]\n", failedRule );
			f_printf(stderr, "\n Returning with error - 65-VELOCITY rejects \n" );
			memcpy(SubResult, "650000", 6);
			return (SubResult);
		}

	}
	else
        {
		if(memcmp( bflzbcom.lbcom_vip_nonvip, "Y", 1 ) == 0 )
		{	
			f_printf(stderr, "\n VIP FLAG active for VELOCITY \n");
			Velocity_Check = 1;
		}
		else 
			f_printf(stderr, "\n No VELOCITY Rule Defined \n");
        }


         #ifdef RULE_ENGINE

        /**************************************************************************/
        /*       CIBIL RULE ENGINE CHECK                                          */
        /**************************************************************************/

        f_printf(stderr, "\n Before Cibil bflzbcom.lbcom_vip_nonvip:[%.1s] \n",bflzbcom.lbcom_vip_nonvip);
        if( (cibilRuleCount > 0) && (memcmp(bflzbcom.lbcom_vip_nonvip,"Y",1) != 0))
        {
             memset( failedRule, '\0', sizeof( failedRule ) );
             memset(New_Cibil_Score,0x00,sizeof(New_Cibil_Score));
             memset(New_Cibil_Score_Date,0x00,sizeof(New_Cibil_Score_Date));
             memset(Cibil_Score_Min,0x00,sizeof(Cibil_Score_Min));
             memset(Current_Cibil_Score,0x00,sizeof(Current_Cibil_Score));

             memcpy(Cibil_Score_Min,bflzlpa03.lpa03_min_cibil_score,5);
             memcpy(Current_Cibil_Score,bflzbcom.lbcom_cibil_score+1,5);
             f_printf(stderr, "\n Going To execute Rule for CIBIL....\n" );
             /*if( RULE_MET <= ( ruleStatus = runRuleEngine("CB", ruleFldDtls, cibilRuleMaster, failedRule,SubResult) ) )*/
             ruleStatus = runRuleEngine("CB", ruleFldDtls, cibilRuleMaster, failedRule,SubResult);
             if( ruleStatus <= 0 )
             {
                 f_printf(stderr, "\n After runRuleEngine processing for CIBIL ruleStatus : [%d]\n", ruleStatus );
                 /* MRL-130  IF RULE IS MET NEED TO CALL CIBIL UPDATE */

                 f_printf(stderr, "\n Cibil Call has been caused due to Rule :[%s]\n", failedRule );
                 if(GetUpdatedCibilScore(New_Cibil_Score,New_Cibil_Score_Date,bflzport.lport_card_no) != 0 )
                 {
/* COMMENTED AS DISCUSSED WITH BFL
                     f_printf(stderr, "\n 19. ERROR IN GETTIGN NEW CIBIL SCORE\n") ;
                     #ifdef  NOT_DEFINE
                         memcpy(Subres, "910001", 6);
                         return ( ECHEC );
                     #else
                         return ( SUCCESS );
                     #endif
*/
                 }
                 else
                 {
			/* MRL_353*/
			if( atoi(New_Cibil_Score) < 2 )
			{
				f_printf(stderr,"New_Cibil_Score is less then 2[%d]",atoi(New_Cibil_Score));
				memcpy( New_Cibil_Score,"-09999", 6);
			}
                     f_printf(stderr, "\n AFTER CIBIL TRIGGER LATEST CIBIL SCORE[%d]\n",atoi(New_Cibil_Score)) ;
                     f_printf(stderr, "\n GOING TO UPDATE CIBIL ") ;
                     UpdateCibilScore(New_Cibil_Score,New_Cibil_Score_Date,Cibil_Score_Min,1);
                 }

/* COMMENTED AS DISCUSSED WITH BFL
                 if(atoi(New_Cibil_Score) < atoi(Cibil_Score_Min))
                 {
                     f_printf(stderr, "\n 19. MINIMUM CIBIL SCORE[%d] ,LATEST CIBIL SCORE[%d]\n",atoi(Cibil_Score_Min),atoi(New_Cibil_Score)) ;
                     f_printf(stderr, "\n 19. NEW CIBIL SCORE IS LESS THEN THE MINIMUM CIBIL SCORE SET AT CARD TYPE\n") ;
                     memcpy(SubResult, "D30002", 6);
                     return ( SubResult );
                 }

                 return (SubResult);
*/
             }
/* COMMENTED AS DISCUSSED WITH BFL
             else if((atoi(Current_Cibil_Score) < atoi(Cibil_Score_Min)) || (bflzbcom.lbcom_cibil_score[0] == '-') )
             {
                   f_printf(stderr, "\n Current CIBIL Score is Less [%s]than the Min CIBIL Score Required[%s]\n",Current_Cibil_Score , Cibil_Score_Min);
                   memcpy(SubResult, "D30017", 6);
                   return ( SubResult );
             }
*/
        }
        else
        {
              f_printf(stderr, "\n No CIBIL Rule Defined \n");
        }


        /**************************************************************************/
        /*       AUTH RULE ENGINE CHECK                                           */
        /**************************************************************************/

        if( (authRuleCount > 0) && (memcmp(bflzbcom.lbcom_vip_nonvip,"Y",1) != 0))
        {
            memset( failedRule, '\0', sizeof( failedRule ) );
            f_printf(stderr, "\n Going To execute Rule For Auth....\n" );
            /*if( RULE_MET <= ( ruleStatus = runRuleEngine( "AU", ruleFldDtls, authRuleMaster, failedRule,SubResult) ) )*/
            ruleStatus = runRuleEngine( "AU", ruleFldDtls, authRuleMaster, failedRule,SubResult);
            if( ruleStatus  <= 0 )
            {
                  f_printf(stderr, "\n After runRuleEngine processing for AUTH ruleStatus : [%d]\n", ruleStatus );
                  f_printf(stderr, "\n Transaction has been failed because of Rule :[%s]\n", failedRule );
                  memcpy(SubResult, "D30017", 6);
                  return (SubResult);
            }
        }
        else
        {
              f_printf(stderr, "\n No AUTH Rule Defined \n");
        }


        #endif
		
        /* END OF CHANGES MRL-130  FOR LTV RULE ENGINE */



        /**************************************************************************/
        /*          CIBIL SCORE VALIDATIONS                                       */
        /**************************************************************************/
	/* Commented Under MRL-130
         if ( GetCibilScore(SubResult) != SUCCESS )
         {
                 f_printf(stderr, "CIBIL SCORE VALIDATION IS UNSUCCESSFULL  \n" );
                 return ( SubResult );
         }
	*/



#ifdef BFL_REMI_TO_PL
        /**************************************************************************/
        /*            REMI TO PL VALIDATIONS                                      */
        /**************************************************************************/
        int amountBreachedFlag=0;
        remi_to_pl_sms=0;
	pos_sms_applicable=0;

        f_printf(stderr, "Calling CheckCustProdAmount\n");
        CheckCustProdAmount(&amountBreachedFlag, &remi_to_pl_sms, &pos_sms_applicable);



        if (amountBreachedFlag)
        {
                f_printf(stderr, "REMI TO PL VALIDATION IS UNSUCCESSFULL  \n" );
                memcpy(SubResult, "M70000", 6);
                return ( SubResult );
        }
        if(remi_to_pl_sms)
        {
                f_printf(stderr, "REMI TO PL SMS to be Triggered. Breach Amount [%.12s]\n",
                remi_to_pl_breach_amount);
        }
#endif



#ifdef BFL_TIMEOUT_ISSUE
	time_after = time(0);
	f_printf(stderr, "Time After: %d\n", time_after);

	if(ibfl_txn_threshold>0 && (time_after - time_before)>ibfl_txn_threshold)
	{
		f_printf(stderr, "Transaction Threshold breached. Transaction time: %d seconds,"
			" Threshold seconds: %d\n", 
			time_after - time_before, ibfl_txn_threshold);
		
		return("910006");
	}
#endif

         return("000000");
}


#ifdef MRL_207
BFL_Update_count(char* Response)
{
        if((strlen(Otp_Request_id) > 0 )&&(strlen(Otp_Code) > 0 ))
                BFL_UpdateOtp_Count(Response);
        else
                BFL_UpdatePin_Count(Response);
}
#endif

int BFL_OnusToDispatch_0110 ( char* Response, char* SubResult, \
 t_struct_iso* TxnData )
{
        char            AuthId[7];
        char*           ISOData;                /* MAP + DATA */
        int             TempLen = 0;
        int             AdditionalDataLen = 0;
        int             iRetval, iRetval1, iAcq_flag = 0;
        t_msg_bl        SendMsg;
       
        char            ResponseCode[4];
        char            CardNumber[20];
        char            BankCode[6];
        char            Sup_subresult[7];
        char            bonus_points[15];
    	
        char  		strtemp[150];
    	int 		iTxnType = 0;
        int             ipos  = 0  ;
        char 		card_type = 0;
        char            fees_amt[13];
        char            strfees[100]; 
        char            BFL_2Balance[79];
        char            Redeem_Balance[90];
       
        char 		fld_54[21];
        
        double 		Amount1 = 0.0;
        double 		Bal_Amount = 0.0;
        double 		TxnAmount1 = 0.0;
        double 		dfees_amount = 0.0;
        double 		dconv_percentage = 0.0;
        double 		loc_giftfees = 0.00;
        double 		dcbafee_amt = 0.0;
	char 		decline_tag[512];

        memset(fees_amt, 0x00, sizeof(fees_amt));
        memset ( AuthId, 0, 7 );
        memset ( ResponseCode, 0, 4 );
           

        sprintf( Sup_subresult, "%.6s", SubResult );

    
    	memset(bonus_points, 0x00, sizeof(bonus_points));
    
        f_printf(stderr, "\n BEGIN OF BFL OnusToDispatch_0110 \n" );

        memset ( BankCode, 0, 6 );
        memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );

        ISOData = TxnData->data;

         iTxnType = iFindTxnType( paczisoauto.processing_code );

         f_printf(stderr," Before checking response code ..\n");

       f_printf(stderr, "TEST before Response [%s]\n",Response);
       if ( memcmp ( Response, "RR", 2 ) != 0 )
        {
		BFL_Update0100 ( iTxnType, card_type, Response );
		/* BFL_REMI_TO_PL */
	/*	BFL_Daily_Auth(Response);*/
        }
       f_printf(stderr, "TEST after Response\n");

        /* Update the Loan velocity file */
        if(( memcmp ( Response, "00", 2 ) == 0 )&& 
                 ( memcmp ( paczisoauto.processing_code, "00", 2 ) == 0 ) )
        {
                 iRetval1 = -1;
                 iRetval1 = UpdateLoanVelocLimit_ltv(); /*MRL_208*/
                 f_printf(stderr," Return Value %d ..\n",iRetval1);
        }
        
	/* MRL_207 */
	BFL_Update_count(Response);

        f_printf(stderr, "TEST before BFL_BalanceInquiry\n");
        BFL_BalanceInquiry ( BankCode, SubResult, "000000000000" );

        /* Modified  by ACR/Michel 22-05-2009 - End */
		/* Check the Result code */
        if ( memcmp ( SubResult, "RR", 2 ) == 0 )       /* Repeat Request */
        {
                switch ( TxnData->entete.systeme )
                {
                        /* PLUS or VISA */
                case 'P':
                case 'V':
                        /* MXP Support ID- BR-MXP-0402-0186 : 100204 */
                        if ( InsertFieldVisa ( RESPONSE_CODE, ISOData, \
                         paczau00.au00_cd_raison_refus, &TempLen ) == ECHEC )
                            ReplaceFieldVisa( RESPONSE_CODE, ISOData, paczau00.au00_cd_raison_refus, &TempLen );
                        AdditionalDataLen += TempLen;

                        if ( ( memcmp ( paczau00.au00_cd_raison_refus, "00", 2 ) == 0 ) ||
                                 ( memcmp ( paczau00.au00_cd_raison_refus, "08", 2 ) == 0 ) )
                        {
                                InsertFieldVisa ( AUTHORIZATION_IDENTIFICATION_RESPONSE, \
                                 ISOData, paczau00.au00_num_auto_centre, &TempLen );
                                AdditionalDataLen += TempLen;
                        }
                        break;

                }
        }
        else
        {
                f_printf(stderr, "TEST before GetApprovalCode\n");
                
                f_printf(stderr,"BFL Additional[%.12s]. SubResult [%.2s]\n",
			paczisoauto.additional_amounts, SubResult);

                if( memcmp ( SubResult, "00", 2 ) == 0 )
                GetApprovalCode ( AuthId, Response, TxnData );
                

                /* Check the request is Balance inquiry */
	
                if ( memcmp ( SubResult, "00", 2 ) == 0 ) 
                {
                        if ( ( memcmp ( paczisoauto.processing_code, "01", 2 ) == 0 ) ||
                                 ( memcmp ( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                                 ( memcmp ( paczisoauto.processing_code, "31", 2 ) == 0 ) ||
                                 ( memcmp ( paczisoauto.processing_code, "21", 2 ) == 0 ) ||
                                 ( memcmp ( paczisoauto.processing_code, "99", 2 ) == 0 ) ||
                                  (memcmp(paczisoauto.processing_code, "00", 2 ) == 0)
                           )
                            {

           f_printf(stderr, "TxnData->entete.systeme[%c][%.4s][%.5s][%.5s]\n", TxnData->entete.systeme, paczisoauto.merchant_type, paczpa05.pa05_codbnq, TxnData->entete.user);
                                switch ( TxnData->entete.systeme )
                                {
                                case 'V':
				#ifdef BFL_CUG_CARD 
                                if( (memcmp( TxnData->entete.user, "PosISO", 6 ) == 0 )&& (memcmp ( paczpa05.pa05_codbnq, "00059", 5 ) == 0)
                                     &&( memcmp(Response,"00",2) == 0 ) )
                                {
                                                     if(memcmp(paczisoauto.processing_code, "30", 2) == 0)
                                                     {
                                                        char szlodBalance[25];
                                                        memset(szlodBalance, 0x00, sizeof(szlodBalance));
                                                        memset(Redeem_Balance, 0x00, sizeof(Redeem_Balance));
                                                        memcpy(szlodBalance, paczisoauto.additional_amounts+8, 12);
                                                        f_printf(stderr, "\n 2. Balance Amount [%s]", szlodBalance);
                                                        TempLen = 0;
                                                        InsertFieldVisa (ADDITIONAL_AMOUNTS,ISOData,szlodBalance,&TempLen);
                                                        AdditionalDataLen += TempLen;
                                                        TempLen = 0;
                                                        AddAdditionalTags(Redeem_Balance,BALANCE_INQUIRY_RESPONSE);
                                                        InsertFieldVisa ( RESERVERD_NATIONAL_57, ISOData, Redeem_Balance , &TempLen );
                                                        AdditionalDataLen += TempLen;
                                                     }
                                                     if((memcmp(paczisoauto.processing_code, "21", 2) == 0) || (memcmp(paczisoauto.processing_code, "99", 2) == 0))
                                                     {
                                                        TempLen = 0;
                                                        memset(Redeem_Balance, 0x00, sizeof(Redeem_Balance));
                                                        AddAdditionalTags(Redeem_Balance,REDEEM_RESPONSE);
                                                        f_printf(stderr, "\n 4. Redeem Response DE57 [%.45s]", Redeem_Balance);
                                                        InsertFieldVisa ( RESERVERD_NATIONAL_57, ISOData, Redeem_Balance , &TempLen );
                                                        AdditionalDataLen += TempLen;
                                                     }
                                                     if(memcmp(paczisoauto.processing_code, "00", 2) == 0) 
                                                     {
                                                         AddAdditionalTags(BFL_2Balance,SALE_RESPONSE);
                                                         f_printf(stderr, "\n DE 57 FOR SALE RESPONSE --BFL CUSTOMER NAME[%.75s]", BFL_2Balance);
                                                         InsertFieldVisa ( RESERVERD_NATIONAL_57, ISOData, BFL_2Balance, &TempLen ); 
                                                         AdditionalDataLen += strlen(BFL_2Balance);
                                                    }
                                       }
                                                #endif
                                       break;

                                }
                        }
                }
		else if ( 
				(memcmp ( SubResult, "M9", 2 ) == 0)||
				(memcmp ( SubResult, "65", 2 ) == 0)||
				(memcmp ( SubResult, "D3", 2 ) == 0)
			 )
		{
			
			char len[16];
			f_printf(stderr, "INSERTING RULE ID in Field 59\n");

			/* Send RULE ID if response code is M9 */
			AdditionalDataLen = 0;
			TempLen = 0;

			memset(decline_tag, 0x00, sizeof(decline_tag));
			strcat(decline_tag, "400");
			sprintf(len, "%03d", strlen(failedRule));
			strncat(decline_tag, len, 3);
			strncat(decline_tag, failedRule, strlen(failedRule));

			f_printf(stderr, "Field 63 ONUS [%s][%d]\n", decline_tag, strlen(decline_tag));

			InsertFieldVisa ( RESERVERD_NATIONAL_57, ISOData, decline_tag, &TempLen ); 
			AdditionalDataLen += TempLen;
		}

               switch ( TxnData->entete.systeme )
                {
                case 'V':
                case 'P':
                        {
                        /* Michel - VISA April 08 mandate 12.02.08 begin */
                        char buff[15];
                        char temp[5] ;
                        char *pBuf ;
                        int field_63_len = 0;

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

                        InsertFieldVisa ( DEBIT_PRIVATE_USE, ISOData, buff, &field_63_len);

                        AdditionalDataLen += field_63_len;
                        memcpy ( ResponseCode, Response, 2 );

                        if ( InsertFieldVisa ( RESPONSE_CODE, ISOData, ResponseCode, \
                         &TempLen ) == ECHEC )
                                ReplaceFieldVisa ( RESPONSE_CODE, ISOData, ResponseCode );
                        AdditionalDataLen += TempLen;

                        if( memcmp ( SubResult, "00", 2 ) == 0 )
                        {
                                  InsertFieldVisa ( AUTHORIZATION_IDENTIFICATION_RESPONSE, ISOData, \
                                  AuthId, &TempLen );
                                  AdditionalDataLen += TempLen;
                        }
			
                        break;

                       }
                   }
               }


        /* Prepare the response ISO for sending response to DISPATCH */
        EnteteCpy ( &(SendMsg.mtexte.entete), &( TxnData->entete ) );
        SendMsg.mtype = (long) PIDROUT;
        SendMsg.mtexte.entete.source = getpid ();

        SendMsg.mtexte.entete.lng = AdditionalDataLen + TxnData->entete.lng;

        if ( memcmp ( SubResult, "RR", 2 ) == 0 )
        {
                AccordToHeader ( &(SendMsg.mtexte.entete), \
                 paczau00.au00_num_auto_centre );
                Cd_reponseToHeader ( &(SendMsg.mtexte.entete), \
                 paczau00.au00_cd_raison_refus );
        }
        else
        {
                AccordToHeader ( &(SendMsg.mtexte.entete), AuthId );
		Cd_reponseToHeader ( &(SendMsg.mtexte.entete), ResponseCode );
        }
        memcpy ( SendMsg.mtexte.data, TxnData->data, SendMsg.mtexte.entete.lng );
        memcpy ( SendMsg.mtexte.footer.acq_rrn, TxnData->footer.acq_rrn, 12 );
        memcpy ( SendMsg.mtexte.footer.termid, TxnData->footer.termid, 8 );
		
        /* Get the Response MTI */
        GetResponseMTI ( &SendMsg );
        EnteteCpy ( &( TxnData->entete ), &(SendMsg.mtexte.entete));

        if ( ( memcmp ( Response, "00", 2 ) != 0 ) &&
                 ( CheckSuppressResult ( paczpa05.pa05_codbnq, \
                        paczpa05.pa05_product_code, Sup_subresult ) == SUCCESS ) )
        {
                f_printf(stderr, "SUPRESSING SUB RESULT [%.6s]\n", SubResult );
        }
        else
        {
                if ( SendMsgToInterf ( &SendMsg, bal1 ) < 0 )
                {
                        isunlock ( lnbcom );
                        isunlock ( lnsauto );
                        isunlock ( lnhauto );
                        return ( ECHEC );
                }
        }

        /* Write the Response message to trace */
        DumpMessage ( &( SendMsg.mtexte ), TxnData->entete.systeme );

        /* If the response code is not 'RR'( Repeat request ) Log the auth details*/
	/* Commented - This is now done after writing in RTH file 
        if ( memcmp ( Response, "RR", 2 ) != 0 )
        {
                BFL_WriteToSauto ( &(SendMsg.mtexte.entete), Response );
        }
	*/

    memcpy( bflrthauth.response_code, Response, 2);
    if ( 0 == memcmp(bflrthauth.response_code, "00", 2 ) )
    {
       memcpy ( bflrthauth.sub_result, SubResult, 6 );
       memcpy( bflrthauth.approval_code,AuthId , 6);
    }
    else
    {
       memcpy ( bflrthauth.approval_code, "000000" , 6);
       memcpy ( bflrthauth.sub_result, Response ,2);
       memcpy ( bflrthauth.sub_result+2, "0000",4);
    }
        /* Release the locks */
        isunlock(lnbcli);
        isunlock(lnporte);
        isunlock(lnbcom);
        isunlock(lnsauto);
        isunlock(lnhauto);
        isunlock(fdparam);
        isunlock(lnparam);


        return ( SUCCESS );
}
#endif


#ifdef BFL_REMI_TO_PL
void BFL_Daily_Auth(char *Response)
{
        t_cust_prod_amount cust_prod_amount;
        t_product_master product_master;
        char today_amount[12+1];
        int itoday_amount;
        char txn_emi_amount[12+1];
        int  itxn_emi_amount;
        char offset[16];
	char cust_open[100];
	int lnamnt =0;


        if ((memcmp(Response, "00", 2)==0)||
        (memcmp(Response, "08", 2)==0))
        {

                f_printf(stderr, "Entered BFL_Daily_Auth\n");
                memset(&product_master, ' ', sizeof(product_master));
                memcpy(product_master.product_id, buisness_prod_code, strlen(buisness_prod_code));

                if(isread(lnprod, &product_master, ISEQUAL)<0)
                {
                        f_printf(stderr, "W:Error in reading the Product Master. [%s]\n", buisness_prod_code);
                        return;
                }
                memcpy(offset, product_master.product_offset, 2);
                offset[2]='\0';

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
			GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,1,1);
                        today_amount[12]='\0';
                        itoday_amount = atoi(today_amount);
		
			/*
                        f_printf(stderr, "Today's Amount:           [%.12s]/[%d]\n",
                        cust_prod_amount.product_amount[atoi(offset)].total_amount, itoday_amount);
			*/

                        memcpy(txn_emi_amount, emi_amount, 12);
                        txn_emi_amount[12]='\0';
                        itxn_emi_amount = atoi(txn_emi_amount);

                        f_printf(stderr, "Transaction's EMI Amount: [%.12s]/[%d]\n",
                        txn_emi_amount, itxn_emi_amount);

                        itoday_amount += itxn_emi_amount;
         	               sprintf(today_amount, "%012d", itoday_amount);
                
		      /*  memcpy(cust_prod_amount.product_amount[atoi(offset)].total_amount, today_amount, 12);*/
		
			GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,2,1);
		

			if(remi_to_pl_sms)
			{
				GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,2,2);	
			/*	cust_prod_amount.product_amount[atoi(offset)].sms_flag[0]='Y';*/
				f_printf(stderr, "Block SMS flag set for this customer.\n");
			}

			memcpy(cust_prod_amount.user, "ONUS    ", 8);


			/* MRL_332 */
			if(1 == pos_sms_applicable)
			{
				GetProdAmnt_Offset(offset,&cust_prod_amount,today_amount,2,3);
				/*cust_prod_amount.product_amount[atoi(offset)].pos_flag[0] = 'Y';*/
			}


                        if(isrewrite(lnamnt, &cust_prod_amount)<0)
                        {
                                f_printf(stderr, "Error in rewriting transaction Amount. ");
                                f_printf(stderr, "Customer: [%.12s]\n", cust_prod_amount.customerid);
                        }
                }
                else
                {
			f_printf(stderr, "This should not occur. Please investigate.\n");
			f_printf(stderr, "Error in finding customer for cust id [%.19s], Error: [%d]\n", 
				cust_prod_amount.customerid, iserrno);
                }
		 isunlock( lnamnt );
		 isrelease( lnamnt );
        }
	isclose(lnamnt);
}

#endif


