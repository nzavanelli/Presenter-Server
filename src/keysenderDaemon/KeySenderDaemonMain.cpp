/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Presenter. Server software to remote control a presentation.         *
 *  Copyright (C) 2018 Felix Wohlfrom                                    *
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
 * KeySenderDaemonMain.cpp
 *
 *  Created on: 24.02.2018
 *      Author: Felix Wohlfrom
 */

#include <QCoreApplication>

#include "KeySenderDaemon.h"

#include <signal.h>

/**
 * The signal handler. Gracefully closes our daemon.
 */
void handleShutDownSignal(int /* signalId */)
{
    QCoreApplication::exit(EXIT_SUCCESS);
}

/**
 * Registers our handler for a given unix signal.
 */
void setShutDownSignal(int signalId)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handleShutDownSignal;
    if (sigaction(signalId, &sa, NULL) == -1)
    {
        perror("setting up termination signal");
        exit(EXIT_FAILURE);
    }
}

/**
 * Main method of the keysender daemon.
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    setShutDownSignal(SIGINT); // shut down on ctrl-c
    setShutDownSignal(SIGTERM); // shut down on killall

    // Start the key sender daemon
    KeySenderDaemon sender;
    return app.exec();
}
