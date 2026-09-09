/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <USBHost_t36.h>

#include <serial.hpp>

namespace neuro {

    class TeensySerial : public neuro::Serial {

        private:

            static const uint32_t kBaudRate = 4'000'000;
            static const size_t kMaxMessageSize = 4096;
            static const uint32_t kDefaultTimeoutMsec = 20;

        public:

            TeensySerial(const bool debug=false)
                : debug_(debug) { }

            // Must be called frequently from loop(): the USB Host library
            // detects an attached device only inside Task().
            void Poll()
            {
                usb_.Task();

                const bool connected = (bool)userial_;

                if (connected && !connected_) {

                    // Only now can begin() actually reach the device.  Called
                    // before the board enumerates, it spins for five seconds
                    // and sets no baud rate at all.
                    userial_.begin(kBaudRate);

                    if (debug_) {
                        printf("connected: %04x:%04x\n",
                                userial_.idVendor(), userial_.idProduct());
                    }
                }

                connected_ = connected;
            }

            auto IsReady() -> bool
            {
                return connected_;
            }

        private:

            void Begin()
            {
                // Start the USB Host controller.  The serial port itself is
                // opened by Poll(), once the FPGA board has enumerated.
                usb_.begin();
            }

            void Write(const uint8_t byte)
            {
                if (debug_) {
                    printf("write: 0x%02X\n", byte);
                }

                userial_.write(byte);
            }

            // Gathers a whole reply into buf_, then reports its size.  As with
            // the POSIX driver, we wait until the link has been quiet for
            // kDefaultTimeoutMsec: the FPGA's answer does not arrive
            // instantaneously, so asking the USB Host library what it holds
            // right now would nearly always report nothing.
            auto Available() -> size_t
            {
                size_t got = 0;

                elapsedMillis quiet;

                while (got < kMaxMessageSize && quiet < kDefaultTimeoutMsec) {

                    usb_.Task();

                    while (got < kMaxMessageSize) {

                        const auto c = userial_.read();

                        if (c < 0) {
                            break;
                        }

                        buf_[got++] = (uint8_t)c;

                        quiet = 0;
                    }
                }

                return got;
            }

            auto Read(const size_t index) -> uint8_t
            {
                const auto byte = buf_[index];

                if (debug_) {
                    printf("read:  0x%02X\n", byte);
                }

                return byte;
            }

            USBHost usb_;

            // min_rxtx=1 so this claims full-speed adapters too; its 4096-byte
            // receive buffer matches Processor::kSystemBufferSizeBytes, which
            // plain USBSerial (648 bytes) would undershoot.
            USBSerial_BigBuffer userial_ = USBSerial_BigBuffer(usb_, 1);

            uint8_t buf_[kMaxMessageSize] = {};

            bool connected_ = false;

            bool debug_;
    };

}
