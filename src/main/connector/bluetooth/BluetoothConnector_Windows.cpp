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
* BluetoothConnector_Windows.cpp
*
*  Created on: 17.08.2017
*      Author: Felix Wohlfrom
*/

#include "BluetoothConnector_Windows.h"

#include <QSettings>
#include <QCoreApplication>

#include <stdio.h>
#include <initguid.h>
#include <ws2bth.h>

// TODO Generate from our string definded in base class
// {be71c255-8349-4d86-b09e-7983c035a191}
DEFINE_GUID(serviceGuid, 0xbe71c255, 0x8349, 0x4d86, 0xb0, 0x9e,
        0x79, 0x83, 0xc0, 0x35, 0xa1, 0x91);

// We just need a small buffer since we just receive really short commands
#define READ_BUFFER_SIZE 20

BluetoothConnector::BluetoothConnector():
        serverSocket(INVALID_SOCKET), socketInfo(NULL), instanceName(NULL),
        readerThread(NULL)
{}

BluetoothConnector::~BluetoothConnector()
{
    stopServer();
}

void BluetoothConnector::startServer()
{
    // Check for Winsock version 2.2.
    WSADATA WSAData = { 0 };
    int status = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if (status != 0) {
        emit error(QString("Unable to initialize Winsock version 2.2\n"));
        return;
    }

    // Read computer name for later exposion in service
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameLength = MAX_COMPUTERNAME_LENGTH + 1;
    if (!GetComputerName(computerName, &computerNameLength))
    {
        emit error(QString("Failed to read computer name. %1\n")
                .arg(getLastWSAError()));
        return;
    }

    // Open a bluetooth socket using RFCOMM protocol
    serverSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (serverSocket == INVALID_SOCKET)
    {
        emit error(QString("Could not open socket. %1\n")
                .arg(getLastWSAError()));
        return;
    }

    // Setting address family to AF_BTH indicates winsock2 to use bluetooth port
    SOCKADDR_BTH localSocketAddress = { 0 };
    localSocketAddress.addressFamily = AF_BTH;
    localSocketAddress.port = BT_PORT_ANY;

    // Bind our server socket to the bluetooth port
    if (bind(serverSocket, (struct sockaddr *) &localSocketAddress,
            sizeof(SOCKADDR_BTH)) == SOCKET_ERROR)
    {
        emit error(QString("Could not bind to socket 0x%1. %2\n")
                .arg((ULONG64)serverSocket, 16).arg(getLastWSAError()));
        return;
    }

    int addrLength = sizeof(SOCKADDR_BTH);
    if (getsockname(serverSocket, (struct sockaddr *)&localSocketAddress,
            &addrLength) == SOCKET_ERROR)
    {
        emit error(QString("Could not get socket name of socket 0x%1. %2\n")
                .arg((ULONG64)serverSocket, 16).arg(getLastWSAError()));
        return;
    }

    // Allocate space for our service socket info
    socketInfo = (LPCSADDR_INFO) HeapAlloc(GetProcessHeap(),
                                              HEAP_ZERO_MEMORY,
                                              sizeof(CSADDR_INFO));
    if (socketInfo == NULL)
    {
        emit error("Unable to allocate memory for CSADDR_INFO\n");
        return;
    }

    // CSADDR_INFO
    socketInfo[0].LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
    socketInfo[0].LocalAddr.lpSockaddr = (LPSOCKADDR)&localSocketAddress;
    socketInfo[0].RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
    socketInfo[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&localSocketAddress;
    socketInfo[0].iSocketType = SOCK_STREAM;
    socketInfo[0].iProtocol = BTHPROTO_RFCOMM;

    // If we got an address, go ahead and advertise it.
    WSAQUERYSETW querySet = { 0 };
    ZeroMemory(&querySet, sizeof(WSAQUERYSETW));
    querySet.dwSize = sizeof(WSAQUERYSETW);
    querySet.lpServiceClassId = (LPGUID)&serviceGuid;

    // Adding a byte to the size to account for the space in the
    // format string in the swprintf call. This will have to change if converted
    // to UNICODE
    size_t instanceNameSize = 0;
    HRESULT res = StringCchLength(computerName, sizeof(computerName),
            &instanceNameSize);
    if (FAILED(res))
    {
        emit error(QString("ComputerName specified is too large\n"));
        return;
    }
    instanceNameSize += sizeof(serviceName) + 1;

    instanceName = (STRSAFE_LPWSTR)HeapAlloc(GetProcessHeap(),
                                       HEAP_ZERO_MEMORY,
                                       instanceNameSize);
    if (instanceName == NULL)
    {
        emit error(QString("Out of memory. %1\n").arg(GetLastError()));
        return;
    }

    StringCbPrintfW(instanceName, instanceNameSize, L"%s %s",
            computerName, serviceName);

    // Define our service description
    querySet.lpszServiceInstanceName = instanceName;
    wchar_t wComment[1024];
    int len = serviceDescription.toWCharArray(wComment);
    wComment[len] = '\0';
    querySet.lpszComment = wComment;
    querySet.dwNameSpace = NS_BTH;
    querySet.dwNumberOfCsAddrs = 1; // Must be 1.
    querySet.lpcsaBuffer = socketInfo; // Req'd.

    // Now register our bluetooth service
    if (WSASetServiceW(&querySet, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR)
    {
        emit error(QString("Could not register bluetooth service: %1\n")
                .arg(getLastWSAError()));
        return;
    }

    // wait for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        emit error(QString("Wait for incoming connections failed on socket "
                "0x%1. %2\n")
                    .arg((ULONG64)serverSocket, 16).arg(getLastWSAError()));
        return;
    }

    // Start the reader thread
    readerThread = new BluetoothReaderThread(serverSocket);

    connect(readerThread, SIGNAL(error(QString)),
                    this, SLOT(errorThread(QString)));
    connect(readerThread, SIGNAL(clientConnected(QString)),
                    this, SLOT(clientConnectedThread(QString)));
    connect(readerThread, SIGNAL(clientDisconnected()),
                      this, SLOT(clientDisconnectedThread()));
    connect(readerThread, SIGNAL(lineReceived(QString, QString)),
                      this, SLOT(lineReceived(QString, QString)));

    readerThread->start();

    emit info(tr("Server ready and waiting for connections"));
    emit serverReady();
}

