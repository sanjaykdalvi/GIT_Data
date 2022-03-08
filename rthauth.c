
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : rthauth.c                                           *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 02-mar-2000                                         *
 * Description         : Loading and Writing RTH AUTH record to a file       *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   19-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   13-Dec-04 Veeresh E            Country wise Risk Parameter logic      * 
 *---------------------------------------------------------------------------*
 * 2   13-Dec-04 Michel Alex      Reviewed-Country wise Risk Parameter logic * 
 * 3   29-May-06 Veeresh              BR-MXP-0605-0033                       *
 *---------------------------------------------------------------------------*
 * 4   31-Jul-07 Michel Alex      Corp Bank SMS Implementation               * 
 *****************************************************************************/

/* Added for IDBI Bank Code - 00086 */
#define IDBI_SMS        1
#define SELECT_CITY     1
#define PHOENIX_MALL    1 

#define CORP_LIC    1
#define IDBI_PHASE2 1
#define IDBI_NEWSMS 1
#define DECLINE_SMS 1
#define SFLP 1

#define MRL_496	1

#define VIJAYA_PHASE_3 1

/*#define  QR_ISSUANCE 1*/

#define BOI_TRAVEL_NEWBIN  1

#define BFL_BLOCK_SMS 1

#define SFLP_PHASE2   1

#define  SHELL_RELOAD_VOID_IS  1

#define EDEN_ACT_DR   1

#define  BOBL_PREPAID 1
   #define		LODHA_PALAV     1 
/* #define   HSBC_DOP_POS   1 */

/* Added for additional changes for UBI Sms */
/* #define UBI_ADDITIONAL_SMS 1  */

 #define SMS_ALLERT    /* chowdesha SMS Allert */
 #define NEW_SMS_MERCHANT 1

#define IDBI_INTERNATIONAL_SMS

#define ADDON_PHASE_2

#define MRL_204 1

#ifdef ADDON_PHASE_2
        extern int Check_First_Txn();
        extern int first_txn ;
        extern lnporte ;
        void Card_Block_Status();
#endif

extern char txn_mob_num[16];


#define MRL_264 1

#define MRL_197 1

 #define NB_FOIS_2 250
 #define ONEVIEW_EMAIL 1
 #define KOTAK_UPDATION 1
 #define BFL_CUG_CARD 1
 #define ADDON        1   
 #define BFL_PHASE2   1

#define NEW_EDEN_SMS 1
#define INTL_SMS_ENABLE 1

#define BOI_GPPC  1

#define RUPAY_ISSUANCE   1   /* Added by Amit for RUPAY Issuance on 28 OCT 2016*/

#define VISA_APR_2017_MANDATE 1 /* Added by Amit for Visa Apr 2017 Mandates */

#define EDENRED_REPORT 1 /* Added by Amit on 7 APR 2017 for PREQ2968 Eden Red Report related Changes */

#define EDENRED_MOBILE_TXN 1  /* Added by Amit for Edenred Mobile Transaction on 26 Apr 2017 */

/* Added for  changes for UBI Sms  22.04.2014*/
 #define UBI_SMS 1  

#define MRL_238

#ifdef MRL_238
        extern int Ecs_Waiver_Sms;
#endif

#define MRL_207 1

#define MRL_287 1

#define MRL_288
extern char rsa_flag[];

extern char mob_num_change_flag[];

 /* Added by Ankit for PREQ2237 on 19-Oct-2015 */
 #define UBI_PREQ2237 1
 #define SHELL_VOID 1
 #include <stdio.h>
 #include <isam.h>
 #include <rthauth.h>
 #include <paczpoau.h>
 #include <pcrzbale.h>
 #include <paczisoauto.h>
 #include <struct_iso.h>
 #include <paczcomm.h>


 #include <string.h>
 #include <paczpa05.h>
 #include <pcrztb73s.h>
 #include <paczport.h>
 #include <sms-msg.h>
 #include <paczpaSQ.h>
 #include <pcrzbcom.h>
 #include <pcrzbcli.h>
 #include <bankacnt.h>
 #include <ath-trc.h>
 #include <onuspr05.h>
 #ifdef ONEVIEW_EMAIL 
 #include <paczpa11.h>
 #include <email_alert.h>
 extern   t_paczpa11  paczpa11;
 t_email_alert email_alert ;
 #endif
 #ifdef DECLINE_SMS
 #include <paczresult.h> /* Added by Ankit for Edenred Decline_sms CR */
 #include <paczpa02.h>
 extern t_paczpa02 paczpa02;
 #endif

#include<sys/time.h>

 #include <pcrztb98.h>
 t_pcrztb98 pcrztb98;

 t_sms_data smsdata;

 t_pcrzbcom pcrzbcom;

 t_pcrzbcli pcrzbcli;
 t_pcrztb73s  pcrztb73s;
 extern t_rthauth		rthauth;
 extern t_paczpoau		paczpoau;
 extern t_pcrzbale		pcrzbale;
 extern t_paczisoauto	        paczisoauto;
 extern t_paczpa05	        paczpa05;
 extern t_ath_trc		athtrc;	
 extern t_onuspr05               onuspr05;
 extern t_bankacnt               bankacnt;  

 extern double atof();
 char sms_amount[13];
 char sms_crdt_limit[13];
 double sms_tran_amount = 0.00;
 double sms_crdt_chk_limit = 0.00;



#define RULE_ENGINE 1

#ifdef RULE_ENGINE
	extern char failedRule[];
#endif

#define BFL_REMI_TO_PL

#ifdef BFL_REMI_TO_PL
       extern int             remi_to_pl_sms;
       extern char            remi_to_pl_breach_amount[];
       extern int lnsauto;
       extern int lnhauto;
       #include <bflauth00.h>
       #include <constantes.h>
       void get_cumul_auth_amount(char *cust, char *p_cumul_auth, char *p_cumul_ecs, int type);
#endif

 char sms_limit[13];
 double isms_limit = 0.00 , isms_amount = 0.00;

int ForInt = 0;
int StrLen = 0;
int tot_count = 0;  
int space_len = 0;
int space_count = 0;

char TempStr;
char FormatedString [50];
char FormatedMerName [25];
char FormatedCity [ 13 ];
char Mer_Name[26];
int  Xml_count; 
int  snFlagAlarm;
char temp_mer [26];
char sms_seq [3];

extern int RBI_SMS_FLAG;

extern int iCNP_Max_Attempts;

extern char gstr_feesdet[250]; 

char dop_amount[13];
double dop_tran_amount = 0.00;
char title [5];
extern int      fdparam;
extern int      fdonusparam;

#ifdef BFL_CUG_CARD
#include <onus.h>
#include <loan_email_sms.h>
#include <bflzbcom.h>
#include <bflzport.h>
#include <bflzbcli.h>
#include <bflzparm.h>
#include <bfl_rthauth.h>
#include <BFLPROC.h>
#include <bfl_dealer_master.h>
#include <bflzaddon.h>

extern t_bfl_dealer_master bfl_dealer_master;
extern t_procdata    procdata;
extern t_bflzport    bflzport;
extern t_bflzbcom    bflzbcom;
extern t_bflzbcli    bflzbcli;
extern t_bflrthauth  bflrthauth;
extern t_lnaddport   lnaddport;

extern int lnrth;   /*BFL RTH*/
extern int Card_First_Trans;
extern int chargeslip_first_fee;
extern int rset_card_1st_fee;
extern int NXTALLD;
void BFL_LoadRthAuthRecord(t_struct_iso*);
int  BFLEmailSMS(t_struct_iso* ISOStruct);
extern int   CheckCoBrandCard();
extern void  BFL_UpdateToLogFile(t_bflrthauth *prthauth1, char cOpFlag);
extern char  parent_rrn[];
extern char  Prefered_Limit[];
extern char* GetDateInc(int);
#endif


/* FUNCTION PROTOTYPE */
void PRINTAUTH( char * rth_auth);
void trim_leading(char *, char );
void GetCardHolderName(char *);

extern char* GetDateYYYYMMDD ( void );
extern char* GetDateYYMMDD ( void );
extern int iUpdate_rth( char * );

extern char card_no[];
int iRet_val;

extern int Card_Activation_Flag;

extern double gcba_fees_amt;

int Porte_Mobile_No ;
#ifdef INTL_SMS_ENABLE
int sms_tobe_send = 0;/* 1 For send  0 For Not to Send */
#endif

#ifdef HSBC_DOP_POS
	extern int DoPBank(char *bank_code); 
#endif

#ifdef VIJAYA_PHASE_3
char* rtrim(char* string);
extern int  decline_sms_balance ;
#endif
#ifdef SHELL_VOID
int     fdcomm = -1;     /* Bhagwat */
t_paczcomm paczcomm;
char Invoice_Number [7];
#endif


extern t_bflzlpa03   bflzlpa03;

#ifdef NEW_SMS_MERCHANT
char Merchant_Name[41];
int StringAlign()
{
 extern char User[];
 char Merchant_Name_Full[41];
 int pos_start = 0;
 int pos_start_pos = 0;

 memset(Merchant_Name_Full,0x00,sizeof(Merchant_Name_Full));
 memset(Merchant_Name,0x00,sizeof(Merchant_Name));
 memcpy(Merchant_Name_Full,rthauth.card_acceptor_name_location,40);

 while(pos_start < 40)
 {
     if((pos_start == 25)&&((memcmp(User, "VISA", 4) == 0)||(memcmp(User, "ADVISA", 6) == 0)||(memcmp(User, "APPLY", 5) == 0)||(memcmp(User, "PosISO", 6 )== 0 )))
     {
        if(pos_start_pos >= 23)
        {
        	memcpy(Merchant_Name+pos_start_pos ,", ", 2 );
        	pos_start_pos++;
        	pos_start_pos++;
        }
        else
        {
                pos_start_pos = pos_start_pos - 1 ;
        	memcpy(Merchant_Name+pos_start_pos ,", ", 2 );
        	pos_start_pos++;
        	pos_start_pos++;
        }
     }

     if(memcmp(Merchant_Name_Full+pos_start,"  ",2)!= 0 )
     {
        memcpy(Merchant_Name+pos_start_pos ,Merchant_Name_Full+pos_start , 1 );
        pos_start_pos++;
        pos_start++;
     }
     else
     {
        pos_start++;
     }

 }
 f_printf(stderr,"Merchant Name...... [%s]\n",Merchant_Name_Full);
}
#endif

