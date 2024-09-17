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

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTcpServer>

#include "../../src/core/serverpresenter.h"
#include "../../src/core/web/webserverdiag.h"
#include "../../src/core/iview.h"
#include "../mock/mockview.h"

class TestWebServerDiag: public QObject
{
    Q_OBJECT

public:
    TestWebServerDiag()
        : logger(&view),
          presenter(&view, &server, &logger) {}

private slots:
    void init();
    void startServerTest();
    void changePageTest();
    void changeEndpoint();
    void stopListenPort();
    void httpResponse();
    void httpResponseWithDelay();

private:
    WebServerDiag server;
    MockView view;
    Logger logger;
    ServerPresenter presenter;

    QNetworkAccessManager qnam;
    QUrl url;
    QTcpServer tcpServer;
};

void TestWebServerDiag::init()
{
    presenter.hostnameChanged("127.0.0.1");
    presenter.listenPortChanged(8008);
    presenter.endpointPathChanged("/");
    presenter.enableHttpResponse(true);
    presenter.enableResponseDelay(false);
    server.setRespPage("Test Page");
    server.startServer(false);
    WebServerData srvData = server.getWebServerData();
    url = QUrl("http://" + srvData.getHostname() + ":" + QString::number(srvData.getPort()) + "/");
    tcpServer.close();
    qnam.clearConnectionCache();
}

void TestWebServerDiag::startServerTest()
{
    QByteArray page("Test Page");
    server.setRespPage(page);

    presenter.startServer();
    QVERIFY(!tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                             server.getWebServerData().getPort()));
    tcpServer.close();

    QEventLoop loop;
    qnam.clearConnectionCache();
    QNetworkReply* reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
    QSignalSpy replySpy(reply, &QNetworkReply::readyRead);
    loop.exec();
    QCOMPARE(replySpy.count(), 1);
    QCOMPARE(reply->readAll(), page);
    QCOMPARE(reply->error(), QNetworkReply::NetworkError::NoError);

    presenter.startServer();  // Already started, so stop the server

    QVERIFY(tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                              server.getWebServerData().getPort()));
    tcpServer.close();

    presenter.startServer();  // Start server

    QVERIFY(!tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                              server.getWebServerData().getPort()));
    tcpServer.close();

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    QSignalSpy replySpySecondStart(reply, &QNetworkReply::readyRead);
    connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
    loop.exec();
    QCOMPARE(replySpySecondStart.count(), 1);
    QCOMPARE(reply->readAll(), page);
    QCOMPARE(reply->error(), QNetworkReply::NetworkError::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());

    presenter.startServer();  // Stop server

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    QVERIFY(!reply->waitForReadyRead(3000));
    QVERIFY(reply->readAll().isEmpty());
    QVERIFY(!server.getWebServerData().isStarted());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
}

void TestWebServerDiag::changePageTest()
{
    server.setRespPage("Page A");

    server.startServer(true);

    QNetworkReply* reply = qnam.get(QNetworkRequest(url));

    QEventLoop loop;
    connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->readAll(), "Page A");
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());

    server.setRespPage("Page B");

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->readAll(), "Page B");
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());
}

void TestWebServerDiag::changeEndpoint()
{
    presenter.hostnameChanged("1.2.3.4");
    presenter.startServer();

    QNetworkReply* reply = qnam.get(QNetworkRequest(url));
    QVERIFY(!reply->waitForReadyRead(3000));
    QVERIFY(reply->readAll().isEmpty());
    QVERIFY(server.getWebServerData().errorHasOccurred());
    QVERIFY(!server.getWebServerData().isStarted());

    presenter.hostnameChanged("localhost");
    presenter.startServer();

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    QEventLoop loop;
    connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
    loop.exec();
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());

    QUrl urlTmp = "http://" + server.getWebServerData().getHostname() + ":12345";
    presenter.listenPortChanged(12345);

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(urlTmp));
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(500);
    loop.exec();
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());

    urlTmp = "http://" + server.getWebServerData().getHostname() + ":12345" + "/test/path";
    presenter.endpointPathChanged("/test/path");

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(urlTmp));
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(500);
    loop.exec();
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isStarted());
}

