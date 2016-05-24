/******************************************************************************
 * @file
 * @brief Mr Meteo.
 * @author Naio Technologies
 * @version 1
******************************************************************************/

#include "config.h"

#include <td_rtc.h>

#include "adc.h"
#include "sensor.h"
#include "sigfox.h"


/* This file declare all "dynamic" library data. It should be last included file
 * Standard size value can be override before including this file
 */
#include <td_config.h>


/*******************************************************************************
 ***************************  GLOBAL VARIABLES  ********************************
 ******************************************************************************/

uint8_t MeasureTime = 0;
uint8_t MeasureValues[3][12];

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   User setup function.
 ******************************************************************************/
void TD_USER_Setup(void)
{
	if (DEBUG) {
		// Disable RX pin that is used for ADC conversion
		init_printf(TD_UART_Init(9600, false, false),
			TD_UART_Putc,
			TD_UART_Start,
			TD_UART_Stop);
		tfp_printf("mrmeteo.: Start\r\n");
	}

	// Initialize all sensors
	InitSensors();

	// If we are in the production mode, the tasks are initiated
	if (!CALIB) {
		TD_SCHEDULER_Append(1*60, 0, 0, TD_SCHEDULER_INFINITE, TakeMeasurement, 0);
		// The Sigfox frame is sent every 12min with an offset of 30s to avoid perturbation on measures
		TD_SCHEDULER_Append(12*60, 0, 30, TD_SCHEDULER_INFINITE, SendSigfoxFrame, 0);
	}
	// Else we start the calibration task
	else {
		TD_SCHEDULER_Append(1, 0, 0, TD_SCHEDULER_INFINITE, CalibrateSensors, 0);
	}
}

/***************************************************************************//**
 * @brief
 *   User loop function.
 ******************************************************************************/
void TD_USER_Loop(void)
{
}
