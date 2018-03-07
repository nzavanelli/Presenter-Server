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
 * KeySender.cpp
 *
 *  Created on: 25.02.2018
 *      Author: Felix Wohlfrom
 */

#include "KeySender.h"

#ifdef _WIN32
    extern "C" {
        #include "key_sender.h"
    }
#endif // _WIN32

#ifdef __linux__
    #include <QHostAddress>

    #include "daemon_port.h"
#endif // __linux__

KeySender::KeySender()
{
    #ifdef __linux__
        socket = new QTcpSocket(this);
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));

        socket->connectToHost(QHostAddress::LocalHost, KEYSENDER_PORT);

        if (socket->state() != QTcpSocket::ConnectedState)
        {
            emit error("Could not connect to key sender daemon. "
                    "Make sure it is up and running.");
        }
    #endif // __linux__
}

KeySender::~KeySender()
{
    #ifdef __linux__
        socket->close();
        delete socket;
    #endif // __linux__
}

void KeySender::sendNext()
{
    #ifdef _WIN32
        send_next();
    #endif // _WIN32

    #ifdef __linux__
        socket->write("sendNext\n");
    #endif // __linux__
}

void KeySender::sendPrev()
{
    #ifdef _WIN32
        send_prev();
    #endif // _WIN32

    #ifdef __linux__
        socket->write("sendPrev\n");
    #endif // __linux__
}

#ifdef __linux__
    void KeySender::socketError(const QAbstractSocket::SocketError socketError)
    {
        if (socketError == QAbstractSocket::ConnectionRefusedError)
        {
            emit error("Connection to key sender daemon refused. Make sure it "
                    "is up and running.");
        }
        else
        {
            emit error(QString("Socket error: %1").arg(socket->errorString()));
        }
    }
#endif // __linux__
