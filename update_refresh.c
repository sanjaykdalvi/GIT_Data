
/*****************************************************************************
 *                                                                           *
 * Project             : MultiXPac ONUS                                      *
 * Module              : ONUS Authorization                                  *
 * File Name           : update_refresh.c                                    *
 * Author(s)           : Gilu Alex, Sudheer K T, Saju Joseph & Azhakesh A K  *
 * Date                : 22-Feb-2000                                         *
 * Description         : Functions to modify refreshing parameters           *
 *                                                                           *
 * Notes               :                                                     *
 *                                                                           *
 * Modification History:                                                     *
 *---------------------------------------------------------------------------*
 * #   Date      Programmer/Reviewer  Description                            *
 *---------------------------------------------------------------------------*
 * 1   12-Mar-03 Shadakappa S         Project CINNAMON : re-structured ONUS  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <paczpoau.h>
 #include <onus.h>
 #include <define.h>

 #include "update_refresh.h"
 
/* Chowdesha added for CBA on 31-03-2011 */
#define CBA  1
extern t_paczpoau 	paczpoau;
 
#define LODHA_PALAV 1

 /* function prototypes */
extern char* GetDateYYYYMMDD ();
extern double atof ( char* );

char* GetRefreshingDate ( char* SysDate, int RefreshDay );
int IsLeapYear( char year[5] );
int DayOfMonth();
int WeekdaysSun0Sat6();
char * GetOldDateYYYYMMDD( char * Date, int no );
int DiffOfDatesYYYYMMDD( char * Date1, char * Date2 );

/* Changed by ACR, 08/10/2005, syntax was not correct */
int NoOfDaysInTheMonth( char monthyear[] );

int MonthOfTheYear();
int WeekDaysSun0Sat6();

