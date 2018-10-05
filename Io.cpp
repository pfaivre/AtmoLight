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

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <FastLED.h>

#include "Display.h"
#include "Io.h"
#include "config.h"


void Io::Task(void *pvParameters) {
    byte buttonState = LOW;
    byte lastButtonModeState = LOW;
    byte lastButtonVarState = LOW;
    byte currentMode = 1;
    
    pinMode(IO_BUTTON_MODE_PIN, INPUT);
    pinMode(IO_BUTTON_VAR_PIN, INPUT);

    for (;;) {
        buttonState = digitalRead(IO_BUTTON_MODE_PIN);
        
        if (buttonState != lastButtonModeState) {
            if (buttonState == HIGH) {
                #if LOG >= 3
                    Serial.println(F("Button pressed"));
                #endif

                currentMode = currentMode >= 7 ? 0 : currentMode + 1;

                switch (currentMode) {
                    case 0:
                        Display::SolidColor(0x000000);
                        break;
                    case 1:
                        Display::White();
                        break;
                    case 2:
                        Display::SolidColor(CHSV(random8(), 255, 255));
                        break;
                    case 3:
                        Display::Pulse(CHSV(random8(), 255, 255));
                        break;
                    case 4:
                        Display::Rainbow();
                        break;
                    case 5:
                        Display::Fire();
                        break;
                    case 6:
                        Display::Aurora();
                        break;
                    case 7:
                        Display::Disco();
                        break;
                }
            }
            
            vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce delay
        }

        lastButtonModeState = buttonState;

        buttonState = digitalRead(IO_BUTTON_VAR_PIN);
        
        if (buttonState != lastButtonVarState) {
            if (buttonState == HIGH) {
                Display::SetColor(CHSV(random8(), 255, 255));
            }
            
            vTaskDelay(50 / portTICK_PERIOD_MS); // Debounce delay
        }

        lastButtonVarState = buttonState;
        
        vTaskDelay(IO_SCAN_DELAY / portTICK_PERIOD_MS);
    }
}


