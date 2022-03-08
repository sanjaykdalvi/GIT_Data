/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus.c                                              *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 14-Feb-2000                                         *
 * Description         : ONUS main program                                   *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   16-Feb-00 Gilu Alex            Bank wise product wise MCC wise        *
 *                                    1/4 day STIP is added                  *
 * 2   02-Mar-00 Gilu Alex            Aditional file is added to log the     *
 *                                    authorisation details ( RTH-AUTH)      *
 * 3   08-Mar-00 Sudheer K T          New screen is added to manage the      *
 *                                    ONUS STIP parameters                   *
 * 4   28-Mar-00 Sudheer K T          New screen is added to view records    *
 *                                    RTH-AUTH file                          *
 * 5   02-Apr-00 Gilu Alex            ONUS STIP is modified to handle 1 day  *
 *                                    /4 day/ Monthly /Multiple day STIP     *
 * 6   17-Mar-00 Gilu Alex            Modified to process AEGN authorisation *
 *                                    messages                               *
 * 7   16-Jun-00 Gilu Alex            Modified to process AEGN ATM           *
 *                                    financial transactions                 *
 * 8   16-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 *                                                                           *
 * 9                                                                         *
 *****************************************************************************/
/* Chowdesha added on 28-Jan-2013 for emv early issuance  */
 #define EMV_EARLY_ISSUANCE  1 

/* Chowdesha Added for CBA */
 #define CBA  1   

 /* Chowdesha/Preema added for IDBI BCHA EHSOT */
  #define  IDBI_BCHA_EHOST  1  
  #define  BOI_FRM          1
  #define  BFL_CUG_CARD     1
  #define  BOBL_PREPAID     1
 
 #include <stdio.h>
 #include <unistd.h>

#define BFL_TIMEOUT_ISSUE 1

#ifdef BFL_TIMEOUT_ISSUE
	int ibfl_txn_threshold;
#endif

 #include <signal.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <memory.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <isam.h>
 #include <paczpa03.h>
 #include <paczpa02.h>
 #include <paczpa05.h>
 #include <paczpa56.h>
 #include <paczpa11.h>
 #include <paczpoau.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczeuro.h>
 #include <paczauto.h>
 #include <paczisoauto.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <ath-trc.h>
 #include <define.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <scenario.h>
 #include <MalFuncError.h>
 #include <onus.h>     /* Added by Pradnya */
 #ifdef BFL_CUG_CARD
 #include <bflzbcom.h>
 #include <bflzport.h>
 #include <bflzbcli.h>
 #include <bflzparm.h>
 #include <bfl_rthauth.h>
 #endif
#ifdef EMV_EARLY_ISSUANCE
	#include <emvparameter.h>
#endif



/* LOG_ROTATION */
#define DTLFILESIZE 1000000000
static int count_onus_req=0;
char onus_trace_name[1024];



#define RULE_ENGINE 1

#ifdef RULE_ENGINE

	#include "ruleEngineMacros.h"
	#include "readAndPopulateRule.h"
	#include "ruleFldDtls.h"
	#include "rule.h"

	/* Changes for compilation error Starts Here */
	int  fdcoau;
	int  FictiMerchant;
	int  print_debug;
	void PackNum(){};
	void UnpackNum(){};
	/* Changes for compilation error Ends Here */

	int  noOfFldMapped =  0;
	int  configFldCnt  = -1;
	int  readDatStatus = -2;
	int  authRuleCount = -2;
	int  cibilRuleCount = -2;
	int  ltvRuleCount = -2;
	int  velocityRuleCount = -2;

	extern t_ruleTypeRuleMaster  * authRuleMaster[];
	extern t_ruleTypeRuleMaster  * cibilRuleMaster[];
	extern t_ruleTypeRuleMaster  * ltvRuleMaster[];
	extern t_ruleTypeRuleMaster  * velocityRuleMaster[];

	extern t_fldDtls ruleFldDtls[]; /* ARRAY OF FIELD DETAILS WHICH IS USED IN RULE ENGINE FOR AUTH */

	extern int readMagnusFieldMappingFile( t_fldDtls fldDtls[], char * fldConfigFile );
	extern int readAndPopulateRule( char *ruleType, t_ruleTypeRuleMaster  *authRuleMaster[] );

