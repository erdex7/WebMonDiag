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

#ifndef SERVERPRESENTER_H
#define SERVERPRESENTER_H

#include <QObject>

#include "ipresenter.h"
#include "logger.h"
#include "web/webpageloader.h"

class IView;
class IWebServerDiag;

class ServerPresenter : public IPresenter
{
    Q_OBJECT

public:
    ServerPresenter(IView* v, IWebServerDiag* s, Logger* log, QObject *parent = nullptr);

    void showView() override;
    void startServer() override;
    void stopServer() override;
    void hostnameChanged(const QString &str) override;
    void endpointPathChanged(const QString& str) override;
    void listenPortChanged(ushort val) override;
    void enableListenPort(bool needListen) override;
    void enableHttpResponse(bool val) override;
    void enableResponseDelay(bool val) override;
    void setReturnEmptyPage(bool val) override;
    void httpResponseTimeChanged(int val) override;
    void returnCodeChanged(int val) override;
    void newWebPageSelected(const QString& path) override;
    void serverErrorHasOccurred(const QString& str) override;

    void reset() override;

private:
    void resetToDefault();
    void setWebPageByPath(const QString& path);

    IView* view;
    IWebServerDiag* server;
    Logger* logger;
    WebPageLoader webPageLoader;
};

#endif // SERVERPRESENTER_H
