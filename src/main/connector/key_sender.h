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
