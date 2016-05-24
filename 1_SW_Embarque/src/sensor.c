#include "sensor.h"


/***************************************************************************//**
 * @brief
 *   Function called every minutes, where all test are done to do measurements we it's appropriate
 *   Then it launches the measures and stores them in MeasureValues[][]
 ******************************************************************************/
void TakeMeasurement(uint32_t arg, uint8_t repetition)
{
	bool sent = false; // Used for debug purpose, to know if a measure as been taken
	uint32_t raw = 0; // Used for debug purposes, contains raw values of measurements before conversion

	if (DEBUG) {
		tfp_printf("sensor.c: Measures for minute %d:\r\n", MeasureTime);
	}

	// For each sensor, if the sensor is set up we start the conversion
	if(SENSOR_1 != 0 && SENSOR_1_PERIOD != 0) {
		// If it's time to take a measure, we take it
		if (MeasureTime % (12 / SENSOR_1_PERIOD) == 0) {
			// We take the value for sensor 1, it returns the converted value
			MeasureValues[0][(MeasureTime*SENSOR_1_PERIOD)/12] = GetValue(1,SENSOR_1_TYPE,SENSOR_1_EN_PORT,SENSOR_1_EN_BIT,SENSOR_1_OFFSET,SENSOR_1_GAIN,SENSOR_1_I2C_ADRESS,SENSOR_1_ADC_NB,&raw,SENSOR_1_I2C_COMMAND);
			if (DEBUG)
				tfp_printf("sensor.c:  * Measure taken, Sensor 1, Period %d min, Value is %d.\r\n", 12/SENSOR_1_PERIOD, MeasureValues[0][(MeasureTime*SENSOR_1_PERIOD)/12]);
			sent = true;
		}
	}
	if(SENSOR_2 != 0 && SENSOR_2_PERIOD != 0) {
		if (MeasureTime % (12 / SENSOR_2_PERIOD) == 0) {
			MeasureValues[1][(MeasureTime*SENSOR_2_PERIOD)/12] = GetValue(2,SENSOR_2_TYPE,SENSOR_2_EN_PORT,SENSOR_2_EN_BIT,SENSOR_2_OFFSET,SENSOR_2_GAIN,SENSOR_2_I2C_ADRESS,SENSOR_2_ADC_NB,&raw,SENSOR_2_I2C_COMMAND);
			if (DEBUG)
				tfp_printf("sensor.c:  * Measure taken, Sensor 2, Period %d min, Value is %d.\r\n", 12/SENSOR_2_PERIOD, MeasureValues[1][(MeasureTime*SENSOR_2_PERIOD)/12]);
			sent = true;
		}
	}
	if(SENSOR_3 != 0 && SENSOR_3_PERIOD != 0) {
		if (MeasureTime % (12 / SENSOR_3_PERIOD) == 0) {
			MeasureValues[2][(MeasureTime*SENSOR_3_PERIOD)/12] = GetValue(3,SENSOR_3_TYPE,SENSOR_3_EN_PORT,SENSOR_3_EN_BIT,SENSOR_3_OFFSET,SENSOR_3_GAIN,SENSOR_3_I2C_ADRESS,SENSOR_3_ADC_NB,&raw,SENSOR_3_I2C_COMMAND);
			if (DEBUG)
				tfp_printf("sensor.c:  * Measure taken, Sensor 3, Period %d min, Value is %d.\r\n", 12/SENSOR_2_PERIOD, MeasureValues[2][(MeasureTime*SENSOR_3_PERIOD)/12]);
			sent = true;
		}
	}

	if (DEBUG && sent == false)
		tfp_printf("sensor.c:  * None.\r\n");

	// We update the measure time used for MeasureValues[][] indexing
	MeasureTime++;
}


/***************************************************************************//**
 * @brief
 *   Take a measure of the battery voltage, do conversion and compare it with a threshold
 *   Returns 1 if low level else 2
 ******************************************************************************/
