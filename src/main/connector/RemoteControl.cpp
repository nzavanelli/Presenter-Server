/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Presenter. Server software to remote control a presentation.         *
 *  Copyright (C) 2017-2018 Felix Wohlfrom                               *
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
 * RemoteControl.cpp
 *
 *  Created on: 09.11.2017
 *      Author: Felix Wohlfrom
 */

#include "RemoteControl.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

RemoteControl::RemoteControl()
{
    keySender = new KeySender();
    connect(keySender, SIGNAL(error(QString)),
            this, SLOT(keySenderError(QString)));
}

RemoteControl::~RemoteControl()
{
    delete keySender;
}

void RemoteControl::handleClientConnected(const QString &name)
{
    write("{ \"type\": \"version\", "
        "\"data\": '{ \"minVersion\": \"1\","
        " \"maxVersion\": \"1\" }' }\n\n");

    emit RemoteControl::clientConnected(name);
}

void RemoteControl::handleLine(const QString& sender, const QString& line)
{
    if (!line.isEmpty())
    {
        messagePart = messagePart + line;
    }
    else
    {
        handleMessage(sender, messagePart);
        messagePart = "";
    }
}

void RemoteControl::handleMessage(const QString& sender, const QString& message)
{
    emit info(QString("Receive: %1: %2").arg(sender).arg(message));

    QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());

    if (document.object()["type"].toString() == tr("command"))
    {
        QString command = document.object()["data"].toString();
        if (command == tr("nextSlide"))
        {
            keySender->sendNext();
        }
        else if (command == tr("prevSlide"))
        {
            keySender->sendPrev();
        }

        emit keySent(sender, command);
    }
}

void RemoteControl::keySenderError(const QString& message)
{
    this->stopServer(); // Stop the server if we have errors to avoid confusion

    emit error(QString("Key sender error: %1").arg(message));
}
