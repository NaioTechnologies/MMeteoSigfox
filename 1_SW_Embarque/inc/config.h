#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include <efm32.h>
#include <td_core.h>

#include <em_gpio.h>

#include <td_uart.h>
#include <td_printf.h>


/*******************************************************************************
 ********************************  CONFIG  *************************************
 ******************************************************************************/

#define VERSION 2 // An integer between 1 and 255
#define CALIB 0 // Boolean to go into calibration mode;
#define DEBUG 0 // !! WARNING, TX pin cannot be used at the same time as DEBUG mode !!

#define BATTERY_ADC_NB 7 // ADC number for battery calculation
#define BATTERY_EN_BIT USR1_BIT // Battery calculation Enable pin
#define BATTERY_EN_PORT USR1_PORT // Battery calculation Enable port
#define BATTERY_BIT TIM2_BIT // Battery calculation Enable pin
#define BATTERY_PORT TIM2_PORT // Battery calculation Enable port
#define BATTERY_LIMIT 3.5 // Battery limit (float between 0 and 6.6)

// Some notes about sensors:
// * Voltage : a delay of XXms is there to wait for sensor to be powered up, change it according to real use
// * I2C : a delay of 15ms is there to wait for sensor to be powered up, change it according to real use

#define SENSOR_1 10 // 0 for nothing, 10 for air temp, 11 for soil temp, 12 for air humidity, 13 for soil moisture
#define SENSOR_1_TYPE 3 // 1 for resistive, 2 for voltage, 3 for I2C, 4 for UART, 5 for soil moisture sensor
#define SENSOR_1_PERIOD 2 // 1 for 12min, 2 for 6min, 3 for 4min, 4 for 3min, 6 for 2min
#define SENSOR_1_EN_PORT USR3_PORT // Needed if SENSOR_1_TYPE = 1 or 2 only
#define SENSOR_1_EN_BIT USR3_BIT // Needed if SENSOR_1_TYPE = 1 or 2 only
#define SENSOR_1_PORT 0 // Needed if SENSOR_1_TYPE = 1 or 2 only
#define SENSOR_1_BIT 0 // Needed if SENSOR_1_TYPE = 1 or 2 only
#define SENSOR_1_OFFSET -46.85*4+10*4 // Offset of the curve (integer)
#define SENSOR_1_GAIN 175.72*4 // Gain of the curve for a maximum at 3.3V (float)
#define SENSOR_1_I2C_COMMAND 0xe3
#define SENSOR_1_I2C_ADRESS 0x40
#define SENSOR_1_ADC_NB 6 // ADC number
/* TODO ON SENSOR
 *  - Implement Resistive
 *  - Implement UART
 */

#define SENSOR_2 12 // 0 for nothing, 10 for air temp, 11 for soil temp, 12 for air humidity, 13 for soil moisture
#define SENSOR_2_TYPE 3 // 1 for resistive, 2 for voltage, 3 for I2C, 4 for UART, 5 for soil moisture sensor
#define SENSOR_2_PERIOD 2 // 1 for 12min, 2 for 6min, 3 for 4min, 4 for 3min, 6 for 2min
#define SENSOR_2_EN_PORT USR3_PORT // Needed if SENSOR_2_TYPE = 1 or 2 only
#define SENSOR_2_EN_BIT USR3_BIT // Needed if SENSOR_2_TYPE = 1 or 2 only
#define SENSOR_2_PORT 0 // Needed if SENSOR_2_TYPE = 1 or 2 only
#define SENSOR_2_BIT 0 // Needed if SENSOR_2_TYPE = 1 or 2 only
#define SENSOR_2_OFFSET -6*2 // Offset of the curve (integer)
#define SENSOR_2_GAIN 125*2 // Gain of the curve for a maximum at 3.3V (float)
#define SENSOR_2_I2C_COMMAND 0xe5
#define SENSOR_2_I2C_ADRESS 0x40
#define SENSOR_2_ADC_NB 7 // ADC number

#define SENSOR_3 13 // 0 for nothing, 10 for air temp, 11 for soil temp, 12 for air humidity, 13 for soil moisture
#define SENSOR_3_TYPE 2 // 1 for resistive, 2 for voltage, 3 for I2C, 4 for UART, 5 for soil moisture sensor
#define SENSOR_3_PERIOD 2 // 1 for 12min, 2 for 6min, 3 for 4min, 4 for 3min, 6 for 2min
#define SENSOR_3_EN_PORT USR3_PORT // Needed if SENSOR_3_TYPE = 1 or 2 only
#define SENSOR_3_EN_BIT USR3_BIT // Needed if SENSOR_3_TYPE = 1 or 2 only
#define SENSOR_3_PORT ADC0_PORT // Needed if SENSOR_3_TYPE = 1 or 2 only
#define SENSOR_3_BIT ADC0_BIT // Needed if SENSOR_3_TYPE = 1 or 2 only
#define SENSOR_3_OFFSET 0 // Offset of the curve (integer)
#define SENSOR_3_GAIN 1.1428571429 // Gain of the curve for a maximum at 3.3V (float)
#define SENSOR_3_I2C_COMMAND 1
#define SENSOR_3_I2C_ADRESS 1
#define SENSOR_3_ADC_NB 6 // ADC number

#if SENSOR_1_PERIOD + SENSOR_2_PERIOD + SENSOR_3_PERIOD > 6
	#error Cannot send more than 6 messages per frame
#endif


/*******************************************************************************
 ***************************  GLOBAL VARIABLES  ********************************
 ******************************************************************************/

extern uint8_t MeasureTime; // The time between last sigfox frame and current measurement
extern uint8_t MeasureValues[3][12]; // The time between last sigfox frame and current measurement


#endif
