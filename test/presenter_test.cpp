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

#include <QtTest>

#include "../src/core/serverpresenter.h"
#include "../src/core/iview.h"
#include "../src/core/iwebserverdiag.h"
#include "mock/mockview.h"

class MockWebServerDiag: public IWebServerDiag
{
public:
    virtual ~MockWebServerDiag() = default;

    WebServerData getWebServerData() const override
    {
        return srvData;
    }

    void startServer(bool val) override
    {
        srvData.setStarted(val);
    }

    void enableListenPort(bool val) override
    {
        srvData.setListen(val);
    }

    void enableHttpResponse(bool val) override
    {
        srvData.setResponding(val);
    }

    void enableResponseDelay(bool val) override
    {
        srvData.enableResponseDelay(val);
    }

    void httpResponseTimeChanged(int val) override
    {
        srvData.setResponseTime(val);
    }

    void setServerData(const WebServerData& data) override
    {
        srvData = data;
    }

    void setListenPortNumber(ushort port) override
    {
        srvData.setPort(port);
    }

    void setHostname(const QString& hostname) override
    {
        srvData.setHostname(hostname);
    }

    void setEndpointPath(const QString& path) override
    {
        srvData.setEndpointPath(path);
    }

    void setResponseCode(int val) override
    {
        srvData.setResponseCode(val);
    }

    void setRespPage(const QString &newRespPage) override
    {
        srvData.setRespPage(newRespPage);
    }

    void setPresenter(IPresenter *p) override
    {

    }

    void setReturnEmptyPage(bool v) override
    {

    }

private:
    WebServerData srvData;
};

class TestPresenter: public QObject
{
    Q_OBJECT

public:
    TestPresenter();

private slots:
    void init();
    void startWebServerTest();
    void hostnameChangedTest();
    void endpointPathChangedTest();
    void listenPortChangedTest();
    void changeHttpRespondingTest();
    void changeResponseTime();
    void returnCodeChanged();

private:
    MockView view;
    MockWebServerDiag server;
    Logger log;
    ServerPresenter presenter;
};

TestPresenter::TestPresenter()
    : log{&view},
      presenter(&view, &server, &log)
{
}

void TestPresenter::init()
{
    presenter.reset();
}

void TestPresenter::startWebServerTest()
{
    presenter.startServer();

    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QCOMPARE(srvData.isStarted(), true);
    QCOMPARE(viewData.endpointPath, '/');
    QVERIFY(viewData.shownStarted);

    presenter.startServer();
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.isStarted(), false);
    QVERIFY(!viewData.shownStarted);
}

void TestPresenter::hostnameChangedTest()
{
    presenter.hostnameChanged("NewName");
    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "NewName");
    QCOMPARE(viewData.endpointPath, '/');
    QCOMPARE(viewData.shownPort, 8080);

    presenter.hostnameChanged("");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "NewName");
    QCOMPARE(viewData.endpointPath, '/');
    QCOMPARE(viewData.shownPort, 8080);

    presenter.hostnameChanged("Invalid!");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "NewName");
    QCOMPARE(viewData.hostnameValue, "NewName");

    presenter.hostnameChanged("AbC123.local");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "AbC123.local");
    QCOMPARE(viewData.hostnameValue, "AbC123.local");

    presenter.hostnameChanged("127. 0.0.1");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "AbC123.local");
    QCOMPARE(viewData.hostnameValue, "AbC123.local");

    presenter.hostnameChanged("127.0.0.1");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "127.0.0.1");
    QCOMPARE(viewData.hostnameValue, "127.0.0.1");
    QCOMPARE(viewData.shownPort, 8080);
}

void TestPresenter::endpointPathChangedTest()
{
    QString pathA = "path/to/api";
    QString pathB = "/api/v2/";
    QString pathC = "";
    QString invalidA = "/wrong path";
    QString invalidB = "wrong\"path";
    QString invalidC = "wrong\\path";
    presenter.endpointPathChanged(pathA);
    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QCOMPARE(srvData.getHostname(), "127.0.0.1");
    QCOMPARE(srvData.getEndpointPath(), '/' + pathA);
    QCOMPARE(srvData.getPort(), 8080);
    QCOMPARE(srvData.getHostname(), "127.0.0.1");
    QCOMPARE(viewData.endpointPath, '/' + pathA);
    QCOMPARE(viewData.shownPort, 8080);

    presenter.endpointPathChanged(invalidA);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), '/' + pathA);
    QCOMPARE(viewData.endpointPath, '/' + pathA);
    QCOMPARE(srvData.getEndpointPath(), '/' + pathA);
    QCOMPARE(viewData.endpointPath, '/' + pathA);

    presenter.endpointPathChanged(pathB);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);
    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);

    presenter.endpointPathChanged(invalidB);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);
    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);

    presenter.endpointPathChanged(invalidC);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);
    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);

    presenter.endpointPathChanged("/");
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), "/");
    QCOMPARE(viewData.endpointPath, "/");
    QCOMPARE(srvData.getEndpointPath(), "/");
    QCOMPARE(viewData.endpointPath, "/");

    presenter.endpointPathChanged(pathB);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);
    QCOMPARE(srvData.getEndpointPath(), pathB);
    QCOMPARE(viewData.endpointPath, pathB);

    presenter.endpointPathChanged(pathC);
    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getEndpointPath(), "/");
    QCOMPARE(viewData.endpointPath, "/");
    QCOMPARE(srvData.getEndpointPath(), "/");
    QCOMPARE(viewData.endpointPath, "/");
}

void TestPresenter::listenPortChangedTest()
{
    presenter.enableListenPort(true);

    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QVERIFY(!srvData.isListen());
    QVERIFY(srvData.readyToListen());
    QVERIFY(viewData.shownListen);

    presenter.enableListenPort(false);

    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QVERIFY(!srvData.isListen());
    QVERIFY(!viewData.shownListen);

    presenter.listenPortChanged(1234);

    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.getPort(), 1234);
    QCOMPARE(viewData.shownPort, 1234);
}

void TestPresenter::changeHttpRespondingTest()
{
    presenter.enableHttpResponse(true);

    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QVERIFY(!srvData.isResponding());
    QVERIFY(srvData.readyToRespond());
    QVERIFY(viewData.checkedResponding);

    presenter.enableHttpResponse(false);

    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QCOMPARE(srvData.isResponding(), false);
    QVERIFY(!viewData.checkedResponding);
}

void TestPresenter::changeResponseTime()
{
    presenter.enableResponseDelay(true);

    WebServerData srvData = server.getWebServerData();
    ViewData viewData = view.getViewData();

    QVERIFY(srvData.getEnableResponseDelay());
    QVERIFY(viewData.checkedNeedRespDelay);

    presenter.enableResponseDelay(false);

    srvData = server.getWebServerData();
    viewData = view.getViewData();

    QVERIFY(!srvData.getEnableResponseDelay());
    QVERIFY(!viewData.checkedNeedRespDelay);
}

void TestPresenter::returnCodeChanged()
{
    QCOMPARE(server.getWebServerData().getReturnCode(), 200);
    QCOMPARE(view.getViewData().shownReturnCode, 200);

    presenter.returnCodeChanged(300);

    QCOMPARE(server.getWebServerData().getReturnCode(), 300);
    QCOMPARE(view.getViewData().shownReturnCode, 300);
}

QTEST_MAIN(TestPresenter)
#include "presenter_test.moc"
