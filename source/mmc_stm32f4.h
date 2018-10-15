/********************************************************************************/
/*!
	@file			mmc_stm32f4.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        16.00
    @date           2018.10.16
	@brief          MMC Driver For STM32 F-4  Devices					@n
					Based on STM32F2xx_StdPeriph_Driver V1.0.0.			@n
					Based on ChaN's FatFs Sample thanks!

    @section HISTORY
		2011.10.21	V1.00	Start Here.
		2012.01.31	V2.00	Fixed Variable-Handling for DMA.
		2012.03.31  V3.00	Optimized SPI Handlings.
		2012.08.31  V4.00	Updated Support grater than 32GB Cards.
		2013.02.05	V5.00	Reduced MAXIMUM SPI Clock.
		2013.10.03	V6.00	Adopted FatFs0.10.
		2013.11.30	V7.00	Fixed CMD12 to stop data transmisson can fail.
		2014.01.15	V8.00	Adopted FatFs0.10a.
		2014.11.10	V9.00	Adopted FatFs0.10c.
		2015.01.14 V10.00	Added about setting SPI-Clock description.
		2015.02.14 V11.00	Optimized global structures to use CCRAM.
		2015.11.27 V12.00	Fixed possibly dead-lock bug on DMA-Rx.
		2016.03.31 V13.00	Added default r/w blocksize calculation.
		2016.11.30 V14.00	Fixed ACMD41 Argument to detect UHS.
		2017.12.25 V15.00	Added read S.M.A.R.T. subroutine(test use only).
		2018.10.16 V16.00	Adopted FatFs0.13c.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __MMC_STM32F4_H
#define __MMC_STM32F4_H	0x1600

#ifdef __cplusplus
 extern "C" {
#endif

/* Generic Inclusion */
#include <inttypes.h>

/* uC Related Inclusion */
#include "stm32f4xx.h"
#include "ff.h"
#include "diskio.h"

/* Defines */
#define USE_SPIMMC_DMA
/* #define USE_PWRCTRL */

/* !!Notice!! */
/*
   MAX SPI Clock frequency can take below listed for each SPI-PORT.
   But SDCard specification says "SD NomalMode supports 25MHz MAX".
   So we should set SPI clock prescaler to suitable value.
   If u need more speed,use sdio_stm32f4.c,h.
 
  *STM32F40x,41x Series(Can set 21MHz MAX for SD NomalMode)
    SPI1(APB2*84MHzMAX) : 42MHzMAX(but Prescaler set to 4(21MHz))
    SPI2(APB1*42MHzMAX) : 21MHzMAX(Prescaler set to 2)
    SPI3(APB1*42MHzMAX) : 21MHzMAX(Prescaler set to 2)
  *STM32F42x,43x Series(Can set 22.5MHz MAX for SD NomalMode)
    SPI1(APB2*90MHzMAX) : 45MHzMAX(but Prescaler set to 4(22.5MHz))
    SPI2(APB1*45MHzMAX) : 22.5MHzMAX(Prescaler set to 2)
    SPI3(APB1*45MHzMAX) : 22.5MHzMAX(Prescaler set to 2)
    SPI4(APB2*90MHzMAX) : 45MHzMAX(but Prescaler set to 4(22.5MHz))
    SPI5(APB2*90MHzMAX) : 45MHzMAX(but Prescaler set to 4(22.5MHz))
    SPI6(APB2*90MHzMAX) : 45MHzMAX(but Prescaler set to 4(22.5MHz))
*/
#if defined (USE_STM32F4DISCOVERY)
#define SPIMMC						SPI1
#define SPIMMC_MAXBAUD				SPI_BaudRatePrescaler_4
#define SPIMMC_PIN_CS				GPIO_Pin_12
#define SPIMMC_PIN_SCK				GPIO_Pin_5
#define SPIMMC_PIN_MISO    			GPIO_Pin_6
#define SPIMMC_PIN_MOSI				GPIO_Pin_7
#define SPIMMC_SRC_CS				GPIO_PinSource12
#define SPIMMC_SRC_SCK				GPIO_PinSource5
#define SPIMMC_SRC_MISO				GPIO_PinSource6
#define SPIMMC_SRC_MOSI				GPIO_PinSource7

#define SPIMMC_PORT_CS				GPIOB
#define SPIMMC_PORT_SCK				GPIOA
#define SPIMMC_PORT_MISO    		GPIOA
#define SPIMMC_PORT_MOSI			GPIOA

#define SPIMMC_CLK_CS				RCC_AHB1Periph_GPIOB
#define SPIMMC_CLK_SCK				RCC_AHB1Periph_GPIOA
#define SPIMMC_CLK_MISO    			RCC_AHB1Periph_GPIOA
#define SPIMMC_CLK_MOSI				RCC_AHB1Periph_GPIOA
 
/* STM32 SPI Peripheral Settings */
#define SPI_PERIF_CLK(x)        	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,x)
#define SPI_GPIO_CLK(x)				RCC_AHB1PeriphClockCmd(SPIMMC_CLK_CS,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_SCK,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_MISO,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_MOSI,x)

#define SPI_AF_SET()				GPIO_PinAFConfig(SPIMMC_PORT_SCK,SPIMMC_SRC_SCK,GPIO_AF_SPI1);	 \
									GPIO_PinAFConfig(SPIMMC_PORT_MISO,SPIMMC_SRC_MISO,GPIO_AF_SPI1); \
									GPIO_PinAFConfig(SPIMMC_PORT_MOSI,SPIMMC_SRC_MOSI,GPIO_AF_SPI1); \

