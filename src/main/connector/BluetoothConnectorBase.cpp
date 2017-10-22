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
 * BluetoothConnectorBase.cpp
 *
 *  Created on: 18.07.2017
 *      Author: Felix Wohlfrom
 */

#include "BluetoothConnectorBase.h"

extern "C" {
    #include "key_sender.h"
}

BluetoothConnectorBase::~BluetoothConnectorBase()
{
    // Empty default destructor
};

void BluetoothConnectorBase::handleKey(const QString& sender, const QString& key)
{
    if (key == tr("nextSlide"))
    {
        send_next();
    }
    else if (key == tr("prevSlide"))
    {
        send_prev();
    }

    emit keySent(sender, key);
}

