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

#include <FastLED.h>


/**
 * Display Mode
 */
enum class Mode : byte {
    Off = 0,
    White = 1,
    SolidColor = 2,
    Pulse = 3,
    Rainbow = 4,
    Fire = 5,
    Aurora = 6,
    Disco = 7
};


/**
 * This handles the LED strip
 */
class Display {
public:
    static void Setup();
    static void Loop();

    /**
     * White mode
     * Just like a regular lamp
     */
    static void White();

    /**
     * Solid Color mode
     * Show the same color on every led of the ring
     */
    static void SolidColor(CRGB color);

    /**
     * Pulse mode
     * Blink smoothly the strip with a solid color
     */
    static void Pulse(CRGB color);

    /**
     * Rainbow mode
     * A colorful rail on which you can ride a unicorn
     */
    static void Rainbow();

    /**
     * Fire mode
     * You don't have a fireplace? No problem
     */
    static void Fire();

    /**
     * Aurora mode
     * Like a polar light, in your house
     */
    static void Aurora();

    /**
     * Disco mode
     */
    static void Disco();

    /**
     * Set the timer
     */
    static void SetRemainingTime(uint16_t seconds);

    /**
     * Set a new color for the current mode
     */
    static void SetColor(CRGB color);

    /**
     * Turn off the leds
     */
    static void SwitchOff();

private:
    /**
     * DisplayMode
     */
    static Mode _mode;

    /**
     * Number of seconds left to display something on the strip.
     * Put the maximum value for uint16_t for unlimited time.
     */
    static uint16_t _remainingTime;

    /**
     * A color that can be used by the modes
     */
    static CRGB _currentColor;

    /**
     * A 8-bit register that can be used by the modes
     */
    static uint8_t _reg8_a;

    /**
     * A 8-bit register that can be used by the modes
     */
    static uint8_t _reg8_b;

    /**
     * A 8-bit register that can be used by the modes
     */
    static uint8_t _reg8_c;

    /**
     * A 8-bit register that can be used by the modes
     */
    static unsigned long _reg16_a;

    /**
     * Indicates if a transition between modes is still happening
     * This is used to stop the refresh after on still modes
     */
    static bool _isTransiting;

    /**
     * Print the same color on every led of the ring
     */
    static void _printSolidColor(CRGB color);

    /**
     * Fade the given color into the strip pixels
     */
    static void _fadeToColor(CRGB color);

    /**
     * Animate to the given color. Makes use of _reg8_a
     */
    static void _animateToColor(CRGB color);

    /**
     * The core logic for the Fire mode
     */
    static void _drawFire();

    /**
     * The core logic for the Aurora mode
     */
    static void _drawAurora();

    /**
     * The core logic for the Disco mode
     */
    static void _drawDisco();
};
