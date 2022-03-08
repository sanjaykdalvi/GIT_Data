
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : onus_stip.c                                         *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 12-Mar-2000                                         *
 * Description         : ONUS STIP functions                                 *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   16-Feb-00 Gilu Alex            Bank wise 1/4 day STIP added           *
 * 2   17-Mar-00 Gilu Alex            AMEX messages added                    *
 * 3   18-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 * 4   13-Dec-04 Veeresh E            Country wise Risk Parameter logic      * 
 *---------------------------------------------------------------------------*
 * 4   13-Dec-04 michel Alex     Reviewed-Country wise Risk Parameter logic  * 
 * 5   04-July-06 Sheela N A	BR-MXP-0606-0020, BR-MXP-0606-0019           *
 *****************************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <time.h>
 #include <isam.h>
 #include <define.h>
 #include <pcrzbale.h>
 #include <paczisoauto.h>
 #include <paczpoau.h>
 #include <paczpa05.h>
 #include <rthauth.h>
 #include <mccparam.h>
 #include <mccveloc.h>
 #include <onus.h>
 #include "onus_stip.h"
#include <bflvelocity_rrnmap.h>

/* Chowdesha added on 26-05-2011 for HSBC DOP Phase 2*/
/* #define HSBC_DOP_PHASE2  1  */

/* Added for limit velocity checking  ..*/
 #define SHELL_PHASE2   1      /* #define HSBC_DOP 1 */ 

/* Added to restrict internet trasaction for MCI Cards 06.11.12 */
    #define MCI_RISK004 1  
    #define BFL_CUG_CARD 1
    #define BFL_PHASE2   1


#ifdef SHELL_PHASE2 
  extern int  gUpdate_mcc_card_veloc;
#endif

/* Added to bypass MCC for Vijaya OTP verifier and PIN block cards */
#define VIJAYA_PHASE_3

 #define STIP_PARAM  1   
 #define EDEN_MCC_CHECK 1 

 #define CARD_PRODUCT_CODE 1
/* Chowdesha added on 21-10-2010 for VIJAYA BANK - VIP CARDS STIP LIMIT  */
  #define STIP_PARAM_VIP_LIMIT  1  

/*MRL_208*/
#include "bflvelloc.h"
#include "readAndPopulateRule.h"
extern t_ruleTypeRuleMaster  * velocityRuleMaster[];

/* #define HSBC_DOP   1 */
/* ISAM FILE DESCRIPTORS */
extern int 		fdparam;

extern char VL_Rule_Done [][7];
extern int  VL_Count;

extern t_pcrzbale		pcrzbale;
extern t_paczisoauto	paczisoauto;
extern t_paczpoau		paczpoau;
extern t_paczpa05		paczpa05;
extern t_rthauth		rthauth;

#if 0
/* MCC Risk management */
typedef struct risk_tag
{
	t_mccparam			mccparam;
	t_mccgroup			mccgroup;
	t_mccveloc			mccveloc;
	struct risk_tag*	link;
}mcc_risk_mgmt;

static mcc_risk_mgmt *HeadMccRisk = NULL;
#endif

/* card level MCC Risk management */
#ifdef SHELL_PHASE2
#include <cardmccparam.h>
	typedef struct card_risk_tag
	{
		t_cardmccparam cardmccparam;
		t_mccgroup		    	mccgroup;
		t_mccveloc			    mccveloc;
		struct card_risk_tag*	    link;
	}mcc_card_risk_mgmt;

	static mcc_card_risk_mgmt *HeadCardRisk = NULL;
#endif

/* Function Prototypes */
extern struct tm* localtime ( long* );
extern long atol ( char* );
extern int atoi ( char* );
extern double atof ( char* );
 
void UpdateRthAuthWithMCCParam ( t_mccparam* );
char* GetDateDec ( int Day );
char* GetDateInc ( int Day );

#ifdef MCI_RISK004
extern char User[];
#endif
#ifdef BFL_CUG_CARD
#include  <loanveloc.h>
#include  <BFLPROC.h>
#include  <bflzport.h>
#ifdef BFL_PHASE2
#include <bflzbcom.h>
extern t_bflzbcom    bflzbcom;
extern int Velocity_Check;
extern int  NXTALLD; 
#endif

