/**
 * AtmoLight
 *
 * Copyright (C) 2016-2018 Pierre Faivre
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

/*
 * Logging verbosity level:
 * - 0 None (does not begin Serial)
 * - 1 Error and Warning
 * - 2 Info
 * - 3 Debug
 */
#define LOG 0


// --------------------
// LEDs: Neopixel Strip
// --------------------

#define LEDS_BRIGHTNESS 255 // over 255
#define LEDS_NUMBER 90
#define LEDS_PIN 6
#define LEDS_DELAY 40 // in milliseconds (40ms gives 25 fps)


// ----------------
// IO: Input/Output
// ----------------

#define IO_BUTTON_MODE_PIN 7
#define IO_BUTTON_VAR_PIN 8
#define IO_SCAN_DELAY 100 // in milliseconds

#define IO_NETWORKING 1 // 1 activates ethernet connection. 0 disables it to save memory space.
#define IO_MAC_ADDRESS { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } // Mac address of the device (should be written on your ethernet board)
#define IO_BROKER_ADDRESS "192.168.0.1" // Address of the MQTT broker (e.g. "192.168.0.1").
