# Balloon-Transponder
This repository will have the Software for the Orange PI SDR transponder.
This software controle the SDR transmitter board and RTL-SDR dongle for RX.
The link to the SDR Transmitter board https://github.com/antonjan/IQ_Modulatori<br>
Ubuntu Operating system.<br>
<br>
    rtl-tcp<br>
    csdr<br>
    cw<br>
    direwolf<br>
    i2c libraries for orange pi<br>
    Alsa for the IQ audio generation and reading<br>
    rtl-sdr<br>
    sox<br>
    Orange Pi IO pin manipulation libraries to control the chip select.<br>
    Python for household.<br>
    Crontab for scheduling<br>
	
    sudo apt-get update
	sudo apt-get install i2c-tools  
        sudo apt-get install python-smbus  
	sudo apt-get install libi2c-dev 
sudo i2cdetect -r 1
WARNING! This program can confuse your I2C bus, cause data loss and worse!
I will probe file /dev/i2c-1 using read byte commands.
I will probe address range 0x03-0x77.
Continue? [Y/n] Y
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                      


Maybe I will be able to run gnu Radio.<br>

# We now need to download the operating system

My Board is the Orange Pi Zero Plus2 H2<br>

    Debian Desktop Jessie<br>
    https://mega.nz/#!kk8U2Zwa!xEO525RNtBxSR4jJGMrzbEXd-aMuhvV-vjRy_NdPHUE<br>

Unrar the image<br>

unrar debian_desktop_jessie_zeroplus2_h3_V0_1 .rar<br>

now write the img file to the SD card<br>

Make sure you have the correct device otherwise you will delete your hard rive<br>


dd if=sdimage.img of=/dev/sdc bs=4M<br>

sync<br>

Then unmounted the sd card and put it in the Orange PI to boot up.<br>


I used a serial debug cable to connect to the Orange pi. (Available from Giga Technology)<br>






# Status
This Project has just started and is not compleet yet.

