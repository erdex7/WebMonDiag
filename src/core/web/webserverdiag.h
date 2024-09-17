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

#ifndef WEBSERVERDIAG_H
#define WEBSERVERDIAG_H

#include <QtHttpServer/QHttpServer>

#include "../iwebserverdiag.h"

class WebServerDiag: public QObject, public IWebServerDiag
{
    Q_OBJECT

public:
    WebServerDiag(QObject* parent = nullptr);

    WebServerData getWebServerData() const override;

    void startServer(bool val) override;
    void setPresenter(IPresenter* p) override;
    void enableListenPort(bool val) override;
    void enableHttpResponse(bool val) override;
    void enableResponseDelay(bool val) override;
    void httpResponseTimeChanged(int val) override;

    void setServerData(const WebServerData& data) override;
    void setListenPortNumber(ushort port) override;
    void setHostname(const QString& hostname) override;
    void setEndpointPath(const QString& path) override;
    void setResponseCode(int val) override;
    void setRespPage(const QString &newRespPage) override;
    void setReturnEmptyPage(bool emptyPage) override;

signals:
    void quitWaitLoop();

private:
    WebServerData srvData;
    std::shared_ptr<QHttpServer> httpServer;
    IPresenter* presenter;
};

#endif // WEBSERVERDIAG_H
