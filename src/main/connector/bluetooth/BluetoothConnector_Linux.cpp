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
 * BluetoothConnector_Linux.cpp
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#include "BluetoothConnector_Linux.h"

#include <qbluetoothlocaldevice.h>
#include <qbluetoothaddress.h>

BluetoothConnector::BluetoothConnector() :
    rfcommServer(NULL), serviceInfo(), clientSockets()
{}

BluetoothConnector::~BluetoothConnector()
{
    stopServer();
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

    emit info(tr("Server ready and waiting for connections"));
    emit serverReady();
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

    handleClientConnected(socket->peerName());
}

void BluetoothConnector::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());

    if (!socket)
    {
        return;
    }

    emit RemoteControl::clientDisconnected();

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

    while (socket->canReadLine())
    {
        QByteArray line = socket->readLine().trimmed();
        handleLine(socket->peerName(),
                  QString::fromUtf8(line.constData(), line.length()));
    }
}

void BluetoothConnector::write(const QString& message)
{
    emit info(QString("Write: %1").arg(message));

    for (QBluetoothSocket* socket: clientSockets)
    {
        QByteArray messageToSend(message.toUtf8());
        int bytesSent = 0;
        while (bytesSent < messageToSend.length()) {
            int sent = socket->write(messageToSend);
            messageToSend = messageToSend.mid(sent);
            bytesSent += sent;
        }
    }
}
