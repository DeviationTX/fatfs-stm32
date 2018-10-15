/********************************************************************************/
/*!
	@file			uart_support.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        7.00
    @date           2016.01.10
	@brief          Based on ST Microelectronics Sample Thanks!

    @section HISTORY
		2011.06.12	V1.00	Start Here.
		2011.09.14	V2.00	Expand fifo buffer uint8_t to uint16_t
		2012.01.31	V3.00	UART IRQ Routine moved from stm32f4xx_it.c.
		2014.05.01	V4.00	Fixed Suitable Interrupt-Preemption level.
		2015.01.09	V5.00	Added buffered UART information.
		2015.09.01	V6.00	Optimized some uart variables.
		2016.01.10  V7.00	Added selectable receive procedure on polling mode.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __UART_SUPPORT_H
#define __UART_SUPPORT_H	0x0700

#ifdef __cplusplus
 extern "C" {
#endif

/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

/* uC Specific Inclusion */
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_conf.h"

/* USART Definition */
#define UART_BUFSIZE			256			/* Buffer size MUST Takes power of 2(64,128,256,512...) */
#define UART_BAUDRATE			230400UL
#define UART_INTERRUPT_MODE					/* If u want polling mode, uncomment this */
#define UART_NOBLOCK_RECV		1			/* Set 1 to non-blocking receive on polling mode */


#if defined(USE_STM32F4DISCOVERY)
 #define UART_DEFAULT_NUM		2
 #define USARTx_Buf				USART2_Buf
#elif defined(USE_ECH_BOARD)	/* STM32F42x/43x has UART7,8 */
 #define UART_DEFAULT_NUM		7
 #define USARTx_Buf				UART7_Buf
#else
 #define UART_DEFAULT_NUM		1
 #define USARTx_Buf				USART1_Buf
#endif

/* General Definition */
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Funcion Prototypes */
extern void conio_init(uint32_t port, uint32_t baudrate);
extern void putch(uint8_t c);
extern uint8_t getch(void);
extern uint8_t keypressed(void);
extern void cputs(char *s);
extern void cgets(char *s, int bufsize);

/* Structs of UART(This is Based on AVRX uC Sample!!!) */
/* @brief USART transmit and receive ring buffer. */
typedef struct USART_Buffer
{
	/* @brief Receive buffer. */
	volatile uint8_t RX[UART_BUFSIZE];
	/* @brief Transmit buffer. */
	volatile uint8_t TX[UART_BUFSIZE];
	/* @brief Receive buffer head. */
	volatile unsigned int RX_Head;
	/* @brief Receive buffer tail. */
	volatile unsigned int RX_Tail;
	/* @brief Transmit buffer head. */
	volatile unsigned int TX_Head;
	/* @brief Transmit buffer tail. */
	volatile unsigned int TX_Tail;
} USART_Buffer_t;

/* Externs */
extern USART_Buffer_t USARTx_Buf;


#ifdef __cplusplus
}
#endif

#endif	/* __UART_SUPPORT_H */
