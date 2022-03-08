
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_utl.c                                          *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 06-Mar-2000                                         *
 * Description         : ONUS utility functions                              *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   28-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   12-Apr-04 A K Azhakesh         CR-MXP-0401-0042                       *
 * 3   08-Jun-04 Saju Joseph          BR-MXP-0405-0045                       *
 * 4   16-Nov-04 Veeresh                                                     *
 *****************************************************************************/
#include <bfl_scheme_master.h>

/* Chowdesha added on 28-Jan-2013 for emv early issuance  */
 #define EMV_EARLY_ISSUANCE  1 

 #define ING_VYSYA_ISSUANCE   1
  #define CHECK_REC_NULL 1 

/*   Chowdesha added for SERV TAX AMT */
  #define  CBA_SERV_TAX_MINUS  1 

/* Added By Nagesh Baswantharao for MUTHOOT_ISS_ACQ2 */ 
  #define MUTHOOT_ISS_ACQ2    1   

/* Chowdesha added for HSBC DOP POS acquiring */
/* #define HSBC_DOP_POS  1 */

/*  Chowdesha added for CBA */
 #define CBA  1   

/* NAGESH BASWANTHARAO ADDED FOR SHELL */
 #define SHELL_PHASE1 1 
 #define AUTHID_DUP   1

/* Chowdesha added on 31-05-2011 for HSBC DOP Phase 2*/
/* #define HSBC_DOP_PHASE2  1 */
/*  Chowdesha/Preema added for IDBI BCHA EHOST */
 #define IDBI_BCHA_EHOST  1  

/* Bharath added for JK CMS APPLY */
 #define JK_CMS_APPLY 1  

/* Bharath added for BOI PHASE2 */
/*  #define BOI_PHASE2  */

    #define BOI_FRM    1
    #define CARD_CONTROL 1
    #define  BFL_CUG_CARD     1
    #define BFL_PHASE2  1
    #define SYNDICATE_PIN_RESET 1
    #define SYNDICATE_EHOST_PIN_RESET 1  /* Added by Amit for Syndicate EHOST ATM Txn on 23 MAR 2017 */
    #define RUPAY_ISSUANCE   1    /* Added by Amit for RUPAY Issuance on 7 july 2016 */

#define MRL_238
#define MRL_324
#define MRL_264
#define MRL_383 1

#define INDCOMBPRO_292

/* MRL_332 */
extern int pos_sms_applicable;

#define BFL_REMI_TO_PL

extern char* BFL_CheckAccountAvailable ( int Type /* 1- Normal 2 - Advice Txn */ );

#ifdef BFL_REMI_TO_PL
        #include <remi_pl_rules.h>
        #include <loan_product_master.h>
        #include <cust_prod_amount.h>

        extern t_remi_pl_rules remi_pl_rules ;
        extern t_product_master product_master ;
        extern t_cust_prod_amount cust_prod_amount ;

        extern char remi_to_pl_breach_amount[];

        extern int lnrules;/*BFL PARAMETERS*/
        extern int lnprod ;/*BFL PARAMETERS*/
      /*  extern int lnamnt ;*//*BFL PARAMETERS*/

#endif

#define MRL_299

#ifdef MRL_238
        int Ecs_Waiver_Sms=0;
#endif

#define MRL_204
#ifdef MRL_204
	int remi_urban_decline = 0;
	#include <paczpa1b.h>
	#include <bflremidealer.h>
	t_paczpa1b paczpa1b;
#endif

#define MRL_304 1
#ifdef MRL_304
	char e_flag[2];
#endif


#define MRL_207 1
#ifdef MRL_207
char OTP_Verify_Flag[2];
#endif
char Limit_Check_Bypass_Flag[2];


#define MRL_287
#ifdef MRL_287
	char mob_num_change_flag[2];
#endif

#define MRL_288 1
#ifdef MRL_288
char rsa_flag[2];
#endif

char txn_mob_num [16];

/* MRL_359 */
char s_tenure[6];
char s_adv_emi_cnt[4];
char s_dbd[6];
char s_procfees[13];
char s_roi[6];
char s_asst_risk[4];
char s_txn_risk[4];
char s_txn_velloc[6];
char s_txn_velloc_days[6];
char s_txn_branch_id[9];
char s_adv_emi_amnt[13];
char s_emi_amnt[13];
char s_cr_pr[11];
char s_mbd[11];
extern char failedRule[7];
char fisr_use_flag[2];

/* Chowdesha added to create the Free Txn record */
 #define CREATE_ONUS_LIMIT  1 
 #define MCI_EMV_ISS 1
 #define JK_EMV_DR 1
 #define BCHA_NEW_BALANCE   1   
 #define INSTAPIN 1
 #define EDENRED 1
 #define EDENRED_NEW 1
 #define EDEN_MCC_CHECK 1 

#define MRL_250

 #define SFLP_FRM   1

 #define LODHA_PALAV 1 

 #define IDBI_ATM_PIN  1
 #define BFL_BAL      1

 #define EDENRED_PIN_BASE_DECLINE 1 /* Added by Amit on 17 Dec 2016 to decline without Pin-EDENRED Pin Based card transaction */

 #define EDENRED_REPORT 1 /* Added by Amit on 7 APR 2017 for PREQ2968 Eden Red Report related Changes */

 #define EDENRED_MOBILE_TXN 1  /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */

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
 #include <paczpa00.h>
 #include <paczpa03.h>
 #include <paczpa05.h>
 #include <paczpa56.h>
 #include <paczpa11.h>
 #include <paczpoau.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczeuro.h>
 #include <paczisoauto.h>
 #include <paczautr.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <define.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <paczresp.h>
 #include <paczsupprs.h>
 #include <iso_aegn.h>
 #include <scenario.h>
 #include <onus.h>
 #include <emvparameter.h>
 #include <saftrns.h>
 #include <paczmlist.h>
 #include "onus_utl.h"
 #include <bflcarddet.h>

#ifdef BFL_CUG_CARD
#include <bflzbcom.h>
#include <bflzport.h>
#include <bflzbcli.h>
#include <bflzparm.h>
#include <bflstring.h>
#include <BFLPROC.h>
#include <bfl_rthauth.h>
#include <bfl_dealer_master.h>
#include <bflauth00.h>
t_procdata procdata;
t_bfl_dealer_master bfl_dealer_master;
t_bfl_dealer_master bfl_dealer_master_card;
extern t_bflzport    bflzport;
extern t_bflzbcom    bflzbcom;
extern t_bflzbcli    bflzbcli;
extern t_bflrthauth  bflrthauth;
extern t_bflauth00   bflauth00;
extern t_bflzlpa20   bflzlpa20;
extern t_bflzlpa03   bflzlpa03;

extern t_bflcarddet  bflcarddet;

#ifdef SFLP_FRM
#include <onuspr05.h>
extern t_onuspr05 onuspr05;
#endif

#ifdef EDENRED_PIN_BASE_DECLINE /* Added by Amit on 17 Dec 2016 to decline without Pin-EDENRED Pin Based card transaction */
#include <paczport.h>
extern t_paczport paczport;
#endif

extern int lnrth;   /*BFL RTH*/
extern int lnbcli ; /*BFL CARD Client file */
extern int lnbcom ; /*BFL CARD ACCOUNT MASTER */
extern int lnporte ;/*BFL CARD MASTER */
extern int lnsauto; /*BFL AUTHORISATION */
extern int lnhauto; /*BFL AUTHORISATION HISTORY */
extern int lnparam; /*BFL PARAMETERS*/
extern int lnmaster;
extern int lncrd;  /*BFL card parameters*/

int AddAdditionalTags(char *, int );
void GetCardHolderPORTName(char *);
int  Open_Dealer_Master(char *);
 int  Read_Card_Dealer_Master();
void UpdateCibilScore(char*, char*,char*,int);
extern int BFL_CheckOpenECSAvailable(char *);
extern int BFL_CheckRecordInMovementAuth ( char* Amount, t_bflauth00* AuthRec, int mti );
int  Card_First_Trans ;
int  chargeslip_first_fee;
#ifdef MRL_383
int rset_card_1st_fee;
#endif
int  Cibil_Done;
int  Ecs_Required;
extern int Mobile_No_Tranx ;
char   parent_rrn[13];
double RoundOffEMI ;
int  No_Match_Case  ;
#endif

#ifdef BFL_BAL
extern void trim_leading(char *, char );
#endif

 #ifdef CARD_CONTROL
 #include <card_control.h>
 #endif

 
#ifdef HSBC_DOP_PHASE2
/*	#include <bankacnt.h>
    #include <paczcard_onlinefee.h>
	#include <pacztrnscodemapp.h>
    #include <paczsubfee.h>
	#include <paczprodfees.h>
	#include <paczfeecode.h>
*/
#endif 
#ifdef CBA
	#include <bankacnt.h>
    #include <paczcard_onlinefee.h>
	#include <pacztrnscodemapp.h>
    #include <paczsubfee.h>
	#include <paczprodfees.h>
	#include <paczcardlimit.h>
	#include <pacztrnslimit.h>
	#include <paczfeecode.h>
	#include <safparam02.h>
	extern 	t_bankacnt bankacnt;  

#endif 

#ifdef CBA
	#define LEN_GL_NUMBER      2
        #define LEN_FEE_AMOUNT    12
	#define SERVICE_TAX_GLNUL 25 
#endif

 #define  CVC2_VER 1 
 #define MUTHOOTGOLD

 /* Chowdesha added on 200209 for VISA MANDATES  */
 /* #define VISAMANDATE   1 */
 
 /* Chowdesha/Preema added on 19-Mar-09 to send 430 */ 
   #define VISAMANDATE_420   1 

/* Chowdesha added for the CMS switch separation */
   #define CMS_EHOST 1   

/* Chowdesha added for PREPAID_CARD  */
   #define PREPAID_CARD  1 

/* Chowdesha added for GIFT CARD */
   #define GIFT_CARD 1 

extern	int		bal1;		/* MsgQID1 */
extern	int		bal2;		/* MsgQID2 */
extern	int 	PIDROUT;

extern	char			ServiceCode[3+1];
extern	char			pvv[5+1];
extern	char			cvv[3+1];
extern	char			cvc2[3+1];
extern	char			PinFormat[3];
extern	char			EffectiveDate[5];
extern	char			CardSecurity[6];
extern	char			LanguageCode[4];
extern	char			User[7];

/* ISAM Data File Descriptors */
extern 	int		fdparam;	/* Parameter file */
extern	int		fdresp;		/* Response Code mapping file */
extern	int		fdsuprs;	/* Subresult suppressing file */

extern char  CODE_SCENARIO_O;


extern 	t_paczisoauto	paczisoauto; /* Authorization(ISO data fields) 
										Structure */
extern	t_paczpa05		paczpa05;		/* Card Issuer details structure */
t_paczpa56		        paczpa56;		/* MCC details - veer */
extern	t_paczpoau		paczpoau;		/* Card holder details structure */
extern	t_pcrzbale		pcrzbale;		/* Account deatils structure */
extern  t_rthauth		rthauth;

/* Function prototypes */
extern char* hsm_verif_csck ( char* CardNumber, char* Security,char* ExpDate, \
 char* ServiceCode );
extern char* hsm_verif_cvv ( char* CardNo, char* cvv,char* ExpDate, \
 char* ServiceCode );

extern char* hsm_verif_cvc2( char* CardNo, char* cvc2,char* ExpDate, \
 char* ServiceCode );

extern char* hsm_verif_pin ( char* PinData, char*, char*, char* CardNo, \
 char*, int len,char*, int l_Pin_Seq);

extern char* hsm_verif_pvv ( char* user, char* CardNo, char* PinData, \
 char* pvv, char* PinFormat );

extern char* hsm_verif_pin_bnp ( char* pin_data, char*, char*, char* CardNo, \
 char*, int, char* DecTable,char* PinValData );
extern int LoadKeys ( char* BankCode, char CardFirstDigit, char pvkIndex );

extern double atof( const char * );  /*Bhargav 19/08/03*/

int CheckCSCK ( char* csck );
int CheckCVV ( char* cvv );
int CheckPVV ( char* pvv );
int VerifyPinBNPBPCL ( char* BankCode, char* SubResult );

  #define TEST_CARD    1   

#ifdef TEST_CARD
#include <posparam03.h>
#endif

#ifdef SHELL_PHASE1 /* HSBC_DOP_POS*/
double gcba_round_amt = 0.0;
double eden_gcba_round_amt = 0.0;
extern double gcba_service_tax;
#endif 
#ifdef CARD_CONTROL
#include <pcrzbcom.h>
extern  t_pcrzbcom pcrzbcom;
#endif
#ifdef BOI_FRM
extern char Orginal_RRN[];
#endif

#ifdef CBA
extern double gcba_fees_amt; 
extern char CBS_LOCAL_AMT [15]; 
extern char CBS_CURR_CODE[5]; 
extern    char gstr_feesdet[250]; 
#endif 

/* Added by Pradnya - 02/04/2013 - Start */

#ifdef EMV_EARLY_ISSUANCE
	extern t_emvparameter emvparameter; 
#endif

#ifdef EDENRED_NEW
        t_acqbankid acqbankid;
#endif

#ifdef BFL_CUG_CARD
char Bfl_Card_Type[3];
char Loan_Value_Ratio[6];
char RedeemPointBalance[11];
char *GetFddDate(char *,char *);
extern char *IncMonth(char * );
extern char *IncYear( char * );
extern int  OpenReadACQCHN(); 
extern char acquiring_channel[];
#endif
#ifdef BFL_PHASE2
extern int Ecs_Waive_Off ; 
#endif
#ifdef EDEN_MCC_CHECK
int WHITELIST_MATCHED = 0 ;
#endif
/* Added by Pradnya - 02/04/2013 - End */

int ReceiveMsgFromQueue ( t_msg_bl *msg_recv )
{
	int result = 0;

	/* Read the request message from Queue */
	result = ReadMsgFromQueue ( (t_msg_blu*)msg_recv, getpid (), \
			   bal1, !MXP_NOWAIT );
	if ( result == -1 )
	{
		return ( ECHEC );
	}
	else
		return ( msg_recv->mtexte.entete.type );
}

void FormatRRNOForMCI ( char* user, t_paczisoauto* isoauto )
{
	int		size = 0;
	int		len = 0;

        if( memcmp ( user, "AGN", 3 ) == 0 )
        {
            if( strlen ( isoauto->retrieval_reference_number) == 0  )
            {
		memset ( isoauto->retrieval_reference_number, '0', 12 );
		size = strlen ( isoauto->acquiring_institution_identification_code );
		if ( size >= 6 )
		{
		   memcpy ( isoauto->retrieval_reference_number, \
		    isoauto->acquiring_institution_identification_code, 6 );
		}
		else
		{
		   len = 6 - size;
		   memcpy ( isoauto->retrieval_reference_number+len, \
		   isoauto->acquiring_institution_identification_code, size );
		}
		memcpy( isoauto->retrieval_reference_number+6, \
		 isoauto->system_trace_audit_number, 6);      
		isoauto->retrieval_reference_number[12] = 0;  
            }
        }

	/* check the user, if user is MCI/EURO form the RRNO else return */
	if ( ( memcmp ( user, "EUROPY", 6 ) == 0 ) ||
		 ( memcmp ( user, "MCI", 3 ) == 0 )  )
	{
		memset ( isoauto->retrieval_reference_number, '0', 12 );
		size = strlen ( isoauto->acquiring_institution_identification_code );
		if ( size >= 6 )
		{
			memcpy ( isoauto->retrieval_reference_number, \
			 isoauto->acquiring_institution_identification_code, 6 );
		}
		else
		{
			len = 6 - size;
			memcpy ( isoauto->retrieval_reference_number+len, \
			 isoauto->acquiring_institution_identification_code, size );
		}
		memcpy( isoauto->retrieval_reference_number+6, \
		 isoauto->system_trace_audit_number, 6);      
		isoauto->retrieval_reference_number[12] = 0;  
	}
}

void DumpMessage ( t_struct_iso* ISOMsg, char Format )
{

	switch ( Format )
	{
	/* VISA Message Format */
	case 'V':
	case 'P': /* Added by Gilu 12Oct05  */
        #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 31 AUG 2016 */
        case 'R':
        #endif
		affiche_message_iso_VISA ( ISOMsg);
		break;

	/* PLUS Message Format */
        /* Gilu as there is no PLUS incoming to ONUS 12Oct05
	case 'P':
		affiche_message_iso_PLUS ( ISOMsg );
		break;
        ****/

	/* MASTER/EURO Message Format */
	case 'M':
	case 'E':
	case 'C':
		affiche_message_iso_MCI ( ISOMsg );
		break;

	/* AEGN Message Format */
	case 'A':
		affiche_message_iso_AEGN ( ISOMsg );
		break;

	default:
		f_printf(stderr, "\nWARNING : Message Format [%c] not supported \n", \
				Format );
	}
}

int GetTCCValue ( char* TCC_Value,char* MCC_Code)
{
   int k=0,Result; 

   memset( (char*)&paczpa56,' ',sizeof(t_paczpa56));
   memcpy( paczpa56.pa56_code_table,"56",2);
   memcpy(paczpa56.pa56_mcc, MCC_Code, 4);
   /* Read the TCC Value from the file **/
   do                                                                    
   {                                                                     
       Result = isread ( fdparam, (char*)&paczpa56, ISEQUAL );        
       f_printf(stderr, " FC-PARAM(TCC-value) Reading times [%d] \n", k );      
       k++;                                                          
   }                                                                     
   while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );                  
   if ( Result < 0 )                                                     
   {
        isrelease( fdparam ); /* V5 theta migration Michel 09.02.2006 */
        isunlock ( fdparam ); /* V5 theta migration Michel 09.02.2006 */ 
        f_printf(stderr, "\nNo TCC value found for this MCC ");
        return ( ECHEC );                                             
   }
   memset(TCC_Value,0,2);
   memcpy(TCC_Value,paczpa56.pa56_tcc,1);                                       
   isrelease( fdparam );                                                      
   isunlock ( fdparam );                                                  

   return( SUCCESS );
}


int VerifyCVV ( char* SubResult, int MTI )
{
	char		BankCode[6];
	char		CardNumber[20];
	char		VeriValue[3];

	memset ( BankCode, 0, 6 );
	memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
	memset ( CardNumber, 0, 20 );
	memcpy ( CardNumber, paczisoauto.primary_account_number, \
	 strlen ( paczisoauto.primary_account_number ) );
	memset ( VeriValue, 0, 3 );
	memset ( SubResult, 0, 7 );

        #ifdef EDENRED_MOBILE_TXN /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */
        if( (memcmp(User,"APPLY",5) == 0) && (memcmp (paczisoauto.processing_code,"00",2 ) == 0) && (memcmp (paczisoauto.pos_entry_mode_code,"01",2 ) == 0) )
        {
                f_printf(stderr,"\nApply Mobile Txn: Skipping CVV Validation");
                return SUCCESS;
        }
        #endif

	/* Set Verification Value to 91 by default */
	memcpy ( VeriValue, "91", 2 );

	/* check the USER if it is AMEX verify CSCK */
	if ( paczisoauto.message_format == 'A' )
	{
/************************************************************************
   Commented by Gilu Alex as Service Code is not required for CSCK verification

		if ( ( CheckCSCK ( CardSecurity ) == SUCCESS ) &&
		 	 ( memcmp ( ServiceCode, "XXX", 3 ) != 0 ) )
************************************************************************/
/* Modified by Gilu Alex to enable CSCK checking if value is present 26-July-04 ******************/
		if ( CheckCSCK ( CardSecurity ) == SUCCESS ) 
		{
			f_printf(stderr, "\n1. Bank code at LoadKeys [%.5s] \n", \
				  BankCode );
			LoadKeys ( BankCode, CardNumber[0], '0' );

			memcpy ( VeriValue, hsm_verif_csck ( CardNumber, CardSecurity, \
			 paczisoauto.date_expiration, ServiceCode), 2 ); 
			switch ( atoi ( VeriValue ) )
			{
			case 91:
				memcpy ( SubResult, "961005", 6 );
				break;

			case 99:
                                f_printf(stderr, "CSCK mismatch...\n");
				memcpy ( SubResult, "961099", 6 );
				break;

			case 30:
				memcpy ( SubResult, "961008", 6 );
				break;

			case 00:
                                f_printf(stderr, "CSCK Verified....\n");
				return ( SUCCESS );
				break;

			default:
				memcpy ( SubResult, "961006", 6 );

			}
			return ( ECHEC );
		}
	}	/* AMEX Card CSCK check end */
	else
	{
		if ( CheckCVV ( cvv ) == SUCCESS )
		{
			if ( ( pvv[0] > '6' ) || ( pvv[0] < '0' ) )
            		{
				memcpy ( rthauth.cvv_status, "30", 2 );
				memcpy ( SubResult, "300001", 6 );
				return ( ECHEC );
            		}
			else
			{
				/* Modify PVV Andra Bank VISA cards which are having same 
				index with different LMKs */
				if ( memcmp ( CardNumber, "453992", 6 ) == 0 )
					pvv[0] = '0';
				if ( memcmp ( CardNumber, "451150", 6 ) == 0 )
					pvv[0] = '2';
				if ( memcmp ( CardNumber, "453995", 6 ) == 0 )
					pvv[0] = '3';
				if ( memcmp ( CardNumber, "557794", 6 ) == 0 )
					pvv[0] = '1';

				f_printf(stderr, "\n2. Bank code at LoadKeys [%.5s] \n",   BankCode );
                         /*** MUDA Veeresh Start ***/
                 if (memcmp(BankCode,"00006",5)==0)
                           pvv[0] = '0'; 
                        /*** MUDA Veeresh End  ***/
				LoadKeys ( BankCode, CardNumber[0], pvv[0] );
				/* Send the request to HSM for Verifying CVV */
				f_printf(stderr, "\nTest Value is [%s] \n",   cvv );
                                #ifdef EMV_EARLY_ISSUANCE
					/* Condition for icvv verification */
					/* Condition to change the service code for chip cards */
					if((memcmp( paczisoauto.pos_entry_mode_code,"05",2)==0)/* ||(memcmp( paczisoauto.pos_entry_mode_code,"07",2)==0)*/) 
					{
					   char loc_servicecode[5];
					   memset(loc_servicecode, 0x00, sizeof(loc_servicecode));
					   f_printf(stderr, "\n EMV early issuance service code changes \n");
					   memcpy(loc_servicecode, "999", 3); 
					   memcpy ( VeriValue, hsm_verif_cvv ( CardNumber, cvv, paczisoauto.date_expiration, loc_servicecode ), 2 );
					}
					else
			     #endif
                             #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 25 Aug 2016 */
                             if((paczisoauto.message_format == 'R') && ((memcmp( paczisoauto.pos_entry_mode_code,"05",2)==0) || (memcmp( paczisoauto.pos_entry_mode_code,"95",2)==0)) )
                             {
                                           char Rupay_src[4];
                                           memset(Rupay_src,0x00,sizeof(Rupay_src));
                                           f_printf(stderr, "\n Rupay Quick EMV  service code changes \n");
                                           memcpy(Rupay_src, "999", 3);
                                           memcpy ( VeriValue, hsm_verif_cvv ( CardNumber, cvv, paczisoauto.date_expiration, Rupay_src), 2 );
                             }
                             else
                             #endif
				memcpy ( VeriValue, hsm_verif_cvv ( CardNumber, cvv, \
				 paczisoauto.date_expiration, ServiceCode ), 2 );
				if ( memcmp ( VeriValue, "00", 2 ) != 0 )
				{
					memcpy ( SubResult, VeriValue, 2 );
					/* Added on 22-07-2011 by Chowdesha/ACR to send back response code as 05 insted of 91 */
					if(memcmp(SubResult, "91", 2) == 0)
						 memcpy(SubResult, "05", 2); 

                                        memcpy ( SubResult+2, "0330", 4 );
					return ( ECHEC );
				}
			}
		}
		else	/* Card is DEBIT, return the Error Code */ 
		{
			if ( paczpoau.poau_sanct_financiere[0] == '0' )
			{
				/* DEBIT CARD should be swiped */
				memcpy ( SubResult, "050001", 6 ); 
				return ( ECHEC );
			}
		}
	}

	return ( SUCCESS );
}


