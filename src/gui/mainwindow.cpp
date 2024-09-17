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

#include "mainwindow.h"
#include "../core/ipresenter.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainLabel = new QLabel;
    mainLabel->setStyleSheet("font-size: 27px; font-weight: bold;");
    mainLabel->setAlignment(Qt::AlignCenter);
    mainLabel->setMaximumWidth(250);

    startBtn = new QPushButton("Start");
    resetBtn = new QPushButton("Reset");

    srvAppBox = createApplicationBox();
    QVBoxLayout* vlt = new QVBoxLayout;
    vlt->addWidget(mainLabel);
    vlt->addWidget(createEndpointBox());
    vlt->addWidget(srvAppBox);
    vlt->addWidget(createWebPageBox());
    vlt->addWidget(resetBtn);
    vlt->addWidget(startBtn);

    logTedt = new QTextEdit;
    logTedt->setReadOnly(true);

    QHBoxLayout* hltMain = new QHBoxLayout;
    hltMain->addLayout(vlt);
    hltMain->addWidget(logTedt);

    QWidget* widget = new QWidget;
    widget->setLayout(hltMain);
    setCentralWidget(widget);

    MainWindow::showStoppedState();
}

void MainWindow::setPresenter(IPresenter *val)
{
    presenter = val;

    connect(startBtn, &QPushButton::clicked, presenter, &IPresenter::startServer);
    connect(resetBtn, &QPushButton::clicked, presenter, &IPresenter::reset);
    connect(respondsChkb, &QCheckBox::clicked, presenter, &IPresenter::enableHttpResponse);
    connect(responseTimeChkb, &QCheckBox::clicked, presenter, &IPresenter::enableResponseDelay);
    connect(responseTimeSpb, &QSpinBox::editingFinished, this, [this]() {
        presenter->httpResponseTimeChanged(responseTimeSpb->value());
    });
    connect(returnCodeCmb, &QComboBox::currentTextChanged, this, [this]() {
        presenter->returnCodeChanged(returnCodeCmb->currentText().toInt());
    });
    connect(srvHostnameLedt, &QLineEdit::editingFinished, this, [this]() {
        presenter->hostnameChanged(srvHostnameLedt->text());
    });
    connect(endpointPathLedt, &QLineEdit::editingFinished, this, [this]() {
        presenter->endpointPathChanged(endpointPathLedt->text());
    });
    connect(listeningChkb, &QCheckBox::clicked, presenter, &IPresenter::enableListenPort);
    connect(portSpb, &QSpinBox::editingFinished, this, [this]() {
        presenter->listenPortChanged(static_cast<ushort>(portSpb->value()));
    });
    connect(openPageFileBtn, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this,
                                    tr("Open Web Page"), "", tr("All files (*.*);;HTML Files (*.htm *.html)"));
        presenter->newWebPageSelected(path);
    });
    connect(returnEmptyPage, &QCheckBox::clicked, presenter, &IPresenter::setReturnEmptyPage);

}

void MainWindow::showStarted(bool val)
{
    if (!val) {
        showStoppedState();
        return;
    }

    portSpb->setEnabled(false);
    srvHostnameLedt->setEnabled(false);
    showRunningState(portSpb->value());
}

void MainWindow::enableListenPort(bool val)
{
    listeningChkb->setChecked(val);
    hideServerAppDetails(val);
}

void MainWindow::enableResponding(bool val)
{
    respondsChkb->setChecked(val);
    return;
}

void MainWindow::setResponseCode(int val)
{
    int index = returnCodeCmb->findText(QString::number(val));
    returnCodeCmb->setCurrentIndex(index);
}

void MainWindow::setResponseTime(int val)
{
    responseTimeSpb->setValue(val);
}

void MainWindow::setHostname(const QString &val)
{
    srvHostnameLedt->setText(val);
}

void MainWindow::setEndpointPath(const QString &val)
{
    endpointPathLedt->setText(val);
}

void MainWindow::setPort(ushort val)
{
    portSpb->setValue(val);
}

void MainWindow::setReturnEmptyPage(bool val)
{
    returnEmptyPage->setChecked(val);
}

void MainWindow::showView()
{
    show();
}

void MainWindow::enableResponseDelay(bool val)
{
    responseTimeChkb->setChecked(val);
}

