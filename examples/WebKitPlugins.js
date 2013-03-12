/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Script Generator project on Qt Labs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

function getArgumentValue(name, names, values, from)
{
    var i = (from == undefined) ? 0 : from;
    for ( ; i < names.length; ++i) {
        if (names[i] == name)
            return values[i];
    }
    return undefined;
}

//
// Plugin class
//

function MyWebPlugin(formUrl, scriptUrl, parent)
{
    QWidget.call(this, parent);

    this.initialized = false;
    this.formReply = this.downloadFile(formUrl, this.formDownloaded);
    if (scriptUrl == undefined)
        this.script = "";
    else
        this.scriptReply = this.downloadFile(scriptUrl, this.scriptDownloaded);
}

MyWebPlugin.prototype = new QWidget();

MyWebPlugin.prototype.downloadFile = function(url, callback)
{
    if (this.accessManager == undefined)
        this.accessManager = new QNetworkAccessManager();
    var reply = this.accessManager.get(new QNetworkRequest(url));
    reply.finished.connect(this, callback);
    return reply;
}

MyWebPlugin.prototype.formDownloaded = function()
{
    var loader = new QUiLoader();
    this.form = loader.load(this.formReply);
    var layout = new QVBoxLayout(this);
    layout.addWidget(this.form, 0, Qt.AlignCenter);
    this.initialize();
}

MyWebPlugin.prototype.scriptDownloaded = function()
{
    var stream = new QTextStream(this.scriptReply);
    this.script = stream.readAll();
    this.initialize();
}

MyWebPlugin.prototype.initialize = function()
{
    if (this.initialized)
        return;
    if ((this.form == undefined) || (this.script == undefined))
        return;
    var ctor = eval(this.script);
    if (typeof ctor != "function")
        return;
    this.instance = new ctor(this.form);
    this.initialized = true;
}


//
// QWebPluginFactory subclass
//

function MyWebPluginFactory(parent)
{
    QWebPluginFactory.call(this, parent);
}

MyWebPluginFactory.prototype = new QWebPluginFactory();

MyWebPluginFactory.prototype.create = function(mimeType, url, argumentNames, argumentValues)
{
    if (mimeType != "application/x-qtform")
        return null;

    var formUrl = getArgumentValue("form", argumentNames, argumentValues);
    var scriptUrl = getArgumentValue("script", argumentNames, argumentValues);
    if (formUrl == undefined)
        return null;

    return new MyWebPlugin(new QUrl(formUrl), new QUrl(scriptUrl));
}


//
// Main
//

var view = new QWebView();
view.settings().setAttribute(QWebSettings.PluginsEnabled, true);

var factory = new MyWebPluginFactory();
view.page().setPluginFactory(factory);

view.load(new QUrl("WebKitPlugins.html"));
view.show();

QCoreApplication.exec();