int   GetSeqSMS(sequence,fd_file)
char  *sequence;
int   fd_file;
{
        t_paczpaSQ      variable;
        int             k;
        int	cc1 = -1;
	int     cc2 = -1;
        int             value;
        char            Sequence [3];

        (void)memcpy(variable.paSQ_notable,"SQ",2);
        (void)memset(variable.paSQ_suitecle,' ',30);
        k = 0;
        do
        {
            cc1 = isread(fd_file,(char *)&variable,ISEQUAL);
            k++;
        } while ( (iserrno == ELOCKED) && ( k<NB_FOIS_2 ) );

        if (cc1 < 0)
	{
	   f_printf(stderr, "Isread error table SQ[%d]\n", iserrno );
        }
	else 
        {
           if(memcmp(variable.paSQ_sequenceSMS, "  ", 2) == 0)
               memcpy(variable.paSQ_sequenceSMS, "01", 2);

           (void)memcpy(Sequence,variable.paSQ_sequenceSMS  ,2);

           Sequence[2] = 0;
           value = atoi(Sequence) + 1;
           if ( value > 99 ) value = 1;

           (void)sprintf( Sequence,"%02d",value );
           (void)sprintf( sequence,"%02d",value );
           (void)memcpy(variable.paSQ_sequenceSMS, Sequence, 2 );

	   if ( cc1 == 0 )
	   {
        	cc2 = isrewrite(fd_file,(char *)&variable);
                isrelease(fd_file);   
        	isunlock(fd_file);

        	if (cc2 < 0)
		{
			f_printf(stderr,"\n SMS Write  iserrno = %d\n", iserrno);
		        return(ECHEC);
		}
	   }
        }
        isrelease(fd_file);   
        isunlock(fd_file);

	return(SUCCES);
}


void LoadRthAuthRecord( t_struct_iso* ISOStruct )
{

	if( memcmp( ISOStruct->entete.banque, "00059", 5 ) == 0 )
	{
		BFL_LoadRthAuthRecord( ISOStruct);
		return;
	}

	return ;
}

int iUpdateRTH()
{

	f_printf(stderr, "\n In RTH Update "); 

	if( memcmp( paczpa05.pa05_codbnq, "00059", 5 ) == 0 )
		BFL_UpdateToLogFile(&bflrthauth, 'C');

	f_printf(stderr, "ONEVIEW DATABASE RTH UPDATE SUCCESS \n"  );
return(0);
}

/*  Function to check and remove leading zeroes(character to trim) from the buffer  */
void trim_leading(char *dest, char char_to_trim)
{
        char *p=strdup(dest);

        if(*p)
        {
            while (*p && *p == char_to_trim) p++;
            strcpy(dest,p);
        }

	free(p);
}

void GetCardHolderName(char *cCharHolder_Name_P)
{

  char cCharHolder_Name_Full [sizeof(paczpoau.poau_nom) + 1 ];
  int iName_Pos = sizeof(paczpoau.poau_nom)- 1;

  memset(cCharHolder_Name_Full,0x00,sizeof(cCharHolder_Name_Full));
  memcpy(cCharHolder_Name_Full,paczpoau.poau_nom,sizeof(paczpoau.poau_nom));

  while((iName_Pos > 0))
  {
     if(cCharHolder_Name_Full[iName_Pos] != ' ')
     {
        memcpy(cCharHolder_Name_P ,cCharHolder_Name_Full ,( iName_Pos + 1 ) ); 
        break ;
     }
     else
       iName_Pos-- ;
  }

}


/* Function added by Ankit for Edenred transaction decline */
#ifdef DECLINE_SMS
char* rtrim(char* string)
{
        char* original = string + strlen(string);
        while(*--original == ' ' && original>=string);
        *(original + 1) = '\0';
        return string;
}

#endif