void MainWindow::setWebPageName(const QString &name)
{
    QString nameTmp = name;
    if (nameTmp.size() > 40) {
        nameTmp = nameTmp.left(18) + "..." + nameTmp.right(19);
    }
    currentWebPageLabel->setText("Displayed web page:\n" + nameTmp);
}

void MainWindow::hideServerAppDetails(bool val)
{
    srvAppBox->setEnabled(val);
}

void MainWindow::showStoppedState()
{
    portSpb->setEnabled(true);
    srvHostnameLedt->setEnabled(true);

    startBtn->setText("Start");
    mainLabel->setText("Stopped");
}

void MainWindow::showRunningState(int port)
{
    startBtn->setText("Stop");
    mainLabel->setText("Running on\n" + QString::number(port) + " port");
}

void MainWindow::appendLog(const QString &str)
{
    logTedt->insertHtml(str);
    logTedt->moveCursor(QTextCursor::End);
}

QGroupBox* MainWindow::createEndpointBox()
{
    QLabel* srvNameLabel = new QLabel("Hostname ");
    srvHostnameLedt = new QLineEdit("192.168.56.1");
    srvHostnameLedt->setToolTip("The name or IP address for HTTP server");

    listeningChkb = new QCheckBox("Listen port ");
    listeningChkb->setToolTip("Server will not listen on the port");
    listeningChkb->setChecked(true);

    portSpb = new QSpinBox;
    portSpb->setRange(0, 65535);
    portSpb->setValue(8008);

    QGridLayout* glt = new QGridLayout;
    glt->addWidget(srvNameLabel, 0, 0);
    glt->addWidget(srvHostnameLedt, 0, 1);
    glt->addWidget(listeningChkb, 1, 0);
    glt->addWidget(portSpb, 1, 1);

    QGroupBox* grpBox = new QGroupBox("Endpoint");
    grpBox->setLayout(glt);
    return grpBox;
}

QGroupBox *MainWindow::createApplicationBox()
{

    respondsChkb = new QCheckBox("Responds via HTTP");
    respondsChkb->setToolTip("Server will accept requests and respond to them");
    respondsChkb->setChecked(true);

    QLabel* pathLbl = new QLabel("Endpoint path");
    endpointPathLedt = new QLineEdit;

    responseTimeChkb = new QCheckBox("Response time (ms)");
    responseTimeChkb->setToolTip("Amount of time to respond");

    responseTimeSpb = new QSpinBox;
    responseTimeSpb->setRange(1, 60000);
    responseTimeSpb->setValue(1);
    responseTimeSpb->setToolTip("Amount of time to respond");

    returnCodeCmb = new QComboBox;
    returnCodeCmb->addItems(QStringList{"0", "100", "101", "102", "103", "200", "201",
                                        "202", "203", "204", "205", "206", "207", "208",
                                        "226", "300", "301", "302", "303", "304", "305",
                                        "306", "307", "308", "400", "401", "402", "403",
                                        "404", "405", "406", "407", "408", "409", "410",
                                        "411", "412", "413", "414", "415", "420", "429",
                                        "500", "501", "502", "503", "504", "505", "506",
                                        "507", "508", "510", "511", "526", "599"});
    QLabel* retCodeLbl = new QLabel("HTTP response code");

    QGridLayout* glt = new QGridLayout;
    glt->addWidget(respondsChkb, 0, 0);
    glt->addWidget(pathLbl, 1, 0);
    glt->addWidget(endpointPathLedt, 1, 1);
    glt->addWidget(responseTimeChkb, 2, 0);
    glt->addWidget(responseTimeSpb, 2, 1);
    glt->addWidget(retCodeLbl, 3, 0);
    glt->addWidget(returnCodeCmb, 3, 1);

    QGroupBox* grpBox = new QGroupBox("Application");
    grpBox->setLayout(glt);
    return grpBox;
}

QGroupBox* MainWindow::createWebPageBox()
{
    openPageFileBtn = new QPushButton("Select web page");
    currentWebPageLabel = new QLabel;
    MainWindow::setWebPageName("None");

    returnEmptyPage = new QCheckBox("Return empty web page");
    returnEmptyPage->setToolTip("Empty page will be shown if checked, otherwise the selected page");

    QVBoxLayout* vlt = new QVBoxLayout;
    vlt->addWidget(openPageFileBtn);
    vlt->addWidget(currentWebPageLabel);
    vlt->addWidget(returnEmptyPage);

    QGroupBox* grpBox = new QGroupBox("Web page");
    grpBox->setLayout(vlt);
    return grpBox;
}

