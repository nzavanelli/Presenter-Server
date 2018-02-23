/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Presenter. Server software to remote control a presentation.         *
 *  Copyright (C) 2018 Felix Wohlfrom                                    *
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
 * AboutWindowTest.h
 *
 *  Created on: 23.02.2018
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_TEST_GUI_ABOUTWINDOWTEST_H_
#define SRC_TEST_GUI_ABOUTWINDOWTEST_H_

#include <QTest>

#include "../../main/gui/AboutWindow.h"

#ifdef _DEBUG
    #ifdef _WIN32
        #ifdef _MSC_VER
             // Uncomment this if you installed visual memory
             // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

/**
 * Verifies that the "about window" dialog works fine.
 */
class AboutWindowTest: public QObject
{
    Q_OBJECT

    private:
        /**
         * A dummy window.
         */
        QWidget* dummyParent;

        /**
         * The window instance to use for tests.
         */
        AboutWindow* aboutWindow;

    private slots:
        /**
         * Initializes the testcase. Will create a dummy window and creates the dialog.
         */
        void init();

        /**
         * Cleans up the dummy window and dialog.
         */
        void cleanup();

    private slots:
        /**
         * Verifies that the window can be created and shown without issues.
         */
        void verifyDialogCreation();

        /**
         * Verifies that the window can be closed by clicking "return" key.
         */
        void verifyCloseByReturnKey();

        /**
         * Verifies that the window can be closed by clicking "escape" key.
         */
        void verifyCloseByEscapeKey();
};

#endif /* SRC_TEST_GUI_ABOUTWINDOWTEST_H_ */
