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
 * AboutWindow.h
 *
 *  Created on: 13.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ABOUTWINDOW_H_
#define SRC_MAIN_GUI_ABOUTWINDOW_H_

#include <QDialog>

namespace Ui {
    class AboutWindow;
}

/**
 * Represents a display that show some information about the presenter server.
 */
class AboutWindow : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window
         */
        AboutWindow(QWidget* parent);

        /**
         * Destructor. Deletes the ui object.
         */
        virtual ~AboutWindow();

    private:
        /**
         * The ui of the dialog.
         */
        Ui::AboutWindow* ui;

        /**
         * Returns the compiler used for compilation.
         *
         * @return The compiler name
         */
        QString getCompiler();

        /**
         * Returns the compiler flags used for compilation.
         *
         * @return The compiler flags if available
         */
        QString getCompilerFlags();
};

#endif // SRC_MAIN_GUI_ABOUTWINDOW_H_

