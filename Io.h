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


/**
 * This class handles the user input/output
 */
class Io {
public:
    static void Task(void *pvParameters);

private:
    /**
     * Connect to the broker through Ethernet.
     * Does nothing if the connection is already active
     */
    static void _connect();

    /**
     * Callback for the PubSubClient library
     * Handles incoming messages from the broker
     */
    static void _callback(char* topic, byte* payload, unsigned int length);

    /**
     * Parse a color in hexadecimal format
     * @param hex Input string (e.g. "#f0abe5"). Case insensitive. Will be modified.
     * @param r Output red value
     * @param g Output green value
     * @param b Output blue value
     * @return true if the parsing is successful
     */
    static bool _parseColor(char hex[], byte* r, byte* g, byte* b);

    /**
     * Change to the next mode
     */
    static void _nextMode();

    /**
     * Select a new variation of the current mode
     * Doesn't affect all the modes
     */
    static void _var();
};
