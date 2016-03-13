# RcCar

install boost library


add this line in ~/.profile
    export PiCCPath="$HOME/RaspberryCC/"  # make adaptions in to your CrossCompiler path
    



Sync Sysroot   change username and ip
    rsync -rav --include='/usr' --include='/usr/include/***' --exclude='/usr/lib/python*/' --exclude='/usr/lib/pypy*/' --exclude='/usr/lib/pymodules/' --exclude='/usr/lib/pyshared/' --exclude='/usr/lib/ruby*/' --exclude='/usr/lib/libreoffice/' --exclude='/usr/lib/jvm/' --include='/usr/lib/***' --exclude='/lib/modules/' --exclude='/lib/udev/' --exclude='/lib/firmware/' --exclude='/lib/systemd/' --include='/lib/***' --exclude='*' pi@192.168.178.29:/* . 
