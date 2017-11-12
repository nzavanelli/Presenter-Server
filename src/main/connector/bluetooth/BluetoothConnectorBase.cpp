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

// This uuid needs to be the same for all clients that want to connect to our
// service
const QString BluetoothConnectorBase::serviceUuid
    = "be71c255-8349-4d86-b09e-7983c035a191";
const QString BluetoothConnectorBase::serviceName
    = "PresenterService";
const QString BluetoothConnectorBase::serviceDescription
    = "A presenter service to remote control presentations from mobile devices";
const QString BluetoothConnectorBase::serviceProvider
    = "Felix Wohlfrom";
