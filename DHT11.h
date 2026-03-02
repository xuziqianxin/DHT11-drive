/**
  * @file    DHT11.h
  * @brief   This document contains all the function prototypes of the DHT11.c file
  * @author  虚字潜心(xu_zi_qian_xin)
  * @version 1.0.0
  * @date    2026-02-23
  * 
  * @copyright Copyright (c) 2026 虚字潜心. All rights reserved.
  * @license  SPDX-License-Identifier: MIT
  */
 
#ifndef __DHT11_H__
#define __DHT11_H__

#ifdef __cplusplus
extern "C"{
#endif
/* Includes -----------------------------------------------------------*/
#include "debug.h"
#include "ch32v30x_gpio.h"

/* Typedef ------------------------------------------------------------*/
/**
  * @breif DHT11 structure, 32-bit pointer passing the address of the IO port.
  */
typedef struct DHT11_Port
{
	uint32_t *GPIO_Port;
	uint16_t GPIO_Pin;
} DHT11_Typedef;

/**
  * @breif List of Bus Communication Errors.
  */
typedef enum 
{
	NORMAL = 0U,
	TIME_OUT_TGO,
	TIME_OUT_TREL,
	TIME_OUT_TREH,
	TIME_OUT_TLOW,
	TIME_OUT_TH,
	TIME_OUT_TEN,
	DATA_ERROR,
	LINE_ERROR
} DHT11_STATUS;

/* Define ------------------------------------------------------------*/
/**
  * @breif This parameter is used to correct the microsecond delay time. 
  * 			 When porting, modify the value to the precise time of a 1µs delay for your delay function. 
  * 			 For example, the default precise time for a 1µs delay is 1.46µs.
  */
#define US_CORRECTION_PARAMETER 1.46

/**
  * @breif The unit used to select the delay time is milliseconds or microseconds, no modification is needed.
  */
#define TIME_UNIT_US 1
#define TIME_UNIT_MS 0

/**
  * @breif This macro is used to set the specified pin to output mode, rewrite according to your platform.
  * @param devicePort IO ports used by the device
  * @param devicePinNum Pins used by the device
  */
#define __DHT11_GPIO_OUTPUT(devicePort, devicePinNum) \
do{ \
	GPIO_InitTypeDef GPIO_InitStruct = {0}; \
	GPIO_InitStruct.GPIO_Pin = devicePinNum; \
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; \
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; \
  GPIO_Init((GPIO_TypeDef *)devicePort, &GPIO_InitStruct); \
}while(0)

/**
  * @breif This macro is used to set the specified pin to input mode, rewrite according to your platform.
  * @param devicePort IO ports used by the device
  * @param devicePinNum Pins used by the device
  */
#define __DHT11_GPIO_INPUT(devicePort, devicePinNum) \
do{ \
	GPIO_InitTypeDef GPIO_InitStruct = {0}; \
	GPIO_InitStruct.GPIO_Pin = devicePinNum; \
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; \
  GPIO_Init((GPIO_TypeDef *)devicePort, &GPIO_InitStruct); \
}while(0)

/**
  * @breif This macro performs microsecond and millisecond delays, please rewrite it according to your platform when porting.
  * @param unit Select the time unit for the delay.
  */
#define Delay(unit) \
do{ \
	if(unit) \
	{ \
    Delay_Us(1); \
	} \
	else \
	{ \
		Delay_Ms(1); \
	} \
}while(0)

/**
  * @breif This macro is used for IO read and write and should be rewritten according to the specific platform.
  * @param devicePort IO ports used by the device
  * @param devicePinNum Pins used by the device
  */
#define __READ_BIT(devicePort, devicePinNum) GPIO_ReadInputDataBit((GPIO_TypeDef *)devicePort, devicePinNum)
#define __WRITE_BIT_SET(devicePort, devicePinNum) GPIO_WriteBit((GPIO_TypeDef *)devicePort, devicePinNum, Bit_SET)
#define __WRITE_BIT_RESET(devicePort, devicePinNum) GPIO_WriteBit((GPIO_TypeDef *)devicePort, devicePinNum, Bit_RESET)

/* Function ------------------------------------------------------------*/
uint32_t DHT11_Get_Value(DHT11_Typedef DHT11, double *temperature, double *humidity);

#ifdef __cplusplus
}
#endif
#endif /* __DHT11_H__ */
