/*
 * main.c:
 *	Main Application for interacting with CC2500 from Ti.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#include "driver.h"
//#include "TI_CC_CC1100-CC2500.h"

#include "gpio.h"
#include "spi.h"

#include <math.h>

extern char paTable[];
extern char paTableLen;

int main(int argc, char ** argv) {
	int res;
	printf("Start rpiCC2500\n");

	TICC *cc = (TICC *) malloc(sizeof(TICC));

	res = CC_Init(cc, "/dev/spidev0.0", 25 /*GNO0-pin Not yet used*/);
	if (res < 0) {
		printf("Failed to init TICC.");
		return 1;
	}
	perror("SPI init");
	usleep(30000);
	TI_CC_SPIStrobe( cc->fd, TI_CCxxx0_SRES);
	printf("Wait for RF to be Ready\n");
	usleep(40);
	printf("RF ok\n");
		
	writeRFSettings(cc);
	printf("BurstReg\n");
	TI_CC_SPIWriteBurstReg(cc->fd, TI_CCxxx0_PATABLE, paTable, paTableLen);
	printf("Strobe\n");
	//TI_CC_SPIStrobe(cc->fd, TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
										// When a pkt is received, it will
										// signal on GDO0
	char rxBuffer[50];
	printf("entering LOOP\n");
	printf("Part No.: %d\n",TI_CC_SPIReadStatus(cc->fd, TI_CCxxx0_PARTNUM));
	printf("Version No.: %d\n",TI_CC_SPIReadStatus(cc->fd, TI_CCxxx0_VERSION));
	char len = 50;
	int j;
	for(j=0;j<10;j++){  // for now send every 500ms a message and check for received packages
		rxBuffer[0] =3;
		rxBuffer[1] =0x01;
		rxBuffer[2] =12;
		rxBuffer[3] =rxBuffer[1]^rxBuffer[2]^0x01;
		printf("Send Package\n");
		RFSendPacket(cc, rxBuffer, 4);
		usleep(500000);
		
		printf("Check for Package\n");
		while(RFReceivePacket(cc, rxBuffer, &len)){
				printf("Got A Package %d\n",len);
				for(i =0; i< len; i++){
						printf("%d ",rxBuffer[i]);
				}
				printf("\n");
				//Receiveddata are stored in rxBuffer
				//Put some intelligence here
				len = 50;
		}
		
	}

	printf("exiting LOOP\n");
	CC_dispose(cc);
	free(cc);

	return 0;
}