#endif



#define BFL_REMI_TO_PL

#ifdef BFL_REMI_TO_PL
        #include <remi_pl_rules.h>
        #include <loan_product_master.h>
        #include <cust_prod_amount.h>

        t_remi_pl_rules remi_pl_rules ;
        t_product_master product_master ;
        t_cust_prod_amount cust_prod_amount;

	/* MRL_332 */
	int             pos_sms_applicable=-1;
        int             lnrules = -1;/*BFL PARAMETERS*/
        int             lnprod =  -1;/*BFL PARAMETERS*/
        int             lnamnt =  -1;/*BFL PARAMETERS*/

        int             remi_to_pl_sms = -1;
        char            remi_to_pl_breach_amount[12+1];
#endif



/* Chowdesha added on 19-March-2010  for CMS IGL */

 #define CMS_EHOST   1     



int				bal1;				/* MsgQID1 */
int				bal2;				/* MsgQID2 */
int 			PIDROUT;			/* DISPATCH PID */
char			flag_trace = 'Y';	/* trace flag */

char			ServiceCode[3+1];
char			pvv[5+1];
char			cvv[3+1];
char			cvc2[3+1];
char			PinFormat[3];
char			EffectiveDate[5];
char			CardSecurity[6];
char			LanguageCode[4];
char			User[7];

char			CODE_BANQUE[5+1]; /* Used in fprintf function */

char			LocalAmount[13];

#ifdef IDBI_BCHA_EHOST
 char Cbs_auth_code[8]; 
#endif 
#ifdef CBA
char CBS_LOCAL_AMT [15]; 
char CBS_CURR_CODE[5]; 
#endif 
t_rthauth		rthauth;		/* RTH AUTH RECORD */
t_paczpa05		paczpa05;		/* CARD ISSUER DETAILS */
t_pcrzbale		pcrzbale;		/* ACCOUNT DETAILS */
t_paczpa02		paczpa02;		/* BANK DETAILS */
t_paczisoauto	paczisoauto;	/* TRANSACTION PACKET DETAILS */
t_paczpoau		paczpoau;		/* CARD HOLDER DETAILS */
t_ath_trc		athtrc;			/* Authorization trace Details */
t_paczpoau              dstpaczpoau;    /* Destination card details in card to card transfer */
#ifdef BFL_CUG_CARD
t_bflzport    bflzport;
t_bflzbcom    bflzbcom;
t_bflzbcli    bflzbcli;
t_bflrthauth  bflrthauth;
#endif
#ifdef EMV_EARLY_ISSUANCE
	t_emvparameter emvparameter; 
#endif

  int  RecuSIGALRM = 0;

/* ISAM Data File Descriptors */
int		fdparam = -1;	/* Parameter file */
int		fdoppos = -1;	/* Negative File */
int		fdseuro = -1;	/* EURO Authorization movement file */
int		fdtrans = -1;	/* Transaction file */
int		fdresp = -1;		/* Response Code mapping file */
int     fdsuprs = -1;    /* Subresult suppressing file */
int    	fdathtrc = -1;   /* Authorisation trace file */
#ifdef BFL_CUG_CARD
int 		lnbcli  = -1; /* BFL CARD Client file */
int 		lnbcom  = -1; /* BFL CARD ACCOUNT MASTER */
int 		lnporte = -1;/*BFL CARD MASTER */
int 		lnsauto = -1;/*BFL AUTHORISATION */
int 		lnhauto = -1;/*BFL AUTHORISATION HISTORY */
int 		lnparam = -1;/*BFL PARAMETERS*/
int 		lnrth   = -1;/*BFL RTH*/
int 		lnmaster = -1; /*BFL Scheme_CRPR_Risk data*/
#endif