extern t_procdata procdata;
extern t_bflzport bflzport;
int Validate_Velocity_Rules(char *);
int  ValidateLoanVelocLimit(char *,t_loanvelloc *,int * ,int *, int * );
int  UpdateLoanVelocLimit();
int  ReverseLoanVelocLimit(char * ,char *,char * );
#endif

#ifdef VIJAYA_PHASE_3
        extern int Ucaf ;
        #include <onuspr05.h>
        extern t_onuspr05 onuspr05;
        extern int fdonusparam;
#endif

#ifdef EDEN_MCC_CHECK
extern int WHITELIST_MATCHED;
int MCC_MATCHED = 0 ;
#endif

/* Day value Today = 0, yesterday = 1, day before yesterday = 2 etc.. */ 
char* GetDateDec ( int Day )
{
	long		d;
	struct tm 	*dat;
	/* Changed to static char by ACR, 13/10/2005,can't return local array */
	/*
	char		wdate[20];
	*/
	static char	wdate[20];

	/* Clean the buffer before using */
	memset( wdate, 0, 20 ); /* ACR, 13/10/2005 */

	time ( &d );
	d = d - ( ( Day ) * 24 * 60 * 60 );

	dat = localtime ( &d );

	sprintf ( wdate, "%02d%02d%02d", (dat->tm_year%100), ((dat->tm_mon)+1), \
	 (dat->tm_mday) );

	return ( wdate );
}

char* GetDateInc ( int Day )
{
        long            d;
        struct tm       *dat;
        /* Changed to static char by ACR, 13/10/2005,can't return local array */
        /*
        char            wdate[20];
        */
        static char     wdate[20];

        /* Clean the buffer before using */
        memset( wdate, 0, 20 ); /* ACR, 13/10/2005 */

        time ( &d );
        d = d + ( ( Day ) * 24 * 60 * 60 );

        dat = localtime ( &d );

                sprintf ( wdate, "%02d%02d%02d", (dat->tm_year%100), ((dat->tm_mon)+1), (dat->tm_mday) );

        return ( wdate );
}



void UpdateRthAuthWithMCCParam ( t_mccparam* mccparam )
{
	/* ONUS STIP PARAMS */
	memcpy(rthauth.mcc_card_type   , mccparam->card_type,4);
	memcpy(rthauth.mcc_group       , mccparam->mcc_group,4);
	memcpy(rthauth.one_day_check   , mccparam->one_day_check,1);
	memcpy(rthauth.one_day_count   , mccparam->one_day_count,3);
	memcpy(rthauth.one_day_amount  , mccparam->one_day_amount,12);
	memcpy(rthauth.four_day_check  , mccparam->four_day_check,1);
	memcpy(rthauth.four_day_count  , mccparam->four_day_count,3);
	memcpy(rthauth.four_day_amount , mccparam->four_day_amount,12);
	memcpy(rthauth.mnth_day_check  , mccparam->mnth_day_check,1);
	memcpy(rthauth.mnth_day_count  , mccparam->mnth_day_count,3);
	memcpy(rthauth.mnth_day_amount , mccparam->mnth_day_amount,12);
	memcpy(rthauth.multi_day_check , mccparam->multi_day_check,1);
	memcpy(rthauth.multi_no_of_days, mccparam->multi_no_of_days,2);
	memcpy(rthauth.multi_day_count , mccparam->multi_day_count,3);
	memcpy(rthauth.multi_day_amount, mccparam->multi_day_amount,12);
	return;
}


#ifdef STIP_PARAM_VIP_LIMIT
int iCheck_VIP()
{
	if ( pcrzbale.bale_vip[0] == '1' )
	{
		/* No Need to check Velocity for VIP accounts */
		return ( SUCCESS );
	}

	return ECHEC; 
}
#endif 


/* Chowdesha added for card level limit maintain for shell phase 2 */
#ifdef SHELL_PHASE2