void TestWebServerDiag::stopListenPort()
{
    presenter.enableListenPort(false);
    presenter.startServer();

    QVERIFY(tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                             server.getWebServerData().getPort()));
    tcpServer.close();
    QEventLoop loop;
    QNetworkReply* reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QVERIFY(reply->readAll().isEmpty());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(!server.getWebServerData().isListen());
    reply->close();
    reply->deleteLater();

    presenter.enableListenPort(true);

    QVERIFY(!tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                             server.getWebServerData().getPort()));
    tcpServer.close();

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isListen());

    presenter.enableListenPort(false);
    presenter.enableListenPort(false);

    QVERIFY(tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                             server.getWebServerData().getPort()));
    tcpServer.close();

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->error(), QNetworkReply::ConnectionRefusedError);
    QVERIFY(reply->readAll().isEmpty());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(!server.getWebServerData().isListen());
    reply->close();
    reply->deleteLater();

    presenter.startServer(); // Stop server
    presenter.enableListenPort(true);

    QVERIFY(tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                             server.getWebServerData().getPort()));
    tcpServer.close();

    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->error(), QNetworkReply::ConnectionRefusedError);
    QVERIFY(reply->readAll().isEmpty());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(!server.getWebServerData().isListen());
    reply->close();
    reply->deleteLater();

    presenter.startServer(); // Start server

    QVERIFY(!tcpServer.listen(QHostAddress(server.getWebServerData().getHostname()),
                              server.getWebServerData().getPort()));
    tcpServer.close();
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(server.getWebServerData().isListen());
}

void TestWebServerDiag::httpResponse()
{
    QVERIFY(!server.getWebServerData().isListen());
    QVERIFY(!server.getWebServerData().isResponding());

    presenter.startServer();
    presenter.enableHttpResponse(false);

    QVERIFY(server.getWebServerData().isListen());
    QVERIFY(!server.getWebServerData().isResponding());
    QEventLoop loop;
    QNetworkReply* reply = qnam.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    bool timeout = false;
    QTimer::singleShot(3000, this, [&]() {
        loop.quit();
        presenter.enableHttpResponse(true);
        timeout = true;
    });
    loop.exec();
    presenter.enableHttpResponse(false);
    QVERIFY(reply->readAll().isEmpty());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());
    QVERIFY(timeout);

    presenter.enableHttpResponse(true);

    QVERIFY(server.getWebServerData().isResponding());
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());

    presenter.enableHttpResponse(false);

    QVERIFY(!server.getWebServerData().isResponding());
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timeout = false;
    QTimer::singleShot(3000, this, [&]() {
        timeout = true;
        presenter.enableHttpResponse(true);
        loop.quit();
    });
    loop.exec();

    QVERIFY(timeout);
    QVERIFY(reply->readAll().isEmpty());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());
}

void TestWebServerDiag::httpResponseWithDelay()
{
    QVERIFY(!server.getWebServerData().isListen());
    QVERIFY(!server.getWebServerData().isResponding());

    presenter.startServer();

    QVERIFY(server.getWebServerData().isListen());
    QVERIFY(server.getWebServerData().isResponding());

    presenter.enableResponseDelay(true);
    presenter.enableHttpResponse(true);
    presenter.httpResponseTimeChanged(1000); // Response 1 sec

    QVERIFY(server.getWebServerData().isResponding());
    QVERIFY(server.getWebServerData().getEnableResponseDelay());
    QCOMPARE(server.getWebServerData().getResponseTime(), 1000);
    QEventLoop loop;
    QNetworkReply* reply = qnam.get(QNetworkRequest(url));
    bool respTimeout = false;
    QTimer::singleShot(1000, this, [&](){
        QVERIFY(reply->isRunning());
        respTimeout = true;
    });
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QVERIFY(respTimeout);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());

    presenter.enableResponseDelay(false);

    QVERIFY(server.getWebServerData().isResponding());
    QVERIFY(!server.getWebServerData().getEnableResponseDelay());
    QCOMPARE(server.getWebServerData().getResponseTime(), 1000);
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    respTimeout = false;
    QTimer::singleShot(1000, this, [&](){
        QVERIFY(reply->isRunning());
        respTimeout = true;
    });
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QVERIFY(!respTimeout);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());

    presenter.httpResponseTimeChanged(1500);
    presenter.enableResponseDelay(true);

    QVERIFY(server.getWebServerData().isResponding());
    QVERIFY(server.getWebServerData().getEnableResponseDelay());
    QCOMPARE(server.getWebServerData().getResponseTime(), 1500);
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    respTimeout = false;
    QTimer::singleShot(1500, this, [&](){
        QVERIFY(reply->isRunning());
        respTimeout = true;
    });
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QVERIFY(respTimeout);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());

    presenter.httpResponseTimeChanged(500);

    QVERIFY(server.getWebServerData().isResponding());
    QVERIFY(server.getWebServerData().getEnableResponseDelay());
    QCOMPARE(server.getWebServerData().getResponseTime(), 500);
    respTimeout = false;
    qnam.clearConnectionCache();
    reply = qnam.get(QNetworkRequest(url));
    QTimer::singleShot(500, this, [&](){
        QVERIFY(reply->isRunning());
        respTimeout = true;
    });
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QVERIFY(respTimeout);
    QCOMPARE(reply->readAll(), server.getWebServerData().getPage());
    QCOMPARE(reply->error(), QNetworkReply::NoError);
    QVERIFY(!server.getWebServerData().errorHasOccurred());
}

QTEST_MAIN(TestWebServerDiag)
#include "webserverdiag_test.moc"