/* SCENARIO CODES */
char	CODE_SCENARIO_I;
char	CODE_SCENARIO_A;
char	CODE_SCENARIO_O;
char	CODE_SAUTO_HAUTO = 'S';

int             snFlagAlarm = 0 ;

/* function prototypes */
extern void FPRINTF ( FILE* stream, char* fmt, ... );
extern void OpenDataFiles ( void );
extern void CloseDataFiles ( void );
extern int	LoadIssuerTable ( void );
extern int 	LoadBankDetails ( void );
extern void FreeIssuerTable ( void );
extern void FreeBankTable ( void );
extern void InitializeOnus ( void );
extern int ReceiveMsgFromQueue ( t_msg_bl* );

extern void Request_0100 ( t_struct_iso* );
extern void Request_0200 ( t_struct_iso* );
extern void Request_0120 ( t_struct_iso* );
extern void Request_0400 ( t_struct_iso* Request);
extern void Request_0420 ( t_struct_iso* Request);
extern char* GetDateYYMMDD (void);
extern char* GetTime (void);

static void OpenTraceFile ( void );
static void RefreshParameters ( void );
static void HandleRequest ( t_msg_bl* );
void SIG2Handler ( int signo );
void SpawnChild ( t_msg_bl* ReqMsg );
void exit_routine ( void );


#ifdef IDBI_BCHA_EHOST 
void  traitSIGALRM( int  X )
{
  signal( SIGALRM, traitSIGALRM );
  FPRINTF(stderr,"RECEIVED SIGNAL SIGALRM\n");
  ++RecuSIGALRM;
  return;
}
#endif 


FILE *pFile2;

void print_trace ( t_msg_bl* ReqMsg)
{
        t_paczisoauto u_paczisoauto;
        t_struct_iso u_Request = ReqMsg->mtexte;
        t_entete_iso u_ISOHeader = u_Request.entete;
        char* ISOData;
        char MessageFormat;
	char field_59data[200] = {'\0'};
        char field_57data[200] = {'\0'};
	char field_63data[200] = {'\0'};
	ISOData = u_Request.data;
        MessageFormat = u_ISOHeader.systeme;

	
	fprintf(pFile2,"|%-6s|",GetDateYYMMDD());
	fprintf(pFile2,"%-6s|",GetTime());

        FillAuto ( ISOData, MessageFormat, &u_paczisoauto );


	fprintf(pFile2,"%-4d|",ReqMsg->mtexte.entete.type);
        fprintf(pFile2, "%-19s|", u_paczisoauto.primary_account_number);
	fprintf(pFile2, "%-6s|", u_paczisoauto.processing_code);
	
	fprintf(pFile2, "%-12s|", u_paczisoauto.amount_transaction);
	fprintf(pFile2, "%-6s|", u_paczisoauto.system_trace_audit_number);
	fprintf(pFile2, "%-4s|", u_paczisoauto.date_expiration);
	fprintf(pFile2, "%-4s|", u_paczisoauto.pos_entry_mode_code );
	fprintf(pFile2, "%-2s|", u_paczisoauto.pos_condition_code);
	fprintf(pFile2, "%-12s|", u_paczisoauto.retrieval_reference_number);
	fprintf(pFile2, "%-6s|", u_paczisoauto.authorization_identification_response);
	fprintf(pFile2, "%-3s|", u_paczisoauto.response_code);
	fprintf(pFile2, "%-8s|", u_paczisoauto.card_acceptor_terminal_identification);
	fprintf(pFile2, "%-15s|", u_paczisoauto.card_acceptor_identification_code);
	fprintf(pFile2, "00059|");	
	fprintf(pFile2, "%-2s|", u_paczisoauto.pos_entry_capability_code);
	GetFieldVisa (56, ISOData,    field_57data );
	fprintf(pFile2, "%-200s|", field_57data );
	GetFieldVisa ( 58, ISOData,    field_59data );
	fprintf(pFile2, "%s|\n", field_59data );
	GetFieldVisa ( 62, ISOData,    field_63data );
	fprintf(pFile2, "%s|\n", field_63data );
	
	fflush(pFile2);

 } 


