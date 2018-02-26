# Max114 / Max118 driver library

-----

## tl;dr

This repository provides a C++/Arduino driver that can be used to interface microcontrollers with a Max114/Max118 multi-channel analog-to-digital converter. It is still in its infancy, but does have a working read-only mode implemented and tested.

-----

## Overview

The Max114/Max118 integrated circuits are 4/8 channel 8-bit analog-to-digital converters that were released by Maxim Integrated Products in 1996 and boast a sampling rate of just over 1Msps. Their low cost and DIP packaging made them attractive to me when I was designing the electronics for a 4-PMT fluorescence-activated cell sorter that our lab needed, and I felt that the lack of high-level code needed to drive this chip using a microcontroller (in our case, the Teensy 3.6) was somewhat disappointing. So I made this! Please, feel free to comment on, contribute to, and use this small library as you see fit (it is released under the GPLv3 license). I'd love to hear from you if you still use this nice little chip and find this library useful!

## Requirements

This driver depends on the Arduino framework for setting pin modes and using `digitalWriteFast()` and `digitalReadFast()`. If your microcontroller of choice does not have an implementation of the Arduino framework (i.e. you can't `#include <Arduino.h>` successfully), you will need to implement at least these functions yourself and `#include` those definitions yourself in the **max114.h** and/or **max118.h** headers. 

## Usage 
### (assuming you use platformio)

1. Clone this repository into your project's **lib/** space
2. `#include <Max114.h>`
3. Create a new Max114 object: `Max114 adc0 = new Max114(int* data_pins_arg, int* channel_pins_arg, int RD_pin_arg, int CS_pin_arg, int WR_pin_arg, int INT_pin_arg, int MODE_pin_arg)`; with arrays passed for the data pins [D0, D1, ..., D7] and channel select pins [A0, A1]. For example, if you connect D0, D1, D2, D3, D4, D5, D6, and D7 to pins 2,3,4,5,6,7,8,9 on your microcontroller, you could make a new array of integers: `int* data_pins = {2,3,4,5,6,7,8,9};` and pass that as the first argument to the constructor for the Max114 object. Likewise for the singular pins; if the MODE pin on the Max114 is connected to pin 17 on your microcontroller, pass the value (not array) 17 as the last argument.  
4. Set the mode for the Max114: `adc0->set_mode(int MODE);`. Currently (2018-02-25) the Read-only mode (mode 0) of the Max114 is implemented and tested, so use `adc0->set_mode(0);`. 
5. Read from any of the 4/8 channels using `int value = adc0->analog_read(int CHANNEL_INDEX);` (zero-indexed, so use `adc0->analog_read(0)` to read IN1).

-----

## Notes

### Conversion speed/bandwidth
* Minimal code to read a single channel rapidly using the read-only mode requires an average of 1.57us/sample on a Teensy 3.6 clocked at 216MHz (using the compile flags `-O3` and `-flto`). This corresponds to a sampling rate of ~0.637Msps, or about half of what is advertised by the manufacturer. This is still faster than the Teensy 3.6's onboard ADC, but the write-read mode described in the manufacturer's datasheet will be required to reach this chip's sampling rate.
* To read all 4 channels, it requires approximately 6us total. This example/test code is provided in the **main.cpp** file.

### TODO list 
* Fix the write-read mode (mode 1) which promises a slightly higher conversion speed
* Implement and test the pipelined write-read mode for optimized multi-channel reading
* Write more tests/example code