# rc-boat-depth-chart-generator
## This repository is structured by development environments.
### arduino-dev
#### This directory contains sketches developed in the Arduino IDE. These sketches are flashed via the UART0 pins on an ATmega1284 (Arduino bootloader). The reason for using the Arduino IDE is to utilize the well-supported libraries for writing to the SD card reader, and reading data from the GPS and Ping sensors. These libraries translate to the pinout/clock-rate of the ATmega1284 with the MightyCore arduino core. 
### atmel-dev 
#### This directory contains programs developed in Atmel Studio 7. These programs are flashed on an ATmega1284 (no bootloader) via SPI interface by compiling the programs in the IDE, and using avrdude via the RPi command line. 
### python-dev
#### This directory contains old python programs to parse NMEA0183 sentences from the GPS.
