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
 * BluetoothConnector_Linux.h
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_LINUX_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_LINUX_H_

#include <QList>

#include <qbluetoothserviceinfo.h>
#include <qbluetoothserver.h>
#include <qbluetoothsocket.h>
#include "BluetoothConnectorBase.h"

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
         * Create a new bluetooth connector.
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

    private slots:
        /**
         * Called if a new client connected.
         */
        void clientConnected();

        /**
         * Called if a client disconnected.
         */
        void clientDisconnected();

        /**
         * Called if new data is available to read.
         */
        virtual void readSocket();

    private:
        /**
         * The bluetooth server.
         */
        QBluetoothServer* rfcommServer;

        /**
         * Our presenter bluetooth service.
         */
        QBluetoothServiceInfo serviceInfo;

        /**
         * The connected clients.
         */
        QList<QBluetoothSocket*> clientSockets;
};

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_LINUX_H_ */
