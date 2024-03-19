# Pylontech Battery Monitor

## Supported Hardware:
Raspberry PI: recommended for lowest power consumption is the Raspberry PI Zero W,
but any other SBC is also compatible.

## Connection:
Connect the Console connector of the master battery to the primary serial interface of the Raspberry Pi using an RS-232 to TTL (3.3v) converter.

## Install required libraries:
Run the script named prepare to install necessary libraries.

## Build the software:
make clean<br>
make<br>
This process will create the executable file named `pylonmonitor`.
## Run the software:
Run the software with root privileges by executing `./pylonmonitor`.

## Raspi supporting WLAN (RPI3, 4 and Zero-W) need remapping of serial port:
1. Disable serial login shell (important !!! Double check this step)<br>
    Enter `sudo raspi-config` in the terminalbr>
    InterfaceOptions - Serial Port:<br>
    login shell: NO<br>
    hardware enabled: YES<br>
2. map primary serial port to connector pins<br>
    sudo nano /boot/config.txt and add at the end:<br>
    dtoverlay=miniuart-bt<br>
3. Reboot and verify configuration:<br>
    ls -l /dev/serial*<br>
    serial0 should map to ttyAMA0<br>
4. Programs using `ttyAMA0` must be run with root privileges

## Number of Pylontech Batteries:
The software automatically detects the number of connected Pylontech batteries.

## Autostart (for RPI only):
Refer to the autostart.txt file for instructions on setting up the software to start automatically.

## Using the pylontech battery monitor:
1. via Web interface: Access the Raspberry Pi's IP address in a web browser.<br>
2. Click on "SETUP" in the top right corner of the web interface to configure network settings and MQTT.<br>
3. Use tools like MQTT Explorer to verify that everything is working as expected

![Screenshot of Pylontech Battery Monitor](pics/screenshot.png)
