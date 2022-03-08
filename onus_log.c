
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_log.c                                          *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 16-Feb-2000                                         *
 * Description         : Log the authorization details to Auth file          *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   24-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2   17-Jan-05 Gilu Alex CR-MXP-0501-0285 Provide original MEID for 00008  *
 *     CR-MXP-0501-0285 Reviewed by Michel Alex   17-Jan-05                  * 
 *                                                                           *
 *****************************************************************************/
 #define CBA  1

/* Added for EMV early issuance */
#define EMV_EARLY_ISSUANCE  1 
#define BFL_CUG_CARD 1
 #define ADDON        1  
#define BFL_PHASE2  1

#define  EDEN_ACT 1

#define  SHELL_RELOAD_VOID_IS  1


 #include <stdio.h> 
 #include <stdlib.h>
 #include <isam.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <paczisoauto.h>
 #include <pcrzbale.h>
 #include <paczpoau.h>
 #include <paczau00.h>
 #include <paczauto.h>
 #include <paczpa05.h>
 #include <paczeuro.h>
 #include <paczmalf.h>
 #include <scenario.h>
 #include <routage.h>
 #include <ath-trc.h>
 #include <MalFuncError.h>
 #include <onus.h>
 #include "onus_log.h"

 #ifdef BFL_CUG_CARD
 #include <bflauth00.h>
 t_bflauth00  bflauth00;
 #include <BFLPROC.h>
 extern t_procdata    procdata;
 #include <bflzport.h>
 extern t_bflzport bflzport;
 #include <bflzbcli.h>
 extern t_bflzbcli bflzbcli;
 #include <bflzbcom.h>
 extern t_bflzbcom  bflzbcom;
 #include <bflhistoryauth.h>
 extern  t_bflhistauth      bflhistauth;
 #include <bfl_dealer_master.h>
 extern  t_bfl_dealer_master bfl_dealer_master;
 #include <bfl_rthauth.h>
 extern t_bflrthauth  bflrthauth;
 #include <bflzaddon.h>
 extern t_lnaddport   lnaddport;
 extern int 	lnsauto;
 extern int 	Cibil_Done;
 extern char   	parent_rrn[];
 extern char   	Original_Amount[];
 #ifdef BFL_PHASE2
 extern char   	Prefered_Limit[];
 extern int    	Ecs_Waive_Off;
 extern int    	Velocity_Check;
 extern int    	Mobile_No_Tranx ;
 extern char    acquiring_channel[];
 #endif
 #endif

#define MRL_383 1
#ifdef MRL_383
        extern int rset_card_1st_fee;
#endif

#define MRL_489	1

#define MRL_287
#ifdef MRL_287
	extern char mob_num_change_flag[];
#endif

/* MRL_356 */
extern char txn_mob_num[16];

#define MRL_304
#ifdef  MRL_304
	extern char e_flag[2];
#endif

 #define MUTHOOTGOLD

#define RULE_ENGINE 1

#ifdef RULE_ENGINE
	extern char failedRule[];
#endif

extern char old_cibil_score[], old_cibil_date[], old_cibil_time[];

extern char LocalAmount[];
extern char rsa_flag[];
/* preema added for Muthoot DR  100609 */
extern char Institution_Id_code[];       
extern char orig_data_OD[];              


extern 	char	CODE_SCENARIO_O;
extern	char	CODE_SAUTO_HAUTO;

extern t_paczisoauto	paczisoauto;	/* Authorization(ISO data fields) 
										  Structure */
extern t_paczpa05		paczpa05;		/* Card Issuer details structure */
extern t_paczpoau		paczpoau;		/* Card holder details structure */
extern t_pcrzbale		pcrzbale;		/* Account deatils structure */
extern t_paczau00		paczau00;		/* Authorization Details */
extern t_paczauto		paczauto;		/* Authorization History Details */
extern t_ath_trc		athtrc;			/* Authorization trace Details */

/* ISAM FILE descriptors */
extern 	int		fdseuro;	/* EURO Authorization movement file */
extern int    	fdathtrc;   /* Authorisation trace file */

/* FUNCTION PROTOTYPES */
extern int DumpISOAuto ( t_paczisoauto* isoautoRec );
extern int EnvoiAlarm();
extern double atof ( char* );
extern char* GetDateYYMMDD (void);
#ifdef CBA
   extern char CBS_LOCAL_AMT []; 
#endif 
int UpdateSautoFromHauto ( char* SubResult );

#ifdef EMV_EARLY_ISSUANCE 
  #include <rthauth.h>
  extern t_rthauth rthauth;
#endif 
 

int WriteToSeuro ( t_entete_iso* IsoHeader, char* TxnType )
{
	t_paczeuro		EuroRec;	


	memset ( (char*)&EuroRec, ' ', sizeof ( t_paczeuro ) );

	memcpy ( EuroRec.euro_reference, paczisoauto.retrieval_reference_number,\
	 12 );

	memcpy ( EuroRec.euro_type, TxnType, 4 );

	memcpy ( EuroRec.euro_date, GetDateYYMMDD (), 6 );

	memcpy ( EuroRec.euro_cd_reponse, IsoHeader->cd_reponse, 2 );

	memcpy ( EuroRec.euro_num_autorisation, IsoHeader->accord, 6 );

	if ( iswrite ( fdseuro, (char*)&EuroRec ) != 0 )
	{
		FPRINTF ( stderr, " ONUS: ERROR IN WRITING TO FC-SEURO %d\n", iserrno );
		return ( ECHEC );
	}
	
	return ( SUCCESS );
}


