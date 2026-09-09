/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fpga_client.h>
#include <processor.hpp>
#include <teensy_serial.hpp>

#include "xor.hpp"

static neuro::TeensySerial serial_;

void setup()
{
    // proc_ is declared in auto-generated xor.hpp
    proc_.Begin(&serial_);

    /*
    // Start the USB Host controller
    usb_.begin();

    // Start the USB Host serial port and set the baud rate for the connected
    // device This sends a control message over USB to configure the external
    // chip's physical UART speed
    userial_.begin(MY_BAUD_RATE);*/
}

void loop() 
{
    /*
    // The USB Host library requires periodic polling to process data
    usb_.Task();

    // Check if the external USB device has sent any data to the Teensy
    if (userial_.available()) {
        Serial.println("Data received from USB Host device:");

        // Read and print the data to the PC Serial Monitor
        while (userial_.available()) {
            char c = userial_.read();
            Serial.print(c);
        }
        Serial.println();
    }

    // Example: Forward data from the PC Serial Monitor to the USB Host device
    if (Serial.available()) {
        while (Serial.available()) {
            char c = Serial.read();
            userial_.write(c);
        }
    }*/
}

#if 0

void setup()
{
    auto ser = neuro::TeensySerial();

    proc.Begin(&ser);
}

void loop()
{
}
#endif
