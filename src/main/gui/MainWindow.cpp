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
 * MainWindow.cpp
 *
 *  Created on: 14.07.2017
 *      Author: Felix Wohlfrom
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AboutWindow.h"

#ifdef _WIN32
    #include "../connector/BluetoothConnector_Windows.h"
#endif // _WIN32
#ifdef __linux__
    #include "../connector/BluetoothConnector_Linux.h"
#endif // __linux__

#include <QIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), btConnector(new BluetoothConnector)
{
    // Initialize window
    ui->setupUi(this);

    // Initialize system tray icon
    #ifdef _WIN32
        icon = new QIcon(":/icon");
    #endif // _WIN32
    #ifdef __linux__
        icon = new QIcon(":/tray");
    #endif // __linux__

    // Quit button can not be connected in ui, since we don't have qApp there
    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // The signals of our bt connector
    connect(btConnector, SIGNAL(info(QString)),
                    this, SLOT(info(QString)));
    connect(btConnector, SIGNAL(error(QString)),
                    this, SLOT(error(QString)));
    connect(btConnector, SIGNAL(clientConnected(QString)),
                    this, SLOT(clientConnected(QString)));
    connect(btConnector, SIGNAL(clientDisconnected()),
                    this, SLOT(clientDisconnected()));
    connect(btConnector, SIGNAL(keySent(QString, QString)),
                    this, SLOT(keySent(QString, QString)));

    btConnector->startServer();

    // Create tray icon and context menu
    openAction = new QAction(tr("&Open"), this);
    connect(openAction, SIGNAL(triggered()), this, SLOT(show()));
    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutScreen()));
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(openAction);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->setIcon(*icon);
    setWindowIcon(*icon);
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete icon;
    delete btConnector;
}

void MainWindow::info(const QString &message)
{
    ui->logger->append(message);
}

void MainWindow::error(const QString &message)
{
    ui->logger->append(QString("<font color=\"#a33\">%1</font>").arg(message));
}

void MainWindow::clientConnected(const QString &name)
{
    ui->logger->append(tr("Connected: %1").arg(name));
}

void MainWindow::clientDisconnected()
{
    ui->logger->append(tr("Disconnected."));
}

void MainWindow::keySent(const QString &sender, const QString &key)
{
    ui->logger->append(tr("Key press, sender %1: %2").arg(sender, key));
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        if (isVisible())
        {
            hide();
        }
        else
        {
            show();
        }
    }
}

void MainWindow::showAboutScreen()
{
    AboutWindow(this).exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        if (trayIcon->supportsMessages()) {
            trayIcon->showMessage(tr("Presenter"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose \"Quit\" in the context menu "
                                    "of the system tray icon."));
        }
        else
        {
            QMessageBox::information(this, tr("Presenter"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray icon."));
        }

        hide();
        event->ignore();
    }
}
