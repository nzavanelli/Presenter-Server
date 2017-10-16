/*
 * BluetoothConnector_Linux.h
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_LINUX_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_LINUX_H_

#include "BluetoothConnectorBase.h"

#include <QList>

#include <qbluetoothserviceinfo.h>
#include <qbluetoothserver.h>
#include <qbluetoothsocket.h>

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

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_LINUX_H_ */
