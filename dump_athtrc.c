
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : dump_athtrc.c                                       *
 * Author(s)           : Azhakesh A K                                        *
 * Date                : 15-Sep-1999                                         *
 * Description         : Writing of ATH trace record to trace file           *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   19-May-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <ath-trc.h>
#include <define.h> /* NRK 11OCt2005 */
#include "dump_athtrc.h"

/* Added by Pradnya - 01/04/2013 - Start */
int  DumpAuthTrace ( t_ath_trc * );
/* Added by Pradnya - 01/04/2013 - End */

int  DumpAuthTrace ( t_ath_trc *athtrc )
{

#ifdef DEBUG

FPRINTF ( stderr, "\n\nPRIMARY KEY Bank code[%.5s]\n",athtrc->bank_code );
FPRINTF ( stderr, "\nPRIMARY KEY MXP Reference[%.12s]\n",athtrc->mxp_ref );
FPRINTF ( stderr, "\nPRIMARY KEY MTI [%.4s]\n",athtrc->mti );

FPRINTF ( stderr, "\n\nSECONDARY KEY DATE [%.8s]\n",athtrc->date );
FPRINTF ( stderr, "\n\nSECONDARY KEY TIME [%.6s]\n",athtrc->time );


FPRINTF ( stderr, "\n================== D A T A ===================\n");

/*************************/
/* ISO Details]*/
/*****************************/

FPRINTF ( stderr , "\nprimary_account_number    		= [%.19s]  ",	
athtrc-> primary_account_number	                ); 

FPRINTF ( stderr , "\nprocessing_code                           = [%.6s]  ",	
athtrc-> processing_code	                        ); 

FPRINTF ( stderr , "\namount_transaction                        = [%.12s] ",	
athtrc-> amount_transaction	                        ); 

FPRINTF ( stderr , "\namount_billing_settlement                 = [%.12s]" ,	
athtrc-> amount_billing_settlement ); 

FPRINTF ( stderr , "\ntransmission_date_time    		= [%.12s] ",	
athtrc-> transmission_date_time	                ); 

FPRINTF ( stderr , "\nconversion_rate_cardholder_billing	= [%.8s] ",
athtrc-> conversion_rate_cardholder_billing         ); 

FPRINTF ( stderr , "\nsystem_trace_audit_number    		= [%.6s] ",
athtrc-> system_trace_audit_number	                ); 

FPRINTF ( stderr , "\ndate_expiration	        		= [%.4s] ",
athtrc-> date_expiration	                        ); 

FPRINTF ( stderr , "\nmerchant_type                		= [%.4s] ",	
athtrc-> merchant_type                              ); 

FPRINTF ( stderr , "\nacquiring_institution_country_code    	= [%.3s] ",
athtrc-> acquiring_institution_country_code	        ); 

FPRINTF ( stderr , "\npos_entry_mode					= [%.12s] ",
athtrc-> pos_entry_mode ); 

FPRINTF ( stderr , "\npos_condition			          	= [%.2s] ",
athtrc-> pos_condition ); 

FPRINTF ( stderr , "\npos_pin_capture		          	= [%.2s] ",
athtrc-> pos_pin_capture ); 

FPRINTF ( stderr , "\nacquiring_institution_identification_cod= [%.12s] ",	
athtrc-> acquiring_institution_identification_code  ); 

FPRINTF ( stderr , "\nforwarding_institution_identification_co= [%.12s] ",
athtrc-> forwarding_institution_identification_code	); 
/*FPRINTF ( stderr , "\ntrack2_data	                        = [%.37s] ",
athtrc-> track2_data	                        ); */

FPRINTF ( stderr , "\nretrieval_reference_number			= [%.12s] ",
athtrc-> retrieval_reference_number ); 

FPRINTF ( stderr , "\nauthorization_identification_response = [%.6s] ",
athtrc-> authorization_identification_response ); 

FPRINTF ( stderr , "\nresponse_code	              			= [%.3s] ",
athtrc-> response_code	                        ); 

FPRINTF ( stderr , "\ncard_acceptor_terminal_identification  = [%.8s] ",
athtrc-> card_acceptor_terminal_identification	); 

FPRINTF ( stderr , "\ncard_acceptor_identification_code		= [%.15s] ",
athtrc-> card_acceptor_identification_code	        ); 

FPRINTF ( stderr , "\ncard_acceptor_name_location         	= [%.40s] ",	
athtrc-> card_acceptor_name_location                ); 

/*FPRINTF ( stderr , "\ntrack1_data	                	= [%.76s] ",
athtrc-> track1_data	                        ); */

FPRINTF ( stderr , "\ncurrency_code_transaction	                = [%.3s] ",
athtrc-> currency_code_transaction	                ); 

FPRINTF ( stderr , "\ncurrency_billing_settlement	                = [%.3s] ",
athtrc-> currency_billing_settlement	                ); 

FPRINTF ( stderr , "\npos_entry_capability_code	                = [%.1s] ",
athtrc-> pos_entry_capability_code	                ); 

FPRINTF ( stderr , "\nreplacement_amounts]      		= [%.42s] ",
athtrc-> replacement_amounts                        ); 

FPRINTF ( stderr , "\nreceiving_institution_identification      = [%.12s] ",
athtrc-> receiving_institution_identification_code	); 

FPRINTF ( stderr , "\naccount_identification_1	                = [%.28s] ",
athtrc-> account_identification_1	                ); 

FPRINTF ( stderr , "\naccount_identification_2	                = [%.28s] ",
athtrc-> account_identification_2	                ); 

FPRINTF ( stderr , "\noriginal_message_type_identifier	        = [%.4s] ",
athtrc-> original_message_type_identifier	        ); 

FPRINTF ( stderr , "\nissuing_institution_identification_code   = [%.12s] ",
athtrc-> issuing_institution_identification_code	); 

/*******************************/
/*   Card Details]    */
/**********************************/

FPRINTF ( stderr , "\nexpiry_date 	    			= [%.4s] ", 
athtrc-> expiry_date 		); 

FPRINTF ( stderr , "\nvalidity_code 	    			= [%.1s] ",
athtrc-> validity_code 		); 

FPRINTF ( stderr , "\nopposition_date 	                        = [%.6s] ", 
athtrc-> opposition_date 		); 

FPRINTF ( stderr , "\ncapture_code 	    			= [%.1s] ",
athtrc-> capture_code 		); 

FPRINTF ( stderr , "\nreferal_code 	    			= [%.1s] ",
athtrc-> referal_code 		        ); 

FPRINTF ( stderr , "\ncurrency_code     			= [%.3s] ",
athtrc-> currency_code 		        ); 

FPRINTF ( stderr , "\ncard_limit 		   			= [%.12s] ",
athtrc-> card_limit 		); 

FPRINTF ( stderr , "\ncash_limit     				= [%.12s] ",
athtrc-> cash_limit 		); 

FPRINTF ( stderr , "\ncard_advice_limit    			= [%.12s] ",
athtrc-> card_advice_limit 		); 

FPRINTF ( stderr , "\ncash_advice_limit   			= [%.12s] ",
athtrc-> cash_advice_limit 		); 

FPRINTF ( stderr , "\ncard_available_in    			= [%.12s] ",
athtrc-> card_available_in ); 

FPRINTF ( stderr , "\ncash_available_in    			= [%.12s] ",
athtrc-> cash_available_in ); 

FPRINTF ( stderr , "\ncard_available_out   			= [%.12s] ",
athtrc-> card_available_out ); 

FPRINTF ( stderr , "\ncash_available_out   			= [%.12s] ",
athtrc-> cash_available_out ); 

FPRINTF ( stderr , "\nlast_auth_approved_date    		= [%.6s] ", 
athtrc-> last_auth_approved_date                    ); 

FPRINTF ( stderr , "\nlast_cash_approved_date    		= [%.6s] ", 
athtrc-> last_cash_approved_date                    ); 

FPRINTF ( stderr , "\nprimary_card_code        			= [%.1s] ",
athtrc-> primary_card_code                          ); 

FPRINTF ( stderr , "\nscenario_code 	    			= [%.1s] ",
athtrc-> scenario_code 		); 

FPRINTF ( stderr , "\ncard_refreshing_frequency 		= [%.2s] ",
athtrc-> card_refreshing_frequency 		); 

FPRINTF ( stderr , "\ncash_refreshing_frequency 		= [%.2s] ",
athtrc-> cash_refreshing_frequency 		); 

FPRINTF ( stderr , "\nwrong_pin_attempt_number     		= [%.1s] ",
athtrc-> wrong_pin_attempt_number 		); 

FPRINTF ( stderr , "\ncard_modification_date    		= [%.6s] ",
athtrc-> card_modification_date                     ); 

/*********************************/
/*      Account Structure       */
/*********************************/


FPRINTF ( stderr , "\naccount_number 	    			= [%.24s] ", 
athtrc-> account_number 		); 

FPRINTF ( stderr , "\naccount_currency_code    			= [%.3s] ", 
athtrc-> account_currency_code 		); 

FPRINTF ( stderr , "\naccount_credit_limit 	    		= [%.12s] ", 
athtrc-> account_credit_limit 		); 

FPRINTF ( stderr , "\naccount_credit_advice	    		= [%.12s] ", 
athtrc-> account_credit_advice		); 

FPRINTF ( stderr , "\naccount_purchase_available 		= [%.12s] ", 
athtrc-> account_purchase_available ); 

FPRINTF ( stderr , "\naccount_cash_limit				= [%.12s] ", 
athtrc-> account_cash_limit ); 

FPRINTF ( stderr , "\naccount_cash_advice				= [%.12s] ", 
athtrc-> account_cash_advice ); 

FPRINTF ( stderr , "\naccount_cash_available			= [%.12s] ", 
athtrc-> account_cash_available ); 

FPRINTF ( stderr , "\nauthorisation_flag 	    		= [%.1s] ", 
athtrc-> authorisation_flag 		); 

FPRINTF ( stderr , "\ndate_of_last_approved_auth     		= [%.6s] ", 
athtrc-> date_of_last_approved_auth 	); 

FPRINTF ( stderr , "\ntime_of_last_authorisation     		= [%.6s] ", 
athtrc-> time_of_last_authorisation 	); 

FPRINTF ( stderr , "\nvip 		    			= [%.1s] ", 
athtrc-> vip 			); 

FPRINTF ( stderr , "\npercentage_over_floor_limit     		= [%.5s] ", 
athtrc-> percentage_over_floor_limit 	); 

FPRINTF ( stderr , "\naccount_modification_date     		= [%.6s] ",
athtrc-> account_modification_date                  ); 


/***************************/
/*    ON US SUB RESULTS   */
/**************************/

FPRINTF ( stderr , "\nrequest_local_amount				= [%.12s] ",
athtrc-> request_local_amount ); 

FPRINTF ( stderr , "\ncalculated_available				= [%.12s] ",
athtrc-> calculated_available ); 

FPRINTF ( stderr , "\nsub_result                		= [%.6s] ",
athtrc-> sub_result      ); 

FPRINTF(stderr,"\n---------------------------------------\n");
FPRINTF(stderr,"\n---- DUMP  ATH TRACE RECORD  ENDED -----\n");
FPRINTF(stderr,"-----------------------------------------\n");

#endif
return(SUCCES); /* NRK 11Oct2005 */
}

