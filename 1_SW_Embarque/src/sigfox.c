#include "sigfox.h"


/***************************************************************************//**
 * @brief
 *   This function generates and sends a complete SigFox frame
 ******************************************************************************/
void SendSigfoxFrame(uint32_t arg, uint8_t repetition)
{
	uint8_t FrameStatus[8] = {0}; // All 4bit status for the frame content
	uint8_t FrameValues[8] = {0}; // All 8bit values for the frame content
	uint8_t Frame[12] = {0}; // The 12 bytes making the complete frame
	uint8_t i = 0;
	uint8_t k = 0;

	if (DEBUG)
		tfp_printf("sigfox.c: Sending frame.\r\n");

	/************** FILLING THE FRAME WITH CORRECT VALUES *****************/
	// First value is the version
	FrameStatus[0] = 2;
	FrameValues[0] = VERSION;
	// Second value is the battery status
	FrameStatus[1] = 1;
	FrameValues[1] = BatteryStatusCode();

	k = 2;
	// For all sensors, we check if there is values to send
	// If there is values to send, we add them in the frame
	if(SENSOR_1 != 0) {
		for (i=0;i<SENSOR_1_PERIOD;i++) {
			FrameStatus[k] = SENSOR_1;
			FrameValues[k] = MeasureValues[0][i];
			k++;
		}
	}
	if(SENSOR_2 != 0) {
		for (i=0;i<SENSOR_2_PERIOD;i++) {
			FrameStatus[k] = SENSOR_2;
			FrameValues[k] = MeasureValues[1][i];
			k++;
		}
	}
	if(SENSOR_3 != 0) {
		for (i=0;i<SENSOR_3_PERIOD;i++) {
			FrameStatus[k] = SENSOR_3;
			FrameValues[k] = MeasureValues[2][i];
			k++;
		}
	}

	// Format the frame to put status and values in a single table
	FormattingFrame(FrameStatus, FrameValues, Frame);

	// Send the frame over sigfox network
	TD_SIGFOX_Send(Frame, 12, 2);

	if (DEBUG)
		tfp_printf("sigfox.c: SigFox frame sent.\r\n");

	// We reset the time for measurements (index of MeasureValues table)
	MeasureTime = 0;
}


/***************************************************************************//**
 * @brief
 *   This function does bit shifting to put all status and values in a 12 bytes frame
 ******************************************************************************/
void FormattingFrame(uint8_t FrameStatus[8], uint8_t FrameValues[8], uint8_t Frame[12]) {
	uint8_t FrameStatusHex[4] = {0};
	uint8_t i = 0;

	// Moves int8 status codes to 4 bit ones for correct sending
	for (i=0;i<4;i++) {
		FrameStatusHex[i] = (FrameStatus[i*2] << 4) | (0x0F & FrameStatus[i*2+1]);
	}
	// Set up the big table
	// 1. We add the first 4 bytes for status
	for (i=0;i<4;i++) {
		Frame[i] = FrameStatusHex[i];
	}
	// 2. Then we add the 48 bytes for values
	for (i=4;i<12;i++) {
		Frame[i] = FrameValues[i-4];
	}

	if (DEBUG) {
		tfp_printf("sigfox.c: Frame is 0x");
		for(i=0;i<12;i++) {
			printf("%02x", Frame[i]);
		}
		tfp_printf("\r\n");
	}
}
