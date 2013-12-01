/*
 * driver.h:
 *	Routines to interact with CC2500.
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

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <stdint.h>

#include "TI_CC_CC1100-CC2500.h"

typedef struct cc {
	char * dev;
	int fd;
	int GDO_pin_mask;
	int gd0_pin;
	
} TICC;

int CC_Reset(int cs_pin);

void writeRFSettings(TICC *cc);

void RFSendPacket(TICC *cc, char *, char);

char RFReceivePacket(TICC *cc, char *, char *);

int CC_Init( TICC *cc, char *dev, int GD0_pin );

void TI_CC_SPIWriteReg(int fd, char addr, char value);

void TI_CC_SPIWriteBurstReg(int fd, char addr, char *buffer, char count);

char TI_CC_SPIReadReg(int fd, char addr);

void TI_CC_SPIReadBurstReg(int fd, char addr, char *buffer, char count);

char TI_CC_SPIReadStatus(int fd, char addr);

void TI_CC_SPIStrobe(int fd, char strobe);


#endif
