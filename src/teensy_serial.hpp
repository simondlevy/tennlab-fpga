/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <serial.hpp>

namespace neuro {

    class TeensySerial : public neuro::Serial {

        private:

            static const uint32_t kBaudRate = 4'000'000;

        public:

            TeensySerial(const bool debug=false)
                : debug_(debug) { }

        private:

            void Begin()
            {
                Serial1.begin(kBaudRate);
            }

            void Write(const uint8_t byte)
            {
                if (debug_) {
                    printf("write: 0x%02X\n", byte);
                }

                Serial1.write(byte);
            }

            auto Available() -> size_t
            {
                const size_t avail = Serial1.available();

                if (debug_) {
                    printf("avail: %u\n", available);
                }

                return avail;
            }

            auto Read(const size_t index) -> uint8_t
            {
                const uint8_t byte = 0;

                if (debug_) {
                    printf("read: 0x%02X\n", byte);
                }

                return byte;
            }
    };

}
