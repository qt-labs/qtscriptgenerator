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

#include "prigenerator.h"
#include "reporthandler.h"
#include "fileout.h"

void PriGenerator::addHeader(const QString &folder, const QString &header)
{
    priHash[folder].headers << header;
}

void PriGenerator::addSource(const QString &folder, const QString &source)
{
    priHash[folder].sources << source;
}

void PriGenerator::generate()
{
    QHashIterator<QString, Pri> pri(priHash);
    while (pri.hasNext()) {
        pri.next();
        QStringList list = pri.value().headers;
        if (list.isEmpty())
            continue;

        QString folder = pri.key();
        FileOut file(m_out_dir + "/generated_cpp/" + folder + "/" + folder + ".pri");
        file.stream << "HEADERS += \\\n";
        qSort(list.begin(), list.end());
        foreach (const QString &entry, list) {
            file.stream << "           $$PWD/" << entry << " \\\n";
        }

        file.stream << "\n";
        file.stream << "SOURCES += \\\n";
        list = pri.value().sources;
        qSort(list.begin(), list.end());
        foreach (const QString &entry, list) {
            file.stream << "           $$PWD/" << entry << " \\\n";
        }
        file.stream << "           $$PWD/" << folder << "_init.cpp\n";

        if (file.done())
            ++m_num_generated_written;
        ++m_num_generated;
    }
}
