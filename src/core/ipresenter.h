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

#ifndef IPRESENTER_H
#define IPRESENTER_H

#include <QObject>

class IPresenter : public QObject
{
    Q_OBJECT

public:
    IPresenter(QObject* parent) : QObject(parent) {}
    virtual ~IPresenter() = default;

public slots:
    virtual void showView() = 0;
    virtual void startServer() = 0;
    virtual void stopServer() = 0;
    virtual void hostnameChanged(const QString& str) = 0;
    virtual void endpointPathChanged(const QString& str) = 0;
    virtual void listenPortChanged(ushort val) = 0;
    virtual void enableListenPort(bool needListen) = 0;
    virtual void enableHttpResponse(bool val) = 0;
    virtual void enableResponseDelay(bool val) = 0;
    virtual void setReturnEmptyPage(bool val) = 0;
    virtual void httpResponseTimeChanged(int val) = 0;
    virtual void returnCodeChanged(int val) = 0;
    virtual void newWebPageSelected(const QString& path) = 0;
    virtual void serverErrorHasOccurred(const QString& str) = 0;

    virtual void reset() = 0;
};

#endif // IPRESENTER_H
