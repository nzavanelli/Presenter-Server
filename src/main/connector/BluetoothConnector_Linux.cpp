/*
 * BluetoothConnector_Linux.cpp
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#include "BluetoothConnector_Linux.h"

extern "C" {
    #include "key_sender.h"
}

#include <qbluetoothlocaldevice.h>
#include <qbluetoothaddress.h>

BluetoothConnector::BluetoothConnector() :
    rfcommServer(NULL), serviceInfo(NULL), clientSockets(NULL)
{}

BluetoothConnector::~BluetoothConnector()
{
    stopServer();
    destroy_keysender();
}

void BluetoothConnector::startServer()
{
    QBluetoothAddress localAdapter = QBluetoothAddress();

    if (rfcommServer)
    {
        return;
    }

    rfcommServer =
            new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(rfcommServer, SIGNAL(newConnection()),
                    this, SLOT(clientConnected()));

    bool result = rfcommServer->listen(localAdapter);
    if (!result)
    {
        emit error(tr("Bluetooth server not started. "
                "Cannot bind remote control server to %1.")
                .arg(localAdapter.toString()));
        emit error(tr("Make sure that bluetooth is available on your system "
                "and enabled."));
        return;
    }

    QBluetoothServiceInfo::Sequence classId;

    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(
            QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);

    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, serviceName);
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
            serviceDescription);
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider,
            serviceProvider);

    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(
            QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    serviceInfo.registerService(localAdapter);

    init_keysender();

    emit info(tr("Server ready and waiting for connections"));
}

void BluetoothConnector::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = NULL;
}

void BluetoothConnector::clientConnected()
{
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
    {
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    clientSockets.append(socket);
    emit BluetoothConnectorBase::clientConnected(socket->peerName());
}

void BluetoothConnector::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());

    if (!socket)
    {
        return;
    }

    emit BluetoothConnectorBase::clientDisconnected();

    clientSockets.removeOne(socket);
    socket->deleteLater();
}

void BluetoothConnector::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());

    if (!socket)
    {
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        handleKey(socket->peerName(),
                  QString::fromUtf8(line.constData(), line.length()));
    }
}

