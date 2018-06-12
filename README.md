# Balloon-Transponder
This repository will have the Software for the Orange PI SDR transponder.
This software controle the SDR transmitter board and RTL-SDR dongle for RX.
The link to the SDR Transmitter board https://github.com/antonjan/IQ_Modulatori<br>
Ubuntu Operating system.<br>
<br>
    rtl-tcp
    csdr
    cw
    direwolf
    i2c libraries for orange pi
    Alsa for the IQ audio generation and reading
    rtl-sdr
    sox
    Orange Pi IO pin manipulation libraries to control the chip select.
    Python for household.
    Crontab for scheduling

Maybe I will be able to run gnu Radio.

We now need to download the operating system


Orange Pi Zero Plus2 H3

    Debian Desktop Jessie
    https://mega.nz/#!kk8U2Zwa!xEO525RNtBxSR4jJGMrzbEXd-aMuhvV-vjRy_NdPHUE

Unrar the image

unrar debian_desktop_jessie_zeroplus2_h3_V0_1 .rar

now write the img file to the SD card

Make sure you have the correct device otherwise you will delete your hard rive


dd if=sdimage.img of=/dev/sdc bs=4M

sync

Then unmounted the sd card and put it in the Orange PI to boot up.


I used a serial debug cable to connect to the Orange pi. (Available from Giga Technology)






# Status
This Project has just started and is not compleet yet.

