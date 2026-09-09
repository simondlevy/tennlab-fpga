/*
 * Copyright (c) 2026 Simon D. Levy
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace neuro {

    class Serial {

        friend class Processor;

        private:

            virtual void Begin() = 0;

            virtual void Write(const uint8_t byte) = 0;

            virtual uint8_t Available() = 0;

            virtual uint8_t Read(uint8_t index) = 0;

            virtual void Close() = 0;
    };
}
