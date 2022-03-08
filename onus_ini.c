
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_ini.c                                          *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 07-Mar-2000                                         *
 * Description         : ONUS initialization program                         *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   18-Feb-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 2                                                                         *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

 #include <stdio.h>
 #include <unistd.h>

 #include <signal.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <memory.h>
 #include <errno.h>
 #include <fcntl.h>
 #include <isam.h>
 #include <paczpa03.h>
 #include <paczpa56.h>
 #include <paczpa05.h>
 #include <paczpa11.h>
 #include <paczpoau.h>
 #include <paczpa0v.h>
 #include <paczau00.h>
 #include <paczeuro.h>
 #include <paczauto.h>
 #include <pcrzbale.h>
 #include <rthauth.h>
 #include <define.h>
 #include <onus.h>
 #include <constantes.h>
 #include <bal.h>
 #include <msg_type.h>
 #include <struct_iso.h>
 #include <scenario.h>
 #include <pacdstcm.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <paczmalf.h>
 #include <MalFuncError.h>

 #include "onus_ini.h"


#define BFL_REMI_TO_PL
#ifdef BFL_REMI_TO_PL

        extern int lnprod ;
        extern int lnrules ;
     /*   extern int lnamnt ;*/

#endif

 
/* Added for JK CMS...... Bharath */
  #define JK_CMS_APPLY 1    

 #define ONEVIEW_EMAIL  1
 #define  BFL_CUG_CARD     1

/*  #define QR_ISSUANCE  1 */


extern int		bal1;		/* MsgQID1 */
extern int		bal2;		/* MsgQID2 */
extern int 	PIDROUT;		/* DISPATCH PID */

extern char		flag_trace;	/* Trace flag */

/* ISAM Data File Descriptors */
extern int		fdparam;	/* Parameter file */
extern int		fdtrans;	/* Transaction file */
extern int		fdresp;		/* Response Code mapping file */
extern int      fdsuprs;    /* Subresult suppressing file */
extern int    	fdathtrc;   /* Authorisation trace file */
extern char CODE_BANQUE[6];
#ifdef BFL_CUG_CARD
extern int lnbcli ; /* BFL CARD Client file */
extern int lnbcom ; /* BFL CARD ACCOUNT MASTER */
extern int lnporte; /*BFL  CARD MASTER */
extern int lnsauto; /*BFL  AUTHORISATION */
extern int lnhauto; /*BFL  AUTHORISATION HISTORY */
extern int lnparam ;/*BFL  PARAMETERS*/
extern int lnrth;   /*BFL  RTH*/
extern int lnmaster;
extern int lncrd;
#endif

/* Function Prototypes */
extern char* getenv ( char* );

static int GetParameters ( void );

#ifdef JK_CMS_APPLY
 #include <onuspr05.h>
extern int fdonusparam;
t_onuspr05 onuspr05;
#endif

