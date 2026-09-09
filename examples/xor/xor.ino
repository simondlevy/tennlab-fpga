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

static neuro::TeensySerial serial_;

void setup()
{
    // proc_ is declared in auto-generated xor.hpp
    proc_.Begin(&serial_);
}

void loop() 
{
    // The Teensy USB Host library requires periodic polling to process data
    serial_.Poll();
}
