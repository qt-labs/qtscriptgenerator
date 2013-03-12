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

function tr(s) { return s; }


function RSSListing(parent)
{
    QWidget.call(this, parent);

    this.xml = new QXmlStreamReader();
    this.currentTag = "";
    this.linkString = "";
    this.titleString = "";
    this.connectionId = -1;

    this.lineEdit = new QLineEdit(this);
    this.lineEdit.text = "http://labs.trolltech.com/blogs/feed";

    this.fetchButton = new QPushButton(tr("Fetch"), this);
    this.abortButton = new QPushButton(tr("Abort"), this);
    this.abortButton.enabled = false;

    this.treeWidget = new QTreeWidget(this);
    this.treeWidget["itemActivated(QTreeWidgetItem*, int)"].connect(
        this, this.itemActivated);
    var headerLabels = new Array();
    headerLabels.push(tr("Title"));
    headerLabels.push(tr("Link"));
    this.treeWidget.setHeaderLabels(headerLabels);
    this.treeWidget.header().setResizeMode(QHeaderView.ResizeToContents);

    this.http = new QHttp(this);
    this.http.readyRead.connect(this, this.readData);
    this.http.requestFinished.connect(this, this.finished);

    this.lineEdit.returnPressed.connect(this, this.fetch);
    this.fetchButton.clicked.connect(this, this.fetch);
    this.abortButton.clicked.connect(this.http, this.http.abort);

    var layout = new QVBoxLayout(this);

    var hboxLayout = new QHBoxLayout();

    // ### working around problem with addWidget() binding
    hboxLayout.addWidget(this.lineEdit, 0, Qt.AlignLeft);
    hboxLayout.addWidget(this.fetchButton, 0, Qt.AlignLeft);
    hboxLayout.addWidget(this.abortButton, 0, Qt.AlignLeft);

    layout.addLayout(hboxLayout);
    layout.addWidget(this.treeWidget, 0, Qt.AlignLeft);

    this.windowTitle = tr("RSS listing example");
    this.resize(640,480);
}

RSSListing.prototype = new QWidget();

RSSListing.prototype.fetch = function()
{
    this.lineEdit.readOnly = true;
    this.fetchButton.enabled = false;
    this.abortButton.enabled = true;
    this.treeWidget.clear();

    this.xml.clear();

    var url = new QUrl(this.lineEdit.text);

    this.http.setHost(url.host());
    this.connectionId = this.http.get(url.path());
}

RSSListing.prototype.readData = function(resp)
{
    if (resp.statusCode() != 200)
        this.http.abort();
    else {
        this.xml.addData(this.http.readAll());
        this.parseXml();
    }
}

RSSListing.prototype.finished = function(id, error)
{
    if (error) {
        print("Received error during HTTP fetch."); // ### qWarning()
        this.lineEdit.readOnly = false;
        this.abortButton.enabled = false;
        this.fetchButton.enabled = true;
    } else if (id == this.connectionId) {
        this.lineEdit.readOnly = false;
        this.abortButton.enabled = false;
        this.fetchButton.enabled = true;
    }
}

RSSListing.prototype.parseXml = function()
{
    while (!this.xml.atEnd()) {
        this.xml.readNext();
        if (this.xml.isStartElement()) {
            if (this.xml.name() == "item")
                this.linkString = this.xml.attributes().value("rss:about").toString();
            this.currentTag = this.xml.name().toString();
        } else if (this.xml.isEndElement()) {
            if (this.xml.name() == "item") {

                var item = new QTreeWidgetItem();
                item.setText(0, this.titleString);
                item.setText(1, this.linkString);
                this.treeWidget.addTopLevelItem(item);

                this.titleString = "";
                this.linkString = "";
            }

        } else if (this.xml.isCharacters() && !this.xml.isWhitespace()) {
            if (this.currentTag == "title")
                this.titleString += this.xml.text().toString();
            else if (this.currentTag == "link")
                this.linkString += this.xml.text().toString();
        }
    }
    if (this.xml.hasError() && (this.xml.error() != QXmlStreamReader.PrematureEndOfDocumentError)) {
        print("XML ERROR:", this.xml.lineNumber() + ":", this.xml.errorString());
        this.http.abort();
    }
}

RSSListing.prototype.itemActivated = function(item)
{
    QDesktopServices.openUrl(new QUrl(item.text(1)));
}


var rsslisting = new RSSListing();
rsslisting.show();
QCoreApplication.exec();