#ifdef BFL_CUG_CARD
void  BFL_LoadRthAuthRecord( t_struct_iso* ISOStruct )
{
	char          *temp;
	t_paczisoauto bfl_authorisation_request;
	extern        char Loan_Value_Ratio[];
	int           i = 0,value = 0,addon = 0 ;
	int           Size_Auth_Record = 0 ;
	char          date_time[15];
	char          branchid[10];
	char          Auth_String[1600];
	char          De_Code[11];
	char          De_Code1[11];
	char          Pre_Card_Limit[13];
	char          make_produ[36];
	char          process_type[21];
	extern char   promo_code[ ]; 
	char Card_first_fee[13];

	memset(Auth_String,0x00,sizeof(Auth_String));
	memset(Pre_Card_Limit,0x00,sizeof(Pre_Card_Limit));
	memset(date_time,0x00,sizeof(date_time));
	memcpy(date_time,GetDateYYYYMMDD(),8);
	memcpy(date_time+8,(char *)GetTime(),6);
	memcpy ( (char*)&bfl_authorisation_request,(char*)&paczisoauto, sizeof ( t_paczisoauto ) );
	memset(Card_first_fee,0x00,sizeof(Card_first_fee));
	temp = (char *)&bfl_authorisation_request;

	for ( i=0; i < sizeof(t_paczisoauto); ++i)
		if ( *(temp+i) == '\0' )
			*(temp+i) = ' ';

	memcpy(bflrthauth.reference_number ,  bfl_authorisation_request.retrieval_reference_number , 12 ); /* Primary Key */
	f_printf(stderr,"Reference No [%.12s]",bflrthauth.reference_number);

	/** DATA **/
	memcpy(bflrthauth.card_number,bflzport.lport_card_no , 19 );
	if(memcmp(paczisoauto.primary_account_number,"203040",6 ) == 0 )
		memcpy(bflrthauth.card_number,paczisoauto.primary_account_number, strlen ( paczisoauto.primary_account_number ) );
	else
		memcpy(bflrthauth.card_number,bflzport.lport_card_no , 19 );

	memcpy(bflrthauth.merchant_code ,bfl_authorisation_request.card_acceptor_identification_code ,15);
	memcpy(bflrthauth.primary_account_number,bflzport.lport_cust_id, 19);
	memcpy(bflrthauth.processing_code,bfl_authorisation_request.processing_code,  processing_code_len);
	memcpy(bflrthauth.amount_transaction,bfl_authorisation_request.amount_transaction,amount_transaction_len);
	memcpy(bflrthauth.amount_cardholder_billing ,"000000000000"    ,12 );
	memcpy(bflrthauth.transmission_date_time  ,date_time+6,2);
	memcpy(bflrthauth.transmission_date_time+2  ,date_time+4,2);
	memcpy(bflrthauth.transmission_date_time+4  ,date_time+0,4);
	memcpy(bflrthauth.transmission_date_time+8  ,date_time+8,6);
	memcpy(bflrthauth.conversion_rate_cardholder_billing,"00000000" , 8 );
	memcpy(bflrthauth.system_trace_audit_number , bfl_authorisation_request.system_trace_audit_number,system_trace_audit_number_len );
	memcpy(bflrthauth.date_expiration   ,bfl_authorisation_request.date_expiration,date_expiration_len);
	memcpy(bflrthauth.merchant_type, "9999" , 4 );
	memcpy(bflrthauth.acquiring_institution_country_code ,bfl_authorisation_request.acquiring_institution_country_code,acquiring_institution_country_code_len );
	memcpy(bflrthauth.primary_account_number_extended_country_code ,bfl_authorisation_request.primary_account_number_extended_country_code,3);
	memcpy(bflrthauth.pos_entry_mode_code,bfl_authorisation_request.pos_entry_mode_code,pos_entry_mode_code_len);
	memcpy(bflrthauth.pos_condition_code,bfl_authorisation_request.pos_condition_code,pos_condition_code_len);
	memcpy(bflrthauth.pos_pin_capture_code,bfl_authorisation_request.pos_pin_capture_code,pos_pin_capture_code_len);
	memcpy(bflrthauth.acquiring_institution_identification_code,bfl_authorisation_request.acquiring_institution_identification_code,acquiring_institution_identification_code_len);
	memcpy(bflrthauth.forwarding_institution_identification_code,bfl_authorisation_request.forwarding_institution_identification_code,forwarding_institution_identification_code_len);
	memcpy(bflrthauth.primary_account_number_extended ,bfl_authorisation_request.primary_account_number_extended,primary_account_number_extended_len);
	memcpy(bflrthauth.card_acceptor_terminal_identification,bfl_authorisation_request.card_acceptor_terminal_identification,card_acceptor_terminal_identification_len);
	memcpy(bflrthauth.card_acceptor_identification_code,bfl_authorisation_request.card_acceptor_identification_code,card_acceptor_identification_code_len);
	memcpy(bflrthauth.card_acceptor_name_location, bfl_authorisation_request.card_acceptor_name_location,card_acceptor_name_location_len);;
	memcpy(bflrthauth.currency_code_transaction ,"356", 3 );
	memcpy(bflrthauth.currency_code_settlement,"356", 3 );
	memcpy(bflrthauth.currency_code_cardholder_billing ,"356",3);
	memcpy(bflrthauth.pos_entry_capability_code,bfl_authorisation_request.pos_entry_capability_code,1);
	memcpy(bflrthauth.other_fees,"000000000000",12);
	memcpy(bflrthauth.date_action,bfl_authorisation_request.date_action,date_action_len);
	memcpy(bflrthauth.original_data_elements,bfl_authorisation_request.original_data_elements,original_data_elements_len);
	memcpy(bflrthauth.file_update_code ,bfl_authorisation_request.file_update_code,file_update_code_len);
	memcpy(bflrthauth.account_identification_2 ,bfl_authorisation_request.account_identification_2,account_identification_2_len);
	memcpy(bflrthauth.original_message_type_identifier ,bfl_authorisation_request.original_message_type_identifier,original_message_type_identifier_len);

	if((memcmp(bflrthauth.response_code,"00",2) == 0 )&&(memcmp(bflrthauth.processing_code,"00",2) == 0 ) )
	{
		if(strlen(Vas_Amount) == 12)
			memcpy(bflrthauth.vas_amount,Vas_Amount,12);
		memcpy(bflrthauth.filler5,"      ",6);
		memcpy(bflrthauth.available_limit_before_void,"            ",12);
		memcpy(bflrthauth.available_limit_after_void, "            ",12);
		memcpy(bflrthauth.filler6,"    ",4);
		memcpy(bflrthauth.advance_emi_months,procdata.adv_emicount,3);
	}
	if(strlen(manufacture_id) > 0 )
		memcpy(bflrthauth.manufacture_id,manufacture_id,strlen(manufacture_id));
	memcpy(bflrthauth.filler4,"    ",4);
	memset(make_produ,0x00,sizeof(make_produ));
	memcpy(make_produ,procdata.product_make,35);

	if(strlen(make_produ) == 35) 
		memcpy(bflrthauth.make_product,make_produ,strlen(make_produ) );

	if(strlen(Otp_Code) >  6 )
		memcpy(bflrthauth.otp_code,Otp_Code+(strlen(Otp_Code)-6),6);
	else
		memcpy(bflrthauth.otp_code,Otp_Code,strlen(Otp_Code));

	memcpy(bflrthauth.filler3 ," ",1);
	memcpy(bflrthauth.remaining_open_to_use,bfl_authorisation_request.remaining_open_to_use,remaining_open_to_use_len);
	memcpy(bflrthauth.file_record_action_and_data,bfl_authorisation_request.file_record_action_and_data,file_record_action_and_data_len);

	/******************CARD HOLDER DETAILS*****************************/
	memcpy(bflrthauth.cardholder_identification,bflzbcli.lbcli_customer_id,19 );

	/****CURRENTLY BRANCH ID IA AT ACCOUNT MASTER NEEDS TO BE CHECK WITH VIVEK*********/
	memcpy(bflrthauth.branch_code ,bflzbcom.lbcom_branch_id ,  5 ); 

	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0 )
		memcpy(bflrthauth.cardholder_name ,bflzport.lport_name_on_card, 26);
	else
		memcpy(bflrthauth.cardholder_name ,bflzbcli.lbcli_first_name, 40);

	memcpy(bflrthauth.middle_name,bflzbcli.lbcli_middle_name, 25 );
	memcpy(bflrthauth.last_name, bflzbcli.lbcli_last_name , 25 );
	memcpy(bflrthauth.address_type ,    "CURRES" , 6 );
	memcpy(bflrthauth.residence_type  ,   "O"   ,  1 );
	memcpy(bflrthauth.mailing_address ,   "Y"   ,  1 );
	memcpy(bflrthauth.process_date,"20",2);
	memcpy(bflrthauth.process_date+2,GetDateYYMMDD(), 6 );
	memcpy(bflrthauth.test_id,bflrthauth.reference_number+2,10 );
	memcpy(bflrthauth.applied_amount,bfl_authorisation_request.amount_transaction,amount_transaction_len);
	memcpy(bflrthauth.emi_crd_swipe_flg,"Y",1);
	memcpy(bflrthauth.txn_date_local, "20",2 );
	memcpy(bflrthauth.txn_date_local+2, GetDateYYMMDD(),6 );

	memcpy(bflrthauth.txn_mobile_num, txn_mob_num, strlen(txn_mob_num));

	if((memcmp(bflrthauth.response_code,"00",2) == 0 ) && (memcmp(bflrthauth.processing_code,"00",2) == 0 )&&(ISOStruct->entete.type != 400))
		memcpy(bflrthauth.loanstatus ,"POSITIVE  ",10);
	else if ((memcmp(bflrthauth.response_code,"00",2) != 0 ) && (memcmp(bflrthauth.processing_code,"00",2) == 0 ))
		memcpy(bflrthauth.loanstatus ,"NEGATIVE  ",10);
	else
		memcpy(bflrthauth.loanstatus ,"          ",10);

	if(memcmp(bflrthauth.response_code,"00",2) == 0 ) 
	{
		memset(process_type,0x00,sizeof(process_type));
		memcpy(process_type,procdata.process_type,20);

		f_printf(stderr,"Process Type[%s]",process_type);

		if(strlen(process_type) == 20 )
			memcpy(bflrthauth.processtype,process_type, 20 ); /****Getting From DataBase-AUG-2016**/
		if(
			(memcmp(bflrthauth.productcode,"REMI",4)==0)
			||(memcmp(bflrthauth.productcode,"ERF",3)==0) 
			||(memcmp(bflrthauth.productcode,"ECF",3)==0)
			||(memcmp(bflrthauth.productcode,"WLT",3)==0)
		)
			memcpy(bflrthauth.branchid,bflzbcom.lbcom_branch_id,8);
		else
		{
			memset(branchid,0x00,sizeof(branchid));
			GetBranchId(branchid);
			if(strlen(branchid) > 0 )
				memcpy(bflrthauth.branchid,branchid,8);
		}
	}
	else
		memcpy(bflrthauth.branchid,bflzbcom.lbcom_branch_id,8);

	memcpy(bflrthauth.individualcorpflag,bflzbcli.lbcli_customer_category,1);
	memcpy(bflrthauth.gender, bflzbcli.lbcli_gender,1 );

	#ifdef ADDON
	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0 )
		memcpy(bflrthauth.dateofbirth+0, lnaddport.laddon_dob,8);
	else
	#endif
		memcpy(bflrthauth.dateofbirth+0,bflzbcli.lbcli_dob,8);

	if(strlen(asset_cat) > 0 )
		memcpy(bflrthauth.category_id,asset_cat,strlen(asset_cat));
	if(strlen(Otp_Request_id) > 0 )
		memcpy(bflrthauth.otp_request_id,Otp_Request_id,strlen(Otp_Request_id));
	if(strlen(Uniq_Identifier_Code) > 0 )
		memcpy(bflrthauth.const_id,"0",1);

	memcpy(bflrthauth.martial_status,"M",1 );
	memcpy(bflrthauth.address1, bflzbcli.lbcli_residence_address1, 40);
	memcpy(bflrthauth.address1+40, bflzbcli.lbcli_residence_address2, 40);
	memcpy(bflrthauth.address1+80, bflzbcli.lbcli_residence_address3, 40);
	memcpy(bflrthauth.zipcode,bflzbcli.lbcli_residence_pincode ,10 );
	memcpy(bflrthauth.phone1,bflzbcli.lbcli_residence_phone,15 );

#ifdef ADDON 
	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0 )
		memcpy(bflrthauth.mobile, lnaddport.laddon_mobile,15 ); 
	else
#endif
		memcpy(bflrthauth.mobile,bflzbcli.lbcli_residence_mobile,15 );

	memcpy(bflrthauth.other_employer_name ,"S",1 );

	if(strlen(rsa_flag)>0)
		memcpy(bflrthauth.fraud_flag,rsa_flag,1);

	if(memcmp(bflrthauth.response_code,"00",2) == 0 )
	{
		if((memcmp(bflzport.lport_card_sub_type,"10",2) == 0) ||(memcmp(bflzport.lport_card_sub_type,"11",2) == 0)||(memcmp(bflzport.lport_card_sub_type,"18",2) == 0)
		||(memcmp(bflzport.lport_card_sub_type,"19",2) == 0)||(memcmp(bflzport.lport_mxp_card_type,"99",2) == 0))
		{
			if(bfl_dealer_master.co_brand_limit_flag[0] != 'Y')
			{
				memcpy(bflrthauth.emi_cardflag,"N",1);
				memcpy(bflrthauth.emi_cardlimit,"000000000000",12 );
			}
			else
			{
				memset(De_Code,0x00,sizeof(De_Code));
				memset(De_Code1,0x00,sizeof(De_Code1));
				memcpy(De_Code,bflzport.lport_dealer_cd,10);
				f_printf(stderr,"\nDealer Code [%s]",De_Code);

				Trim_string(De_Code,De_Code1,10);

				f_printf(stderr,"\nAfter Trim Dealer Code [%s]",De_Code1);
				if((memcmp(De_Code1,bfl_dealer_master.mid+(15-strlen(De_Code1)),strlen(De_Code1)) != 0)||(memcmp(bflzport.lport_mxp_card_type,"99",2) == 0))
				{
					if(CheckCoBrandCard() != 2)
					{
						memcpy(bflrthauth.emi_cardflag,"Y",1);
						memcpy(bflrthauth.emi_cardlimit,bflzbcom.lbcom_acc_credit_limit,12 );
					}
					else
					{
						memcpy(bflrthauth.emi_cardflag,"N",1);
						memcpy(bflrthauth.emi_cardlimit,"000000000000",12 );
					}
				}
				else
				{
					memcpy(bflrthauth.emi_cardflag,"N",1);
					memcpy(bflrthauth.emi_cardlimit,"000000000000",12 );
				}

			}
		}
		else
		{
			memcpy(bflrthauth.emi_cardflag,"N",1);
			memcpy(bflrthauth.emi_cardlimit,"000000000000",12 );
		}
	}
	else
	{
		memcpy(bflrthauth.emi_cardflag,"N",1);
		memcpy(bflrthauth.emi_cardlimit,"000000000000",12 );
	}

	if(strlen(Loan_Value_Ratio) > 0 )
		memcpy(bflrthauth.loantovalueratio,Loan_Value_Ratio,strlen(Loan_Value_Ratio));

	/********************************/
	memcpy(Pre_Card_Limit,bflzbcom.lbcom_acc_prefferd_limit,12);


	if((strlen(Prefered_Limit) > 0)
           &&(memcmp(bflzbcom.lbcom_acc_prefferd_limit,"000000000000",12) == 0 )
           && (memcmp(prefered_limit_flag,"Y",1) == 0 )&&(strlen(prefered_limit_flag) > 0)
          )
		memcpy(bflrthauth.preferredeligibility,"Y",1 );
	else
		memcpy(bflrthauth.preferredeligibility,"N",1 );

	if(memcmp(bflrthauth.preferredeligibility,"Y",1) == 0)
		memcpy(bflrthauth.preferredcardlimit,Prefered_Limit, 12 );
	else
		memcpy(bflrthauth.preferredcardlimit,"000000000000", 12 );

