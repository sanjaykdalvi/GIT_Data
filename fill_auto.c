
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : fill_auto.c                                         *
 * Author(s)           : Gilu Alex                                           *
 * Date                : 10-Mar-2000                                         *
 * Description         : Get the ISO fields for VISA, MASTER,AEGN            *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   25-Mar-00 Gilu Alex            Added AEGN                             *
 * 2   24-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 3   15-Feb-06 Desai CR-MXP-0508-0557 MCI-PCI Review Veeresh               *
 *                                                                           *
 *****************************************************************************/
/* Chowdesha added for CBA */
#define CBA  1

#define SHELL_RELOAD_VOID_IS 1

 #define RUPAY_ISSUANCE   1  /* Added by Amit for RUPAY Issuance on 8 july 2016 */

 #include <stdio.h>
 #include <string.h>
 #include <iso_visa.h>
 #include <iso_aegn.h>
 #include <struct_iso.h>
 #include <paczisoauto.h>
 #include <constantes.h>
 #include <onus.h>



/* Function Prototypes */
int FillAuto ( char* ISOData, char Format, t_paczisoauto* AuthRecord );
int FillAutoVisa ( char* ISOData, t_paczisoauto* AuthRecord );
int FillAutoEuro ( char* ISOData, t_paczisoauto* AuthRecord );
int FillAutoMci ( char* ISOData, t_paczisoauto* AuthRecord );
int FillAutoAegn ( char* ISOData, t_paczisoauto* AuthRecord );

extern int GetFieldVisa ( int, char*, char* );
extern int GetFieldEuro ( int, char*, char* );
extern int GetFieldMci ( int, char*, char* );
extern int GetFieldAegn ( int, char*, char* );

#ifdef RUPAY_ISSUANCE
int FillAutoRupay ( char* ISOData, t_paczisoauto* AuthRecord );
#endif


int FillAuto ( char* ISOData, char Format, t_paczisoauto* AuthRecord )
{

	/* Initialize the Auth record structure */
	memset ( AuthRecord, 0, sizeof ( t_paczisoauto ) );
	AuthRecord->message_format = Format;
	switch ( Format )
	{
	case VISA_SYSTEM:
	case PLUS_SYSTEM:
		analyse_visa ( ISOData );
		FillAutoVisa ( ISOData, AuthRecord );
		break;

	case EURO_SYSTEM:
		analyse_euro ( ISOData );
		FillAutoEuro ( ISOData, AuthRecord );
		break;

	case MCI_SYSTEM:
	case CIRRUS_SYSTEM:
		analyse_mci ( ISOData );
		FillAutoMci ( ISOData, AuthRecord );
		break;

	case AEGN_SYSTEM:
		analyse_aegn ( ISOData );
		FillAutoAegn ( ISOData, AuthRecord );
		break;
	
	/* Added by Amit for RUPAY Issuance on 7 july 2016 */
    	#ifdef RUPAY_ISSUANCE
        case RUPAY_SYSTEM:
      		analyse_visa(ISOData);
          	FillAutoRupay ( ISOData, AuthRecord );
      		break;
    	#endif

	default:
		f_printf(stderr, "\nWARNING : Format [%d] not supported for FillAuto\n", Format );
		return ( ECHEC );
	}

	return ( SUCCESS );
}

