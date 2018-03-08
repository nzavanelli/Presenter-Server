## Presenter Server for Windows and Linux [![Build Status](https://travis-ci.org/FelixWohlfrom/Presenter-Server.svg?branch=master)](https://travis-ci.org/FelixWohlfrom/Presenter-Server)

TL;DR: Check how to [run](#how-to-run).

This is a simple reference server implementation for the remote control protocol. Currently it can be used to remote control a presentation e.g. on your notebook e.g. via an android smartphone.

It supports remote control via bluetooth. Remote controling via network (e.g. wifi) is also planned.

A reference client for android can be found [here](https://github.com/FelixWohlfrom/Presenter-Client-Android).

### Supported Systems
The following systems are currently supported:
- Windows 10
- Ubuntu 16.04 LTS
- Ubuntu 17.10 (on both wayland and X server)

On Linux write access to /dev/uinput or /dev/input/uinput required - e.g. as root.

### How to Build
Follow these instructions how to build the presenter server from source.

#### Dependencies
The following dependencies are required to build the presenter server:
- [Qt](https://www.qt.io/)
    - The version that was used to build a released version can be found in the "About" window.
    - On Windows, I try to build it with the latest version of qt.
    - On Linux, the versions delivered with the [Supported Systems](#supported-systems) are used.
- [cmake](https://cmake.org/)
    - The minimum required version can be found in the root [CMakeLists.txt](src/CMakeLists.txt).
- A compiler that is supported by cmake. E.g. gcc (recommended) or Visual Studio.

On Windows:
- Download and install the required packages
- For QT it is recommended to install also "MinGW" in the "Tools" section to have gcc as compiler available

On Ubuntu:  
Install the following packages: **cmake qt5-default qttools5-dev-tools qtconnectivity5-dev g++**

#### Build
Once the dependencies are installed, you can prepare the build. This is similar on Windows and Linux:
- Create a directory where you want to build the binaries. In the following steps, this will be called *build*.
- Call cmake to generate the build files in this *build* directory. You can do this by using *cmake-gui* for a graphical interface or the *cmake* command line interface.  
As source folder, please select the [src](src) folder in this directory.
- Now you will find the build files in the *build* directory.
    - If you generated buildfiles for make, you can call now "make" in the *build* directory.
    - If you generated build files for Visual Studio, you can open the generated project now in Visual Studio
    - For any other build system, check the according documentation

### How to Run
#### Official releases
If you downloaded the binaries from [the official releases](../../releases), simply unpack the binaries.
On Windows just run the **Presenter_Server.exe** with a double click.
For Linux you need to build it from [source](#how-to-build) for now.

#### Build from source
On Windows run **_build_/main/Presenter_Server.exe**.
On Linux run **_build_/presenter_server.sh**.
