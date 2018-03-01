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
 * KeySenderDaemon.cpp
 *
 *  Created on: 24.02.2018
 *      Author: Felix Wohlfrom
 */

#include "KeySenderDaemon.h"

#include <QThread>
#include <QTcpSocket>
#include <QCoreApplication>

#include "../daemon_port.h"

extern "C" {
    #include "key_sender.h"
}

KeySenderDaemon::KeySenderDaemon()
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::LocalHost, KEYSENDER_PORT))
    {
        qWarning("Server could not start");
        QCoreApplication::exit(EXIT_FAILURE);
        return;
    }
    else
    {
        qDebug("Server started");
    }

    init_keysender();
    QThread::sleep(0.5); // Make sure that the keysender is ready

    qInfo("Key sender up and running. Waiting for commands...");
}

KeySenderDaemon::~KeySenderDaemon()
{
    qInfo("Stopping server");
    destroy_keysender();

    server->close();
    delete server;
}

void KeySenderDaemon::newConnection()
{
    qInfo("Received new connection");
    QTcpSocket *serverSocket = server->nextPendingConnection();

    connect(serverSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(serverSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(serverSocket, SIGNAL(disconnected()),
            serverSocket, SLOT(deleteLater()));
}

void KeySenderDaemon::readyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    while (socket->canReadLine())
    {
        QString command = socket->readLine().trimmed();

        if (command == "sendNext")
        {
            qInfo("sendNext");
            send_next();
        }
        else if (command == "sendPrev")
        {
            qInfo("sendPrev");
            send_prev();
        }
        else
        {
            qWarning("Ignoring command: '%s'", command.toStdString().c_str());
        }
    }
}

void KeySenderDaemon::disconnected()
{
    QCoreApplication::exit(EXIT_SUCCESS);
}
