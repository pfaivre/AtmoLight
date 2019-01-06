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

#include <Arduino_FreeRTOS.h>

#include "Display.h"
#include "Io.h"
#include "config.h"


void setup() {
    #if LOG >= 1
        Serial.begin(38400);
        Serial.println(F("AtmoLight"));
    #endif

    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    // The Display's Task method is running in background
    // You just have to call one of the static methods to trigger a light effect
    xTaskCreate(
      Display::Task
        ,  NULL // Name (not necessary here)
        ,  256 // Stack size (in words: 1 word = 2 bytes on AVR)
        ,  NULL // Parameters
        ,  2 // Priority (the higher, the more)
        ,  NULL // Task's handle
    );

    xTaskCreate(
      Io::Task
        ,  NULL
        ,  256
        ,  NULL
        ,  2
        ,  NULL
    );
}

void loop() {
    // Idle. The code here will only be executed when there is no task running

    // All the work is actually done in the FreeRTOS threads
}
