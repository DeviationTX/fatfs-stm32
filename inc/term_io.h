/********************************************************************************/
/*!
	@file			term_io.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        6.00
    @date           2013.11.30
	@brief          Interface of Chan's Universal string handler. @n
					Based on Chan's FatFs Test Terminal Thanks!

    @section HISTORY
		2010.06.01	V1.00	Start Here.
		2010.12.31	V2.00	Removed uart includion.
		2011.03.10	V3.00	C++ Ready.
		2013.04.09	V4.00	Imploved TouchInput Handling.
		2013.10.20	V5.00	Changed Key Definition.
		2013.11.30	V6.00	Fixed KeyInput Definition.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __TERM_IO_H
#define __TERM_IO_H	0x0600

#ifdef __cplusplus
 extern "C" {
#endif

/* Basics */
#include <stdarg.h>
#include <string.h>
#include "xprintf.h"

/* Miscs */
#include "hw_config.h"
#include "ff.h"
#include "display_if.h"
#if	defined(USE_TOUCH_CTRL)
 #include "touch_if.h"
#endif
/* Can execute Key Input below BOARDS */
#if defined(USE_STM32PRIMER2) || defined(USE_REDBULL) || defined(USE_ETHERPOD) || \
    defined(USE_STM32F4DISCOVERY)
 #define USE_HWKEY_INPUT_SUPPORT
#endif

/* #define xavail() keypressed() */
#define __kbhit() keypressed()

/* Externals */
extern char xgetc_n (void);
extern unsigned char xgetc (void);

/* Macros */
/* File loader control buttons */
#define BTN_UP		'\x05'	/* (Up) ^[E] */
#define BTN_DOWN	'\x18'	/* (Down) ^[X] */
#define BTN_LEFT	'\x13'	/* (Left) ^[S] */
#define BTN_RIGHT	'\x04'	/* (Right) ^[D] */
#define BTN_OK		'\x0D'	/* (Ok) [Enter] */
#define BTN_ESC		'\x1A'	/* (Cancel) [Esc] */
#define BTN_CAN		'\x1B'	/* (Cancel) [Esc] */
#define BTN_PAUSE	' '		/* (Pause) [Space] */


#ifdef __cplusplus
}
#endif

#endif	/* __TERM_IO_H */