int CardAvailRefresh ( void )
{
	char		LastAuthDate[9];
	char		VelocityCode[3];
	char		SystemDate[9];
	char		AmountBuf[13];
	int		RefreshCount ;
	int		VelocityCount ;
	int 		MonthDay ;
	int		WeekDay ;
	int         	DiffOfDates ;
	double		FltCardLimit ;
	double		FltExcessLimit ;

	memset ( LastAuthDate, 0, 9 );
	memset ( VelocityCode, 0, 3 );
	memset ( SystemDate, 0, 9 );

	/* Initialize the parameters */
	RefreshCount = 0 ;
	VelocityCount = 0 ;
	MonthDay = 0 ;
	WeekDay = 0 ;
	DiffOfDates = 0 ;
	FltCardLimit = 0.0 ;
	FltExcessLimit = 0.0 ;

	/* Get the syetm date */
	memcpy ( SystemDate, GetDateYYYYMMDD (), 8 );

	memcpy ( LastAuthDate, "20", 2 );
    if (memcmp ( paczpoau.poau_dat_dern_auto, "      ", 6 ) == 0 )
	   memcpy ( LastAuthDate+2, "000000", 6 );
    else
       memcpy ( LastAuthDate+2, paczpoau.poau_dat_dern_auto, 6 );

	memcpy ( VelocityCode, paczpoau.poau_periodicite_plafond, 2 );


	FPRINTF ( stderr, "\nCARD AVAILABLE REFRESH \n" );
	FPRINTF ( stderr, "\nLast Auth Date [%.8s], Refresh Code [%.2s]\n", \
	  LastAuthDate, VelocityCode );
	
	if ( memcmp ( LastAuthDate, SystemDate, 8 ) == 0 )
	{
		FPRINTF ( stderr, "Auth already recorded today - No Refreshing \
		 required \n" );
		return ( SUCCESS );
	}

	VelocityCount = VelocityCode[1] - 48;

	if ( VelocityCode[0] == 'D' )
		FPRINTF ( stderr, "DAILY REFRESHING \n" );
	else if ( VelocityCode[0] == 'W' )
	{
		FPRINTF ( stderr, "WEEKLY REFRESHING \n" );
		switch ( VelocityCount )
		{
		case 0:
			FPRINTF ( stderr, "Every Sunday \n");
			break;

		case 1:
			FPRINTF ( stderr, "Every Monday \n");
			break;

		case 2:
			FPRINTF ( stderr, "Every Tuesday \n");
			break;

		case 3:
			FPRINTF ( stderr, "Every Wednsday \n");
			break;

		case 4:
			FPRINTF ( stderr, "Every Thursday \n");
			break;

		case 5:
			FPRINTF ( stderr, "Every Friday \n");
			break;

		case 6:
			FPRINTF ( stderr, "Every Saturday \n");
			break;

		default:
			FPRINTF ( stderr, "Unknown Refresh Code \n" );
			return ( ECHEC );			/* Added by ACR, 08/10/2005 */
		}
		/************ Modified by ACR, 08/10/2005 ************/
		/* If the differece b/w sys date and last auth date > 7 days
		   do an unconditional refresh of card parameters */
		DiffOfDates = DiffOfDatesYYYYMMDD( SystemDate, LastAuthDate ) ;
		if( DiffOfDates <= 7 )
		{
			WeekDay = WeekDaysSun0Sat6 ();

			if( WeekDay > VelocityCount )
			{
				if( DiffOfDates > (WeekDay - VelocityCount) )
				{
					FPRINTF ( stderr, "Refreshing to be done today \n");
				}
				else
				{
					FPRINTF ( stderr, "Refreshing not to be done today \n");
					return ( SUCCESS );
				}
			}
			else if( WeekDay != VelocityCount )
			{
				FPRINTF ( stderr, "Refreshing not to be done today \n");
				return ( SUCCESS );
			}
		}
		else
		{
			FPRINTF ( stderr, "Last Auth more than week old\n" );
			FPRINTF ( stderr, "Refreshing to be done today \n");
		}
	}
	else if ( VelocityCode[0] == 'M' )
	{
		FPRINTF ( stderr, "Monthly Refreshing - on %c \n", VelocityCode[1] );
		MonthDay = DayOfMonth ();
		if ( VelocityCount < MonthDay )
		{
			if ( memcmp ( LastAuthDate, SystemDate, 6 ) == 0 )
				return ( SUCCESS );
			else
				FPRINTF ( stderr, "Last Transaction is older than Month \n" );
		}
		else if ( VelocityCount != MonthDay )
		{
			FPRINTF ( stderr, "Refreshing not to be done today \n");
			return ( SUCCESS );
		}
	}
	else
	{
		/* PAYMENT APPROPRIATION */
		if ( VelocityCode[0] == 'N' )
		{
			FPRINTF ( stderr, "No Refresh-Payment Appropriation \n" );
			return ( SUCCESS );
		}
		else
		{
			FPRINTF ( stderr, "INVALID REFRESH CODE [%.2s] \n", VelocityCode );
			return ( ECHEC );
		}
	}
	
	FPRINTF ( stderr, "REFRESH IS DONE AND ITS OK \n" );
	/* Calculate Available Card has Card Limit + Excess Card Limit */
	memset ( AmountBuf, 0, 13 );
	memcpy ( AmountBuf, paczpoau.poau_plafond_carte, 12 );
	FltCardLimit = atof ( AmountBuf );

	memcpy ( AmountBuf, paczpoau.poau_card_advice, 12 );
	FltExcessLimit = atof ( AmountBuf );
	sprintf ( AmountBuf, "%012.0f", FltCardLimit+FltExcessLimit );
	memcpy ( paczpoau.poau_residuel_carte, AmountBuf, 12 );
	memcpy ( paczpoau.poau_dat_dern_auto, SystemDate+2, 6 );
        memcpy ( paczpoau.poau_stip_count, "0000", 4);
	
	return ( SUCCESS );
}