/* STM32 SPI DMA Settings */
#define SPIMMC_DMA_STREAM_CLOCK 	RCC_AHB1Periph_DMA2
#define SPIMMC_DMA_CHANNEL_RX		DMA_Channel_3
#define SPIMMC_DMA_CHANNEL_TX		DMA_Channel_3
#define SPIMMC_DMA_STREAM_RX     	DMA2_Stream2
#define SPIMMC_DMA_STREAM_TX     	DMA2_Stream3
#define SPIMMC_DMA_FLAG_RX_TC       DMA_FLAG_TCIF2
#define SPIMMC_DMA_FLAG_TX_TC       DMA_FLAG_TCIF3
#define DMA_STREAM_IRQ           	DMA2_Stream3_IRQn

#elif defined (USE_32F429IDISCOVERY)
#define SPIMMC						SPI4
#define SPIMMC_MAXBAUD				SPI_BaudRatePrescaler_4
#define SPIMMC_PIN_CS				GPIO_Pin_4
#define SPIMMC_PIN_SCK				GPIO_Pin_2
#define SPIMMC_PIN_MISO    			GPIO_Pin_5
#define SPIMMC_PIN_MOSI				GPIO_Pin_6
#define SPIMMC_SRC_CS				GPIO_PinSource4
#define SPIMMC_SRC_SCK				GPIO_PinSource2
#define SPIMMC_SRC_MISO				GPIO_PinSource5
#define SPIMMC_SRC_MOSI				GPIO_PinSource6

#define SPIMMC_PORT_CS				GPIOE
#define SPIMMC_PORT_SCK				GPIOE
#define SPIMMC_PORT_MISO    		GPIOE
#define SPIMMC_PORT_MOSI			GPIOE

#define SPIMMC_CLK_CS				RCC_AHB1Periph_GPIOE
#define SPIMMC_CLK_SCK				RCC_AHB1Periph_GPIOE
#define SPIMMC_CLK_MISO    			RCC_AHB1Periph_GPIOE
#define SPIMMC_CLK_MOSI				RCC_AHB1Periph_GPIOE
 
/* STM32 SPI Peripheral Settings */
#define SPI_PERIF_CLK(x)        	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4,x)
#define SPI_GPIO_CLK(x)				RCC_AHB1PeriphClockCmd(SPIMMC_CLK_CS,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_SCK,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_MISO,x); 		\
									RCC_AHB1PeriphClockCmd(SPIMMC_CLK_MOSI,x)

#define SPI_AF_SET()				GPIO_PinAFConfig(SPIMMC_PORT_SCK,SPIMMC_SRC_SCK,GPIO_AF_SPI4);	 \
									GPIO_PinAFConfig(SPIMMC_PORT_MISO,SPIMMC_SRC_MISO,GPIO_AF_SPI4); \
									GPIO_PinAFConfig(SPIMMC_PORT_MOSI,SPIMMC_SRC_MOSI,GPIO_AF_SPI4); \

/* STM32 SPI DMA Settings */
#define SPIMMC_DMA_STREAM_CLOCK 	RCC_AHB1Periph_DMA2
#define SPIMMC_DMA_CHANNEL_RX		DMA_Channel_4
#define SPIMMC_DMA_CHANNEL_TX		DMA_Channel_4
#define SPIMMC_DMA_STREAM_RX     	DMA2_Stream0
#define SPIMMC_DMA_STREAM_TX     	DMA2_Stream1
#define SPIMMC_DMA_FLAG_RX_TC       DMA_FLAG_TCIF0
#define SPIMMC_DMA_FLAG_TX_TC       DMA_FLAG_TCIF1
#define DMA_STREAM_IRQ           	DMA2_Stream1_IRQn

#else
 #error "Define Target Board!!"
#endif

/* Socket Contacts is not used on this project */
#define SOCKPORT					1			/* Socket contact port 	1: NONE */
#define SOCKWP						0			/* Write protect switch	0: WP Disable */
#define SOCKINS						1			/* Card detect switch	1: SD Incerted */

/* Port Controls */
#define CS_HIGH()					SPIMMC_PORT_CS->BSRRL  = SPIMMC_PIN_CS		/* MMC CS = H */
#define CS_LOW()					SPIMMC_PORT_CS->BSRRH  = SPIMMC_PIN_CS		/* MMC CS = L */

#ifdef USE_PWRCTRL
	#warning "use MMC power control"
	#define SPIMMC_PWR              GPIOD
	#define SPIMMC_CLK_PWR 			RCC_AHB1Periph_GPIOD
	#define SPIMMC_PIN_PWR          GPIO_Pin_10
	#define SPIMMC_PWR_CLK(x)		RCC_AHB1PeriphClockCmd(SPIMMC_CLK_PWR ,x);
	#define PWR_ON()        		(SPIMMC_PWR->BSRRH = GPIO_Pin_PWR)
	#define PWR_OFF()       		(SPIMMC_PWR->BSRRL = GPIO_Pin_PWR)
	#define PWR_ISON()      		((GPIO_ReadOutputDataBit(SPIMMC, SPIMMC_PIN_PWR) == Bit_SET ) ? 0 : 1)
#else
	#warning "none MMC power control"
	#define PWR_ON()
	#define PWR_OFF()
	#define PWR_ISON()				1
#endif

/* Function Prototypes */
/* Comment/Uncomment the following line to use S.M.A.R.T. via CMD56 Command! */
/* U MUST make to work stdio functions! */
/* (test use only!) */
//#define ENABLE_SMART_READ
#define READ_SMART_CMD56	0x21 /* Get SMART(DELKIN:0x21,SANDISK:0x01) */


#ifdef __cplusplus
}
#endif

#endif /*  __MMC_STM32F4_H */
