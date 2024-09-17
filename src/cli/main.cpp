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

#include <QCoreApplication>
#include <QCommandLineParser>

#include <iostream>

#include "../core/serverpresenter.h"
#include "../core/web/webserverdiag.h"
#include "commands/icommand.h"
#include "commandlineview.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qRegisterMetaType<QSharedPointer<ICommand>>("QSharedPointer<ICommand>");

    QCommandLineParser parser;
    parser.setApplicationDescription("Web Monitoring Diagnistics");
    parser.addHelpOption();
    QCommandLineOption hostnameOption("n", "Hostname", "IP address or name");
    parser.addOption(hostnameOption);
    QCommandLineOption portOption("p", "Port", "Key for port");
    parser.addOption(portOption);
    parser.process(a);

    QString hostname = parser.value(hostnameOption);
    QString port = parser.value(portOption);

    if (hostname.isEmpty()) {
        hostname = "127.0.0.1";
        std::cout << "Hostname not specified, using default: " + hostname.toStdString() + "\n\n";
    }

    if (!WebServerData::isHostnameValid(hostname)) {
        std::cout << "Invalid hostname: " << hostname.toStdString() << "\n";
        return 1;
    }

    if (port.isEmpty()) {
        std::cout << "Port not specified\n\n";
        parser.showHelp();
    }
    bool portChk = false;
    port.toUShort(&portChk);
    if (!portChk) {
        std::cout << "Invalid port value: " << port.toStdString() << "\n";
        return 1;
    }

    CommandLineView view;
    WebServerDiag server;
    Logger logger(&view);
    logger.setTextAsHtml(false);
    ServerPresenter presenter(&view, &server, &logger);
    presenter.hostnameChanged(hostname);
    presenter.listenPortChanged(port.toUShort());
    presenter.startServer();

    QObject::connect(&view, &CommandLineView::quitApp, &a, &QCoreApplication::quit);

    if (server.getWebServerData().errorHasOccurred()) {
        return 1;
    }

    return a.exec();
}
