/*
 * spi.c:
 *	Spi interface.
 *	Copyright (C) 2013 Roman Gassmann
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
 *	GMN	Roman Gassmann	gmn@gr-solutions.net
 *
 * History:
 *	2013-12-01	GMN	Program prepared for realease
 */
 
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include<string.h>

#include "spi.h"

// Reference:
//   https://www.kernel.org/doc/Documentation/spi/
//   https://www.kernel.org/doc/Documentation/spi/spi-summary
//   https://www.kernel.org/doc/Documentation/spi/spidev


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define SPI_DEFAULT_DELAY	0
#define SPI_DEFAULT_MODE	SPI_MODE_0
#define SPI_DEFAULT_BPW	8
#define SPI_DEFAULT_SPEED	10000000

int spi_config(int fd, uint8_t mode, uint8_t bits, uint32_t speed, uint16_t delay){

	//spi mode
	if ( ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1 ){
		printf ("[%s] Can't set SPI mode.\n", __func__);
		return -1;
	}

	// bits per word
	if( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1 ){
		printf ("[%s] Can't set SPI bits per word.\n", __func__);
		return -1;
	}

	// max speed hz
	if( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1 ){
		printf ("[%s] Can't set SPI max speed.\n", __func__);
		return -1;
	}

	return 1;
}

int CC_SPI_Init(const char* dev) {

	/*if ( check_kernel_module("spidev") < 0 ){
		printf ("[%s] Kernel module \"spidev\" not loaded.\n", __func__);
		return -1;
	}
	
	if ( check_kernel_module("spi_bcm2708") < 0 ){
		printf ("[%s] Kernel module \"spi_bcm2708\" not loaded.\n", __func__);
		return -1;
	}	*/

	int fd = open(dev, O_RDWR);
	if ( fd < 0 ){
		printf ("[%s] Can't open SPI device.\n", __func__);
		return -1;
	}else{
		// device open, config default values
		if ( spi_config(fd, SPI_DEFAULT_MODE, SPI_DEFAULT_BPW, SPI_DEFAULT_SPEED, SPI_DEFAULT_DELAY) ){
			return fd;
		}else{
			printf ("[%s] Can't set default SPI config.\n", __func__);
			return -1;
		}
	}
} 

int CC_SPI_Transfer(int fd, uint8_t txbuf[], uint8_t rxbuf[], uint8_t len){
	//int ret;

	struct spi_ioc_transfer transfer = {
		.tx_buf = (unsigned long)txbuf,
		.rx_buf = (unsigned long)rxbuf,
		.len = len,
		.delay_usecs = SPI_DEFAULT_DELAY,
		.speed_hz = SPI_DEFAULT_SPEED,
		.bits_per_word = SPI_DEFAULT_BPW,
		.cs_change =1,
	};

	if( ioctl(fd, SPI_IOC_MESSAGE(1), &transfer)  < 1 ){
		printf ("[%s] Can't send SPI message.\n", __func__);
		return -1;
	}else{
		return 1;
	}
}



