#ifndef __SIGFOX_H
#define __SIGFOX_H

#include "config.h"

#include <td_sigfox.h>
#include <em_gpio.h>

#include "sensor.h"

void SendSigfoxFrame(uint32_t arg, uint8_t repetition);
void FormattingFrame(uint8_t FrameStatus[8], uint8_t FrameValues[8], uint8_t Frame[12]);

#endif