int FillAutoVisa ( char* ISOData, t_paczisoauto* AuthRecord )
{
	char STRING_FIELD[512]; 
	/* Added by ACR/Michel 21-02-2008 */
	char field_63[50] ;
	char network_id[10];
	char msg_reason_code[10];
	char stip_reason_code[10] ;

	/* Get the VISA fields and update the ISO auth structure */

	/* PRIMARY ACCOUNT NUMBER  */
	if ( GetFieldVisa ( PRIMARY_ACCOUNT_NUMBER, ISOData, \
	   AuthRecord->primary_account_number ) != 0 )
	{
		/* Get the PAN from TRACK2 Data */
		GetFieldVisa ( TRACK2_DATA, ISOData, AuthRecord->track2_data );
		memcpy ( AuthRecord->primary_account_number, \
		  AuthRecord->track2_data, 16 );
		AuthRecord->primary_account_number[16] = 0;
	}

	/* PROCESSING CODE */
	GetFieldVisa ( PROCESSING_CODE, ISOData, AuthRecord->processing_code );

	/* TRANSACTION AMOUNT */
	GetFieldVisa ( AMOUNT_TRANSACTION, ISOData, \
	   AuthRecord->amount_transaction );

	/* CARD HOLDER BILLING AMOUNT */
	GetFieldVisa ( AMOUNT_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->amount_cardholder_billing );

	/*  TRANSMISSION DATA AND TIME */
	GetFieldVisa ( TRANSMISSION_DATE_TIME, ISOData, \
	  AuthRecord->transmission_date_time );

	/* CONVERSION RATE FOR CARD HOLDER BILLING */
	GetFieldVisa ( CONVERSION_RATE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->conversion_rate_cardholder_billing );

	/* STAN */
	GetFieldVisa ( SYSTEM_TRACE_AUDIT_NUMBER, ISOData, \
	  AuthRecord->system_trace_audit_number );

	/* EXPIRY DATE */
	GetFieldVisa ( DATE_EXPIRATION, ISOData, AuthRecord->date_expiration );

	/* MERCHANT TYPE */
	GetFieldVisa ( MERCHANT_TYPE, ISOData, AuthRecord->merchant_type );

	/* ACQUIRING INSTITUTION COUNTRY CODE */
	GetFieldVisa ( ACQUIRING_INSTITUTION_COUNTRY_CODE, ISOData, \
	  AuthRecord->acquiring_institution_country_code );

	/* POS ENTRY MODE */
	GetFieldVisa ( POS_ENTRY_MODE_CODE, ISOData, \
	  AuthRecord->pos_entry_mode_code );

	/* POS CONDITION CODE */
	GetFieldVisa ( POS_CONDITION_CODE, ISOData, AuthRecord->pos_condition_code);

	/* POS PIN CAPTURE CODE */
	GetFieldVisa ( POS_PIN_CAPTURE_CODE, ISOData, \
	  AuthRecord->pos_pin_capture_code );

	/* ACQUIRING INSTITUTION IDENTIFICATION CODE */
	GetFieldVisa ( ACQUIRING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->acquiring_institution_identification_code );

	/* FORWARDING INSTITUTION IDENTIFICATION CODE */
	GetFieldVisa ( FORWARDING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->forwarding_institution_identification_code );

	/* TRACK2 DATA */
	GetFieldVisa ( TRACK2_DATA, ISOData, AuthRecord->track2_data );

	/* RRNO */
	GetFieldVisa ( RETRIEVAL_REFERENCE_NUMBER, ISOData, \
	  AuthRecord->retrieval_reference_number );

	/* AUTHORIZATION IDENTIFICATION RESPONSE */
	GetFieldVisa ( AUTHORIZATION_IDENTIFICATION_RESPONSE, ISOData, \
	  AuthRecord->authorization_identification_response );

	/* RESPONSE CODE */
	GetFieldVisa ( RESPONSE_CODE, ISOData, AuthRecord->response_code );

	/* TERMINAL ID */
	GetFieldVisa ( CARD_ACCEPTOR_TERMINAL_IDENTIFICATION, ISOData, \
	  AuthRecord->card_acceptor_terminal_identification );

	/* MERCHANT ID/CARD ACCEPTOR ID */
	GetFieldVisa ( CARD_ACCEPTOR_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->card_acceptor_identification_code );

	/* MERCHANT/CARD_ACCEPTOR LOCATION  */
	GetFieldVisa ( CARD_ACCEPTOR_NAME_LOCATION, ISOData, \
	  AuthRecord->card_acceptor_name_location );

	/* ADDITIONAL RESPONSE DATA */
	GetFieldVisa ( ADDITIONAL_RESPONSE_DATA, ISOData, \
	  AuthRecord->additional_response_data );

	/* TRACK1 DATA */
	GetFieldVisa ( TRACK1_DATA, ISOData, AuthRecord->track1_data );

	/* NATIONAL ADDITIONAL DATA  */
	GetFieldVisa ( ADDITIONAL_DATA_NATIONAL, ISOData, \
	  AuthRecord->additional_data_national );

	/* PRIVATE ADDITIONAL DATA */
	GetFieldVisa ( ADDITIONAL_DATA_PRIVATE, ISOData, \
	  AuthRecord->additional_data_private );

	/* TRANSACTION CURRENCY CODE */
	GetFieldVisa ( CURRENCY_CODE_TRANSACTION, ISOData, \
	  AuthRecord->currency_code_transaction );

	/* CARD HOLDER BILLING CURRENCY CODE */
	GetFieldVisa ( CURRENCY_CODE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->currency_code_cardholder_billing );

	/* PIN DATA */
	GetFieldVisa ( PIN_DATA, ISOData, AuthRecord->pin_data );

	/* SECURITY RELATED CONTROL INFORMATION */
	GetFieldVisa ( SECURITY_RELATED_CONTROL_INFORMATION, ISOData, \
	  AuthRecord->security_related_control_information );

     /* Copy the data coming in Field 57 to field 48 */
        /* Added by ACR 15-03-2011 */
        GetFieldVisa ( RESERVERD_NATIONAL_57, ISOData, \
          AuthRecord->additional_data_private );

#ifdef SHELL_RELOAD_VOID_IS
        GetFieldVisa ( NATIONAL_POS_GEOGRAPHIC_DATA, ISOData, \
          AuthRecord->national_pos_geographic_data  );
#endif

	/* POS ENTRY CAPABILITY CODE */
	GetFieldVisa ( POS_ENTRY_CAPABILITY_CODE, ISOData, \
	  AuthRecord->pos_entry_capability_code );

	/* DATE OF ACTION */
	GetFieldVisa ( DATE_ACTION, ISOData, AuthRecord->date_action );

	/* ORIGINAL DATA ELEMENTS */
	GetFieldVisa ( ORIGINAL_DATA_ELEMENTS, ISOData, \
	  AuthRecord->original_data_elements );

	/* FILE UPDATE CODE */
	GetFieldVisa ( FILE_UPDATE_CODE, ISOData, AuthRecord-> file_update_code );

	/* FILE NAME  */
	GetFieldVisa ( FILE_NAME, ISOData, AuthRecord->file_name );

	/* ACCOUNT IDENTIFICATION 1 */
	GetFieldVisa ( ACCOUNT_IDENTIFICATION_1, ISOData, \
	  AuthRecord->account_identification_1);

	/* ACCOUNT IDENTIFICATION 2 */
	GetFieldVisa ( ACCOUNT_IDENTIFICATION_2, ISOData, \
	  AuthRecord->account_identification_2);

	/* REMAINING OPEN TO USE AMOUNT */
	GetFieldVisa ( REMAINING_OPEN_TO_USE, ISOData, \
	  AuthRecord->remaining_open_to_use );

	/* FILE RECORD ACTION AND DATA */
	GetFieldVisa ( FILE_RECORD_ACTION_AND_DATA, ISOData, \
	  AuthRecord->file_record_action_and_data );

	/* OTHER AMOUNTS */
	GetFieldVisa ( OTHER_AMOUNTS, ISOData, AuthRecord->other_amounts );

	/* REPLACEMENT AMOUNTS */
	GetFieldVisa ( REPLACEMENT_AMOUNTS, ISOData, \
	  AuthRecord->replacement_amounts );

	/* ORIGINAL MESSAGE TYPE IDENTIFIER */
	GetFieldVisa ( ORIGINAL_MESSAGE_TYPE_IDENTIFIER, ISOData, \
	  AuthRecord->original_message_type_identifier );

	/* DEBIT PRIVATE USE FOR BPL (FIELD 63 ) */
	if ( IsPresent ( DEBIT_PRIVATE_USE ) == SUCCESS )
	{
		/*
		GetETOP ( DEBIT_PRIVATE_USE, ISOData, \
		  AuthRecord->remaining_open_to_use, 12 );
		*/
		GetFieldVisa( DEBIT_PRIVATE_USE, ISOData, field_63 );
		analyse_63(field_63) ;
		memset( network_id, 0x00, 5 );
		memset( msg_reason_code, 0x00, 5 );
		memset( stip_reason_code, 0x00, 5 );
		/*
		GetSubField63( 1,field_63, network_id ); 
		GetSubField63( 3,field_63, msg_reason_code );
		GetSubField63( 4,field_63, stip_reason_code );
		*/
		GetSubField63( 0,field_63, network_id ); 
		GetSubField63( 2,field_63, msg_reason_code );
		GetSubField63( 3,field_63, stip_reason_code );

		memcpy(AuthRecord->remaining_open_to_use, network_id,4 );
		memcpy(AuthRecord->remaining_open_to_use+4, msg_reason_code,4 );
		memcpy(AuthRecord->remaining_open_to_use+8, stip_reason_code,4);
	}

	/* RESERVED PRIVATE */
	if ( IsPresent ( RESERVED_PRIVATE_126) == SUCCESS )
	{
		GetFieldVisa ( RESERVED_PRIVATE_126, ISOData, STRING_FIELD );
		/* ACR/Michel, analyse_126 function should be called before
		any of the sub fields are extracted Date: 21-02-2008 */
		analyse_126( STRING_FIELD );		/* Added by ACR/Michel 06/03/2008 */
		GetSubField126( 9, STRING_FIELD, AuthRecord->reserved_private_126 );

    /*    f_printf(stderr, "reserved_private_126 [%s] \n", AuthRecord->reserved_private_126 );  MCI-PCI CR-MXP-0508-0557 240106*/
	}

	return ( SUCCESS );
}

