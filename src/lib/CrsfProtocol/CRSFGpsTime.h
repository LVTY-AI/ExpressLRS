#pragma once

#include <stdint.h>

#include "CRSF.h"

/**
 * Advance a serialised CRSF GPS_TIME frame by the time it spent in transit.
 * The frame's date is intentionally left untouched; the bounded OTA delay
 * correction only needs to carry milliseconds through seconds and minutes.
 */
static inline void crsfGpsTimeAdvanceMs(uint8_t *buf, uint16_t elapsedMs)
{
    if (elapsedMs == 0)
        return;

    uint16_t milliseconds = ((uint16_t)buf[10] << 8) | buf[11];
    milliseconds += elapsedMs;
    buf[10] = (milliseconds % 1000) >> 8;
    buf[11] = milliseconds % 1000;

    const uint8_t carrySeconds = milliseconds / 1000;
    if (carrySeconds == 0)
        return;

    buf[9] += carrySeconds;
    if (buf[9] < 60)
        return;

    buf[8] += buf[9] / 60;
    buf[9] %= 60;
    if (buf[8] < 60)
        return;

    buf[7] += buf[8] / 60;
    buf[8] %= 60;
}

static inline void crsfRecalcCrc(uint8_t *buf)
{
    const uint8_t payloadLength = buf[CRSF_TELEMETRY_LENGTH_INDEX];
    buf[CRSF_FRAME_NOT_COUNTED_BYTES + payloadLength - 1] =
        crsf_crc.calc(&buf[CRSF_TELEMETRY_TYPE_INDEX], payloadLength - CRSF_TELEMETRY_CRC_LENGTH, 0);
}
