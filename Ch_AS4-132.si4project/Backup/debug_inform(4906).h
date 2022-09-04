#ifndef __DEBUG_INFORM_H__
#define __DEBUG_INFORM_H__

#define DEBUG_LCASACTION_SUBMENU_NO			0
#define DEBUG_MOTORCTRL_SUBMENU_NO			0

#ifndef NDEBUG_DP
#define dp(fmt,args...) iprintf( fmt, ## args )
#define dlp(fmt,args...) iprintf( "DLP [%s %s %d]" fmt, __FILE__,__FUNCTION__,__LINE__, # args )
#define dfp(fmt,args...) iprintf( "DFP [%s %d]-" fmt, __FUNCTION__,__LINE__, ## args ) 
#define dtmp(a)						iprintf(#a" = %d\n",tmp.a)
#define dprintf(fmt,args...) printf( fmt, ## args )
	
#define diprintf(fmt,args...) iprintf( fmt, ## args )
#define dfpErr(fmt,args...)  iprintf( "DFPERR [%s %d]-" fmt, __FUNCTION__,__LINE__, ## args ) 
	
#define errorFunction(X)			{ iprintf( "[%s %d]",__FUNCTION__,__LINE__); errFunction(X); }

#define dp0(msg) { \
	const uint8_t message[] = msg; \
	MSS_UART_polled_tx( &g_mss_uart0, message, sizeof(message) ); \
}

#define dpi0(msg) { \
	const uint8_t message[] = msg; \
	MSS_UART_irq_tx( &g_mss_uart0, message, sizeof(message) ); \
}

#else
#define dp(fmt,args...)
#define dlp(fmt,args...)
#define dfp(fmt,args...) 

#define dtmp(a)
#define dprintf(fmt,args...)

#define diprintf(fmt,args...)
//#define diprintf(fmt,args...) iprintf( fmt, ## args )
#define dfpErr(fmt,args...)  iprintf( "DFPERR [%s %d]-" fmt, __FUNCTION__,__LINE__, ## args ) 

#define errorFunction(X)			{ errFunction(X); }

#define dp0(msg)
#define dpi0(msg)

#endif




#endif //__DEBUG_INFORM_H__
