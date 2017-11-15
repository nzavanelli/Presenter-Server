## Presenter Server for Windows and Linux [![Build Status](https://travis-ci.org/FelixWohlfrom/Presenter-Server.svg?branch=master)](https://travis-ci.org/FelixWohlfrom/Presenter-Server)

This is a simple reference server implementation for the remote control protocol. Currently it can be used to remote control a presentation e.g. on your notebook e.g. via an android smartphone.

It supports remote control via bluetooth. Remote controling via network (e.g. wifi) is also planned.

A reference client for android can be found [here](https://github.com/FelixWohlfrom/Presenter-Client-Android).

The following systems are currently supported:
- Windows 10
- Ubuntu 17.10 using an Xorg based window manager (tested on kde5 and gnome)
  - Unfortunately the qt5 bluetooth seems to require root access
    to communicate with the bluetooth devices

Currently the user interface is quite limited and mainly for demonstration purpose, but for the future it is planned to have a more user friendly interface.