/* Gilu Alex  21-Dec-04 CVV2 additional verification */

int VerifyCVV2Data(char* SubResult)
{
   int iField126len = 0; 
   memset ( SubResult, 0, 7 );

   iField126len = strlen( paczisoauto.reserved_private_126 ) ;
/* Changes done for Retail CUG if FLD 126 contains XLS flag 0/1 then do not check for CVV2 data */
   if((memcmp(paczpoau.poau_cd_banque,"00070",5) == 0)&& iField126len == 1)
     return SUCCESS;

   if( iField126len >=0 )
   {
		if( paczisoauto.reserved_private_126[0] == '2' )
		{
			f_printf(stderr, "CVV2 value is on the card but is  illegible\n");
			memcpy ( SubResult, "050027", 6 );
			return ( ECHEC );
		}
		else if( paczisoauto.reserved_private_126[0] == '9' )
		{
			f_printf(stderr, "CH states that the card has no CVV2 imprint\n");
			memcpy ( SubResult, "050028", 6 );
			return ( ECHEC );
		}

#ifdef CVC2_VER		
		/* Added for CVV2 value chacking 07122010   */
		if( iField126len == 0 )
			return SUCCESS ;

		/* Valid values for 126[0] are 0, 1, 2 and 9 */
		if( paczisoauto.reserved_private_126[0] !=  '0' && paczisoauto.reserved_private_126[0] !=  '1'  )
		{
			f_printf(stderr, " Invalid CVV2 presence indicator ...\n");
			memcpy ( SubResult, "050028", 6 );
			return ( ECHEC );
		}
		if( paczisoauto.reserved_private_126[0] ==  '1'  )
		{
			if( CheckCVC2( cvc2 ) != SUCCESS )
			{
				f_printf(stderr, " CVV2 value is not present ...\n");
				memcpy ( SubResult, "050028", 6 );
				return ( ECHEC );
			}
		}
		/* Added for CVV2/CVC2 value chacking 07122010   */
#endif

	}
	return ( SUCCESS );
} /************** Fucntions added by Veeresh 29/06/05 start ****/

void VerifyCalCVC2(char *Card, char *expdate, char *cvc2)
{
    int icvc2=0,i=0;
   
    for(i=6; i<=15; i++)                    
    { 
        icvc2+= ((int)Card[i]-'0');
    }
    for(i=0; i<=3; i++)                    
    { 
        icvc2+= ((int)expdate[i]-'0');
    }    
    icvc2=icvc2*7;                                      
    sprintf( cvc2, "%03d", icvc2 );                     
    return;
}


 /************** Fucntions added by Veeresh 29/06/05 End   ****/




/* BRAVO  - Gilu Alex 05-May-04. CVC2 verification start */

int VerifyCVC2 ( char* SubResult, int MTI )
{
   char		BankCode[6];
   char		CardNumber[20];
   char		VeriValue[3];
   char		Cal_CVC2[4];
   char		RevExpiryDate[5];

   memset ( BankCode, 0, 6 );
   memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
   memset ( CardNumber, 0, 20 );
   memcpy ( CardNumber, paczisoauto.primary_account_number, \
   strlen ( paczisoauto.primary_account_number ) );
   memset ( VeriValue, 0, 3 );
   memset ( SubResult, 0, 7 );
   if( memcmp( BankCode, "00070", 5 ) ==0 )
   return ( SUCCESS );

   #ifdef EDENRED_MOBILE_TXN /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */
   if( (memcmp(User,"APPLY",5) == 0) && (memcmp (paczisoauto.processing_code,"00",2 ) == 0) && (memcmp (paczisoauto.pos_entry_mode_code,"01",2 ) == 0) )
   {
        f_printf(stderr,"\nApply Mobile Txn: Skipping CVC2 Validation");
        return SUCCESS;
   }
   #endif

   /* Set Verification Value to 91 by default */
   memcpy ( VeriValue, "91", 2 );

/***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, start *****
FPRINTF( stderr, "Inside VerifyCVC2 function - CVC2 [%s]\n", cvc2);
 ***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, end   *****/

   if ( CheckCVC2 ( cvc2 ) == SUCCESS )
   {
      pvv[0] = '0'; /* default value */

      if ( memcmp ( CardNumber, "451150", 6 ) == 0 )
         pvv[0] = '2';
      if ( memcmp ( CardNumber, "453995", 6 ) == 0 )
         pvv[0] = '3';
      if ( memcmp ( CardNumber, "557794", 6 ) == 0 )
         pvv[0] = '1';

       /* Chowdesha/Bharath Added for Canara Bank to fix the CVC2 verification failure */
      if ( memcmp ( CardNumber, "534622", 6 ) == 0 )
         pvv[0] = '1';

      if ( memcmp ( CardNumber, "545422", 6 ) == 0 )
         pvv[0] = '1';

      if ( memcmp ( CardNumber, "516299", 6 ) == 0 )
         pvv[0] = '1';

      #ifdef LODHA_PALAV
	if(memcmp(BankCode, "00087", 5) == 0)
           pvv[0] = '0';
      #endif 

      FPRINTF( stderr, "Inside VerifyCVC2 function Key Index [%c]\n", pvv[0] );
      f_printf(stderr, "\n 3.Bank code at LoadKeys [%.5s] \n", BankCode );
      LoadKeys ( BankCode, CardNumber[0], pvv[0] );

      /* Send the request to HSM for Verifying CVC2/CVV2 */
      /* sevice code is not used for CVC2 verification */

      memcpy( ServiceCode, "000", 3 );

/**** end *****/
/*** For Canara Bank Expiry sent to HSM Server is YYMM fo Expiry REVSD ***/
/*** Veeresh after Discussion with Gilu during CANARA CERT ****/

      #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 1 SEP 2016 */
      if ( paczisoauto.message_format == 'R' )
      {
        memset ( RevExpiryDate,0,5);
        memcpy( RevExpiryDate,paczisoauto.date_expiration+2,2);
        memcpy( RevExpiryDate+2,paczisoauto.date_expiration,2);
        f_printf(stderr,"For RUPAY Bank exp_rev[%.4s] service code[%s] \n",RevExpiryDate,ServiceCode);
        memcpy ( VeriValue, hsm_verif_cvc2( CardNumber, cvc2, \
                 RevExpiryDate, ServiceCode ), 2 );
      }
      else
      #endif
      if ( memcmp(BankCode ,"00003",5)==0)
      {
      	memset ( RevExpiryDate,0,5);     
      	memcpy( RevExpiryDate,paczisoauto.date_expiration+2,2);
      	memcpy( RevExpiryDate+2,paczisoauto.date_expiration,2);
     /*f_printf(stderr,"For Canara Bank card[%.19s] cvc2[%.3s] exp_rev[%.4s] service[%.3s]\n",CardNumber, cvc2,RevExpiryDate, ServiceCode);*/
        memcpy ( VeriValue, hsm_verif_cvc2( CardNumber, cvc2, \
                 RevExpiryDate, ServiceCode ), 2 );
      /************ Only for Canara Bank EXP revsd End   ****/
      }
      else
      {
			f_printf(stderr, "\n Test Se :: [%s]", ServiceCode);
			 memcpy ( VeriValue, hsm_verif_cvc2( CardNumber, cvc2, \
			 paczisoauto.date_expiration, ServiceCode ), 2 );
      }

      if ( memcmp ( VeriValue, "00", 2 ) != 0 )
      { 
         /*** Veeresh 29/06/05 start ***/
         if( ( memcmp(BankCode,"00032",5)==0) || (memcmp(BankCode,"00007",5)==0) 
           )
         {
              VerifyCalCVC2( CardNumber, paczisoauto.date_expiration,Cal_CVC2);
            if ( memcmp( Cal_CVC2, cvc2, 3)!=0)
            { 
             /*** Veeresh 29/06/05 End ***/
                 memcpy ( SubResult, VeriValue, 2 );
                 memcpy ( SubResult+2, "0000", 4 );
                 return ( ECHEC );
            }
          }
          else
          {
                 memcpy ( SubResult, VeriValue, 2 );
                 memcpy ( SubResult+2, "0000", 4 );
                 return ( ECHEC );
          }
      }
      FPRINTF( stderr, "Inside VerifyCVC2 function SubResult [%s]\n",SubResult );
      FPRINTF( stderr, " CVC2/CVV2 Succefully verified ....\n");
   }
   
   return ( SUCCESS );
}

/* BRAVO  - Gilu Alex 05-May-04. CVC2 verification end   */

int checkCardBlock()
{

   if (
		      (  memcmp(paczpoau.poau_cd_banque,"00003",5) == 0  )
        ||  (  memcmp(paczpoau.poau_cd_banque,"00005",5) == 0  )    
        ||  (  memcmp(paczpoau.poau_cd_banque,"00008",5) == 0  ) 
        ||  (  memcmp(paczpoau.poau_cd_banque,"00006",5) == 0  ) 
	   #ifdef HSBC_DOP_PHASE2
        ||  (DoPBank(paczpa05.pa05_codbnq) == TRUE)
	    #endif
      )
   if ( paczpoau.poau_sanct_financiere[0] == '0' )
   {                                              
      f_printf(stderr, " RTH CARD IS BLOCKED " );
      return ( ECHEC );                          
   } 
   return ( SUCCESS );                        
}


int VerifyPVV ( char * SubResult )
{
	char		BankCode[6];
	char		CardNumber[20];
	char		VeriValue[3];

	
        #ifdef EDENRED_MOBILE_TXN /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */
        if( (memcmp(User,"APPLY",5) == 0) && (memcmp (paczisoauto.processing_code,"00",2 ) == 0) && (memcmp (paczisoauto.pos_entry_mode_code,"01",2 ) == 0) )
        {
                f_printf(stderr,"\nApply Mobile Txn: Skipping PVV Validation");
                return SUCCESS;
        }
        #endif

	/* for AMEX there is no PVV Check return SUCCESS */
	if ( paczisoauto.message_format == 'A' )
	{
		f_printf(stderr, "AMEX Card there is no PVV CHECK\n" );
		return ( SUCCESS );
	}
	/* Check for ATM transaction with no PIN data */
	if ( ( memcmp ( paczisoauto.merchant_type, "6011", 4 ) == 0 ) &&
		 ( strlen ( paczisoauto.pin_data) == 0 ) )
	{
		f_printf(stderr, "550004: ERROR ATM TXNS WITH NO PIN DATA\n" );
		memcpy ( SubResult, "550004", 6 );
		return ( ECHEC );
	}

	memset ( BankCode, 0, 6 );
	memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );
	memset ( CardNumber, 0, 20 );
	memcpy ( CardNumber, paczisoauto.primary_account_number, 20 );
	memset ( VeriValue, 0, 3 );
	memset ( SubResult, 0, 7 );

	memcpy ( VeriValue, "00", 2 );
	if ( CheckPVV ( pvv ) == SUCCESS ) 
	{
		memcpy ( PinFormat, "01", 2 );
		if( ( memcmp ( BankCode, "00008", 5 ) == 0 ) ||
		   ( memcmp ( BankCode, "00006", 5 ) == 0 ) /* MUDA */
                  )
			pvv[0] = '0';
		/* Send the verification request to HSM */
		/* Call the Verification method based on Parameter */
		switch ( paczpa05.pa05_pvv_method[0] )
		{
		case '1': /* VISA */
           if ( strlen(paczisoauto.pin_data) != 0 ) 
           {
             if(paczpoau.poau_nb_pin_faux[0] < paczpa05.pa05_pin_try_count[0])
             {                                                                  
		    	memcpy ( VeriValue, hsm_verif_pvv ( User, CardNumber, \
		 	    paczisoauto.pin_data, pvv, PinFormat ), 2 );
             }
             else                                  
             {                                     
                memcpy ( SubResult, "750002", 6 );
                return ( ECHEC );                 
             }                                     
           }
		   break;

		default:
            f_printf(stderr, "PVV Verification is not Valid. Is applicable only for VISA methord\n");
            memcpy ( SubResult, "961009", 6 ); 
            return ( ECHEC );                 
			/* RETURN ERROR */
			break;	
		}
		memcpy ( rthauth.pvv_status, VeriValue, 2 );
		if ( memcmp ( rthauth.pvv_status, "00", 2 ) != 0 )
		{
			memcpy ( SubResult, "550055", 6 );
			return ( ECHEC );
		}
	}
	return ( SUCCESS );
}


int InternationalCard ( char CardUse )
{
	switch ( CardUse )
	{
	case '0': 
		f_printf(stderr, "\nTHE CARD IS AN INTERNATIONAL CARD \n");
		return (  SUCCESS );
              
	case '1': 
		f_printf(stderr, "\nTHE CARD IS A NATIONAL CARD \n");
		return ( ECHEC );
               
	case '2':
		f_printf(stderr, "\nTHE CARD IS GLOBAL CARD \n");
		return ( ECHEC );
	          
	default:
		f_printf(stderr, "\nUNKNOWN CARD \n");
              
	}

	return ( ECHEC );
}
/* #ifdef NEVER */ /* Commented by Amit for RUPAY ISSUANCE on 30 NOV 2016 */
int DomestiCard ( char CardUse )
{
	switch ( CardUse )
	{
	case '0': 
		f_printf(stderr, "\nTHE CARD IS AN INTERNATIONAL CARD \n");
		return (  ECHEC );
              
	case '1': 
		f_printf(stderr, "\nTHE CARD IS A NATIONAL CARD \n");
		return ( SUCCESS );
               
	case '2':
		f_printf(stderr, "\nTHE CARD IS GLOBAL CARD \n");
		return ( ECHEC );
	          
	default:
		f_printf(stderr, "\nUNKNOWN CARD \n");
              
	}

	return ( ECHEC );
}
/* #endif */ /* Commented by Amit for RUPAY ISSUANCE on 30 NOV 2016 */
int IsLocalUsed ( char* CountryCode, char* CodePays )
{
	if (  ( memcmp ( CountryCode, CodePays, 3 ) == 0 ) ||
		  ( memcmp ( CountryCode, "524", 3 ) == 0 )    ||
		  ( memcmp ( CountryCode, "064", 3 ) == 0 ) )
	{
		f_printf(stderr, "\nTHE CARD IS USED INSIDE INDIA/NEPAL/BHUTAN \n" );
		return ( SUCCESS );
	}

	f_printf(stderr, "\nTHE CARD IS USED OUTSIDE INDIA/NEPAL/BHUTAN \n" );
	return ( ECHEC );
}

int CheckCSCK ( char* csck )
{
   /*********Commented by Gilu Alex on 27-07-04 as suggested by Jai -Amex 
	if ( ( memcmp ( csck, "XXXXX", 5 ) == 0 ) ||
		 ( memcmp ( csck, "     ", 5 ) == 0 ) ||
		 ( strlen ( csck ) == 0 ) )
  **********************************/
  /* The following needs to be removed when implementing emergency replacement*/

        if( ( memcmp ( csck, "     ", 5 ) == 0 ) || 
            ( strlen ( csck ) == 0 )
          )
          memcpy ( csck, "00000", 5 );
 /* end of modification - Gilu */
 
	if ( memcmp ( csck, "XXXXX", 5 ) == 0 ) 
	{
		return ( ECHEC );
	}
	else
	{
		return ( SUCCESS );
	}
}


int CheckCVV ( char* cvv )
{
	if ( ( memcmp ( cvv, "XXXXX", 3 ) == 0 ) ||
		 ( memcmp ( cvv, "     ", 3 ) == 0 ) ||
		 ( strlen ( cvv ) == 0 ) )
	{
		return ( ECHEC );
	}
	else
	{
		return ( SUCCESS );
	}
}

/* Project BRAVO - Gilu Alex 08-May-04 start */

int CheckCVC2( char* cvc2 )
{
        /* CVC2 neednot be verified if CVC1 is present */
        /* CVC2 is verified if CVC1 is success 
        if( memcmp( cvv, "XXX", 3 ) != 0 )
        {
           FPRINTF( stderr, "CVC1 present... Hence CVC2 is not verified.\n");
           return ( ECHEC );
        }
         ********************************/

/* FPRINTF( stderr, "Inside CheckCVC2 function - CVC2 [%s]\n", cvc2);*/

	if ( ( memcmp ( cvc2, "XXX", 3 ) == 0 ) ||
		 ( memcmp ( cvc2, "   ", 3 ) == 0 ) ||
		 ( strlen ( cvc2 ) == 0 ) )
	{

FPRINTF( stderr, "Inside CheckCVC2 function - cvc2 value not present/valid\n");
		return ( ECHEC );
	}

	return ( SUCCESS );
}

/* Project BRAVO - Gilu Alex 08-May-04 end   */

int CheckPVV ( char* pvv )
{
	if ( strlen ( pvv ) == 0 )
		return ( ECHEC );
	if ( memcmp ( pvv, "XXXXX", 5 ) == 0 )
		return ( ECHEC );
	return ( SUCCESS );
}

/* CR-MXP-0401-0042 Start */
int GetErrorCodeUsingBlockCode ( char CaptureCode, char* SubResult )
{

	memset ( SubResult, ' ', 6 );

        f_printf(stderr, "CaptureCode [%c]\n", CaptureCode );
	switch ( CaptureCode )
	{

        /* CR-MXP-0401-0042 start */

        case 'R':
	        memcpy ( SubResult, "01000R", 6 );
               return ( SUCCESS ); 
	        break;
        case 'I':
    	       memcpy ( SubResult, "05000I", 6 );
               return ( SUCCESS ); 
 	       break;
        case 'J':
	       memcpy ( SubResult, "05000J", 6 );
               return ( SUCCESS ); 
	       break;
        case 'V':
	       memcpy ( SubResult, "05000V", 6 );
               return ( SUCCESS ); 
	       break;
        case 'O':
	       memcpy ( SubResult, "05000O", 6 );
               return ( SUCCESS ); 
	       break;
        case 'W':
	       memcpy ( SubResult, "05000W", 6 );
               return ( SUCCESS ); 
	       break;
        case 'E':
	       memcpy ( SubResult, "54000E", 6 );
               return ( SUCCESS ); 
	       break;

        /* CR-MXP-0401-0042 end   */

	}
	return ( ECHEC );
}
/* CR-MXP-0401-0042 end   */

int GetErrorCodeUsingCaptureCode ( char CaptureCode, char* SubResult )
{

        char   BankCode[6]; 

        memset ( BankCode, 0, 6 );                   
        memcpy ( BankCode, paczpa05.pa05_codbnq, 5 );	

	memset ( SubResult, ' ', 6 );

	switch ( CaptureCode )
	{

	case ' ':/*BR-MXP-0405-0045*/
	case '0':
		return ( ECHEC );

	case '1':
		memcpy ( SubResult, "410001", 6 );
		break;

	case '2':
		memcpy ( SubResult, "430002", 6 );
		break;

	case '3':
		memcpy ( SubResult, "040003", 6 );
		break;

	case '4':
		memcpy ( SubResult, "040004", 6 );
		break;

	case '5':
		memcpy ( SubResult, "040005", 6 );
		break;

	case '6':
		memcpy ( SubResult, "040006", 6 );
		break;

	case '7':
		memcpy ( SubResult, "070007", 6 );
		break;

        /* CR-MXP-0401-0042 start */

        case 'U':
	       memcpy ( SubResult, "04000U", 6 );
	       break;
        case 'C':
	       memcpy ( SubResult, "04000C", 6 );
	       break;
        case 'Y':
	       memcpy ( SubResult, "04000Y", 6 );
	       break;
        case 'M':
	       memcpy ( SubResult, "04000M", 6 );
	       break;
        case 'Z':
	       memcpy ( SubResult, "04000Z", 6 );
	       break;
        case 'D':
	       memcpy ( SubResult, "04000D", 6 );
	       break;
        case 'L':
	       memcpy ( SubResult, "04000L", 6 );
	       break;
        case 'S':
	       memcpy ( SubResult, "04000S", 6 );
	       break;
        case 'T':
	       memcpy ( SubResult, "04000T", 6 );
	       break;
        case 'F':
	       memcpy ( SubResult, "04000F", 6 );
	       break;
        case 'Q':
	       memcpy ( SubResult, "04000Q", 6 );
	       break;

        default:
	       return ( ECHEC );
	       break;

        /* CR-MXP-0401-0042 end   */

	}

	return ( SUCCESS );
}

int GetErrorCodeUsingReferalCode ( char ReferalCode, char* SubResult )
{

	memset ( SubResult, ' ', 6 );
	switch ( ReferalCode )
	{
	case '1':
		memcpy ( SubResult, "010001", 6 );
		break;

	default:
                memcpy ( SubResult, "410324", 6 );
		break;
	}

	return ( SUCCESS );
}

int GetResponseForCancelAcc ( char ReferalCode, char* SubResult )
{

	memset ( SubResult, ' ', 6 );
	switch ( ReferalCode )
	{
	case '1':
		memcpy ( SubResult, "010003", 6 );
		break;

	default:
		memcpy ( SubResult, "050003", 6 );
		break;
	}

	return ( SUCCESS );
}

int GetErrorCodeUsingReferalCode2 ( char ReferalCode, char* SubResult )
{

	memset ( SubResult, ' ', 6 );
	switch ( ReferalCode )
	{
	case '0':
		memcpy ( SubResult, "510001", 6 );
		break;

	default:
		memcpy ( SubResult, "010002", 6 );
		break;
	}

	return ( SUCCESS );
}

int	GetActualResponseCode ( char* Bank, char* ProductCode, char* SubResult, \
 char* ActualCode )
{
	int 		Result = ECHEC;
	int 		k = 0;
	t_paczresp	paczresp;

	memset ( ActualCode, 0, 6 );	
	memset ( (char*)&paczresp, ' ' , sizeof ( t_paczresp ) );

	memcpy ( paczresp.bank_code, Bank, 5 );
	memcpy ( paczresp.product_code, ProductCode, 5 );
	memcpy ( paczresp.subresult, SubResult, 6 );

	/* Read the Response Code mapping file */
	do
	{
		Result = isread ( fdresp, (char*)&paczresp, ISEQUAL );
        f_printf(stderr, " FC-RESP Reading times [%d] \n", k );
		k++;
	}
	while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
	if ( Result < 0 )
	{
		f_printf(stderr, "\nNo SubResult Mapping for this BIN \n");
		memcpy ( ActualCode, SubResult, 3 );
		return ( ECHEC );
	}
    isrelease( fdresp );
	isunlock ( fdresp );
	
	memcpy ( ActualCode, paczresp.response_code, 3 );
	return ( SUCCESS );
}

int	CheckSuppressResult ( char* Bank, char* ProductCode, char* SubResult )
{
	int 			Result = ECHEC;
	int 			k = 0;
	t_paczsupprs	paczsupprs;

    f_printf(stderr, " CheckSuppressResult () \n" );

	memset ( (char*)&paczsupprs, ' ' , sizeof ( t_paczsupprs ) );

	memcpy ( paczsupprs.bank_code, Bank, 5 );
	memcpy ( paczsupprs.product_code, ProductCode, 5 );
	memcpy ( paczsupprs.subresult, SubResult, 6 );

 
	/* Read the Suppress result code file */
	do
	{
		Result = isread ( fdsuprs, (char*)&paczsupprs, ISEQUAL );
		k++;
	}
	while ( ( iserrno == ELOCKED ) && ( k < NB_FOIS ) );
	isunlock ( fdsuprs );
	if ( Result < 0 )
	{
        
	    f_printf(stderr, "FC-SUPPRES ERROR [%d] \n" , iserrno);
		return ( ECHEC );
	}
	
	f_printf(stderr, "Suppress result code found for Bank[%.5s] \n", \
	 Bank );
	return ( SUCCESS );
}