uint8_t BatteryStatusCode(void)
{
	uint32_t battery = 0;
	float battery_f = 0;
	float limit_f = BATTERY_LIMIT;
	float temp_f = 0;

	uint8_t battery_unit;
	uint32_t battery_decimal;
	uint8_t limit_unit;
	uint32_t limit_decimal;

	GPIO_PinOutSet(BATTERY_EN_PORT, BATTERY_EN_BIT);

	// Wait for the 100nF capacitor to be stable
	TD_RTC_Delay(T50MS); // Tau = R*C = 50k*100n = 5ms

	// Take value from ADC (ref 3.3V)
	battery = MeasureADC(BATTERY_ADC_NB);
	// Convert value to volts
	battery_f = ((battery*2)*3.3)/4096-0.020;
	// Battery measure is 5mV accurate

	GPIO_PinOutClear(BATTERY_EN_PORT, BATTERY_EN_BIT);

	if (DEBUG) {
		battery_unit = battery_f;
		temp_f = battery_f*1000;
		battery_decimal = temp_f - battery_unit*1000;
		limit_unit = limit_f;
		temp_f = limit_f*1000;
		limit_decimal = temp_f - limit_unit*1000;

		tfp_printf("sensor.c: Battery level measured is %d.%03dV with a limit of %d.%03dV.\r\n", battery_unit, battery_decimal, limit_unit, limit_decimal);
	}

	// Compare value with limit and return appropriate flag
	if (battery_f > limit_f) {
		return 2; // Battery level ok
	}
	else {
		return 1; // Battery low level
	}
}


/***************************************************************************//**
 * @brief
 *   Ask the ADC to take a value and convert it to a byte according to the sensor type
 ******************************************************************************/
uint8_t GetValue(uint8_t nb, uint8_t type, uint8_t enPort, uint8_t enBit, float offset, float gain, uint8_t i2cAddress, uint8_t adcNb, uint32_t* raw, uint8_t i2cCommand)
{
	uint8_t value=0; // Returned value
	uint32_t rawValue=0; // Raw value from ADC
	float rawValueVolts=0;
	float rawValuekOhms=0;
	float temp=24; // Current soil temperature used for tests on the WaterMark soil moisture sensor

	// Depending of the type of sensor
	switch (type) {
		case 1: // Resistive
			value = 0;
			break;
		case 2: // Voltage
			// Power up the sensor
			GPIO_PinOutSet(enPort, enBit);
			// Wait random delay for the value to stabilize
			TD_RTC_Delay(T50MS);
			// Get the value from ADC
			rawValue = MeasureADC(adcNb);
			// Power down the sensor to save energy!
			GPIO_PinOutClear(enPort, enBit);
			// Output the raw value for debug purposes
			*raw = rawValue;
			// Conversion from 0-4096 to 0-255
			rawValueVolts = (float)rawValue;
			rawValueVolts = (rawValueVolts+offset)*gain;
			rawValueVolts = rawValueVolts/16;
			value = (uint8_t)rawValueVolts;
			break;
		case 3: // I2C
			// Power up
			GPIO_PinOutSet(enPort, enBit);
			// Wait sensor to be powered up
			TD_RTC_Delay(T15MS); // From SHT21 datasheet, worst case delay

			I2C_TransferSeq_TypeDef i2cTransfert;
			uint8_t values[3] = {0,0,0};

		    values[0] = i2cCommand;
		    i2cTransfert.addr = i2cAddress << 1;
		    i2cTransfert.flags = I2C_FLAG_WRITE_WRITE;
		    i2cTransfert.buf[0].len = 1;
		    i2cTransfert.buf[0].data = &values[0];
		    int _status = I2C_TransferInit(I2C0, &i2cTransfert);
		    while ( _status == i2cTransferInProgress) {
		      _status = I2C_Transfer(I2C0);
		    }


			// Prepare for a read of 2 bytes
			i2cTransfert.addr = i2cAddress << 1;     // Chip address << 1
			i2cTransfert.flags = I2C_FLAG_READ;     // READ Operation
			i2cTransfert.buf[0].data = values;      // buffer
			i2cTransfert.buf[0].len = 3;            // Read 3 bytes


			// Start I2C read
			_status = I2C_TransferInit(I2C0, &i2cTransfert);
			while ( _status == i2cTransferInProgress) {
			      _status = I2C_Transfer(I2C0);
			}

			// Power down the sensor to save energy!
			GPIO_PinOutClear(enPort, enBit);

			*raw = (values[1] && 0xfc) | (values[0] << 8);
			float tempe_f = (float)*raw / 65536 * gain + offset;
			value = tempe_f;

			//tfp_printf("sensor.c: I2C: %d.\r\n", tempe);
			break;
		case 4: // UART
			value = 0;
			break;
//		case 5: // Soil Moisture
//			// TODO : Fix or remove
//			GPIO_PinOutSet(enPort, enBit);
//			TD_RTC_Delay(T500MS);
//			rawValue = MeasureADC(adcNb);
//			GPIO_PinOutClear(enPort, enBit);
//			*raw = rawValue;
//
//			rawValueVolts = rawValue*3.3/4096;
//			rawValuekOhms = -33000 / (rawValueVolts-3.3);
//			rawValuekOhms = rawValuekOhms/1000;
//
//			if(rawValuekOhms < 0.750) {
//				rawValueVolts = 20*(rawValuekOhms*(1+0.018*(temp-24))-0.55);
//			} else if (rawValuekOhms > 70) {
//				rawValueVolts = 256;
//			} else {
//				rawValueVolts = 2.246 + 5.239 * rawValuekOhms * (1+0.018*(temp-24)) + 0.06756 * rawValuekOhms*rawValuekOhms * (1+0.018*(temp-24))*(1+0.018*(temp-24));
//			}
//
//			if (rawValueVolts > 256)
//				rawValueVolts = 255;
//			else if (rawValueVolts < 0)
//				rawValueVolts = 0;
//
//			value = (uint8_t)rawValueVolts;
//			break;
		default:
			value = 0;
			break;
	}

	return value;
}


