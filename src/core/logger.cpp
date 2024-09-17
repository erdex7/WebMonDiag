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

#include "logger.h"
#include "iview.h"

#include <QDateTime>

Logger::Logger(IView *v) : view(v)
{

}

void Logger::addMessage(const QString &str)
{
    addText(str);
}

void Logger::addHighlightedMessage(const QString &str)
{
    addText(str, "0000FF");
}

void Logger::addError(const QString &str)
{
    addText(str, "FF0000");
}

void Logger::setTextAsHtml(bool val)
{
    textAsHtml = val;
}

void Logger::addText(const QString &str, const QString &color)
{
    QString res;
    res.append(QDateTime::currentDateTime().toString() + ": ");

    if (!textAsHtml) {
        res.append(str);
        view->appendLog(res);
        return;
    }

    QString htmlStr;
    if (!color.isEmpty()) {
        htmlStr = "<font color=\"#" + color + "\">";
    }

    htmlStr.append("<b>").append(res).append("</b> ");
    htmlStr.append(str).append("<br/>");
    if (!color.isEmpty()) {
        htmlStr.append("</font>");
    }

    view->appendLog(htmlStr);
}

