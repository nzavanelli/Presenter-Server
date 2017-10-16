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

