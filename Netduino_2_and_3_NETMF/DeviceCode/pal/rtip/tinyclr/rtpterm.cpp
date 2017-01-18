 /*
 | RTPTERM.C - Runtime Platform Network Services
 |
 |   PORTED TO THE EBSnet Inc RTIP TCP/IP PLATFORM
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: shane $
 |  $Date: 2004/10/05 18:51:31 $
 |  $Name:  $
 |  $Revision: 1.4 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

/************************************************************************
* Headers
************************************************************************/

#include "rtpterm.h"
#include "rtpstr.h"
#include <tinyhal.h>


/************************************************************************
* Defines
************************************************************************/
#define TERMINAL_UP_ARROW       1
#define TERMINAL_DOWN_ARROW     2
#define TERMINAL_RIGHT_ARROW    3
#define TERMINAL_LEFT_ARROW     4
#define TERMINAL_ESCAPE         27
#define TERMPORT 2
/************************************************************************
* Types
************************************************************************/

/************************************************************************
* Data
************************************************************************/

/************************************************************************
* Macros
************************************************************************/

/************************************************************************
* Function Prototypes
************************************************************************/

extern void dummy_lcd_printf(char * str);

/************************************************************************
* Function Bodies
************************************************************************/

/*----------------------------------------------------------------------*
                            rtp_term_init
 *----------------------------------------------------------------------*/
int rtp_term_init(void)
{
    NATIVE_PROFILE_PAL_NETWORK();
	return 1;
}

/*----------------------------------------------------------------------*
                            rtp_term_kbhit
 *----------------------------------------------------------------------*/
int rtp_term_kbhit (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
     return 1;
}


/*----------------------------------------------------------------------*
                            rtp_term_getch
 *----------------------------------------------------------------------*/
int rtp_term_getch (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
int ch = 0;
     
    return (ch);
}


/*----------------------------------------------------------------------*
                             rtp_term_putc
 *----------------------------------------------------------------------*/
void rtp_term_putc (char ch)
{
    NATIVE_PROFILE_PAL_NETWORK();
    char str[2] = {0};
    
    str[0] = ch;
    lcd_printf(str);
    return;
}



/************************************************************************/
/*      THERE IS NO NEED TO CHANGE ANYTHING BELOW THIS COMMENT          */
/************************************************************************/




/*----------------------------------------------------------------------*
                             rtp_term_puts
 *----------------------------------------------------------------------*/
void rtp_term_puts (const char * string)
{
    NATIVE_PROFILE_PAL_NETWORK();
    while (*string)
    {
        rtp_term_putc(*string++);
    }
    rtp_term_putc('\n');
}


/*----------------------------------------------------------------------*
                             rtp_term_cputs
 *----------------------------------------------------------------------*/
int rtp_term_cputs (const char * string)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return (0);
}


/*----------------------------------------------------------------------*
                             rtp_term_gets
 *----------------------------------------------------------------------*/
int rtp_term_gets (char * string)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}


/*----------------------------------------------------------------------*
                          rtp_term_promptstring
 *----------------------------------------------------------------------*/
int rtp_term_promptstring (char * string, unsigned int handle_arrows)
{
    NATIVE_PROFILE_PAL_NETWORK();
/* ----------------------------------- */
/*  Endptr always points to            */
/*  null-terminator.                   */
/* ----------------------------------- */
char * endptr = &string[rtp_strlen(string)];
int ch;
char clbuff[80];

	rtp_memset((unsigned char *)clbuff, ' ', 79);
	clbuff[0] = '\r';
	clbuff[78] = '\r';
	clbuff[79] = 0;

#define CLEAR_LINE() rtp_term_cputs(clbuff)

    /* ----------------------------------- */
	/*  Print out the default answer.      */
	/* ----------------------------------- */
	rtp_term_cputs(string);
    
    ch = rtp_term_getch( );
	while (ch != -1)
	{
		switch(ch)
		{
		    /* ----------------------------------- */
			/*  Return.                            */
			/* ----------------------------------- */
		    case '\n':
		    case '\r':
			    rtp_term_putc('\n');
			    return (0);

            /* ----------------------------------- */
			/*  Backspace.                         */
			/* ----------------------------------- */
		    case '\b':
			    if(endptr > string)
			    {
				    rtp_term_cputs("\b \b");
				    *(--endptr) = 0;
			    }               /* ----------------------------------- */
			    goto getnext;   /*  Get next character.                */
			                    /* ----------------------------------- */
			                
		    case TERMINAL_UP_ARROW:
			    if(handle_arrows)
			    {
			        /* ----------------------------------- */
				    /*  Erase the current line.            */
				    /* ----------------------------------- */
				    CLEAR_LINE();                       /* ----------------------------------- */
				    return (rtp_term_up_arrow ( ));     /*  TERMINAL_UP_ARROW                  */
			    }                                       /* ----------------------------------- */
			    break;

		    case TERMINAL_DOWN_ARROW:
			    if(handle_arrows)
			    {
			        /* ----------------------------------- */
				    /*  Erase the current line.            */
				    /* ----------------------------------- */
				    CLEAR_LINE();                       /* ----------------------------------- */
				    return (rtp_term_down_arrow ( ));   /*  TERMINAL_DOWN_ARROW                */
			    }                                       /* ----------------------------------- */
			    break;

		    case TERMINAL_ESCAPE:
			    if(handle_arrows)
			    {
				    /* ----------------------------------- */
				    /*  Erase the current line.            */
				    /* ----------------------------------- */
				    CLEAR_LINE();                       /* ----------------------------------- */
				    return (rtp_term_escape_key ( ));   /*  TERMINAL_ESCAPE                    */
			    }                                       /* ----------------------------------- */
			    break;
		}

        /* ----------------------------------- */
		/*  Display the editing.               */
		/* ----------------------------------- */
		rtp_term_putc((char)ch);
		*endptr++ = (char)ch;
		*endptr = 0;

getnext:        
        ch = rtp_term_getch( );
	}
	return (-1);
}


/*----------------------------------------------------------------------*
                           rtp_term_up_arrow
 *----------------------------------------------------------------------*/
int rtp_term_up_arrow (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return ((int) TERMINAL_UP_ARROW);
}


/*----------------------------------------------------------------------*
                          rtp_term_down_arrow
 *----------------------------------------------------------------------*/
int rtp_term_down_arrow (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return ((int) TERMINAL_DOWN_ARROW);
}


/*----------------------------------------------------------------------*
                          rtp_term_left_arrow
 *----------------------------------------------------------------------*/
int rtp_term_left_arrow (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return ((int) TERMINAL_LEFT_ARROW);
}


/*----------------------------------------------------------------------*
                          rtp_term_right_arrow
 *----------------------------------------------------------------------*/
int rtp_term_right_arrow (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return ((int) TERMINAL_RIGHT_ARROW);
}


/*----------------------------------------------------------------------*
                          rtp_term_escape_key
 *----------------------------------------------------------------------*/
int rtp_term_escape_key (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return ((int) TERMINAL_ESCAPE);
}


/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
