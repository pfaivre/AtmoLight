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
#include "config.h"

CRGB strip[LEDS_NUMBER];


void Display::Task(void *pvParameters) {
    unsigned long prevMillisCountdown = millis(); // Timer used by the remaining time countdown
    
    FastLED.addLeds<NEOPIXEL, LEDS_PIN>(strip, LEDS_NUMBER);
    FastLED.setBrightness(LEDS_BRIGHTNESS);

    // Set a seed from analog input to get different values each start
    random16_set_seed(analogRead(0));

    // TODO: At startup, display the last mode
    Display::White();

    for (;;) {
        while (_remainingTime > 0) {
            if (_mode == Mode::White || _mode == Mode::SolidColor) {
                if (_isTransiting && strip[0] != _currentColor) {
                    _animateToColor(_currentColor);
                    FastLED.show();
                }
                else
                    _isTransiting = false;
            }
            else if (_mode == Mode::Pulse) {
                fill_solid(strip, LEDS_NUMBER, _currentColor & CRGB(CHSV(0, 0, 64 * cos(0.001 * millis()) + 192)));
                FastLED.show();
            }
            else if (_mode == Mode::Rainbow) {
                fill_rainbow(strip, LEDS_NUMBER, _reg8_a++, 255 / LEDS_NUMBER);
                FastLED.show();
            }
            else if (_mode == Mode::Fire) {
                _drawFire();
                FastLED.show();
            }
            else if (_mode == Mode::Aurora) {
                _drawAurora();
                FastLED.show();
            }
            else if (_mode == Mode::Disco) {
                _drawDisco();
                FastLED.show();
            }
            
            // Countdown
            if (millis() - prevMillisCountdown >= 1000) {
                prevMillisCountdown = millis();
                _remainingTime--;

                // If the time is up
                if (_remainingTime <= 0) {
                    #if LOG >= 3
                        Serial.println(F("Time's up"));
                    #endif
                    // Switch the lights off
                    SolidColor(0x000000);
                }
            }
            
            vTaskDelay(LEDS_DELAY / portTICK_PERIOD_MS);
        }
        
        vTaskDelay(LEDS_DELAY / portTICK_PERIOD_MS);
    }
}

void Display::White() {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::SolidColor;
    _isTransiting = true;
    _currentColor = 0xFFBB88;
    _reg8_a = 0;

    #if LOG >= 2
        Serial.println(F("SolidColor"));
    #endif
}

void Display::SolidColor(CRGB color) {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::SolidColor;
    _isTransiting = true;
    _currentColor = color;
    _reg8_a = 0;

    #if LOG >= 2
        Serial.println(F("SolidColor"));
    #endif
}

void Display::Pulse(CRGB color) {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::Pulse;
    _isTransiting = true;
    _currentColor = color;

    #if LOG >= 2
        Serial.println(F("Pulse"));
    #endif
}

void Display::Rainbow() {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::Rainbow;
    _isTransiting = true;

    #if LOG >= 2
        Serial.println(F("Rainbow"));
    #endif
}

void Display::Fire() {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::Fire;
    _isTransiting = true;

    #if LOG >= 2
        Serial.println(F("Fire"));
    #endif
}

void Display::Aurora() {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::Aurora;
    _isTransiting = true;
    _reg8_b = 160;
    _reg8_c = 110;

    #if LOG >= 2
        Serial.println(F("Aurora"));
    #endif
}

void Display::Disco() {
    _remainingTime = (uint16_t)0 - 1; // Unlimited
    _mode = Mode::Disco;
    _reg8_a = 0;
    _reg16_a = millis();
    _isTransiting = true;
    _currentColor = CHSV(random8(), random8() / 16 + 239, 255);

    for (byte i = 0; i < 32; i++) {
        for (byte i = 0; i < LEDS_NUMBER; i++) {
            strip[i] = blend(strip[i], 0x000000, 18);
        }
        vTaskDelay(16 / portTICK_PERIOD_MS);
    }

    #if LOG >= 2
        Serial.println(F("Disco"));
    #endif
}

void Display::SwitchOff() {
    _remainingTime = 0;
    _printSolidColor(CRGB(0, 0, 0));

    #if LOG >= 2
        Serial.println(F("SwitchOff"));
    #endif
}

void Display::SetRemainingTime(uint16_t seconds) {
    _remainingTime = seconds;
    _isTransiting = true;
    
    #if LOG >= 2
        Serial.print(F("SetRemainingTime:"));
        Serial.println(seconds);
    #endif
}

