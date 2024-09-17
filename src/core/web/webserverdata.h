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

#ifndef WEBSERVERDATA_H
#define WEBSERVERDATA_H

#include <QString>

class WebServerData
{
public:
    bool isListen() const;
    bool readyToListen() const;
    bool isStarted() const;
    bool isResponding() const;
    bool readyToRespond() const;
    bool getEnableResponseDelay() const;
    bool getReturnEmptyPage() const;
    bool errorHasOccurred() const;
    int getReturnCode() const;
    ushort getPort() const;
    int getResponseTime() const; // ms
    QString getHostname() const;
    QString getEndpointPath() const;
    QString getPage() const;

    void setHostname(const QString &newHostname);
    void setEndpointPath(const QString& path);
    void setRespPage(const QString &newRespPage);
    void setPort(ushort newPort);
    void enableResponseDelay(bool needDelay);
    void setErrorHasOccurred(bool val);
    void setResponseTime(int val); // ms
    void setResponseCode(int newReturnCode);
    void setListen(bool newListen);
    void setStarted(bool newStarted);
    void setResponding(bool newResponding);
    void setReturnEmptyPage(bool emptyPage);

    static bool isHostnameValid(const QString &str);

private:
    QString hostname = "127.0.0.1";
    QString endpointPath = "/";
    QString respPage;
    ushort port = 8080;
    int responseCode = 200;
    int respTimeMs = 1;
    bool listen = true;
    bool started = false;
    bool needResponseDelay = false;
    bool responding = true;
    bool hasError = false;
    bool returnEmptyPage = false;
};

bool operator==(const WebServerData& a, const WebServerData& b);

#endif // WEBSERVERDATA_H