void UpdateRthAuthWith_Cardlevel_MCCLimit( t_cardmccparam *cardmccparam)
{
    f_printf(stderr, "\n RTH Card level limit Update ..............");
	/* memcpy(rthauth.mcc_card_type   , cardmccparam->card_type,4); */
	memcpy(rthauth.mcc_group       , cardmccparam->mcc_group,4);
	memcpy(rthauth.one_day_check   , cardmccparam->one_day_card_check,1);
	memcpy(rthauth.one_day_count   , cardmccparam->one_day_card_count,3);
	memcpy(rthauth.one_day_amount  , cardmccparam->one_day_card_amount,12);
	memcpy(rthauth.four_day_check  , cardmccparam->four_day_card_check,1);
	memcpy(rthauth.four_day_count  , cardmccparam->four_day_card_count,3);
	memcpy(rthauth.four_day_amount , cardmccparam->four_day_card_amount,12);
	memcpy(rthauth.mnth_day_check  , cardmccparam->mnth_day_card_check,1);
	memcpy(rthauth.mnth_day_count  , cardmccparam->mnth_day_card_count,3);
	memcpy(rthauth.mnth_day_amount , cardmccparam->mnth_day_card_amount,12);
	memcpy(rthauth.multi_day_check , cardmccparam->multi_day_card_check,1);
	memcpy(rthauth.multi_no_of_days, cardmccparam->multi_card_no_of_days,2);
	memcpy(rthauth.multi_day_count , cardmccparam->multi_day_card_count,3);
	memcpy(rthauth.multi_day_amount, cardmccparam->multi_day_card_amount,12);
	return;
}



#endif

#ifdef BFL_CUG_CARD

int  Validate_Velocity_Rules(char *SubRes)
{
   char Customer_Risk[3];
   char Transaction_Risk[3];
   char Count_Transaction[4];
   char Days_Transaction[4];
   int  Max_Count = 0;
   int  No_Days = 0 ;
   int  Used_Count = 0 ;
   int  Return_Veloc = -1;
   t_loanvelloc loanvelloc;
    
    memset(Customer_Risk,0,3);
    memset(Transaction_Risk,0,3);
    memset(Count_Transaction,0,4);
    memset(Days_Transaction,0,4);
    memset(&loanvelloc,0x20,sizeof(t_loanvelloc));

    memcpy(Customer_Risk,procdata.customer_risk,2);
    memcpy(Transaction_Risk,procdata.trans_risk,2);

    memcpy(Days_Transaction,procdata.risk_vel_days,3);
    memcpy(Count_Transaction,procdata.trans_velocity,3);

    No_Days = atoi(Days_Transaction);
    Max_Count = atoi(Count_Transaction);
    f_printf(stderr," Count [%d]and No of Days[%d]\n",Max_Count,No_Days);
    f_printf(stderr," Count [%.3s]and No of Days[%.3s]\n",procdata.trans_velocity,procdata.risk_vel_days);

    if((No_Days == 0) ||( Max_Count == 0 ))
    {
       f_printf(stderr,"Invalid Count [%d]and No of Days[%d]\n",Max_Count,No_Days);
       return( SUCCESS);
    }

    if((No_Days == 999) &&( Max_Count == 999 ))
    {
       f_printf(stderr,"\nNO NEED TO CHECK THE VELOCITY CHECK\n");
       f_printf(stderr,"\nDEFAULT COUNT [%d]and DEFAULT NUMBER OF DAYS[%d]\n",Max_Count,No_Days);
       return( SUCCESS );
    }

    #ifdef BFL_PHASE2
    if(memcmp(bflzbcom.lbcom_vip_nonvip,"Y",1) == 0 )
    {
       f_printf(stderr,"SKIPPING VLOCITY CHECKS----ACCOUNT IS UNDER VIP STATUS[%.1s]\n",bflzbcom.lbcom_vip_nonvip);
       Velocity_Check  = 1;
       return( SUCCESS );
    }
    #endif


    memcpy(loanvelloc.bank ,"00059",5); 
    memcpy(loanvelloc.card_number,bflzport.lport_cust_id,19); 
    memcpy(loanvelloc.customer_risk,Customer_Risk,2); 
    memcpy(loanvelloc.transaction_risk,Transaction_Risk,2); 
    f_printf(stderr,"1.PID[%d],TIME[%.14s]\n",getpid(),(char *)GetYYYYMMDDHHMMSS());
    Return_Veloc = ValidateLoanVelocLimit(SubRes,&loanvelloc,&Max_Count,&No_Days, &Used_Count );
    f_printf(stderr,"ValidateLoanVelocLimit  Return Error [%d]",Return_Veloc);
    f_printf(stderr,"2.PID[%d],TIME[%.14s]\n",getpid(),(char *)GetYYYYMMDDHHMMSS());
    if(memcmp(SubRes,"00",2) == 0 )
    return ( SUCCESS );
    else
    return (ECHEC);
}



