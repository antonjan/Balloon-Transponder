# All the Shell scripts will be plased here. 
 Setting up IQ Modulator Board

1) Send I2C configuration to I535 to set Local Oscillator frequency.<br>

2) Enable IQ modulator by Enabling the Chip select pin.<br>

3) Read RF power output by reading I2C form A/D.<br>

3) Enable PA stage by enabling the Chip select pin.<br>

4) Read Rf power and check if PA amp is working.<br>

5) Send small burst of Modulation via alsa.<br>

The following script will setup the IQ modulator board for tx or standby.<br>

setup_iq_modulator.sh <freq> <on/off><br>

