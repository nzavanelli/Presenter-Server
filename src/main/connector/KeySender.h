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
 * KeySender.h
 *
 *  Created on: 25.02.2018
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_KEYSENDER_H_
#define SRC_MAIN_CONNECTOR_KEYSENDER_H_

#include <QObject>

#ifdef __linux__
    #include <QTcpSocket>
#endif // __linux__

/**
 * This key sender class will redirect the keysender calls to the platform
 * specific implementations. For windows this will be a direct call to the
 * native implementation. On linux, this will be a call to the keysender daemon
 * that needs to run with elevated privileges to the input device.
 */
class KeySender: public QObject
{
    Q_OBJECT

    public:
        /**
         * Creates a new keysender instance for the specific platform.
         */
        KeySender();

        /**
         * Cleanup the keysender instance.
         */
        virtual ~KeySender();

        /**
         * Sends the key to switch to next slide.
         */
        void sendNext();

        /**
         * Sends the key to switch to previous slide.
         */
        void sendPrev();

        // FIXME: After dropping ubuntu 16.04 support, this can be moved into
        // the #ifdef __linux__ block
        signals:
            /**
             * Signals an error.
             *
             * @param message The error message
             */
            void error(const QString& message);

    #ifdef __linux__
        private slots:
            /**
             * Handler for errors during connection to keysender daemon.
             *
             * @param socketError The socket error
             */
            void socketError(const QAbstractSocket::SocketError socketError);

        private:
            /**
             * The socket that connects to the keysender daemon.
             */
            QTcpSocket* socket;
    #endif // __linux__
};

#endif /* SRC_MAIN_CONNECTOR_KEYSENDER_H_ */