#ifdef ADDON_PHASE_2
	f_printf(stderr,"\nAdd on CARD flag for card [%.19s] is [%.1s]\n",
	bflzport.lport_card_no, bflzport.lport_primary_addon_card_flag);
	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0)
	{
		f_printf(stderr,"Updating bflrthauth.preferredcardlimit 000000000000 for card [%.19s]\n",
		bflzport.lport_card_no);

		memcpy(bflrthauth.preferredcardlimit,"000000000000", 12 );
		memcpy(bflrthauth.add_on_card_elg,"N", 1 );
	}
	else
	{
		memcpy(bflrthauth.add_on_card_elg,"Y", 1);
		f_printf(stderr,"Updated add on card eligibality as Y for Primary card \n");
	}
#endif


	if(bfl_dealer_master.co_brand_limit_flag[0] == 'Y')
		memcpy(bflrthauth.swipe_dealer_card_code, bfl_dealer_master.co_brand_card_code,2 ); 

	memcpy(bflrthauth.tms_store_id  ,bfl_dealer_master.store_id,10 ); 

	/*MRL_300 & MRL_383*/
	if(Card_First_Trans == 3 || rset_card_1st_fee ==2)
	{
		memcpy(Card_first_fee,bflzport.lport_emi_card_charges,12);
		if(atof(Card_first_fee) > 0.0)
		{
			memcpy(bflrthauth.card_fees,bflzport.lport_emi_card_charges,12);
			memcpy(bflrthauth.first_swipe_flag ,"Y" ,1);
		}
		else
		 f_printf(stderr,"\n Card Fee.Not Added Due to Improper value of Card fee...[%.12s].\n",bflzport.lport_emi_card_charges);
	}
	else
	memcpy(bflrthauth.card_fees,"            ",12);		


	Card_First_Trans = 0;
	chargeslip_first_fee = 0;
	
	if(strlen(parent_rrn) > 0 )
		memcpy(bflrthauth.parent_deal_id ,parent_rrn,12); /******"BASED ON THE PARENT DEAL ID FOUND******/

	memcpy(bflrthauth.spice_store_id , bfl_dealer_master.store_id,10); 

	if((memcmp(bflzport.lport_card_sub_type,"10",2) == 0) ||(memcmp(bflzport.lport_card_sub_type,"11",2) == 0)||(memcmp(bflzport.lport_card_sub_type,"18",2) == 0)
	||(memcmp(bflzport.lport_card_sub_type,"19",2) == 0))
		memcpy(bflrthauth.card_type,"COBRAND",  7 );
	else
		memcpy(bflrthauth.card_type,"EMI",  3 );

	if(memcmp(bflzport.lport_mxp_card_type,"99",2) == 0)
		memcpy(bflrthauth.virtualcard_flag,"Y",1);
	else
		memcpy(bflrthauth.virtualcard_flag,"N",1);

	memcpy(bflrthauth.uidno,bflzbcli.lbcli_uid,20);
	memcpy(bflrthauth.panno,bflzbcli.lbcli_pan,15);
	memcpy(bflrthauth.addpreference,bflzbcli.lbcli_mailing_addr_flag,1);
	memcpy(bflrthauth.transaction_source,procdata.source,2);
	memcpy(bflrthauth.customer_risk,bflzbcom.lbcom_cust_risk_clasif,2);
	memcpy(bflrthauth.transaction_risk,procdata.trans_risk,2);
	memcpy(bflrthauth.velocity_count,procdata.trans_velocity,3);
	memcpy(bflrthauth.velocity_days,procdata.risk_vel_days,3);

	if((memcmp(procdata.source,"WP",2) == 0 )&&(strlen(promo_code) > 0))
		memcpy(bflrthauth.promo_code,promo_code,strlen(promo_code));
	if((strlen(procdata.adv_emiamount)> 0 )&&((memcmp(procdata.adv_emiamount,"            ",12) != 0)&&(memcmp(procdata.adv_emiamount,"000000000000",12) != 0)))
		memcpy ( bflrthauth.advance_emi_amount,  procdata.adv_emiamount,12 );
	else
		memcpy ( bflrthauth.advance_emi_amount, "000000000000",12 );

#ifdef BFL_PHASE2
	if(strlen(relation_type)  > 0 )
		memcpy(bflrthauth.relationship_type,relation_type,1);
	memset(bflrthauth.filler,' ',8);
#endif



	#ifdef RULE_ENGINE
	if ( (memcmp(bflrthauth.response_code,"00",2) != 0) && (' ' != failedRule[0]) && ('\0' != failedRule[0]))
	{
		memcpy(bflrthauth.failed_rule_id, failedRule, strlen(failedRule) );
	}
	#endif

	#ifdef ADDON_PHASE_2
	if(110 == ISOStruct->entete.type 
	&& memcmp(bflrthauth.response_code,"00",2) == 0 
	&& memcmp(bflrthauth.processing_code,"000000",6)==0)
	{
		if( Check_First_Txn() != SUCCESS )
		{
			f_printf(stderr, "Check_First_Txn () failed \n");
		}
		else
		{
			f_printf(stderr, "This is the first Transaction\n");
			memcpy(bflrthauth.add_on_first_trans,"Y",1);
			f_printf(stderr,"Updating first transaction as Y in RTH\n");
		}
		Card_Block_Status();
	}
	#endif

#ifdef MRL_287
	if((strlen(mob_num_change_flag) > 0) && (memcmp(mob_num_change_flag,"Y",1) == 0))
		memcpy(bflrthauth.mob_num_change,"Y",1);
	else
		memcpy(bflrthauth.mob_num_change,"N",1);
#endif



/*MRL_287 changes to add transaction type in FINS_AUTHORIZATIONMASTER*/
#ifdef MRL_287

	if((strlen(Otp_Request_id) > 0 )&&(strlen(Otp_Code) > 0 ))
	{
		memcpy(bflrthauth.transaction_type_otp_pin, "O", 1);
		f_printf(stderr,"updating transaction_type [%.1s]",bflrthauth.transaction_type_otp_pin);
	}
	else
	{
		memcpy(bflrthauth.transaction_type_otp_pin, "P", 1);
		f_printf(stderr,"updating transaction_type [%.1s]",bflrthauth.transaction_type_otp_pin);
	}

#endif

	if(ISOStruct->entete.type == 410)
		return;


	Size_Auth_Record = strlen((char *)&bflrthauth);
	f_printf(stderr,"sizeof Auth Record[%d]size of Auth Record[%d]",Size_Auth_Record,sizeof(t_bflrthauth));
	if(Size_Auth_Record  < sizeof(t_bflrthauth))
	{
		temp = NULL ;
		temp = (char *)&bflrthauth;

		for ( i=0; i < sizeof(t_bflrthauth); ++i)
		if ( *(temp+i) == '\0' )
		*(temp+i) = ' ';
		f_printf(stderr,"sizeof Auth Record[%d]",strlen((char *)&bflrthauth));
	}

	if( iswrite( lnrth, (char *)&bflrthauth) < 0 )
	{
		memcpy(bflrthauth.reference_number, GetDateYYMMDD(), 6 );
		memcpy(bflrthauth.reference_number+6, (char *)GetTime(), 6 );

		if( iswrite( lnrth,(char *)&bflrthauth) < 0 )
		f_printf(stderr, "**********Error writting BFL-RTH-AUTH %d *********\n", iserrno );
		else
		iUpdateRTH();
	}
	else
		iUpdateRTH();

	return;
}
#endif

