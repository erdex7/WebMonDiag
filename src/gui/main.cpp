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

#include "mainwindow.h"
#include "../core/serverpresenter.h"
#include "../core/web/webserverdiag.h"
#include "../core/logger.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WebServerDiag webSrv;
    MainWindow mainWindow;

    Logger logger(&mainWindow);
    logger.setTextAsHtml(true);

    ServerPresenter serverPresentor(&mainWindow, &webSrv, &logger);
    serverPresentor.showView();

    return a.exec();
}
