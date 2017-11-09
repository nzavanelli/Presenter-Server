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
 * BluetoothConnector.h
 *
 *  Created on: 10.11.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_H_
#define SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_H_

// This is just a wrapper class that will include either the linux or windows
// specific implementation of the bluetooth connector class.

#ifdef _WIN32
    #include "../connector/bluetooth/BluetoothConnector_Windows.h"
#endif // _WIN32
#ifdef __linux__
    #include "../connector/bluetooth/BluetoothConnector_Linux.h"
#endif // __linux__

#endif /* SRC_MAIN_CONNECTOR_BLUETOOTH_BLUETOOTHCONNECTOR_H_ */