#ifdef BFL_CUG_CARD
int  BFLEmailSMS(t_struct_iso* ISOStruct)
{
        unsigned int email_seq;
        char filename[128];
        char temp_buffer[128];
        char next_trans_date[7];
        int fdbflemail;
        t_loan_email_sms loan_email_sms;
        t_paczport paczport;
        char phone_number[15];
	char *AvlBal;
	char tenure[5+1];
	int itenure;
#ifdef BFL_BLOCK_SMS
        char Open_Ecs_Balance[13];
#endif
	char ctime_of_day[64];
	struct timeval time_of_day;
	int iswrite_return=0;
	int iswrite_retries=0;




        memset(next_trans_date,0x00,sizeof(next_trans_date));
        memset(&loan_email_sms, ' ', sizeof(loan_email_sms));

        if(atoi(NXTALLD) > 0 )
        memcpy(next_trans_date,(char*) GetDateInc(NXTALLD),6); /* Copying NExt Transaction Date */

        f_printf(stderr, "Inside BFL SMS.\n");

	f_printf(stderr, "Response code [%.6s]\n",bflrthauth.response_code);
	f_printf(stderr, "MTI : [%d]\n",ISOStruct->entete.type);
	f_printf(stderr, "PROCESSING CODE : [%.6s]\n",bflrthauth.processing_code);
        

        /************************ Only Sale/Void for virtual cards *************************/
	if((memcmp(bflzport.lport_mxp_card_type, "99", 2)==0)&&
	   ( memcmp(bflrthauth.response_code, "00", 2) == 0 )&&
	    ((memcmp(bflrthauth.processing_code,"00",2)!=0)&&
	    (memcmp(bflrthauth.processing_code,"02",2)!=0)))
	{
		/* Do not send SMS/EMAIL for virtual cards if transactions
		 * are other than Void & Sale.
		 */
		return;
	}


	if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
		ISOStruct->entete.type == 110 &&
		memcmp(bflrthauth.processing_code,"000000",6)==0)
	/* Sale transaction MTI : 0110, PROCESSING CODE : 000000 */
	{
		
		f_printf(stderr, "BFL Sale transaction 00.\n");
		f_printf(stderr, "processing_fee [%.12s], short_name[%.20s].\n",bfl_dealer_master.processing_fee,bfl_dealer_master.short_name);
	
		if((memcmp(buisness_prod_code, "REMI",4 ) == 0) 
		#ifdef MRL_496
		|| (memcmp(buisness_prod_code, "ECF", 3) == 0) || (memcmp(buisness_prod_code,  "ERF", 3) == 0)
		#endif
		)
		{
			if(
				(memcmp(bfl_dealer_master.processing_fee, "000000000000",12)!=0)&&
				(memcmp(bfl_dealer_master.processing_fee, "            ",12)!= 0)&&
				(memcmp(bfl_dealer_master.short_name, "                    ",20) != 0)
			)
			{
				memcpy(loan_email_sms.loan_transaction_type, "TRNS1", 5);
			}
			else if(
				((memcmp(bfl_dealer_master.processing_fee, "000000000000",12) == 0) || 
				(memcmp(bfl_dealer_master.processing_fee, "            ",12) == 0)) &&
				(memcmp(bfl_dealer_master.short_name, "                    ",20) == 0)
			)
			{
				memcpy(loan_email_sms.loan_transaction_type, "TRNS4", 5);
			}
			else if(memcmp(bfl_dealer_master.short_name, "                    ",20) != 0)
			{
				memcpy(loan_email_sms.loan_transaction_type, "TRNS2", 5);
			}
			else 
			{
				memcpy(loan_email_sms.loan_transaction_type, "TRNS3", 5);
			}
			
		}
		else
		{
			memcpy(loan_email_sms.loan_transaction_type, "CDTAP", 5);
		}
                memcpy(loan_email_sms.loan_tenure, procdata.bfl_loan_ten + 2, 3);
                memcpy(loan_email_sms.loan_emi_amount, procdata.emiamount, 12);
	}
#ifdef BFL_BLOCK_SMS
        else if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
                ISOStruct->entete.type == 410 &&
                memcmp(bflrthauth.processing_code,"000000",6)==0)
        /* Void transaction for Sale*/
        {
                f_printf(stderr, "Sale - Reversal transaction 00.\n");
                memcpy(loan_email_sms.loan_transaction_type, "TXREV", 5);
        }
#endif
	else if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
		ISOStruct->entete.type == 410 &&
		memcmp(bflrthauth.processing_code,"020000",6)==0)
	/* Void transaction for Sale*/
	{
		f_printf(stderr, "Sale - Void transaction 00.\n");
		memcpy(loan_email_sms.loan_transaction_type, "TXVOD", 5);
	}
	else if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
		ISOStruct->entete.type == 410 &&
		memcmp(bflrthauth.processing_code,"380000",6)==0)
	/* Reversal of Reward Redemption MTI : 0410, PROCESSING CODE : 990000 */
	{
		f_printf(stderr, "BFL Reward - Reversal transaction 00.\n");
		memcpy(loan_email_sms.loan_transaction_type, "RWREV", 5);
	}
	else if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
		ISOStruct->entete.type == 110 &&
		memcmp(bflrthauth.processing_code,"990000",6)==0)
	/* Reward Redemption MTI : 0110, PROCESSING CODE : 990000 */
	{
		f_printf(stderr, "BFL Reward Redemption transaction 00.\n");
		memcpy(loan_email_sms.loan_transaction_type, "RWRED", 5);
	}
	else if(memcmp(bflrthauth.response_code, "00", 2) == 0 &&
		ISOStruct->entete.type == 410 &&
		memcmp(bflrthauth.processing_code,"20",2)==0)
	/* Refunds */
	{
		f_printf(stderr, "Refunds.\n");
		memcpy(loan_email_sms.loan_transaction_type, "TXCAN", 5);
	}
	else if (memcmp(bflrthauth.response_code, "960307", 6) == 0)
	{
		f_printf(stderr, "Invalid Scheme transaction 00.\n");
		memcpy(loan_email_sms.loan_transaction_type, "TRSCH", 5);
	}
	else if (memcmp(bflrthauth.response_code, "55", 2) == 0)
	{
		f_printf(stderr, "Incorrect pin Entry 1st time.\n");
		memcpy(loan_email_sms.loan_transaction_type, "PININ", 5);
#ifdef MRL_207

		memcpy(loan_email_sms.loan_pin_otp_count, bflzlpa03.lpa03_max_wrng_pin_allowed, 1);
		f_printf(stderr, "max_wrng_otp: [%.1s]\n",bflzlpa03.lpa03_max_wrng_pin_allowed);
#endif
	}
	else if(memcmp(bflrthauth.response_code, "75", 2) == 0)
	{
		f_printf(stderr, "Incorrect pin Entry more than 3 times.\n");
		memcpy(loan_email_sms.loan_transaction_type, "PINBL", 5);
	}
	else if(memcmp(bflrthauth.response_code, "51", 2) == 0)
	{
		f_printf(stderr, "Amount exhausted in the card.\n");
		memcpy(loan_email_sms.loan_transaction_type, "NOLMT", 5);
	}
	else if(
		(memcmp(bflrthauth.processing_code,"30",2)==0) 
		&&
		(( memcmp(bflzport.lport_cs_block_flag,"Y",1 ) == 0 )
		||( memcmp(bflzport.lport_rto_block_flag,"Y",1 ) == 0 )
		||(( memcmp(bflzbcom.lbcom_sys_block_type,"UB",2 ) != 0 )&& ( memcmp(bflzbcom.lbcom_sys_block_type,"  ",2) != 0 ) )
		||(( memcmp(bflzbcom.lbcom_fraud_block_type,"UB",2 ) != 0 )&& ( memcmp(bflzbcom.lbcom_fraud_block_type,"  ",2 ) != 0 )))
		)
	{
		f_printf(stderr, "Blanace Inquiry : Card is blocked.\n");
		memcpy(loan_email_sms.loan_transaction_type, "BALBL", 5);
	}
	else if(memcmp(bflrthauth.processing_code,"30",2)==0 &&
		memcmp(bflrthauth.response_code, "00", 2) == 0)
	{
		f_printf(stderr, "Blanace Inquiry Successful SMS.\n");
		memcpy(loan_email_sms.loan_transaction_type, "BALUB", 5);
	}
	else if(memcmp(bflrthauth.response_code, "B9", 2) == 0 || 
		memcmp(bflrthauth.response_code, "C1", 2) == 0 ||
		memcmp(bflrthauth.response_code, "J1", 2) == 0 ||
		memcmp(bflrthauth.response_code, "K4", 2) == 0)
	{
		f_printf(stderr, "Card is blocked.\n");
		/* MRL_288 */
		if ((bflzbcom.lbcom_fraud_block_reason, "TEMPORARY_RCU_BLOCK") ||
			(bflzbcom.lbcom_fraud_block_reason, "PERMANENT_RCU_BLOCK"))
			memcpy(loan_email_sms.loan_transaction_type, "PRCUB", 5);
		else
			memcpy(loan_email_sms.loan_transaction_type, "CDBLK", 5);
	}
	else if(memcmp(bflrthauth.response_code, "J4", 2) == 0)
	{
		f_printf(stderr, "ECS limit exhausted.\n");
		memcpy(loan_email_sms.loan_transaction_type, "ECSLT", 5);
	}
	else if(memcmp(bflrthauth.response_code, "96", 2) == 0)
	{
		f_printf(stderr, "Technical error.\n");
		memcpy(loan_email_sms.loan_transaction_type, "TCISS", 5);
	}
        else if((memcmp(bflrthauth.response_code, "L6", 2) == 0) || (memcmp(bflrthauth.response_code, "L7", 2) == 0))
        {
                f_printf(stderr, "RURAL CARD POLICY FOR ECOM AND REMI\n");
                memcpy(loan_email_sms.loan_transaction_type, "RUDCL", 5);
        }
#ifdef MRL_204
        else if(memcmp(bflrthauth.response_code,"M8", 2) == 0)
        {
                f_printf(stderr, "RURAL CARD TRANSACTION FOR REMI\n ");
                memcpy(loan_email_sms.loan_transaction_type, "TXNDL", 5);
        }
#endif
#ifdef MRL_207
	else if(memcmp(bflrthauth.response_code,"N0",2)==0)
	{
		f_printf(stderr, "CARD BLOCKED DUE TO WRONG OTP MAX ATTEMPT\n ");
		memcpy(loan_email_sms.loan_transaction_type, "OTPBL", 5);
	}
	else if ( memcmp(bflrthauth.response_code,"L3",2) ==0)
	{
		f_printf(stderr, "INVALID OTP ENTERED\n ");
		memcpy(loan_email_sms.loan_transaction_type, "OTPIN", 5);
		memcpy(loan_email_sms.loan_pin_otp_count, bflzlpa03.lpa03_max_wrng_otp_allowed, 1);
		f_printf(stderr, "max_wrng_otp: [%.1s]\n",bflzlpa03.lpa03_max_wrng_otp_allowed);
	}
