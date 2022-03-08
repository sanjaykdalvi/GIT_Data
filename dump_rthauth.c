
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : dump_rthauth.c                                      *
 * Author(s)           : Gilu Alex, Sudheer K T                              *
 * Date                : 15-Sep-1999                                         *
 * Description         : Writing of RTH Auth record to trace file            *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   18-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <rthauth.h>
#include <define.h> /* NRK 11Oct2005 */
#include "dump_rthauth.h"


int  PrintRthAuthRecord( t_rthauth   *rthauth)
{
   int  i,lg;
   char * data ;

   lg = sizeof(t_rthauth);
   data = ( char * ) rthauth;

   for(i =0; i< lg; i++)
   {
      if (i%24 == 0)
         FPRINTF(stderr, "\n");

      FPRINTF(stderr, "%02X ", data[i]);
   }
   FPRINTF(stderr, "\n");
   return(SUCCES);
}

int  DumpRthAuthRecord(t_rthauth   *rthauth)
{

#ifdef DEBUG

FPRINTF ( stderr, "\n\nKEY 1 reference_number = [%.12s]\n",rthauth->reference_number);
FPRINTF ( stderr, "KEY 2 card_number      = [%.19s]\n ",rthauth->card_number);
FPRINTF ( stderr, "KEY 3 approval_code    = [%.6s]\n",rthauth->approval_code);
FPRINTF ( stderr, "KEY 4 merchant_code    = [%.15s]\n",rthauth->merchant_code);

FPRINTF ( stderr, "\n================== D A T A ===================\n");

/*************************/
/* ISO Details]*/
/*****************************/

FPRINTF ( stderr , "\nprimary_account_number    		= [%.19s]  ",	
rthauth-> primary_account_number	                ); 

FPRINTF ( stderr , "\nprocessing_code                           = [%.6s]  ",	
rthauth-> processing_code	                        ); 

FPRINTF ( stderr , "\namount_transaction                        = [%.12s] ",	
rthauth-> amount_transaction	                        ); 

FPRINTF ( stderr , "\namount_cardholder_billing                 = [%.12s]" ,	
rthauth-> amount_cardholder_billing	                ); 

FPRINTF ( stderr , "\ntransmission_date_time    		= [%.12s] ",	
rthauth-> transmission_date_time	                ); 

FPRINTF ( stderr , "\nconversion_rate_cardholder_billing	= [%.8s] ",
rthauth-> conversion_rate_cardholder_billing         ); 

FPRINTF ( stderr , "\nsystem_trace_audit_number    		= [%.6s] ",
rthauth-> system_trace_audit_number	                ); 

FPRINTF ( stderr , "\ndate_expiration	        		= [%.4s] ",
rthauth-> date_expiration	                        ); 

FPRINTF ( stderr , "\nmerchant_type                		= [%.4s] ",	
rthauth-> merchant_type                              ); 

FPRINTF ( stderr , "\nacquiring_institution_country_code    	= [%.3s] ",
rthauth-> acquiring_institution_country_code	        ); 

FPRINTF ( stderr , "\nprimary_account_numberextendedcountrycod= [%.3s] ",
rthauth-> primary_account_number_extended_country_code); 

FPRINTF ( stderr , "\npos_entry_mode_code	          	= [%.12s] ",
rthauth-> pos_entry_mode_code	                ); 

FPRINTF ( stderr , "\npos_condition_code	          	= [%.2s] ",
rthauth-> pos_condition_code	                        ); 

FPRINTF ( stderr , "\npos_pin_capture_code	          	= [%.2s] ",
rthauth-> pos_pin_capture_code	                ); 

FPRINTF ( stderr , "\nacquiring_institution_identification_cod= [%.12s] ",	
rthauth-> acquiring_institution_identification_code  ); 

FPRINTF ( stderr , "\nforwarding_institution_identification_co= [%.12s] ",
rthauth-> forwarding_institution_identification_code	); 

FPRINTF ( stderr , "\nprimary_account_number_extended    	= [%.28s] ",
rthauth-> primary_account_number_extended	        ); 

/*FPRINTF ( stderr , "\ntrack2_data	                        = [%.37s] ",
rthauth-> track2_data	                        ); */

FPRINTF ( stderr , "\nresponse_code	              		= [%.3s] ",
rthauth-> response_code	                        ); 

FPRINTF ( stderr , "\ncard_acceptor_terminal_identification   = [%.8s] ",
rthauth-> card_acceptor_terminal_identification	); 

FPRINTF ( stderr , "\ncard_acceptor_identification_code	= [%.15s] ",
rthauth-> card_acceptor_identification_code	        ); 

FPRINTF ( stderr , "\ncard_acceptor_name_location         	= [%.40s] ",	
rthauth-> card_acceptor_name_location                ); 

FPRINTF ( stderr , "\nadditional_response_data	   = [%.25s] ",
rthauth-> additional_response_data	                ); 

/*FPRINTF ( stderr , "\ntrack1_data	                	= [%.76s] ",
rthauth-> track1_data	                        ); */

FPRINTF ( stderr , "\nadditional_data_national    		= [%.1s] ",
rthauth-> additional_data_national	                ); 

FPRINTF ( stderr , "\ncurrency_code_transaction	                = [%.3s] ",
rthauth-> currency_code_transaction	                ); 

FPRINTF ( stderr , "\ncurrency_code_settlement	                = [%.3s] ",
rthauth-> currency_code_settlement	                ); 

FPRINTF ( stderr , "\ncurrency_code_cardholder_billing	        = [%.3s] ",
rthauth-> currency_code_cardholder_billing	        ); 

FPRINTF ( stderr , "\npin_data	                                = [%.16s] ",
rthauth-> pin_data	                                ); 

FPRINTF ( stderr , "\nsecurity_related_control_information	= [%.16s] ",
rthauth-> security_related_control_information	); 

FPRINTF ( stderr , "\nreserved_iso_55	            		= [%.1s] ",
rthauth-> reserved_iso_55	                        ); 

FPRINTF ( stderr , "\nnational_pos_geographic_data	    	= [%.14s] ",
rthauth-> national_pos_geographic_data	        ); 

FPRINTF ( stderr , "\npos_entry_capability_code	                = [%.1s] ",
rthauth-> pos_entry_capability_code	                ); 

FPRINTF ( stderr , "\nother_amounts]               		= [%.36s] ",	
rthauth-> other_amounts                              ); 

FPRINTF ( stderr , "\ndate_action	                	= [%.6s] ",
rthauth-> date_action	                        ); 

FPRINTF ( stderr , "\noriginal_data_elements]   		= [%.42s] ",
rthauth-> original_data_elements	                ); 

FPRINTF ( stderr , "\nfile_update_code	                        = [%.1s] ",
rthauth-> file_update_code	                        ); 

FPRINTF ( stderr , "\nreplacement_amounts]      		= [%.42s] ",
rthauth-> replacement_amounts                        ); 

FPRINTF ( stderr , "\nreceiving_institution_identification      = [%.12s] ",
rthauth-> receiving_institution_identification_code	); 

FPRINTF ( stderr , "\nfile_name	                                = [%.18s] ",
rthauth-> file_name	                                ); 

FPRINTF ( stderr , "\naccount_identification_1	                = [%.28s] ",
rthauth-> account_identification_1	                ); 

FPRINTF ( stderr , "\naccount_identification_2	                = [%.28s] ",
rthauth-> account_identification_2	                ); 

FPRINTF ( stderr , "\noriginal_message_type_identifier	        = [%.4s] ",
rthauth-> original_message_type_identifier	        ); 

FPRINTF ( stderr , "\nissuing_institution_identification_code   = [%.12s] ",
rthauth-> issuing_institution_identification_code	); 

FPRINTF ( stderr , "\nremaining_open_to_use	      		= [%.14s] ",
rthauth-> remaining_open_to_use	                ); 

FPRINTF ( stderr , "\nfile_record_action_and_data	      	= [%.1s] ",
rthauth-> file_record_action_and_data	        ); 

/*******************************/
/*   Card Details]    */
/**********************************/

FPRINTF ( stderr , "\ncardholder_name 	                        = [%.26s] ",
rthauth-> cardholder_name 		); 

FPRINTF ( stderr , "\nbank_code 	    			= [%.5s] ",
rthauth-> bank_code 			); 

FPRINTF ( stderr , "\nexpiry_date 	    			= [%.4s] ", 
rthauth-> expiry_date 		); 

FPRINTF ( stderr , "\nvalidity_code 	    			= [%.1s] ",
rthauth-> validity_code 		); 

FPRINTF ( stderr , "\nopposition_date 	                        = [%.6s] ", 
rthauth-> opposition_date 		); 

FPRINTF ( stderr , "\ncapture_code 	    			= [%.1s] ",
rthauth-> capture_code 		); 

FPRINTF ( stderr , "\nreferal_code 	    			= [%.1s] ",
rthauth-> referal_code 		        ); 

FPRINTF ( stderr , "\ncard_type 	     		= [%.2s] ",
rthauth-> card_type 			); 

FPRINTF ( stderr , "\naccount_billing_mode    			= [%.1s] ",
rthauth-> account_billing_mode                       ); 

FPRINTF ( stderr , "\ncurrency_code     			= [%.3s] ",
rthauth-> currency_code 		        ); 

FPRINTF ( stderr , "\ncard_floor_limit    			= [%.12s] ",
rthauth-> card_floor_limit 		); 

FPRINTF ( stderr , "\ncash_floor_limit     			= [%.12s] ",
rthauth-> cash_floor_limit 		); 

FPRINTF ( stderr , "\navailable_card     			= [%.12s] ",
rthauth-> available_card 		); 

FPRINTF ( stderr , "\navailable_cash     			= [%.12s] ",
rthauth-> available_cash 		); 

FPRINTF ( stderr , "\nlast_approved_auth_date    		= [%.6s] ", 
rthauth-> last_approved_auth_date                    ); 

FPRINTF ( stderr , "\nfirst_account_number     			= [%.24s] ",
rthauth-> first_account_number 	        ); 

FPRINTF ( stderr , "\naccount_number_2 	    			= [%.24s] ",
rthauth-> account_number_2 		); 

FPRINTF ( stderr , "\nprimary_card_flag        			= [%.1s] ",
rthauth-> primary_card_flag                          ); 

FPRINTF ( stderr , "\ncash_floor_percentage    			= [%.3s] ",
rthauth-> cash_floor_percentage                      ); 

FPRINTF ( stderr , "\nscenario_code 	    			= [%.1s] ",
rthauth-> scenario_code 		); 

FPRINTF ( stderr , "\nrefreshing_frequency     			= [%.2s] ",
rthauth-> refreshing_frequency 		); 

FPRINTF ( stderr , "\nfalse_pin_attempt_number     		= [%.1s] ",
rthauth-> false_pin_attempt_number 		); 

FPRINTF ( stderr , "\ncard_modification_date    		= [%.6s] ",
rthauth-> card_modification_date                     ); 

FPRINTF ( stderr , "\ncard_authorisation_cumul       		= [%.12s] ",
rthauth-> card_authorisation_cumul                        ); 

FPRINTF ( stderr , "\ncumul_used               			= [%.12s] ",
rthauth-> cumul_used                                 ); 

FPRINTF ( stderr , "\ncardholder_identification    		= [%.26s] ",
rthauth-> cardholder_identification                  ); 

FPRINTF ( stderr , "\ncheck_verification_value_2    		= [%.3s] ",
rthauth-> check_verification_value_2                 ); 

/*********************************/
/*      Account Structure       */
/*********************************/


FPRINTF ( stderr , "\naccount_number 	    			= [%.24s] ", 
rthauth-> account_number 		); 

FPRINTF ( stderr , "\naccount_bank_code 	    		= [%.5s] ", 
rthauth-> account_bank_code 		); 

FPRINTF ( stderr , "\naccount_currency_code    			= [%.3s] ", 
rthauth-> account_currency_code 		); 

FPRINTF ( stderr , "\nfloor_credit_limit 	    		= [%.12s] ", 
rthauth-> floor_credit_limit 		); 

FPRINTF ( stderr , "\nbank_balance 	    			= [%.12s] ", 
rthauth-> bank_balance 		); 

FPRINTF ( stderr , "\ncredit_on_line 	    			= [%.12s] ", 
rthauth-> credit_on_line 		); 

FPRINTF ( stderr , "\nused_cumulated 	    			= [%.12s] ", 
rthauth-> used_cumulated 		); 

FPRINTF ( stderr , "\naccount_authorisation_cumul 	    	= [%.12s] ", 
rthauth-> account_authorisation_cumul 		); 

FPRINTF ( stderr , "\nauthorisation_flag 	    		= [%.1s] ", 
rthauth-> authorisation_flag 		); 

FPRINTF ( stderr , "\ndate_of_last_approved_auth     		= [%.6s] ", 
rthauth-> date_of_last_approved_auth 	); 

FPRINTF ( stderr , "\ntime_of_last_authorisation     		= [%.6s] ", 
rthauth-> time_of_last_authorisation 	); 

FPRINTF ( stderr , "\nmatching_date 	    			= [%.6s] ", 
rthauth-> matching_date 		); 

FPRINTF ( stderr , "\nbank_account_number      			= [%.24s] ",
rthauth-> bank_account_number                        ); 

FPRINTF ( stderr , "\nvip 		    			= [%.1s] ", 
rthauth-> vip 			); 

FPRINTF ( stderr , "\npercentage_over_floor_limit     		= [%.5s] ", 
rthauth-> percentage_over_floor_limit 	); 

FPRINTF ( stderr , "\ncalculated_available                      = [%.12s] ",
rthauth-> calculated_available );

FPRINTF ( stderr , "\nbranch_code 	    			= [%.5s] ", 
rthauth-> branch_code 		); 

FPRINTF ( stderr , "\naccount_type 	    			= [%.1s] ", 
rthauth-> account_type 		); 

FPRINTF ( stderr , "\naccount_modification_date     		= [%.6s] ",
rthauth-> account_modification_date                  ); 

/*******************************************/
    /*   PVV, CVV Information     */
/******************************************/

FPRINTF ( stderr , "\nservice_code          			= [%.3s] ", 
rthauth-> service_code ); 

/*FPRINTF ( stderr , "\ntrack2_pvv            			= [%.5s] ", 
rthauth-> track2_pvv ); 

FPRINTF ( stderr , "\ntrack2_cvv            			= [%.3s] ", 
rthauth-> track2_cvv );*/ 

FPRINTF ( stderr , "\npin_format            			= [%.2s] ", 
rthauth-> pin_format ); 

FPRINTF ( stderr , "\ncvv_index             			= [%.2s] ", 
rthauth-> cvv_index ); 

/************************************/
/*   ONUS STIP PARAM  INFORMATION   */
/************************************/

FPRINTF ( stderr , "\nmcc_card_type         			= [%.4s] ", 
rthauth-> mcc_card_type ); 

FPRINTF ( stderr , "\nmcc_group            			= [%.4s] ", 
rthauth->  mcc_group   ); 

FPRINTF ( stderr , "\none_day_check         			= [%.1s] ", 
rthauth-> one_day_check ); 

FPRINTF ( stderr , "\none_day_count         			= [%.3s] ", 
rthauth-> one_day_count ); 

FPRINTF ( stderr , "\none_day_amount        			= [%.12s] ", 
rthauth-> one_day_amount ); 

FPRINTF ( stderr , "\nfour_day_check        			= [%.1s] ", 
rthauth-> four_day_check ); 

FPRINTF ( stderr , "\nfour_day_count        			= [%.3s] ", 
rthauth-> four_day_count ); 

FPRINTF ( stderr , "\nfour_day_amount       			= [%.12s] ", 
rthauth-> four_day_amount ); 

FPRINTF ( stderr , "\nmnth_day_check        			= [%.1s] ", 
rthauth-> mnth_day_check ); 

FPRINTF ( stderr , "\nmnth_day_count        			= [%.3s] ", 
rthauth-> mnth_day_count ); 

FPRINTF ( stderr , "\nmnth_day_amount       			= [%.12s] ", 
rthauth-> mnth_day_amount ); 

FPRINTF ( stderr , "\nmulti_day_check       			= [%.1s] ", 
rthauth-> multi_day_check ); 

FPRINTF ( stderr , "\nmulti_no_of_days]     			= [%.2s] ", 
rthauth-> multi_no_of_days ); 

FPRINTF ( stderr , "\nmulti_day_count       			= [%.3s] ", 
rthauth-> multi_day_count ); 

FPRINTF ( stderr , "\nmulti_day_amount      			= [%.12s] ", 
rthauth-> multi_day_amount ); 

/********************************/
/*    MCC VELOCITY INFORMATION  */
/********************************/

FPRINTF ( stderr , "\nveloc_one_day_count         		= [%.3s] ", 
rthauth-> veloc_one_day_count ); 

FPRINTF ( stderr , "\nveloc_one_day_amount        		= [%.12s] ", 
rthauth-> veloc_one_day_amount ); 

FPRINTF ( stderr , "\nveloc_four_day_count        		= [%.3s] ", 
rthauth-> veloc_four_day_count ); 

FPRINTF ( stderr , "\nveloc_four_day_amount       		= [%.12s] ", 
rthauth-> veloc_four_day_amount ); 

FPRINTF ( stderr , "\nveloc_mnth_day_count        		= [%.3s] ", 
rthauth-> veloc_mnth_day_count ); 

FPRINTF ( stderr , "\nveloc_mnth_day_amount       		= [%.12s] ", 
rthauth-> veloc_mnth_day_amount ); 

FPRINTF ( stderr , "\nveloc_multi_day_count       		= [%.3s] ", 
rthauth-> veloc_multi_day_count ); 

FPRINTF ( stderr , "\nveloc_multi_day_amount      		= [%.12s] ", 
rthauth-> veloc_multi_day_amount ); 

/***************************/
/*    ON US SUB RESULTS   */
/**************************/

FPRINTF (stderr , "\nactual_date                		= [%.8s]", 
rthauth->actual_date );

FPRINTF (stderr , "\nactual_time                		= [%.6s]", 
rthauth->actual_time );

FPRINTF ( stderr , "\ncvv_status                		= [%.2s] ", 
rthauth-> cvv_status                                ); 

FPRINTF ( stderr , "\npvv_status               		= [%.2s] ",
rthauth-> pvv_status                                 ); 

FPRINTF ( stderr , "\nuser                      		= [%.6s] ",
rthauth-> user                                       ); 

FPRINTF ( stderr , "\npoau_result               		= [%.2s] ",
rthauth-> poau_result    ); 

FPRINTF ( stderr , "\nbale_result               		= [%.2s] ",
rthauth-> bale_result    ); 

FPRINTF ( stderr , "\nmcc_result                		= [%.2s] ",
rthauth-> mcc_result  ); 

FPRINTF ( stderr , "\nmer_status                		= [%.2s] ",
rthauth-> mer_status    ); 

FPRINTF ( stderr , "\nsub_result                		= [%.6s] ",
rthauth-> sub_result      ); 

FPRINTF ( stderr , "\nrecord_status             		= [%.2s] ",
rthauth-> record_status ); 

FPRINTF(stderr,"\n---------------------------------------\n");
FPRINTF(stderr,"\n---- DUMP  RTH AUTH RECORD  ENDED -----\n");
FPRINTF(stderr,"-----------------------------------------\n");

#endif
return(SUCCES);
}

