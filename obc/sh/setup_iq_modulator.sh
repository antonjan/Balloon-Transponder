#!/bin/bash
# The ORange Pi use the following library https://github.com/xpertsavenue/i2cTools-Zero
#https://github.com/ua3nbw/Si570-
#1) Send I2C configuration to I535 to set Local Oscillator frequency.
#2) Enable IQ modulator by Enabling the Chip select pin.
#3) Read RF power output by reading I2C form A/D.
#3) Enable PA stage by enabling the Chip select pin.
#4) Read Rf power and check if PA amp is working.
#5) Send small burst of Modulation via alsa
# init frequency of i535
if [ $# -lt 1 ]
then
	echo "Usage: $0  <freq  HZ> and mode <on/off $1 >"
  exit 1
fi
#list all I2C busses
ls -l /dev/i2c-*
#Check what devicess is on I2C bus
sudo i2cdetect -y 1
#set LO freqency
./si570 -h #Display help
./si570 -s 145.854 #set the frequency
./si570 -i #display register details