int GetResponseMTI ( t_msg_bl* IntrfMsg )
{

	switch ( IntrfMsg->mtexte.entete.systeme )
	{
	case 'P':
	case 'V':
        #ifdef RUPAY_ISSUANCE /* Added by Amit for RUPAY ISSUANCE on 31 AUG 2016 */
        case 'R':
        #endif
		switch ( IntrfMsg->mtexte.entete.type )
		{
		case MSG_0220:
		case MSG_0221:
			IntrfMsg->mtexte.entete.type = MSG_0230;
			IntrfMsg->mtexte.data[0] = 0x02;
			IntrfMsg->mtexte.data[1] = 0x30;
			break;

		case MSG_0400:
		case MSG_0401:
			IntrfMsg->mtexte.entete.type = MSG_0410;
			IntrfMsg->mtexte.data[0] = 0x04;
			IntrfMsg->mtexte.data[1] = 0x10;
			break;

		case MSG_0200:
			IntrfMsg->mtexte.entete.type = MSG_0210;
			IntrfMsg->mtexte.data[0] = 0x02;
			IntrfMsg->mtexte.data[1] = 0x10;
			break;

		case MSG_0100:
		case MSG_0101:
			IntrfMsg->mtexte.entete.type = MSG_0110;
			IntrfMsg->mtexte.data[0] = 0x01;
			IntrfMsg->mtexte.data[1] = 0x10;
			break;

                  /*#ifdef VISAMANDATE*/	/* Commented for Visa Mandates 2012 on 09.04.2012*/
                       case MSG_0120:
			  IntrfMsg->mtexte.entete.type = MSG_0130;
			  IntrfMsg->mtexte.data[0] = 0x01;
			  IntrfMsg->mtexte.data[1] = 0x30;
			  break;
                  /*#endif*/ 
                  #ifdef VISAMANDATE_420   
                        case MSG_0420:
			  IntrfMsg->mtexte.entete.type = MSG_0430;
		  	  IntrfMsg->mtexte.data[0] = 0x04;
		  	  IntrfMsg->mtexte.data[1] = 0x30;
		  	  break;
                   #endif  
           
	  #ifdef CMS_EHOST
			case MSG_0600:
				IntrfMsg->mtexte.entete.type = MSG_0610;
				IntrfMsg->mtexte.data[0] = 0x06;
				IntrfMsg->mtexte.data[1] = 0x10;
			    break;  
		  #endif 

		}
		break;

	case 'E':
	case 'M':
	case 'C':
		switch ( IntrfMsg->mtexte.entete.type )
		{
		case MSG_0220:
		case MSG_0221:
			IntrfMsg->mtexte.entete.type = MSG_0230;
			IntrfMsg->mtexte.data[0] = 0xF0;
			IntrfMsg->mtexte.data[1] = 0xF2;
			IntrfMsg->mtexte.data[2] = 0xF3;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_0400:
		case MSG_0401:
			IntrfMsg->mtexte.entete.type = MSG_0410;
			IntrfMsg->mtexte.data[0] = 0xF0;
			IntrfMsg->mtexte.data[1] = 0xF4;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_0200:
			IntrfMsg->mtexte.entete.type = MSG_0210;
			IntrfMsg->mtexte.data[0] = 0xF0;
			IntrfMsg->mtexte.data[1] = 0xF2;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_0100:
			IntrfMsg->mtexte.entete.type = MSG_0110;
			IntrfMsg->mtexte.data[0] = 0xF0;
			IntrfMsg->mtexte.data[1] = 0xF1;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;
		}
		break;

	case 'A':
		switch ( IntrfMsg->mtexte.entete.type )
		{
		case MSG_0220:
		case MSG_0221:
			IntrfMsg->mtexte.entete.type = MSG_0230;
			IntrfMsg->mtexte.data[0] = 0xF0;
			IntrfMsg->mtexte.data[1] = 0xF2;
			IntrfMsg->mtexte.data[2] = 0xF3;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_1400:
			IntrfMsg->mtexte.entete.type = MSG_1410;
			IntrfMsg->mtexte.data[0] = 0xF1;
			IntrfMsg->mtexte.data[1] = 0xF4;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_1100:
			IntrfMsg->mtexte.entete.type = MSG_1110;
			IntrfMsg->mtexte.data[0] = 0xF1;
			IntrfMsg->mtexte.data[1] = 0xF1;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;

		case MSG_1200:
			IntrfMsg->mtexte.entete.type = MSG_1210;
			IntrfMsg->mtexte.data[0] = 0xF1;
			IntrfMsg->mtexte.data[1] = 0xF2;
			IntrfMsg->mtexte.data[2] = 0xF1;
			IntrfMsg->mtexte.data[3] = 0xF0;
			break;
		}
		break;
	}
	
	return ( SUCCESS );
}

int VerifyPinBNPBPCL ( char* BankCode, char* SubResult )
{
	char	PinValData[13];
	char	DecTable[17];


	memset ( PinValData, 0, 13 );
	memset ( DecTable, 0, 17 );

	if ( memcmp ( BankCode, "00022", 5 ) == 0 )	/* BNP CYPRUS */
	{
		memcpy ( PinValData, paczisoauto.primary_account_number+3, 7 );
		memcpy ( PinValData+7, "NFFFF", 5 );
		memcpy ( DecTable, "0123456789012345", 16 );
	}
	else if ( memcmp ( BankCode, "00017", 5 ) == 0 ) /* BPCL CASH */
	{
		memcpy ( PinValData, paczisoauto.primary_account_number, 11 );
		memcpy ( PinValData+11, "N", 1 );
		memcpy ( DecTable, "8351296470461538", 16 );
	}
	else
	{
		memcpy ( PinValData, "ABcDEF01234N", 12 );
		memcpy ( DecTable, "8351296470461538", 16);
	}

	f_printf(stderr, "PIN VERIFICATION START FOR BNP CYPRUS \n" );

	if ( paczpoau.poau_nb_pin_faux[0] <= paczpa05.pa05_pin_try_count[0] )
	{
		if ( memcmp ( BankCode, "00022", 5 ) == 0 )
		{
			if ( memcmp ( (char*)hsm_verif_pin_bnp ( paczisoauto.pin_data, \
									"01", \
									"04", \
									paczisoauto.primary_account_number, \
									paczpoau.poau_offset, \
									16, \
									DecTable, \
									PinValData ), "00", 2 ) != 0 )
			{
				memcpy ( SubResult, "550002", 6 );
				return ( ECHEC );
			}
		}
		else if ( memcmp ( BankCode, "00017", 5 ) == 0 )
		{
			if ( memcmp ( (char*)hsm_verif_pin_bnp ( paczisoauto.pin_data, \
								"01", \
								"04", \
								paczisoauto.primary_account_number, \
								"0000FFFFFFFF", \
								strlen ( paczisoauto.primary_account_number), \
								DecTable, \
								PinValData ), "00", 2 ) != 0 )
			{
				memcpy ( SubResult, "550005", 6 );
				return ( ECHEC );
			}
		}
	}
	else
	{
		memcpy ( SubResult, "750075", 6 );
		return ( ECHEC );
	}

	return ( SUCCESS );
}

int CheckKeyEnterExpiry ( char* ExpiryDate, char* SubResult )
{
	switch ( paczisoauto.message_format )
	{
	case 'A':
		/* AMEX CARD expiry date check is not mandatory */ 
		if ( ( paczisoauto.pos_entry_mode_code[6] == 'V' ) &&
			 ( memcmp ( ExpiryDate, "XXXX", 4 ) == 0 ) )
		{
            memcpy( SubResult, "540001", 6 );
			return ( ECHEC );
		}
		else
		{
			return ( SUCCESS );
		}
		break;

               /* If the expiration date field is not present, then sending 05 - Requested by Ravi Kiran during Orange Certification - Michel - 29.12.04 - Start */

	case 'V':
		if ( ( memcmp ( paczisoauto.pos_entry_mode_code, "01", 2 ) == 0 ) &&
      		     ( memcmp ( ExpiryDate, "XXXX", 4 ) == 0 ) )
		{
		        f_printf(stderr, "\nKEY ENTERED TRANSACTION WITHOUT EXPIRY(FILED 14)\n");
                        memcpy( SubResult, "050002", 6 );
			return ( ECHEC );
		}

               /* If the expiration date field is not present, then sending 05 - Requested by Ravi Kiran during Orange Certification - Michel - 29.12.04 - End */

	
	default:
		if ( ( memcmp ( paczisoauto.pos_entry_mode_code, "01", 2 ) != 0 ) &&
			 ( memcmp ( ExpiryDate, "XXXX", 4 ) == 0 ) )
		{
            memcpy( SubResult, "540002", 6 );
			return ( ECHEC );
		}
	}

	return ( SUCCESS );
}

int GetApprovalCode ( char* ApprovalCode, char* SubResult, \
 t_struct_iso* TxnData )
{
        #ifdef AUTHID_DUP
        char Auth_Bank_Code[6];

        memset(Auth_Bank_Code,0x00,sizeof(Auth_Bank_Code));
        memcpy(Auth_Bank_Code ,TxnData->entete.banque,5);
        #endif

	switch ( TxnData->entete.systeme )
	{
	case 'A':
		if ( ( memcmp ( SubResult, "000", 3 ) == 0 ) ||
			 ( memcmp ( SubResult, "000", 3 ) == 0 ) )
		{
                        #ifdef AUTHID_DUP
                        get_autorisation_onus( ApprovalCode,fdparam ,Auth_Bank_Code);
                        #else
			get_autorisation ( ApprovalCode, fdparam );
                        #endif
		}
		else
			memset ( ApprovalCode, '0', 6 );

		if ( memcmp ( ApprovalCode, "000000", 6 ) != 0 )
		{
			if ( memcmp ( paczpa05.pa05_codbnq, "00012", 5 ) == 0 )
			{
				memcpy ( ApprovalCode, ApprovalCode+4, 2 );
				memcpy ( ApprovalCode+2, "    ", 4 );
			}
			if ( ApprovalCode[0] == '0' )
				ApprovalCode[0] = '9';
		}
		break;

	default:
		if ( ( memcmp ( SubResult, "00", 2 ) == 0 )	||
			 ( memcmp ( SubResult, "00", 2 ) == 0 ) )
		{
                        #ifdef AUTHID_DUP
                        get_autorisation_onus( ApprovalCode,fdparam ,Auth_Bank_Code);
                        #else
			get_autorisation ( ApprovalCode, fdparam );
                        #endif
		}
		else
			memset ( ApprovalCode, '0', 6 );
	}

	return ( SUCCESS );
}






#ifdef BFL_CUG_CARD
int checkCardManualBlock_BFL()
{

   if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
   return ( SUCCESS );
   memset(Bfl_Card_Type,0x00,sizeof(Bfl_Card_Type));
   memcpy(Bfl_Card_Type,bflzport.lport_mxp_card_type,2);
   if (( bflzport.lport_cs_block_flag[0] == 'Y' )&&(memcmp(Bfl_Card_Type,"99",2) != 0 ))
   {
      f_printf(stderr, "\n 7. CARD IS UNDER MANUAL BLOCKED \n" );
      f_printf(stderr, "\n 7. CARD BLOCK REASON [%.30s] \n",bflzport.lport_cs_block_reason );
      return ( ECHEC );
   }
   return ( SUCCESS );
}


int checkOtpPinBlock_BFL(char *SubRes)
{
 #ifdef MRL_207
   if((strlen(Otp_Request_id) > 0 )&&(strlen(Otp_Code) > 0 ))
   {
        /* OTP Block verification */
        if((memcmp(bflzport.lport_cs_block_flag,"O",1)==0) || (memcmp(bflzport.lport_cs_block_flag,"B",1)==0))
        {
                f_printf(stderr, "Card is OTP blocked. [%.1s]\n", bflzport.lport_cs_block_flag);
                memcpy(SubRes,"N00001",6);
                return ( ECHEC);
        }
   }
   else
   {
        /* PIN Block verification */
        if((memcmp(bflzport.lport_cs_block_flag,"P",1)==0) || (memcmp(bflzport.lport_cs_block_flag,"B",1)==0))
        {
                f_printf(stderr, "Card is PIN blocked. [%.1s]\n", bflzport.lport_cs_block_flag);
                memcpy(SubRes,"750008",6);
                return ( ECHEC);
        }
   }
	
	
       return ( SUCCESS );    
  #endif

}


int checkCardSystemBlock_BFL(char *SubRes)
{

   if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
   return ( SUCCESS );

   if ( memcmp(bflzport.lport_cs_block_flag,"Y",1 ) == 0 )
   {
      f_printf(stderr, "\n 8. CARD IS UNDER SYSTEM BLOCKED \n" );
      f_printf(stderr, "\n 8. CARD BLOCK REASON [%.30s] \n",bflzport.lport_cs_block_reason );
       memcpy(SubRes,"C15679",6);
      return ( ECHEC );
   }
   if ( memcmp(bflzport.lport_rto_block_flag,"Y",1 ) == 0 )
   {
      f_printf(stderr, "\n 8. CARD IS UNDER RTO SYSTEM BLOCKED \n" );
      memcpy(SubRes,"J15679",6);
      return ( ECHEC );
   }
  
   if(checkOtpPinBlock_BFL(SubRes) == ECHEC)
    {
        return ( ECHEC);
    }	
  else
       return ( SUCCESS );    
   
}





