/*
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * This file is part of WebMonDiag.
 *
 * Copyright (C) 2024, Ewgeny Repin <erpndev@gmail.com>
 *
 * WebMonDiag is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebMonDiag is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WebMonDiag.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "commandlineview.h"
#include "commandreader.h"
#include "commands/icommand.h"
#include "../core/ipresenter.h"

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QSharedPointer>
#include <QThread>
#include <QVariant>

#include <iostream>

CommandLineView::CommandLineView(QObject *parent)
    : QObject{parent}
{
    commandReader = new CommandReader(this);
    connect(commandReader, &CommandReader::newToggleCommandReady,
            this, &CommandLineView::readToggleCommand);
    connect(commandReader, &CommandReader::newCommandReady, this, &CommandLineView::readCommand);
    connect(commandReader, &CommandReader::failedValue, this, &CommandLineView::failedValue);
}

CommandLineView::~CommandLineView()
{
    commandReader->quit();
    commandReader->wait(1000);
    if (commandReader->isRunning()) {
        commandReader->terminate();
        commandReader->wait(1000);
    }
}

void CommandLineView::setPresenter(IPresenter* p)
{
    presenter = p;
}

void CommandLineView::showStarted(bool val)
{
    if (val) {
        showView();
    }
}

void CommandLineView::enableListenPort(bool val)
{
    listenPort = val;
}

void CommandLineView::enableResponding(bool val)
{
    httpResp = val;
}

void CommandLineView::setResponseCode(int val)
{
    respCode = val;
}

void CommandLineView::setResponseTime(int val)
{
    respTime = val;
}

void CommandLineView::setHostname(const QString &val)
{
    hostname = val;
}

void CommandLineView::setEndpointPath(const QString &val)
{
    endpointPath = val;
}

void CommandLineView::setPort(ushort val)
{
    port = val;
}

void CommandLineView::showView()
{
    showState();

    std::cout << "\n === Web Monitoring Diagnistics ===\n";
    std::cout << "\t 1 - On/Off port listening\n";
    std::cout << "\t 2 - On/Off response via HTTP\n";
    std::cout << "\t 3 - On/Off response delay\n";
    std::cout << "\t 4 - On/Off return empty page\n";
    std::cout << "\t 5 - Set response time (ms)\n";
    std::cout << "\t 6 - Set response code\n";
    std::cout << "\t 7 - Set custom web page\n";
    std::cout << "\t 8 - Set endpoint path\n";
    std::cout << "\t 9 - Show state\n";
    std::cout << "\t q - Quit\n";
    commandReader->start();
}

void CommandLineView::enableResponseDelay(bool val)
{
    respDelay = val;
}

void CommandLineView::setWebPageName(const QString &name)
{
    webPageStr = name;
}

void CommandLineView::setReturnEmptyPage(bool val)
{
    emptyPage = val;
}

void CommandLineView::appendLog(const QString &str)
{
    std::cout << " " + str.toStdString() + "\n";
    std::cout.flush();
}

void CommandLineView::readToggleCommand(char ch)
{
    switch (ch)
    {
        case '1':
            presenter->enableListenPort(!listenPort);
            break;
        case '2':
            presenter->enableHttpResponse(!httpResp);
            break;
        case '3':
            presenter->enableResponseDelay(!respDelay);
            break;
        case '4':
            presenter->setReturnEmptyPage(!emptyPage);
            break;
        case '9':
            showState();
            break;
        case 'Q':
        case 'q':
            emit quitApp();
            return;
        default:
            std::cout << "Unknown command\n";
    }

    commandReader->start();
}

void CommandLineView::readCommand(const QSharedPointer<ICommand> &cm)
{
        cm->exec(presenter);
        commandReader->start();
}

void CommandLineView::failedValue()
{
    std::cout << "Failed value\n";
    std::cin.clear();
    std::cin.ignore();

    commandReader->start();
}

void CommandLineView::showState()
{
    QString portStr;
    portStr.setNum(port);

    QString str = QString("\n Hostname: " + hostname + '\n');
    str.append(QString(" Port: " + portStr + '\n'));
    str.append(QString(" Endpoint path: " + endpointPath + '\n'));
    str.append(QString(" Listen port: " + QVariant(listenPort).toString()) + '\n');
    str.append(QString(" Response via HTTP: " + QVariant(httpResp).toString()) + '\n');
    str.append(QString(" Response delay: ") + QVariant(respDelay).toString() + '\n');
    str.append(QString(" Return empty page: " + QVariant(emptyPage).toString()) + '\n');
    str.append(QString(" Response time: " + QString::number(respTime)) + "(ms)\n");
    str.append(QString(" Response code: " + QString::number(respCode)) + '\n');
    str.append(QString(" Web page: ") + (webPageStr.isEmpty() ? "none" : webPageStr) + '\n');

    std::cout << str.toStdString();
}