int UpdateSautoFromHauto ( char* SubResult )
{

	memcpy ( paczau00.au00_reference_auto, paczauto.auto_reference_auto, 12 );
	memcpy ( paczau00.au00_montant_distribue, "000000000000", 12 );
	memcpy ( paczau00.au00_merchant_bank_code, paczauto.auto_cd_banque, 5 );
	memcpy ( paczau00.au00_reference_issuer, paczauto.auto_reference_issuer,12);
	memcpy ( paczau00.au00_num_auto_centre, paczauto.auto_num_auto_centre, 6 );
	memcpy ( paczau00.au00_num_carte, paczauto.auto_num_carte, 19 );
	memcpy ( paczau00.au00_num_commercant, paczauto.auto_num_commercant, 15 );
	memcpy ( paczau00.au00_num_auto_emetteur,paczauto.auto_num_auto_emetteur,6);
	memcpy ( paczau00.au00_cd_emetteur, paczauto.auto_cd_emetteur, 9 );
	memcpy ( paczau00.au00_cd_banque, paczauto.auto_cd_banque, 5 );
	memcpy ( paczau00.au00_date_validite, paczauto.auto_date_validite, 4 );
	memcpy ( paczau00.au00_date_demande, paczauto.auto_date_demande, 6 );
	memcpy ( paczau00.au00_heure_demande, paczauto.auto_heure_demande, 6 );
	memcpy ( paczau00.au00_montant_demande, paczauto.auto_montant_demande, 12 );
	memcpy ( paczau00.au00_cd_monnaie, paczauto.auto_cd_monnaie, 3 );
	memcpy ( paczau00.au00_cd_scenario, paczauto.auto_cd_scenario, 1 );
	memcpy ( paczau00.au00_cd_compte, paczauto.auto_cd_compte, 1 );
	memcpy ( paczau00.au00_cd_reponse_centre,paczauto.auto_cd_reponse_centre,1);
	memcpy ( paczau00.au00_cd_raison_refus,paczauto.auto_cd_raison_refus, 2 );
	memcpy ( paczau00.au00_cd_raison_refus_emet,\
	 paczauto.auto_cd_raison_refus_emet, 2 );
	memcpy ( paczau00.au00_mode_appel, paczauto.auto_mode_appel, 2 );
	memcpy ( paczau00.au00_cd_appel_centre, paczauto.auto_cd_appel_centre, 2 );
	memcpy ( paczau00.au00_date_creation, paczauto.auto_date_creation, 6 );
	memcpy ( paczau00.au00_cd_user, paczauto.auto_cd_user, 6 );
	memcpy ( paczau00.au00_systeme_codage, paczauto.auto_systeme_codage, 1 );
	memcpy ( paczau00.au00_processing_code, paczauto.auto_processing_code, 6 );
	memcpy ( paczau00.au00_receiving_institution_identification_code, paczauto.auto_receiving_institution_identification_code, 11);
        memcpy ( paczau00.au00_montant_mon_loc,paczauto.auto_montant_mon_loc,12);
	return ( SUCCESS );
}

