/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <processor.hpp>
#include <posix/usb_serial.hpp>

#include "xor.hpp"

static const std::string kPortName = "/dev/ttyUSB1";

static constexpr int kNumInputs = 2;
static constexpr int kNumOutputs = 1;
static constexpr int kChargeWidth = 2;
static constexpr int kEntryValueFactor = 1;

static void run(const uint8_t a, const uint8_t b)
{
    proc.ClearActivity();

    if (a) {
        proc.ApplySpike(0, 0, 1);
    }

    if (b) {
        proc.ApplySpike(1, 0, 1);
    }

    proc.Run(3);
    printf("input = %d,%d; output = %d\n", a, b, proc.GetOutputCount(0));
}

int main()
{
    auto ser = neuro::UsbSerial(kPortName);

    proc.Begin(&ser);

    proc.ClearActivity();

    run(0, 0);
    run(0, 1);
    run(1, 0);
    run(1, 1);

    return 0;
}