int ValidateLoanVelocLimit(char *SubRes,t_loanvelloc *loanvel,int *MaxCount,int *DayCount, int *Totalloan_count )
{

 	int 	i = 0;
        char	CountBuf[4];
  
 	*Totalloan_count = 0;
	f_printf(stderr,"Max Count Allowable: [%d],No-Days[%d]",*MaxCount,*DayCount);

        int   fd_lnvel = -1;
        char  velloc_open[50];

                memset(velloc_open,0x00,sizeof(velloc_open));
                sprintf(velloc_open,"%s/LN-VELOCITY",getenv("MultipacDat"));

                fd_lnvel = isopen(velloc_open,ISINOUT+ISMANULOCK);
                if(fd_lnvel < 0 )
                {
                        f_printf(stderr," ValidateLoanVelocLimit File [%s] Open Error [%d]",velloc_open,iserrno);
                        isclose(fd_lnvel);
                        memcpy(SubRes,"960000",6);
                        return(ECHEC);
                }


 	for(i = 0; i < *DayCount;i++)
 	{

		memcpy(loanvel->date_used,(char*)GetDateDec ( i ),6);
                f_printf(stderr,"Velocity Count [%.3s],MaxCount[%d] Date[%.6s]",loanvel->velocity_count,*MaxCount,loanvel->date_used);
		memset ( CountBuf, 0, 4 );
	
		if(isread(fd_lnvel,(char *)loanvel,ISEQUAL)== 0 )
        	{

	   		memcpy(CountBuf,loanvel->velocity_count, 3 );
	   		*Totalloan_count += atoi(CountBuf);
                        isrelease(fd_lnvel);

        	}
        	else
        	{
    			if(iserrno != 111)
                	f_printf(stderr,"Read Error [%d], Date[%.6s]\n",iserrno,loanvel->date_used);

        	}
 		if(*MaxCount < (*Totalloan_count + 1) )
 		{
                        NXTALLD = *DayCount - i ;
			f_printf(stderr,"Velocity Count Excedded");
			f_printf(stderr,"MAX Allowable Velocity Count[%d] ,Used count [%d],Next Trans[%d]",*MaxCount,*Totalloan_count,NXTALLD);
        		memcpy(SubRes,"650000",6);
                        i = *DayCount ;
                        isclose(fd_lnvel);
        		return(0);
 		}

 	}
         f_printf(stderr,"Velocity Count After Reading[%d],MaxCount[%d]",*Totalloan_count,*MaxCount);
	
	 isrelease(fd_lnvel);
 
 	if(*MaxCount < (*Totalloan_count + 1) )
 	{
		f_printf(stderr,"Velocity Count Excedded");
		f_printf(stderr,"MAX Allowable Velocity Count[%d] ,Used count [%d]",*MaxCount,*Totalloan_count);
        	memcpy(SubRes,"650000",6);
                isclose(fd_lnvel);
        	return(0);
 	}
 	else
 	memcpy(SubRes,"000000",6);

 isclose(fd_lnvel);
 return(0);

}