#endif
#ifdef MRL_197
        else if((memcmp(bflrthauth.response_code, "65", 2) == 0)
		 && 
		((memcmp(buisness_prod_code, "ECF", 3) == 0) || (memcmp(buisness_prod_code, "ERF", 3) == 0) )
	)
        {
                f_printf(stderr,"Velocity Count Exceeded..\n");
                memcpy(loan_email_sms.loan_transaction_type,"EVELC",5);
        }
#endif
	else
	{
		f_printf(stderr, "Unknown BFL response : [%.2s]\n",bflrthauth.response_code);
		return -1;
	}


	/* OPENING LN-EMAIL C-ISAM FILE */
        sprintf(filename, "%s/LN-EMAIL",getenv("MultipacDat"));
        if((fdbflemail = isopen(filename,ISINOUT + ISMANULOCK))<0)
        {
                f_printf(stderr,"ONUS: ERROR OPEN EMAIL-MSG:%d\n",iserrno);
                return -1;
        }



        memcpy(loan_email_sms.loan_msg_type, "000", 3);
        memcpy(loan_email_sms.loan_date, GetDateYYYYMMDD(), 6);
        memcpy(loan_email_sms.loan_time, (char *)GetTime(), 6);

	/* Fetch the unique sequence from C-Isam and take last 8 digits as P.K. */
	isuniqueid(fdbflemail, &email_seq);
	sprintf(temp_buffer, "%08u", email_seq);
	memcpy(loan_email_sms.loan_seq_no, temp_buffer + (strlen(temp_buffer)-8), 8);

        memcpy(loan_email_sms.loan_bank_code, paczpoau.poau_cd_banque, 5);
        memcpy(loan_email_sms.loan_bank_code, "00059", 5);
        memcpy(loan_email_sms.loan_email_flag, bflzbcli.lbcli_email_flag, 1);
        memcpy(loan_email_sms.loan_sms_flag, bflzbcli.lbcli_sms_flag, 1);
        memcpy(loan_email_sms.loan_card_number, bflrthauth.card_number, 19);
	memcpy(loan_email_sms.loan_pin, bflzport.lport_pin, 4);
	memcpy(loan_email_sms.loan_card_type, bflzport.lport_mxp_card_type, 2);
	f_printf(stderr, "Card Type : [%.2s]\n",loan_email_sms.loan_card_type);
        memcpy(loan_email_sms.loan_rrn, bflrthauth.reference_number, 12);
        memcpy(loan_email_sms.loan_txn_date, GetDateYYYYMMDD(), 8);
        memcpy(loan_email_sms.loan_txn_time, (char *)GetTime(), 6);
        memcpy(loan_email_sms.loan_merchant_details, bflrthauth.card_acceptor_name_location, 40);
	memcpy(loan_email_sms.loan_reward_point, Requested_reward_point, strlen(Requested_reward_point));
	memcpy(loan_email_sms.loan_short_name, bfl_dealer_master.short_name, 20);
	memcpy(loan_email_sms.loan_proc_fees, bfl_dealer_master.processing_fee, 12);


	memcpy(tenure, procdata.bfl_loan_ten, 5); tenure[5]='\0';
	itenure = atoi(tenure);
	sprintf(tenure, "%03d", itenure);
	memcpy(loan_email_sms.loan_no_of_emi, tenure, 3);
	


	if(
		memcmp(loan_email_sms.loan_transaction_type, "OTPBL", 5)==0 || 
		memcmp(loan_email_sms.loan_transaction_type, "PININ", 5)==0 || 
		memcmp(loan_email_sms.loan_transaction_type, "PINBL", 5)==0 ||
		memcmp(loan_email_sms.loan_transaction_type, "OTPIN", 5)==0 
	)
	{
		if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0)
		{
			f_printf(stderr, "PIN SMS for ADDON Card.\n");
			memcpy(loan_email_sms.loan_mobile_number, lnaddport.laddon_mobile, 15);  
			memcpy(loan_email_sms.loan_email_address, lnaddport.laddon_email, 50); 
		}
		else
		{
			f_printf(stderr, "PIN SMS for PRIMARY Card.\n");
			memcpy(loan_email_sms.loan_mobile_number, bflzbcli.lbcli_residence_mobile, 15);
			memcpy(loan_email_sms.loan_email_address, bflzbcli.lbcli_residence_email, 50); 
		}
	}
	else
	{
		f_printf(stderr, "NON-PIN SMS for Primary Card\n");
		memcpy(loan_email_sms.loan_mobile_number, bflzbcli.lbcli_residence_mobile, 15);
		memcpy(loan_email_sms.loan_email_address, bflzbcli.lbcli_residence_email, 50); 
	}
	f_printf(stderr, "Mobile numner : [%.15s].\n", bflzbcli.lbcli_residence_mobile);


        memcpy(loan_email_sms.loan_auth_amount, bflrthauth.amount_transaction, 12);
        memcpy(loan_email_sms.loan_loan_amount, bflrthauth.amount_transaction, 12);
	AvlBal = (char*)BFL_GetBalance ( &bflzbcom);
	memcpy(loan_email_sms.loan_otbl_amount+1, AvlBal, 12);

#ifdef BFL_BLOCK_SMS

	memcpy (Open_Ecs_Balance, (char*)BFL_GetOpenECSBalance(&bflzbcom), 12);
        memcpy (loan_email_sms.loan_ecs_available, Open_Ecs_Balance, 12);
        memcpy(loan_email_sms.loan_cust_name, bflzport.lport_name_on_card, 26);
#endif

	
	f_printf(stderr, "SMS-EMAIL [%.*s]\n",sizeof(loan_email_sms),&loan_email_sms);

        if(iswrite(fdbflemail, &loan_email_sms)<0)
        {
                f_printf(stderr,"ONUS: iswrite error loan_email_sms:%d\n",iserrno);
                isclose(fdbflemail);
                return -1;
        }


#ifdef BFL_REMI_TO_PL
	/* SMS for REMI TO PL Amount breach */
	if((1 == remi_to_pl_sms) && memcmp(bflrthauth.response_code, "00", 2) == 0)
	{
		t_loan_email_sms b_loan_email_sms;
		b_loan_email_sms = loan_email_sms;
		char customer[19+1];
		char cumul_auth[12];
		char cumul_ecs[12];

		memcpy(b_loan_email_sms.loan_date, GetDateYYYYMMDD(), 6);
		memcpy(b_loan_email_sms.loan_time, (char *)GetTime(), 6);


		/* Fetch the unique sequence from C-Isam and take last 8 digits as P.K. */
		isuniqueid(fdbflemail, &email_seq);
		sprintf(temp_buffer, "%08u", email_seq);
		memcpy(b_loan_email_sms.loan_seq_no, temp_buffer + (strlen(temp_buffer)-8), 8);

		memcpy(b_loan_email_sms.loan_transaction_type, "EMICP", 5);
		f_printf(stderr,"Message : EMICP\n");

		memcpy(customer, bflrthauth.primary_account_number, 19);
		customer[19]='\0';

		if(memcmp(customer, "                   ", 19)!=0)
			get_cumul_auth_amount(customer, cumul_auth, cumul_ecs, 1);

		f_printf(stderr, "SMS: Cumul: [%.12s], ECS: [%.12s](not to be used)\n", cumul_auth, cumul_ecs);
		f_printf(stderr, "ECS: [%.12s]\n", remi_to_pl_breach_amount);
		memcpy(b_loan_email_sms.loan_remi_pl_breach_emi, remi_to_pl_breach_amount, 12);
		memcpy(b_loan_email_sms.loan_remi_pl_breach_auth, cumul_auth, 12);
		f_printf(stderr, "SMS-EMAIL [%.*s]\n",sizeof(b_loan_email_sms),&b_loan_email_sms);


		iswrite_return = iswrite(fdbflemail, &b_loan_email_sms);

		if(0==iswrite_return)
			f_printf(stderr, "EMICP SMS written Successfully.\n");
		else
			f_printf(stderr, "ERROR IN WRITING EMICP. ERROR: %d\n", iserrno);
	}
#endif



#ifdef MRL_238
        if(memcmp(bflrthauth.response_code, "00", 2) == 0 && 1 == Ecs_Waiver_Sms)
        {
		t_loan_email_sms b_loan_email_sms;
		b_loan_email_sms = loan_email_sms;

		memcpy(b_loan_email_sms.loan_date, GetDateYYYYMMDD(), 6);
		memcpy(b_loan_email_sms.loan_time, (char *)GetTime(), 6);

		/* Fetch the unique sequence from C-Isam and take last 8 digits as P.K. */
		isuniqueid(fdbflemail, &email_seq);
		sprintf(temp_buffer, "%08u", email_seq);
		memcpy(b_loan_email_sms.loan_seq_no, temp_buffer + (strlen(temp_buffer)-8), 8);

                memcpy(b_loan_email_sms.loan_transaction_type,"ECSWV",5) ;
                memcpy(b_loan_email_sms.loan_rrn, bflrthauth.reference_number, 12) ;

                f_printf(stderr,"Message : ECSWV\n");
		f_printf(stderr, "SMS-EMAIL [%.*s]\n",sizeof(b_loan_email_sms),&b_loan_email_sms);

		iswrite_return = iswrite(fdbflemail, &b_loan_email_sms);
		if(0==iswrite_return)
			f_printf(stderr, "ECSWV SMS written Successfully.\n");
		else
			f_printf(stderr, "ERROR IN WRITING ECSWV. ERROR: %d\n", iserrno);
        }
