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

function AnalogClock(parent) {
    QWidget.call(this, parent);

    var timer = new QTimer(this);
    timer.timeout.connect(this, "update()");
    timer.start(1000);

    this.setWindowTitle("Analog Clock");
    this.resize(200, 200);
}

AnalogClock.prototype = new QWidget();

AnalogClock.prototype.paintEvent = function() {
    var side = Math.min(this.width, this.height);
    var time = new Date();

    var painter = new QPainter();
    painter.begin(this);
    painter.setRenderHint(QPainter.Antialiasing);
    painter.translate(this.width / 2, this.height / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(new QPen(Qt.NoPen));
    painter.setBrush(new QBrush(AnalogClock.hourColor));

    painter.save();
    painter.rotate(30.0 * ((time.getHours() + time.getMinutes() / 60.0)));
    painter.drawConvexPolygon(AnalogClock.hourHand);
    painter.drawLine(0, 0, 100, 100);
    painter.restore();

    painter.setPen(AnalogClock.hourColor);

    for (var i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

    painter.setPen(new QPen(Qt.NoPen));
    painter.setBrush(new QBrush(AnalogClock.minuteColor));

    painter.save();
    painter.rotate(6.0 * (time.getMinutes() + time.getSeconds() / 60.0));
    painter.drawConvexPolygon(AnalogClock.minuteHand);
    painter.restore();

    painter.setPen(AnalogClock.minuteColor);

    for (var j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
	painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    painter.end();
};

AnalogClock.hourColor = new QColor(127, 0, 127);

AnalogClock.minuteColor = new QColor(0, 127, 127, 191);

AnalogClock.hourHand = new QPolygon([new QPoint(7, 8),
                                     new QPoint(-7, 8),
                                     new QPoint(0, -40)]);
AnalogClock.minuteHand = new QPolygon([new QPoint(7, 8),
                                       new QPoint(-7, 8),
                                       new QPoint(0, -70)]);

var clock = new AnalogClock();
clock.show();

QCoreApplication.exec();
