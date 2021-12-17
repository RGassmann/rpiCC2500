/*
 * gpio.c:
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include "gpio.h"

// Docs:
//   http://www.farnell.com/datasheets/1642649.pdf

// Parts of this code borrowed from Wiring Pi

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE         (BCM2708_PERI_BASE + 0x200000) // GPIO controller

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |=  (1 << (((g) % 10) * 3))
#define SET_GPIO_ALT(g,a) *(gpio + (((g) / 10))) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))

/*#define GPIOEDS_GET(g) (((*(gpio + 0x0040/4 + ((g) / 32))) & (1 << ((g) % 32) )) ? 1:0)
//#define GPIOEDS_CLR(g) *(gpio + 0x0040/4 + ((g) / 32)) |= (1 << ((g) % 32) )
//#define SET_GPIO_FED(g) *(gpio + 0x0058/4 + ((g) / 32)) |= (1 << ((g) % 32) )
//#define SET_GPIO_FEDCLR(g) *(gpio + 0x0058/4 + ((g) / 32)) &= ~(1 << ((g) % 32) )
*/
#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

//#define GPIO_READ(g)  *(gpio + 13) &= (1<<(g))
#define GPIO_GET *(gpio+13)
#define GPIO_GETBIT(g) ((*(gpio+13) & (1<<(g))) >> g)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// I/O access
volatile unsigned *gpio;

int mem_fd;
uint32_t *gpio_mem, *gpio_map;

int gpio_setup() {
  // open /dev/mem
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    fprintf(stderr, "can't open /dev/mem\n");
    return -1;
  }

  // mmap GPIO

  // Allocate MAP block
  int ret = posix_memalign((void *) &gpio_mem,
                           (size_t) PAGE_SIZE, (size_t) BLOCK_SIZE);
  if (ret != 0 || gpio_mem == NULL) {
    fprintf(stderr, "posix_memalign failed: %s\n", strerror(ret));
    return -1;
  }

  // Now map it
  gpio_map = (uint32_t *) mmap((void *) gpio_mem,
                               BLOCK_SIZE,
                               PROT_READ  | PROT_WRITE,
                               MAP_SHARED | MAP_FIXED,
                               mem_fd,
                               GPIO_BASE);

  if ((long) gpio_map < 0) {
    fprintf(stderr, "mmap error %d\n", (int) gpio_map);
    return -1;
  }

  // Always use a volatile pointer!
  gpio = (volatile uint32_t *) gpio_map;

  return 0;
}

void gpio_shutdown() {
  munmap((caddr_t) gpio_mem, BLOCK_SIZE);

  free(gpio_mem);

  close(mem_fd);
}

uint32_t gpio_word() {
  return gpio[13];
}

uint32_t gpio_set_input(uint32_t pin) {
  return INP_GPIO(pin);
}

uint32_t gpio_set_output(uint32_t pin) {
  INP_GPIO(pin);
  return OUT_GPIO(pin);
}

uint32_t gpio_alternate_function(uint32_t pin, uint32_t alternate) {
  INP_GPIO(pin);
  return SET_GPIO_ALT(pin, alternate);
}

uint32_t gpio_set(uint32_t pins) {
  return GPIO_SET = pins;
}

inline uint32_t gpio_clear(uint32_t pins) {
  return GPIO_CLR = pins;
}

int gpio_read(int port)
{
	return( (GPIO_GET & (1<<port)) ? 1 : 0);
}

uint32_t gpio_get(uint32_t pins) {
  return GPIO_GETBIT(pins);
}
