/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <USBHost_t36.h>

#include <string>

#include <serial.hpp>

namespace neuro {

    class TeensySerial : public neuro::Serial {

        private:

            static const uint32_t kBaudRate = 4'000'000;
            static const size_t kMaxMessageSize = 4096;
            static const uint32_t kDefaultTimeoutMsec = 20;

        public:

            void Begin()
            {
                // Start the USB Host controller
                usb_.begin();

                // Start the USB Host serial port and set the baud rate for the
                // connected device This sends a control message over USB to
                // configure the external chip's physical UART speed
                userial_.begin(kBaudRate);
            }

            void Write(const uint8_t byte)
            {
                userial_.write(byte);
            }

            auto Available() -> uint8_t
            {
                return userial_.available();
            }

            auto Read(uint8_t index) -> uint8_t
            {
                return userial_.read();
            }

            void Poll()
            {
                usb_.Task();
            }

        private:

            USBHost usb_;
            USBSerial userial_ = USBSerial(usb_);
    };

}