int FillAutoEuro ( char* ISOData, t_paczisoauto* AuthRecord )
{

	/* Get the EURO fields and update the ISO auth structure */

	/* PRIMARY ACCOUNT NUMBER  */
	GetFieldEuro ( PRIMARY_ACCOUNT_NUMBER, ISOData, \
	   AuthRecord->primary_account_number ); 

	/* PROCESSING CODE */
	GetFieldEuro ( PROCESSING_CODE, ISOData, AuthRecord->processing_code );

	/* TRANSACTION AMOUNT */
	GetFieldEuro ( AMOUNT_TRANSACTION, ISOData, \
	   AuthRecord->amount_transaction );

	/* CARD HOLDER BILLING AMOUNT */
	GetFieldEuro ( AMOUNT_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->amount_cardholder_billing );

	/*  TRANSMISSION DATA AND TIME */
	GetFieldEuro ( TRANSMISSION_DATE_TIME, ISOData, \
	  AuthRecord->transmission_date_time );

	/* CONVERSION RATE FOR CARD HOLDER BILLING */
	GetFieldEuro ( CONVERSION_RATE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->conversion_rate_cardholder_billing );

	/* STAN */
	GetFieldEuro ( SYSTEM_TRACE_AUDIT_NUMBER, ISOData, \
	  AuthRecord->system_trace_audit_number );

	/* EXPIRY DATE */
	GetFieldEuro ( DATE_EXPIRATION, ISOData, AuthRecord->date_expiration );

	/* MERCHANT TYPE */
	GetFieldEuro ( MERCHANT_TYPE, ISOData, AuthRecord->merchant_type );

	/* ACQUIRING INSTITUTION COUNTRY CODE */
	GetFieldEuro ( ACQUIRING_INSTITUTION_COUNTRY_CODE, ISOData, \
	  AuthRecord->acquiring_institution_country_code );

	/* POS ENTRY MODE */
	GetFieldEuro ( POS_ENTRY_MODE_CODE, ISOData, \
	  AuthRecord->pos_entry_mode_code );

	/* POS CONDITION CODE */
	GetFieldEuro ( POS_CONDITION_CODE, ISOData, AuthRecord->pos_condition_code);

	/* POS PIN CAPTURE CODE */
	GetFieldEuro ( POS_PIN_CAPTURE_CODE, ISOData, \
	  AuthRecord->pos_pin_capture_code );

	/* ACQUIRING INSTITUTION IDENTIFICATION CODE */
	GetFieldEuro ( ACQUIRING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->acquiring_institution_identification_code );

	/* FORWARDING INSTITUTION IDENTIFICATION CODE */
	GetFieldEuro ( FORWARDING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->forwarding_institution_identification_code );

	/* TRACK2 DATA */
	GetFieldEuro ( TRACK2_DATA, ISOData, AuthRecord->track2_data );

	/* RRNO */
	GetFieldEuro ( RETRIEVAL_REFERENCE_NUMBER, ISOData, \
	  AuthRecord->retrieval_reference_number );

	/* AUTHORIZATION IDENTIFICATION RESPONSE */
	GetFieldEuro ( AUTHORIZATION_IDENTIFICATION_RESPONSE, ISOData, \
	  AuthRecord->authorization_identification_response );

	/* RESPONSE CODE */
	GetFieldEuro ( RESPONSE_CODE, ISOData, AuthRecord->response_code );

	/* TERMINAL ID */
	GetFieldEuro ( CARD_ACCEPTOR_TERMINAL_IDENTIFICATION, ISOData, \
	  AuthRecord->card_acceptor_terminal_identification );

	/* MERCHANT ID/CARD ACCEPTOR ID */
	GetFieldEuro ( CARD_ACCEPTOR_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->card_acceptor_identification_code );

	/* MERCHANT/CARD_ACCEPTOR LOCATION  */
	GetFieldEuro ( CARD_ACCEPTOR_NAME_LOCATION, ISOData, \
	  AuthRecord->card_acceptor_name_location );

	/* ADDITIONAL RESPONSE DATA */
	GetFieldEuro ( ADDITIONAL_RESPONSE_DATA, ISOData, \
	  AuthRecord->additional_response_data );

	/* TRACK1 DATA */
	GetFieldEuro ( TRACK1_DATA, ISOData, AuthRecord->track1_data );

	/* NATIONAL ADDITIONAL DATA  */
	GetFieldEuro ( ADDITIONAL_DATA_NATIONAL, ISOData, \
	  AuthRecord->additional_data_national );

	/* PRIVATE ADDITIONAL DATA */
	GetFieldEuro ( ADDITIONAL_DATA_PRIVATE, ISOData, \
	  AuthRecord->additional_data_private );

	/* TRANSACTION CURRENCY CODE */
	GetFieldEuro ( CURRENCY_CODE_TRANSACTION, ISOData, \
	  AuthRecord->currency_code_transaction );

	/* CARD HOLDER BILLING CURRENCY CODE */
	GetFieldEuro ( CURRENCY_CODE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->currency_code_cardholder_billing );

	/* PIN DATA */
	GetFieldEuro ( PIN_DATA, ISOData, AuthRecord->pin_data );

	/* SECURITY RELATED CONTROL INFORMATION */
	GetFieldEuro ( SECURITY_RELATED_CONTROL_INFORMATION, ISOData, \
	  AuthRecord->security_related_control_information );

	/* POS ENTRY CAPABILITY CODE */
	GetFieldEuro ( POS_ENTRY_CAPABILITY_CODE, ISOData, \
	  AuthRecord->pos_entry_capability_code );

	/* DATE OF ACTION */
	GetFieldEuro ( DATE_ACTION, ISOData, AuthRecord->date_action );

	/* ORIGINAL DATA ELEMENTS */
	GetFieldEuro ( ORIGINAL_DATA_ELEMENTS, ISOData, \
	  AuthRecord->original_data_elements );

	/* RECEIVING INSTITUTION IDENTIFICATION CODE */
	GetFieldEuro ( RECEIVING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->receiving_institution_identification_code );

	/* FILE UPDATE CODE */
	GetFieldEuro ( FILE_UPDATE_CODE, ISOData, AuthRecord-> file_update_code );

	/* FILE NAME  */
	GetFieldEuro ( FILE_NAME, ISOData, AuthRecord->file_name );

	/* ACCOUNT IDENTIFICATION 1 */
	GetFieldEuro ( ACCOUNT_IDENTIFICATION_1, ISOData, \
	  AuthRecord->account_identification_1);

	/* ACCOUNT IDENTIFICATION 2 */
	GetFieldEuro ( ACCOUNT_IDENTIFICATION_2, ISOData, \
	  AuthRecord->account_identification_2);

	/* REMAINING OPEN TO USE AMOUNT */
	GetFieldEuro ( REMAINING_OPEN_TO_USE, ISOData, \
	  AuthRecord->remaining_open_to_use );

	/* FILE RECORD ACTION AND DATA */
	GetFieldEuro ( FILE_RECORD_ACTION_AND_DATA, ISOData, \
	  AuthRecord->file_record_action_and_data );

	/* OTHER AMOUNTS */
	GetFieldEuro ( OTHER_AMOUNTS, ISOData, AuthRecord->other_amounts );

	/* REPLACEMENT AMOUNTS */
	GetFieldEuro ( REPLACEMENT_AMOUNTS, ISOData, \
	  AuthRecord->replacement_amounts );

	return ( SUCCESS );
}