void Display::SetColor(CRGB color) {
    _isTransiting = true;
    _currentColor = color;
    _reg8_a = 0;
    _reg8_b = random8();
    _reg8_c = random8();

    #if LOG >= 2
        Serial.print(F("SetColor "));
        Serial.print(_currentColor.r);
        Serial.print(" ");
        Serial.print(_currentColor.g);
        Serial.print(" ");
        Serial.println(_currentColor.b);
    #endif
}

void Display::_printSolidColor(CRGB color) {
    fill_solid(strip, LEDS_NUMBER, color);
    FastLED.show();
}

void Display::_fadeToColor(CRGB color) {
    fill_solid(strip, LEDS_NUMBER, blend(strip[0], color, 32));
}

void Display::_animateToColor(CRGB color) {
    short start = LEDS_NUMBER / 2 - _reg8_a;
    short end = LEDS_NUMBER / 2 + _reg8_a;

    if (start < 0)
        start = 0;

    if (end > LEDS_NUMBER)
        end = LEDS_NUMBER;
    
    for (short i = start; i < end; i++) {
        strip[i] = color;
    }
    
    _reg8_a++;
}

void Display::_drawFire() {
    CHSV color1, color2;
    
    for (byte i=0 ; i<LEDS_NUMBER ; i++) {
        // First wave, going forwards
        color1 = CHSV(10, 255, 96 * cos(0.003 * millis() + 0.2 * i - sin(i)) + 159);

        // Second wave, goind backwards
        color2 = CHSV(25, 255, 127 * cos(-0.0085 * millis() + 3.2 * i + sin(i)) + 127);
        
        strip[i] = CRGB(color1) + CRGB(color2);
    }

    FastLED.show();
}

void Display::_drawAurora() {
    CHSV color1, color2;
    
    for (byte i=0 ; i<LEDS_NUMBER ; i++) {
        // First wave, going forwards
        //color1 = CHSV(160, 255, 127.0 * cos(0.0005 * millis() + 0.1 * i) + 127);
        color1 = CHSV(_reg8_b, 255, 127.0 * cos(0.0005 * millis() + 0.1 * i) + 127);

        // Second wave, goind backwards
        //color2 = CHSV(110, 255, 127.0 * cos(-0.001 * millis() + 0.8 * i + sin(i)) + 127);
        color2 = CHSV(_reg8_c, 255, 127.0 * cos(-0.001 * millis() + 0.8 * i + sin(i)) + 127);
        
        strip[i] = CRGB(color1) + CRGB(color2);
    }

    FastLED.show();
}


void Display::_drawDisco() {
    // In this mode _reg16_a is the last millis() and _reg8_a is the seleced section

    // Transition to fill with initial colors
    if (_isTransiting == true) {
        // Paint each section for 200 ms
        if (millis() - _reg16_a >= 200) {
            _reg16_a = millis();
            _reg8_a++;
            _currentColor = CHSV(random8(), random8() / 16 + 239, 255);
            if (_reg8_a * 10 > LEDS_NUMBER)
                _isTransiting = false;
        }
        else {
            for (byte i = _reg8_a * 10; i < _reg8_a * 10 + 10 && i < LEDS_NUMBER; i++) {
                strip[i] = blend(strip[i], _currentColor, 38);
            }
        }
        return;
    }
    
    // After a few seconds
    if (millis() - _reg16_a >= random8()*6 + 2500) {
        _reg16_a = millis();
        
        // Choose a 10 led section
        byte nbSections = LEDS_NUMBER / 10; // FIXME: this might miss the last section if incomplete
        _reg8_a = random8() * nbSections / 255;

        // Choose a color
        _currentColor = CHSV(random8(), random8() / 16 + 239, 255);
    }

    // Fade the selected section to the current color
    for (byte i = _reg8_a * 10; i < _reg8_a * 10 + 10 && i < LEDS_NUMBER; i++) {
        strip[i] = blend(strip[i], _currentColor, 24);
    }
}

uint16_t Display::_remainingTime = 0;

Mode Display::_mode = Mode::Off;

CRGB Display::_currentColor = CRGB(0x000000);

uint8_t Display::_reg8_a = 0;

uint8_t Display::_reg8_b = 0;

uint8_t Display::_reg8_c = 0;

unsigned long Display::_reg16_a = 0;

bool Display::_isTransiting = false;
