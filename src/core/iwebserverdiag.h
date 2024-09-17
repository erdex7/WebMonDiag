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

#ifndef IWEBSERVERDIAG_H
#define IWEBSERVERDIAG_H

#include "web/webserverdata.h"

class IPresenter;

class IWebServerDiag
{
public:
    virtual ~IWebServerDiag() = default;

    virtual WebServerData getWebServerData() const = 0;

    virtual void startServer(bool val) = 0;
    virtual void setPresenter(IPresenter* p) = 0;
    virtual void enableListenPort(bool val) = 0;
    virtual void enableHttpResponse(bool val) = 0;
    virtual void enableResponseDelay(bool val) = 0;
    virtual void httpResponseTimeChanged(int val) = 0;

    virtual void setServerData(const WebServerData& data) = 0;
    virtual void setListenPortNumber(ushort port) = 0;
    virtual void setHostname(const QString& hostname) = 0;
    virtual void setEndpointPath(const QString& path) = 0;
    virtual void setResponseCode(int val) = 0;
    virtual void setRespPage(const QString &newRespPage) = 0;
    virtual void setReturnEmptyPage(bool emptyPage) = 0;
};

#endif // IWEBSERVERDIAG_H
