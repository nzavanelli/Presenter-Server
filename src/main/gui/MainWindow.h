/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Presenter. Server software to remote control a presentation.         *
 *  Copyright (C) 2017-2018 Felix Wohlfrom                               *
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
 * MainWindow.h
 *
 *  Created on: 14.07.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_MAINWINDOW_H_
#define SRC_MAIN_GUI_MAINWINDOW_H_

#include <QSystemTrayIcon>

#include <QMainWindow>

#include "Logger.h"

#include "../connector/bluetooth/BluetoothConnector.h"

namespace Ui {
    class MainWindow;
}

/**
 * The main window. Shows the status log.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window. Optional
         */
        MainWindow(QWidget *parent = 0);

        /**
         * Destructor. Will clean up status icon and bluetooth connector.
         */
        ~MainWindow();

    protected:
        /**
         * Handler that minimizes the window to system tray.
         *
         * @param event The event
         */
        void changeEvent(QEvent *event);

    private slots:
        /**
         * Used to show status information of the servers.
         *
         * @param message The message to display
         */
        void info(const QString &message);

        /**
         * Called if an error happened during server initialization.
         *
         * @param message The error message to display
         */
        void error(const QString &message);

        /**
         * Called once a server is ready to accept connections.
         */
        void serverReady();

        /**
         * Called if a new client connected.
         *
         * @param name The client name
         */
        void clientConnected(const QString &name);

        /**
         * Called if a client disconnected.
         */
        void clientDisconnected();

        /**
         * Called if a key event was sent.
         *
         * @param sender The name of the sender
         * @param key The key that was sent
         */
        void keySent(const QString &sender, const QString &key);

        /**
         * Called on clicks on the system tray icon.
         *
         * @param reason The activation reason (single click, double click etc.)
         */
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

        /**
         * Called on clicks on the "show log" menu item.
         */
        void showLog();

        /**
         * Called on clicks on "info" menu item
         */
        void showAboutScreen();

        void restore();

    private:
        /**
         * The main window ui.
         */
        Ui::MainWindow* ui;

        /**
         * The logger window.
         */
        Logger* logger;

        /**
         * The bluetooth connector class. Will create the bluetooth server.
         */
        BluetoothConnector* btConnector;

        /**
         * The action to open our log window.
         */
        QAction *openAction;

        /**
         * The action that opens the "about" window.
         */
        QAction *aboutAction;

        /**
         * The action to finally quit our server.
         */
        QAction *quitAction;

        /**
         * The icon that shows our logo.
         */
        QIcon *icon;

        /**
         * The system tray icon that is shown while our server is run in
         * background.
         */
        QSystemTrayIcon *trayIcon;

        /**
         * The context menu for our tray icon.
         */
        QMenu *trayIconMenu;
};

#endif // SRC_MAIN_GUI_MAINWINDOW_H_