/* MAIN */
main ( argc, argv )
int     argc;   
char *  argv[]; 
{

	t_msg_bl	RequestMsg;	
	int 		TypeOfMessage = 0;
  int      iFlag        = 0;
	int count_char =0;
  
        memset(CODE_BANQUE, '\0', 6);    
        memcpy(CODE_BANQUE, argv[1], 5); 

	/* Register the signal SIGUSR2 */
	signal ( SIGUSR2, SIG2Handler );

	/* Ignore the SIGCHLD signal */
	signal ( SIGCHLD, SIG_IGN );



    #ifdef IDBI_BCHA_EHOST 
		if( (memcmp(CODE_BANQUE, "00011", 5) == 0) 
	       #ifdef CBA 
               ||(memcmp(CODE_BANQUE, "00054", 5) == 0)
	       #endif 
               #ifdef BOI_FRM 
               ||(memcmp(CODE_BANQUE, "00006", 5) == 0)
	       #endif 
          )
		{
				signal( SIGALRM, traitSIGALRM );
		}
                else
                signal ( SIGALRM, SIG_IGN );/* Ignore the SIGALRM  signal */

	#endif 
	/* open the trace file */
	OpenTraceFile ();

	/* Open ISAM Data files */
	OpenDataFiles ();  /* onus_ini.c */

	/* Read INI file*/
	ReadIniFile( &count_char );

	/* Load the Issuer Table */
	LoadIssuerTable ();      /* onus_load.c  */

	/* Load the Bank details Table */
	LoadBankDetails ();      /* onus_load.c */

	/* Initialize the ONUS process */
	InitializeOnus ();      /* onus_ini.c */
  
#ifndef ONEVIEW_ORACLE_DIRECT_UPDATE
	dbConnect(&iFlag); /*dbConnect opens file to write the records instead of Oracle OneView update*/	
    if(iFlag == 1){
	       f_printf(stderr,"\n FileOpen Error ");	      
	}
#endif

        if(memcmp(CODE_BANQUE, "00059", 5) == 0)
        {
		/*system("loan_velloc_recreation  $HOME");*/
		system("migrate_remi_pl_rules");
        }


#ifdef RULE_ENGINE

	if ( 0 == memcmp( CODE_BANQUE, "00059", 5 ) )
	{
		char fldConfigFile[1024];
		char scriptname[1024];

		memset(fldConfigFile,'\0', sizeof(fldConfigFile));
		memset(scriptname,'\0', sizeof(scriptname));

		sprintf( scriptname, "%s/%s", getenv("MultipacShl"), "fetchRuleConfDtls.sh");

		/* Calling the script here to fetch the data from 1-View */
		system(scriptname);

		sprintf( fldConfigFile, "%s/%s", getenv("MultipacDat"), "LN-RULEFIELDMAPPING.txt");

		/* FOR AUTH RULE */
		/* TODO */
		configFldCnt = readMagnusFieldMappingFile(ruleFldDtls, fldConfigFile); 
		/* READ DATA FROM MAPPING FILE TO MAP MAGNUS FIELD AND POPULATE MAPPING ARRAY */

		if( configFldCnt <= 0 )
		{
			f_printf(stderr,"\n ERROR: Error in readMagnusFieldMappingFile"
				" while reading confg file\n");
		}

		if( configFldCnt > 0 )
		{
			authRuleCount = readAndPopulateRule("AU", authRuleMaster );
			if( authRuleCount < 0 )
			{
				f_printf(stderr, "\n readAndPopulateRule failed to read dat file...:[%d]\n", 
					readDatStatus );
			}
			else
				f_printf(stderr, "\n Rule Count For AUTH:[%d]\n", authRuleCount );

			cibilRuleCount = readAndPopulateRule("CB", cibilRuleMaster );
			if( cibilRuleCount < 0 )
			{
				f_printf(stderr, "\n readAndPopulateRule failed to read dat file...:[%d]\n",
					readDatStatus );
			}
			else
				f_printf(stderr, "\n Rule Count For CIBIL:[%d]\n", cibilRuleCount );

			ltvRuleCount = readAndPopulateRule("LT", ltvRuleMaster );
			if( ltvRuleCount < 0 )
			{
				f_printf(stderr, "\n readAndPopulateRule failed to read dat file...:[%d]\n",
					readDatStatus );
			}
			else
			f_printf(stderr, "\n Rule Count For LTV:[%d]\n", ltvRuleCount );

			velocityRuleCount = readAndPopulateRule("VL", velocityRuleMaster );
			if( velocityRuleCount < 0 )
			{
				f_printf(stderr, "\n readAndPopulateRule failed to read dat file...:[%d]\n",
					 readDatStatus );
			}
			else
			f_printf(stderr, "\n Rule Count For VELOCITY:[%d]\n", velocityRuleCount );
		}
	}
        #endif




#ifdef BFL_TIMEOUT_ISSUE
	char filename[1024];
	char bfl_txn_threshold[1024];
	int  thres_len;
	FILE *fp_txn_ini;
	ibfl_txn_threshold = 0;

	sprintf(filename, "%s/loan_txn_threshold.ini", getenv("MultipacDat"));
	if((fp_txn_ini = fopen(filename, "r"))==NULL)
	{
		f_printf(stderr, "Error in opening [%s]. Error: %d\n", filename, errno);
	}
	else
	{

		memset(bfl_txn_threshold, 0x00, sizeof(bfl_txn_threshold));
		fgets(bfl_txn_threshold, sizeof(bfl_txn_threshold), fp_txn_ini);
		thres_len = strlen(bfl_txn_threshold);
		if(bfl_txn_threshold[thres_len-1]=='\n') bfl_txn_threshold[thres_len-1]='\0';
		ibfl_txn_threshold = atoi(bfl_txn_threshold);

		f_printf(stderr, "Transaction Timeout Parameter: [%s]/[%d]\n", 
			bfl_txn_threshold, ibfl_txn_threshold);

		fclose(fp_txn_ini);
	}
	
#endif

	/* Receive the request messages from DISPATCH and process */
	while ( 1 )
	{

		/* Initialize SCENARIO CODE */
		CODE_SCENARIO_I = 'x';	
		CODE_SCENARIO_O = SCENARIO_REFUS;
		CODE_SCENARIO_A = SCENARIO_REFUS;

		memset ( (char*)&RequestMsg, 0, sizeof ( t_msg_bl ) );
		TypeOfMessage = ReceiveMsgFromQueue ( &RequestMsg );  /* onus_utl.c */
		switch ( TypeOfMessage )
		{

		/* Reinitialising ONUS process */
		case MSG_REFRESH:
			f_printf(stderr, "Parameters refreshed at %.6s \n", GetTime () );	
			RefreshParameters ();
			break;

		case	-1:	/* ERROR */
			f_printf(stderr, "ERROR : Receiving Message from Queue\n");
			break;

		default:	/* Spawn child process to handle request */
			count_onus_req++;
			if(count_onus_req%1000 == 0)
			{
				checkDTLFILE();
			}
			SpawnChild ( &RequestMsg );

		} /* SWITCH */

	} /* WHILE */

}

