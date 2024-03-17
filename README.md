# Pylontech Battery Monitor

## Supported Hardware:
Raspberry PI, recommended for lowest power consumption: Raspberry PI Zero W
or any other SBC

## Connection:
Connect the Console connector of the master battery via an RS-232 to TTL (3,3v) converter to the primary serial interface of the RPI

## Install required libraries:
run the script: prepare

## Build the software:
make clean<br>
make<br>
this build the executable file: pylonmonitor

## Run the software:
run as root:  ./pylonmonitor

## Raspi supporting WLAN (RPI3, 4 and Zero-W) need remapping of serial port:
1. Switch-off serial login shell (important !!!, double check)<br>
    sudo raspi-config<br>
    InterfaceOptions - Serial Port:<br>
    login shell: NO<br>
    hardware enabled: YES<br>
2. map primary serial port to connector pins<br>
    sudo nano /boot/config.txt and add at the end:<br>
    dtoverlay=miniuart-bt<br>
3. Reboot and check if ok:<br>
    ls -l /dev/serial*<br>
    serial0 must map to ttyAMA0<br>
4. to use ttyAMA0 the program must be running as root

## Number of Pylontech Batteries:
this software detects the number of batteries automatically

## Autostart (for RPI only):
see file: autostart.txt

## Using the pylontech battery monitor:
1. via Web interface: open the IP of the Raspberry PI in the browser<br>
2. via MQTT: configure the IP address of the MQTT broker in file: mqttthread.h. The topic name can be configure in readbatt.h. These settings will be done via webinterface in a later version of this software. This MQTT client does not use password or authentication. If your MQTT broker requires it, you need to activate it in file: mqttthred.cpp, function init().

![Screenshot of Pylontech Battery Monitor](pics/screenshot.png)
