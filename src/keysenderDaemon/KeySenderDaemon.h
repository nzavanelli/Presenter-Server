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
 * KeySenderDaemon.h
 *
 *  Created on: 24.02.2018
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_KEYSENDERDAEMON_KEYSENDERDAEMON_H_
#define SRC_KEYSENDERDAEMON_KEYSENDERDAEMON_H_

#include <QObject>
#include <QTcpServer>

/**
 * The key sender daemon. Will listen on a network port and emit key presses
 * once it receives the corresponding message.
 */
class KeySenderDaemon: public QObject {
    Q_OBJECT

    public:
        /**
         * Creates a new daemon instance and starts up the server.
         */
        KeySenderDaemon();

        /**
         * Stops the server instance.
         */
        ~KeySenderDaemon();

    public slots:
        /**
         * Handler for incomming network connections.
         */
        void newConnection();

        /**
         * The handler for data available to read.
         */
        void readyRead();

        /**
         * Handler for disconnecting clients. Will close the sender if called.
         */
        void disconnected();

    private:
        /**
         * The server instance.
         */
        QTcpServer* server;
};

#endif /* SRC_KEYSENDERDAEMON_KEYSENDERDAEMON_H_ */
