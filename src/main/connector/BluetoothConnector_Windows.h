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
 * BluetoothConnector_Windows.h
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_WINDOWS_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_WINDOWS_H_

#include "BluetoothConnectorBase.h"

#include <QThread>

#include <winsock2.h>
#include <strsafe.h>

/**
 * A simple thread that will read data received by a given bluetooth socket.
 * It emits several signals, e.g. if a client connected, a client disconnected
 * or a new line was received from a client.
 */
class BluetoothReaderThread: public QThread
{
    Q_OBJECT

    public:
        /**
         * Creates the mew server thread for a given socket.
         *
         * @param serverSocket The socket to listen to.
         */
        BluetoothReaderThread(const SOCKET serverSocket);

        /**
         * Will send a stop signal to the server thread. The server will stop
         * on one of the next iterations of its event loop.
         */
        void stop();

    signals:
        /**
         * Signals an error.
         *
         * @param message The error message
         */
        void error(const QString &message);

        /**
         * Signals that a new client is now connected.
         *
         * @param name The name of the connected client.
         */
        void clientConnected(const QString &name);

        /**
         * Signals that a client disconnected.
         */
        void clientDisconnected();

        /**
         * Signals that a new line has been received.
         *
         * @param name The name of the client that sent the line
         * @param line The line that has been received.
         */
        void lineReceived(const QString &name, const QString &line);

    private:
        /**
         * The server socket instance.
         */
        SOCKET serverSocket;

        /**
         * The client socket instance. NULL if no client is connected.
         */
        SOCKET clientSocket;

        /**
         * If the thread should continue running.
         */
        boolean keepRunning;

        /**
         * Main method of the thread.
         */
        void run();
};

/**
 * The windows specific implementation of BluetoothConnectorBase.
 * Will create a new bluetooth server socket and wait until a client is
 * connected. To start the server, call the {@link #startServer} method, to
 * stop it, call {@link #stopServer}.
 */
class BluetoothConnector: public BluetoothConnectorBase
{
    Q_OBJECT

    public:
        /**
         * Creates a new bluetooth connector server.
         */
        BluetoothConnector();

        /**
         * Deletes the bluetooth connector.
         * Will stop running servers.
         */
        ~BluetoothConnector();

        /**
         * Starts a new server.
         */
        void startServer();

        /**
         * Stops the running server.
         */
        void stopServer();

        /**
         * Reads the error returned by WSAGetLastError() and returns the
         * message string for the error code.
         *
         * @return The message string from WSAGetLastError()
         */
        static QString getLastWSAError();

    private:
        /**
         * Our server socket.
         */
        SOCKET serverSocket;

        /**
         * Stores socket information about our bluetooth socket.
         */
        LPCSADDR_INFO socketInfo;

        /**
         * The socket instance name.
         */
        STRSAFE_LPWSTR instanceName;

        /**
         * The thread that will read the data received by our server socket.
         */
        BluetoothReaderThread* readerThread;

   private slots:
        /**
         * Signal handler if an error happened in the reader thread.
         *
         * @param error The error message
         */
        void errorThread(const QString &error);

        /**
         * Signal handler if a client connected in the reader thread.
         *
         * @param name The name of the connected client
         */
        void clientConnectedThread(const QString &name);

        /**
         * Signal handler if a client disconnected in the reader thread.
         */
        void clientDisconnectedThread();

       /**
        * Signal handler if a line has been received in the reader thread.
        *
        * @param name The name of the client that sent the line
        * @param line The line that has been received
        */
        void lineReceived(const QString &name, const QString &line);
};

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_WINDOWS_H_ */