int checkAccountSystemBlock_BFL()
{
	
	f_printf(stderr, "\n INSIDE checkAccountSystemBlock_BFL \n");
	

	if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
	(  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
		return ( SUCCESS );

	if((memcmp(bflzbcom.lbcom_sys_block_type,"UB",2 )!=0) && (memcmp(bflzbcom.lbcom_sys_block_type,"  ",2 )!=0))
	{
		if((memcmp(bflzbcom.lbcom_sys_block_type,"TB",2 )==0) && (memcmp(bflzbcom.lbcom_sys_block_reason,"High Risk Classified",20 )==0) 
		&& (memcmp(bflzbcom.lbcom_line_type,"1",1 )==0) && ((memcmp(transaction_source,"SI",2) ==0) || (memcmp(transaction_source,"SP",2)==0)) )
		{
			f_printf(stderr,"Bypassing SYS BLOCK for SI-SP [%.2s]\n",bflzbcom.lbcom_sys_block_type);
		}
		else
		{
			f_printf(stderr, "\n 18. ACOUNT IS UNDER SYSTEM BLOCK STATUS [%.2s] \n" ,bflzbcom.lbcom_sys_block_type);
			return ( ECHEC );
		}
	}


	if((memcmp(bflzbcom.lbcom_fraud_block_type,"UB",2)!=0)
		&&(memcmp(bflzbcom.lbcom_fraud_block_type,"  ",2)!=0))
	{
		f_printf(stderr, "\n 18. ACOUNT IS UNDER SYSTEM FRAUD BLOCK STATUS [%.2s] \n" ,bflzbcom.lbcom_fraud_block_type);
		return ( ECHEC );
	}

	#ifdef MRL_288
	f_printf(stderr,"Block type [%.2s], fraud_block_reason [%.19s]",bflzbcom.lbcom_fraud_block_type, bflzbcom.lbcom_fraud_block_reason);

	if(
		((memcmp(bflzbcom.lbcom_fraud_block_type,"TB",2 ) == 0) ||
		(memcmp(bflzbcom.lbcom_fraud_block_type,"PB",2 ) == 0))
		&&
		(memcmp(bflzbcom.lbcom_fraud_block_reason, "TEMPORARY_RCU_BLOCK",19)==0)
	)
	{
		f_printf(stderr, "Transaction is declined due to %.2s \n", bflzbcom.lbcom_fraud_block_type);
		return ( ECHEC );
	}

	/* if((memcmp(rsa_flag,"Y",1) == 0))
	{
		char count[3];
		int icount,Result;

		memcpy(count,bflzbcom.lbcom_rsa_flag_count,2) ;
		count[2]='\0';

		icount = (atoi(count)+1)%100;
		sprintf(count, "%02d", icount);

		memcpy (bflzbcom.lbcom_rsa_flag_count,count,2);
		f_printf(stderr, "\n Rsa_Flag_Count [%.2s] \n" ,count);

		 TB: Tempararyly Blocked 
		memcpy (bflzbcom.lbcom_fraud_block_type,"TB",2 );
		memcpy (bflzbcom.lbcom_fraud_block_reason,"TEMPORARY_RCU_BLOCK",19 );
	 	memcpy(bflzbcom.lbcom_fraud_block_date, (char *)GetDateYYMMDD(), 6);
                memcpy(bflzbcom.lbcom_fraud_block_ibil_DoneCibil_Done/

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
	}  Commented By RISHABH seperate function used UpdateRsaBcom() on 19thJan,2018*/

	#endif


	return ( SUCCESS );
}

int Check_Card_Delivery_Status()
{

   
   if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
   return ( SUCCESS );
    
   if(( bflzport.lport_card_delivery_flag[0] == 'N' )&& (memcmp(Bfl_Card_Type,"99",2) != 0 ))
   {
      f_printf(stderr, "\n 12. DLIVERY STATUS IS NO \n" );
      return ( ECHEC );
   }
   return ( SUCCESS );
}

int Verify_Dealer_Code(char *SubResu)
{
   char Supergroup_Id[3];

   memset(Supergroup_Id,0x00,sizeof(Supergroup_Id));
   memcpy(Supergroup_Id,bfl_dealer_master.super_grp_id,2);
/*
   if((memcmp(bflzport.lport_mxp_card_type,"09",2) == 0 )||(memcmp(bflzport.lport_mxp_card_type,"17",2) == 0 ))
  */
   if (memcmp(bflzport.lport_mxp_card_type,"09",2) == 0 )
   {

      if(Read_Card_Dealer_Master() !=  SUCCESS )
      {
      	memcpy(SubResu, "J30001", 6);
      	return ( ECHEC );
      }
   }
   
   if(memcmp(bflzlpa03.lpa03_cug_check,"Y",1) == 0 )
   {
      f_printf(stderr, "\n CUG CARD :FUTURE GROUP REQUIRMENT: MRL-5\n"); 
      if(memcmp(bflzport.lport_mxp_card_type,Supergroup_Id,2) != 0 )
      {
          f_printf(stderr, "\n DEALER GROUP[%.2s],CARDTYPE[%.2s]\n",Supergroup_Id,bflzport.lport_mxp_card_type); 
          f_printf(stderr, "\n CUG CARD WILL BE ALLOWED ONLY ON THERE RESPECTIVE GROUP DEALERS \n"); 
          memcpy(SubResu, "M10001", 6);
          return ( ECHEC );
      }
   }


   return ( SUCCESS );
}

int Validate_Card_Source(char *SubRes)
{
    char Bfl_Current_Date[9];
    char Card_Creation_Date[9]; 
    char Card_Source_Type[12];
    char transac_amount[13];
    char remi_cnp_amount[13];
    char Rural_product_codes[51];
    char Product_Code_Rural[10];
    int  Diff_in_Days = 0 ;
    int  Diff_in_Year = 0 ;


                memset(transac_amount,0x00,sizeof(transac_amount));
                memset(remi_cnp_amount,0x00,sizeof(remi_cnp_amount));
                memset(Rural_product_codes,0x00,sizeof(Rural_product_codes));
                memset(Product_Code_Rural,0x00,sizeof(Product_Code_Rural));
                memcpy(transac_amount,paczisoauto.amount_transaction,12);
                memcpy(remi_cnp_amount,bflzlpa20.lpa20_remi_amount,12);


    #ifdef BFL_PHASE2
    if(( memcmp(buisness_prod_code,"REMI",4) == 0 )&&(memcmp(bflzport.lport_mxp_card_type,"08",2) == 0))
    {
                f_printf(stderr, "\n  RURAL CARD IS NOT ALLOWED FOR REMI PRODUCT\n" );
                memcpy(SubRes, "L60001", 6);
                return ( ECHEC );
    }

#ifdef MRL_204

	if(memcmp(buisness_prod_code, "REMI", 4)==0)
	{
		memset((char *)&paczpa1b , ' ' , sizeof(t_paczpa1b));
		memcpy(paczpa1b.pa1b_notabl, "1B", 2);
		memcpy(paczpa1b.pa1b_code_banque, "00059", 5);
		memcpy(paczpa1b.pa1b_code_agence, bflzbcom.lbcom_branch_id, 20);


		if(isread(fdparam,&paczpa1b,ISEQUAL) < 0)
		{
			f_printf(stderr,"Error In reading Branch Master [%.*s]/[%d]\n",
			sizeof(paczpa1b), &paczpa1b, iserrno);
			memcpy(SubRes, "M80001", 6);
			return ( ECHEC );
		}
		f_printf(stderr, "Location Active Flag: [%.1s]\n", paczpa1b.pa1b_remi_act_loc);
		isunlock(fdparam);
		isrelease(fdparam);



		if(memcmp(paczpa1b.pa1b_remi_act_loc, "Y", 1)!=0)
		{
			remi_urban_decline=1;
			f_printf(stderr, "\n REMI transactions not allowed on NON-URBAN locations.\n");
			memcpy(SubRes, "M80002", 6);
			return ( ECHEC );
		}
	}

#endif

    if(( memcmp(buisness_prod_code,"ECF",3) == 0 ) &&(memcmp(bflzport.lport_mxp_card_type,"08",2) == 0))
    {
                f_printf(stderr, "\n  RURAL CARD IS NOT ALLOWED FOR ECF PRODUCT\n" );
                memcpy(SubRes, "L70001", 6);
                return ( ECHEC );
    }
    if((memcmp(bflzport.lport_mxp_card_type,"08",2) == 0) && (  memcmp( paczisoauto.processing_code, "00", 2 ) == 0 ) )
    {
                memcpy (Product_Code_Rural+0,"|",1);
                memcpy (Product_Code_Rural+1,buisness_prod_code,strlen(buisness_prod_code));  
                memcpy (Product_Code_Rural+strlen(buisness_prod_code)+1,"|",1);  
      		memcpy (Rural_product_codes,bflzlpa20.lpa20_rural_product_codes,50);
                if(strstr(Rural_product_codes,Product_Code_Rural) == NULL)
		{
			f_printf(stderr,"RURAL CARD WILL NOT ALLOWED FOR THIS PRODUCT CODE[%s]\n",Product_Code_Rural);
			f_printf(stderr,"ALLOWABLE PRODUCTS FOR RURAL CARD [%.50s]\n",bflzlpa20.lpa20_rural_product_codes);
                        memcpy(SubRes, "M60001", 6);
			return ( ECHEC );

		}

    }


    if(((memcmp(buisness_prod_code,"DPF",3)==0) || (memcmp(buisness_prod_code,"MPD",3)==0))
	&&(memcmp(transaction_source,"EP",2) == 0))
    {
       if(strlen(Vas_Amount)== 0 )
       {
                f_printf(stderr,"Vas Amount is not present in the request packet for DPF product on EP channel \n" );
                memcpy(SubRes,"L40000",6);
                return ( ECHEC );
       }
       if(memcmp(Vas_Amount,"            ",12)== 0 )
       {
                f_printf(stderr,"Vas Amount is space  in the request packet for DPF product on EP channel \n" );
                memcpy(SubRes,"L40000",6);
                return ( ECHEC );
       }

       if((strlen(Otp_Code) <= 0 ) || (strlen(Otp_Request_id) <= 0 ) )
       {
                f_printf(stderr," OTP Request ID /OTP Code is not present \n" );
                memcpy(SubRes,"L40000",6);
                return ( ECHEC );
       }

    }


/* Commented to improve performance
    if(( memcmp(buisness_prod_code,"ECF",3) == 0 ) || ( memcmp(buisness_prod_code,"ERF",3) == 0 ))
    {
                if((atof(transac_amount)/100.00) > 125000.00)
                {
                	f_printf(stderr, "\n REQUEST AMOUNT IS MORE THAN THE  ECF TRANSACTION MAX CAP  \n" );
                	memcpy(SubRes, "L80000", 6);
                	return ( ECHEC );
                }
    }
*/
    #endif



/* Commented by Ankit to improve performance 
    memset(Bfl_Current_Date,0x00,sizeof(Bfl_Current_Date));
    memset(Card_Creation_Date,0x00,sizeof(Card_Creation_Date));
    memset(Card_Source_Type,0x00,sizeof(Card_Source_Type));
    memcpy(Card_Source_Type,bflzport.lport_src_type,10);
    memcpy(Bfl_Current_Date,(char *)GetDateYYYYMMDDHHMMSS() + 0,8);
    memcpy(Card_Creation_Date+0,"20",2);
    memcpy(Card_Creation_Date+2,bflzport.lport_card_crea_dt,6);
    Diff_in_Days = DateDifferenceInDays(Bfl_Current_Date,Card_Creation_Date);

    if(((memcmp(Card_Source_Type, "B2C",3) == 0 ) || (memcmp(Card_Source_Type,"B2CSMS",6) == 0 )||(memcmp(Card_Source_Type,"B2CVOICE",8) == 0 ))&&(Diff_in_Days >= 0 ))
    {
        #ifdef MAIL_24052016
   	if(Diff_in_Days <= 75)
   	{
      		f_printf(stderr, "\n 17. CARD SOURCE TYPE B2C,B2CSMS,B2CVOICE CURRRENT DATE -CARD CREATION DATE   =< 75 [%d]\n",Diff_in_Days );
                memcpy(SubRes, "D10001", 6);
      		return ( ECHEC );
   	}
        #endif
    } 
*/

    if(Verify_Dealer_Code(SubRes) != SUCCESS)
    {
      		f_printf(stderr, "\n RULE 6.DEALER CODE IS NOT MATCHING FOR DDF CARD TYPE\n" );
      		return ( ECHEC );
    }


   return ( SUCCESS );
}

int OpenECSValidations(char *SubResult)
{
   char BFL_Buisness_Product_Code[11];
   char Open_Ecs_Expiry_Date[9];
   char Open_Ecs_Expiry_Date_Card_Type[9];
   char openecsflag[2];
   char new_date[9];
   char Bfl_Merchantid[16];
   char Bfl_Terminalid[9];
   int  Pine_Lab_Ecs = 0;

        memset(BFL_Buisness_Product_Code,0x00,sizeof(BFL_Buisness_Product_Code));
        memset(Open_Ecs_Expiry_Date,0x00,sizeof(Open_Ecs_Expiry_Date));
        memset(Open_Ecs_Expiry_Date_Card_Type,0x00,sizeof(Open_Ecs_Expiry_Date_Card_Type));
        memset(openecsflag,0x00,sizeof(openecsflag));
        memset(new_date,0x00,sizeof(new_date));
        memset(Bfl_Merchantid,0x00,sizeof(Bfl_Merchantid));
        memset(Bfl_Terminalid,0x00,sizeof(Bfl_Terminalid));
        memcpy(Open_Ecs_Expiry_Date+0,"20",2);
        memcpy(Open_Ecs_Expiry_Date+2,bflzbcom.lbcom_ecs_expiry_date,6);
        memcpy(BFL_Buisness_Product_Code,buisness_prod_code,strlen(buisness_prod_code));
        memcpy(openecsflag,bflzbcom.lbcom_open_ecs_flag,1);
        memcpy(Open_Ecs_Expiry_Date_Card_Type,bflzlpa20.lpa20_ecs_exp_date,8);
        memcpy(Bfl_Merchantid,paczisoauto.card_acceptor_identification_code,15);
        memcpy(Bfl_Terminalid,paczisoauto.card_acceptor_terminal_identification,8);
        Ecs_Required = 0;

   if(memcmp(paczisoauto.processing_code,"00",2) != 0)
   return ( SUCCESS );
        OpenReadACQCHN();

  	if(memcmp(transaction_source,"EP",2) == 0)
  	{
     			if(memcmp(acquiring_channel,"03",2) == 0 )
     			{
     				f_printf(stderr, "\n ECS CHECKING IS REQUIRED FOR THE TRANSACTIONS FROM PINELABS [%.2s]\n" ,acquiring_channel);
        			Pine_Lab_Ecs = 1;
     			}
  	}

   f_printf(stderr, "\n BFL_Buisness_Product_Code [%s]\n" ,BFL_Buisness_Product_Code);


   if(BFL_CheckOpenECSAvailable(SubResult) != SUCCESS )
   {
      f_printf(stderr, "\n 15. Open ECS Available is less  REMI and E-COM BFL Buisness Product Code\n" );
      if(!(( memcmp(BFL_Buisness_Product_Code,"REMI",4) == 0 )|| ( memcmp(BFL_Buisness_Product_Code,"WLT",3) == 0 )|| (memcmp(BFL_Buisness_Product_Code,"ERF",3) == 0) ||(memcmp(BFL_Buisness_Product_Code,"ECF",3) == 0) || (Pine_Lab_Ecs == 1)))
      {
          Ecs_Required = 1;
          return ( SUCCESS );
      }

      #ifdef BFL_PHASE2
   	if(memcmp(bflzbcom.lbcom_ecs_wave_off,"Y",1) == 0
#ifdef INDCOMBPRO_292
	/*
	 * Waive-Off functionality for only REMI/ECF/PINELABS-REMI/PINELABS-ECF products.
	 */
	&&
	(
		(memcmp(BFL_Buisness_Product_Code,"REMI",4)==0)||
		(memcmp(BFL_Buisness_Product_Code,"ERF",3)==0)||
		(memcmp(BFL_Buisness_Product_Code,"ECF",3)==0)||
		(memcmp(BFL_Buisness_Product_Code,"WLT",3)==0)||
		(
			Pine_Lab_Ecs==1 &&
			(
				(memcmp(BFL_Buisness_Product_Code,"REMI",4)==0)||
				(memcmp(BFL_Buisness_Product_Code,"ERF",3)==0)||
				(memcmp(BFL_Buisness_Product_Code,"WLT",3)==0)||
				(memcmp(BFL_Buisness_Product_Code,"ECF",3)==0)
			)
		)
	)
#endif
 )
   	{

     		f_printf(stderr, "\n  Open ECS Available Check is Waived Off\n" );
     		Ecs_Waive_Off = 1;
#ifdef MRL_238
                Ecs_Waiver_Sms = 1;
#endif
     		return ( SUCCESS );
   	}
   	#endif

      return ( ECHEC );
   }

   GetEcsTenureDate(new_date);

   if(memcmp(Open_Ecs_Expiry_Date,new_date,8) < 0 )
   {
      f_printf(stderr, "\n 14. Open ECS Expiry Date is Less then the Open ECS Expiry Date After Tenure - Advance Emi Count \n" );
      f_printf(stderr, "\n  Open ECS Expiry Date [%s], Open ECS Expiry Date After Tenure - Advance Emi Count  [%s] \n",Open_Ecs_Expiry_Date,new_date);
      if(!(( memcmp(BFL_Buisness_Product_Code,"REMI",4) == 0 )|| ( memcmp(BFL_Buisness_Product_Code,"WLT",3) == 0 )|| (memcmp(BFL_Buisness_Product_Code,"ERF",3) == 0) ||(memcmp(BFL_Buisness_Product_Code,"ECF",3) == 0) || (Pine_Lab_Ecs == 1)))
      {
          Ecs_Required = 1;
          return ( SUCCESS );
      }
  
      #ifdef BFL_PHASE2
   	if(memcmp(bflzbcom.lbcom_ecs_wave_off,"Y",1) == 0 )
   	{

     		f_printf(stderr, "\n  Open ECS Available Check is Waived Off\n" );
     		Ecs_Waive_Off = 1;
#ifdef MRL_238
                Ecs_Waiver_Sms = 1;
#endif
     		return ( SUCCESS );
   	}
      #endif

      memcpy(SubResult,"K95679",6);
      return ( ECHEC );
   }

   if(( memcmp(BFL_Buisness_Product_Code,"REMI",4) == 0 )|| ( memcmp(BFL_Buisness_Product_Code,"WLT",3) == 0 )||(memcmp(BFL_Buisness_Product_Code,"ERF",3) == 0) ||(memcmp(BFL_Buisness_Product_Code,"ECF",3) == 0)|| (Pine_Lab_Ecs == 1))
   ;
   else
   return ( SUCCESS );

   if(memcmp(Open_Ecs_Expiry_Date,Open_Ecs_Expiry_Date_Card_Type,8) < 0 )/*******************/
   {
      f_printf(stderr, "\n 14. Open ECS Expiry Date is Less then the Open ECS Expiry Date at Card Type  for REMI and E-COM BFL Buisness Product Code\n" );
      f_printf(stderr, "\n  Open ECS Expiry Date [%s], Open ECS Expiry Date at Card Type [%s] \n",Open_Ecs_Expiry_Date,Open_Ecs_Expiry_Date_Card_Type );

       #ifdef BFL_PHASE2
        if(memcmp(bflzbcom.lbcom_ecs_wave_off,"Y",1) == 0 )
        {

                f_printf(stderr, "\n  Open ECS Available Check is Waived Off\n" );
#ifdef MRL_238
                Ecs_Waiver_Sms = 1;
#endif
                Ecs_Waive_Off = 1;
                return ( SUCCESS );
        }
      #endif

      memcpy(SubResult,"K95679",6);
      return ( ECHEC );
   }

   
   return ( SUCCESS );
}


int checkCardActivation_BFL(char *SubRe)
{
   char systemdate[9];
   char cardactdate[9];
   memset(systemdate,0x00,sizeof(systemdate));
   memset(cardactdate,0x00,sizeof(cardactdate));
   memcpy(systemdate,(char *)GetDateYYYYMMDD(),8);
   memcpy(cardactdate,"20",2);
   memcpy(cardactdate+2,bflzport.lport_activation_dt,6);
   if(memcmp(cardactdate ,systemdate,8) > 0) 
   {
      f_printf(stderr, "\n 11. Card Activation date is Future date then the Current System date\n" );
      memcpy (SubRe, "C40002", 6);
      return ( ECHEC );
   }

    if((  memcmp( paczisoauto.processing_code, "30", 2 ) == 0 ) ||
                 (  memcmp( paczisoauto.processing_code, "31", 2 ) == 0 ))
   return ( SUCCESS );
   
   if(bflzport.lport_activation_flag[0] != 'Y') 
   {
      f_printf(stderr, "\n  Card  is in inactive mode %.1s\n" ,bflzport.lport_activation_flag);
      memcpy (SubRe, "C40003", 6);
      return ( ECHEC );
   }

   return ( SUCCESS );
}

#ifdef MRL_204

int BFL_RemiCheck(char* subresult)
{
	char  file_open[50];
	int  fd_remi_del;
	t_bflremidealer bflremidealer;

	memset(&bflremidealer,' ', sizeof(bflremidealer));
	memcpy(subresult, "000000", 6);

	/****opening REMI-DEALERS.DAT******/

	memset(file_open,0x00,sizeof(file_open));
	sprintf(file_open,"%s/REMI-DEALERS",getenv("MultipacDat"));

	fd_remi_del = isopen(file_open,ISINOUT+ISMANULOCK);
	if(fd_remi_del < 0 )
	{
		f_printf(stderr,"File [%s] Open Error [%d]",file_open,iserrno);
		memcpy(subresult, "M80006", 6);
		return(ECHEC);
	}

	/****Check if MID is present in REMI-DEALERS.DAT******/
	memset(&bflremidealer, ' ', sizeof(bflremidealer));
	memcpy(bflremidealer.mid, bfl_dealer_master.mid, 15);
	if(isread(fd_remi_del, &bflremidealer, ISEQUAL)<0)
	{
		/* Not a Remi Transaction. No Need to validate the branch master */
		f_printf(stderr, "No MID found in REMI-DEALERS [%.15s]\n", (bfl_dealer_master.mid));
		memcpy(subresult, "000000", 6);
		isclose(fd_remi_del);
		return ( ECHEC );
	}

	/*******Check urban flag in branch master********/

	memset((char *)&paczpa1b , ' ' , sizeof(t_paczpa1b));
	memcpy(paczpa1b.pa1b_notabl, "1B", 2);
	memcpy(paczpa1b.pa1b_code_banque, "00059", 5);
	memcpy(paczpa1b.pa1b_code_agence, bflzbcom.lbcom_branch_id, 20);


	if(isread(fdparam,&paczpa1b,ISEQUAL) < 0)
	{
		f_printf(stderr,"Error In reading Branch Master BFL_RemiCheck, [%d]\n",iserrno);
		memcpy(subresult, "M80004", 6);
		isclose(fd_remi_del);
		return ( ECHEC );
	}
	isunlock(fdparam);
	isrelease(fdparam);

	f_printf(stderr, "Urban Flag: [%.1s]\n", paczpa1b.pa1b_remi_act_loc);
	if(memcmp(paczpa1b.pa1b_remi_act_loc,"Y",1) != 0)
	{
		remi_urban_decline=1;
		f_printf(stderr, "\n  RURAL CARD IS NOT ALLOWED FOR REMI PRODUCT: BFL_RemiCheck\n" );
		memcpy(subresult, "M80005", 6);
		isclose(fd_remi_del);
		return ( ECHEC );
	}

	isclose(fd_remi_del);
	return (SUCCESS);
}
#endif

int BFL_BalanceInquiry ( char* BankCode, char* SubResult, char *loc_amt )
{
        char            AccBalance[13];

        memset ( AccBalance, 0, 13 );
        memset ( RedeemPointBalance, 0, 11 );

        memset ( paczisoauto.additional_amounts, 0, 21 );

        memcpy ( paczisoauto.additional_amounts, paczisoauto.processing_code+2, 2 );
        memcpy ( paczisoauto.additional_amounts+2, "02", 2 );
        memcpy ( paczisoauto.additional_amounts+4, "356", 3 );
        memcpy ( paczisoauto.additional_amounts+7, "C", 1 );

        if((  memcmp( paczisoauto.processing_code, "21", 2 ) != 0 )&& (  memcmp( paczisoauto.processing_code, "99", 2 ) != 0 ))
        {
        	memcpy ( AccBalance, (char*)BFL_GetBalance( &bflzbcom), 12 );
        	memcpy ( Avail_Amount,AccBalance,12);
        }
        memcpy ( RedeemPointBalance, (char*)BFL_GetRedeemBalance( &bflzbcom), 10 );

        if(  memcmp( paczisoauto.processing_code, "21", 2 ) != 0 )
        f_printf(stderr, "\nBFL -BFL_BalanceInquiry function AccBalance        : [%.12s] \n", AccBalance);
        f_printf(stderr, "\nBFL -BFL_BalanceInquiry function RedeemPointBalance: [%.10s] \n", RedeemPointBalance);

        if( memcmp ( BankCode, "00059", 5 ) == 0 )
        {
                  CODE_SCENARIO_O = SCENARIO_BALE;
                  f_printf(stderr, "\n BFL  CARD CODE_SCENARIO_O [%d]\n",CODE_SCENARIO_O);
                  switch (CODE_SCENARIO_O)
                  {

                    case SCENARIO_BALE:
                            f_printf(stderr,"\n ACCOUNT BALANCE [%.12s]\n",AccBalance);
                            f_printf(stderr,"\n ACCOUNT BALANCE REDDEM POINTS [%.8s]\n",RedeemPointBalance);
                            if(atof(AccBalance) < 0.0 )
                            memcpy (AccBalance,"000000000000",12);
                            memcpy (paczisoauto.additional_amounts+8, AccBalance, 12 );
                            break;
                     default :
                            f_printf(stderr,"\n INVALID CHOICE\n");
                            break;
                  }
        }
        return ( SUCCESS );
}

int BFL_AuthScenarioCheck ( char* Result, int Type )
{
   char         CardResult[7];
   char         AccountResult[7];

   memset ( Result, 0, 7 );
   memset ( CardResult, '0', 6 );
   memset ( AccountResult, '0', 6 );
   CODE_SCENARIO_O = SCENARIO_BALE;
   switch ( CODE_SCENARIO_O )
   {
      case SCENARIO_POAU:               /* Card available check */
                           f_printf(stderr, "\n SCENARIO CARD-POAU. ERROR-NEEDS TO BE CHECKED\n" );
                           break;

      case SCENARIO_BALE:               /* Account Available check */
                           f_printf(stderr, "\n BFL SCENARIO ACCOUNT MASTER \n" );
                           memcpy ( AccountResult, BFL_CheckAccountAvailable ( Type ), 6 );
                           memcpy ( Result, AccountResult, 6 );
                           break;

                 default:
                                     f_printf(stderr, "\n INVALID AUTH SCENARIO I [%c] \n", CODE_SCENARIO_O);
                                     memcpy ( Result, "961010", 6 );
   }
   /* Copy the Card Available and Account Available Result to RTH AUTH */
   memcpy ( rthauth.poau_result, CardResult, 2 );
   memcpy ( rthauth.bale_result, AccountResult, 2 );

   CODE_SCENARIO_O = SCENARIO_BALE;
   if ( memcmp ( Result, "00", 2 ) != 0 )
   {
      return ( ECHEC );
   }
   else
      return ( SUCCESS );
}


int taglenvalue(char *str,int Value)
{

int i = 0;
int paisa = 0;

char length[4];
char tag[4];
char *s = str;
char *p = str;

while(i<strlen(str))
{
   int local_data =0;
   memset(tag,0x00,sizeof(tag));
   memset(length,0x00,sizeof(length));
   f_printf(stderr,"BFL STRING [%s]\n",s);
   memcpy(tag,s,3);
   s+=3;
   memcpy(length,s,3);
   s+=3;
   f_printf(stderr,"TAG  %s\n",tag);

   switch(atoi(tag))
   {
     case 51:

            memcpy(transaction_source,s,atoi(length));
            f_printf(stderr,"Tag= Length= Transaction_source Value:%d=%d==%s\n",atoi(tag),atoi(length),transaction_source);
            break;
     case 52:

            memcpy(cell_number,s,atoi(length));
            f_printf(stderr,"Tag= Length= Cell_number Value:%d=%d==%s\n",atoi(tag),atoi(length),cell_number);
            break;


     case 20:

            memcpy(manufacture_id,s,atoi(length));
	    f_printf(stderr,"Tag= Length= manufacture_id Value:%d=%d==%s\n",atoi(tag),atoi(length),manufacture_id);
            break;

     case 26:

            memcpy(asset_cat,s,atoi(length));
            f_printf(stderr,"Tag= Length= asset_cat Value:%d=%d==%s\n",atoi(tag),atoi(length),asset_cat);
            break;

     case 36:

            memcpy(model_id,s,atoi(length));
            f_printf(stderr,"Tag= Length= model_id Value:%d=%d==%s\n",atoi(tag),atoi(length),model_id);
            break;

     case 1:

            memcpy(scheme_id,s,atoi(length));
            f_printf(stderr,"Tag = Length=scheme_id Value:%d=%d==%s\n",atoi(tag),atoi(length),scheme_id);
            break;

     case 5:

            memcpy(buisness_prod_code,s,atoi(length));
            f_printf(stderr,"Tag=Length= Buisness_prod_code Value:%d=%d==%s\n",atoi(tag),atoi(length),buisness_prod_code);
            break;

     case 54:

            memcpy(imei_ser_no,s,atoi(length));
            f_printf(stderr,"Tag= Length= IMEI_ser_no Value:%d=%d==%s\n",atoi(tag),atoi(length),imei_ser_no);
            break;

     case 55:
            memcpy(access_code,s,atoi(length));
            f_printf(stderr,"Tag= Length= Access_code Value:%d=%d==%s\n",atoi(tag),atoi(length),access_code);
            break;

     case 57:
	    memset(rsa_flag, 0x00,sizeof(rsa_flag));
	    memcpy(rsa_flag,s,atoi(length));
	    f_printf(stderr,"Tag= Length = rsa_flag value:%d=%d==%s\n",atoi(tag),atoi(length),rsa_flag);
	    break;

     case 58:
            memcpy(promo_code,s,atoi(length));
            f_printf(stderr,"Tag= Length= Promo_code Value:%d=%d==%s\n",atoi(tag),atoi(length),promo_code);
            break;

     case 59:
            memcpy(addr_change_falg,s,atoi(length));
	      f_printf(stderr,"Tag= Length= Adress_change Value:%d=%d==%s\n",atoi(tag),atoi(length),addr_change_falg);
            break;

#ifdef MRL_287
     case 97:
	    memcpy(mob_num_change_flag,s,atoi(length));
	    f_printf(stderr,"Tag= Length=Mobile_number_change Value:%d=%d==%s\n",atoi(tag),atoi(length),mob_num_change_flag);
	    break;
#endif

		
     case 60:
            memcpy(relation_type,s,atoi(length));
            f_printf(stderr,"Tag= Length= Relationship_type Value:%d=%d==%s\n",atoi(tag),atoi(length),relation_type);
            break;

     case 201:
            memcpy(customer_id,s,atoi(length));
            f_printf(stderr,"Tag= Length= customer_id Value:%d=%d==%s\n",atoi(tag),atoi(length),customer_id);
            break;

     case 202:
            memcpy(emi_card_fee,s,atoi(length));
            f_printf(stderr,"Tag= Length= emi_card_fee Value:%d=%d==%s\n",atoi(tag),atoi(length),emi_card_fee);
            break;

     case 203:
            memcpy(ecs_mandate,s,atoi(length));
            f_printf(stderr,"Tag= Length= ecs_mandate Value:%d=%d==%s\n",atoi(tag),atoi(length),ecs_mandate);
            break;
     case 82:
            memcpy(Requested_reward_point,s,atoi(length));
            f_printf(stderr,"Tag= Length= Requested_reward_point Value:%d=%d==%s\n",atoi(tag),atoi(length),Requested_reward_point);
            break;
     #ifdef BFL_PHASE2
     case 87:
            memset(Otp_Request_id,0x00,sizeof(Otp_Request_id));
            memcpy(Otp_Request_id,s,atoi(length));
            f_printf(stderr,"Tag= Length= OTP REQUEST ID value:%d=%d==%s\n",atoi(tag),atoi(length),Otp_Request_id);
            break;
     case 88:
            memset(Otp_Code,0x00,sizeof(Otp_Code));
            memcpy(Otp_Code,s,atoi(length));
            f_printf(stderr,"Tag= Length= OTP CODE value:%d=%d==%s\n",atoi(tag),atoi(length),Otp_Code);
            break;
     case 89:
            memset(Vas_Amount,0x00,sizeof(Vas_Amount));
            memcpy(Vas_Amount,s,atoi(length));
            f_printf(stderr,"Tag= Length= Vas_Amount value:%d=%d==%s\n",atoi(tag),atoi(length),Vas_Amount);
            break;
     case 90:
            memset(parent_rrn,0x00,sizeof(parent_rrn));
            memcpy(parent_rrn,s,atoi(length));
            f_printf(stderr,"Tag= Length= PARENT value:%d=%d==%s\n",atoi(tag),atoi(length),parent_rrn);
            break;
     case 91:
            memset(prefered_limit_flag,0x00,sizeof(prefered_limit_flag));
            memcpy(prefered_limit_flag,s,atoi(length));
            f_printf(stderr,"Tag= Length= PREFERED LIMIT RESET FLAG  value:%d=%d==%s\n",atoi(tag),atoi(length),prefered_limit_flag);
            
	    if(memcmp(bfl_dealer_master.preferred_limit_flag,"N",1 ) == 0)
	    { 
            	memcpy(prefered_limit_flag,"N",1);
		f_printf(stderr,"Prefered flag changed to [N] as Dealer prefered flag is [N] \n");
	    }	
	break;
     case 96:
            memset(Uniq_Identifier_Code,0x00,sizeof(Uniq_Identifier_Code));
            memcpy(Uniq_Identifier_Code,s,atoi(length));
            f_printf(stderr,"Tag= Length = UNIQ IDENTIFIER CODE value:%d=%d==%s\n",atoi(tag),atoi(length),Uniq_Identifier_Code);
            break;
     #endif
	 #ifdef MRL_207
     case 98:
           memset(OTP_Verify_Flag, 0x00,sizeof(OTP_Verify_Flag));
           memcpy(OTP_Verify_Flag,s,atoi(length));
           f_printf(stderr,"Tag= Length = OTP VERIFICATION RESULT value:%d=%d==%s\n",atoi(tag),atoi(length),OTP_Verify_Flag);
          break;
     #endif
#ifdef MRL_304
	case 56:
		memcpy(e_flag,s,1);
		f_printf(stderr,"Tag= Length = E_FLAG value:%d=%d==%s\n",atoi(tag),atoi(length),e_flag);
		break;
#endif

	case 105:
		memset(txn_mob_num, 0x00, sizeof(txn_mob_num));
		memcpy(txn_mob_num,s,atoi(length));
		f_printf(stderr,"Tag= Length = Txn_Mobile_Number value:%d=%d==%s\n",
			atoi(tag),atoi(length),txn_mob_num);
		break;
	
		

	case 3:
		memset(s_tenure,0x00,sizeof(s_tenure));
		memcpy(s_tenure,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_tenure value:%d=%d==%s\n",atoi(tag),atoi(length),s_tenure);
		break;

	case 6:
		memset(s_adv_emi_cnt,0x00,sizeof(s_adv_emi_cnt));
		memcpy(s_adv_emi_cnt,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_adv_emi_cnt value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_adv_emi_cnt);
		break;

	case 67:
		memset(s_dbd,0x00,sizeof(s_dbd));
		memcpy(s_dbd,s,atoi(length));
		local_data = atoi(s_dbd);
		f_printf(stderr,"local_data[%d]",local_data);
		memset(s_dbd,0x00,sizeof(s_dbd));
		sprintf(s_dbd,"%03d",local_data);
		strcat(s_dbd,"00");	
		f_printf(stderr,"Tag= Length= S_dbd value:%d=%d==%s\n",atoi(tag),atoi(length),s_dbd);
		break;

	case 99:
		memset(s_procfees,0x00,sizeof(s_procfees));
		memcpy(s_procfees,s,atoi(length));
		paisa = atoi(s_procfees);
		f_printf(stderr, "paisa	[%d] ", paisa);
		memset(s_procfees, 0x00, sizeof(s_procfees));
		sprintf(s_procfees, "%010d", paisa);
		strcat(s_procfees, "00");
		f_printf(stderr,"Tag= Length= S_procfees value:%d=%d==%s\n",atoi(tag),atoi(length),s_procfees);
		break;

	case 7:
		memset(s_roi,0x00,sizeof(s_roi));
		memcpy(s_roi,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_roi value:%d=%d==%s\n",atoi(tag),atoi(length),s_roi);
		break;

	case 100:
		memset(s_asst_risk,0x00,sizeof(s_asst_risk));
		memcpy(s_asst_risk,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_asst_risk value:%d=%d==%s\n",atoi(tag),atoi(length),s_asst_risk);
		break;

	case 101:
		memset(s_txn_risk,0x00,sizeof(s_txn_risk));
		memcpy(s_txn_risk,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_txn_risk value:%d=%d==%s\n",atoi(tag),atoi(length),s_txn_risk);
		break;
	case 102:
		memset(s_txn_velloc,0x00,sizeof(s_txn_velloc));
		memcpy(s_txn_velloc,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_txn_velloc value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_txn_velloc);
		break;
	case 103:
		memset(s_txn_velloc_days,0x00,sizeof(s_txn_velloc_days));
		memcpy(s_txn_velloc_days,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_txn_velloc_days value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_txn_velloc_days);
		break;
	case 104:
		memset(s_txn_branch_id,0x00,sizeof(s_txn_branch_id));
		memcpy(s_txn_branch_id,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_txn_branch_id value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_txn_branch_id);
		break;
	
	case 107:	
	        memset(Limit_Check_Bypass_Flag, 0x00,sizeof(Limit_Check_Bypass_Flag));
	        memcpy(Limit_Check_Bypass_Flag,s,atoi(length));
	        f_printf(stderr,"Tag= Length = Limit Check Bypass Flag value:%d=%d==%s\n",atoi(tag),atoi(length),Limit_Check_Bypass_Flag);
          break;

	case 64:
		memset(s_adv_emi_amnt,0x00,sizeof(s_adv_emi_amnt));
		memcpy(s_adv_emi_amnt,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_adv_emi_amnt value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_adv_emi_amnt);
		break;
	case 63:
		memset(s_emi_amnt,0x00,sizeof(s_emi_amnt));
		memcpy(s_emi_amnt,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_emi_amnt value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_emi_amnt);
		break;
	case 42:
		memset(s_cr_pr,0x00,sizeof(s_cr_pr));
		memcpy(s_cr_pr,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_cr_pr value:%d=%d==%s\n",
			atoi(tag),atoi(length),s_cr_pr);
		break;
	case 43:
		memset(s_mbd,0x00,sizeof(s_mbd));
		memcpy(s_mbd,s,atoi(length));
		f_printf(stderr,"Tag= Length= S_mbd value:%d=%d==%s\n", 	
			atoi(tag),atoi(length),s_mbd);
		break;

     default:
            break;
    }
    s += atoi(length);
    i = s - p;
   f_printf(stderr,"LENGTH READ %d\n",i);

 }
return(0);
}


int Refresh_strings()
{
        /********REQUEST PACKET**************/
        memset(manufacture_id,0x00,sizeof(manufacture_id));
        memset(asset_cat,0x00,sizeof(asset_cat));
        memset(cr_pr_source,0x00,sizeof(cr_pr_source));
        memset(model_id,0x00,sizeof(model_id));
        memset(scheme_id,0x00,sizeof(scheme_id));
        memset(imei_ser_no,0x00,sizeof(imei_ser_no));
        memset(transaction_source,0x00,sizeof(transaction_source));
        memset(addr_change_falg,0x00,sizeof(addr_change_falg));
        memset(promo_code,0x00,sizeof(promo_code));
        memset(relation_type,0x00,sizeof(relation_type));
        memset(access_code,0x00,sizeof(access_code));
        memset(buisness_prod_code,0x00,sizeof(buisness_prod_code));
        memset(cell_number,0x00,sizeof(cell_number));
        #ifdef BFL_PHASE2
        memset(Otp_Code,0x00,sizeof(Otp_Code));
        memset(Vas_Amount,0x00,sizeof(Vas_Amount));
        memset(Otp_Request_id,0x00,sizeof(Otp_Request_id));
        memset(prefered_limit_flag,0x00,sizeof(prefered_limit_flag));
        memset(Uniq_Identifier_Code,0x00,sizeof(Uniq_Identifier_Code));
        #endif
	memset(e_flag,0x00,sizeof(e_flag));
	memset(txn_mob_num, 0x00, sizeof(txn_mob_num));


#ifdef MRL_287
	memset(mob_num_change_flag,0x00,sizeof(mob_num_change_flag));
#endif
 #ifdef MRL_207
        memset(OTP_Verify_Flag,0x00,sizeof(OTP_Verify_Flag));
 #endif
        memset(Limit_Check_Bypass_Flag,0x00,sizeof(Limit_Check_Bypass_Flag));



        /********PRO-C DATA**************/
        memset(cr_pr_source,0x00,sizeof(cr_pr_source));
        memset(bfl_loan_tenure,0x00,sizeof(bfl_loan_tenure));
        memset(customer_id,0x00,sizeof(customer_id));
        memset(loan_due_day,0x00,sizeof(loan_due_day));
        #ifdef NEVER
        memset(charge_id_1,0x00,sizeof(charge_id_1));
        memset(charge_amount_1,0x00,sizeof(charge_amount_1));
        memset(charge_id_2,0x00,sizeof(charge_id_2));
        memset(charge_amount_2,0x00,sizeof(charge_amount_2));
        memset(charge_id_3,0x00,sizeof(charge_id_3));
        memset(charge_amount_3,0x00,sizeof(charge_amount_3));
        memset(charge_id_4,0x00,sizeof(charge_id_4));
        memset(charge_amount_4,0x00,sizeof(charge_amount_4));
        memset(charge_id_5,0x00,sizeof(charge_id_5));
        memset(charge_amount_5,0x00,sizeof(charge_amount_5));
        #endif
        memset(bfl_auth_source,0x00,sizeof(bfl_auth_source));
        memset(emi_amount,0x00,sizeof(emi_amount));
        memset(adv_emi_count,0x00,sizeof(adv_emi_count));
        memset(adv_emi_amount,0x00,sizeof(adv_emi_amount));
        memset(roi,0x00,sizeof(roi));
        memset(proce_fee,0x00,sizeof(proce_fee));
        memset(emi_card_fee,0x00,sizeof(emi_card_fee));
        memset(dbd,0x00,sizeof(dbd));
        memset(reward_point_balance,0x00,sizeof(reward_point_balance));
        memset(ecs_mandate,0x00,sizeof(ecs_mandate));
        memset(Asset_Risk,0x00,sizeof(Asset_Risk));
        memset(Requested_reward_point,0x00,sizeof(Requested_reward_point));
        memset(trns_amount,0x00,sizeof(trns_amount));
        memset(Avail_Amount,0x00,sizeof(Avail_Amount));

        return(0);
}

int VerifytheSchemeData()
{

    int iRet = -1;
    if(memcmp ( paczisoauto.processing_code, "00", 2 ) != 0 )
    return (ECHEC) ;

    if((memcmp(transaction_source,"EI",2) == 0 ) || 
       (memcmp(transaction_source,"WI",2) == 0 ) || 
       (memcmp(transaction_source,"CI",2) == 0 ) ||
       (memcmp(transaction_source,"XI",2) == 0 ) ||
       (memcmp(transaction_source,"RI",2) == 0 ) ||
       (memcmp(transaction_source,"MI",2) == 0 ) ||
       (memcmp(transaction_source,"SI",2) == 0 ))       /* MRL_359 */

    iRet = Get_Verify_invoice_Finance();
  
    if((memcmp(transaction_source,"EP",2) == 0 ) || 
       (memcmp(transaction_source,"WP",2) == 0 ) || 
       (memcmp(transaction_source,"AP",2) == 0 ) || 
       (memcmp(transaction_source,"CP",2) == 0 ) ||
       (memcmp(transaction_source,"XP",2) == 0 ) ||  
       (memcmp(transaction_source,"RP",2) == 0 ) ||  
       (memcmp(transaction_source,"MP",2) == 0 ) ||
       (memcmp(transaction_source,"SP",2) == 0 )) 	/* MRL_359 */
    iRet = Get_Verify_Product_Finance();

    if(iRet != SUCCES)
    {
           f_printf(stderr,"\n Scheme Validation Failed  \n");
           return (ECHEC) ;
    }
        memcpy(bflrthauth.creditprogram,procdata.cr_pr_sour,10);
        memcpy(bflrthauth.productcode,buisness_prod_code,strlen(buisness_prod_code));
        memcpy(bflrthauth.schemeid,scheme_id,strlen(scheme_id));
        memcpy(bflrthauth.loantenure,procdata.bfl_loan_ten,5);
        memcpy(bflrthauth.rateofinterest,procdata.rate_of_interest,5);
        if(( memcmp(buisness_prod_code,"RDP",3) == 0 ) || ( memcmp(buisness_prod_code,"RVS",3) == 0 )
            || ( memcmp(buisness_prod_code,"RDL",3) == 0 ) ||( memcmp(buisness_prod_code,"RCD",3) == 0 )
	    || ( memcmp(buisness_prod_code,"RLS",3) == 0 ) || (memcmp(buisness_prod_code,"RCF",3) == 0) || (memcmp(buisness_prod_code,"RDD",3) == 0))
        	memcpy(bflrthauth.dueday,"05",2);
        else if((memcmp(buisness_prod_code,"REMI",4)==0) 	
		|| (memcmp(buisness_prod_code,"MPD",3)==0)
		|| (memcmp(buisness_prod_code,"MPC",3)==0)
		)
                 memcpy(bflrthauth.dueday,"02",2);
        else if((memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) == 0)||(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) == 0))
        	memcpy(bflrthauth.dueday,"02",2);
        else
        	memcpy(bflrthauth.dueday,bflzbcli.lbcli_base_loan_fdd+4,2);
        memcpy(bflrthauth.chargeid1,procdata.charge_id1,8);
        memcpy(bflrthauth.chargeamount1,procdata.charge_amount1,12);
        memcpy(bflrthauth.chargeid2,procdata.charge_id2,8);
        memcpy(bflrthauth.chargeamount2,procdata.charge_amount2,12);
        memcpy(bflrthauth.chargeid3,procdata.charge_id3,8);
        memcpy(bflrthauth.chargeamount3,procdata.charge_amount3,12);
        memcpy(bflrthauth.chargeid4,procdata.charge_id4,8);
        memcpy(bflrthauth.chargeamount4,procdata.charge_amount4,12);
        memcpy(bflrthauth.chargeid5,procdata.charge_id5,8);
        memcpy(bflrthauth.chargeamount5,procdata.charge_amount5,12);
        if((strlen(addr_change_falg) > 0 ) && (memcmp(addr_change_falg,"Y",1)== 0))
        memcpy(bflrthauth.address_changed ,"Y",1);
        else
        memcpy(bflrthauth.address_changed ,"N",1);
        if(strlen(model_id) > 0 )
        memcpy(bflrthauth.modelid,model_id,strlen(model_id));

#ifdef MRL_287
	if((strlen(mob_num_change_flag) > 0) && (memcmp(mob_num_change_flag,"Y",1) == 0))
		memcpy(bflrthauth.mob_num_change,"Y",1);
	else
		memcpy(bflrthauth.mob_num_change,"N",1);
	f_printf(stderr, "Mobile Change Flag: %.1s\n", bflrthauth.mob_num_change );
#endif

#ifdef MRL_304
	if((strlen(e_flag) > 0 ) && (memcmp(e_flag,"Y",1)== 0))
		memcpy(bflrthauth.e_flag ,"Y",1);
	else
		memcpy(bflrthauth.e_flag ,"N",1);
#endif
     
        return (SUCCES);
}

int Get_Verify_Product_Finance()
{

        f_printf(stderr,"Get_Verify_Product_Finance........\n");

        memset((char *)&procdata,0x20,sizeof(t_procdata));

        if(Get_Verfiy_Proc_Data() != SUCCES)
        {
           f_printf(stderr,"\n Product Finaance Scheme Validation Error \n");
           return (ECHEC) ;
        }

        memcpy(bfl_loan_tenure,procdata.bfl_loan_ten,5);
        memcpy(adv_emi_count,procdata.adv_emicount,3);
        memcpy(adv_emi_amount,procdata.adv_emiamount,12);
        memcpy(roi,procdata.rate_of_interest,5);
        memcpy(proce_fee,procdata.proce_fees,12);
        memcpy(dbd,procdata.DBD,10);

        return (SUCCES);
}

int Get_Verify_invoice_Finance()
{
        if(Get_Verfiy_Proc_Data() != SUCCES)
        {
           f_printf(stderr,"\n Scheme Validation Failed for Invloice FInaance  \n");
           return (ECHEC) ;
        }
        /***********PRO-C DATA*****************/
        memcpy(bfl_loan_tenure,procdata.bfl_loan_ten,5);
        memcpy(adv_emi_count,procdata.adv_emicount,3);
        memcpy(roi,procdata.rate_of_interest,5);
        memcpy(proce_fee,procdata.proce_fees,12);
        memcpy(dbd,procdata.DBD,10);

        /**********************REQUEST DATA***********************/
        memcpy(manufacture_id,"        ",8);
        memcpy(asset_cat,bfl_dealer_master.asset_catg_id,8); 
      /*  memcpy(model_id,"        ",8);*/
        f_printf(stderr,"Modrel ID For Invoice [%s]\n",model_id); 
        memcpy(imei_ser_no,"               ",15);
        memcpy(relation_type," ",1);
    /*    memcpy(fraud_flag," ",1);
        memcpy(addr_change_falg," ",1);*/
        memcpy(promo_code,"                              ",30);
        return (SUCCES);
}


int Get_Verfiy_Proc_Data()
{
        char merchantid[16];
        int result = -1;
        char no_of_months[6];
        char adv_no_of_months[4];
        char interest_rate[6];
        char Emi_Amount[13];
        char Adv_Emi_Amount[13];
	 t_scheme u_scheme;
	 t_risk u_risk;
	 t_cr_pr u_cr_pr;
	char custrisk[3] = {0};
	char cardtyp[3] = {0};
	char  reltyp[2] = {0};

        memset((char *)&procdata,0x20,sizeof(t_procdata));
        memset(merchantid,0x00,sizeof(merchantid));
        memset(no_of_months,0x00,sizeof(no_of_months));
        memset(adv_no_of_months,0x00,sizeof(adv_no_of_months));
        memset(Emi_Amount,0x00,sizeof(Emi_Amount));
        memset(Adv_Emi_Amount,0x00,sizeof(Adv_Emi_Amount));
        memset(interest_rate,0x00,sizeof(interest_rate));
        memcpy(procdata.source,transaction_source,2);
        memcpy(procdata.source+2,"C",1);
        memcpy(merchantid,paczisoauto.card_acceptor_identification_code,15);
        memcpy(trns_amount,paczisoauto.amount_transaction,12);
        memcpy(procdata.customer_risk,bflzbcom.lbcom_cust_risk_clasif,2);
        memcpy(procdata.card_type,bflzport.lport_mxp_card_type,2);
        if(strlen(relation_type) > 0 )
        memcpy(procdata.relationship_type,relation_type,1);

        f_printf(stderr,"\n Transction merchantid[%s] \n",merchantid);
        f_printf(stderr,"\n Transction manufacture_id[%s] \n",manufacture_id);
        f_printf(stderr,"\n Transction asset_cat[%s] \n",asset_cat);
        f_printf(stderr,"\n Transction model_id[%s] \n",model_id);
        f_printf(stderr,"\n Transction scheme_id[%s] \n",scheme_id);
        f_printf(stderr,"\n Transction trns_amount[%s] \n",trns_amount);
        f_printf(stderr,"\n Transction buisness_prod_code[%s] \n",buisness_prod_code);
        f_printf(stderr,"TIME BEFORE SCHEME VERIFICATION[%.6s] PID[%d]\n",(char *)GetTime (),getpid());

	if((memcmp(transaction_source,"SI",2) != 0) && (memcmp(transaction_source,"SP",2) != 0))
	{
		memcpy(custrisk, procdata.customer_risk, 2);
		memcpy(cardtyp, procdata.card_type, 2);
		memcpy(reltyp, procdata.relationship_type, 1);
			
		 Download_Scheme_Data_Api(merchantid, manufacture_id, asset_cat, model_id, scheme_id, trns_amount, buisness_prod_code, custrisk, cardtyp, reltyp, &procdata,&result);
	

		f_printf(stderr,"TIME AFTER  SCHEME VERIFICATION[%.6s] PID[%d]\n",
			(char *)GetTime (),getpid());
	}
	else
	{
		f_printf(stderr, "SI-SP transaction. Skipping DB Call.\n");
		memcpy(procdata.bfl_loan_ten,s_tenure,strlen(s_tenure));
		memcpy(procdata.adv_emicount,s_adv_emi_cnt,strlen(s_adv_emi_cnt));
		memcpy(procdata.DBD,s_dbd,strlen(s_dbd));
		memcpy(procdata.proce_fees,s_procfees,strlen(s_procfees));
		memcpy(procdata.rate_of_interest,s_roi,strlen(s_roi));
		memcpy(procdata.asset_risk,s_asst_risk,strlen(s_asst_risk));
		memcpy(procdata.trans_risk,s_txn_risk,strlen(s_txn_risk));
		memcpy(procdata.trans_velocity,s_txn_velloc,strlen(s_txn_velloc));
		memcpy(procdata.risk_vel_days,s_txn_velloc_days,strlen(s_txn_velloc_days));
		memcpy(procdata.branch,s_txn_branch_id,strlen(s_txn_branch_id));
		memcpy(procdata.adv_emiamount,s_adv_emi_amnt,strlen(s_adv_emi_amnt));
		memcpy(procdata.emiamount,s_emi_amnt,strlen(s_emi_amnt));
		memcpy(procdata.cr_pr_sour,s_cr_pr,strlen(s_cr_pr));
		result=0;
	}

        if(result < 0 )
        {
           f_printf(stderr,"\n Error in Getting the Scheme_Data from Oracle Table \n");
           return (ECHEC) ;
        }
        else if(result > 0 )
        {
           f_printf(stderr,"\n Scheme Validation Error[%d] \n",result);
           return (ECHEC) ;
        }
        else
        {
            f_printf(stderr,"\n Scheme Validation Success[%d] \n",result);
            f_printf(stderr,"\n PROC DATA[%s] \n",&procdata);
        }

        f_printf(stderr,"\n Transction Amount2[%s] \n",trns_amount);
        f_printf(stderr,"\n Transction model_id[%s] \n",model_id);
        memcpy(no_of_months,procdata.bfl_loan_ten,5);
	memcpy(adv_no_of_months,procdata.adv_emicount,3);
        memcpy(interest_rate,procdata.rate_of_interest,5);
        if((memcmp(procdata.rate_of_interest,"00000",5) == 0)&&(memcmp(procdata.bfl_loan_ten,"00000",5) == 0))
        {
           f_printf(stderr,"\n Invalid ROI[%.5s] and Invalid BFL LOAN Tenure[%.5s]\n",procdata.rate_of_interest , procdata.bfl_loan_ten);
           return (ECHEC) ;
        }
        BFL_EMICalculated_Amount(trns_amount, no_of_months, interest_rate, Emi_Amount,Adv_Emi_Amount,adv_no_of_months);
        memcpy(procdata.emiamount,Emi_Amount,12);
        memcpy(emi_amount,Emi_Amount,12);
        memcpy(procdata.adv_emiamount,Adv_Emi_Amount,12);
        return SUCCES;
}


int BFL_EMICalculated_Amount(char *ptrTxnAmount,char *ptrnummonths,char *ptrinterest_per, char *ptrFinalEMIAmt,char *Adv_Emi,char *adv_emi_no)
{
                double dtxnamount, dt1, dt2;
                double dinterestamt, demi, emiamount, dint_percent, dfinal_emi,adv_emamount;
                double  loan_val_rat,adv_count,total_count;
                double dint_per_month;
                RoundOffEMI = 0.0;
                int iperiod_inmonth;
                int adviperiod_inmonth;
                int remaining_inmonth = 0;
                double vasamount_remain = 0.0;
                char szbuffer[25];
                #ifdef BFL_PHASE2
                double vasamount; 
                char   Local_Vas_Amount[13];
                #endif
		double dtxnamount_before_vas, emiamount_before_vas, demi_before_vas;

                f_printf(stderr, "\n Equated Monthly Installment calculation for BFL (EMI)  ");
                f_printf(stderr, "\n Transaction Amount [%.12s] Interest Rate [%.5s] Number of Months [%.5s] Number of adv emi[%.3s] ", ptrTxnAmount, ptrinterest_per ,ptrnummonths , adv_emi_no);

                /* Transaction Amount in 10+2 format */
                memset(szbuffer, 0x00, sizeof(szbuffer));
                memset(Loan_Value_Ratio, 0x00, sizeof(Loan_Value_Ratio));
                memcpy(szbuffer, ptrTxnAmount, strlen(ptrTxnAmount));
                dtxnamount = atof(szbuffer);
                dtxnamount = dtxnamount/100.00;

		dtxnamount_before_vas = dtxnamount;
                /* Number of Months */
                memset(szbuffer, 0x00, sizeof(szbuffer));
                memcpy(szbuffer, ptrnummonths, strlen(ptrnummonths));
                iperiod_inmonth = atoi(szbuffer);

                /* Number of Advance Months */
                memset(szbuffer, 0x00, sizeof(szbuffer));
                memcpy(szbuffer, adv_emi_no, strlen(adv_emi_no));
                adviperiod_inmonth = atoi(szbuffer);

                remaining_inmonth = (iperiod_inmonth - adviperiod_inmonth );
                #ifdef BFL_PHASE2
                if(
			(strlen(Vas_Amount) > 0 )
			&&(memcmp(Vas_Amount,"000000000000",12)!=0)
			&&(memcmp(Vas_Amount,"            ",12) != 0)
			)
                {
                        f_printf(stderr, "\n Adding VAS Amount  ");

                        memset(Local_Vas_Amount,0x00,sizeof(Local_Vas_Amount));
                        memcpy(Local_Vas_Amount,Vas_Amount,12);
                        f_printf(stderr, "\n VAS Amount [%s] ",Local_Vas_Amount);
                        vasamount = atof(Local_Vas_Amount);
                        vasamount = vasamount/100.00;
                        vasamount_remain = vasamount/remaining_inmonth ;
                        f_printf(stderr, "\n VAS Amount [%f] ",vasamount);
                        f_printf(stderr, "\n Remaining Months VAS Amount [%f] ",vasamount_remain);
                        dtxnamount = dtxnamount ;
                        f_printf(stderr, "\n Transaction Amount + VAS Amount [%f] ",dtxnamount);
                }
                #endif

                /* Rate of Interest */
                memset(szbuffer, 0x00, sizeof(szbuffer));
                memcpy(szbuffer, ptrinterest_per, strlen(ptrinterest_per));

                if((memcmp(ptrinterest_per, "     ",5) == 0) || (memcmp(ptrinterest_per, "00000",5) == 0))
                        dint_percent = 0.0;                      
                else
                {
                        dint_percent = atof(szbuffer);
                        if(dint_percent <= 0.0)
                           dint_percent = 0.0;
                        else
                           dint_percent = dint_percent/100.00;
                }

                dint_per_month =(dint_percent/12.00)/100.00;
                f_printf( stderr,"\n interest per month [%f] ",dint_per_month);
                f_printf( stderr,"\n Transaction Amount [%f] Interest Rate [%f] Number of Months [%d]", dtxnamount, dint_percent,iperiod_inmonth);

                demi =(dtxnamount * dint_per_month) ;
                demi_before_vas =(dtxnamount_before_vas * dint_per_month) ;
                f_printf(stderr, "\n 1. demi Value [%f] ", demi);
		if(dint_percent <= 0.0)
                {
                        f_printf(stderr,"\n Int rate is 0.0 Cal the EMI amount with txn amount and numb months" );
                        emiamount = dtxnamount/iperiod_inmonth;
                        emiamount = emiamount + vasamount_remain;
                        f_printf(stderr, "\n EMI  Amount + (VAS Amount/Remaining months) [%f] ",emiamount);
                        emiamount_before_vas = dtxnamount_before_vas/iperiod_inmonth;
                        f_printf(stderr,"\n.Before RoundOffEmi.[%f]",emiamount);
                        f_printf(stderr,"\n.Before RoundOffEmi_before_vas.[%f]",emiamount_before_vas);

                        RoundOffEMI = emiamount;
                        RoundOffEmi(0);
                        emiamount = RoundOffEMI; 

                        RoundOffEMI = emiamount_before_vas;
                        RoundOffEmi(0);
                        emiamount_before_vas = RoundOffEMI; 

                        f_printf(stderr,"\n.After RoundOffEmi.[%f]",emiamount);
                        adv_emamount = (emiamount_before_vas*adviperiod_inmonth);
                }
                else
                {
                        emiamount = ((dtxnamount/iperiod_inmonth)+ demi);
                        emiamount = emiamount + vasamount_remain;
                        emiamount_before_vas = ((dtxnamount_before_vas/iperiod_inmonth)+ demi_before_vas);

                        f_printf(stderr,"\n.Before RoundOffEmi.[%f]",emiamount);
                        f_printf(stderr,"\n.Before RoundOffEmi_before_VAS.[%f]",emiamount_before_vas);

                        RoundOffEMI = emiamount;
                        RoundOffEmi(0);
                        emiamount = RoundOffEMI;

                        RoundOffEMI = emiamount_before_vas;
                        RoundOffEmi(0);
                        emiamount_before_vas = RoundOffEMI;

                        f_printf(stderr,"\n.After RoundOffEmi.[%f]",emiamount);
                        adv_emamount = ((emiamount_before_vas*adviperiod_inmonth));
                }
                RoundOffEMI = 0.0;
    
                f_printf( stderr,"\n emiamount Mid [%f]\n", emiamount);
                emiamount = emiamount *100.00 ;
                adv_emamount = adv_emamount*100.00 ;
                sprintf(ptrFinalEMIAmt,"%012.0f",emiamount);
                sprintf(Adv_Emi,"%012.0f",adv_emamount);
                if(strlen(adv_emi_no) == 0 )
                memcpy(Adv_Emi,"000000000000",12);


                /*********Get Loan Value Ratio************/
                if((memcmp(adv_emi_no,"00000",5) != 0 )&&(memcmp(adv_emi_no,"     ",5) != 0))
                {
                adv_count =  atoi(adv_emi_no);
                total_count = atoi(ptrnummonths);
                loan_val_rat = ((total_count - adv_count)/total_count) ;
                loan_val_rat = ((loan_val_rat)*(10000.00)) ;
                sprintf(Loan_Value_Ratio,"%05.0f",loan_val_rat);
                }
                else
                memcpy(Loan_Value_Ratio,"10000",5);
                f_printf( stderr,"\n LOAN VALUE RATIO[%f]\n", loan_val_rat);
                f_printf( stderr,"\n LOAN VALUE RATIO[%s]\n", Loan_Value_Ratio);
                /*********Get Loan Value Ratio************/
                
                f_printf( stderr,"\n emiamount  [%s]\n", ptrFinalEMIAmt);
                f_printf( stderr,"\n Advance emiamount  [%s]\n", Adv_Emi);

                return (0);
}

int AddAdditionalTags(char *additonal_data, int Val)
{
           char Card_name[27];
           char RedeemPoint_Amount[13];
           char Ecs_Available[13];
           #ifdef BFL_PHASE2
           char Sms_Number[11];
           char Prefered_Amount[13];
           char Card_first_fee[13];
           char Processing_Fees[13];
           char Advance_EMI_Amount[13];
           char Prefered_Card_Fee[13];
           #endif
           int length_data = 0 ;
           char length_name[4];
           char new_date[9];
	   char *AvlBal;
           #ifdef BFL_BAL 
           char Credit_Available[13];
           char Bal_Txn_Amount[13];
           char trim_zeros[13];
           double Bal_Txn_Amt = 0.0 ;
           int  CARD_BLOCK = 0 ; 
           #endif
		
           memset(Card_name,0x00,sizeof(Card_name));
           memset(new_date,0x00,sizeof(new_date));
           #ifdef BFL_PHASE2
           memset(Sms_Number,0x00,sizeof(Sms_Number));
           memset(Prefered_Amount,0x00,sizeof(Prefered_Amount));
           memset(Card_first_fee,0x00,sizeof(Card_first_fee));
           memset(Processing_Fees,0x00,sizeof(Processing_Fees));
           memset(Advance_EMI_Amount,0x00,sizeof(Advance_EMI_Amount));
           memset(Prefered_Card_Fee,0x00,sizeof(Prefered_Card_Fee));
           #endif
           memset(length_name,0x00,sizeof(length_name));
           memset(RedeemPoint_Amount,0x00,sizeof(RedeemPoint_Amount));
           memcpy(Card_name,bflzport.lport_name_on_card,sizeof(bflzport.lport_name_on_card));
           /*if((memcmp(paczisoauto.processing_code,"00",2) == 0) MRL-359 && (Ecs_Required != 1))*/

	   memset(Ecs_Available,0x00,sizeof(Ecs_Available));
	   memcpy(Ecs_Available,(char*)BFL_GetOpenECSBalance( &bflzbcom),12);
	   if(atof(Ecs_Available) < 0.0 )
		   memcpy(Ecs_Available,"000000000000",12);

           #ifdef BFL_BAL
           if(memcmp(paczisoauto.processing_code,"30",2) == 0)
           {
              memset(Ecs_Available,0x00,sizeof(Ecs_Available));
              memcpy(Ecs_Available,(char*)BFL_GetOpenECSBalance( &bflzbcom),12);
              if(atof(Ecs_Available) < 0.0 )
              memcpy(Ecs_Available,"000000000000",12);

              memset(Credit_Available,0x00,sizeof(Credit_Available));
              memcpy(Credit_Available,paczisoauto.additional_amounts+8, 12);
              if(atof(Credit_Available) < 0.0 )
              memcpy(Credit_Available,"000000000000",12);
              memset(Bal_Txn_Amount,0x00,sizeof(Bal_Txn_Amount));

              if(( memcmp(bflzport.lport_cs_block_flag,"Y",1 ) == 0 )
		 ||( memcmp(bflzport.lport_rto_block_flag,"Y",1 ) == 0 )
		 ||(( memcmp(bflzbcom.lbcom_sys_block_type,"UB",2 ) != 0 )&& ( memcmp(bflzbcom.lbcom_sys_block_type,"  ",2 ) != 0 ) )
		 ||(( memcmp(bflzbcom.lbcom_fraud_block_type,"UB",2 ) != 0 )&& ( memcmp(bflzbcom.lbcom_fraud_block_type,"  ",2 ) != 0 )))
               CARD_BLOCK = 1 ; 
              else if((memcmp(Credit_Available,"000000000000",12) != 0 )&& (memcmp(Ecs_Available,"000000000000",12) != 0 ))
              {
              	if((atof(Credit_Available)/100.00) < ((atof(Ecs_Available)/100.00) * 3 ) )
                {
              		memcpy(Bal_Txn_Amount,Credit_Available,12);
                        f_printf(stderr,"BALENQ:Adding Card Credit Bal Amount [%.12s] ",Bal_Txn_Amount);
                }
                else
                {
                      Bal_Txn_Amt = (atof(Ecs_Available));
                      Bal_Txn_Amt = (Bal_Txn_Amt * 3.0 );
                      sprintf(Bal_Txn_Amount,"%012.0f",Bal_Txn_Amt);
                      f_printf(stderr," BALENQ:Adding     3 * ECS AVAIALABLE [%.12s] ",Bal_Txn_Amount);
                }
                 memset ( trim_zeros , 0x00 , sizeof (trim_zeros) );
                 memcpy ( trim_zeros, Bal_Txn_Amount , 12 );
                 trim_leading(trim_zeros , '0' );
                 memset(Bal_Txn_Amount,0x00,sizeof(Bal_Txn_Amount));
                 memcpy (Bal_Txn_Amount,trim_zeros,strlen(trim_zeros));
              }
              else
              ;
            }
           #endif
           if(memcmp(paczisoauto.processing_code,"21",2) == 0)
           GetRedeemAmount(RedeemPoint_Amount);
           if(memcmp(paczisoauto.processing_code,"99",2) == 0)
           GetRedeemAmount(RedeemPoint_Amount);

           if(Val == SALE_RESPONSE)
           {
                memset(additonal_data,0x00,sizeof(additonal_data));
                        if(
				(memcmp(buisness_prod_code,"REMI",4) == 0 )
				||(memcmp(buisness_prod_code,"ERF",3) == 0 )
				||(memcmp(buisness_prod_code,"ECF",3) == 0 )
				||(memcmp(buisness_prod_code,"WLT",3) == 0 )
				||(memcmp(transaction_source,"EP",2) == 0)
				||(memcmp(transaction_source,"EI",2) == 0))
                        {
           		        strcat(additonal_data,"204");
           			strcat(additonal_data,"008");

                           if((memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) != 0)&&(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) != 0))
                                GetNewDate(new_date); 
                           else if ((memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) == 0)||(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) == 0))
                                GetNewDate1(new_date); 
    
           			strncat(additonal_data,new_date,8);
                        }
                        else
                        {
           		        strcat(additonal_data,"204");
                        	strcat(additonal_data,"037");
                         	strncat(additonal_data,"Please contact FOS for First Due Date",37);
                        }
           	strcat(additonal_data,"205");
                GetCardHolderPORTName(Card_name);
                length_data = strlen(Card_name);
                sprintf(length_name,"%03d",length_data);
           	strncat(additonal_data,length_name,3);
           	strncat(additonal_data,Card_name,length_data);
                if(chargeslip_first_fee == 3)
                {
                         memcpy(Card_first_fee,bflzport.lport_emi_card_charges,12);
                         f_printf(stderr,"\n EMI Card Charges [%.12s].\n",bflzport.lport_emi_card_charges);
                         if(atof(Card_first_fee) > 0.0 )
                         {
                        	f_printf(stderr,"\n  Adding Card Fee..To Response...\n");
                		strcat(additonal_data,"066");
                		strcat(additonal_data,"012");
                		strncat(additonal_data,bflzport.lport_emi_card_charges,12);
                        	memcpy(emi_card_fee,bflzport.lport_emi_card_charges,12);
                        /*	memcpy(bflrthauth.card_fees,bflzport.lport_emi_card_charges,12);
                        	memcpy(bflrthauth.first_swipe_flag ,"Y" ,1);*/
                         }
                         else
                         f_printf(stderr,"\n Card Fee.Not Added Due to Improper value of Card fee...[%.12s].\n",bflzport.lport_emi_card_charges);
                         
                }
                else
                memcpy(bflrthauth.card_fees,"            ",12);


		 #ifdef MRL_383
                        if(rset_card_1st_fee == 2)
                        {
                         memcpy(Card_first_fee,bflzport.lport_emi_card_charges,12);
                         f_printf(stderr,"\n REMI EMI Card Charges [%.12s].\n",bflzport.lport_emi_card_charges);
                         if(atof(Card_first_fee) > 0.0 )
                         {
                                f_printf(stderr,"\n  Adding REMI Card Fee..To Response...\n");
                                strcat(additonal_data,"091");
                                strcat(additonal_data,"012");
                                strncat(additonal_data,bflzport.lport_emi_card_charges,12);
                                memcpy(remi_card_fee,bflzport.lport_emi_card_charges,12);
                     /*           memcpy(bflrthauth.card_fees,bflzport.lport_emi_card_charges,12);
                                memcpy(bflrthauth.first_swipe_flag ,"Y" ,1);*/
                         }
                         else
                         f_printf(stderr,"\n REMI Card Fee.Not Added Due to Improper value of Card fee...[%.12s].\n",bflzport.lport_emi_card_charges);

			}
			else
			memcpy(bflrthauth.card_fees,"            ",12);
                #endif

                if((memcmp(procdata.proce_fees,"            ",12) != 0 ) && (memcmp(procdata.proce_fees,"000000000000",12) != 0 ))
                        memcpy(bflrthauth.processing_fees,procdata.proce_fees,12);
                else
                memcpy(bflrthauth.processing_fees,"            ",12);
                memcpy(Processing_Fees,procdata.proce_fees,12);
                memcpy(Advance_EMI_Amount,procdata.adv_emiamount,12);
                strcat(additonal_data,"201");
                strcat(additonal_data,"019");
                strncat(additonal_data,bflzport.lport_cust_id,19);
                if(Ecs_Required == 1)
                {
                        strcat(additonal_data,"203");
                        strcat(additonal_data,"012");
                        strncat(additonal_data,"Required    ",12);
                        memcpy (bflrthauth.ecs_mandate,"Required    ",12);
                        Ecs_Required = 0;
                }
                else 
                {
                        strcat(additonal_data,"203");
                        strcat(additonal_data,"012");
                        strncat(additonal_data,"Not Required",12);
                        memcpy (bflrthauth.ecs_mandate,"Not Required",12);
                }

                if (strlen(Ecs_Available) > 0 )
		{
			strcat(additonal_data,"401");
			strcat(additonal_data,"012");
			strncat(additonal_data,Ecs_Available,12);
		}

                if(Mobile_No_Tranx == 1)
                {
                  strcat(additonal_data,"206");
                  strcat(additonal_data,"016");
                  strncat(additonal_data,paczisoauto.primary_account_number,16);
                }
                if(Mobile_No_Tranx == 0)
                {
                	strcat(additonal_data,"052");
                	strcat(additonal_data,"010");
                	strncat(additonal_data,bflzbcli.lbcli_residence_mobile,10);
                }
		strcat(additonal_data,"210");
                strcat(additonal_data,"012");
		AvlBal = (char*)BFL_GetBalance (&bflzbcom);
		strncat(additonal_data,AvlBal,12);

                if(
			(Card_First_Trans == 1)
			&&
			(
			  	(memcmp(bflzport.lport_card_sub_type,"11",2) ==  0 ) ||
				(memcmp(bflzport.lport_card_sub_type,"19",2) ==  0 ) ||
				(memcmp(transaction_source, "SI", 2)==0) ||
				(memcmp(transaction_source, "SP", 2)==0)
			)
		)
                {
                  strcat(additonal_data,"080");
                  strcat(additonal_data,"001");
                  strcat(additonal_data,"Y");
                }
                /*Mobile_No_Tranx = 0; */
                if(strlen(RedeemPointBalance) > 0 )
                {
                        strcat(additonal_data,"077");
                        strcat(additonal_data,"006");
                        strncat(additonal_data,RedeemPointBalance+4,6);
                }
                strcat(additonal_data,"301");
                strcat(additonal_data,"010");
                strncat(additonal_data,bflzbcom.lbcom_branch_id,10);
                strcat(additonal_data,"302");
                strcat(additonal_data,"002");
                strncat(additonal_data,bflzport.lport_mxp_card_type,2);
                #ifdef BFL_PHASE2
                memcpy(Prefered_Amount,bflzbcom.lbcom_acc_prefferd_limit,12);
                if((atof(Prefered_Amount) > 0.0)
                   &&(memcmp(bfl_dealer_master.preferred_limit_flag,"Y",1) == 0 )
                   &&( memcmp(buisness_prod_code,"REMI",4) == 0 )
                   &&((memcmp(transaction_source,"EI",2) == 0) 
                   || (memcmp(transaction_source,"EP",2) == 0)))
                {
                	f_printf(stderr,"Adding SMS Tag,Account Prefer Limit[%.12s]\n",Prefered_Amount);
                	f_printf(stderr,"Adding SMS Tag,Account Prefer Limit[%f]\n",atof(Prefered_Amount));
                        memcpy(Prefered_Card_Fee,bflzlpa20.lpa20_prefered_card_fees,12);
                        GetSMSNumber(Sms_Number);
                        length_data = strlen(Sms_Number);
                        memset(length_name,0x00,sizeof(length_name));
                	sprintf(length_name,"%03d",length_data);
                        if(length_data > 0 )
                        {
                		strcat(additonal_data,"086");
                		strncat(additonal_data,length_name,3);
                		strncat(additonal_data,Sms_Number,length_data);
                                strcat(additonal_data,"094");
                                strncat(additonal_data,"012",3);
                                strncat(additonal_data,Prefered_Amount,12);
                                if(atof(Prefered_Card_Fee) > 0.0 )
                                {
                                	strcat(additonal_data,"095");
                                	strncat(additonal_data,"012",3);
                                	strncat(additonal_data,Prefered_Card_Fee,12);
                                }
                        }
                }
                #endif

            BFL_Gross_Amount(Advance_EMI_Amount,Processing_Fees ,Card_first_fee);

           }
           if((Val == BALANCE_INQUIRY_RESPONSE) || (Val == REDEEM_RESPONSE)|| (Val == REDEEMVOID_RESPONSE)|| (Val == SALEVOID_RESPONSE )||(Val == SALEREFUND_RESPONSE))
           {
                 if(Val == REDEEMVOID_RESPONSE)
                 {
                 	memset ( RedeemPointBalance, 0, 11 );
                 	memcpy ( RedeemPointBalance, (char*)BFL_GetRedeemBalance( &bflzbcom), 10 );
                 }
                	if(strlen(RedeemPointBalance) > 0 )
                	{
                		strcat(additonal_data,"077");
                		strcat(additonal_data,"006");
                		strncat(additonal_data,RedeemPointBalance+4,6);
                        }
                        #ifdef BFL_BAL
                        if(Val == BALANCE_INQUIRY_RESPONSE)
                        {
                             if(strlen(Ecs_Available) > 0 )
                             {
                                f_printf(stderr,"Adding ECS amount[%.12s]in Balance Inquiry Response",Ecs_Available);
                		strcat(additonal_data,"092");
                		strcat(additonal_data,"012");
                        	strncat(additonal_data,Ecs_Available,12);
                             }
		#ifdef MRL_204
				if(1 == remi_urban_decline)
				{
					strcat(additonal_data,"093");
					strcat(additonal_data,"020");
					strncat(additonal_data,"Decline - Customer Location not Active",38);
				}
				else
		#endif
                                if(CARD_BLOCK == 1)
                                {
                                        strcat(additonal_data,"093");
                                        strcat(additonal_data,"020");
                                        strncat(additonal_data,"Decline - Card block",20);
                                        CARD_BLOCK = 0 ;

                                }
                                else if((memcmp(Ecs_Available,"000000000000",12) == 0 ) ||
                                       ((memcmp(Ecs_Available,"000000000000",12) != 0 )&&((atof(Ecs_Available)/ 100.00) < 1670.00))|| 
                                       (memcmp(bflzbcom.lbcom_ecs_expiry_date,(char *)GetDateYYMMDD(),6)< 0 ))
                                {
                			strcat(additonal_data,"093");
                			strcat(additonal_data,"028");
                        		strncat(additonal_data,"Decline - Fresh ECS required",28);
                                }
                                else if((atof(Credit_Available)/ 100.00) < 5000.00)
                                {
                			strcat(additonal_data,"093");
                			strcat(additonal_data,"034");
                        		strncat(additonal_data,"Decline -  Card limit insufficient",34);
                                }
                                else 
                                {
                                      if(atof(Bal_Txn_Amount) > 0.0 )
                                      {
                                         strcat(additonal_data,"093");
                                         length_data = (strlen(Bal_Txn_Amount)+28+1);
                                         sprintf(length_name,"%03d",length_data);
                                         strncat(additonal_data,length_name,3);
                                         strncat(additonal_data,"Transaction allowed for Rs. ",28);
                                         strncat(additonal_data,Bal_Txn_Amount+0,(strlen(Bal_Txn_Amount)-2));
                                         strncat(additonal_data,".",1);
                                         strncat(additonal_data,Bal_Txn_Amount+(strlen(Bal_Txn_Amount)-2),2);
                                      }

                                }
                        }
                        #endif
                        if((Val == REDEEM_RESPONSE) || (Val == REDEEMVOID_RESPONSE)|| (Val == SALEVOID_RESPONSE )|| (Val == SALEREFUND_RESPONSE))
			{
				strcat(additonal_data,"079");
				strcat(additonal_data,"012");
				AvlBal = (char*)BFL_GetBalance (&bflzbcom);
				strncat(additonal_data,AvlBal,12);

				if (strlen(Ecs_Available) > 0 )
				{
					strcat(additonal_data,"401");
					strcat(additonal_data,"012");
					strncat(additonal_data,Ecs_Available,12);
				}

				strcat(additonal_data,"061");
				GetCardHolderPORTName(Card_name);
				length_data = strlen(Card_name);
				sprintf(length_name,"%03d",length_data);
				strncat(additonal_data,length_name,3);
				strncat(additonal_data,Card_name,length_data);

				strcat(additonal_data,"062");
				strcat(additonal_data,"015");
				strcat(additonal_data,"CS");
				strncat(additonal_data,paczisoauto.retrieval_reference_number,12);
				strcat(additonal_data," ");

				if(Mobile_No_Tranx == 1)
				{
					strcat(additonal_data,"084");
					strcat(additonal_data,"016");
					strncat(additonal_data,paczisoauto.primary_account_number,16);
					Mobile_No_Tranx = 0;
				}
			       if((strlen(RedeemPoint_Amount) > 0 ) && (Val == REDEEM_RESPONSE))
				{
					if(memcmp(paczisoauto.processing_code,"21",2) == 0) /****REDEEM BALANCE INQUIRY*******/
					strcat(additonal_data,"081");
					if(memcmp(paczisoauto.processing_code,"99",2) == 0) /****REDEEM TRANSACTION **********/
					strcat(additonal_data,"083");
					strcat(additonal_data,"012");
					strncat(additonal_data,RedeemPoint_Amount,12);
					strcat(additonal_data,"201");
					strcat(additonal_data,"019");
					strncat(additonal_data,bflzport.lport_cust_id,19);
				}
			       if((Val == REDEEMVOID_RESPONSE)&&(strlen(Requested_reward_point) > 0 ))
				{
					strcat(additonal_data,"082");
					strcat(additonal_data,"006");
					strncat(additonal_data,Requested_reward_point,6);

				}
			}
            }
         
  return(0);

}

