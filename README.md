# ESP RainMaker Liftmaster MJ5011U Controller

## Introduction

This is the repo for the controller of a liftmaster motor, this can be used in any other model compatible. It can be flashed in any ESP32 that is compatible with rainmaker <br>

## Program the ESP

To program the microcontroller you must have espressif IDF sdk to compile and flash your device.

After installed use the next commands:

```

$ export ESPPORT=/dev/tty.SLAB_USBtoUART #Set your ESP32's serial port here
-or-
$ Set-Item -Path Env:ESPPORT -Value COM# #Use this if in Powershell and using UART to flash

$ idf.py set-target esp32 # Set target platform to esp32, esp32s2, esp32c3 or esp32s3 
$ idf.py build
$ idf.py erase_flash
$ idf.py flash monitor

```

Once flashed the device will show you a unique QR code, scan this with the rainmaker app to add the device.

## Connections to board

The required connections are pretty simple, there are two screw terminals at the device, one has 3 holes, here is where we connect wires to the board (OPEN/COM/CLOSE), The screw terminal with two holes is connected to the magnetic switch. 






<table border="hidden">
<tr>
<td><img align="left" height="350" alt="image" src="imgs\liftmaster board.jpg" ></td>
<td><img align="right" height="300" alt="image" src="imgs\terminals.jpg"></td>
</tr>
</table>


## Schematics

This are the suggested schematics for the project, of course you can get ready to use modules, but I wanted to complicate my existence :)

<img height="500" alt="image" src="imgs\Schematic.png" style="max-width: 100%; display: inline-block;">

## Supported ESP-IDF versions

ESP RainMaker can work with ESP IDF 4.1 and above.

## Phone Apps

### Android

- [Google PlayStore](https://play.google.com/store/apps/details?id=com.espressif.rainmaker)
- [Direct APK](https://github.com/espressif/esp-rainmaker/wiki)
- [Source Code](https://github.com/espressif/esp-rainmaker-android)

### iOS
- [Apple App Store](https://apps.apple.com/app/esp-rainmaker/id1497491540)
- [Source Code](https://github.com/espressif/esp-rainmaker-ios)


