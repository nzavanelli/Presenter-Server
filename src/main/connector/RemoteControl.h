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
 * RemoteControl.h
 *
 *  Created on: 09.11.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_REMOTECONTROL_H_
#define SRC_MAIN_CONNECTOR_REMOTECONTROL_H_

#include <QObject>
#include <QString>

class RemoteControl: public QObject
{
    Q_OBJECT

    public:
        virtual ~RemoteControl();

        /**
         * Starts a new server.
         */
        virtual void startServer() = 0;

        /**
         * Stops the running server.
         */
        virtual void stopServer() = 0;

    protected:
        /**
         * Callback method called once a new client connected.
         *
         * @param name The name of the connected client.
         */
        void handleClientConnected(const QString& name);

        /**
         * Callback to handle a complete line read from given sender.
         *
         * @param sender The sender that sent the line.
         * @param line The line to handle.
         */
        void handleLine(const QString& sender, const QString &line);

        /**
         * Write a given message to the connected client.
         *
         * @param message The message to write.
         */
        virtual void write(const QString& message) = 0;

    private:
        /**
         * A not completely received message. A message is complete once
         * two consecutive new line characters are received.
         */
        QString messagePart;

        /**
         * Will handle a complete remote protocol message from given sender.
         *
         * @param sender The sender that sent the message.
         * @param message The message to handle.
         */
        void handleMessage(const QString& sender, const QString &message);

    signals:
        /**
         * Will be emitted when the connector wants to show some information.
         *
         * @param message The message to show
         */
        void info(const QString &message);

        /**
         * Signals an error.
         *
         * @param message The error message
         */
        void error(const QString &message);

        /**
         * Emitted once the server is ready to accept client connections.
         */
        void serverReady();

        /**
         * Signals that a new client is now connected.
         *
         * @param name The name of the connected client
         */
        void clientConnected(const QString &name);

        /**
         * Signals that a client disconnected.
         *
         * @param name The name of the disconnected client
         */
        void clientDisconnected();

        /**
         * Signals that a key event has been sent.
         *
         * @param name The name of the client that sent the key
         * @param key The key that has been sent
         */
        void keySent(const QString &name, const QString &key);
};

#endif /* SRC_MAIN_CONNECTOR_REMOTECONTROL_H_ */
