CFLAGS = -g -Wall
LDFLAGS = -g  -Wall -lm

.PHONY: clean

all: rpiCC2500 spidev_test

clean:
	rm rpiCC2500 spidev_test *.o
	
gpio.o: gpio.c

spi.o: spi.c

driver.o: driver.c

main.o: main.c

spidev_test.o: spidev_test.c

spidev_test: spidev_test.o
	$(CC) $(LDFLAGS) -o $@ spidev_test.o

rpiCC2500: main.o driver.o gpio.o spi.o
	$(CC) $(LDFLAGS) -o $@ main.o driver.o gpio.o spi.o

it: all
run: all
	sudo ./rpiCC2500