/***************************************************************************//**
 * @brief
 *   Initialize the sensors I/O by settuping them
 ******************************************************************************/
void InitSensors(void) {
	// Set up the battery measure pin as input
	GPIO_PinModeSet(BATTERY_EN_PORT, BATTERY_EN_BIT, gpioModePushPull, 1);
	GPIO_PinModeSet(BATTERY_PORT, BATTERY_BIT, gpioModeInput, 1);

	// For each sensor, set up the EN pin as output and the reading pin as input
	// Only if it uses the ADC
	if(SENSOR_1 && (SENSOR_1_TYPE == 1 || SENSOR_1_TYPE == 2 || SENSOR_1_TYPE == 5)) {
		GPIO_PinModeSet(SENSOR_1_EN_PORT, SENSOR_1_EN_BIT, gpioModePushPull, 1);
		GPIO_PinModeSet(SENSOR_1_PORT, SENSOR_1_BIT, gpioModeInput, 1);
	}
	if(SENSOR_2 && (SENSOR_2_TYPE == 1 || SENSOR_2_TYPE == 2 || SENSOR_2_TYPE == 5)) {
		GPIO_PinModeSet(SENSOR_2_EN_PORT, SENSOR_2_EN_BIT, gpioModePushPull, 1);
		GPIO_PinModeSet(SENSOR_2_PORT, SENSOR_2_BIT, gpioModeInput, 1);
	}
	if(SENSOR_3 && (SENSOR_3_TYPE == 1 || SENSOR_3_TYPE == 2 || SENSOR_3_TYPE == 5)) {
		GPIO_PinModeSet(SENSOR_3_EN_PORT, SENSOR_3_EN_BIT, gpioModePushPull, 1);
		GPIO_PinModeSet(SENSOR_3_PORT, SENSOR_3_BIT, gpioModeInput, 1);
	}
	// For each sensor, set up the EN pin as output and the reading pin as input
	// Only if it uses the ADC
	if((SENSOR_1 && SENSOR_1_TYPE == 3) || (SENSOR_2 && SENSOR_2_TYPE == 3) || (SENSOR_3 && SENSOR_3_TYPE == 3)) {
		// Configure needed clock to have I2C working
		CMU_ClockEnable(cmuClock_GPIO, true);
		CMU_ClockEnable(cmuClock_I2C0, true);

		// Configure I/O pins for working as I2C - Output Open Drain
		GPIO_PinModeSet(SCL_PORT, SCL_BIT,gpioModeWiredAnd,1);
		GPIO_PinModeSet(SDA_PORT, SDA_BIT,gpioModeWiredAnd,1);
		I2C0->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | _I2C_ROUTE_LOCATION_LOC0;

		// Init the I2C with default param 100KHz
		I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
		I2C_Init(I2C0, &i2cInit);
	}

	if (DEBUG)
		tfp_printf("sensor.c: Sensors I/O initialized\r\n");
}


