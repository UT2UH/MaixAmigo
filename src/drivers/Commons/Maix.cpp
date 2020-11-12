/*
 * Maix.cpp - K210-specific APIs
 * Copyright 2020 fukuen.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Maix.h"
#include "w25qxx.h"

/**
 * User-defined Literals
 *  usage:
 *
 *   uint32_t = test = 10_MHz; // --> 10000000
 */

unsigned long long operator"" _kHz(unsigned long long x) {
    return x * 1000;
}

unsigned long long operator"" _MHz(unsigned long long x) {
    return x * 1000 * 1000;
}

unsigned long long operator"" _GHz(unsigned long long x) {
    return x * 1000 * 1000 * 1000;
}

unsigned long long operator"" _kBit(unsigned long long x) {
    return x * 1024;
}

unsigned long long operator"" _MBit(unsigned long long x) {
    return x * 1024 * 1024;
}

unsigned long long operator"" _GBit(unsigned long long x) {
    return x * 1024 * 1024 * 1024;
}

unsigned long long operator"" _kB(unsigned long long x) {
    return x * 1024;
}

unsigned long long operator"" _MB(unsigned long long x) {
    return x * 1024 * 1024;
}

unsigned long long operator"" _GB(unsigned long long x) {
    return x * 1024 * 1024 * 1024;
}


MaixClass Maix;

uint8_t MaixClass::getCpuFreqMHz(void) {
    return sysctl_clock_get_freq(SYSCTL_CLOCK_CPU) / 1000000;
}

bool MaixClass::flashInit(uint8_t spi_index, uint8_t spi_ss) {
    int rc = w25qxx_init(3, 0); // SPI3
//    int rc = w25qxx_init(3, 0, SPI_STAND_CLOCK_RATE); // SPI3
    return rc == 0;
}

bool MaixClass::flashEraseSector(uint32_t sector) {
    int rc = w25qxx_sector_erase(sector * w25qxx_FLASH_SECTOR_SIZE);
    return rc == 0;
}

bool MaixClass::flashRead(uint32_t offset, uint32_t *data, size_t size) {
    auto rc = w25qxx_read_data(offset, (uint8_t *) data, (uint32_t) size, W25QXX_STANDARD);
//    auto rc = w25qxx_read_data(offset, (uint8_t *) data, (uint32_t) size);
    return rc == W25QXX_OK;
}

bool MaixClass::flashWrite(uint32_t offset, uint32_t *data, size_t size) {
    auto rc = w25qxx_write_data(offset, (uint8_t *) data, (uint32_t) size);
    return rc == W25QXX_OK;
}

uint32_t MaixClass::getFreeHeap(void)
{
    return (uint32_t) get_free_heap_size();
}

void MaixClass::reset(void)
{
    sysctl_reset(SYSCTL_RESET_SOC);
}

void MaixClass::restart(void)
{
    sysctl_reset(SYSCTL_RESET_SOC);
}
