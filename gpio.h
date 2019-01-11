/*
 * gpio.h:
 *	Access routines for the GPIO's on RPi.
 *	Parts of this code are borrowed from WiringPi and R42/pinokia - Luis Reis
 *
 *
 *
 **************************************************************
 * This file is part of rpiCC2500
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of Version 3 of the GNU General Public License as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed WITHOUT ANY WARRANTY; without even the implied
 *	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *	See Version 3 of the GNU General Public License for more details.
 *
 *	You should have received a copy of Version 3 of the GNU General Public
 *	License along with this program.
 *
 *	Section 5a of the license requires that all changes to this file be
 *	recorded prominently in this file. Please record all changes here.
 *
 **************************************************************
 *
 * Authors:
 *			Luis Reis
 *	GMN	Roman Gassmann	gmn@gr-solutions.net
 *
 * History:
 *	2013-12-01	GMN	Program prepared for realease
 */

#ifndef __GPIO_H
#define __GPIO_H

int gpio_setup();
void gpio_shutdown();

#define inl(f) inline f __attribute__((always_inline))

uint32_t gpio_word();
uint32_t gpio_set_input(uint32_t pin);
uint32_t gpio_set_output(uint32_t pin);
uint32_t gpio_alternate_function(uint32_t pin, uint32_t alternate);
uint32_t gpio_set(uint32_t pins);
uint32_t gpio_clear(uint32_t pins);
int gpio_read(int port);
uint32_t gpio_get(uint32_t pins);

#endif