void GetCardHolderPORTName(char *CharHolder_Name)
{

  char cCharHolder_Name_Full [sizeof(bflzport.lport_name_on_card) + 1 ];
  int iName_Pos = sizeof(bflzport.lport_name_on_card)- 1;

  memset(cCharHolder_Name_Full,0x00,sizeof(cCharHolder_Name_Full));
  memcpy(cCharHolder_Name_Full,bflzport.lport_name_on_card,sizeof(bflzport.lport_name_on_card));

  while((iName_Pos > 0))
  {
     if(cCharHolder_Name_Full[iName_Pos] != ' ')
     {
        memcpy(CharHolder_Name ,cCharHolder_Name_Full ,( iName_Pos + 1 ) );
        break ;
     }
     else
       iName_Pos-- ;
  }

}


int VerifyPIN_BFL(char *AccessCode,char *SubResul)
{
	f_printf(stderr,"In VerifyPIN_BFL\n");
	/*READ LN-CRDDET*/
	if(ReadCRDDET() != 0)
	{
		f_printf(stderr,"ERROR reading LN-CRDDET \n");
	}

	#ifdef BFL_PHASE2
	if((strlen(Otp_Request_id) > 0 )&&(strlen(Otp_Code) > 0 ))
	{
		int Otp_Result = -1 ;

		/* MRL_207 */
		if(memcmp(OTP_Verify_Flag,"Y",1)==0)
		{
			f_printf(stderr, "OTP Verify Flag is Y.\n");
			return ( SUCCESS );
		}
		else if(memcmp(OTP_Verify_Flag,"N",1)==0)
		{
			f_printf(stderr, "OTP Verify Flag is N.\n");

			if((bflcarddet.lcrd_wrong_otp_attempt[0]+1) >= bflzlpa03.lpa03_max_wrng_otp_allowed[0])
			memcpy(SubResul,"N00000",6);
			else
			memcpy(SubResul,"L30000",6);
			
			return ( ECHEC );
		}
		else
		{
			Otp_Result = VerifyOTP();
		}

		if(Otp_Result == SUCCESS )
		return ( SUCCESS );
		else if(Otp_Result == ECHEC)
		{
			/* MRL_207*/
			if((bflcarddet.lcrd_wrong_otp_attempt[0]+1) >= bflzlpa03.lpa03_max_wrng_otp_allowed[0])
			memcpy(SubResul,"N00000",6);
			else
			memcpy(SubResul,"L30000",6);
			
			return ( ECHEC );
		}
		else
		;
	}
	#endif

	if(memcmp(bflzport.lport_mxp_card_type,"99",2) == 0)
	return ( SUCCESS );

	if(strlen(access_code) == 4)
	memcpy(AccessCode,access_code,4);
	else
	memcpy(AccessCode,access_code+2,4);

	if(memcmp(bflzport.lport_mxp_card_type,"99",2) == 0)
	return ( SUCCESS );

	if(strlen(access_code) == 0)
	{
		memcpy(SubResul,"550007",6);

		if((bflcarddet.lcrd_wrong_pin_attempt[0]+1) >= bflzlpa03.lpa03_max_wrng_pin_allowed[0])
		memcpy(SubResul,"750009",6);
		return ( ECHEC );
	}

	if(memcmp(AccessCode,bflzport.lport_pin ,4) == 0)
	{
		f_printf(stderr, "\n  ACCESSCODE IS MATCHED \n");
		return ( SUCCESS );
	}
	else
	{
		f_printf(stderr, "\n 2. ACCESSCODE IS NOT MATCHING WITH THE ACCESS CODE FROM CARD MASTER");
		f_printf(stderr, "\n ACCESSCODE[%.6s],PORTE PIN[%.4s]\n",AccessCode,bflzport.lport_pin);


		if((bflcarddet.lcrd_wrong_pin_attempt[0]+1) >= bflzlpa03.lpa03_max_wrng_pin_allowed[0])
		memcpy(SubResul,"750007",6);
		else
		memcpy(SubResul,"550007",6);
		
		return ( ECHEC );
	}

}

