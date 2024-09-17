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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../core/iview.h"

class QPushButton;
class QLabel;
class QSpinBox;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QTextEdit;

class MainWindow : public QMainWindow, public IView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void setPresenter(IPresenter* val) override;
    void showStarted(bool val) override;
    void enableListenPort(bool val) override;
    void enableResponding(bool val) override;
    void setResponseCode(int val) override;
    void setResponseTime(int val) override;
    void setHostname(const QString &val) override;
    void setEndpointPath(const QString& val) override;
    void setPort(ushort val) override;
    void setReturnEmptyPage(bool val) override;
    void showView() override;
    void enableResponseDelay(bool val) override;
    void setWebPageName(const QString& name) override;
    void appendLog(const QString& str) override;

private:
    void showRunningState(int port);
    void showStoppedState();
    void hideServerAppDetails(bool val);

    QGroupBox* createEndpointBox();
    QGroupBox* createApplicationBox();
    QGroupBox* createWebPageBox();

    QLabel* mainLabel;
    QLabel* currentWebPageLabel;
    QLineEdit* srvHostnameLedt;
    QLineEdit* endpointPathLedt;
    QPushButton* startBtn;
    QPushButton* resetBtn;
    QPushButton* openPageFileBtn;
    QCheckBox* respondsChkb;
    QCheckBox* responseTimeChkb;
    QCheckBox* listeningChkb;
    QCheckBox* returnEmptyPage;
    QSpinBox* portSpb;
    QSpinBox* responseTimeSpb;
    QComboBox* returnCodeCmb;
    QGroupBox* srvAppBox;
    QTextEdit* logTedt;
    IPresenter* presenter;
};

#endif // MAINWINDOW_H
