/*
 * Maix.h - K210-specific APIs
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

#ifndef MAIX_H
#define MAIX_H

#include <Arduino.h>

class MaixClass {
    public:
        void reset();
        void restart();

//        uint16_t getVcc();
//        uint32_t getChipId();

        uint32_t getFreeHeap();
//        uint16_t getMaxFreeBlockSize();
//        uint8_t getHeapFragmentation(); // in %
//        void getHeapStats(uint32_t* free = nullptr, uint16_t* max = nullptr, uint8_t* frag = nullptr);

//        uint32_t getFreeContStack();
//        void resetFreeContStack();

//        const char * getSdkVersion();
//        String getCoreVersion();
//        String getFullVersion();


        uint8_t getCpuFreqMHz();

        bool flashInit(uint8_t spi_index = 3, uint8_t spi_ss = 0);
        uint32_t getFlashChipId();
        uint8_t getFlashChipVendorId();

        //gets the actual chip size based on the flash id
        uint32_t getFlashChipRealSize();
        //gets the size of the flash as set by the compiler
        uint32_t getFlashChipSize();
        uint32_t getFlashChipSpeed();
//        FlashMode_t getFlashChipMode();
        uint32_t getFlashChipSizeByChipId();

        uint32_t magicFlashChipSize(uint8_t byte);
        uint32_t magicFlashChipSpeed(uint8_t byte);
//        FlashMode_t magicFlashChipMode(uint8_t byte);

        bool checkFlashConfig(bool needsEquals = false);

        bool checkFlashCRC();

        bool flashEraseSector(uint32_t sector);
        bool flashWrite(uint32_t offset, uint32_t *data, size_t size);
        bool flashRead(uint32_t offset, uint32_t *data, size_t size);

//        uint32_t getSketchSize();
//        String getSketchMD5();
//        uint32_t getFreeSketchSpace();
//        bool updateSketch(Stream& in, uint32_t size, bool restartOnFail = false, bool restartOnSuccess = true);

//        String getResetReason();
//        String getResetInfo();
//        struct rst_info * getResetInfoPtr();

//        bool eraseConfig();

#ifndef CORE_MOCK
        inline uint32_t getCycleCount() __attribute__((always_inline));
#else
        uint32_t getCycleCount();
#endif
};

#ifndef CORE_MOCK

uint32_t MaixClass::getCycleCount()
{
    return 0;
//    return get_cycle_count();
}

#endif // !defined(CORE_MOCK)

extern MaixClass Maix;

#endif //MAIX_H