int UpdateLoanVelocLimit_ltv ( )
{
	t_bflvelloc bflvelloc;
	t_rrn_ruleid_map rrn_rule;
	int  loopVar         =  0;
	int fd_lnvel1,fdrrnrule;
	int Counter =0;
	char velloc_open[200];
	char        CountBuffer[4];
	memset(velloc_open,0x00,sizeof(velloc_open));
	sprintf(velloc_open,"%s/LN-VELOCITY",getenv("MultipacDat"));

	fd_lnvel1 = isopen(velloc_open,ISINOUT+ISMANULOCK);
	if(fd_lnvel1 < 0 )
	{
		f_printf(stderr,"Update-File [%s] Open Error [%d]",velloc_open,iserrno);
		isclose(fd_lnvel1);
		return(ECHEC);
	}

	memset(&bflvelloc,0x20,sizeof(t_bflvelloc));
	memcpy(bflvelloc.customer_id, bflzport.lport_cust_id,19);
	memcpy(bflvelloc.date_used,(char*)GetDateDec ( 0 ),6);
	
	/******	Changes done to add VELOCITY RULE who satisfies the parameter (14 April 2018) ******/		
	
	  memset(velloc_open,0x00,sizeof(velloc_open));
	  sprintf(velloc_open,"%s/LN-VELRRNMAP",getenv("MultipacDat"));
	  fdrrnrule = isopen(velloc_open,ISINOUT+ISMANULOCK);
	  if(fdrrnrule < 0 )
	  {
		  f_printf(stderr," ValidateLoanVelocLimit File [%s] Open Error [%d]",velloc_open,iserrno);
		  isclose(fdrrnrule);
		  return(-1);
	  }

	f_printf(stderr,"DATA IN UpdateLoanVelocLimit_ltv \n");

	while(VL_Rule_Done[loopVar][0] != '\0' )
	{ 
		f_printf(stderr,"VL_Rule_Done[%*.s] \n",strlen(VL_Rule_Done[loopVar]),VL_Rule_Done[loopVar]);
			
		memcpy(bflvelloc.ruleid,VL_Rule_Done[loopVar], 5); 	
	
		if ( isread ( fd_lnvel1 , (char *)&bflvelloc, ISEQUAL ) < 0 )
		{
			f_printf(stderr, "\n Unable To Read Current Date Record[%d]\n",iserrno);
			memcpy(CountBuffer,"001",3);
			stchar ( CountBuffer, bflvelloc.velocity_count , 3 );
			
			f_printf(stderr,"\nFirst Time Transaction For The Day ltv");
			f_printf(stderr,"VELLOCITY  RECORD [%.41s]\n",&bflvelloc);
			
			if ( iswrite ( fd_lnvel1 , (char *)&bflvelloc ) < 0 )
			{
				f_printf(stderr, "Write ERROR in LOAN VELOC :[%d] \n", iserrno );
			}
			else
			f_printf(stderr,"\nWrite Success");
		}
		else
		{
			memcpy ( CountBuffer, bflvelloc.velocity_count, 3 );
			Counter = atoi ( CountBuffer ) + 1;
			sprintf ( CountBuffer, "%03d", Counter );
			if (Counter > 0 )
			{
				stchar ( CountBuffer, bflvelloc.velocity_count , 3 );
				if ( isrewrite ( fd_lnvel1 , (char *)&bflvelloc ) < 0 )
				{
					f_printf(stderr, "REWRITE ERROR IN LOAN VELOC : [%d] \n",iserrno);
				}
				else
					f_printf(stderr,"\nRE-Write Success");
			}
			f_printf(stderr,"\nUpdate Success");
			isrelease ( fd_lnvel1 );
		}
		

		  memset(&rrn_rule, ' ', sizeof(t_rrn_ruleid_map));
		  memcpy(rrn_rule.rrn, paczisoauto.retrieval_reference_number, 12);
		  memcpy(rrn_rule.ruleid, bflvelloc.ruleid, 7);
		  memcpy(rrn_rule.product_code,buisness_prod_code ,strlen(buisness_prod_code));
		  memcpy(rrn_rule.trans_date, bflvelloc.date_used, 6);

		 if ( iswrite ( fdrrnrule , (char *)&rrn_rule ) < 0 )
		 {
			 f_printf(stderr, "Write ERROR in RRN-RULEID-MAP :[%d] \n", iserrno );
		 }
		 else
			 f_printf(stderr,"\nWrite Success in RRN-RULEID-MAP\n");


		loopVar++;
	}
		 
	isrelease (fdrrnrule);
	 isclose(fdrrnrule);
	 isclose(fd_lnvel1);

}


