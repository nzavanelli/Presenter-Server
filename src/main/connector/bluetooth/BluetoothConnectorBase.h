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
 * BluetoothConnectorBase.h
 *
 *  Created on: 17.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTORBASE_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTORBASE_H_

#include "../RemoteControl.h"

#include <QString>

/**
 * The BluetoothConnectorBase class provides the minimum interface that are
 * needed to connect to our remote control server via bluetooth. You can start
 * and stop such a server, receive key events and emits several signals on
 * status changes.
 */
class BluetoothConnectorBase: public RemoteControl
{
    Q_OBJECT

    protected:
        static const QString serviceUuid;
        static const QString serviceName;
        static const QString serviceDescription;
        static const QString serviceProvider;
};

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTORBASE_H_ */
