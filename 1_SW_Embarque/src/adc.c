#include <em_cmu.h>
#include <em_adc.h>

#include "adc.h"

/***************************************************************************//**
 * @brief
 *   Configures tha ADC for a single measure over selected channel and return raw value
 ******************************************************************************/
uint32_t MeasureADC(uint8_t chan)
{
	uint32_t setpoint;

	/* Base the ADC configuration on the default setup. */
	ADC_InitSingle_TypeDef single_init = ADC_INITSINGLE_DEFAULT;
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;

	/* Initialize timebase */
	init.timebase = ADC_TimebaseCalc(0);
	init.prescale = ADC_PrescaleCalc(40000, 0);
	init.warmUpMode = adcWarmupKeepADCWarm;

	CMU_ClockEnable(cmuClock_ADC0, true);
	ADC_Init(ADC0, &init);

	/* Set input of the selected channel. Reference must be 3.3V */
	single_init.reference = adcRefVDD;
	switch (chan) {
		case 1:
			single_init.input = adcSingleInpCh1;
			break;
		case 2:
			single_init.input = adcSingleInpCh2;
			break;
		case 3:
			single_init.input = adcSingleInpCh3;
			break;
		case 4:
			single_init.input = adcSingleInpCh4;
			break;
		case 5:
			single_init.input = adcSingleInpCh5;
			break;
		case 6:
			single_init.input = adcSingleInpCh6;
			break;
		case 7:
			single_init.input = adcSingleInpCh7;
			break;
		default:
			single_init.input = adcSingleInpCh6;
			break;
	}
	single_init.rep = false; // no auto rearm
	ADC_InitSingle(ADC0, &single_init);

	// Start one ADC sample
	ADC_Start(ADC0, adcStartSingle);

	// Active wait for ADC to complete
	while ((ADC0->STATUS & ADC_STATUS_SINGLEDV) == 0) {
		;
	}
	setpoint = ADC_DataSingleGet(ADC0);

	CMU_ClockEnable(cmuClock_ADC0, false);
	return(setpoint);
}