void LoadAuthTrace ( t_struct_iso* ISOStruct )
{

      char          *temp;  
      char          tempx[1286];
	  char			mti[5];
      int           i; 
	  t_paczisoauto	authorisation_request;
  #ifdef BFL_CUG_CARD
  if(memcmp(ISOStruct->entete.banque,"00059",5) == 0 )
  return;
  #endif

 /* replace all nulls with spaces in isoauth structure */

  memcpy ( (char*)&authorisation_request,(char*)&paczisoauto, \
   sizeof ( t_paczisoauto ) );
  temp = (char *)&authorisation_request;

   for ( i=0; i < sizeof(t_paczisoauto); ++i)
       if ( *(temp+i) == '\0' )     
	  *(temp+i) = ' ';


/***************************************/
/*             Key Fields              */
/***************************************/
 
memcpy(athtrc.bank_code, ISOStruct->entete.banque, 5 );
memcpy(athtrc.mxp_ref, ISOStruct->entete.reference, 12 );
sprintf ( mti, "%04d", ISOStruct->entete.type );
memcpy(athtrc.mti, mti, 4 );

memcpy(athtrc.date	,GetDateYYYYMMDD(),8);
memcpy(athtrc.time	,GetTime(),6);
	
/***************************************/
/*          Iso Auth Details           */
/***************************************/

memcpy(athtrc.primary_account_number, authorisation_request.primary_account_number, 19 );

memcpy(athtrc.processing_code,authorisation_request.processing_code,processing_code_len);

memcpy(athtrc.amount_transaction,authorisation_request.amount_transaction,amount_transaction_len);

memcpy(athtrc.amount_billing_settlement,authorisation_request.amount_cardholder_billing,amount_cardholder_billing_len);

memcpy(athtrc.transmission_date_time,authorisation_request.transmission_date_time,transmission_date_time_len);

memcpy(athtrc.conversion_rate_cardholder_billing,authorisation_request.conversion_rate_cardholder_billing,conversion_rate_cardholder_billing_len);

memcpy(athtrc.system_trace_audit_number,authorisation_request.system_trace_audit_number,system_trace_audit_number_len);

memcpy(athtrc.date_expiration,authorisation_request.date_expiration,date_expiration_len);

memcpy(athtrc.merchant_type,authorisation_request.merchant_type,merchant_type_len);

memcpy(athtrc.acquiring_institution_country_code,authorisation_request.acquiring_institution_country_code,acquiring_institution_country_code_len);

memcpy(athtrc.pos_entry_mode,authorisation_request.pos_entry_mode_code,pos_entry_mode_code_len);

memcpy(athtrc.pos_condition,authorisation_request.pos_condition_code,pos_condition_code_len);

memcpy(athtrc.pos_pin_capture,authorisation_request.pos_pin_capture_code,pos_pin_capture_code_len);

memcpy(athtrc.acquiring_institution_identification_code,authorisation_request.acquiring_institution_identification_code,acquiring_institution_identification_code_len);

memcpy(athtrc.forwarding_institution_identification_code,authorisation_request.forwarding_institution_identification_code,forwarding_institution_identification_code_len);

/***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, start ******
memcpy(athtrc.track2_data,authorisation_request.track2_data,track2_data_len-1);
 ***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, end   ******/

if(memcmp(athtrc.primary_account_number, "7000", 4) == 0) /* Muthoot - Michel */
   memcpy(athtrc.track2_data, paczisoauto.other_amounts, 16);

memcpy(athtrc.retrieval_reference_number,authorisation_request.retrieval_reference_number, 12 );

if ( strlen ( paczisoauto.response_code ) != 0 )
memcpy ( athtrc.response_code, paczisoauto.response_code, strlen ( paczisoauto.response_code ) );

if ( strlen ( paczisoauto.authorization_identification_response ) != 0 )
memcpy ( athtrc.authorization_identification_response, paczisoauto.authorization_identification_response, 6 );

/****** Saju *********/
  memcpy ( athtrc.response_code,ISOStruct->entete.cd_reponse,2  );
  memcpy ( athtrc.authorization_identification_response,ISOStruct->entete.accord,6 );

memcpy(athtrc.card_acceptor_terminal_identification,authorisation_request.card_acceptor_terminal_identification,card_acceptor_terminal_identification_len);

/************
memcpy(athtrc.card_acceptor_identification_code,authorisation_request.card_acceptor_identification_code,card_acceptor_identification_code_len);
************/
/* CR-MXP-0501-0285 start */
if ( memcmp( ISOStruct->entete.banque, "00008", 5 ) == 0 )
   memcpy(athtrc.card_acceptor_identification_code,authorisation_request.card_acceptor_identification_code,card_acceptor_identification_code_len);
else
   memcpy(athtrc.card_acceptor_identification_code,ISOStruct->entete.commercant,15);
/* CR-MXP-0501-0285 end */

memcpy(athtrc.card_acceptor_name_location,authorisation_request.card_acceptor_name_location,card_acceptor_name_location_len);

/***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, start ******
memcpy(athtrc.track1_data,authorisation_request.track1_data,track1_data_len);
 ***** MCI-PCI CR-MXP-0508-0557, Veeresh/Desai, 170106, end   ******/

memcpy(athtrc.currency_code_transaction,authorisation_request.currency_code_transaction,currency_code_transaction_len);

if ( strlen ( paczisoauto.currency_code_settlement ) != 0 )
memcpy(athtrc.currency_billing_settlement,authorisation_request.currency_code_settlement,currency_code_settlement_len);
else
memcpy(athtrc.currency_billing_settlement,authorisation_request.currency_code_cardholder_billing,currency_code_cardholder_billing_len);

memcpy(athtrc.pos_entry_capability_code,authorisation_request.pos_entry_capability_code,pos_entry_capability_code_len);

memcpy(athtrc.replacement_amounts,authorisation_request.replacement_amounts,replacement_amounts_len);

memcpy(athtrc.receiving_institution_identification_code,authorisation_request.receiving_institution_identification_code,receiving_institution_identification_code_len);

memcpy(athtrc.account_identification_1,authorisation_request.account_identification_1,account_identification_1_len);

memcpy(athtrc.account_identification_2,authorisation_request.account_identification_2,account_identification_2_len);

memcpy(athtrc.original_message_type_identifier,authorisation_request.original_message_type_identifier,original_message_type_identifier_len);

memcpy(athtrc.issuing_institution_identification_code,authorisation_request.issuing_institution_identification_code,issuing_institution_identification_code_len);

/***************************************/
/*          Card     Details           */
/***************************************/

memcpy(athtrc.expiry_date                ,paczpoau.poau_date_validite,4);
memcpy(athtrc.validity_code              ,paczpoau.poau_code_validite,1);
memcpy(athtrc.opposition_date            ,paczpoau.poau_date_opposition,6);
memcpy(athtrc.capture_code               ,paczpoau.poau_carte_a_capturer,1);
memcpy(athtrc.referal_code               ,paczpoau.poau_cd_appel,1);
memcpy(athtrc.currency_code              ,paczpoau.poau_cd_monnaie,3);
memcpy(athtrc.card_limit           		 ,paczpoau.poau_plafond_carte,12);
memcpy(athtrc.cash_limit           		 ,paczpoau.poau_plafond_cash,12);
memcpy(athtrc.card_advice_limit    		 ,paczpoau.poau_card_advice,12);
memcpy(athtrc.cash_advice_limit     	 ,paczpoau.poau_cash_advice,12);
memcpy(athtrc.primary_card_code          ,paczpoau.poau_cd_carte_prim,1);
memcpy(athtrc.scenario_code              ,paczpoau.poau_cd_scenario,1);
memcpy(athtrc.card_refreshing_frequency  ,paczpoau.poau_periodicite_plafond,2);
memcpy(athtrc.cash_refreshing_frequency  ,paczpoau.poau_periodicite_cash,2);
memcpy(athtrc.wrong_pin_attempt_number   ,paczpoau.poau_nb_pin_faux,1);
memcpy(athtrc.card_modification_date     ,paczpoau.poau_date_maj_AA,2);
memcpy(athtrc.card_modification_date+2   ,paczpoau.poau_date_maj_MM,2);
memcpy(athtrc.card_modification_date+4   ,paczpoau.poau_date_maj_JJ,2);

/********************************************/
/*            Account Structure             */
/********************************************/

memcpy(athtrc.account_number		    ,pcrzbale.bale_numero_compte,24);
memcpy(athtrc.account_currency_code     ,pcrzbale.bale_code_monnaie,3);
memcpy(athtrc.account_credit_limit	    ,pcrzbale.bale_plafond_credit,12);
memcpy(athtrc.account_credit_advice     ,pcrzbale.bale_credit_advice,12);
memcpy(athtrc.account_purchase_available,pcrzbale.bale_solde_bancaire,12);
memcpy(athtrc.account_cash_limit		,pcrzbale.bale_facilites_caisse,12);
memcpy(athtrc.account_cash_advice		,pcrzbale.bale_cash_advice,12);
memcpy(athtrc.account_cash_available	,pcrzbale.bale_cumul_utilisations,12);
memcpy(athtrc.authorisation_flag	    ,pcrzbale.bale_top_autorisation,1);
memcpy(athtrc.date_of_last_approved_auth ,pcrzbale.bale_date_autorisation,6);
memcpy(athtrc.time_of_last_authorisation ,pcrzbale.bale_heure_autorisation,6);
memcpy(athtrc.vip			    		 ,pcrzbale.bale_vip,1);
memcpy(athtrc.percentage_over_floor_limit ,pcrzbale.bale_pourc_depass_vip,5);
memcpy(athtrc.account_modification_date   ,pcrzbale.bale_date_maj_AA,2);
memcpy(athtrc.account_modification_date+2 ,pcrzbale.bale_date_maj_MM,2);
memcpy(athtrc.account_modification_date+4 ,pcrzbale.bale_date_maj_JJ,2);

memcpy(athtrc.request_local_amount, LocalAmount, 12 ); 

  temp = (char *)&athtrc;

   for ( i=0; i < sizeof(t_ath_trc ); ++i)
       if ( *(temp+i) == '\0' )     
	  *(temp+i) = ' ';

if( iswrite( fdathtrc, (char *)&athtrc ) < 0 )
{
    memcpy(athtrc.mxp_ref, GetDateYYMMDD(), 6 );
    memcpy(athtrc.mxp_ref+6, GetTime(), 6 );

    if( iswrite( fdathtrc, (char *)&athtrc ) < 0 )
       FPRINTF(stderr, "**********Error writting ATH-AUTH %d *********\n", iserrno );

}

return ;
}