int FillAutoMci ( char* ISOData, t_paczisoauto* AuthRecord )
{

	/* Get the MCI fields and update the ISO auth structure */

	/* PRIMARY ACCOUNT NUMBER  */
	GetFieldMci ( PRIMARY_ACCOUNT_NUMBER, ISOData, \
	   AuthRecord->primary_account_number ); 

	/* PROCESSING CODE */
	GetFieldMci ( PROCESSING_CODE, ISOData, AuthRecord->processing_code );

	/* TRANSACTION AMOUNT */
	GetFieldMci ( AMOUNT_TRANSACTION, ISOData, \
	   AuthRecord->amount_transaction );

	/* CARD HOLDER BILLING AMOUNT */
	GetFieldMci ( AMOUNT_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->amount_cardholder_billing );

	/*  TRANSMISSION DATA AND TIME */
	GetFieldMci ( TRANSMISSION_DATE_TIME, ISOData, \
	  AuthRecord->transmission_date_time );

	/* CONVERSION RATE FOR CARD HOLDER BILLING */
	GetFieldMci ( CONVERSION_RATE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->conversion_rate_cardholder_billing );

	/* STAN */
	GetFieldMci ( SYSTEM_TRACE_AUDIT_NUMBER, ISOData, \
	  AuthRecord->system_trace_audit_number );

	/* EXPIRY DATE */
	GetFieldMci ( DATE_EXPIRATION, ISOData, AuthRecord->date_expiration );

	/* MERCHANT TYPE */
	GetFieldMci ( MERCHANT_TYPE, ISOData, AuthRecord->merchant_type );

	/* ACQUIRING INSTITUTION COUNTRY CODE */
	GetFieldMci ( ACQUIRING_INSTITUTION_COUNTRY_CODE, ISOData, \
	  AuthRecord->acquiring_institution_country_code );

	/* POS ENTRY MODE */
	GetFieldMci ( POS_ENTRY_MODE_CODE, ISOData, \
	  AuthRecord->pos_entry_mode_code );

	/* POS CONDITION CODE */
	GetFieldMci ( POS_CONDITION_CODE, ISOData, AuthRecord->pos_condition_code);

	/* POS PIN CAPTURE CODE */
	GetFieldMci ( POS_PIN_CAPTURE_CODE, ISOData, \
	  AuthRecord->pos_pin_capture_code );

	/* ACQUIRING INSTITUTION IDENTIFICATION CODE */
	GetFieldMci ( ACQUIRING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->acquiring_institution_identification_code );

	/* FORWARDING INSTITUTION IDENTIFICATION CODE */
	GetFieldMci ( FORWARDING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->forwarding_institution_identification_code );

	/* TRACK2 DATA */
	GetFieldMci ( TRACK2_DATA, ISOData, AuthRecord->track2_data );

	/* RRNO */
	GetFieldMci ( RETRIEVAL_REFERENCE_NUMBER, ISOData, \
	  AuthRecord->retrieval_reference_number );

	/* AUTHORIZATION IDENTIFICATION RESPONSE */
	GetFieldMci ( AUTHORIZATION_IDENTIFICATION_RESPONSE, ISOData, \
	  AuthRecord->authorization_identification_response );

	/* RESPONSE CODE */
	GetFieldMci ( RESPONSE_CODE, ISOData, AuthRecord->response_code );

	/* TERMINAL ID */
	GetFieldMci ( CARD_ACCEPTOR_TERMINAL_IDENTIFICATION, ISOData, \
	  AuthRecord->card_acceptor_terminal_identification );

	/* MERCHANT ID/CARD ACCEPTOR ID */
	GetFieldMci ( CARD_ACCEPTOR_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->card_acceptor_identification_code );

	/* MERCHANT/CARD_ACCEPTOR LOCATION  */
	GetFieldMci ( CARD_ACCEPTOR_NAME_LOCATION, ISOData, \
	  AuthRecord->card_acceptor_name_location );

	/* ADDITIONAL RESPONSE DATA */
	GetFieldMci ( ADDITIONAL_RESPONSE_DATA, ISOData, \
	  AuthRecord->additional_response_data );

	/* TRACK1 DATA */
	GetFieldMci ( TRACK1_DATA, ISOData, AuthRecord->track1_data );

	/* NATIONAL ADDITIONAL DATA  */
	GetFieldMci ( ADDITIONAL_DATA_NATIONAL, ISOData, \
	  AuthRecord->additional_data_national );

	/*ADDITIONAL DATA PRIVATE */
	GetFieldMci ( 47, ISOData, \
	  AuthRecord->additional_data_private );

	/* TRANSACTION CURRENCY CODE */
	GetFieldMci ( CURRENCY_CODE_TRANSACTION, ISOData, \
	  AuthRecord->currency_code_transaction );

	/* CARD HOLDER BILLING CURRENCY CODE */
	GetFieldMci ( CURRENCY_CODE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->currency_code_cardholder_billing );

 #ifdef CBA 
	/* SETTLEMENT CURRENCY CODE */
	GetFieldMci ( CURRENCY_CODE_SETTLEMENT, ISOData, \
	  AuthRecord->currency_code_settlement );
  #endif 

	/* PIN DATA */
	GetFieldMci ( PIN_DATA, ISOData, AuthRecord->pin_data );

	/* SECURITY RELATED CONTROL INFORMATION */
	GetFieldMci ( SECURITY_RELATED_CONTROL_INFORMATION, ISOData, \
	  AuthRecord->security_related_control_information );

	/* POS ENTRY CAPABILITY CODE */
	GetFieldMci ( POS_ENTRY_CAPABILITY_CODE, ISOData, \
	  AuthRecord->pos_entry_capability_code );

	/* DATE OF ACTION */
	GetFieldMci ( DATE_ACTION, ISOData, AuthRecord->date_action );

	/* ORIGINAL DATA ELEMENTS */
	GetFieldMci ( ORIGINAL_DATA_ELEMENTS, ISOData, \
	  AuthRecord->original_data_elements );

	/* FILE UPDATE CODE */
	GetFieldMci ( FILE_UPDATE_CODE, ISOData, AuthRecord-> file_update_code );

	/* FILE NAME  */
	GetFieldMci ( FILE_NAME, ISOData, AuthRecord->file_name );

	/* ACCOUNT IDENTIFICATION 1 */
	GetFieldMci ( ACCOUNT_IDENTIFICATION_1, ISOData, \
	  AuthRecord->account_identification_1);

	/* ACCOUNT IDENTIFICATION 2 */
	GetFieldMci ( ACCOUNT_IDENTIFICATION_2, ISOData, \
	  AuthRecord->account_identification_2);

	/* REMAINING OPEN TO USE AMOUNT */
	GetFieldMci ( REMAINING_OPEN_TO_USE, ISOData, \
	  AuthRecord->remaining_open_to_use );

	/* FILE RECORD ACTION AND DATA */
	GetFieldMci ( FILE_RECORD_ACTION_AND_DATA, ISOData, \
	  AuthRecord->file_record_action_and_data );

	/* OTHER AMOUNTS */
	GetFieldMci ( OTHER_AMOUNTS, ISOData, AuthRecord->other_amounts );

	/* REPLACEMENT AMOUNTS */
	GetFieldMci ( REPLACEMENT_AMOUNTS, ISOData, \
	  AuthRecord->replacement_amounts );

        /* PRIVATE FIELD */  
        /* preema add field 126 start 251007 */
/*	if ( IsPresent ( RESERVED_PRIVATE_126) == SUCCESS )
	{ */
           GetFieldMci ( RESERVED_PRIVATE_126, ISOData, AuthRecord->reserved_private_126 );

       /* } */
        /* preema add field 126 end 250107 */

	return ( SUCCESS );
}

