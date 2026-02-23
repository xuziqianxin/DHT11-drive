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
/* IIC LIB Includes ----------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Typedef ------------------------------------------------------------*/
typedef struct DHT11_Port
{
	uint32_t *GPIO_Port;
	uint16_t GPIO_Pin;
} DHT11_Typedef;

typedef enum 
{
	NORMAL,
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
#define US_CORRECTION_PARAMETER 1.46

#define TIME_UNIT_US 1
#define TIME_UNIT_MS 0

#define NORMAL 			0
#define TIME_OUT 		1
#define DATA_ERROR 	2
#define LINE_ERROR  3

#define GPIO_OUTPUT(devicePort) \
do{ \
	GPIO_InitTypeDef GPIO_InitStruct = {0}; \
	GPIO_InitStruct.Pin = devicePort; \
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; \
  GPIO_InitStruct.Pull = GPIO_NOPULL; \
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; \
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); \
}while(0)

#define GPIO_INPUT(devicePort) \
do{ \
	GPIO_InitTypeDef GPIO_InitStruct = {0}; \
	GPIO_InitStruct.Pin = devicePort; \
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; \
  GPIO_InitStruct.Pull = GPIO_NOPULL; \
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); \
}while(0)

#define Delay(unit) \
do{ \
	if(unit) \
	{ \
		uint32_t ticks = SystemCoreClock / 1000000UL; \
		uint32_t reload = SysTick->LOAD; \
		uint32_t start  = SysTick->VAL; \
		uint32_t elapsed = 0; \
		while (elapsed < ticks) \
		{ \
			uint32_t current = SysTick->VAL; \
			if(current <= start) \
			{ \
					elapsed += start - current; \
			} \
			else \
			{ \
					elapsed += reload - current + start; \
			} \
			start = current; \
		} \
	} \
	else \
	{ \
		HAL_Delay(0); \
	} \
}while(0)

#define __READ_BIT(devicePort, devicePinNum) HAL_GPIO_ReadPin((GPIO_TypeDef *)devicePort, devicePinNum)
#define __WRITE_BIT_SET(devicePort, devicePinNum) HAL_GPIO_WritePin((GPIO_TypeDef *)devicePort, devicePinNum, GPIO_PIN_SET)
#define __WRITE_BIT_RESET(devicePort, devicePinNum) HAL_GPIO_WritePin((GPIO_TypeDef *)devicePort, devicePinNum, GPIO_PIN_RESET)

/* Function ------------------------------------------------------------*/
uint32_t DHT11_Get_Value(DHT11_Typedef DHT11, double *temperature, double *humidity);

#ifdef __cplusplus
}
#endif
#endif /* __DHT11_H__ */
