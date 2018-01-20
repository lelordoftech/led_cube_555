/*
 * define.h
 *
 * Created: 20-Jan-2018
 * Author: Vuong Le
 */

#ifndef DEFINE_H
#define DEFINE_H

// CPU Speed
#define F_CPU 14745600
//#define FOSC        14745600
//#define FOSC        16000000

/*
 * Khai bao BUTTON, LED
 * PD5 - BUTTON
 * PD6 - LED RED
 */
// Define masks used for status LEDs and input buttons.
#define LED_STATUS  0x40
#define BUTTON      0x20

// Define port used for status and input.
#define LED_PORT    PORTD
#define BUTTON_PORT PORTD

/*
 * Khai bao 74hc595
 */
#define LED_PIN         0x01 // PA0     -  1    - 0b00000001
#define CLOCK_PIN       0x02 // PA1     - 11    - 0b00000010
#define LATCH_PIN       0x04 // PA2     - 12    - 0b00000100
#define DATA_PIN        0x08 // PA3     - 14    - 0b00001000
#define LATCH_LED_PIN   0x05 // PA0/PA2 - 1/12  - 0b00000101

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#endif
