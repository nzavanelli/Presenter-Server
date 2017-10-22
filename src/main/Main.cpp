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
 * Main.cpp
 *
 *  Created on: 14.07.2017
 *      Author: Felix Wohlfrom
 */
#include <QApplication>
#include <QMessageBox>

#include "gui/MainWindow.h"

#ifdef _DEBUG
    #ifdef _WIN32
        #include <iostream>
        #include <io.h>
        #include <Fcntl.h>
        #include <Windows.h>

        #ifdef _MSC_VER
            // Uncomment this if you installed visual memory
            // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

int main(int argc, char *argv[])
{
    // Enable debug console on win32
    #ifdef _DEBUG
        #ifdef _WIN32
            int hConHandle;
            long long lStdHandle;
            FILE *fp;
            // allocate a console for this app
            AllocConsole();

            // redirect unbuffered STDOUT to the console
            lStdHandle = (long long)GetStdHandle(STD_OUTPUT_HANDLE);
            hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
            fp = _fdopen(hConHandle, "w");
            *stdout = *fp;
            setvbuf(stdout, NULL, _IONBF, 0);
        #endif
    #endif

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Presenter"),
                QObject::tr("System tray not detected on this system"));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow window;
    window.show();
    return app.exec();
}
