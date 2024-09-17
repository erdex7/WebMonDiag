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

#include "serverpresenter.h"
#include "iview.h"
#include "web/webpageloader.h"
#include "web/webpageloader.h"
#include "iwebserverdiag.h"

#include <QVariant>
#include <QRegularExpression>

ServerPresenter::ServerPresenter(IView *v, IWebServerDiag *s, Logger *log, QObject *parent)
    : IPresenter(parent),
    view(v),
    server(s),
    logger(log)
{
    view->setPresenter(this);
    server->setPresenter(this);

    setWebPageByPath("");
    resetToDefault();
}

void ServerPresenter::showView()
{
    view->showView();
    logger->addHighlightedMessage("Begin");
}

void ServerPresenter::startServer()
{
    if (server->getWebServerData().isStarted()) {
        stopServer();
        return;
    }
    server->startServer(true);
    if (server->getWebServerData().errorHasOccurred()) {
        stopServer();
        return;
    }
    logger->addMessage("Server is running");
    view->setPort(server->getWebServerData().getPort());
    view->showStarted(true);
}

void ServerPresenter::stopServer()
{
    server->startServer(false);
    view->showStarted(false);
    logger->addMessage("Server is stopped");
}

void ServerPresenter::hostnameChanged(const QString &str)
{
    if (!WebServerData::isHostnameValid(str)) {
        logger->addMessage("Invalid hostname: " + str);
        view->setHostname(server->getWebServerData().getHostname());
        return;
    }

    if (server->getWebServerData().getHostname() == str) {
        view->setHostname(str);
        return;
    }
    server->setHostname(str);
    view->setHostname(str);
    logger->addMessage("Hostname set to " + str);
}

void ServerPresenter::endpointPathChanged(const QString &str)
{
    if (server->getWebServerData().getEndpointPath() == str) {
        view->setEndpointPath(str);
        return;
    }

    QString pathRes = str;

    if (str.isEmpty()) {
        pathRes = "/";
    }

    if (pathRes.at(0) != '/') {
        pathRes.insert(0, '/');
    }

    QRegularExpression re("^[a-zA-Z0-9\\.\\-_\\~%+\\/]+$");
    QRegularExpressionMatch reMatch = re.match(pathRes);
    if (!reMatch.hasMatch()) {
        logger->addError("Invalid endpoint path: " + pathRes);
        view->setEndpointPath(server->getWebServerData().getEndpointPath());
        return;
    }

    server->setEndpointPath(pathRes);
    view->setEndpointPath(pathRes);

    bool needRestart = server->getWebServerData().isStarted();

    if (needRestart) {
        server->startServer(false);
        server->startServer(true);
    }

    logger->addMessage("Endpoint path set to " + pathRes);
}

void ServerPresenter::listenPortChanged(ushort val)
{
    if (server->getWebServerData().getPort() == val) {
        view->setPort(val);
        return;
    }

    bool needRestart = server->getWebServerData().isStarted();

    server->setListenPortNumber(val);
    view->setPort(val);

    if (needRestart) {
        server->startServer(false);
        server->startServer(true);
    }
    QString valStr;
    valStr.setNum(val);
    logger->addMessage("Port set to " + valStr);
}

void ServerPresenter::enableListenPort(bool needListen)
{
    if (server->getWebServerData().readyToListen() == needListen) {
        view->enableListenPort(needListen);
        return;
    }
    server->enableListenPort(needListen);
    view->enableListenPort(needListen);

    QString msg = QString("Port listening %1").arg(needListen ? "started" : "stopped");
    logger->addMessage(msg);
}

void ServerPresenter::enableHttpResponse(bool val)
{
    if (server->getWebServerData().readyToRespond() == val) {
        view->enableResponding(val);
        return;
    }

    server->enableHttpResponse(val);
    view->enableResponding(val);

    logger->addMessage(QString("HTTP response %1").arg(val ? "started" : "stopped"));
}

void ServerPresenter::enableResponseDelay(bool val)
{
    if (server->getWebServerData().getEnableResponseDelay() == val) {
        view->enableResponseDelay(val);
        return;
    }

    server->enableResponseDelay(val);
    view->enableResponseDelay(val);

    logger->addMessage(QString("Response delay %1").arg(val ? "started" : "stopped"));
}

void ServerPresenter::setReturnEmptyPage(bool val)
{
    if (server->getWebServerData().getReturnEmptyPage() == val) {
        view->setReturnEmptyPage(val);
        return;
    }

    server->setReturnEmptyPage(val);
    view->setReturnEmptyPage(val);

    logger->addMessage(QString("Empty page return is %1").arg((val ? "enabled" : "disabled")));
}

void ServerPresenter::httpResponseTimeChanged(int val)
{
    if (server->getWebServerData().getResponseTime() == val) {
        view->setResponseTime(val);
        return;
    }

    server->httpResponseTimeChanged(val);
    view->setResponseTime(val);

    logger->addMessage("HTTP response delay time is set to " + QString::number(val) + "ms");
}

void ServerPresenter::returnCodeChanged(int val)
{
    if (server->getWebServerData().getReturnCode() == val) {
        view->setResponseCode(val);
        return;
    }

    server->setResponseCode(val);
    view->setResponseCode(val);

    logger->addMessage("HTTP response code set to " + QString::number(val));
}

void ServerPresenter::newWebPageSelected(const QString &path)
{
    setWebPageByPath(path);
    logger->addMessage("Web page set to " + (path.isEmpty() ? "none" : path));
}

void ServerPresenter::serverErrorHasOccurred(const QString &str)
{
    logger->addError("Error has occurred: " +str);
}

void ServerPresenter::reset()
{
    logger->addHighlightedMessage("Start data reset...");

    resetToDefault();

    logger->addHighlightedMessage("Data reset completed");
}

void ServerPresenter::resetToDefault()
{
    WebServerData data;

    if (server->getWebServerData().isStarted()) {
        stopServer();
    }
    enableListenPort(data.readyToListen());
    enableHttpResponse(data.readyToRespond());
    returnCodeChanged(data.getReturnCode());
    listenPortChanged(data.getPort());
    hostnameChanged(data.getHostname());
    endpointPathChanged(data.getEndpointPath());
    httpResponseTimeChanged(data.getResponseTime());
    enableResponseDelay(data.getEnableResponseDelay());
    setReturnEmptyPage(data.getReturnEmptyPage());
}

void ServerPresenter::setWebPageByPath(const QString &path)
{
    webPageLoader.load(path);
    server->setRespPage(webPageLoader.getPage());
    view->setWebPageName(path);
}