void BluetoothConnector::stopServer()
{
    // Close server socket. Needs to be done before killing reader thread
    // to make sure all blocking calls are closed
    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    if (readerThread != NULL)
    {
        readerThread->stop();
        readerThread->wait();
        delete(readerThread);
        readerThread = NULL;
    }

    if (socketInfo != NULL)
    {
        HeapFree(GetProcessHeap(), 0, socketInfo);
        socketInfo = NULL;
    }

    if (instanceName != NULL)
    {
        HeapFree(GetProcessHeap(), 0, instanceName);
        instanceName = NULL;
    }
}

QString BluetoothConnector::getLastWSAError()
{
    wchar_t *errorString = NULL;

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&errorString, 0, NULL);

    QString error = QString::fromWCharArray(errorString);
    LocalFree(errorString);

    return error;
}

void BluetoothConnector::write(const QString& message)
{
    emit info(QString("Write: %1").arg(message));

    QByteArray bytes = message.toUtf8();

    int lengthWritten = 0;
    int lengthToWrite = bytes.size();
    const char* writeBuffer = bytes.data();

    SOCKET clientSocket = readerThread->getClientSocket();

    while (lengthWritten < lengthToWrite)
    {
        int length = send(clientSocket,
                writeBuffer + lengthWritten,
                lengthToWrite - lengthWritten,
                0);

        if (length == SOCKET_ERROR)
        {
            emit error(QString("Writing command failed. %1\n")
                    .arg(BluetoothConnector::getLastWSAError()));
            QCoreApplication::processEvents();
            return;
        }
        else
        {
            lengthWritten += length;
        }
    }
}

void BluetoothConnector::errorThread(const QString &error)
{
    emit RemoteControl::error(error);
}

void BluetoothConnector::clientConnectedThread(const QString &name)
{
    handleClientConnected(name);
}