int checkDTLFILE()
{
	int size;
	struct stat st;
	stat(onus_trace_name, &st);
	size = st.st_size;

	f_printf (stderr,"onus trace size: %d bytes\n", size);

	if( size < DTLFILESIZE )
	{
		f_printf (stderr," FILE IS SMALL [%d]", st.st_size);
		return (SUCCES);
	}
	else
	{
		fclose(stderr);
		memset(onus_trace_name,' ',sizeof(onus_trace_name));
		sprintf ( onus_trace_name, "%s/ONUS_%s.%s.%.6s.%.6s", getenv ( "MultipacTrc" ), \
			getenv ( "BAL_REF" ), CODE_BANQUE, GetTime(), GetDateYYMMDD () );
		freopen(onus_trace_name,"a+",stderr);
		setbuf(stderr,NULL);
		return (SUCCES);
	}
}

void SpawnChild ( t_msg_bl* ReqMsg )
{
	int ChildPID = 0;


   #ifdef IDBI_BCHA_EHOST
	  /* create child process */
   	  ChildPID = fork ();  
	
	switch ( ChildPID )
	{
	case	0:	
                  if(memcmp(ReqMsg->mtexte.entete.banque, "00059", 5) != 0)
                  {
                     CloseDataFiles ();
                     iscleanup ();
                     exit ( 0 );
                     break;
                  }
		f_printf_Begin();
		f_printf(stderr, "Child Created with PID: %d  TIME[%.6s]\n"
			,getpid(), (char *)GetTime());

		print_trace(ReqMsg);

		HandleRequest ( ReqMsg );

		print_trace(ReqMsg);

                f_printf(stderr, "Stop Child Process Bye !!!!!!!!!!!!!PID: %d  TIME[%.6s]\n"
			,getpid(), (char *)GetTime());
		f_printf_end();

		exit ( 0 );
		break;

	case 	-1:
		f_printf(stderr, "Error creating Child Process, error= %d\n", errno );
		break;

	default:
               /* sleep(1);*/
		usleep(50000);
                f_printf(stderr, "Reading Next Request from Queue....\n" );
		return;
	}   
   #endif  
	return;
}