#ifdef BFL_CUG_CARD

int BFL_WriteToSauto ( t_entete_iso* IsoHeader, char* SubResult )
{
        int     TempLen = 0;
        char    description[MAXSIZEDSCR];
        char    *temp = NULL;
        int     i = 0;
	char merchantid[15+1];
	char terminalid[8+1];
	char dealer_source[4];


        FPRINTF ( stderr, "\n==== ISO RECORD - WRITING BFL-SAUTO ========= \n" );
        if(memcmp(paczisoauto.processing_code,"21",2) == 0 )
        return ( SUCCESS );
        if(memcmp(paczisoauto.processing_code,"30",2) == 0 )
        return ( SUCCESS );

       
        memset ( (char*)&bflauth00, ' ', sizeof ( t_bflauth00 ) );

        memcpy ( bflauth00.reference_number, paczisoauto.retrieval_reference_number, 12 );
        FPRINTF( stderr, "Issuer Reference [%s] \n", paczisoauto.retrieval_reference_number );
      
        if ( strlen ( paczisoauto.primary_account_number ) == 0 )
        {
                memcpy ( bflauth00.card_number,bflzport.lport_card_no , 19 );
        }
	else
        {
                memcpy ( bflauth00.card_number, paczisoauto.primary_account_number,strlen ( paczisoauto.primary_account_number ) );
        }

        if(memcmp(IsoHeader->cd_reponse,"B1",2) == 0)
        memcpy ( bflauth00.customer_id,"                   " ,19 );
        else
        memcpy ( bflauth00.customer_id,bflzport.lport_cust_id ,19 );

        memcpy ( bflauth00.bank_code,"00059" ,5 );
	memcpy ( bflauth00.merchant_branch_id, bfl_dealer_master.supplier_branch,20 );
#ifdef ADDON 
        if(memcmp(bflzport.lport_primary_addon_card_flag,"A",1) == 0 )
        memcpy ( bflauth00.mobile_no, lnaddport.laddon_mobile, 15 ); 
        else
#endif
        memcpy ( bflauth00.mobile_no, bflzbcli.lbcli_residence_mobile, 15 );
        if(memcmp(IsoHeader->cd_reponse,"00",2) == 0)
	memcpy ( bflauth00.auth_code, IsoHeader->accord, 6 );
        else
	memcpy ( bflauth00.auth_code, "        ", 8 );
	memcpy ( bflauth00.merchant_no,paczisoauto.card_acceptor_identification_code, 15 );
	memcpy ( bflauth00.terminal_no, paczisoauto.card_acceptor_terminal_identification, strlen(paczisoauto.card_acceptor_terminal_identification) );
	memcpy ( bflauth00.merchant_name, paczisoauto.card_acceptor_name_location, 40 );
	memcpy ( bflauth00.merchant_loc,  paczisoauto.card_acceptor_name_location+25, 13 );
	memcpy ( bflauth00.card_exp_date, bflzport.lport_exp_dt, 4 );
	memcpy ( bflauth00.sys_exp_date, bflzport.lport_stexp_dt, 4 );
	memcpy ( bflauth00.auth_req_date, GetDateYYMMDD (), 6 );
	memcpy ( bflauth00.auth_req_time, GetTime (), 6 );
        if(strlen(Original_Amount) > 0 )
	memcpy ( bflauth00.auth_amount, Original_Amount , 12 );
	memcpy ( bflauth00.auth_match_flag," " ,1 );
	memcpy ( bflauth00.response_code, IsoHeader->cd_reponse, 2 );
	memcpy ( bflauth00.response_sub_code, IsoHeader->cd_reponse, 2 );
        if(strlen(transaction_source) > 0 )
	memcpy ( bflauth00.auth_source_code, transaction_source, 2 );
        else
	memcpy ( bflauth00.auth_source_code, "  ", 2 );
        if(memcmp(IsoHeader->cd_reponse,"00",2) == 0)
	memcpy ( bflauth00.auth_source_rev, "A ", 2 );
        else
	memcpy ( bflauth00.auth_source_rev, "  ", 2 );
	memcpy ( bflauth00.auth_processing_code,paczisoauto.processing_code , 6 );
	memcpy ( bflauth00.pos_entry_mode, paczisoauto.pos_entry_mode_code,3 );
        if((strlen(Otp_Code) > 0 ) && (strlen(Otp_Request_id) > 0 ) ) 
        memcpy ( bflauth00.pos_entry_mode+2, "9",1 );

     /*	memcpy ( bflauth00.partner_edc_channel, "0000000000", 10 ); */

	memcpy(merchantid, bflauth00.merchant_no, 15);
	merchantid[15]='\0';
	memcpy(terminalid, bflauth00.terminal_no, 8);
	terminalid[8]='\0';

	memcpy(bflauth00.txn_mobile_num, txn_mob_num, strlen(txn_mob_num));

        if(strlen(acquiring_channel) > 0 )
	memcpy(bflauth00.partner_edc_channel, acquiring_channel, 2);
        else
	memcpy(bflauth00.partner_edc_channel, "          ", 10);

        if(strlen(buisness_prod_code) > 0)
	memcpy ( bflauth00.product_id, buisness_prod_code ,strlen(buisness_prod_code));
          if(strlen(scheme_id) > 0 )
		memcpy ( bflauth00.scheme_code, scheme_id, strlen(scheme_id) );
          else
		memset ( bflauth00.scheme_code,' ' , 10 );
          if(strlen(manufacture_id) > 0 )
		memcpy ( bflauth00.manufacture_id, manufacture_id, strlen(manufacture_id) );
          else
		memset ( bflauth00.manufacture_id ,' ' , 10 );
          if(strlen(asset_cat) > 0 )
          memcpy ( bflauth00.asset_id, asset_cat, strlen(asset_cat) );
          else
		memset ( bflauth00.asset_id,' ' , 10 );
          if(strlen(model_id) > 0 )
		memcpy ( bflauth00.model_id, model_id, strlen(model_id) );
          else
		memset ( bflauth00.model_id ,' ' , 10 );
          if(strlen(imei_ser_no) > 0 )
	  	memcpy ( bflauth00.product_serial_no, imei_ser_no, strlen(imei_ser_no) );
          else
		memset ( bflauth00.product_serial_no,' ' , 15 );
        if(strlen(procdata.bfl_loan_ten)> 0 )
	memcpy ( bflauth00.tenor,procdata.bfl_loan_ten , 5 );
        if(strlen(procdata.emiamount)> 0 )
	memcpy ( bflauth00.emi_amount, procdata.emiamount, 12 );
        if((strlen(procdata.adv_emiamount)> 0 )&&((memcmp(procdata.adv_emiamount,"            ",12) != 0)&&(memcmp(procdata.adv_emiamount,"000000000000",12) != 0)))
	memcpy ( bflauth00.advance_emi_amount,  procdata.adv_emiamount,12 );
        else
	memcpy ( bflauth00.advance_emi_amount, "000000000000",12 );
        if(strlen(buisness_prod_code) > 0 )
	memcpy ( bflauth00.bfl_buisness_product_code,buisness_prod_code ,strlen(buisness_prod_code) );
        if((strlen(promo_code) > 0)&&(memcmp(transaction_source,"WP",2) == 0 ))
	  memcpy ( bflauth00.promo_code, promo_code, strlen(promo_code) );
        else
	  memcpy ( bflauth00.promo_code, "                              ", 30 );
          memset ( bflauth00.exception_msg,' ',150);

        if((strlen(relation_type)  > 0 )&& (memcmp(relation_type,"C",1) == 0)&&(memcmp(parent_rrn,"            ",12) != 0 )&&(strlen(parent_rrn)> 0))
	memcpy ( bflauth00.parent_rrn_number, parent_rrn, 12 );
        else
	memcpy ( bflauth00.parent_rrn_number, "            ", 12 ); 
	if(
		(memcmp(buisness_prod_code, "REMI", 4)==0)
		|| (memcmp(buisness_prod_code, "MPC", 3)==0)
		|| (memcmp(buisness_prod_code, "MPD", 3)==0)
	)
		memcpy ( bflauth00.customer_due_cycle, "02", 2 );
	else if(
		(memcmp(bflzbcli.lbcli_base_loan_fdd,"      ",6) == 0)||
		(memcmp(bflzbcli.lbcli_base_loan_fdd,"000000",6) == 0)
	)
		memcpy ( bflauth00.customer_due_cycle, "02", 2 );
	else
		memcpy ( bflauth00.customer_due_cycle, bflzbcli.lbcli_base_loan_fdd+4, 2 );

        if((memcmp(paczisoauto.processing_code,"00",2) == 0 ) && (memcmp(IsoHeader->cd_reponse,"00",2) == 0) )
        {
		memcpy ( bflauth00.reward_bfl, procdata.bfl_accured_point, 10 );
		memcpy ( bflauth00.reward_dealer, procdata.dealer_accured_point, 10 );
		memcpy ( bflauth00.reward_manufacture, procdata.manufacture_accured_point, 10 );
        }

	  #ifdef MRL_383
	 f_printf(stderr,"rset_card_1st_fee = %d\n",rset_card_1st_fee);
	  if(rset_card_1st_fee == 2)
          {
                  fprintf(stderr,"Writing Charge ID and REMI_EMI_CARD_CHARGES in LN-SAUTO for Sale\n");
                  memcpy ( bflauth00.charge_id1, "00601634", 8 );
                  memcpy ( bflauth00.charge_id1_amount, bflzport.lport_emi_card_charges, 12 );
                  fprintf(stderr,"Charge ID1 [%.8s] and CARD_ID1_AMOUNT [%.12s] in RTH-AUTH\n",bflauth00.charge_id1,bflauth00.charge_id1_amount);
          }
	  else
	  #endif
	  {
		  memcpy ( bflauth00.charge_id1, procdata.charge_id1, 8 );
		  memcpy ( bflauth00.charge_id1_amount, procdata.charge_amount1, 12 );
	  }
	memcpy ( bflauth00.charge_id2, procdata.charge_id2, 8 );
	memcpy ( bflauth00.charge_id2_amount, procdata.charge_amount2, 12 );
	memcpy ( bflauth00.charge_id3, procdata.charge_id3, 8 );
	memcpy ( bflauth00.charge_id3_amount, procdata.charge_amount3, 12 );
        memcpy ( bflauth00.charge_id4, procdata.charge_id4, 8 );
	memcpy ( bflauth00.charge_id4_amount, procdata.charge_amount4, 12 );
	memcpy ( bflauth00.charge_id5,  procdata.charge_id5, 8 );
	memcpy ( bflauth00.charge_id5_amount, procdata.charge_amount5, 12 );
	memcpy ( bflauth00.txn_risk_classification, procdata.trans_risk, 2 );
	memcpy ( bflauth00.asset_risk_classification, procdata.asset_risk,2 );
	memcpy ( bflauth00.customer_risk_classification, procdata.customer_risk, 2 );
	memcpy ( bflauth00.credit_program_code, procdata.cr_pr_sour, 10 );
        memcpy ( bflauth00.last_upd_date,GetDateYYMMDD (),6);
        if((strlen(rsa_flag) > 0 ))
	memcpy ( bflauth00.fraud_flag,rsa_flag , 1);
        if((strlen(addr_change_falg) > 0 ))
	memcpy ( bflauth00.addr_chng_flg,addr_change_falg ,1);

#ifdef MRL_287
	if(strlen(mob_num_change_flag)>0)
		memcpy(bflauth00.mob_num_change, mob_num_change_flag, 1);
#endif

#ifdef MRL_304
	if(strlen(e_flag)>0)
		memcpy(bflauth00.e_flag, e_flag, 1);
#endif

        if((strlen(relation_type) > 0 ))
	memcpy ( bflauth00.relationship_type,relation_type, 1 );
        if(strlen(procdata.adv_emicount) > 0 )
	memcpy ( bflauth00.no_of_adv_emi,procdata.adv_emicount+1,2);
        if(strlen(procdata.proce_fees) > 0 )
	memcpy ( bflauth00.processing_fee,procdata.proce_fees,12 );

	f_printf(stderr,"Cibil_Done [%d]",Cibil_Done);

        if(1 == Cibil_Done)
        {
		/* MRL_336 */
		memcpy ( bflauth00.old_cibil_date, old_cibil_date, 6 );
		memcpy ( bflauth00.old_cibil_time, old_cibil_time, 6 );
		memcpy ( bflauth00.old_cibil_score, old_cibil_score,6 );

		memcpy ( bflauth00.cibil_done,"Y" , 1);
		memcpy ( bflauth00.cibil_date,bflzbcom.lbcom_cibil_score_date, 6);
		memcpy ( bflauth00.cibil_time, GetTime(), 6);
		memcpy ( bflauth00.cibil_score,bflzbcom.lbcom_cibil_score, 6);

        }
        else
                memcpy (bflauth00.cibil_done,bflrthauth.cibil_done,1);

       	Cibil_Done = 0;
        if(strlen(procdata.DBD) > 0 )
	memcpy( bflauth00.dbd_subven_perc,procdata.DBD , 10 );
	memcpy( bflauth00.me_dealer_id, paczisoauto.card_acceptor_identification_code+5,10 );
        if(strlen(emi_card_fee) > 0 )
	memcpy( bflauth00.card_charges,emi_card_fee ,12 );
        if(strlen(procdata.rate_of_interest) > 0 )
	memcpy( bflauth00.interest_rate, procdata.rate_of_interest, 5 );
        if(strlen(Avail_Amount) > 0 )
	memcpy( bflauth00.available_limit,Avail_Amount,12);
        memset( Avail_Amount,0x00,strlen(Avail_Amount));
	memcpy ( bflauth00.transaction_amount, paczisoauto.amount_transaction , 12 );
        memcpy ( bflauth00.customer_bscore,bflzbcom.lbcom_b_score,6);
        if((strlen(Requested_reward_point) == 6 )&& (memcmp(paczisoauto.processing_code,"99",2) == 0 ) )
        memcpy(bflauth00.redeem_point,Requested_reward_point,6);
        memcpy(bflauth00.credit_limit,bflzbcom.lbcom_acc_credit_limit,12);
        #ifdef BFL_PHASE2
        if(strlen(prefered_limit_flag) > 0)
        memcpy(bflauth00.prefered_limit_flag,prefered_limit_flag,1);
 
        if((strlen(Prefered_Limit) > 0)&&(memcmp(bflzbcom.lbcom_acc_prefferd_limit,"000000000000",12) == 0 )
        &&(memcmp(prefered_limit_flag,"Y",1) == 0 )&&(strlen(prefered_limit_flag) > 0))
           memcpy(bflauth00.prefered_limit,Prefered_Limit,12);
        if(Ecs_Waive_Off == 1)
        memcpy(bflauth00.ecs_bypass,"Y",1);
        Ecs_Waive_Off = 0;
        if(Velocity_Check == 1)
        memcpy(bflauth00.velcoity_bypass,"Y",1);
        Velocity_Check = 0;
        if(strlen(Vas_Amount) == 12)
        memcpy(bflauth00.vas_amount,Vas_Amount,12);


	if(memcmp(acquiring_channel, "01", 2)==0)
		memcpy(dealer_source, "WLD", 3);
	else if(memcmp(acquiring_channel, "02", 2)==0)
		memcpy(dealer_source, "WLP", 3);
	else if(memcmp(acquiring_channel, "03", 2)==0)
		memcpy(dealer_source, "PLD", 3);
	else if(memcmp(acquiring_channel, "04", 2)==0)
		memcpy(dealer_source, "MSD", 3);
	else if(memcmp(acquiring_channel, "05", 2)==0)
		memcpy(dealer_source, "EAE", 3);
	else if(memcmp(acquiring_channel, "06", 2)==0)
		memcpy(dealer_source, "BFW", 3);
	else if(memcmp(acquiring_channel, "07", 2)==0)
		memcpy(dealer_source, "WLO", 3);
	else if(memcmp(acquiring_channel, "08", 2)==0)
		memcpy(dealer_source, "EXP", 3);
	else if(memcmp(acquiring_channel, "09", 2)==0)
		memcpy(dealer_source, "ERE", 3);
	else if(memcmp(acquiring_channel, "10", 2)==0)
		memcpy(dealer_source, "EAR", 3);
	/* 
 	 * WebPOS Invoice Finance transaction for (LSF)
	 */
	else if(memcmp(acquiring_channel, "33", 2)==0)
		memcpy(dealer_source, "WLI", 3);
	/* 
 	 * WebPOS Product Finance transaction for (CD)
	 */
	else if(memcmp(acquiring_channel, "95", 2)==0)
		memcpy(dealer_source, "WPC", 3);
	else
		memcpy(dealer_source, "   ", 3);
		
		
        memcpy(bflauth00.dealer_source,dealer_source,3);

      
        if(Mobile_No_Tranx == 1)
        memcpy(bflauth00.mobile_transaction,"Y",1);
        else
        memcpy(bflauth00.mobile_transaction,"N",1);
        Mobile_No_Tranx = 0 ;
        #endif



	#ifdef RULE_ENGINE

		f_printf(stderr,"\n SubResult [%.2s]\n",SubResult);

		if ((memcmp(SubResult, "00", 2 ) != 0) && (' ' != failedRule[0]) && ('\0' !=  failedRule[0]))
		{
			memcpy(bflauth00.failed_rule_id, failedRule, strlen(failedRule) );
			f_printf(stderr,"\n failedRule [%.6s]\n",failedRule);
			f_printf(stderr,"\n bflauth00.failed_rule_id [%.6s] \n", bflauth00.failed_rule_id );
		}
	#endif


            f_printf(stderr,"\n RECORD [%.1250s]\n",&bflauth00);

		/* Added this line SO we can get record even if buffering fails */
            fprintf(stderr,"\n RECORD [%.1250s]\n",&bflauth00); 

            f_printf(stderr,"\n num_commercant ..[%.15s]\n",bflauth00.merchant_no);
            f_printf(stderr,"\n commercant [%.15s]\n",IsoHeader->commercant);
            temp = (char*)&bflauth00;
            for(i = 0 ; i < sizeof(t_bflauth00);++i)
             if(*(temp+i) == '\0')
              *(temp+i) = ' ' ;

        if ( iswrite ( lnsauto, (char*)&bflauth00 ) != 0 )
        {
                if ( iserrno == 100 )
                {
                        DPRINTF ( stdout, "DUPLICATE RRNO ISSUED ****** [%.12s]\n", \
                         bflauth00.reference_number );
                        fflush ( stdout );
                }

                f_printf(stderr, "\n ONUS: WRITE ERROR LN-SAUTO %d\n", iserrno );
                return ( iserrno );
        }
        isunlock(lnsauto);
        isrelease(lnsauto);


        return ( SUCCESS );
}