int FillAutoAegn ( char* ISOData, t_paczisoauto* AuthRecord )
{
	char	TmpAmount[13];

	/* Get the AMEX fields and update the ISO auth structure */

	/* PRIMARY ACCOUNT NUMBER  */
	GetFieldAegn ( PRIMARY_ACCOUNT_NUMBER, ISOData, \
	   AuthRecord->primary_account_number ); 

	/* PROCESSING CODE */
	GetFieldAegn ( PROCESSING_CODE, ISOData, AuthRecord->processing_code );

	/* TRANSACTION AMOUNT */
	GetFieldAegn ( AMOUNT_TRANSACTION, ISOData, \
	   AuthRecord->amount_transaction );

	/* SETTLEMENT AMOUNT */
	GetFieldAegn ( AMOUNT_SETTLEMENT, ISOData, \
	  AuthRecord->amount_cardholder_billing );

	/* STAN */
	GetFieldAegn ( SYSTEM_TRACE_AUDIT_NUMBER, ISOData, \
	  AuthRecord->system_trace_audit_number );

	/* LOCAL TRANSACTION DATE AND TIME */
	GetFieldAegn ( DATE_TIME_LOCAL_TRANSACTION, ISOData, \
	  AuthRecord->transmission_date_time );

	/* EXPIRY DATE */
	GetFieldAegn ( DATE_EXPIRATION, ISOData, AuthRecord->date_expiration );

	/* ACQUIRING INSTITUTION COUNTRY CODE */
	GetFieldAegn ( ACQUIRING_INSTITUTION_COUNTRY_CODE, ISOData, \
	  AuthRecord->acquiring_institution_country_code );

	/* POS DATA CODE */
	GetFieldAegn ( POS_DATA_CODE, ISOData, \
	  AuthRecord->pos_entry_mode_code );

	/* CARD ACCEPTOR BUSINESS CODE */
	GetFieldAegn ( CARD_ACCEPTOR_BUSINESS_CODE, ISOData, \
	  AuthRecord->merchant_type );

	/* ACQUIRING INSTITUTION IDENTIFICATION CODE */
	GetFieldAegn ( ACQUIRING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->acquiring_institution_identification_code );

	/* TRACK2 DATA */
	GetFieldAegn ( TRACK2_DATA, ISOData, AuthRecord->track2_data );

	/* RRNO */
	GetFieldAegn ( RETRIEVAL_REFERENCE_NUMBER, ISOData, \
	  AuthRecord->retrieval_reference_number );

	/* APPROVAL CODE */
	GetFieldAegn ( APPROVAL_CODE, ISOData, \
	  AuthRecord->authorization_identification_response );

	/* RESPONSE/ACTION CODE */
	GetFieldAegn ( ACTION_CODE, ISOData, AuthRecord->response_code );

	/* TERMINAL ID */
	GetFieldAegn ( CARD_ACCEPTOR_TERMINAL_IDENTIFICATION, ISOData, \
	  AuthRecord->card_acceptor_terminal_identification );

	/* MERCHANT ID/CARD ACCEPTOR ID */
	GetFieldAegn ( CARD_ACCEPTOR_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->card_acceptor_identification_code );

	/* MERCHANT/CARD_ACCEPTOR LOCATION  */
	GetFieldAegn ( CARD_ACCEPTOR_NAME_LOCATION, ISOData, \
	  AuthRecord->card_acceptor_name_location );

	/* TRACK1 DATA */
	GetFieldAegn ( TRACK1_DATA, ISOData, AuthRecord->track1_data );

	/* TRANSACTION CURRENCY CODE */
	GetFieldAegn ( CURRENCY_CODE_TRANSACTION, ISOData, \
	  AuthRecord->currency_code_transaction );

	/* SETTLEMENT CURRENCY CODE */
	GetFieldAegn ( CURRENCY_CODE_SETTLEMENT, ISOData, \
	  AuthRecord->currency_code_settlement );

	/* PIN DATA */
	GetFieldAegn ( PIN_DATA, ISOData, AuthRecord->pin_data );

	/* SECURITY RELATED CONTROL INFORMATION */
	GetFieldAegn ( SECURITY_RELATED_CONTROL_INFORMATION, ISOData, \
	  AuthRecord->security_related_control_information );

	/* AMEX ORIGINAL DATA ELEMENTS */
	GetFieldAegn ( AMEX_ORIGINAL_DATA_ELEMENTS, ISOData, \
	  AuthRecord->original_data_elements );



	GetFieldAegn ( AMOUNTS_ORIGINAL, ISOData, \
	  AuthRecord->replacement_amounts );

	/* for Reversal txn */
    if ( memcmp(AuthRecord->amount_transaction,"000000000000",12) == 0 )  
	{
       memcpy ( AuthRecord->amount_transaction, \
		AuthRecord->replacement_amounts, 12 );
	}
	else	/* for Partial reversal */
	{
		if ( strlen ( AuthRecord->replacement_amounts ) != 0 )
		{
			memcpy ( TmpAmount, AuthRecord->replacement_amounts, 12 );
			memcpy ( AuthRecord->replacement_amounts, \
			 AuthRecord->amount_transaction, 12 );
			memcpy ( AuthRecord->amount_transaction, TmpAmount, 12 );
		}
	}

	/* PRIVATE DATA 63 */
	GetFieldAegn ( PRIVATE_DATA_63, ISOData, \
	  AuthRecord->additional_data_private );

	return ( SUCCESS );
}

