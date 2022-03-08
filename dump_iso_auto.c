
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : dump_iso_auto.c                                     *
 * Author(s)           : Gilu Alex                                           *
 * Date                : 15-Sep-1999                                         *
 * Description         : Function to write ISO AUTH details to trace file    *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   19-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <paczisoauto.h>
#include <define.h> /* NRK 11Oct2005 */
#include "dump_iso_auto.h"



int DumpISOAuto ( t_paczisoauto* isoautoRec )
{

#ifdef DEBUG
  FPRINTF(stderr,"\nDump isoauto Start\n  ");
  FPRINTF(stderr,"------------------\n  ");
  if (strlen(isoautoRec->primary_account_number  ) != 0 )
    FPRINTF(stderr,"\n primary_account_number..............%s",
                        isoautoRec->primary_account_number );
  if (strlen(isoautoRec->processing_code  ) != 0 )
    FPRINTF(stderr,"\n processing_code.....................%s",
                        isoautoRec->processing_code );
  if (strlen(isoautoRec->amount_transaction  ) != 0 )
    FPRINTF(stderr,"\n amount_transaction..................%s",
                        isoautoRec->amount_transaction );
  if (strlen(isoautoRec->amount_cardholder_billing  ) != 0 )
    FPRINTF(stderr,"\n amount_cardholder_billing..................%s",
                        isoautoRec->amount_cardholder_billing );
  if (strlen(isoautoRec->transmission_date_time  ) != 0 )
    FPRINTF(stderr,"\n transmission_date_time..............%s",
                        isoautoRec->transmission_date_time );
  if (strlen(isoautoRec->conversion_rate_cardholder_billing) != 0 )
    FPRINTF(stderr,"\n conversion_rate_cardholder_billing..............%s",
                        isoautoRec->conversion_rate_cardholder_billing);
  if (strlen(isoautoRec->system_trace_audit_number  ) != 0 )
    FPRINTF(stderr,"\n system_trace_audit_number...........%s",
                        isoautoRec->system_trace_audit_number );
  if (strlen(isoautoRec->date_expiration  ) != 0 )
    FPRINTF(stderr,"\n date_expiration.....................%s",
                        isoautoRec->date_expiration );
  if (strlen(isoautoRec->merchant_type  ) != 0 )
    FPRINTF(stderr,"\n merchant_type.......................%s",
                         isoautoRec->merchant_type );
  if (strlen(isoautoRec->acquiring_institution_country_code  ) != 0 )
    FPRINTF(stderr,"\n acquiring_institution_country_code..%s",
                        isoautoRec->acquiring_institution_country_code);
  if (strlen(isoautoRec->primary_account_number_extended_country_code)!=0)
    FPRINTF(stderr,"\n primary_account_number_extended_country_code %s",
                     isoautoRec->primary_account_number_extended_country_code);
  if (strlen(isoautoRec->pos_entry_mode_code  ) != 0 )
    FPRINTF(stderr,"\n pos_entry_mode_code...............%s",
                        isoautoRec->pos_entry_mode_code );
  if (strlen(isoautoRec->pos_condition_code  ) != 0 )
    FPRINTF(stderr,"\n pos_condition_code..................%s",
                        isoautoRec->pos_condition_code );
  if (strlen(isoautoRec->pos_pin_capture_code  ) != 0 )
    FPRINTF(stderr,"\n pos_pin_capture_code................%s",
                        isoautoRec->pos_pin_capture_code );
  if (strlen(isoautoRec->acquiring_institution_identification_code) != 0 )
    FPRINTF(stderr,"\n acquiring_institution_identification_code %s",                        isoautoRec->acquiring_institution_identification_code);
  if (strlen(isoautoRec->forwarding_institution_identification_code) != 0)
    FPRINTF(stderr,"\n forwarding_institution_identification_code %s"                       ,isoautoRec->forwarding_institution_identification_code);
  if (strlen(isoautoRec->primary_account_number_extended  ) != 0 )
    FPRINTF(stderr,"\n primary_account_number_extended.....%s",
                        isoautoRec->primary_account_number_extended );
  /*if (strlen(isoautoRec->track2_data  ) != 0 )
    FPRINTF(stderr,"\n track2_data.........................%s",isoautoRec->track2_data );*/
  if (strlen(isoautoRec->retrieval_reference_number  ) != 0 )
    FPRINTF(stderr,"\n retrieval_reference_number..........%s",
                        isoautoRec->retrieval_reference_number );
  if (strlen(isoautoRec->authorization_identification_response  ) != 0 )
    FPRINTF(stderr,"\n authorization_identification_response %s",
                        isoautoRec->authorization_identification_response);
  if (strlen(isoautoRec->response_code  ) != 0 )
    FPRINTF(stderr,"\n response_code.......................%s",
                        isoautoRec->response_code );
  if (strlen(isoautoRec->card_acceptor_terminal_identification  ) != 0 )
    FPRINTF(stderr,"\n card_acceptor_terminal_identification %s",
                        isoautoRec->card_acceptor_terminal_identification);
  if (strlen(isoautoRec->card_acceptor_identification_code  ) != 0 )
    FPRINTF(stderr,"\n card_acceptor_identification_code...%s",
                        isoautoRec->card_acceptor_identification_code );
  if (strlen(isoautoRec->card_acceptor_name_location  ) != 0 )
    FPRINTF(stderr,"\n card_acceptor_name_location.........%s",
                        isoautoRec->card_acceptor_name_location );
  if (strlen(isoautoRec->additional_response_data  ) != 0 )
    FPRINTF(stderr,"\n additional_response_data............%s",
                        isoautoRec->additional_response_data );
  /*if (strlen(isoautoRec->track1_data  ) != 0 )
    FPRINTF(stderr,"\n track1_data.........................%s",isoautoRec->track1_data );*/

  if (strlen(isoautoRec->additional_data_national  ) != 0 )
    FPRINTF(stderr,"\n additional_data_national.............%s",
                        isoautoRec->additional_data_national );

  if (strlen(isoautoRec->additional_data_private  ) != 0 )
    FPRINTF(stderr,"\n additional_data_private.............%s",
                        isoautoRec->additional_data_private );
  if (strlen(isoautoRec->currency_code_transaction  ) != 0 )
    FPRINTF(stderr,"\n currency_code_transaction...........%s",
                        isoautoRec->currency_code_transaction );
  if (strlen(isoautoRec->currency_code_cardholder_billing  ) != 0 )
    FPRINTF(stderr,"\n currency_code_cardholder_billing   %s",
                        isoautoRec->currency_code_cardholder_billing );
  if (strlen(isoautoRec->pin_data  ) != 0 )
    FPRINTF(stderr,"\n pin_data...........................%s",isoautoRec->pin_data );
  if (strlen(isoautoRec->security_related_control_information  ) != 0 )
    FPRINTF(stderr,"\n security_related_control_information %s",
                        isoautoRec->security_related_control_information);
  if (strlen(isoautoRec->reserverd_iso_55  ) != 0 )
    FPRINTF(stderr,"\n reserverd_iso_55.....................%s",
                        isoautoRec->reserverd_iso_55 );
  if (strlen(isoautoRec->national_pos_geographic_data  ) != 0 )
    FPRINTF(stderr,"\n national_pos_geographic_data.........%s",
                        isoautoRec->national_pos_geographic_data );
  if (strlen(isoautoRec->pos_entry_capability_code  ) != 0 )
    FPRINTF(stderr,"\n pos_entry_capability_code............%s",
                        isoautoRec->pos_entry_capability_code );
  if (strlen(isoautoRec->other_amounts  ) != 0 )
    FPRINTF(stderr,"\n other_amounts........................%s",
                        isoautoRec->other_amounts );
  if (strlen(isoautoRec->date_action  ) != 0 )
    FPRINTF(stderr,"\n date_action..........................%s",isoautoRec->date_action );
  if (strlen(isoautoRec->original_data_elements  ) != 0 )
    FPRINTF(stderr,"\n original_data_elements..............%s",
                        isoautoRec->original_data_elements );
  if (strlen(isoautoRec->file_update_code  ) != 0 )
    FPRINTF(stderr,"\n file_update_code....................%s",
                        isoautoRec->file_update_code );
  if (strlen(isoautoRec->receiving_institution_identification_code) != 0 )
    FPRINTF(stderr,"\n receiving_institution_identification_code %s",                        isoautoRec->receiving_institution_identification_code);
  if (strlen(isoautoRec->file_name  ) != 0 )
    FPRINTF(stderr,"\n file_name...........................%s",isoautoRec->file_name );
  if (strlen(isoautoRec->account_identification_1  ) != 0 )
    FPRINTF(stderr,"\n account_identification_1............%s",
                        isoautoRec->account_identification_1 );
  if (strlen(isoautoRec->account_identification_2  ) != 0 )
    FPRINTF(stderr,"\n account_identification_2............%s",
                        isoautoRec->account_identification_2 );
  if (strlen(isoautoRec->original_message_type_identifier  ) != 0 )
    FPRINTF(stderr,"\n original_message_type_identifier....%s",
                        isoautoRec->original_message_type_identifier );
  if (strlen(isoautoRec->issuing_institution_identification_code  ) != 0 )
    FPRINTF(stderr,"\n issuing_institution_identification_code %s",
                        isoautoRec->issuing_institution_identification_code);
  if (strlen(isoautoRec->remaining_open_to_use  ) != 0 )
    FPRINTF(stderr,"\n remaining_open_to_use...............%s",
                        isoautoRec->remaining_open_to_use );
  if (strlen(isoautoRec->file_record_action_and_data  ) != 0 )
    FPRINTF(stderr,"\n file_record_action_and_data.........%s",
                        isoautoRec->file_record_action_and_data );
  if (strlen(isoautoRec->replacement_amounts         ) != 0 )
    FPRINTF(stderr,"\n replacement_amounts.................%s",
                        isoautoRec->replacement_amounts         );

  FPRINTF(stderr,"\n Dump isoauto Stop\n  ");
  FPRINTF(stderr,"-----------------\n  ");
#endif
  return(SUCCES); /* NRK 11Oct2005 */

}