#ifdef SPAWN_OLD
void SpawnChild ( t_msg_bl* ReqMsg )
{
	int ChildPID = 0;

	/* create child process */
   	/*ChildPID = fork (); */
     HandleRequest ( ReqMsg );

  /************
	switch ( ChildPID )
	{
	case	0:	

		f_printf(stderr, "Child Created with PID: %d \n", getpid() );
		HandleRequest ( ReqMsg );
		
        f_printf(stderr, "Stop Child Process Bye !!!!!!!!!!!!! \n" );
		CloseDataFiles (); 
		iscleanup ();
        f_printf(stderr, "Reading Next Request from Queue....\n" );
		exit ( 0 );
		break;

	case 	-1:
		f_printf(stderr, "Error creating Child Process, error= %d\n", errno );
		break;

	default:
		return;
	}   *************/

	return;
}
#endif 
static void HandleRequest ( t_msg_bl* ReqMsg )
{

	/* Initialize the RTH AUTH record */
	memset ( (char*)&rthauth, ' ', sizeof ( t_rthauth ) );
        #ifdef BFL_CUG_CARD
	memset ( (char*)&bflrthauth, ' ', sizeof ( t_bflrthauth ) );
        #endif
	memset ( (char*)&paczisoauto, 0, sizeof ( t_paczisoauto ) );
	memset ( (char*)&pcrzbale, ' ', sizeof ( t_pcrzbale ) );
	memset ( (char*)&paczpoau, ' ', sizeof ( t_paczpoau ) );

	switch ( ReqMsg->mtexte.entete.type )
	{

	/* Authorization request */
	case MSG_0100:
	case MSG_0101:
	case MSG_1100:
		f_printf(stderr, "\n Request_0100 \n" );
		Request_0100 ( &(ReqMsg->mtexte) );
		break;

	/* Reversal request */
	case MSG_0400:
	case MSG_0401:
	case MSG_1400:
	case MSG_0220:
		f_printf(stderr, "\n Request_0400 \n" );
		Request_0400 ( &(ReqMsg->mtexte) );
		break;


	default:
		f_printf(stderr, "WARNING : Message type [%d] not supported. \n", \
			ReqMsg->mtexte.entete.type );
	}
	return;
}

