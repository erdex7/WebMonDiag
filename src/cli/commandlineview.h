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

#ifndef COMMANDLINEVIEW_H
#define COMMANDLINEVIEW_H

#include <QObject>

#include "../core/iview.h"

class CommandReader;
class ICommand;

class CommandLineView : public QObject, public IView
{
    Q_OBJECT

public:
    CommandLineView(QObject *parent = nullptr);
    ~CommandLineView();

    void setPresenter(IPresenter* p) override;
    void showStarted(bool val) override;
    void enableListenPort(bool val) override;
    void enableResponding(bool val) override;
    void setResponseCode(int val) override;
    void setResponseTime(int val) override;
    void setHostname(const QString &val) override;
    void setEndpointPath(const QString& val) override;
    void setPort(ushort val) override;
    void showView() override;
    void enableResponseDelay(bool val) override;
    void setWebPageName(const QString &name) override;
    void setReturnEmptyPage(bool val) override;
    void appendLog(const QString &str) override;

signals:
    void quitApp();

private slots:
    void readToggleCommand(char ch);
    void readCommand(const QSharedPointer<ICommand>& cm);
    void failedValue();

private:
    void showState();

    IPresenter* presenter;
    CommandReader* commandReader;

    bool listenPort = false;
    bool httpResp = false;
    bool respDelay = false;
    bool emptyPage = false;
    int respTime = 0;
    int respCode = 0;
    ushort port = 0;
    QString hostname;
    QString endpointPath;
    QString webPageStr;
};

#endif // COMMANDLINEVIEW_H
