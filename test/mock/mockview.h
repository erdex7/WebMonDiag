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

#ifndef MOCKVIEW_H
#define MOCKVIEW_H

struct ViewData {
    bool shownStarted = false;
    bool shownListen = false;
    bool checkedResponding = false;
    bool checkedNeedRespDelay = false;
    bool checkedReturnCode = false;
    bool shownEndpoint = false;
    int shownReturnCode = -1;
    int shownResponseTime = -1;
    QString hostnameValue;
    QString endpointPath;
    ushort shownPort = 0;
};

class IPresenter;

class MockView: public IView
{
    // IView interface
public:
    virtual ~MockView() = default;

    void setPresenter(IPresenter *val) override
    {
        presenter = val;
    }

    void showStarted(bool val) override
    {
        viewData.shownStarted = val;
    }

    void enableListenPort(bool val) override
    {
        viewData.shownListen = val;
    }

    void enableResponding(bool val) override
    {
        viewData.checkedResponding = val;
    }

    void setResponseCode(int val) override
    {
        viewData.shownReturnCode = val;
    }

    void setResponseTime(int val) override
    {
        viewData.shownResponseTime = val;
    }

    void setHostname(const QString &val) override
    {
        viewData.hostnameValue = val;
    }

    void setEndpointPath(const QString& val) override
    {
        viewData.endpointPath = val;
    }

    void setPort(ushort val) override
    {
        viewData.shownPort = val;
    }

    void showView() override
    {

    }

    void enableResponseDelay(bool val) override
    {
        viewData.checkedNeedRespDelay = val;
    }

    void setReturnEmptyPage(bool c) override
    {

    }

    ViewData getViewData() const
    {
        return viewData;
    }

    void setWebPageName(const QString& name) override
    {

    }

    void appendLog(const QString& str) override
    {

    }

private:
    IPresenter* presenter;
    ViewData viewData;
};

#endif
