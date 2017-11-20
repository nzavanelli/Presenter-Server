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
 * Logger.h
 *
 *  Created on: 19.11.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_LOGGER_H_
#define SRC_MAIN_GUI_LOGGER_H_

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
    class Logger;
}

/**
 * Will show a logger window that will display detailed information about
 * the communication between the server and the client(s) and might help
 * debugging errors.
 */
class Logger : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window
         */
        Logger(QWidget* parent);

        /**
         * Destructor. Deletes the ui object.
         */
        virtual ~Logger();

        /**
         * Append a message to the logger.
         *
         * @param message The message to log.
         */
        void append(const QString &message);

    protected:
        /**
         * We need our own close event handler to hide instead of close.
         *
         * @param event The event
         */
        void closeEvent(QCloseEvent *event);

    private:
        /**
         * The ui of the dialog.
         */
        Ui::Logger* ui;
};

#endif // SRC_MAIN_GUI_LOGGER_H_

