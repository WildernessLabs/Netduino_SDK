#ifndef _RTIPDATA_H
#define _RTIPDATA_H

// ********************************************************************
// RS232 DATA
// ********************************************************************
#if (INCLUDE_SLIP || INCLUDE_CSLIP || INCLUDE_PPP)
typedef struct rs232_data
{
	RS232_IF_INFO rs232_if_info_arry[CFG_NUM_RS232];
  
} RS232_DATA;

typedef RS232_DATA RTP_FAR * PRS232_DATA;

#endif

// *********************************************************************
// PPP
// *********************************************************************
#if (INCLUDE_PPP)
typedef struct ppp_data
{
	PPP_XMIT_ESCAPE_FNC 		ppp_escape_routine;
	PPP_GIVE_STRING_FNC			ppp_give_string_routine;

} PPP_DATA;

typedef PPP_DATA RTP_FAR * PPPP_DATA;
#endif

#endif /* _RTIPDATA_H */