int CashAvailRefresh ( void )
{
	char		LastAuthDate[9];
	char		VelocityCode[3];
	char		SystemDate[9];
	char		AmountBuf[13];
	int		RefreshCount;
	int		VelocityCount;
	int		WeekDay ;
	int         	MaxDaysInMonth ;
	int		MonthDay ;
	int         	DiffOfDates ;
	double		FltCashLimit ;
	double		FltExcessLimit ;

	memset ( LastAuthDate, 0, 9 );
	memset ( VelocityCode, 0, 3 );
	memset ( SystemDate, 0, 9 );

	RefreshCount = 0;
	VelocityCount = 0;
	WeekDay = 0;
	MaxDaysInMonth = 0;
	MonthDay = 0;
	DiffOfDates = 0;
	FltCashLimit = 0.0;
	FltExcessLimit = 0.0;

	/* Get the System Date */
	memcpy ( SystemDate, GetDateYYYYMMDD (), 8 );

	/* Get the Last auth date from card auth structure */
	memcpy ( LastAuthDate, paczpoau.poau_last_cash_date, 8 );
	memcpy ( VelocityCode, paczpoau.poau_periodicite_cash, 2 );
    if ( memcmp ( LastAuthDate, "        ", 8 ) == 0 )
       memcpy ( LastAuthDate,"00000000",8);
	FPRINTF ( stderr, "\nCASH AVAILABLE REFRESH \n" );
	FPRINTF ( stderr, "Last Cash Date [%.8s], Refresh Code [%.2s] \n", \
	 LastAuthDate, VelocityCode );

	if ( memcmp ( LastAuthDate, SystemDate, 8 ) == 0 )
	{
		FPRINTF ( stderr, "Auth already recorded today-No refreshing \
			required \n" );
		return ( SUCCESS );
	}

	VelocityCount = VelocityCode[1] - 48;

	if ( VelocityCode[0] == 'D' )
		FPRINTF ( stderr, "DAILY REFRESHING \n" );
	else if ( VelocityCode[0] == 'W' )
	{
		FPRINTF ( stderr, "Weekly Refresh - When first txn received on or \
		 after " );
		switch ( VelocityCount )
		{
		case 0:
			FPRINTF ( stderr, "Every Sunday \n");
			break;

		case 1:
			FPRINTF ( stderr, "Every Monday \n");
			break;

		case 2:
			FPRINTF ( stderr, "Every Tuesday \n");
			break;

		case 3:
			FPRINTF ( stderr, "Every Wednsday \n");
			break;

		case 4:
			FPRINTF ( stderr, "Every Thursday \n");
			break;

		case 5:
			FPRINTF ( stderr, "Every Friday \n");
			break;

		case 6:
			FPRINTF ( stderr, "Every Saturday \n");
			break;

		default:
			FPRINTF ( stderr, "Unknown Refresh Code \n" );
		}
		
		/* Modified by ACR, 06/10/2005 */
		/* If the differece b/w sys date and last auth date > 7 days
		   do an unconditional refresh of card parameters */
		DiffOfDates = DiffOfDatesYYYYMMDD( SystemDate, LastAuthDate ) ;
		if( DiffOfDates <= 7 )
		{
			WeekDay = WeekDaysSun0Sat6 ();

			if( WeekDay > VelocityCount )
			{
				if( DiffOfDates > (WeekDay - VelocityCount) )
				{
					FPRINTF ( stderr, "Refreshing to be done today \n");
				}
				else
				{
					FPRINTF ( stderr, "Refreshing not to be done today \n");
					return ( SUCCESS );
				}
			}
			else if( WeekDay != VelocityCount )
			{
				FPRINTF ( stderr, "Refreshing not to be done today \n");
				return ( SUCCESS );
			}
		}
		else
		{
		    FPRINTF ( stderr, "Last Auth more than week old\n" );
		    FPRINTF ( stderr, "Refreshing to be done today \n");
		}
	}
	else if ( VelocityCode[0] == 'M' )
	{
		FPRINTF ( stderr, "MONTHLY REFRESHING \n" );                    
		if ( memcmp ( LastAuthDate, SystemDate, 6 ) == 0 )              
		{                                                               
			FPRINTF ( stderr, "Refreshing not to be done today \n");
			return ( SUCCESS );                                     
		}
		else
		{ 
			FPRINTF ( stderr, "Last transaction not in current month\n");
		} 
	}
	else
	{
		/* PAYMENT APPROPRIATION */
		/* Below condition is Added by ACR, 10-04-2011 */
		if ( VelocityCode[0] == 'N' )
		{
			FPRINTF ( stderr, "No Refresh-Payment Appropriation \n" );
			return ( SUCCESS );
		}
		else
		{
			FPRINTF ( stderr, "Invalid refresh code [%.2s] \n", VelocityCode ) ;
			return ( ECHEC );
		}
	}
	FPRINTF ( stderr, "REFRESH IS DONE AND ITS OK \n" );
	/* Calculate Available Cash has Cash Limit + Excess Cash Limit */
	memset ( AmountBuf, 0, 13 );
	memcpy ( AmountBuf, paczpoau.poau_plafond_cash, 12 );
	FltCashLimit = atof ( AmountBuf );

	memcpy ( AmountBuf, paczpoau.poau_cash_advice, 12 );
	FltExcessLimit = atof ( AmountBuf );
	sprintf ( AmountBuf, "%012.0f", FltCashLimit+FltExcessLimit );
	memcpy ( paczpoau.poau_residuel_cash, AmountBuf, 12 );
	memcpy ( paczpoau.poau_last_cash_date, SystemDate, 8 );

	return ( SUCCESS );
}

