/**
 * @file    DHT11.c
 * @brief   This file provides code for initializing and using the functions of the DHT11.
 * @author  虚字潜心(xu_zi_qian_xin)
 * @version 1.0.0
 * @date    2026-02-23
 * 
 * @copyright Copyright (c) 2026 虚字潜心. All rights reserved.
 * @license  SPDX-License-Identifier: MIT
 */

#include "DHT11.h"

/**
	* @breif Used to convert the BCD code sent by the device into an integer value.
	* @param bcd 1-byte data sent to the device as a parameter.
	*/
static inline uint8_t BCD_To_Int(uint8_t bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0xFF);
}

/**
	* @breif Used to convert BCD codes sent by the device into floating-point values
	* @param bcd 1-byte data sent to the device as a parameter.
	*/
static inline double BCD_To_Float(uint8_t bcd)
{
	uint8_t value = BCD_To_Int(bcd);
	return value * ((value < 10) ? 0.1f : 0.01f);
}

/**
	* @breif Get the temperature and humidity values from the DHT11.
	* @param DHT11 Device structure.
	* @param temperature Address of the temperature variable.
	* @param humidityAddress of the humidity variable.
	*/
uint32_t DHT11_Get_Value(DHT11_Typedef DHT11, double *temperature, double *humidity)
{
	uint8_t data[5] = { 0 };
	uint8_t usCnt = 0;
	uint8_t bitCnt = 0;
	
	/* Send initialization signal */
	__DHT11_GPIO_OUTPUT(DHT11.GPIO_Port, DHT11.GPIO_Pin);
	__WRITE_BIT_SET(DHT11.GPIO_Port, DHT11.GPIO_Pin);
	Delay(TIME_UNIT_MS);
	__WRITE_BIT_RESET(DHT11.GPIO_Port, DHT11.GPIO_Pin);
	for(uint8_t i = 0; i < 20; i++)
	{
		Delay(TIME_UNIT_MS);
	}
	__WRITE_BIT_SET(DHT11.GPIO_Port, DHT11.GPIO_Pin);
	
	/* Waiting for device response */
	__DHT11_GPIO_INPUT(DHT11.GPIO_Port, DHT11.GPIO_Pin);
	while(__READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin))
	{
		for(uint8_t i = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 1; i++)
		{
			Delay(TIME_UNIT_US);
			if(i > 13)
			{
				return TIME_OUT_TGO;
			}
		}
	}
	
	/* Check response signal */
	for(uint8_t i = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 0; i++)
	{
		Delay(TIME_UNIT_US);
		if(i > 85)
		{
			return TIME_OUT_TREL;
		}
	}
	for(uint8_t i = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 1; i++)
	{
		Delay(TIME_UNIT_US);
		if(i > 88)
		{
			return TIME_OUT_TREH;
		}
	}
	
	/* Read Data */
	while(1)
	{
		for(uint8_t i = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 0; i++)
		{
			Delay(TIME_UNIT_US);
			if(i > 60)
			{
				return TIME_OUT_TLOW;
			}
		}
		for(usCnt = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 1; usCnt++)
		{
			Delay(TIME_UNIT_US);
			if(usCnt >= 80)
			{
				return TIME_OUT_TH;
			}
		}
		if((usCnt >= (23 / US_CORRECTION_PARAMETER)) && ( usCnt <= 30))
		{
			data[bitCnt / 8] <<= 1;
			data[bitCnt / 8] = data[bitCnt / 8] + 0;
			bitCnt++;
		}
		else if((usCnt >= (68 / US_CORRECTION_PARAMETER)) && (usCnt <= 80))
		{
			data[bitCnt / 8] <<= 1;
			data[bitCnt / 8] = data[bitCnt / 8] + 1;
			bitCnt++;
		}
		
		if(bitCnt == 40)
		{
			break;
		}
	}
	
	/* Check Completion Signal */
	for(uint8_t i = 0; __READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 0; i++)
	{
		Delay(TIME_UNIT_US);
		if(i > 56)
		{
			return TIME_OUT_TEN;
		}
	}
	
	/* Process data after the bus is idle */
	if(__READ_BIT(DHT11.GPIO_Port, DHT11.GPIO_Pin) == 1)
	{
		if((uint8_t)(data[0] + data[1] + data[2] + data[3]) == data[4])
		{
			*humidity = BCD_To_Int(data[0]) + BCD_To_Float(data[1]);
			if((data[3] & 0x80) == 1)
			{
				data[3] = data[3] | 0x80;
				*temperature = -(BCD_To_Int(data[2]) + BCD_To_Float(data[3]));
			}
			else
			{
				*temperature = BCD_To_Int(data[2]) + BCD_To_Float(data[3]);
			}
		}
		else
		{
			return DATA_ERROR;
		}
		return NORMAL;
	}
	else
	{
		return LINE_ERROR;
	}
}