/* Added by Amit for RUPAY Issuance on 7 july 2016 */
#ifdef RUPAY_ISSUANCE
int FillAutoRupay ( char* ISOData, t_paczisoauto* AuthRecord )
{
	char STRING_FIELD[512]; 
	/* Added by ACR/Michel 21-02-2008 */
	char field_63[50] ;
	char network_id[10];
	char msg_reason_code[10];
	char stip_reason_code[10] ;
	char DE22[10] ;
	char DE61[50] ;
        memset(DE22,' ' ,sizeof(DE22));
        memset(DE61,' ' ,sizeof(DE61));

	/* Get the VISA fields and update the ISO auth structure */

	/* PRIMARY ACCOUNT NUMBER  */
	if ( GetFieldVisa ( PRIMARY_ACCOUNT_NUMBER, ISOData, \
	   AuthRecord->primary_account_number ) != 0 )
	{
		/* Get the PAN from TRACK2 Data */
		GetFieldVisa ( TRACK2_DATA, ISOData, AuthRecord->track2_data );
		memcpy ( AuthRecord->primary_account_number, \
		  AuthRecord->track2_data, 16 );
		AuthRecord->primary_account_number[16] = 0;
	}

	/* PROCESSING CODE */
	GetFieldVisa ( PROCESSING_CODE, ISOData, AuthRecord->processing_code );

	/* TRANSACTION AMOUNT */
	GetFieldVisa ( AMOUNT_TRANSACTION, ISOData, \
	   AuthRecord->amount_transaction );

	/* CARD HOLDER BILLING AMOUNT */
	GetFieldVisa ( AMOUNT_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->amount_cardholder_billing );

	/*  TRANSMISSION DATA AND TIME */
	GetFieldVisa ( TRANSMISSION_DATE_TIME, ISOData, \
	  AuthRecord->transmission_date_time );

	/* CONVERSION RATE FOR CARD HOLDER BILLING */
	GetFieldVisa ( CONVERSION_RATE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->conversion_rate_cardholder_billing );

	/* STAN */
	GetFieldVisa ( SYSTEM_TRACE_AUDIT_NUMBER, ISOData, \
	  AuthRecord->system_trace_audit_number );

	/* EXPIRY DATE */
	GetFieldVisa ( DATE_EXPIRATION, ISOData, AuthRecord->date_expiration );

	/* MERCHANT TYPE */
	GetFieldVisa ( MERCHANT_TYPE, ISOData, AuthRecord->merchant_type );

	/* ACQUIRING INSTITUTION COUNTRY CODE */
	GetFieldVisa ( ACQUIRING_INSTITUTION_COUNTRY_CODE, ISOData, \
	  AuthRecord->acquiring_institution_country_code );

	/* POS ENTRY MODE */
	GetFieldVisa ( POS_ENTRY_MODE_CODE, ISOData, \
	  AuthRecord->pos_entry_mode_code );
	 /* Added below by Amit as extra 0 coming from NPCI credi card issuance in DE22 */
	 memcpy(DE22,AuthRecord->pos_entry_mode_code+1,3);
	 memcpy(AuthRecord->pos_entry_mode_code,DE22,3);

	/* POS CONDITION CODE */
	GetFieldVisa ( POS_CONDITION_CODE, ISOData, AuthRecord->pos_condition_code);

	/* POS PIN CAPTURE CODE */
	GetFieldVisa ( POS_PIN_CAPTURE_CODE, ISOData, \
	  AuthRecord->pos_pin_capture_code );

	/* ACQUIRING INSTITUTION IDENTIFICATION CODE */
	GetFieldVisa ( ACQUIRING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->acquiring_institution_identification_code );

	/* FORWARDING INSTITUTION IDENTIFICATION CODE */
	GetFieldVisa ( FORWARDING_INSTITUTION_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->forwarding_institution_identification_code );

	/* TRACK2 DATA */
	GetFieldVisa ( TRACK2_DATA, ISOData, AuthRecord->track2_data );

	/* RRNO */
	GetFieldVisa ( RETRIEVAL_REFERENCE_NUMBER, ISOData, \
	  AuthRecord->retrieval_reference_number );

	/* AUTHORIZATION IDENTIFICATION RESPONSE */
	GetFieldVisa ( AUTHORIZATION_IDENTIFICATION_RESPONSE, ISOData, \
	  AuthRecord->authorization_identification_response );

	/* RESPONSE CODE */
	GetFieldVisa ( RESPONSE_CODE, ISOData, AuthRecord->response_code );

	/* TERMINAL ID */
	GetFieldVisa ( CARD_ACCEPTOR_TERMINAL_IDENTIFICATION, ISOData, \
	  AuthRecord->card_acceptor_terminal_identification );

	/* MERCHANT ID/CARD ACCEPTOR ID */
	GetFieldVisa ( CARD_ACCEPTOR_IDENTIFICATION_CODE, ISOData, \
	  AuthRecord->card_acceptor_identification_code );

	/* MERCHANT/CARD_ACCEPTOR LOCATION  */
	GetFieldVisa ( CARD_ACCEPTOR_NAME_LOCATION, ISOData, \
	  AuthRecord->card_acceptor_name_location );

	/* ADDITIONAL RESPONSE DATA */
	GetFieldVisa ( ADDITIONAL_RESPONSE_DATA, ISOData, \
	  AuthRecord->additional_response_data );

	/* TRACK1 DATA */
	GetFieldVisa ( TRACK1_DATA, ISOData, AuthRecord->track1_data );

	/* NATIONAL ADDITIONAL DATA  */
	GetFieldVisa ( ADDITIONAL_DATA_NATIONAL, ISOData, \
	  AuthRecord->additional_data_national );

	/* PRIVATE ADDITIONAL DATA */
	GetFieldVisa ( ADDITIONAL_DATA_PRIVATE, ISOData, \
	  AuthRecord->additional_data_private );

	/* TRANSACTION CURRENCY CODE */
	GetFieldVisa ( CURRENCY_CODE_TRANSACTION, ISOData, \
	  AuthRecord->currency_code_transaction );

	/* CARD HOLDER BILLING CURRENCY CODE */
	GetFieldVisa ( CURRENCY_CODE_CARDHOLDER_BILLING, ISOData, \
	  AuthRecord->currency_code_cardholder_billing );

	/* PIN DATA */
	GetFieldVisa ( PIN_DATA, ISOData, AuthRecord->pin_data );

	/* SECURITY RELATED CONTROL INFORMATION */
	GetFieldVisa ( SECURITY_RELATED_CONTROL_INFORMATION, ISOData, \
	  AuthRecord->security_related_control_information );

     /* Copy the data coming in Field 57 to field 48 */
        /* Added by ACR 15-03-2011 */
        GetFieldVisa ( RESERVERD_NATIONAL_57, ISOData, \
          AuthRecord->additional_data_private );

#ifdef SHELL_RELOAD_VOID_IS
        GetFieldVisa ( NATIONAL_POS_GEOGRAPHIC_DATA, ISOData, \
          AuthRecord->national_pos_geographic_data  );
#endif

	/* POS ENTRY CAPABILITY CODE */
	GetFieldVisa ( POS_ENTRY_CAPABILITY_CODE, ISOData, \
	  AuthRecord->pos_entry_capability_code );

	/* DATE OF ACTION */
	GetFieldVisa ( DATE_ACTION, ISOData, AuthRecord->date_action );

	/* ORIGINAL DATA ELEMENTS */
	GetFieldVisa ( ORIGINAL_DATA_ELEMENTS, ISOData, \
	  AuthRecord->original_data_elements );

	/* FILE UPDATE CODE */
	GetFieldVisa ( FILE_UPDATE_CODE, ISOData, AuthRecord-> file_update_code );

	/* FILE NAME  */
	GetFieldVisa ( FILE_NAME, ISOData, AuthRecord->file_name );

	/* ACCOUNT IDENTIFICATION 1 */
	GetFieldVisa ( ACCOUNT_IDENTIFICATION_1, ISOData, \
	  AuthRecord->account_identification_1);

	/* ACCOUNT IDENTIFICATION 2 */
	GetFieldVisa ( ACCOUNT_IDENTIFICATION_2, ISOData, \
	  AuthRecord->account_identification_2);

	/* REMAINING OPEN TO USE AMOUNT */
	GetFieldVisa ( REMAINING_OPEN_TO_USE, ISOData, \
	  AuthRecord->remaining_open_to_use );

	/* FILE RECORD ACTION AND DATA */
	GetFieldVisa ( FILE_RECORD_ACTION_AND_DATA, ISOData, \
	  AuthRecord->file_record_action_and_data );

	/* DE 61 RUPAY ISSUANCE */
	GetFieldVisa ( OTHER_AMOUNTS, ISOData, DE61 );
	memcpy ( AuthRecord->other_amounts, DE61, 36 );

	/* REPLACEMENT AMOUNTS */
	GetFieldVisa ( REPLACEMENT_AMOUNTS, ISOData, \
	  AuthRecord->replacement_amounts );

	/* ORIGINAL MESSAGE TYPE IDENTIFIER */
	GetFieldVisa ( ORIGINAL_MESSAGE_TYPE_IDENTIFIER, ISOData, \
	  AuthRecord->original_message_type_identifier );

	/* DEBIT PRIVATE USE FOR BPL (FIELD 63 ) */
	if ( IsPresent ( DEBIT_PRIVATE_USE ) == SUCCESS )
	{
		/*
		GetETOP ( DEBIT_PRIVATE_USE, ISOData, \
		  AuthRecord->remaining_open_to_use, 12 );
		*/
		GetFieldVisa( DEBIT_PRIVATE_USE, ISOData, field_63 );
		analyse_63(field_63) ;
		memset( network_id, 0x00, 5 );
		memset( msg_reason_code, 0x00, 5 );
		memset( stip_reason_code, 0x00, 5 );
		/*
		GetSubField63( 1,field_63, network_id ); 
		GetSubField63( 3,field_63, msg_reason_code );
		GetSubField63( 4,field_63, stip_reason_code );
		*/
		GetSubField63( 0,field_63, network_id ); 
		GetSubField63( 2,field_63, msg_reason_code );
		GetSubField63( 3,field_63, stip_reason_code );

		memcpy(AuthRecord->remaining_open_to_use, network_id,4 );
		memcpy(AuthRecord->remaining_open_to_use+4, msg_reason_code,4 );
		memcpy(AuthRecord->remaining_open_to_use+8, stip_reason_code,4);
	}

	/* RESERVED PRIVATE */
	if ( IsPresent ( RESERVED_PRIVATE_126) == SUCCESS )
	{
		GetFieldVisa ( RESERVED_PRIVATE_126, ISOData, STRING_FIELD );
		/* ACR/Michel, analyse_126 function should be called before
		any of the sub fields are extracted Date: 21-02-2008 */
		analyse_126( STRING_FIELD );		/* Added by ACR/Michel 06/03/2008 */
		GetSubField126( 9, STRING_FIELD, AuthRecord->reserved_private_126 );

    /*    f_printf(stderr, "reserved_private_126 [%s] \n", AuthRecord->reserved_private_126 );  MCI-PCI CR-MXP-0508-0557 240106*/
	}

	return ( SUCCESS );
}
#endif

