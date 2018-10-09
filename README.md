# Balloon-Transponder
This repository will have the Software for the Orange PI SDR transponder.<br>
This software controle the SDR transmitter board and RTL-SDR dongle for RX.<br>
The link to the SDR Transmitter board <a href="https://github.com/antonjan/IQ_Modulatori">https://github.com/antonjan/IQ_Modulator</a><br>
# Project Status
This project has just started 15 June 2018.<br>
The code is not compleet yet but hardware board is compleet.<br>
# Block diagram of SDR transponder and OBC<br>
![Alt text](doc/images/Block_Diagram_v2.jpg?raw=true "Block Diagram")<br>
# Flow Diagram
![Alt text](doc/images/Transponder_Signal_Path.jpg?raw=true "Flow Diagram")<br>
# Software instelation prosedure for Orange Pi Zero
Armbian Operating system.<br>
The operating system I recomentd is Armbian and it can be downloaded here <a href="https://www.armbian.com/orange-pi-zero/">https://www.armbian.com/orange-pi-zero/</a><br>
Then unzip the file using the.<br>
unzip Debian_stretch_next.7z
We now need to burn the SD card to be plased in Orange Pi.I would use 16Gb or bigger I think it suports upto 32Gb<br>
Run the following command to burn the file to the SD card.
df -k
Look for the SD card. in my case its /dev/mmcblk0p1<br> 
<b>Make sure you select the corect one as you could delete your hardrive (Warning)</b>
sudo dd bs=1M if=./Debian_stretch_next.img of=/dev/mmcblk0p1<br>
then run sync to make sure all is compleet.<br>
Now place the SD in Orange Pi and connect an ethernet cable from your pi to your network with dhcp enabled.<br>
Power up the Orange Pi an look what is the last Ip alocated on router to a device.<br>
Then connect your Admin pc or laptop to this ip using root<br>
Looking ad the Debian dock here is also a good adiee <a href="https://docs.armbian.com/">https://docs.armbian.com/</a><br>
ssh root@192.168.1.105 was myne but yours will be difrent.<br>
Default password 1234<br>
You will be forsed to change it and add a new user.<br>
Create a new user balloon and give it a password<br>
Then swiche to this user.<br>
su - balloon<br>
Give this user sudo access.<br>
usermod -aG sudo balloon<br>
We can now start configuring your Orange pi<br>
run the following commands.<br>
sudo apt-get update<br>
sudo apt-get upgrade<br>
if you know your wifi id and key the do the following to make yor Pi connect<br>
nmtui-connect SSIDi or run this if you dont now your wifi name nmtui-connect<br>
and enter your Key<br>
#ok now we need to sink our time zone<br>
dpkg-reconfigure tzdata<br>
#we now need to enable the I2C bus and I used the following utility.<br>
sudo armbian-config<br>
select Hardware - toggle board low level functions: UART, I2C, SPI, …
Enable the I2C bus as we need it to controle the Local Ocelattor and Read output RF power level from A/D converter<br>
Ok We now need to install all the dependinsy applications reqierd to controle the RTL recever and IQ modulator.<br>
Lets isntall the SDR reseption applications for the RTL dongle and iarspy<br>

Lets install the aplication that will create the CW for the Telemetry Beacon.<br>
sudo apt-get install cw<br>
cw<br>
aplay -l<br>
sudo aplay -l<br>
sudo alsamixer<br>
sudo armbianmonitor -m<br>
nmtui<br>
   

su - balloon
sudo rtl_test
#Ok we now need to install the csdr DSP signal prosessing libraeries.<br>
#Lets first install the dependensy libraries<br>
sudo apt-get install libfftw3-dev<br>
#we need to install the compiler<br>
sudo pt-get install cmake<br> 
cd<br>
git clone https://github.com/simonyiszk/csdr.git<br>
#Change directory into csdr<br>
cd csdr<br>
#Lets compile the library<br>
make<br>
#Install teh library<br>
sudo make install<br>
#Lets check if csdr is working<br>
csdr<br>
#you should see the following<br>
csdr - a simple commandline tool for Software Defined Radio receiver DSP.<br>
#usage: <br>
#csdr function_name <function_param1> <function_param2> [optional_param] ...<br>
#We now need to install rtl sdr libraeries and drivers<br>
cd<br>
git clone git://git.osmocom.org/rtl-sdr.git<br>
cd rtl-sdr/<br>
mkdir build<br>
cd build<br>
cmake ../<br>
make<br>
sudo make install<br>
sudo ldconfig<br>
sudo apt-get install libusb-1.0-0-dev<br>
sudo apt-get install mplayer<br>
#Ok now we need to connect a rtl usbdongle to the usb port and see if the rtl_test utility is working.<br>
sudo rtl_test<br>
#[sudo] password for balloon:<br> 
#Found 1 device(s):<br>
#0:  Realtek, RTL2838UHIDIR, SN: 00000001<br>
#
#Using device 0: Generic RTL2832U OEM<br>
#Found Rafael Micro R820T tuner<br>
#Supported gain values (29): 0.0 0.9 1.4 2.7 3.7 7.7 8.7 12.5 14.4 15.7 16.6 19.7 20.7 22.9 25.4 28.0 29.7 32.8 33.8 36.4 37.2 38.6 40.2 42.1 43.4 43.9 44.5 48.0 49.6 <br>
#[R82XX] PLL not locked!<br>
#Sampling at 2048000 S/s.<br>
#
#Info: This tool will continuously read from the device, and report if<br>
#samples get lost. If you observe no further output, everything is fine.<br>
#
#Reading samples in async mode...<br>
#Allocating 15 zero-copy buffers<br>
#lost at least 164 bytes<br>
#
#OK lets test rtl and csdr together.<br>
sudo rtl_sdr -s 2400000 -f `python -c "print int($1*1e6)"` -g $GAIN - | csdr convert_u8_f | csdr fir_decimate_cc 10 0.05 HAMMING | csdr fmdemod_quadri_cf | csdr fractional_decimator_ff 5 | csdr deemphasis_wfm_ff 48000 50e-6 | csdr convert_f_i16 |sudo  mplayer -cache 1024 -quiet -rawaudio samplesize=2:channels=1:rate=48000 -demuxer rawaudio -<br>
#we now need to makesure we have the ALSA audio configeration setup so we can use the alsa loopback and the line in and out configured.<br>
#we will be using alsa out to send the I and Q audio to the IQ modulator via alsa using aplay command.<br>
#we will be using loopback 0 for CW and afsk audio libraries to csdr to modulate FM iq stream.<br>

