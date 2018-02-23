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
 * AboutWindowTest.cpp
 *
 *  Created on: 23.02.2018
 *      Author: Felix Wohlfrom
 */

#include "AboutWindowTest.h"

void AboutWindowTest::init()
{
    dummyParent = new QWidget();
    aboutWindow = new AboutWindow(dummyParent);
}

void AboutWindowTest::cleanup()
{
    delete aboutWindow;
    delete dummyParent;
}

void AboutWindowTest::verifyDialogCreation()
{
    aboutWindow->show();
    QVERIFY2(aboutWindow->isVisible(), "Did not show about window");
}

void AboutWindowTest::verifyCloseByReturnKey()
{
    aboutWindow->show();
    QTest::keyClick(aboutWindow, Qt::Key_Return);
    QVERIFY2(!aboutWindow->isVisible(),
                 "Could not close about window using 'return' key");
}

void AboutWindowTest::verifyCloseByEscapeKey()
{
    aboutWindow->show();
    QTest::keyClick(aboutWindow, Qt::Key_Escape);
    QVERIFY2(!aboutWindow->isVisible(),
                 "Could not close about window using 'escape' key");
}

QTEST_MAIN(AboutWindowTest)
