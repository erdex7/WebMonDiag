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

#include "webserverdiag.h"
#include "../ipresenter.h"

#include <QTcpServer>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QCoreApplication>

WebServerDiag::WebServerDiag(QObject *parent) : QObject(parent)
{
    httpServer = std::make_shared<QHttpServer>();
}

WebServerData WebServerDiag::getWebServerData() const
{
    return srvData;
}

void WebServerDiag::startServer(bool val)
{
    if (!val) {
        emit quitWaitLoop();
        srvData.setStarted(val);
        for (QTcpServer* tcpServer : httpServer->servers()) {
            tcpServer->close();
            tcpServer->deleteLater();
        }
        return;
    }

    srvData.setStarted(true);

    httpServer->route(srvData.getEndpointPath(), QHttpServerRequest::Method::Get,
                      [this](QHttpServerResponder&& responder) {
            if (!srvData.isResponding()) {
                QEventLoop loop;
                connect(this, &WebServerDiag::quitWaitLoop, &loop, &QEventLoop::quit, Qt::DirectConnection);
                loop.exec();
            }
            if (srvData.getEnableResponseDelay()) {
                QEventLoop loop;
                QTimer::singleShot(srvData.getResponseTime(), &loop, &QEventLoop::quit);
                connect(this, &WebServerDiag::quitWaitLoop, &loop, &QEventLoop::quit);
                loop.exec();
            }

            if (!srvData.isStarted()) {
                return;
            }

            QHttpServerResponse httpResp(srvData.getPage(),
                        static_cast<QHttpServerResponse::StatusCode>(srvData.getReturnCode()));
            httpResp.write(std::move(responder));

        }
    );

    bool needListenSrv = srvData.isListen();
    enableListenPort(true);
    if (srvData.errorHasOccurred()) {
        return;
    }

    if (!needListenSrv) {
        enableListenPort(false);
    }
}

void WebServerDiag::setPresenter(IPresenter *p)
{
    presenter = p;
}

void WebServerDiag::enableListenPort(bool val)
{
    srvData.setListen(val);
    if (!srvData.isStarted()) {
        return;
    }
    if (val) {
        quint16 res = httpServer->listen(QHostAddress(srvData.getHostname()), srvData.getPort());
        if (!res) {
            qWarning() << "Error binding server to address and port";
            presenter->serverErrorHasOccurred("Failed to bind to address and port");
            srvData.setErrorHasOccurred(true);
            srvData.setStarted(false);
            return;
        }
        srvData.setErrorHasOccurred(false);
        srvData.setStarted(true);
    } else {
        for (QTcpServer* tcpServer : httpServer->servers()) {
            tcpServer->close();
        }
    }
}

void WebServerDiag::enableHttpResponse(bool val)
{
    srvData.setResponding(val);
    if (val) {
        emit quitWaitLoop();
    }
}

void WebServerDiag::enableResponseDelay(bool val)
{
    srvData.enableResponseDelay(val);
}

void WebServerDiag::httpResponseTimeChanged(int val)
{
    srvData.setResponseTime(val);
}

void WebServerDiag::setServerData(const WebServerData &data)
{
    srvData = data;
}

void WebServerDiag::setListenPortNumber(ushort port)
{
    srvData.setPort(port);
}

void WebServerDiag::setHostname(const QString &hostname)
{
    srvData.setHostname(hostname);
}

void WebServerDiag::setEndpointPath(const QString &path)
{
    srvData.setEndpointPath(path);
}

void WebServerDiag::setResponseCode(int val)
{
    srvData.setResponseCode(val);
}

void WebServerDiag::setRespPage(const QString &newRespPage)
{
    srvData.setRespPage(newRespPage);
}

void WebServerDiag::setReturnEmptyPage(bool emptyPage)
{
    srvData.setReturnEmptyPage(emptyPage);
}