int GetRedeemAmount(char *Redeem_Amt)
{

     double RedeemAmount = 0.0;
     double Redrate = 0.0;
     char   RedeemAmt[13];
     char   Redeemrate[6];

           memset(RedeemAmt,0,13);
           memset(Redeemrate,0,6);
           RoundOffEMI = 0.0;

           memcpy(Redeemrate,bflzlpa20.lpa20_redeem_rate,5);
           f_printf(stderr,"Redeem Rate [%.5s]\n",bflzlpa20.lpa20_redeem_rate);

           f_printf(stderr, "\n  Calculating the Redeem Amount \n");
        if(memcmp(paczisoauto.processing_code,"21",2) == 0)
        {
           if(strlen(RedeemPointBalance) > 0 )
           {
              Redrate = atof(Redeemrate);
             /* Redrate = (Redrate /100.00 ) ;*/
              f_printf(stderr, "\n  Redeem Rate [%f]\n",Redrate);
              RedeemAmount = (Redrate*atoi(RedeemPointBalance)); 

              f_printf(stderr,"\n.Before RoundDownRedeemAmount.[%f]",RedeemAmount);
	      RoundOffEMI = (RedeemAmount/100.00);
              RoundOffEmi(1);
	      RedeemAmount = (RoundOffEMI*100.00);
              f_printf(stderr,"\n.After RoundDownRedeemAmount.[%f]",RedeemAmount);

              f_printf(stderr, "\n  Redeem Amount [%f]\n",RedeemAmount);
              sprintf(Redeem_Amt,"%012.0f",RedeemAmount);
              f_printf(stderr, "\n  Redeem Amount [%s]\n",Redeem_Amt);
           }
           else
           memcpy(Redeem_Amt,"000000000000",12);
           return ( SUCCESS );
        }

        if(memcmp(paczisoauto.processing_code,"99",2) == 0)
        {
           if(strlen(Requested_reward_point) > 0 )
           {
              Redrate = atof(Redeemrate);
              /*Redrate = (Redrate /100.00 ) ;*/
              f_printf(stderr, "\n  Redeem Rate [%f]\n",Redrate);
              RedeemAmount = (Redrate*atoi(Requested_reward_point));
     
              f_printf(stderr,"\n.Before RoundDownRedeemAmount.[%f]",RedeemAmount);
	      RoundOffEMI = (RedeemAmount/100.00);
              RoundOffEmi(1);
	      RedeemAmount = (RoundOffEMI*100.00);
              f_printf(stderr,"\n.After RoundDownRedeemAmount.[%f]",RedeemAmount);
   
              f_printf(stderr, "\n  Redeem Amount [%f]\n",RedeemAmount);
              sprintf(Redeem_Amt,"%012.0f",RedeemAmount);
              f_printf(stderr, "\n  Redeem Amount [%s]\n",Redeem_Amt);
           }
           else
           memcpy(Redeem_Amt,"000000000000",12);
           return ( SUCCESS );
        }
}

