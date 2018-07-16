# Balloon-Transponder
This repository will have the Software for the Orange PI SDR transponder.<br>
This software controle the SDR transmitter board and RTL-SDR dongle for RX.<br>
The link to the SDR Transmitter board <a href="https://github.com/antonjan/IQ_Modulatori">https://github.com/antonjan/IQ_Modulatori</a><br>
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
ok now we need to sink our time zone<br>
dpkg-reconfigure tzdata<br>
we now need to enable the I2C bus and I used the following utility.<br>
sudo armbian-config<br>
select Hardware - toggle board low level functions: UART, I2C, SPI, …
Enable the I2C bus as we need it to controle the Local Ocelattor and Read output power from A/D converter<br>
Ok We now need to install all the dependinsy applications reqierd to controle the RTL recever and IQ modulator.<br>
Lets isntall the SDR reseption applications for the RTL dongle and iarspy<br>

Lets install the aplication that will create the CW for the Telemetry Beacon.<br>
sudo apt-get install cw
cw
aplay -l
sudo aplay -l
sudo alsamixer
sudo armbianmonitor -m
nmtui
   

28  su - balloon
   29  sudo rtl_test
   30  cd
   31  git clone https://github.com/simonyiszk/csdr.git
   32  cd csdr/
   33  ls
   34  cd
   35  sudo apt-get install libfftw3-dev
   36  cd csdr/
   37  make
   38  sudo make install
   39  csdr
   40  cd
   41  sudo apt-get install cw
   42  cw -h
   43  cw test 123
   44  man cw
   45  echo "zr6aic balloon experiment" | cw -w 25 -t 700
   46  aplay -l
   47  sudo aplay -l
   48  speacker-test
   49  speaker-test
   50  cat /proc/asound/cards
   51  pqd
   52  cd /
   53  ls
   54  sudo vi /boot/armbianEnv.txt
   55  sudo shutdown -r 0
   56  aplay -l
   57  sudo aplay -l
   58  sudo arecord -l
   59  echo "zr6aic balloon experiment" | cw -w 25 -t 700



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

