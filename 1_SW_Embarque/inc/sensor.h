#ifndef __SENSOR_H
#define __SENSOR_H

#include "config.h"

#include <em_cmu.h>
#include <em_rmu.h>
#include <em_i2c.h>

#include <td_rtc.h>

#include "adc.h"

void TakeMeasurement(uint32_t arg, uint8_t repetition);
uint8_t BatteryStatusCode(void);
uint8_t GetValue(uint8_t nb, uint8_t type, uint8_t enPort, uint8_t enBit, float offset, float gain, uint8_t i2cAddress, uint8_t adcNb, uint32_t* raw, uint8_t i2cCommand);
void InitSensors(void);
void CalibrateSensors(uint32_t arg, uint8_t repetition);

#endif