int  Open_Dealer_Master(char *subre)
{
        char  file_open[50];
        int   fd_dea_mas = -1;

                memset(file_open,0x00,sizeof(file_open));
                sprintf(file_open,"%s/DEALER-MASTER",getenv("MultipacDat"));

                fd_dea_mas = isopen(file_open,ISINOUT+ISMANULOCK);
                if(fd_dea_mas < 0 )
                {
                        f_printf(stderr,"File [%s] Open Error [%d]",file_open,iserrno);
                        isclose(fd_dea_mas);
                        memcpy(subre,"K10000",6);
                        return(ECHEC);
                }
                memset(&bfl_dealer_master,0x20,sizeof(t_bfl_dealer_master));
                memcpy(bfl_dealer_master.mid,paczisoauto.card_acceptor_identification_code,15);

                if(isread(fd_dea_mas,(char *)&bfl_dealer_master,ISEQUAL) < 0 )
                {
                        f_printf(stderr,"DEALER MASTER RECORD [%.15s] READ Error [%d]",&bfl_dealer_master,iserrno);
                        isclose(fd_dea_mas);
                        memcpy(subre,"K10000",6);
                        return(ECHEC);
                }
                else
                f_printf(stderr,"DEALER-MASTER READ SUCCESS");
                isrelease(fd_dea_mas);
                isunlock(fd_dea_mas);
                isclose(fd_dea_mas);
                memcpy(subre,"000000",6);
           return ( SUCCESS );

}

int  Read_Card_Dealer_Master()
{
        char Merchant_Dealer_Code[11];
	char Merchant_Co_Dealer[9];
	char Card_Dealer_Code[11];
	char dealer_Co[11];
        char Card_Dealer[16];
        char Card_Deal_code[9];
        char Card_Dealer_co[9];
        char file_open[50];
        int  fd_dea_mas = -1;


        memset(file_open,0x00,sizeof(file_open));
	memset(Merchant_Dealer_Code,0x00,sizeof(Merchant_Dealer_Code));
	memset(Merchant_Co_Dealer,0x00,sizeof(Merchant_Co_Dealer));
	memset(Card_Dealer_Code,0x00,sizeof(Card_Dealer_Code));
	memset(dealer_Co,0x00,sizeof(dealer_Co));
	memset(Card_Deal_code,0x00,sizeof(Card_Deal_code));
	memset(Card_Dealer_co,0x00,sizeof(Card_Dealer_co));
        memset(Card_Dealer,'0',16);

	memcpy(Card_Dealer_Code,bflzport.lport_dealer_cd,10);
	memcpy(Merchant_Dealer_Code,bfl_dealer_master.dealer_grp_id,8);
	f_printf(stderr,"\n Dealer Code .....[%s]",Card_Dealer_Code);
        Trim_string(Card_Dealer_Code,dealer_Co,10);
        Trim_string(Merchant_Dealer_Code,Merchant_Co_Dealer,8);
        f_printf(stderr,"\n Dealer Code After Trim.....[%s]",dealer_Co);
        f_printf(stderr,"\n Length Dealer Code After Trim.....[%d]",strlen(dealer_Co));

        memcpy(Card_Dealer+0,"059",3);
        memcpy(Card_Dealer+(15-strlen(dealer_Co)),dealer_Co,strlen(dealer_Co)); 
        Card_Dealer[15] = '\0' ;
        f_printf(stderr,"\n CARD HOLDER DEALER.....[%s],Length[%d]",Card_Dealer,sizeof(Card_Dealer));


                memset(&bfl_dealer_master_card,0x20,sizeof(t_bfl_dealer_master));
                memcpy(bfl_dealer_master_card.mid,Card_Dealer,15);

                sprintf(file_open,"%s/DEALER-MASTER",getenv("MultipacDat"));

                fd_dea_mas = isopen(file_open,ISINOUT+ISMANULOCK);
                if(fd_dea_mas < 0 )
                {
                        f_printf(stderr,"File [%s] Open Error [%d]",file_open,iserrno);
                        isclose(fd_dea_mas);
                        return(ECHEC);
                }


                if(isread(fd_dea_mas,(char *)&bfl_dealer_master_card,ISEQUAL) < 0 )
                {
                        f_printf(stderr,"CARD DEALER MASTER RECORD [%.15s] READ Error [%d]",&bfl_dealer_master_card,iserrno);
                        isclose(fd_dea_mas);
                        return(ECHEC);
                }
                else
                {
                	f_printf(stderr,"CARD DEALER-MASTER READ SUCCESS");
                        memcpy(Card_Deal_code,bfl_dealer_master_card.dealer_grp_id,8);
                        Trim_string(Card_Deal_code,Card_Dealer_co,8);
                        if( memcmp(Card_Dealer_co,Merchant_Co_Dealer,strlen(Merchant_Co_Dealer)) != 0 )
                        {
                                f_printf(stderr, "\n 6.DDF CARD TYPE-CARD DEALER CODE [%s]and MERCHANT DEALER CODE [%s]is not matching\n",Card_Dealer_co,Merchant_Co_Dealer);
                	   	isrelease(fd_dea_mas);
                		isunlock(fd_dea_mas);
                		isclose(fd_dea_mas);
                                return(ECHEC);
                        }
                        	isrelease(fd_dea_mas);
                                isunlock(fd_dea_mas);
                                isclose(fd_dea_mas);
                }
           return ( SUCCESS );

}


void BFL_UpdateToLogFile(t_bflrthauth *prthauth, char cOperationFlag)
{
        char szFile[100]; 
        char szBuff[2500];
        FILE *bflfplog ;

        memset(szFile, 0x00, sizeof(szFile));
        memset(szBuff, 0x00, sizeof(szBuff));
        /* Open the file, if file is not present create the file */
        /* Copying the date and time */
        f_printf(stderr,"\nInside BFL_UpdateToLogFile\n");

        sprintf(szFile,"%s/BFLRTH-ONEVIEW-UPD.%s",getenv("MultipacEdp"),GetDateYYMMDD());

        if((bflfplog = fopen(szFile, "a+")) == NULL)
        {
                bflfplog = fopen(szFile, "w+");
                if(bflfplog == NULL)
                {
                     f_printf(stderr, "\n File %s Not opened\n", szFile);
                     f_printf(stderr,"Error: %d (%s)\n", errno, strerror(errno));
                     return;
                }
        }
        szBuff[0] = cOperationFlag ;
        memcpy(szBuff+1, prthauth, sizeof(t_bflrthauth));
	f_printf(stderr, "BFLRTH-RECORD[%s]\n", szBuff);

	/* Added the below line so that we can get record even in the case of buffering failure */
	fprintf(stderr, "BFLRTH-RECORD[%s]\n", szBuff);
        fprintf(bflfplog , "%s\n", szBuff);
        fflush(bflfplog);
        fclose( bflfplog );
        return;
}

void BFL_UpdateAccountAmount(
char *Current_Authorisation,
char *Current_EcsAuthorisation,
char *Reward_Balance, 
char *Cibil_Sco,
char *Cibil_Ti,
char *Prefered_Lim,
#ifdef MRL_288
char *RCU_Code,
char *RCU_Count,
#endif
char cOperationFlag)
{
        char szFile[100];
        char szBuff[2500];
        FILE *bflfplog ;

        memset(szFile, 0x00, sizeof(szFile));
        memset(szBuff, 0x00, sizeof(szBuff));
        /* Open the file, if file is not present create the file */
        /* Copying the date and time */
        f_printf(stderr,"\nInside BFL_UpdateAccountAmount \n");
        f_printf(stderr,"\nsize of CIBIL score [%d] sizeof Cibil Date [%d]",strlen(Cibil_Sco),strlen(Cibil_Ti) );

        sprintf(szFile,"%s/BFLACCOTBL-UPD.%s",getenv("MultipacEdp"),GetDateYYMMDD());

        if((bflfplog = fopen(szFile, "a+")) == NULL)
        {
                bflfplog = fopen(szFile, "w+");
                if(bflfplog == NULL)
                {
                     f_printf(stderr, "\n File %s Not opened\n", szFile);
                     f_printf(stderr,"Error: %d (%s)\n", errno, strerror(errno));
                     return;
                }
        }
        szBuff[0] = cOperationFlag ;
        strncat(szBuff, bflzbcom.lbcom_customer_id,19 );
        strncat(szBuff, Current_Authorisation,12 );
        strncat(szBuff, Current_EcsAuthorisation,12 );
        strncat(szBuff, Reward_Balance,10 );

        strncat(szBuff, Cibil_Sco,6);

        strncat(szBuff,Cibil_Ti ,6);
        strncat(szBuff,Prefered_Lim,12);

#ifdef MRL_288
	strncat(szBuff,RCU_Code,2);
	strncat(szBuff,RCU_Count,2);

	f_printf(stderr,  "%s\n", szBuff);
#endif
       if(strlen(szBuff) != 82 )
	{
		f_printf(stderr,"Error in BFLACCOTBL-UPD szBuff size [%d]",strlen(szBuff));
	}
	else 
	{
		fprintf(bflfplog , "%s\n", szBuff);
		fflush(bflfplog);
        }
	fclose( bflfplog );
        return;
}

char old_cibil_score[6], old_cibil_date[6], old_cibil_time[6];

void UpdateCibilScore(char *New_Cibil_Sco,char *New_Cibil_Sco_Date,char *Min_Cibil_Score,int Call)
{
   char buffer[50];
   char cibil_score[7];
   char customer_id[20];
	 int blockflag = 0;

   Cibil_Done = 1;
   memcpy(bflrthauth.cibil_done ,"Y",1);
   memset(buffer,0x00,sizeof(buffer));
	
   memcpy(old_cibil_score, bflzbcom.lbcom_cibil_score, 6);
   memcpy(old_cibil_date,  bflzbcom.lbcom_cibil_score_date, 6);
   memcpy(old_cibil_time,  bflzbcom.lbcom_cibil_score_time, 6);
   
   if(strlen(New_Cibil_Sco) <= 6 )
   {
        if(memcmp(New_Cibil_Sco+0,"-",1) != 0 )
        {
   		memcpy(bflzbcom.lbcom_cibil_score+0,"+",1);
   		memcpy(bflzbcom.lbcom_cibil_score+1,New_Cibil_Sco,strlen(New_Cibil_Sco));
        }
        else
        {
                memcpy(bflzbcom.lbcom_cibil_score+0,New_Cibil_Sco,strlen(New_Cibil_Sco));

        }
   }
	f_printf(stderr,"\nNew CIBIL Score : [%.6s],%d\n",bflzbcom.lbcom_cibil_score,strlen(New_Cibil_Sco));
   if(strlen(New_Cibil_Sco_Date) == 8)
   {
   	memcpy(bflzbcom.lbcom_cibil_score_date+0 ,New_Cibil_Sco_Date+6,2);
   	memcpy(bflzbcom.lbcom_cibil_score_date+2 ,New_Cibil_Sco_Date+2,2);
   	memcpy(bflzbcom.lbcom_cibil_score_date+4 ,New_Cibil_Sco_Date+0,2);
        memcpy(bflrthauth.cibil_date,New_Cibil_Sco_Date+0,4);
        memcpy(bflrthauth.cibil_date+4,New_Cibil_Sco_Date+6,2);
        if(atoi(New_Cibil_Sco) < atoi(Min_Cibil_Score))
        {
        }
	f_printf(stderr,"\n Updating CibilScore to LN-BCOM\n");
	isrewrite ( lnbcom , (char*)&bflzbcom);
	isunlock ( lnbcom );

	memset(customer_id, 0x00, sizeof(customer_id));
	memcpy(customer_id, bflzbcom.lbcom_customer_id, 19);
	customer_id[19] = '\0';

	memset(cibil_score, 0x00, sizeof(cibil_score));
	memcpy(cibil_score, bflzbcom.lbcom_cibil_score, 6);
	cibil_score[6] = '\0';

        f_printf(stderr,"TIME BEFORE CIBIL SCORE UPDATE[%.6s] PID[%d]\n",(char *)GetTime (),getpid());

        f_printf(stderr,"TIME AFTER CIBIL SCORE UPDATE[%.6s] PID[%d]\n",(char *)GetTime (),getpid());

	if((memcmp(New_Cibil_Sco,"00000",5) == 0 ) || (memcmp(New_Cibil_Sco,"000-1",5) == 0 )||
           (atoi(New_Cibil_Sco) == 0 ) || (atoi(New_Cibil_Sco) == 1 )|| (atoi(New_Cibil_Sco) == -1 ))
        {
                f_printf(stderr,"\nNO MATCH CASE.................\n");
                f_printf(stderr,"\nTransaction Should Not Get Declined For NO MATCH Cases,");
                f_printf(stderr," Account should not be Blocked for these scores");
                if(No_Match_Case  == 1)
                memcpy(New_Cibil_Sco,"00002",5);
                else
                memcpy(New_Cibil_Sco,Min_Cibil_Score,5);
        }

   }

	memcpy(bflzbcom.lbcom_cibil_score_time, (char*)GetTime(), 6);
	f_printf(stderr,"\nNew CIBIL Score date : [%.6s],%d\n",bflzbcom.lbcom_cibil_score_date);

}


/* MRL_233 */
int CheckChildTrasAmt(t_bflauth00* AuthRec)
{

        t_bflzlpa06 bflzlpa06;
        char percentage[16];
        int error = 0;
	int parent_amount;
	int retValue =0;
	int parent_amt = 0;
	int per_amt =0;

        memset(&bflzlpa06, ' ', sizeof(t_bflzlpa06));
        memcpy(bflzlpa06.lpa06_table_cd, "06", 2);
        memcpy(bflzlpa06.lpa06_bank_code,"00059", 5);
        memcpy(bflzlpa06.lpa06_product_id,AuthRec->product_id, 10);

	memset(percentage, 0x00, sizeof(percentage));

       retValue=isread(lnparam, (char *) &bflzlpa06, ISEQUAL );
	if(retValue == 0)
	{
		isrelease ( lnparam );
		isunlock  ( lnparam );

		memcpy(percentage, bflzlpa06.lpa06_cd_percentage, 3);
		percentage[3]='\0';

		if(memcmp(percentage, "   ", 3)==0)
			memcpy(percentage, "100", 3);

		per_amt = atoi(percentage);
		per_amt = (per_amt/100);	
	
		parent_amount = ((atoi( AuthRec->transaction_amount)) * (atoi(percentage)))/100;

		
	}
	else 
	{
		f_printf(stderr, "ERROR IN FETCHING PARENT PERCENTAGE-ERROR: %d\n", iserrno);
		memcpy(percentage, "100", 3);
		percentage[3]='\0';
		parent_amount = (atoi( AuthRec->transaction_amount)) * (atoi(percentage)/100);
	} 

	f_printf(stderr, "PARENT PERCENTAGE: =%.3s=, PARENT AMT: =%d=, CHILD AMOUNT: =%d=\n",
		percentage, parent_amount, paczisoauto.amount_transaction);


	if(atoi(paczisoauto.amount_transaction) <= parent_amount)
	{
		f_printf(stderr,"Child [%.12s] less than parent [%.12s]",
				paczisoauto.amount_transaction,
				AuthRec->transaction_amount);
		f_printf(stderr,"for product [%.10s] percentage [%.3s]",
				AuthRec->product_id, bflzlpa06.lpa06_cd_percentage);
		return(SUCCESS);
	}
	else
	{
		f_printf(stderr,"Child [%s] greater then parent [%s]"
			,paczisoauto.amount_transaction
			,AuthRec->transaction_amount);
		return(ECHEC);
	}
}


