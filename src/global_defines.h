#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

static const unsigned int DATA_PINS[]    = {2,3,4,5,6,7,8,9}; // digital input to microcontroller from ADC (TODO:endian?)
static const unsigned int CHANNEL_PINS[] = {10,11}; 		  // A0, A1 in datasheet, used to choose channel
static const unsigned int CS_PIN = 12;
static const unsigned int WR_PIN = 39;
static const unsigned int RD_PIN = 23;
static const unsigned int INT_PIN = 22;
static const unsigned int MODE_PIN = 21;


#endif
