# Copyright (c) 2026 Simon D. Levy
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

SKETCH = $(shell basename "`pwd`")

ROOT = ../..

SRC = $(ROOT)/src

FQBN = teensy:avr:teensy41

PORT = /dev/ttyACM0

NET = $(ROOT)/networks/xor.txt

LIBS = $(HOME)/Documents/Arduino/libraries

build: $(SKETCH).ino xor.hpp $(SRC)/*.hpp
	arduino-cli compile --libraries $(LIBS) --libraries $(ROOT) \
		--fqbn $(FQBN) $(SKETCH).ino

xor.hpp: $(NET)
	$(ROOT)/utils/mkhdr.py $(NET) -o ./xor.hpp

flash:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) .

clean:
	rm -rf xor.hpp

edit:
	vim $(SKETCH).ino

listen:
	miniterm.py $(PORT) 115200 --exit-char 3
