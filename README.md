# AtmoLight

Ambiance lighting system

## What it looks like

![Preview 1](docs/images/aurora.gif)  

![Preview 2](docs/images/fire.gif)  

![Preview 3](docs/images/rainbow.gif)  

## How it works

Atmolight is build around the FreeRTOS operating system and FastLED library.

There are 2 main parts:
 * **Display**: handles the logic to create the visual animations
 * **Io**: handles external communication
     * Through two buttons (next mode and variation)
     * Through network using MQTT protocol

## What is needed to make it work

 * An Arduino compatible board
 * Basic electronic parts (2 buttons, resistors, wires)
 * A LED strip compatible with FastLED (such as [Neopixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections))
 * FreeRTOS and Fastled libraries (can be installed using the library manager)

## Networking

If you want to control the lights over network, you will need some additional parts:
 * An Arduino board with more than 33 kio of flash (Arduino UNO has not enough, but you can still disable networking on config.h to save space)
 * An Ethernet shield or Ethernet interface built-in the board
 * PubSubClient library (can be installed using the library manager)
 * An MQTT broker on your network (a server that will dispatch messages to all the devices)
 * A device that will send commands through MQTT (there is a lot of smartphone applications for that)

On config.h, change the values of `IO_MAC_ADDRESS` and `IO_BROKER_ADDRESS` according to your configuration.

Once started and connected, just publish messages to the following topics:

 * `lights/all`

| message | description |
| ------- | ----------- |
| mode    | Select the next display mode |
| var     | Change the variation of the current mode (if any) |
| on      | Activates the display |
| off     | Deactivates the display |

 * `lights/all/color`

| message | description |
| ------- | ----------- |
| #xxxxxx | Change the current color (hexadecimal format). Affects some modes only |