void OpenDataFiles ( void )
{
	char	*DataFilePath = NULL;
	char	file[64];

	/* V5-Migration Added by ACR to remove purify error, 14-09-05 */
	memset( file, 0x00, 64 );

	/* Get the MultiXPAC data files path */
	DataFilePath = getenv ( "MultipacDat" );
	if ( DataFilePath == NULL )
	{
		f_printf(stderr, "ONUS: ERROR getting Data files path\n");
		exit ( 1 );
	}

	/* Open the parameter file */
	sprintf ( file, "%s/FC-PARAM", DataFilePath );
	fdparam = isopen ( file, ISAUTOLOCK+ISINOUT);
	if ( fdparam < 0 )
	{
		f_printf(stderr, "ONUS: ERROR OPEN FC-PARAM: %d \n", iserrno );
		exit ( 1 );
	}

	/* V5-Migration Added by ACR to remove purify error, 14-09-05 */
     	#ifdef BFL_CUG_CARD
     	if(memcmp(CODE_BANQUE,"00059",5) != 0)
     	{
     	#endif

	memset( file, 0x00, 64 );
      #ifdef JK_CMS_APPLY
	/* Open the onus parameter file */
	sprintf ( file, "%s/ONUSPARAM", DataFilePath );
	fdonusparam = isopen ( file, ISAUTOLOCK+ISINOUT);
	if ( fdonusparam < 0 )
	{
		f_printf(stderr, "ONUS: ERROR OPEN ONUS-PARAM: %d \n", iserrno );
		exit ( 1 );
	}
	memset( file, 0x00, 64 );
      #endif



	/* V5-Migration Added by ACR to remove purify error, 14-09-05 */
	memset( file, 0x00, 64 );

	/* Open FC-RESP Subresult to actual code mapping file */
	sprintf ( file, "%s/FC-RESP", DataFilePath );
	fdresp = isopen ( file, ISMANULOCK+ISINPUT ); 
	if ( fdresp < 0 )
	{
		f_printf(stderr, "ONUS: ERROR OPEN FC-RESP: %d \n", iserrno );
		exit ( 1 );
	}

	/* Open FC-SUPPRS Subresult suppressing file */
	sprintf ( file, "%s/FC-SUPPRS", DataFilePath );
	fdsuprs = isopen ( file, ISMANULOCK+ISINPUT ); 
	if ( fdsuprs < 0 )
	{
		f_printf(stderr, "ONUS: ERROR OPEN FC-SUPPRS: %d \n", iserrno );
		exit ( 1 );
	}


	/* Open ATH-MTRC authorization movement trace file */
	sprintf ( file, "%s/ATH-MTRC", DataFilePath );
	fdathtrc = isopen ( file, ISAUTOLOCK+ISINOUT ); 
	if ( fdathtrc < 0 )
	{
		f_printf(stderr, "ONUS: ERROR OPEN ATH-MTRC: %d \n", iserrno );
		exit ( 1 );
	}


     #ifdef BFL_CUG_CARD
     }
     else
     {
     #endif


       #ifdef BFL_CUG_CARD
       memset(file,0x00,64);
       sprintf(file,"%s/LN-BCLI",DataFilePath);
       /*lnbcli = isopen(file,ISINOUT+ISAUTOLOCK);*/
       lnbcli = isopen( file, ISINOUT + ISMANULOCK);
       if(lnbcli < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-BCLI %d \n",iserrno);
                exit(1);
       }

       memset(file,0x00,64);
       sprintf(file,"%s/LN-BCOM",DataFilePath);
       lnbcom = isopen(file,ISINOUT+ISAUTOLOCK);
       if(lnbcom < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-BCOM %d \n",iserrno);
                exit(1);
       }
        
       memset(file,0x00,64);
       sprintf(file,"%s/LN-PORTE",DataFilePath);
       lnporte = isopen(file,ISINOUT+ISMANULOCK);
       if(lnporte < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-PORTE %d \n",iserrno);
                exit(1);
       }
	f_printf(stderr, "LNPORTE ISOPEN: %d\n", lnporte);
        
       memset(file,0x00,64);
       sprintf(file,"%s/LN-SAUTO",DataFilePath);
       lnsauto = isopen(file,ISINOUT+ISAUTOLOCK);
       if(lnsauto < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-SAUTO %d \n",iserrno);
                exit(1);
       }

       memset(file,0x00,64);
       sprintf(file,"%s/LN-AUTP",DataFilePath);
       lnhauto = isopen(file,ISINOUT+ISAUTOLOCK);
       if(lnhauto < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-AUTP  %d \n",iserrno);
                exit(1);
       }

       memset(file,0x00,64);
       sprintf(file,"%s/LN-PARAM",DataFilePath);
       lnparam = isopen(file,ISINOUT+ISMANULOCK);
       if(lnparam < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-PARAM %d \n",iserrno);
                exit(1);
       }

       memset(file,0x00,64);
       sprintf(file,"%s/LN-RTHAUTH",DataFilePath);
       lnrth = isopen(file,ISINOUT+ISAUTOLOCK);
       if(lnrth < 0 )
       {

                f_printf(stderr,"ONUS: ERROR OPEN  LN-RTHAUTH  %d \n",iserrno);
                exit(1);
       }
       #ifdef NEVER
       memset(file,0x00,64);
       sprintf(file,"%s/LN-VELLOC",DataFilePath);
       fdlnvel = isopen(file,ISINOUT+ISAUTOLOCK);
       if(fdlnvel < 0 )
       {
               f_printf(stderr,"Onus:ERROR OPENING LN-VELLOC %d \n" , iserrno);
               exit(1);
       }
       #endif
       #endif

#ifdef BFL_REMI_TO_PL
        memset(file,0x00,64);
        sprintf(file,"%s/LN-REMI-PL-RULES",DataFilePath);
        lnrules = isopen(file,ISINOUT+ISMANULOCK);
        if(lnrules < 0 )
        {
                f_printf(stderr,"Onus:ERROR OPENING [%s] %d \n" ,file ,iserrno);
                exit(1);
        }

        memset(file,0x00,64);
        sprintf(file,"%s/LN-PRODUCT-MASTER",DataFilePath);
        lnprod = isopen(file,ISINOUT+ISMANULOCK);
        if(lnprod < 0 )
        {
                f_printf(stderr,"Onus:ERROR OPENING [%s] %d \n" ,file ,iserrno);
                exit(1);
        }
/*
        memset(file,0x00,64);
        sprintf(file,"%s/LN-CUST",DataFilePath);
        lnamnt = isopen(file,ISINOUT+ISMANULOCK);
        if(lnamnt < 0 )
        {
                f_printf(stderr,"Onus:ERROR OPENING [%s] %d \n" ,file ,iserrno);
                exit(1);
        }
*/

	memset(file,0x00,64);
	 sprintf(file,"%s/LN-MASTER",DataFilePath);
	 lnmaster = isopen(file,ISINOUT+ISMANULOCK);
	 if(lnmaster < 0 )
	 {
		 f_printf(stderr,"Onus:ERROR OPENING [%s] %d \n" ,file ,iserrno);
		 exit(1);
	 }
#endif



#ifdef BFL_CUG_CARD
      }
#endif
        /* SMS Project Michel 31.07.2007 End   */

	return;
}

