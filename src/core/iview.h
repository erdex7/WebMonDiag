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

#ifndef IVIEW_H
#define IVIEW_H

#include <QString>

class IPresenter;

class IView
{
public:
    virtual ~IView() = default;
    virtual void setPresenter(IPresenter* val) = 0;
    virtual void showStarted(bool val) = 0;
    virtual void enableListenPort(bool val) = 0;
    virtual void enableResponding(bool val) = 0;
    virtual void setResponseCode(int val) = 0;
    virtual void setResponseTime(int val) = 0;
    virtual void setHostname(const QString& val) = 0;
    virtual void setEndpointPath(const QString& val) = 0;
    virtual void setPort(ushort val) = 0;
    virtual void showView() = 0;
    virtual void enableResponseDelay(bool val) = 0;
    virtual void setWebPageName(const QString& name) = 0;
    virtual void setReturnEmptyPage(bool val) = 0;
    virtual void appendLog(const QString& str) = 0;
};

#endif // IVIEW_H