void BluetoothConnector::clientDisconnectedThread()
{
    emit RemoteControl::clientDisconnected();
}

void BluetoothConnector::lineReceived(const QString &name, const QString &line)
{
    handleLine(name, line);
}

BluetoothReaderThread::BluetoothReaderThread(const SOCKET serverSocket) :
        serverSocket(serverSocket), clientSocket(INVALID_SOCKET),
        keepRunning(true)
{}

void BluetoothReaderThread::stop()
{
    // Close the connection
    if (closesocket(clientSocket) == SOCKET_ERROR)
    {
        emit error(QString("Could not close socket 0x%1. %2\n")
                .arg((ULONG64)serverSocket, 16)
                .arg(BluetoothConnector::getLastWSAError()));
        QCoreApplication::processEvents();
    }

    keepRunning = false;
}

SOCKET BluetoothReaderThread::getClientSocket()
{
    return clientSocket;
}

void BluetoothReaderThread::run()
{
    while (serverSocket != INVALID_SOCKET && keepRunning)
    {
        SOCKADDR_BTH clientInfo;
        int infoLen = sizeof(clientInfo);

        // wait for incoming connections
        clientSocket = accept(serverSocket, (sockaddr*)&clientInfo, &infoLen);
        if (clientSocket == INVALID_SOCKET)
        {
            emit error(QString("Could not connect to client. %1\n")
                    .arg(BluetoothConnector::getLastWSAError()));
            QCoreApplication::processEvents();
            return;
        }

        // Read the friendly client name from registry - if there's an easier
        // way, let me know
        QString clientName("Unknown");
        QString rootKey = QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\BTHENUM\\DEV_%1")
            .arg(QString::number(clientInfo.btAddr, 16).toUpper());
        QSettings settings(rootKey, QSettings::NativeFormat);
        QStringList keys = settings.allKeys();
        for (int i = 0; i < keys.length(); i++)
        {
            if (keys[i].endsWith("FriendlyName"))
            {
                clientName = settings.value(keys[i]).toString();
            }
        }

        emit clientConnected(clientName);
        QCoreApplication::processEvents();

        // Initialize read buffer. One additional character for
        // null termination.
        char readBuffer[READ_BUFFER_SIZE + 1];
        QString receivedLine;
        int lengthReceived = 0;
        bool continueRead = true;
        bool hasError = false;
        while (continueRead)
        {
            // read the received data
            lengthReceived = recv(clientSocket, readBuffer,
                    READ_BUFFER_SIZE, 0);

            switch (lengthReceived)
            {
                case 0: // socket connection has been closed gracefully
                    emit clientDisconnected();
                    QCoreApplication::processEvents();
                    continueRead = false;
                    break;

                case SOCKET_ERROR:
                    emit error(QString("Receiving command failed. %1\n")
                            .arg(BluetoothConnector::getLastWSAError()));
                    QCoreApplication::processEvents();
                    continueRead = false;
                    hasError = true;
                    break;

                default:
                    readBuffer[lengthReceived] = '\0'; // NULL terminate string
                    receivedLine.append(readBuffer);
                    int newLineIndex = receivedLine.indexOf('\n');
                    while (newLineIndex > -1)
                    {
                        emit lineReceived(clientName, receivedLine.left(newLineIndex));
                        QCoreApplication::processEvents();

                        receivedLine = receivedLine.mid(newLineIndex + 1);

                        newLineIndex = receivedLine.indexOf('\n');
                    }
                    break;
            }
        }

        if (hasError)
        {
            // Close the connection
            if (closesocket(clientSocket) == SOCKET_ERROR)
            {
                emit error(QString("Could not close socket 0x%1. %2\n")
                        .arg((ULONG64)serverSocket, 16)
                        .arg(BluetoothConnector::getLastWSAError()));
                QCoreApplication::processEvents();
            }
            else
            {
                // Make the connection invalid regardless
                clientSocket = INVALID_SOCKET;
            }
        }

        if (clientSocket != INVALID_SOCKET)
        {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }
    }
}
