/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Presenter. Server software to remote control a presentation.         *
 *  Copyright (C) 2017 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * key_sender.c
 *
 *  Created on: 13.08.2017
 *      Author: Felix Wohlfrom
 */

#include "key_sender.h"

#ifdef _WIN32
    #include <windows.h>

    void send_key(int key)
    {
        // This structure will be used to create the keyboard
        // input event.
        INPUT ip;

        // Set up a generic keyboard event.
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;

        // Press the given key
        ip.ki.wVk = key;
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));

        // Release the given key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    }
#endif // _WIN32

#ifdef __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <linux/uinput.h>

    #define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

    int fdo;

    void emit_key(int fd, int type, int code, int val)
    {
        struct input_event ie;

        ie.type = type;
        ie.code = code;
        ie.value = val;

        ie.time.tv_sec = 0;
        ie.time.tv_usec = 0;

        write(fd, &ie, sizeof(ie));
    }

    void send_key(int key)
    {
        emit_key(fdo, EV_KEY, key, 1);
        emit_key(fdo, EV_SYN, SYN_REPORT, 0);
        emit_key(fdo, EV_KEY, key, 0);
        emit_key(fdo, EV_SYN, SYN_REPORT, 0);
    }
#endif // __linux__

void init_keysender()
{
    #ifdef __linux__
        struct uinput_user_dev uidev;

        fdo = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fdo < 0)
        {
            die("error: open on /dev/uinput");
        }

        if (ioctl(fdo, UI_SET_EVBIT, EV_KEY) < 0)
        {
            die("error: ioctl: EV_KEY");
        }
        if (ioctl(fdo, UI_SET_KEYBIT, KEY_LEFT) < 0)
        {
            die("error: ioctl: KEY_LEFT");
        }
        if (ioctl(fdo, UI_SET_KEYBIT, KEY_RIGHT) < 0)
        {
            die("error: ioctl: KEY_RIGHT");
        }
        memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "slideChanger");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor  = 0x1;
        uidev.id.product = 0x1;
        uidev.id.version = 1;

        if (write(fdo, &uidev, sizeof(uidev)) < 0)
        {
            die("error: ioctl: write");
        }
        if (ioctl(fdo, UI_DEV_CREATE) < 0)
        {
            die("error: ioctl: UI_DEV_CREATE");
        }
    #endif // __linux__
}

void send_next()
{
    #ifdef _WIN32
        send_key(VK_RIGHT);
    #endif // _WIN32

    #ifdef __linux__
        send_key(KEY_RIGHT);
    #endif // __linux__
}

void send_prev()
{
    #ifdef _WIN32
        send_key(VK_LEFT);
    #endif // _WIN32

    #ifdef __linux__
        send_key(KEY_LEFT);
    #endif // __linux__
}

void destroy_keysender()
{
    #ifdef __linux__
        if (ioctl(fdo, UI_DEV_DESTROY) < 0)
        {
            die("error: ioctl - UI_DEV_DESTROY");
        }

        close(fdo);
    #endif // __linux__
}
