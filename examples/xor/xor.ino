/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <tennlab_fpga.h>
#include <processor.hpp>
#include <teensy_serial.hpp>

#include "xor.hpp"

static neuro::TeensySerial serial_(true);

static void run(const uint8_t a, const uint8_t b)
{
    // proc_ is declared in auto-generated xor.hpp
    proc_.ClearActivity();

    /*
    if (a) {
        proc_.ApplySpike(0, 0, 1);
    }

    if (b) {
        proc_.ApplySpike(1, 0, 1);
    }

    proc_.Run(3);

    printf("input = %d,%d; output = %d\n", a, b, proc_.GetOutputCount(0));
    */
}


void setup()
{
    Serial.begin(0);
    while (!Serial) {
    }

    proc_.Begin(&serial_);
}

void loop() 
{
    // The Teensy USB Host library requires periodic polling to process data
    serial_.Poll();

    // Nothing can be sent until the FPGA board has enumerated
    if (!serial_.IsReady()) {
        return;
    }

    static uint32_t msec_prev_;
    const auto msec_curr = millis();
    if (true /*msec_curr - msec_prev_ > 1000*/) {
        msec_prev_ = msec_curr;
        run(0, 0);
        //run(0, 1);
        //run(1, 0);
        //run(1, 1);
    }
}
