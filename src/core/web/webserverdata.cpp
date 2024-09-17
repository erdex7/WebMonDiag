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

#include <QRegularExpression>

#include "webserverdata.h"

bool WebServerData::isListen() const
{
    return listen && isStarted();
}

bool WebServerData::readyToListen() const
{
    return listen;
}

bool WebServerData::isStarted() const
{
    return started;
}

bool WebServerData::isResponding() const
{
    return responding && isListen();
}

bool WebServerData::readyToRespond() const
{
    return responding;
}

bool WebServerData::getEnableResponseDelay() const
{
    return needResponseDelay;
}

bool WebServerData::getReturnEmptyPage() const
{
    return returnEmptyPage;
}

bool WebServerData::errorHasOccurred() const
{
    return hasError;
}

int WebServerData::getReturnCode() const
{
    return responseCode;
}

ushort WebServerData::getPort() const
{
    return port;
}

int WebServerData::getResponseTime() const
{
    return respTimeMs;
}

QString WebServerData::getHostname() const
{
    return hostname;
}

QString WebServerData::getEndpointPath() const
{
    return endpointPath;
}

QString WebServerData::getPage() const
{
    if (getReturnEmptyPage()) {
        return " ";
    }
    return respPage;
}

void WebServerData::setHostname(const QString &newHostname)
{
    hostname = newHostname;
}

void WebServerData::setEndpointPath(const QString &path)
{
    endpointPath = path;
}

void WebServerData::setRespPage(const QString &newRespPage)
{
    respPage = newRespPage;
}

void WebServerData::setPort(ushort newPort)
{
    port = newPort;
}

void WebServerData::enableResponseDelay(bool needDelay)
{
    needResponseDelay = needDelay;
}

void WebServerData::setErrorHasOccurred(bool val)
{
    hasError = val;
}

void WebServerData::setResponseTime(int val)
{
    respTimeMs = val;
}

void WebServerData::setResponseCode(int newReturnCode)
{
    responseCode = newReturnCode;
}

void WebServerData::setListen(bool newListen)
{
    listen = newListen;
}

void WebServerData::setStarted(bool newStarted)
{
    started = newStarted;
}

void WebServerData::setResponding(bool newResponding)
{
    responding = newResponding;
}

void WebServerData::setReturnEmptyPage(bool emptyPage)
{
    returnEmptyPage = emptyPage;
}

bool WebServerData::isHostnameValid(const QString& str)
{
    static QRegularExpression regExp("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*"
                              "([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");
    QRegularExpressionMatch match = regExp.match(str);
    return match.hasMatch();
}

bool operator==(const WebServerData& a, const WebServerData& b)
{
    return  a.getReturnCode() == b.getReturnCode() &&
            a.getHostname() == b.getHostname() &&
            a.getPage() == b.getPage() &&
            a.getPort() == b.getPort() &&
            a.getResponseTime() == b.getResponseTime() &&
            a.isResponding() == b.isResponding() &&
            a.isStarted() == b.isStarted() &&
            a.getEnableResponseDelay() == b.getEnableResponseDelay() &&
            a.getReturnEmptyPage() == b.getReturnEmptyPage() &&
            a.getEndpointPath() == b.getEndpointPath();
}