#Instsalling the xwindows and vnc.<br>
sudo apt-get xorg lightdm xfce4 tango-icon-theme gnome-icon-theme<br>
sudo apt-get install xorg lightdm xfce4 tango-icon-theme gnome-icon-theme<br>
sudo apt-get install tightvncserver thunar-volman udisks2 gvfs<br>
sudo apt-get install gnuradio<br>
#Installing gnuradio<br>
sudo apt-get install gnuradio gr-osmosdr<br>
#Test gnuradiobr>
sudo gnuradio-companion <br>
#we now need to install CW<br>
cd<br>
sudo apt-get install cw<br>
cw -h<br>
cw test 123<br>
man cw<br>
echo "zr6aic balloon experiment" | cw -w 25 -t 700<br>
aplay -l<br>
sudo aplay -l<br>
speacker-test<br>
speaker-test<br>
cat /proc/asound/cards<br>
pqd
cd /
ls
sudo vi /boot/armbianEnv.txt<br>
sudo shutdown -r 0<br>
aplay -l<br>
sudo aplay -l<br>
sudo arecord -l<br>
echo "zr6aic balloon experiment" | cw -w 25 -t 700<br>
sudo apt-get install gnuradio gr-osmosdr<br>
sudo gnuradio-companion<br>       
#we now need to install the IQ Test applications<br>
sudo apt-get install python-matplotlib<br>
sudo apt-get install python-tk<br>
git clone https://github.com/jgibbard/iqtool.git<br>
cd iqtool/<br>
chmod 755 *.py<br>
./iqgen.py -h<br>



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
	
sudo apt-get update<br>
sudo apt-get install i2c-tools <br> 
sudo apt-get install python-smbus <br> 
sudo apt-get install libi2c-dev<br> 
sudo i2cdetect -r 1<br>
WARNING! This program can confuse your I2C bus, cause data loss and worse!<br>
I will probe file /dev/i2c-1 using read byte commands.<br>
I will probe address range 0x03-0x77.<br>
Continue? [Y/n] Y<br>
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f<br>
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- <br>
70: -- -- -- -- -- -- -- --                      <br>

The min and maximum the IQ modulator operating frequensy is 140 to 1500Mhz<br>
How can you cotrole the frequency by using the following command.<br>
/home/balloon/Balloon-Transponder/si570/si570 -s 145.855<br>
if you want to read the frequensy <br>
/home/balloon/Balloon-Transponder/si570/si570 -r<br>
Please remeber you will need to Enable the si570 by disabeling Port PA06.=(0)<br>

We now need to set the IQ signal biase volatge to about 2.5V by adjusting the Potensiometers and measuring the the volatge on pin 15 and 16 of the TRF3701.<br>
Its easer to put yur ptobe on the potensiometer wiper pin.<br>

Seting up the IQ audio signal level.<br>
By using the application alsamixer you can sent the level to about 500mv(p-p) onI and Q  for maximum modelation on cap c7 and c12.<br>
Please remeber to enable the IQ modulator you need to disable Port PA07 = 0<br>
Maybe I will be able to run gnu Radio.<br>
we now will create IQ audio test files.


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
<br>
sudo apt-get install gnuradio gr-osmosdr<br>
sudo gnuradio-companion<br> 
#we now need to install the IQ Test applications<br>
sudo apt-get install python-matplotlib<br>
sudo apt-get install python-tk<br>
git clone https://github.com/jgibbard/iqtool.git<br>
cd iqtool/<br>
chmod 755 *.py<br>
./iqgen.py -h<br>

ref: https://github.com/MLAB-project/si570<br>
