#include "crc.h"

uint32_t crc32_le(uint32_t crc, uint8_t const *buf, uint32_t len)
{
//    uint32_t crc = 0xFFFFFFFF;
    crc = 0xFFFFFFFF;
    uint32_t byte, mask;
    // Read from flash and update CRC32 value
    for (uint32_t n = 0; n < len; n++) {
        byte = buf[n];
        crc = crc ^ byte;
        for (uint32_t j = 0; j < 8; j++) {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}