/* MRL_233 */
int CheckParentRecord(char *SubResult)
{
	int Result;
	struct keydesc skey;
	t_bflauth00   bflauth00;
	f_printf(stderr,"Inside CheckParentRecord");


	memset((char*)&bflauth00, 0x20, sizeof(t_bflauth00));
	memcpy(bflauth00.reference_number, parent_rrn, 12);
	
	Result = isread ( lnsauto, &bflauth00, ISEQUAL);
	if(Result!=0)
	{
		f_printf(stderr, "ISREAD PARENT TRANSACTION FAILED: %d\n", iserrno);
		memcpy(SubResult, "H50001", 6);
		return(ECHEC);
	}
	isunlock ( lnsauto );
		

	f_printf(stderr, "PARENT TRANSACTION[%.*s]\n", sizeof(bflauth00), &bflauth00);


	if((memcmp(bflauth00.relationship_type, "P",1)==0)
		&& (memcmp(bflauth00.auth_req_date,(char*)GetDateYYMMDD(),6)==0)
		&& (memcmp(bflauth00.terminal_no 
			,paczisoauto.card_acceptor_terminal_identification,8)==0)
		&& (memcmp(bflauth00.merchant_no 
			,paczisoauto.card_acceptor_identification_code,15)==0)
		)
	{
		NULL;
	}
	else
	{
		memcpy(SubResult, "H50030", 6);
		return(ECHEC);
	}


	f_printf(stderr, "Read iserrno [%d] \n", iserrno);
	f_printf(stderr," bflauth00 [%s] \n",bflauth00.reference_number);
	f_printf(stderr," RRN[%.12s]\n",paczisoauto.retrieval_reference_number);


	if(memcmp(bflauth00.auth_source_rev, "A",1) == 0)
	{
		if(CheckChildTrasAmt(&bflauth00)== SUCCESS)
		{
			f_printf(stderr,"1.PARENT TRANSACTION FOUND.\n");
			memcpy ( SubResult, "000000", 6 );
			return ( SUCCESS);
		}
		else
		{
			f_printf(stderr,"CHILD TRANSACTION GREATER THAN PARENT.\n");
			memcpy ( SubResult, "N20001", 6 );
			return(ECHEC);
		}
	}
	else
	{
		f_printf(stderr,"PARERNT TRANSACTION NOT AUTHORISED \n ");
		memcpy(SubResult, "N10001", 6);
		return(ECHEC) ;
	}


}


int CheckSubSequentTransaction(char *SubRes)
{
  /*  if((memcmp(transaction_source,"WP",2) != 0)&&(memcmp(transaction_source,"MP",2) != 0))
    return(SUCCESS);*/

    if(relation_type[0] == 'C')
    {
        f_printf(stderr,"Child Transaction SubsequentTransaction Check is not Required");
    	return(SUCCESS);
    }

    f_printf(stderr,"Inside CheckSubsequentTransaction");
    if(memcmp(bflzbcom.lbcom_vip_nonvip,"Y",1) == 0 )
    {
        f_printf(stderr,"VIP FALG IS ENABLED TO THIS CUSTOMER ,SUBSEQUENT TRANSACTION CHECK WILL NOT BE DONE \n");
        return ( SUCCESS );
    }

    if(memcmp ( bflzbcom.lbcom_last_auth_date,(char *)GetDateYYMMDD (), 6 ) == 0 )
    {

        memset ( (char*)&bflauth00, 0x20, sizeof ( t_bflauth00) );
    	if( BFL_CheckRecordInMovementAuth( paczisoauto.amount_transaction, &bflauth00,MSG_0102) == SUBSEQUENT)
    	{
                f_printf(stderr, "SUBSEQUENT RECORD EXISTS IN LN-SAUTO \n" );
                FPRINTF ( stderr, "merchant_no [%.15s]\n",bflauth00.merchant_no);
                FPRINTF ( stderr, "terminal_no[%.8s]\n",bflauth00.terminal_no);
                FPRINTF ( stderr, "Response Code[%.2s]\n",bflauth00.response_code);
       		return (ECHEC) ;
    	}
    	else
    	{
       		f_printf(stderr,"UNABLE TO FIND ANY SUBSEQUENT  TRANSACTION");
        	return(SUCCESS);
    	}
    }
    else
       		f_printf(stderr,"FIRST TRANSACTION OF THE DAY ");
    return(SUCCESS);
}

int GetNewDate(char *newdate_fdd)
{
  char newdate[9];
  char first_due_date[9];
  char transaction_date[9];
  char leap_year[3];

  memset(first_due_date,0x00,sizeof(first_due_date));
  memset(leap_year,0x00,sizeof(leap_year));
  memset(transaction_date,0x00,sizeof(transaction_date));
  memcpy(transaction_date,(char *)GetYYYYMMDDHHMMSS()+0,8);
  memcpy(first_due_date+0,transaction_date+0,6);
  if((memcmp(buisness_prod_code,"REMI",4)==0)
	|| (memcmp(buisness_prod_code,"MPC",3)==0)
	|| (memcmp(buisness_prod_code,"MPD",3)==0)
	)
  memcpy(first_due_date+6,"02",2);
  else
  memcpy(first_due_date+6,bflzbcli.lbcli_base_loan_fdd+4,2);
  memcpy(newdate,(char *)GetFddDate(first_due_date,transaction_date),8);
  memcpy(leap_year,newdate+2,2);
  if(memcmp(newdate+4,"02",2) == 0)
  {
  	if((atoi(leap_year)%4) == 0)
  	{
  		if((memcmp(newdate+4,"02",2) == 0)&&(memcmp(newdate+6,"29",2)> 0))
  		memcpy(newdate+4,"0301",4);
  	}
  	else
  	{
  		if((memcmp(newdate+4,"02",2) == 0)&&(memcmp(newdate+6,"28",2)> 0))
  		memcpy(newdate+4,"0301",4);
  	}
  }
  if((memcmp(newdate+4,"04",2) == 0)&&(memcmp(newdate+6,"30",2)> 0))
  memcpy(newdate+4,"0501",4);
  if((memcmp(newdate+4,"06",2) == 0)&&(memcmp(newdate+6,"30",2)> 0))
  memcpy(newdate+4,"0701",4);
  if((memcmp(newdate+4,"09",2) == 0)&&(memcmp(newdate+6,"30",2)> 0))
  memcpy(newdate+4,"1001",4);
  if((memcmp(newdate+4,"11",2) == 0)&&(memcmp(newdate+6,"30",2)> 0))
  memcpy(newdate+4,"1201",4);

  memcpy(newdate_fdd,newdate,8);
  f_printf(stderr,"NewDate [%s]\n",newdate_fdd);

}

char *GetFddDate(char *date1,char *transadate)
{

char date[7];
char date2[7];
memcpy(date2,date1+6,2);
memcpy(date2+2,date1+4,2);
memcpy(date2+4,date1+2,2);

if(memcmp(date1, transadate,8) > 0 )
{
  /*if(DateDifferenceInDays(date1,transadate) >= 10 ) */ /* Mail From Chandan on 24-05-2016 */
  if(DateDifferenceInDays(date1,transadate) >= 11 )
  {
        f_printf(stderr,"\n1. incremented Month[%s]",date1);
        return(date1);
  }
  else
  {
  memcpy(date2,IncMonth(date2),6);
  memcpy(date1+2,date2+4,2);
  memcpy(date1+4,date2+2,2);
  memcpy(date1+6,date2+0,2);
  f_printf(stderr,"\n3. incremented Month [%s]",date1);
  return(date1);
  }

}
else
{
  memcpy(date2,IncMonth(date2),6);
  memcpy(date1+2,date2+4,2);
  memcpy(date1+4,date2+2,2);
  memcpy(date1+6,date2+0,2);
 /* if(DateDifferenceInDays(date1,transadate) >= 10 ) *//* Mail From Chandan on 24-05-2016 */
  if(DateDifferenceInDays(date1,transadate) >= 11 )
  {
  f_printf(stderr,"\n6. incremented Month [%s]",date1);
  return(date1);
  }
  else
  {
  memcpy(date2,IncMonth(date2),6);
  memcpy(date1+2,date2+4,2);
  memcpy(date1+4,date2+2,2);
  memcpy(date1+6,date2+0,2);
  f_printf(stderr,"\n7. incremented Month REVERSE...........[%s]",date1);
  return(date1);
  }
}
}

int RoundOffEmi(int option)
{
  char Amount_EMI[13]; 
  memset(Amount_EMI,0x00,sizeof(Amount_EMI));
  RoundOffEMI = (RoundOffEMI*100.00);
  sprintf(Amount_EMI,"%012.0f",RoundOffEMI);
  f_printf(stderr,"\n.RoundOffEmi.[%s]",Amount_EMI);
  if(memcmp(Amount_EMI+10,"00",2) == 0)
  {
    RoundOffEMI = (RoundOffEMI/100.00 );
    return(0);
  }
  else
  memcpy(Amount_EMI+10,"00",2);
  RoundOffEMI = atof(Amount_EMI);
  RoundOffEMI = (RoundOffEMI/100.00 );
  f_printf(stderr,"\n.RoundOffEmi.[%f]",RoundOffEMI);
  if(option == 1)
  ;
  else
  RoundOffEMI += 1.0;
  f_printf(stderr,"\n.RoundOffEmi.[%f]",RoundOffEMI);
}

int GetNewDate1(char *newdate_fdd)
{
	char newdate[9];
	char first_due_date[9];
	char transaction_date[9];

	memset(first_due_date,0x00,sizeof(first_due_date));
	memset(transaction_date,0x00,sizeof(transaction_date));
	memcpy(transaction_date,(char *)GetYYYYMMDDHHMMSS()+0,8);
	memcpy(first_due_date+0,transaction_date+0,6);

	if(
	#ifdef MRL_485
		(BFL_parameter()==0)
	#else
	( memcmp(buisness_prod_code,"RDP",3) == 0 ) || ( memcmp(buisness_prod_code,"RVS",3) == 0 )
	    || ( memcmp(buisness_prod_code,"RDL",3) == 0 ) ||( memcmp(buisness_prod_code,"RCD",3) == 0 )
	    || ( memcmp(buisness_prod_code,"RLS",3) == 0 ) || (memcmp(buisness_prod_code,"RCF",3) == 0) || (memcmp(buisness_prod_code,"RDD",3) == 0)
	#endif 
	)
	memcpy(first_due_date+6,"05",2);
	else
	memcpy(first_due_date+6,"02",2);
	memcpy(newdate,(char *)GetFddDate(first_due_date,transaction_date),8);
	memcpy(newdate_fdd,newdate,8);
	f_printf(stderr,"NEW DATE FOR BLANK FDD [%s]\n",newdate_fdd);

}

int GetEcsTenureDate(char *new_dat)
{
   char tenure[6];
   char date2[7];
   char adv_tenure[4];

   memset(tenure,0x00,sizeof(tenure));
   memset(date2,0x00,sizeof(date2));
   memset(adv_tenure,0x00,sizeof(adv_tenure));


   if((memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) != 0)&&(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) != 0))
         GetNewDate(new_dat);
   else if ((memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) == 0)||(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) == 0))
         GetNewDate1(new_dat);

     memcpy(date2,new_dat+6,2);
     memcpy(date2+2,new_dat+4,2);
     memcpy(date2+4,new_dat+2,2);
     memcpy(tenure,procdata.bfl_loan_ten,5);
     memcpy(adv_tenure,procdata.adv_emicount,3);
     f_printf(stderr, "\n Loan Tenure [%.5s]\n" ,procdata.bfl_loan_ten);
     f_printf(stderr, "\n Adv Emi Count [%.3s]\n" ,procdata.adv_emicount);
     f_printf(stderr, "\n ECS FDD  [%.6s]\n" ,date2);
     for(int i = 1 ; i < (atoi(tenure)- atoi(adv_tenure));i++)
     memcpy(date2,IncMonth(date2),6);
     f_printf(stderr, "\n ECS..TENURE ENDS DATE  [%.6s]\n" ,date2);
     memset(new_dat,0x00,sizeof(new_dat));
     memcpy(new_dat,"20",2);
     memcpy(new_dat+2,date2+4,2);
     memcpy(new_dat+4,date2+2,2);
     memcpy(new_dat+6,date2+0,2);
     return(SUCCESS);
}

int Trim_string(char* str1,char* str2,int size)
{
    for(int i = 0 ; i < size ; i++)
    {
       if(memcmp(str1+i," ",1) != 0)
       memcpy(str2+i,str1+i,1);
       else
       {
         str2[i] = '\0' ;
         i = size;
       }

    }
return(0);
}

#ifdef BFL_PHASE2
int GetSMSNumber(char *Sms_Number)
{
  char Sms_Number_New[11];
  

  memset(Sms_Number_New,0x00,sizeof(Sms_Number_New));
  memcpy(Sms_Number_New,bflzlpa03.lpa03_sms_code_value ,10);/***************************/
  Trim_string(Sms_Number_New,Sms_Number,10);
  f_printf(stderr,"SMS Number [%s],Length[%d]",Sms_Number,strlen(Sms_Number));
  return(SUCCESS);
}

int VerifyOTP()
{

	if(strlen(Otp_Code)<= 0 )
		return(VERIFYPIN);

	if(strlen(Otp_Request_id) <= 0 )
		return(VERIFYPIN);

	f_printf(stderr,"\nOTP CHECK\n");

#ifdef MRL_207_REMOVE
	if((memcmp(transaction_source,"EI",2) != 0)&&(memcmp(transaction_source,"EP",2) != 0))
	{
	f_printf(stderr,"INVALID TRANSACTION SOURCE CONTAINING OTP");
	return (ECHEC);
	}
#endif


	f_printf(stderr,"TIME BEFORE OTP CHECK[%.6s] PID[%d]\n",(char *)GetTime (),getpid());
#ifdef MRL_324
	if(bflOtpCheck(Otp_Request_id, Otp_Code)!= 0 )
#else
	if(bfl_otp_check(Otp_Request_id, Otp_Code)!= 0 )
#endif 
		return (ECHEC) ;

	f_printf(stderr,"TIME AFTER  OTP CHECK[%.6s] PID[%d]\n",(char *)GetTime (),getpid());

	return(SUCCESS);
}

int BFL_Gross_Amount(char *Adv_Emi_Amount, char *Processing_Fee, char *Card_Fee)
{
      double down_payment ;
      char   Down_Payment[13];
      memset(Down_Payment,0x00,sizeof(Down_Payment));
      f_printf(stderr,"\n BFL_Gross_Amount inputs Adv_Emi_Amount [%.12s],Processing_Fee [%.12s] ,Card_Fee[%.12s] \n", Adv_Emi_Amount,Processing_Fee ,Card_Fee);
      down_payment = atof(Adv_Emi_Amount)+ atof(Card_Fee) + atof(Processing_Fee);
      sprintf(Down_Payment,"%012.0f",down_payment);
      f_printf(stderr,"\n BFL_Gross_Amount [%s]\n", Down_Payment);
      memcpy(bflrthauth.down_payment,Down_Payment,12);
      memcpy(bflrthauth.emi_amount,procdata.emiamount,12);
      memcpy(bflrthauth.dbd,procdata.DBD,10);
      return (0);
}

int GetCustomerAge(int *Current_Age)
{
	char Curren_Date[9];
        char Customer_Birth_Date[9];
        char Curren_Year[5];
        char Birth_Year[5];
        char Curren_Month[5];
        char Birth_Month[5];
        
        memset(Curren_Date,0x00,sizeof(Curren_Date));
        memset(Customer_Birth_Date,0x00,sizeof(Customer_Birth_Date));
        memset(Curren_Year,0x00,sizeof(Curren_Year));
        memset(Birth_Year,0x00,sizeof(Birth_Year));
        memset(Curren_Month,0x00,sizeof(Curren_Month));
        memset(Birth_Month,0x00,sizeof(Birth_Month));
   
        if((memcmp(bflzbcli.lbcli_dob,"00000000",8) != 0 ) && (memcmp(bflzbcli.lbcli_dob,"        ",8)!= 0 ) )
        {
        	memcpy(Curren_Date,(char *)GetDateYYYYMMDDHHMMSS() + 0,8);
        	memcpy(Customer_Birth_Date,bflzbcli.lbcli_dob,8);
                memcpy(Curren_Year,Curren_Date+0,4); 
                memcpy(Birth_Year,Customer_Birth_Date+0,4); 
                *Current_Age = (atoi(Curren_Year) - atoi(Birth_Year));
                if((*Current_Age == 25 ) || (*Current_Age == 61 ) || (*Current_Age == 21 ) || (*Current_Age == 66 ) )
                {
                  	memcpy(Curren_Month,Curren_Date+4,4);
                        memcpy(Birth_Month,Customer_Birth_Date+4 ,4);

                        f_printf(stderr," Current Age of Customer %d\n",*Current_Age );
                        if(atoi(Birth_Month)> atoi(Curren_Month))
                        *Current_Age = (*Current_Age - 1 );

                        f_printf(stderr,"Derived Current Age of Customer %d\n",*Current_Age );
                }
              
        }
        else
        {
             f_printf(stderr,"INVALID DATE OF BIRTH OF CUSTOMER [%.8s]\n",bflzbcli.lbcli_dob);
             return (ECHEC) ;

        }

        return (SUCCESS);
}
#endif
#endif


char *GetProdAmnt_Offset(char *offset, t_cust_prod_amount* cust_prod, char* value, int Action, int Member)
{

	switch(atoi(offset))
	{
	case 0:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_1, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_1, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_1, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_1,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_1,value, 1);	
		}
		break;
	case 1:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_2, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_2, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_2, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_2,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_2,value, 1);	
		}
		break;
	case 2:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_3, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_3, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_3, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_3,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_3,value, 1);	
		}
		break;
	case 3:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_4, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_4, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_4, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_4,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_4,value, 1);	
		}
		break;
	case 4:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_5, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_5, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_5, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_5,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_5,value, 1);	
		}
		break;
	case 5:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_6, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_6, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_6, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_6,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_6,value, 1);	
		}
		break;
	case 6:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_7, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_7, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_7, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_7,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_7,value, 1);	
		}
		break;
	case 7:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_8, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_8, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_8, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_8,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_8,value, 1);	
		}
		break;
	case 8:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_9, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_9, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_9, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_9,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_9,value, 1);	
		}
		break;
	case 9:
		if(Action == 1)
		{	
			if(Member == 1)
				memcpy(value, cust_prod->total_amount_10, 12);	
			else if(Member == 2)	
				memcpy(value, cust_prod->sms_flag_10, 1);	
			else if(Member ==3)
				memcpy(value, cust_prod->pos_flag_10, 1);	
		}
		else if(Action == 2)
		{
			if(Member == 1)
				memcpy(cust_prod->total_amount_10,value, 12);	
			else if(Member == 2)
				memcpy(cust_prod->sms_flag_10,value, 1);	
		}
		break;
	default:
		break;	
	}

	return 0;
}

#ifdef BFL_REMI_TO_PL

void CheckCustProdAmount(int *amountBreachedFlag, int *smsFlag, int *pos_sms_applicable)
{

        t_remi_pl_rules remi_pl_rules;
        t_product_master product_master;
       /* t_cust_prod_amount cust_prod_amount;*/

        char    prod_offset[16];
        int     iprod_offset;

        char    total_amount[12+1];
        int     itotal_amount;

        char    prod_cap_limit[12+1];
        int     iprod_cap_limit;

        char    txn_emi_amount[12+1];
        int     itxn_emi_amount;
	int PUT =1;

	char cust_open[100];
        int lnamnt =0;

	 *amountBreachedFlag=0;
        *smsFlag=0;
	*pos_sms_applicable=0;


        memset(&remi_pl_rules, ' ', sizeof(remi_pl_rules));
        memset(&product_master, ' ', sizeof(product_master));
        memset(&cust_prod_amount, ' ', sizeof(cust_prod_amount));
	


        memcpy(product_master.product_id, buisness_prod_code, strlen(buisness_prod_code));
        if(isread(lnprod, &product_master, ISEQUAL)<0)
        {
                f_printf(stderr, "No setup found for Product Code [%s]\n", buisness_prod_code);
                return;
        }
        f_printf(stderr, "Product Master read.\n");


        memcpy(remi_pl_rules.pl_tag, bflzbcom.lbcom_pl_tag, 5);
        memcpy(remi_pl_rules.product, buisness_prod_code, strlen(buisness_prod_code));
        if(isread(lnrules, &remi_pl_rules, ISEQUAL)<0)
        {
                f_printf(stderr, "No setup found for Product Code [%s] and Customer Risk [%.3s]\n",
                        buisness_prod_code, bflzbcom.lbcom_cust_risk_clasif);
                return;
        }
        f_printf(stderr, "Rules Master read.\n");

	memset(cust_open,0x00,sizeof(cust_open));
	sprintf(cust_open,"%s/LN-CUST",getenv("MultipacDat"));

	lnamnt = isopen(cust_open,ISINOUT+ISMANULOCK);
	if(lnamnt < 0 )
	{
		f_printf(stderr," LN-CUST ERROR File [%s] Open Error [%d] 960005",cust_open,iserrno);
		isclose(lnamnt);
		return;
	}



        memcpy(cust_prod_amount.customerid, bflzbcom.lbcom_customer_id, 19);
        if(isread(lnamnt, &cust_prod_amount, ISEQUAL)<0)
        {
                f_printf(stderr, "Customer Data Read error. Customer [%.19s]. Error: [%d]\n",
                        bflzbcom.lbcom_customer_id, iserrno);
		if(ENOREC == iserrno)
		{
			f_printf(stderr, "Customer Data missing. Creating record [%.*s]\n", 
				sizeof(cust_prod_amount), &cust_prod_amount);
			if(iswrite(lnamnt, &cust_prod_amount)<0)
			{
				f_printf(stderr, "Error in creating new record. Error: [%d]\n",	
					iserrno);
			}
		}
	isunlock( lnamnt );
	isrelease( lnamnt );	
        }
	
	isclose(lnamnt);

        f_printf(stderr, "Customer Product Amount read.\n");
        memcpy(prod_offset, product_master.product_offset, 2);
        prod_offset[2]='\0';
        iprod_offset = atoi(prod_offset);
	
	f_printf(stderr, "Product Offset: [%.2s]/[%d]\n", prod_offset, iprod_offset);
	
	GetProdAmnt_Offset(prod_offset, &cust_prod_amount, total_amount, PUT,1);
	
        total_amount[12]='\0';
        itotal_amount = atoi(total_amount);

        memcpy(txn_emi_amount, emi_amount, 12);
        txn_emi_amount[12]='\0';
        itxn_emi_amount = atoi(txn_emi_amount);

        memcpy(prod_cap_limit, remi_pl_rules.limit, 12);
        prod_cap_limit[12]='\0';
        iprod_cap_limit = atoi(prod_cap_limit);

        f_printf(stderr, "Product Offset     : [%s]/[%d]\n", prod_offset, iprod_offset);
        f_printf(stderr, "Total Amount       : [%s]/[%d]\n", total_amount, itotal_amount);
        f_printf(stderr, "EMI Amount         : [%s]/[%d]\n", txn_emi_amount, itxn_emi_amount);
        f_printf(stderr, "EMI Amount + Total : [            ]/[%d]\n", itotal_amount + itxn_emi_amount);
        f_printf(stderr, "Product Cap Limit  : [%s]/[%d]\n", prod_cap_limit, iprod_cap_limit);


	if( itotal_amount >= iprod_cap_limit )
	{
		*amountBreachedFlag = 1 ;
		return ;
	}


        if((itotal_amount + itxn_emi_amount)  >= iprod_cap_limit)
        {
                sprintf(remi_to_pl_breach_amount, "%012d", itotal_amount + itxn_emi_amount);
                *smsFlag=1;
        }
	/* MRL_332 */
	else
	{
		*pos_sms_applicable = 1;
		f_printf(stderr, "POS CAP Applicable.\n");
	}
        return;
}

#endif
