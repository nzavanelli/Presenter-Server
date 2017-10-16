/*
 * BluetoothConnectorBase.h
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_H_

#include <QObject>
#include <QString>

// This uuid needs to be the same for all clients that want to connect to our
// service
static const QString serviceUuid("be71c255-8349-4d86-b09e-7983c035a191");
static const QString serviceName("PresenterService");
static const QString serviceDescription("A presenter service to remote"
        " control presentations from mobile devices");
static const QString serviceProvider("Felix Wohlfrom");

/**
 * The BluetoothConnectorBase class provides the minimum interface that are
 * needed to connect to our remote control server via bluetooth. You can start
 * and stop such a server, receive key events and emits several signals on
 * status changes.
 */
class BluetoothConnectorBase: public QObject
{
    Q_OBJECT

    public:
        virtual ~BluetoothConnectorBase();

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
         * Will handle received key events.
         *
         * @param sender The sender that sent the key.
         * @param key The sent key.
         */
        void handleKey(const QString& sender, const QString &key);

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

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTHCONNECTORBASE_H_ */