static void RefreshParameters ( void )
{
	/* Close all the data files  and reinitialize ONUS process */
	CloseDataFiles ();
	FreeIssuerTable ();
	FreeBankTable ();

	OpenDataFiles ();
	LoadIssuerTable ();
	LoadBankDetails ();
	InitializeOnus ();
	return;
}

/* SIGUSR2 interrupt handler  */
void SIG2Handler ( int signo )
{

	/* Ignore the SIGUSR2 signal */
	signal ( SIGUSR2, SIG_IGN );

	f_printf(stderr, "-----------------------------------------------\n");
	f_printf(stderr, "------ ONUS exit signal received --------------\n");
	f_printf(stderr, "-----------------------------------------------\n");

	/* call the exit function */
	 exit_routine (); 
	return;
}

void exit_routine ( void )
{
	t_msg_bl	send_message;
	int			result = 0;

	/* close all the ISAM data files */
	CloseDataFiles ();
	
	/* Free the memory of Issuer table */
	FreeIssuerTable ();

	/* Free the memory of Bank table */ 
	FreeBankTable ();

	/* Check the DISPATCH is running */
	do
	{
		result = kill ( PIDROUT, 0 );
	}
	while ( ( result != 0 ) && ( errno == EINTR ) );

	if ( result == 0 )
	{
		/* Send the exit message to DISPATCH */
		memset ( (char*)&send_message, 0, sizeof ( t_msg_bl ) );
		send_message.mtype = (long) PIDROUT;
		send_message.mtexte.entete.source = getpid();
		send_message.mtexte.entete.type = MSG_DESACTIF; 
		send_message.mtexte.entete.lng = TYPE_ALGO;

		result = SendMsgToInterf ( &send_message, bal1 );
		if ( result < 0 )
		{
			f_printf(stderr, "\nONUS: ERROR Sending message to DISPATCH\n");
		}
	}
	
	exit ( 0 );
}

static void OpenTraceFile ( void )
{
	char file[64];

	memset ( onus_trace_name, 0, 64 );
	sprintf ( onus_trace_name, "%s/ONUS_%s.%s.%.6s.%.6s", getenv ( "MultipacTrc" ),
		getenv ( "BAL_REF" ), CODE_BANQUE, GetTime(), GetDateYYMMDD () );
	freopen ( onus_trace_name, "a+", stderr );
	setbuf ( stderr, NULL );

	f_printf(stderr,"___________________________________________________\n");
	f_printf(stderr,"New Session at : %s %s\n",GetDateYYMMDD(),GetTime());   
	f_printf(stderr,"---------------------------------------------------\n");
	fflush(stderr);                                                   


	memset(file,' ',64);
	(void)sprintf(file,"%s/ONUS_TRC_00059.%s.%s",getenv("MultipacTrc"),getenv("BAL_REF"), GetDateYYMMDD());
	pFile2 = fopen ( file, "a+" );

	fprintf(pFile2,"-----------------LOAN_ISO_TRC FILE----------------------\n");
	fprintf(pFile2,"-------------------------------------------------------\n");
	(void)fprintf(pFile2,"New Session at : %s %s\n",GetDateYYMMDD(),GetTime());

	fprintf(pFile2,"-------------------------------------------------------------------------------------------------------------------\n");
	fprintf(pFile2,"|Date|Time|MTI| FLD2 | FLD3 | FLD4 | FLD11 | FLD14 | FLD22 | FLD25 | FLD37 | FLD38 | FLD39 | FLD41 | FLD42 | BankID | FLD60  | FLD57  | FLD59 | FLD63|\n ");
	fflush(pFile2);

	return;
}

char* GetInterfaceName ( void )
{
	return ( ONUS_INT );
}

int GetStcmCode ( void )
{
	return ( STCM_COD_ONUS );
}