#endif

	f_printf(stderr, "Addon Flag: [%.1s]\n", bflzport.lport_primary_addon_card_flag);
	/* 
	 * Upto this point an sms is sent for primary card.
	 * However, if the card is an ADDON card, send an SMS on ADDON card too.
	 * Only for below mentioned transactions.
	 */
	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0)
	{
		if(
			(memcmp(loan_email_sms.loan_transaction_type, "CDTAP", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TXREV", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TXVOD", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "RWREV", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "RWRED", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TXCAN", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TRSCH", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "NOLMT", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "BALBL", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "BALUB", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "CDBLK", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "ECSLT", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TCISS", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "RUDCL", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "EVELC", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TRNS1", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TRNS2", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TRNS3", 5)==0) ||
			(memcmp(loan_email_sms.loan_transaction_type, "TRNS4", 5)==0)
		)
		{
			memcpy(loan_email_sms.loan_date, GetDateYYYYMMDD(), 6);
			memcpy(loan_email_sms.loan_time, (char *)GetTime(), 6);

			/* Fetch the unique sequence from C-Isam and take last 8 digits as P.K. */
			isuniqueid(fdbflemail, &email_seq);
			sprintf(temp_buffer, "%08u", email_seq);
			memcpy(loan_email_sms.loan_seq_no, temp_buffer + (strlen(temp_buffer)-8), 8);

			memcpy(loan_email_sms.loan_rrn, bflrthauth.reference_number, 12) ;

			/* Addon Mobile number and Email address */
			memcpy(loan_email_sms.loan_mobile_number, lnaddport.laddon_mobile, 15);  
			memcpy(loan_email_sms.loan_email_address, lnaddport.laddon_email, 50); 

			f_printf(stderr,"Message : %.5s\n", loan_email_sms.loan_transaction_type);
			f_printf(stderr, "SMS-EMAIL [%.*s]\n",sizeof(loan_email_sms),&loan_email_sms);

			iswrite_return = iswrite(fdbflemail, &loan_email_sms);
			if(0==iswrite_return)
				f_printf(stderr, "ADDON SMS written Successfully.\n");
			else
				f_printf(stderr, "ERROR IN WRITING ADDON. ERROR: %d\n", iserrno);
		}
	}


        isclose(fdbflemail);
        return;
}
#endif

#ifdef BFL_REMI_TO_PL
        /*
         * The below function was made specifically for REMI-to-PL.
         * The same was later made generic. Hence, the 2nd and 3rd arguments
         *     names are not in sync with the data they contain.
         * For the MRL_264 functionality, we are fetching Unmatched-loan counts
         *     in the 2nd argument, i.e. "cumul_auth".
	 * The 4th argument is used as a "switch" to decide what data is to be 
	 *     populated in the 2nd and 3rd fields.
	 *     1 - for REMI-TO-PL MRL_188
	 *     2 - for Unmatched-Loan count MRL_264
         */
void get_cumul_auth_amount(char *cust, char *p_cumul_auth, char *p_cumul_ecs, int type)
{

        t_bflauth00 bflauth00;
        char filename[1024];
        struct keydesc skey;
        int retry;
        char cumul_auth[12+1] = "000000000000";
        char cumul_ecs[12+1] = "000000000000";
        int icumul_auth = 0;
        int icumul_ecs = 0;
        int res_Value;
	int Unbooked_count = 0;

        skey.k_flags = ISDUPS;
        skey.k_nparts = 1;
        skey.k_part[0].kp_start = 31;
        skey.k_part[0].kp_leng = 19;
        skey.k_part[0].kp_type = CHARTYPE;

/*
	int lnsauto;
	int lnhauto;

*/
	f_printf(stderr, "get_cumul_auth_amount TYPE: %d\n", type);
/*

	sprintf(filename, "%s/LN-SAUTO", getenv("MultipacDat"));
	if((lnsauto = isopen(filename, ISINOUT + ISMANULOCK))<0)
	{
		f_printf(stderr, "Error in opening [%s]. Error [%d]\n", 
			filename, iserrno);
		return;
	}

	sprintf(filename, "%s/LN-AUTP", getenv("MultipacDat"));
	if((lnhauto = isopen(filename, ISINOUT + ISMANULOCK))<0)
	{
		f_printf(stderr, "Error in opening [%s]. Error [%d]\n", 
			filename, iserrno);
		isclose(lnsauto);
		return;
	}

*/

	switch(type)
	{

		case 1:

			f_printf(stderr, "=================================================\n");
			f_printf(stderr, "Starting search in LN-SAUTO.\n");
			/*
			* Fetching the transactions from LN-SAUTO.
			*/
			memset(&bflauth00, ' ', sizeof(bflauth00));
			memcpy(bflauth00.customer_id, cust, strlen(cust));
			retry=0;
			do
			{
				res_Value = isstart ( lnsauto, &skey, 19,  &bflauth00, ISEQUAL );
				retry++;
			}
			while ( ( iserrno == ELOCKED ) && ( retry < NB_FOIS ) );

			if(0==res_Value)
			{
				f_printf(stderr, "isstart successful for LN-SAUTO.\n");
				isread(lnsauto, &bflauth00, ISCURR);
				f_printf(stderr, "1-----------> %d\n", iserrno);
				while(iserrno!=EENDFILE)
				{
					f_printf(stderr, "--------->[%.12s] [%.19s] [%.12s] [%.12s] [%.1s] [%.1s] [%.10s] [%.2s]\n",
					bflauth00.reference_number, bflauth00.customer_id,
					bflauth00.auth_amount, bflauth00.emi_amount,
					bflauth00.auth_match_flag, bflauth00.auth_source_rev,
					 bflauth00.product_id, bflauth00.response_code);
					f_printf(stderr, "Customer: [%s], [%d]\n", cust, strlen(cust));

					if(memcmp(cust, bflauth00.customer_id, strlen(cust))!=0)
						break;

					if(('0'==bflauth00.auth_match_flag[0] || ' '==bflauth00.auth_match_flag[0])
					&&
					(memcmp(bflauth00.product_id, bflrthauth.productcode, 8)==0)
					&&
					('A'==bflauth00.auth_source_rev[0])
					&&
					(memcmp(bflauth00.response_code, "00", 2)==0)
					)
					{
						f_printf(stderr, "\nBefore: Cumul Amount: %d, Cumul Ecs: %d\n",
						icumul_auth, icumul_ecs);

						memcpy(cumul_auth, bflauth00.auth_amount, 12);
						cumul_auth[12]='\0';
						memcpy(cumul_ecs, bflauth00.emi_amount, 12);
						cumul_ecs[12]='\0';


						f_printf(stderr, "Current:  Cumul Amount: %d, Cumul Ecs: %d\n",
						atoi(cumul_auth), atoi(cumul_ecs));


						icumul_auth += atoi(cumul_auth);
						icumul_ecs += atoi(cumul_ecs);

						f_printf(stderr, "After:  Cumul Amount: %d, Cumul Ecs: %d\n\n",
						icumul_auth, icumul_ecs);

					}
					if(isread(lnsauto, &bflauth00, ISNEXT)!=0)
					{
						f_printf(stderr, "BFLAUTH: ERRNO: %d\n", iserrno);
						break;
					}
				}
			}
			else
			{
				f_printf(stderr, "isstart failed for LN-SAUTO. Errno: %d\n", iserrno);
			}



			f_printf(stderr, "=================================================\n");
			f_printf(stderr, "Starting search in LN-AUTP.\n");

			/*
			* Fetching the transactions from LN-AUTP.
			*/
			memset(&bflauth00, ' ', sizeof(bflauth00));
			memcpy(bflauth00.customer_id, cust, strlen(cust));
			retry=0;
			do
			{
				res_Value = isstart ( lnhauto, &skey, 19,  &bflauth00, ISEQUAL );
				retry++;
			}
			while ( ( iserrno == ELOCKED ) && ( retry < NB_FOIS ) );

			if(0==res_Value)
			{
				f_printf(stderr, "isstart successful for LN-AUTP.\n");
				isread(lnhauto, &bflauth00, ISCURR);
				f_printf(stderr, "2-----------> %d\n", iserrno);
				while(iserrno!=EENDFILE)
				{
					f_printf(stderr, "--------> [%.12s] [%.19s] [%.12s] [%.12s] [%.1s] [%.1s] [%.10s] [%.2s]\n",
					bflauth00.reference_number, bflauth00.customer_id,
					bflauth00.auth_amount, bflauth00.emi_amount,
					bflauth00.auth_match_flag, bflauth00.auth_source_rev, 
					bflauth00.product_id, bflauth00.response_code);
					f_printf(stderr, "Customer: [%s], [%d]\n", cust, strlen(cust));

					if(memcmp(cust, bflauth00.customer_id, strlen(cust))!=0)
					break;

					if(('0'==bflauth00.auth_match_flag[0] || ' '==bflauth00.auth_match_flag[0])
					&&
					(memcmp(bflauth00.product_id, bflrthauth.productcode, 8)==0)
					&&
					('A'==bflauth00.auth_source_rev[0])
					&&
					(memcmp(bflauth00.response_code, "00", 2)==0)
					)
					{
						f_printf(stderr, "\nBefore: Cumul Amount: %d, Cumul Ecs: %d\n",
						icumul_auth, icumul_ecs);

						memcpy(cumul_auth, bflauth00.auth_amount, 12);
						cumul_auth[12]='\0';
						memcpy(cumul_ecs, bflauth00.emi_amount, 12);
						cumul_ecs[12]='\0';


						f_printf(stderr, "Current:  Cumul Amount: %d, Cumul Ecs: %d\n",
						atoi(cumul_auth), atoi(cumul_ecs));


						icumul_auth += atoi(cumul_auth);
						icumul_ecs += atoi(cumul_ecs);

						f_printf(stderr, "After:  Cumul Amount: %d, Cumul Ecs: %d\n\n",
						icumul_auth, icumul_ecs);

					}

					if(isread(lnhauto, &bflauth00, ISNEXT)!=0)
					{
						f_printf(stderr, "BFLAUTHH: ERRNO: %d\n", iserrno);
						break;
					}

				}
			}
			else
			{
				f_printf(stderr, "isstart failed for LN-AUTP. Error: %d\n", iserrno);
			}

			sprintf(p_cumul_auth, "%012d", icumul_auth);
			sprintf(p_cumul_ecs, "%012d", icumul_ecs);

			break;

		#ifdef MRL_264
		case 2:
		
			f_printf(stderr, "=================================================\n");
			f_printf(stderr, "Starting search in LN-SAUTO for CIBIL Unbooked count.\n");
			/*
			* Fetching the transactions from LN-SAUTO.
			*/
			memset(&bflauth00, ' ', sizeof(bflauth00));
			memcpy(bflauth00.customer_id, cust, strlen(cust));
			retry=0;
			do
			{
				res_Value = isstart ( lnsauto, &skey, 19,  &bflauth00, ISEQUAL );
				retry++;
			}
			while ( ( iserrno == ELOCKED ) && ( retry < NB_FOIS ) );

			if(0==res_Value)
			{
				f_printf(stderr, "isstart successful for LN-SAUTO for Case 2.\n");
				isread(lnsauto, &bflauth00, ISCURR);
				f_printf(stderr, "SAUTO Read Return: %d\n", iserrno);
				while(iserrno!=EENDFILE)
				{
					f_printf(stderr, "Case2: [%.12s] [%.19s] [%.12s] [%.12s] [%.1s] [%.1s] [%.10s] [%.2s]\n",
						bflauth00.reference_number, bflauth00.customer_id,
						bflauth00.auth_amount, bflauth00.emi_amount,
						bflauth00.auth_match_flag, bflauth00.auth_source_rev, 
						bflauth00.product_id, bflauth00.response_code);
					
					f_printf(stderr, "Customer: [%s], [%d]\n", cust, strlen(cust));

					if(memcmp(cust, bflauth00.customer_id, strlen(cust))!=0)
						break;
					
					if(('0'==bflauth00.auth_match_flag[0] || ' '==bflauth00.auth_match_flag[0])
					&&('A'==bflauth00.auth_source_rev[0])
					&& (memcmp(bflauth00.response_code, "00", 2)==0)
					)
					{
						Unbooked_count++;
					}
					if(isread(lnsauto, &bflauth00, ISNEXT)!=0)
					{
						f_printf(stderr, "BFLAUTH for Case 2: ERRNO: %d\n", iserrno);
						break;
					}
				}
			}
			else
			{
				f_printf(stderr, "isstart failed for LN-SAUTO for case 2. Errno: %d\n", iserrno);
			}
			
			
			f_printf(stderr, "=================================================\n");
			f_printf(stderr, "Starting search in LN-AUTP.\n");

			/*
			* Fetching the transactions from LN-AUTP.
			*/
			memset(&bflauth00, ' ', sizeof(bflauth00));
			memcpy(bflauth00.customer_id, cust, strlen(cust));
			retry=0;
			do
			{
				res_Value = isstart ( lnhauto, &skey, 19,  &bflauth00, ISEQUAL );
				retry++;
			}
			while ( ( iserrno == ELOCKED ) && ( retry < NB_FOIS ) );

			if(0==res_Value)
			{
				f_printf(stderr, "isstart successful for LN-AUTP.\n");
				isread(lnhauto, &bflauth00, ISCURR);
				f_printf(stderr, "AUTP Read Return: %d\n", iserrno);
				while(iserrno!=EENDFILE)
				{
					f_printf(stderr, "HCase2 [%.12s] [%.19s] [%.12s] [%.12s] [%.1s] [%.1s] [%.10s] [%.2s]\n",
						bflauth00.reference_number, bflauth00.customer_id,
						bflauth00.auth_amount, bflauth00.emi_amount,
						bflauth00.auth_match_flag, bflauth00.auth_source_rev, 
						bflauth00.product_id, bflauth00.response_code);

					f_printf(stderr, "Customer: [%s], [%d]\n", cust, strlen(cust));

					if(memcmp(cust, bflauth00.customer_id, strlen(cust))!=0)
						break;

					if(('0'==bflauth00.auth_match_flag[0] || ' '==bflauth00.auth_match_flag[0])
					&&('A'==bflauth00.auth_source_rev[0])
					&& (memcmp(bflauth00.response_code, "00", 2)==0))
					{
						Unbooked_count++;
					}
					
					if(isread(lnhauto, &bflauth00, ISNEXT)!=0)
					{
						f_printf(stderr, "BFLAUTHH: ERRNO: %d\n", iserrno);
						break;
					}

				}
			}
			else
			{
				f_printf(stderr, "isstart failed for LN-AUTP. Error: %d\n", iserrno);
			}
			
			sprintf(p_cumul_auth, "%02d",Unbooked_count);
			f_printf(stderr, "Setting the Unbooked count to %s\n", p_cumul_auth);
			
			break;
		#endif

		default:
			f_printf(stderr, "get_cumul_auth_amount executed for non-recognized type.\n");
			
	}
	
        f_printf(stderr, "RRN: [%.12s] before unlocking lnsauto:[%.6s] \n"
		,bflrthauth.reference_number, (char *)GetTime() );
	isunlock(lnsauto);
        f_printf(stderr, "RRN: [%.12s] after unlocking lnsauto:[%.6s] \n"
		,bflrthauth.reference_number ,(char *)GetTime() );

        f_printf(stderr, "RRN: [%.12s] before unlocking lnhauto:[%.6s] \n"
		,bflrthauth.reference_number, (char *)GetTime());
	isunlock(lnhauto);
        f_printf(stderr, "RRN: [%.12s] after unlocking lnhauto:[%.6s] \n"
		,bflrthauth.reference_number,(char *)GetTime() );

/*
        f_printf(stderr, "Before closing lnsauto \n" );
	isclose(lnsauto);
        f_printf(stderr, "after closing lnsauto:[%.6s] \n",(char *)GetTime() );
        f_printf(stderr, "Before closing lnhauto \n" );
	isclose(lnhauto);
        f_printf(stderr, "after closing lnhauto:[%.6s] \n",(char *)GetTime() );
*/
}