char* GetRefreshingDate ( char* SysDate, int RefreshDay )
{
        /* Modified by ACR, 09-10-2006 */
        /*
	char		NewDate[9];
        */
        static char NewDate[9] ;
	int			WeekDay = 0;
	int			DaysToGoBack = 0;

	WeekDay = WeekDaysSun0Sat6 ();
	while ( RefreshDay != WeekDay )
	{
		DaysToGoBack++;
		WeekDay--;
		if ( WeekDay <0 )
			WeekDay = 6;
	}

	sprintf ( NewDate, "%.8s", GetOldDateYYYYMMDD ( SysDate, DaysToGoBack ) );
	return ( NewDate );
}

/* Modified by ACR 09-l0-2006, this syntax is not correct */
/*
int IsLeapYear( char year[5] )
*/
int IsLeapYear( char year[] )
{
   int yr = atoi(year);
   if(((yr % 4) == 0) && (((yr % 400) == 0)||((yr % 100) != 0))) return(TRUE);
   return(FALSE);
}


int DayOfMonth()
{                                          
   time_t    d;                             
   struct tm *dat;                         
   char      dt[3];                          

   (void)time(&d);                      
   dat = localtime(&d);                 
   memset( dt, 0, 3 );                  
   strftime(dt, 3 ,"%d",dat);

   return(atoi(dt));                          
}                                          



int WeekDaysSun0Sat6()
{                                          
   time_t    d;                             
   struct tm *dat;                         
   char      dt[2];                          

   (void)time(&d);                      
   dat = localtime(&d);                 
   memset( dt, 0, 2 );                  
   strftime(dt, 2 ,"%w",dat);

   return(atoi(dt));                          
}                                          


int MonthOfTheYear()
{                                          
   time_t    d;                             
   struct tm *dat;                         
   char      dt[3];                          

   (void)time(&d);                      
   dat = localtime(&d);                 
   memset( dt, 0, 3 );                  
   strftime(dt, 3 ,"%m",dat);

   return(atoi(dt));                          
}                                          

/* Changed by ACR, 08/10/2005, this syntax is not correct */
/*
int NoOfDaysInTheMonth( char monthyear[7] )
*/
int NoOfDaysInTheMonth( char monthyear[] )
{
   char cmnth[3];
   char year[5];

   sprintf( cmnth, "%.2s", monthyear );
   sprintf( year, "%.4s", monthyear+2 );
   
   switch( atoi(cmnth))
   {
       case 4 : return(30);
       case 6 : return(30);
       case 9 : return(30);
       case 11: return(30);
       case 2 : if(IsLeapYear(year)==TRUE ) return(29); return(28);
       default: return(31);
   }
}


