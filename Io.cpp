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
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "Display.h"
#include "Io.h"
#include "config.h"

#if IO_NETWORKING == 1
    #include <EthernetClient.h>
    #include <PubSubClient.h>

    EthernetClient eth;
    PubSubClient mqtt(eth);
    const char t_lights_all[] = "lights/all";
    const char t_lights_all_color[] = "lights/all/color";
    bool ethConnected = false;
    unsigned long prevMillisNetwork; // Timer used for the network monitoring
#endif

byte currentMode = 1;

void Io::Task(void *pvParameters) {
    byte buttonState = LOW;
    byte lastButtonModeState = LOW;
    byte lastButtonVarState = LOW;
    
    pinMode(IO_BUTTON_MODE_PIN, INPUT);
    pinMode(IO_BUTTON_VAR_PIN, INPUT);

    #if IO_NETWORKING == 1
        prevMillisNetwork = millis();
        
        // disable SD card
        pinMode(4, OUTPUT);
        digitalWrite(4, HIGH);

        _connect();
    #endif

    for (;;) {
        buttonState = digitalRead(IO_BUTTON_MODE_PIN);
        
        if (buttonState != lastButtonModeState) {
            if (buttonState == HIGH) {
                _nextMode();
            }
        }

        lastButtonModeState = buttonState;

        buttonState = digitalRead(IO_BUTTON_VAR_PIN);
        
        if (buttonState != lastButtonVarState) {
            if (buttonState == HIGH) {
                _var();
            }
        }

        lastButtonVarState = buttonState;

        #if IO_NETWORKING == 1
            if (mqtt.connected()) {
                mqtt.loop();
            }

            // Periodically check network status
            if (millis() - prevMillisNetwork >= 30000) {
                prevMillisNetwork = millis();
                _connect();
            }
        #endif
        
        vTaskDelay(IO_SCAN_DELAY / portTICK_PERIOD_MS);
    }
}

#if IO_NETWORKING == 1

void Io::_connect() {
    // Start the ethernet interface and try to get an IP address from the DHCP server.
    if (!ethConnected) {
        #if LOG >= 1
            Serial.println(F("Connecting eth..."));
        #endif
        byte mac[] = IO_MAC_ADDRESS;
        if (Ethernet.begin(mac, 5000) == 0) {
            digitalWrite(13, LOW);
            ethConnected = false;
            #if LOG >= 2
                Serial.print(F("eth failed "));
                Serial.println(Ethernet.localIP());
            #endif
        }
        else {
            ethConnected = true;
            #if LOG >= 2
                Serial.print(F("DHCP assigned IP "));
                Serial.println(Ethernet.localIP());
            #endif
        }
    }

    // Connect to the MQTT broker
    if (ethConnected && !mqtt.connected()) {
        #if LOG >= 2
            Serial.println(F("Trying to connect to the broker..."));
        #endif
        mqtt.setServer(IO_BROKER_ADDRESS, 1883);
        mqtt.setCallback(Io::_callback);
        char clientId[23]; // "light_xx:xx:xx:xx:xx:xx"
        byte mac[] = IO_MAC_ADDRESS;
        sprintf(clientId, "light_%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        if (mqtt.connect(clientId))
        {
            digitalWrite(13, HIGH);
            mqtt.subscribe(t_lights_all);
            mqtt.subscribe(t_lights_all_color);
            #if LOG >= 1
                Serial.println(F("Connected to broker"));
            #endif
        }
        else
        {
            digitalWrite(13, LOW);
            #if LOG >= 1
                Serial.println(F("Failed to connect to broker"));
            #endif
        }
    }
}

void Io::_callback(char* topic, byte* payload, unsigned int length) {
    char buffer[17] = { 0 };
    strncpy(buffer, (char*)payload, min(16, length));
    
    #if LOG >= 3
        Serial.print(F("In msg ["));
        Serial.print(topic);
        Serial.print("] ");
        Serial.print(buffer);
        Serial.println();
    #endif

    if (strcmp(buffer, "on") == 0) {
        Display::SetRemainingTime((uint16_t)0 - 1); // Unlimited
    }
    else if (strcmp(buffer, "off") == 0) {
        Display::SwitchOff();
    }
    else if (strcmp(buffer, "mode") == 0) {
        _nextMode();
    }
    else if (strcmp(buffer, "var") == 0) {
        _var();
    }
    else if (buffer[0] == '#' && length == 7) {
        byte r=0, g=0, b=0;
        if (_parseColor(buffer, &r, &g, &b))
        {
            CRGB newColor(r, g, b);
            Display::SetColor(newColor);
        }
    }
}

bool Io::_parseColor(char hex[], byte* r, byte* g, byte* b) {
    byte tr=0, tg=255, tb=0;

    // For efficiency, we trust here the string is 7 char long. Should be checked above.
    for (byte i=1; i < 7; i++) {
        if (!isxdigit(hex[i]))
            return false;
    }

    hex[7] = '\0';
    tb = (byte)strtol(hex+5, NULL, 16);
    hex[5] = '\0';
    tg = (byte)strtol(hex+3, NULL, 16);
    hex[3] = '\0';
    tr = (byte)strtol(hex+1, NULL, 16);
    
    *r = tr;
    *g = tg;
    *b = tb;

    return true;
}

#endif

void Io::_nextMode() {
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

void Io::_var() {
    CRGB newColor = CHSV(random8(), 255, 255);
    Display::SetColor(newColor);

    #if IO_NETWORKING == 1
        if (mqtt.connected()) {
            char hex[] = "#000000";
            sprintf(hex, "#%02X%02X%02X", newColor.r, newColor.g, newColor.b);
            mqtt.unsubscribe(t_lights_all_color);
            mqtt.publish(t_lights_all_color, hex);
            mqtt.subscribe(t_lights_all_color);
        }
    #endif
}
