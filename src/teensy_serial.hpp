/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

#include <serial.hpp>

namespace neuro {

    class TeensySerial : public neuro::Serial {

        private:

            static const size_t kMaxMessageSize = 4096;
            static const uint32_t kDefaultTimeoutMsec = 20;

        public:

            TeensySerial()
            {
            }

            void Begin()
            {
            }

            void Write(const uint8_t byte)
            {
            }

            auto Available() -> uint8_t
            {
                return 0;
            }

            auto Read(uint8_t index) -> uint8_t
            {
                return 0;
            }

            void Close()
            {
            }
    };

}
