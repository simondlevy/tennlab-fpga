/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace neuro {

    class Serial {

        public:

            static void Connect();

            static void Write(const uint8_t byte);

            static size_t Available();

            static uint8_t Read(const size_t index);
    };
}
