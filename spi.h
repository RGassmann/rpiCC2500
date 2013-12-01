/*
 * spi.h:
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
 
#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>

int CC_SPI_Init(const char* dev);

int CC_SPI_Transfer (int fd, uint8_t txbuf[], uint8_t rxbuf[], uint8_t len);

#endif