#endif

#ifdef ADDON_PHASE_2
void Card_Block_Status()
{
	f_printf(stderr,"Inside Card_Block_Status()...\n");
	int check = 0, card_count = 0;
	char add_on_card_count[3];
	t_bflzport l_bflzport ;
	struct keydesc portekey;

	portekey.k_flags = ISDUPS;
	portekey.k_nparts = 1;
	portekey.k_part[0].kp_start = 19;
	portekey.k_part[0].kp_leng =  19;
	portekey.k_part[0].kp_type = CHARTYPE;

	f_printf(stderr,"Cust id for Card_Block_Status   [%.19s]\n", bflzbcom.lbcom_customer_id);

	memset(&l_bflzport,' ',sizeof(t_bflzport));
	memcpy(l_bflzport.lport_cust_id,bflzbcom.lbcom_customer_id,19);

	if(isstart(lnporte, &portekey, 19, &l_bflzport, ISEQUAL) == 0)
	{
		f_printf(stderr,"IS START....\n");
		if(isread( lnporte, (char *)&l_bflzport, ISCURR) == 0)
		{
			f_printf(stderr,"IS READ.... ISCURR\n");
			while(EENDFILE != iserrno)
			{
				if(memcmp(l_bflzport.lport_cust_id,bflzbcom.lbcom_customer_id,19) != 0)
				{
					break;
				}

				if(memcmp(l_bflzport.lport_primary_addon_card_flag,"A",1) == 0)
				{
					card_count ++;
				}

				f_printf(stderr,"Checking Card for block or unblock..\n");
				f_printf(stderr,"Card [%.19s] CustId [%.19s] Status_rto [%.1s] Status_cs [%.1s]\n",
				l_bflzport.lport_card_no, bflzbcom.lbcom_customer_id,
				l_bflzport.lport_rto_block_flag, l_bflzport.lport_cs_block_flag);

				if(memcmp(l_bflzport.lport_rto_block_flag,"Y",1) == 0
				|| memcmp(l_bflzport.lport_cs_block_flag,"Y",1) == 0)
				{
					f_printf(stderr,"Card [%.19s]...is blocked\n", l_bflzport.lport_card_no);
					check = 1;
				}

				isread( lnporte,  &l_bflzport, ISNEXT );
			}
		}
	}

	f_printf(stderr," Counter =  [%d] \n",card_count);

	if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0)
	{
		card_count = 0;
		f_printf(stderr,"In case of TXN through addon card, card count = %d\n",card_count);
	}

	memset(bflrthauth.add_on_card_count,' ',sizeof(bflrthauth.add_on_card_count));
	sprintf(add_on_card_count,"%02d",card_count);
	memcpy(bflrthauth.add_on_card_count,add_on_card_count,2);

	f_printf(stderr,"BEFORE STRUCTURE---  ADD ON CARD COUNT [%.2s] \n",add_on_card_count);
	f_printf(stderr,"ADD ON CARD COUNT [%.2s] \n",bflrthauth.add_on_card_count);

	if(1 == check)
	{
		f_printf(stderr,"Updating card block status as [B]\n");
		memcpy(bflrthauth.card_block_status,"B",1);
	}
}
#endif