int ReverseLoanVelocLimit_ltv( char *date_us, char *reference_number)
{
        t_bflvelloc 		bflvelloc;
        t_rrn_ruleid_map 	rrn_ruleid;
        int  loopVar         =  0;
        int fd_lnvel2 	     =  0;
	int  fd_rrn_ruleid    =  0;
        int Counter 	     =  0;
        int io 		     =  0;
        char file_open[200];
        char CountBuffer[4];
        struct keydesc optkey1;

        f_printf(stderr," In ReverseVelocityRule  \n");

        memset(file_open,0x00,sizeof(file_open));
        sprintf(file_open,"%s/LN-VELOCITY",getenv("MultipacDat"));


        fd_lnvel2 = isopen(file_open,ISINOUT+ISMANULOCK);
        if(fd_lnvel2 < 0 )
        {
                f_printf(stderr,"Update-File [%s] Open Error [%d]",file_open,iserrno);
                isclose(fd_lnvel2);
                return ;
        }

        memset(file_open,0x00,sizeof(file_open));
        sprintf(file_open,"%s/LN-VELRRNMAP",getenv("MultipacDat"));
        fd_rrn_ruleid = isopen(file_open,ISINOUT+ISMANULOCK);
        if(fd_rrn_ruleid < 0 )
        {
                f_printf(stderr,"Update-File [%s] Open Error [%d]",file_open,iserrno);
                isclose(fd_lnvel2);
                return ;
        }

        memset(&bflvelloc,0x20,sizeof(t_bflvelloc));
        memset(&rrn_ruleid,' ',sizeof(t_rrn_ruleid_map));

        memcpy(rrn_ruleid.rrn,reference_number, 12);
        
	optkey1.k_flags            = ISDUPS;
        optkey1.k_nparts           = 1;
        optkey1.k_part[0].kp_start = 0;
        optkey1.k_part[0].kp_leng  = 19;
        optkey1.k_part[0].kp_type  = CHARTYPE;


        if ( isstart(fd_rrn_ruleid, &optkey1, 12, (char*)&rrn_ruleid, ISEQUAL) < 0)
        {
                f_printf(stderr,"Error in ISSTAR iserrno[%d] \n",iserrno);
                return;
        }

        if( isread(fd_rrn_ruleid ,  (char *)&rrn_ruleid , ISCURR) < 0)
        {
                f_printf(stderr,"Error in ISREAD iserrno[%d]\n",iserrno);
                return;
        }

        memcpy(bflvelloc.customer_id, bflzport.lport_cust_id,19);
        memcpy(bflvelloc.date_used,date_us ,6);

        while(iserrno != EENDFILE)
        {
                if(memcmp(rrn_ruleid.rrn,reference_number ,12) ==0)
                {
                        memcpy(bflvelloc.ruleid,rrn_ruleid.ruleid, 7);

                        fprintf(stderr,"Reading Data [%.50s] \n ",&bflvelloc);

                        if ( isread ( fd_lnvel2 , (char *)&bflvelloc, ISEQUAL ) < 0 )
                        {
                                fprintf(stderr,"\nBFL- Reverse Velocity Record Not Found[%d]",iserrno);
                        }
                        else
                        {
                                memcpy ( CountBuffer, bflvelloc.velocity_count, 3 );
                                if(atoi ( CountBuffer ) > 0 )
                                Counter = atoi ( CountBuffer );
                                Counter --;
                                sprintf ( CountBuffer, "%03d", Counter );

                                if(Counter >=  0)
                                {

                                        memcpy(bflvelloc.velocity_count, CountBuffer, 3);

                                        fprintf(stderr,"\nBFL-Updating the Reverse Velocity Count" );
                                        if ( isrewrite ( fd_lnvel2 , (char *)&bflvelloc ) < 0 )
                                        {
                                                fprintf(stderr, "REWRITE ERROR IN REVERSE LOAN VELOC : [%d] \n",iserrno);
                                        }
                                        else
                                        {
                                                fprintf(stderr,"Updating record [%.50s] \n",&bflvelloc);
                                        }

                                }
                                isrelease( fd_lnvel2 );
                        }

                }
        io = isread(fd_rrn_ruleid, (char*)&rrn_ruleid, ISNEXT);
        if(io < 0)
        break;

        }

	isrelease( fd_lnvel2 );
        isrelease( fd_rrn_ruleid );
	
	isclose(fd_rrn_ruleid);
	isclose(fd_lnvel2);

}


#endif
