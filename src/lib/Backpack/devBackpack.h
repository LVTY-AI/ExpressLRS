#pragma once

#include "device.h"

void checkBackpackUpdate();
void sendCRSFTelemetryToBackpack(uint8_t *data);
void sendMAVLinkTelemetryToBackpack(uint8_t *data);
#if defined(PLATFORM_ESP32)
void forwardVideoReceiverMspToBackpack(const uint8_t *message);
#endif

extern bool HTEnableFlagReadyToSend;
extern bool BackpackTelemReadyToSend;

extern device_t Backpack_device;
