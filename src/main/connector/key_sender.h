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
 * key_sender.h
 *
 *  Created on: 13.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_CONNECTOR_KEY_SENDER_H_
#define SRC_MAIN_CONNECTOR_KEY_SENDER_H_
/**
 * Will initialize our key sender.
 */
void init_keysender();

/**
 * Will send the "next" key to the system.
 */
void send_next();

/**
 * Will send the "previous" key to the system.
 */
void send_prev();

/**
 * Will destroy the key sender.
 */
void destroy_keysender();
#endif /* SRC_MAIN_CONNECTOR_KEY_SENDER_H_ */