int DiffOfDatesYYYYMMDD( char * Date1, char * Date2 )
{
  int day1;
  int day2;

  int month1;
  int month2;

  int year1;
  int year2;

  int noday1=0;
  int noday2=0;

  int refyear = 0;
  int yr, i, leap;

  char Convert[5];

  static char daytab[2][13] = {
      {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
  };

  sprintf( Convert, "%.2s", Date1+6 );
  day1 = atoi(Convert);

  sprintf( Convert, "%.2s", Date2+6 );
  day2 = atoi(Convert);

  sprintf( Convert, "%.2s", Date1+4 );
  month1 = atoi(Convert);

  sprintf( Convert, "%.2s", Date2+4 );
  month2 = atoi(Convert);

  sprintf( Convert, "%.4s", Date1 );
  year1 = atoi(Convert);

  sprintf( Convert, "%.4s", Date2 );
  year2 = atoi(Convert);

  if( year1 < year2 )
     refyear = year1;
  else
     refyear = year2;

  for( yr = refyear ; yr < year1 ; yr++ )
  {
     leap = yr%4 == 0 && yr%100 != 0 || yr%400 == 0;
     for( i = 1; i <= 12 ; i++ )
	noday1 += daytab[leap][i];
  }

  leap = year1%4 == 0 && year1%100 != 0 || year1%400 == 0;
  for( i = 1; i < month1; i++ )
	noday1 += daytab[leap][i];

  noday1 += day1;

  for( yr = refyear ; yr < year2 ; yr++ )
  {
     leap = yr%4 == 0 && yr%100 != 0 || yr%400 == 0;
     for( i = 1; i <= 12 ; i++ )
	noday2 += daytab[leap][i];
  }

  leap = year2%4 == 0 && year2%100 != 0 || year2%400 == 0;
  for( i = 1; i < month2; i++ )
	noday2 += daytab[leap][i];

  noday2 += day2;

  if( noday1 > noday2 )
     return(noday1-noday2);
  else
     return(noday2-noday1);

}

char * GetOldDateYYYYMMDD( char * Date, int no )
{
  int day;
  int month;
  int year;
  int noday=0;
  int yr, i, leap;
  char Convert[5];
  /* Modified by ACR, 09-10-2006 */
  /*
  char new_date[9];
  */
  static char new_date[9];

  static char daytab[2][13] = {
      {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
      {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
  };

  sprintf( Convert, "%.2s", Date+6 );
  day = atoi(Convert);

  sprintf( Convert, "%.2s", Date+4 );
  month = atoi(Convert);

  sprintf( Convert, "%.4s", Date   );
  year = atoi(Convert);

  leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

  for( i = 1; i < month; i++ )
	noday += daytab[leap][i];

  noday += day;

  day = 0;
  month = 1;

  if( noday < no )
  {
     noday = 365-no+noday;
     year--;
     leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
  }
  else
     noday -= no;

  for( i = 1; noday != 0; i++ )
  {
      if( noday > daytab[leap][i])
      {
	 noday -= daytab[leap][i];
	 month++;
	 if( month == 13 )
	 {
	    month = 1;
	    year--;
	    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
         }
      }
      else
      {
	 day = noday;
	 noday = 0;
      }
  }
  sprintf( new_date, "%04d%02d%02d", year, month, day );
  return( new_date );
}

#ifdef CBA 
int FreeTxnCountAmountRefresh( void )
{
	char		LastAuthDate[9],SystemDate[9]; 
	memset ( LastAuthDate, 0x00, sizeof(LastAuthDate) );
	memset(SystemDate, 0x00, sizeof(SystemDate));

	/* Get the System Date */
	memcpy ( SystemDate, (char *)GetDateYYYYMMDD (), 8 );
	memcpy ( LastAuthDate, "20", 2 );

	if (memcmp ( paczpoau.poau_dat_dern_auto, "      ", 6 ) == 0 )
		memcpy ( LastAuthDate+2, "000000", 6 );
	else
		memcpy ( LastAuthDate+2, paczpoau.poau_dat_dern_auto, 6 );

	/* Get the Last auth date from card auth structure */
	/* memcpy ( LastAuthDate, paczpoau.poau_last_cash_date, 8 ); */

	if ( memcmp ( LastAuthDate, "        ", 8 ) == 0 )
		memcpy ( LastAuthDate,"00000000",8);

	FPRINTF(stderr, "\n Free Txn Count and Free Txn Amount Refresh " );
	FPRINTF(stderr, " \nLast Cash Date [%.8s]",  LastAuthDate);

	if ( memcmp ( LastAuthDate+4, SystemDate+4, 2 ) == 0 )
	{
		FPRINTF ( stderr, "Free Txn Count and Free Txn Amount already refreshed - No refreshing required \n" );
		return ( SUCCESS );
	}
	/* Reset the Free txn count and amount field values to zeros */   
	return ( SUCCESS );
}
#endif

#ifdef LODHA_PALAV
int NewCardAvailRefresh ( void )
{
        char            LastAuthDate[9];
        char            VelocityCode[3];
        char            SystemDate[9];
        char            AmountBuf[13];
        int             RefreshCount ;
        int             VelocityCount ;
        int             MonthDay ;
        int             WeekDay ;
        int             DiffOfDates ;
        double          FltCardLimit ;
        double          FltExcessLimit ;

        memset ( LastAuthDate, 0, 9 );
        memset ( VelocityCode, 0, 3 );
        memset ( SystemDate, 0, 9 );

        /* Initialize the parameters */
        RefreshCount = 0 ;
        VelocityCount = 0 ;
        MonthDay = 0 ;
        WeekDay = 0 ;
        DiffOfDates = 0 ;
        FltCardLimit = 0.0 ;
        FltExcessLimit = 0.0 ;

        /* Get the syetm date */
        memcpy ( SystemDate, GetDateYYYYMMDD (), 8 );

        memcpy ( LastAuthDate, "20", 2 );
        if (memcmp ( paczpoau.poau_dat_dern_auto, "      ", 6 ) == 0 )
	memcpy ( LastAuthDate+2, "000000", 6 );
        else
        memcpy ( LastAuthDate+2, paczpoau.poau_dat_dern_auto, 6 );

        memcpy ( VelocityCode, paczpoau.poau_periodicite_plafond, 2 );


        FPRINTF ( stderr, "\nCARD AVAILABLE REFRESH \n" );
        FPRINTF ( stderr, "\nLast Auth Date [%.8s], Refresh Code [%.2s]\n", \
          LastAuthDate, VelocityCode );

        
        VelocityCount = VelocityCode[1] - 48;

        if ( VelocityCode[0] == 'D' )
                FPRINTF ( stderr, "DAILY REFRESHING \n" );
        
        FPRINTF ( stderr, "REFRESH IS DONE AND ITS OK \n" );
        /* Calculate Available Card has Card Limit + Excess Card Limit */
        memset ( AmountBuf, 0, 13 );
        memcpy ( AmountBuf, paczpoau.poau_plafond_carte, 12 );
        FltCardLimit = atof ( AmountBuf );

        memcpy ( AmountBuf, paczpoau.poau_card_advice, 12 );
        FltExcessLimit = atof ( AmountBuf );
        sprintf ( AmountBuf, "%012.0f", FltCardLimit+FltExcessLimit );
        memcpy ( paczpoau.poau_residuel_carte, AmountBuf, 12 );
        memcpy ( paczpoau.poau_dat_dern_auto, SystemDate+2, 6 );

        return ( SUCCESS );
}

#endif