int BFL_UpdateSauto (char* SubResult )
{
        int     Result = -1;
        char    UsedAmount[13];
        char    AuthAmount[13];
        char    LocAmount[13];

        double  FltAmount = 0.0;

        FPRINTF ( stderr, "BFL-UPDATING AUTHORIZATION \n" );

        memset ( LocAmount, 0, 13 );
        FPRINTF ( stderr, "BFL-UPDATING AUTHORIZATION RECORD AMOUNT[%.12s]\n",bflauth00.transaction_amount);
        FPRINTF ( stderr, "BFL-UPDATING AUTHORIZATION RECORD SOURCE REV[%.1s]\n",bflauth00.auth_source_rev );
        if ( CODE_SAUTO_HAUTO == 'H' )
        {
                memset((char *)&bflauth00,0x20,sizeof(t_bflauth00));
        	memcpy((char *)&bflauth00,(char *)&bflhistauth,sizeof(t_bflhistauth));
        }

        if(memcmp ( bflauth00.transaction_amount, paczisoauto.amount_transaction, 12 ) == 0 )
	{
                if(
			(
				(memcmp(bflauth00.product_id,"REMI",4)==0)
				||(memcmp(bflauth00.product_id,"ERF",3)==0)
				||(memcmp(bflauth00.product_id,"ECF",3)==0)
				||(memcmp(bflauth00.product_id,"WLT",3)==0)
			) 
                   &&(memcmp(paczisoauto.processing_code,"20",2) == 0 )&&(memcmp(SubResult,"00",2) == 0)
		)
			;
                else if (memcmp(SubResult,"00",2) == 0)
                {
        		memcpy ( LocAmount, "000000000000", 12 );
        		LocAmount[12] = 0;
                        stchar ( LocAmount, bflauth00.auth_amount, 12 );
                }
                else
                ;
        }
        else if(bflauth00.auth_source_rev[0]=='R')
					;
				else
        return;

        if((memcmp(paczisoauto.processing_code,"23",2) == 0 ) || (memcmp(paczisoauto.processing_code,"39",2) == 0 )||(memcmp(paczisoauto.processing_code,"03",2) == 0 ))
        ;
        else if(bflauth00.auth_source_rev[0] == 'R')
        return;

        /*stchar ( GetDateYYMMDD (), bflauth00.auth_req_date, 6 );*/
        memcpy ( bflauth00.last_upd_date,GetDateYYMMDD (),6);

        if((memcmp(paczisoauto.processing_code,"20",2) == 0 ) || (memcmp(paczisoauto.processing_code,"02",2) == 0 ))
        {
        	memcpy ( bflauth00.original_rrn_refund,paczisoauto.retrieval_reference_number , 12 );
        	memcpy ( bflauth00.auth_processing_code,paczisoauto.processing_code,6);
		
		#ifdef MRL_489
			memcpy(bflauth00.rev_txn_time, GetTime(), 6);
			f_printf(stderr, "Reverse Transaction Time [%.6s]\n", bflauth00.rev_txn_time);
		#endif
        }

        /* Update the transaction as reversed */
        if(memcmp(SubResult,"00",2) == 0)
        {
					f_printf(stderr, "Source Rev : [%c]/[%.2s]\n",bflauth00.auth_source_rev[0],paczisoauto.processing_code);
					if(bflauth00.auth_source_rev[0]=='A')
					{
						bflauth00.auth_source_rev[0] = 'R';
						f_printf(stderr, "Srouce rev changed to \"R\"\n");
					}
					else if(bflauth00.auth_source_rev[0]=='R' && memcmp(paczisoauto.processing_code,"39",2)==0)
					{
						f_printf(stderr, "Srouce rev changed to \"A\"\n");
						bflauth00.auth_source_rev[0] = 'A';
					}
					else if(bflauth00.auth_source_rev[0]=='R' && memcmp(paczisoauto.processing_code,"03",2)==0)
					{
						f_printf(stderr, "Srouce rev changed to \"A\"\n");
						bflauth00.auth_source_rev[0] = 'A';
					}
        } 

        if((memcmp(SubResult,"00",2) == 0)&&(memcmp(paczisoauto.processing_code,"23",2) == 0 ))
           bflauth00.auth_source_rev[0] = 'Q';
	
	f_printf(stderr,"CODE_SAUTO_HAUT [%.1s]\n",CODE_SAUTO_HAUTO );

        if ( CODE_SAUTO_HAUTO == 'S' )
        {
                Result = isrewrite ( lnsauto, (char*)&bflauth00 );
                isunlock ( lnsauto );
                isrelease ( lnsauto );
                /*
                isunlock ( lnhauto );
                */

                if ( Result != 0 )
                {
                        f_printf(stderr, "ONUS: REWRITE ERROR LN-SAUTO \n" );
                        return ( ECHEC );
                }
        }
        else if ( CODE_SAUTO_HAUTO == 'H' )
        {
                bflauth00.auth_source_rev[0] = 'H';
		 /*#ifdef MRL_383
                        fprintf(stderr,"Writing Charge ID and REMI_EMI_CARD_CHARGES in LN-SAUTO for Refund\n");
                        memcpy ( bflauth00.charge_id1, "00601635", 8 );
                        memcpy ( bflauth00.charge_id1_amount, bflzport.lport_emi_card_charges, 12 );
                        fprintf(stderr,"Charge ID1 [%.8s] and CARD_ID1_AMOUNT [%.12s] in RTH-AUTH\n",bflauth00.charge_id1,bflauth00.charge_id1_amount);
                #endif*/

             /*   UpdateSautoFromHauto ( SubResult );*/
                        f_printf(stderr, "ONUS: WRITING RECORD TO   LN-SAUTO \n" );

                Result = iswrite ( lnsauto , (char*)&bflauth00 );
                isunlock ( lnsauto );
                isrelease ( lnsauto );

                if ( Result != 0 )
                {
                        f_printf(stderr, "ONUS: REWRITE ERROR FC-SAUTO \n" );
                        return ( ECHEC );
                }
        }

        return ( SUCCESS );
}
#endif
