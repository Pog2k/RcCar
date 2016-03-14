# RcCar

# install boost library

    sudo apt-get install libboost-all-dev

# add this line in ~/.profile
    export PiCCPath="$HOME/RaspberryCC/"  # make adaptions in to your CrossCompiler path
    



# Sysroot vom pi mit host pc synchronisieren. Auf dem pi installierte Pakete können nun mit dem CrossCompiler verwendet werden.         

# change username and ip
    rsync -rav --include='/usr' --include='/usr/include/***' --exclude='/usr/lib/python*/' --exclude='/usr/lib/pypy*/' --exclude='/usr/lib/pymodules/' --exclude='/usr/lib/pyshared/' --exclude='/usr/lib/ruby*/' --exclude='/usr/lib/libreoffice/' --exclude='/usr/lib/jvm/' --include='/usr/lib/***' --exclude='/lib/modules/' --exclude='/lib/udev/' --exclude='/lib/firmware/' --exclude='/lib/systemd/' --include='/lib/***' --exclude='*' pi@192.168.178.29:/* . 




# Raspi 3    Remap UART 
#    First update:

    sudo apt-get update
    sudo apt-get upgrade
    sudo apt-get dist-upgrade
    sudo rpi-update


#    To disable onboard Pi3 Bluetooth and restore UART0/ttyAMA0 over GPIOs 14 & 15 modify:

    sudo nano /boot/config.txt

#    Add to the end of the file

    dtoverlay=pi3-disable-bt

#    We also need to run to stop BT modem trying to use UART

    sudo systemctl disable hciuart


#    Reboot and test serial coms with:

    minicom -b 115200 -o -D /dev/ttyAMA0