void CloseDataFiles ( void )
{

	isclose ( fdparam );
	isclose ( fdresp );
	isclose ( fdsuprs );
        #ifdef BFL_CUG_CARD
	isclose ( lnbcli );
	isclose ( lnbcom );
	f_printf(stderr, "LNPORTE ISCLOSE: %d\n", lnporte);
	isclose ( lnporte );
	isclose ( lnsauto );
	isclose ( lnhauto );
	isclose ( lnparam );
	isclose ( lnrth);
/*	isclose ( lnamnt );*/
	isclose ( lnmaster);
        isclose ( lncrd);
	#endif
	return;
}

void InitializeOnus ( void )
{
	int		result = 0;
	t_msg_bl	InterfaceMessage;

	/* Open the message queue 1 */
	if ( QOpen ( BAL1, &bal1 ) != SUCCESS )
	{
		f_printf(stderr, "ONUS : ERROR BAL1 QOpen(); errno == %d\n", errno);
		exit ( 0 );
	}

	/* Open the message queue 2 */
	if ( QOpen ( BAL2, &bal2 ) != SUCCESS )
	{
		f_printf(stderr, "ONUS : ERROR BAL2 QOpen(); errno == %d\n", errno);
		exit ( 0 );
	}

	/* Put the ONUS PID into the bal2 Queue */
	if ( QPutPid ( TYPE_ALGO, bal2, getpid () ) == -1 )
	{
		f_printf(stderr, "ONUS: ERROR while putting PID into Queue \n");
		exit ( 0 );
	}

	/* Get the DISPATCH PID from Queue */
	if ( ( PIDROUT = GetPidFromQueue ( TYPE_DISPATCH, bal2, !MXP_NOWAIT ) ) \
			== ECHEC )
	{
		f_printf(stderr, "ONUS: ERROR MSGRCV PIDROUT \n");
		exit ( 0 );
	}

	/* Send INIT message to DISPATCH */
	memset ( (char*)&InterfaceMessage, 0, sizeof ( t_msg_bl ) );
	InterfaceMessage.mtype = (long) PIDROUT;
	InterfaceMessage.mtexte.entete.source = getpid ();
	InterfaceMessage.mtexte.entete.type = MSG_INIT;
	InterfaceMessage.mtexte.entete.lng = TYPE_ALGO;
        memcpy(InterfaceMessage.mtexte.entete.banque, CODE_BANQUE, 5 );

	result = SendMsgToInterf ( &InterfaceMessage, bal1 );
	if ( result < 0 )
	{
		f_printf(stderr, "ONUS: ERROR MSG INIT INITIALIZATION \n");
		exit ( 0 );
	}

	/* Send the SIGN ON message to DISPATCH */
	memset ( (char*)&InterfaceMessage, 0, sizeof ( t_msg_bl ) );
	InterfaceMessage.mtype = (long) PIDROUT;
	InterfaceMessage.mtexte.entete.source = getpid ();
	InterfaceMessage.mtexte.entete.type = MSG_SIGNED_ON;
	InterfaceMessage.mtexte.entete.lng = TYPE_ALGO;
        memcpy(InterfaceMessage.mtexte.entete.banque, CODE_BANQUE, 5 );

	result = SendMsgToInterf ( &InterfaceMessage, bal1 );
	if ( result < 0 )
	{
		f_printf(stderr, "ONUS: ERROR SIGN ON INITIALIZATION \n");
		exit ( 0 );
	}

	/* Get the ONUS parameters */
	result = GetParameters () ;
	if ( result != SUCCESS )
	{
		f_printf(stderr, "ONUS: ERROR GetParameters \n");
		exit ( 0 );
	}

	/* Initialize the field attributes table for VISA,MCI etc */
    /**
	InitTabEuro ();
    ********/
	InitConvert ();
	InitTabVisa ();
	InitTabMci ();
	InitTabAegn ();
	return;
}

static int GetParameters ( void )
{
	t_paczpa0v	paczpa0v;	/* Onus parameter structure */

	memset ( (char*)&paczpa0v, ' ', sizeof ( t_paczpa0v ) );
	memcpy ( paczpa0v.pa0v_notabl, "0V", 2 );
	memset ( paczpa0v.pa0v_suitecle, ' ', 30 );

	if ( isread ( fdparam, (char*)&paczpa0v, ISEQUAL ) != 0 )
	{
        f_printf(stderr, "ERROR : Reading Variable Factor (0V) \n");
		return ( ECHEC );
	}
	isunlock ( fdparam );
	isrelease ( fdparam );

	/* Get the trace flag value of ONUS */
	flag_trace = paczpa0v.pa0v_flag_trace_onus[0]; 

	return ( SUCCESS );
}

