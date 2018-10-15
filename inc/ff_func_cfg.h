/********************************************************************************/
/*!
	@file			ff_func_cfg.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        13.00
    @date           2017.06.29
	@brief          Interface of FatFs API Control.						@n
					Based on Chan's FatFs Test Terminal Thanks!

    @section HISTORY
		2014.01.30	V1.00	Renamed from ff_func_xxxx.c
		2014.11.19	V2.00	Adopted FatFs0.10c.
		2015.02.14	V3.00	Adopted FatFs0.11.
		2015.08.01	V4.00	Adopted STM32 HAL Libraries.
		2015.09.01	V5.00	Optimized Performance Timer Handlings.
		2015.12.16	V6.00	Fixed "fm" command arguments.
		2016.01.30	V7.00	Added CID Parser for MMC.
		2016.03.31	V8.00	Added more minute SD/MMC spec parsing.
		2016.04.20	V9.00	Adopted FatFs0.12.
		2016.07.11 V10.00	Adopted FatFs0.12a.
		2016.11.30 V11.00	Adopted FatFs0.12b.
		2017.05.31 V12.00	Adopted FatFs0.13.
		2017.06.29 V13.00	Adopted SD Specification v6.00.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __FF_FUNC_CFG_H
#define __FF_FUNC_CFG_H	0x1300

#ifdef __cplusplus
 extern "C" {
#endif

/* Basics */
#include <string.h> /* memset et al*/

/* Miscs */
#include "ff.h"
#include "diskio.h"
#include "ff_rtc_if.h"
#include "term_io.h"


/* Defines and Configures */
/* Determine FatFs Filer or Command */
#define USE_DISPLAY_FILER	1	/* 1:Enter Display Filer(ESC to Enter Commands) */
                                /* 0:Enter Commands Direct */
/* Determine FatFs Filer Fonts */
#define USE_FILER_FONTX		1	/* 1:use fontx user fonts */
								/* 0:use internal filer font */

/* Determine FatFs Filer Fonts */
#define PARSE_SD_INFO		1	/* 1:use dump & parse CSD/CIS/SDstatus registers */
								/* 0:use dump CSD/CIS/SDstatus registers only */


/* Determine FatFs Buffer memories */
#if   defined(STM32F40_41xxx)

 #if defined(USE_IJG_LIB) || defined(USE_LIBPNG) || defined(USE_GIFLIB)
  #define __ATTR_MEM	__attribute__ ((section(".dmaram"))) __attribute__ ((aligned (4)))
  #define BUFSIZE	16*1024
 #else
  #define __ATTR_MEM	__attribute__ ((aligned (4)))
  #define BUFSIZE	32*1024
 #endif

#elif defined(STM32F427_437xx)
  #define __ATTR_MEM	__attribute__ ((section(".dmaram"))) __attribute__ ((aligned (4)))
  #define BUFSIZE	48*1024

#elif defined(STM32F429_439xx)
  #define __ATTR_MEM	__attribute__ ((section(".dmaram"))) __attribute__ ((aligned (4)))
  #define BUFSIZE	48*1024

#else
 #error "U MUST DEFINE "MPU_MODEL"!! "

#endif

/* Function Prototypes */
extern int ff_test_term(void);
/* Inline Static Function */
/* MUST called by a timer interrupt-handler every 1ms */
extern volatile UINT Timer;
static inline void ff_support_timerproc(void)
{
	Timer++;
}


#ifdef __cplusplus
}
#endif

#endif /* __FF_FUNC_CFG_H */
