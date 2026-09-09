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

                const bool connected = (bool)uart_;

                if (connected && !connected_) {

                    // Only now can begin() actually reach the device.  Called
                    // before the board enumerates, it spins for five seconds
                    // and sets no baud rate at all.
                    uart_.begin(kBaudRate);

                    if (debug_) {
                        printf("jtag channel: %04x:%04x %s\n",
                                jtag_.idVendor(), jtag_.idProduct(),
                                jtag_ ? "claimed" : "NOT CLAIMED");
                        printf("uart channel: %04x:%04x claimed\n",
                                uart_.idVendor(), uart_.idProduct());
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

                uart_.write(byte);
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

                        const auto c = uart_.read();

                        if (c < 0) {
                            break;
                        }

                        buf_[got++] = (uint8_t)c;

                        quiet = 0;
                    }
                }

                if (debug_) {
                    printf("avail: %u\n", got);
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

            // The Cmod A7's FT2232H exposes two interfaces: channel A (0) is
            // JTAG, channel B (1) is the UART we want.  One driver object
            // claims one interface, in construction order, so the first of
            // these absorbs the JTAG channel and the second gets the UART.
            //
            // min_rxtx=1 so these claim full-speed adapters too; BigBuffer is
            // required for the FT2232H, whose high-speed bulk endpoints are
            // 512 bytes -- more than plain USBSerial will accept -- and its
            // 4096-byte receive buffer matches Processor::kSystemBufferSizeBytes.
            USBSerial_BigBuffer jtag_ = USBSerial_BigBuffer(usb_, 1);
            USBSerial_BigBuffer uart_ = USBSerial_BigBuffer(usb_, 1);

            uint8_t buf_[kMaxMessageSize] = {};

            bool connected_ = false;

            bool debug_;
    };

}