/***************************************************************************//**
 * @brief
 *   Task called every second when we are in calibration mode
 *   Simply takes values from ADC and display them in different formats
 ******************************************************************************/
void CalibrateSensors(uint32_t arg, uint8_t repetition)
{
	uint32_t raw=0;

	int8_t deg_unit;
	uint32_t deg_decimal;
	int8_t hum_unit;
	uint32_t hum_decimal;

	int32_t temp;

	// For each sensor, if it is set up
	if(SENSOR_1 != 0) {
		// Take the measure
		MeasureValues[0][0] = GetValue(1,SENSOR_1_TYPE,SENSOR_1_EN_PORT,SENSOR_1_EN_BIT,SENSOR_1_OFFSET,SENSOR_1_GAIN,SENSOR_1_I2C_ADRESS,SENSOR_1_ADC_NB, &raw,SENSOR_1_I2C_COMMAND);
		if(DEBUG) {
			// Convert the value to many different formats (Celsius, RH...)
			temp = MeasureValues[0][0]*100;
			temp = (temp-4000)/4;
			deg_unit = temp/100;
			deg_decimal = temp - deg_unit*100;

			temp = MeasureValues[0][0]*100;
			temp = (temp)/2;
			hum_unit = temp/100;
			hum_decimal = temp - hum_unit*100;
			// Display all the values over UART
			tfp_printf("sensor.c: Sensor 1, raw value %d, value is %d (%d.%02d deg OR %d.%02d hum).\r\n", raw, MeasureValues[0][0], deg_unit, deg_decimal, hum_unit, hum_decimal);
		}
	}
	if(SENSOR_2 != 0) {
		MeasureValues[1][0] = GetValue(2,SENSOR_2_TYPE,SENSOR_2_EN_PORT,SENSOR_2_EN_BIT,SENSOR_2_OFFSET,SENSOR_2_GAIN,SENSOR_2_I2C_ADRESS,SENSOR_2_ADC_NB, &raw,SENSOR_2_I2C_COMMAND);
		if(DEBUG) {
			temp = MeasureValues[1][0]*100;
			temp = (temp-4000)/4;
			deg_unit = temp/100;
			deg_decimal = temp - deg_unit*100;

			temp = MeasureValues[1][0]*100;
			temp = (temp)/2;
			hum_unit = temp/100;
			hum_decimal = temp - hum_unit*100;
			tfp_printf("sensor.c: Sensor 2, raw value %d, value is %d (%d.%02d deg OR %d.%02d hum).\r\n", raw, MeasureValues[1][0], deg_unit, deg_decimal, hum_unit, hum_decimal);
		}
	}
	if(SENSOR_3 != 0) {
		MeasureValues[2][0] = GetValue(3,SENSOR_3_TYPE,SENSOR_3_EN_PORT,SENSOR_3_EN_BIT,SENSOR_3_OFFSET,SENSOR_3_GAIN,SENSOR_3_I2C_ADRESS,SENSOR_3_ADC_NB, &raw,SENSOR_3_I2C_COMMAND);
		if(DEBUG) {
			temp = MeasureValues[2][0]*100;
			temp = (temp-4000)/4;
			deg_unit = temp/100;
			deg_decimal = temp - deg_unit*100;

			temp = MeasureValues[2][0]*100;
			temp = (temp)/2;
			hum_unit = temp/100;
			hum_decimal = temp - hum_unit*100;
			tfp_printf("sensor.c: Sensor 3, raw value %d, value is %d (%d.%02d deg OR %d.%02d hum).\r\n", raw, MeasureValues[2][0], deg_unit, deg_decimal, hum_unit, hum_decimal);
		}
	}
	BatteryStatusCode();
}
