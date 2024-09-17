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

#include "commandreader.h"
#include "commands/changeendpointpath.h"
#include "commands/changeresptime.h"
#include "commands/changehttpcode.h"
#include "commands/changewebpage.h"

#include <iostream>

#include <QSharedPointer>

CommandReader::CommandReader(QObject *parent)
    : QThread(parent)
{

}

void CommandReader::run()
{
    QMutexLocker locker(&mutex);
    char ch;
    std::cout << ">" << std::flush;
    std::cin >> ch;

    if (ch == 'q' || ch == 'Q') {
        emit newToggleCommandReady(ch);
        return;
    }

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        emit failedValue();
        return;
    }

    ICommand* cmd = nullptr;
    switch (ch) {
    case '5': {
        std::cout << "Enter new response time (ms): ";
        int val;
        std::cin >> val;
        if (std::cin.fail()) {
            emit failedValue();
            return;
        }
        ChangeRespTime* changeResp = new ChangeRespTime;
        changeResp->setRespTime(val);
        cmd = changeResp;
        break;
    }
    case '6': {
        std::cout << "Enter HTTP code: ";
        int code;
        std::cin >> code;
        if (std::cin.fail()) {
            emit failedValue();
            return;
        }
        ChangeHttpCode* changeCode = new ChangeHttpCode;
        if (!changeCode->setHttpCode(code)) {
            emit failedValue();
            return;
        }
        cmd = changeCode;
        break;
    }
    case '7': {
        QString res = readPath();
        ChangeWebPage* chgPage = new ChangeWebPage;
        cmd = chgPage;
        if (!chgPage->setPath(res)) {
            emit failedValue();
            return;
        }
        break;
    }
    case '8': {
        QString res = readPath();
        ChangeEndpointPath* chgPath = new ChangeEndpointPath;
        chgPath->setEndpointPath(res);
        cmd = chgPath;
    }
    default:
        break;
    }

    if (cmd) {
        QSharedPointer<ICommand> cmdSp(cmd);
        emit newCommandReady(cmdSp);
    } else {
        emit newToggleCommandReady(ch);
    }
}

QString CommandReader::readPath() const
{
    std::cout << "Enter path: ";
    std::cout.flush();
    std::string path;
    std::cin.ignore();
    std::getline(std::cin, path);
    return QString::fromStdString(path);
}
