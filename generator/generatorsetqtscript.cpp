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

#include "generatorsetqtscript.h"
#include "reporthandler.h"
#include "classgenerator.h"
#include "shellheadergenerator.h"
#include "shellimplgenerator.h"
#include "docgenerator.h"

GeneratorSet *GeneratorSet::getInstance() {
    return new GeneratorSetQtScript();
}

GeneratorSetQtScript::GeneratorSetQtScript() 
{}

QString GeneratorSetQtScript::usage() {
    QString usage =
        "QtScript:\n" 
        "  --nothing-to-report-yet                   \n";

    return usage;
}

bool GeneratorSetQtScript::readParameters(const QMap<QString, QString> args) {
    return GeneratorSet::readParameters(args);
}

void GeneratorSetQtScript::buildModel(const QString pp_file) {
    // Building the code inforamation...
    ReportHandler::setContext("MetaJavaBuilder");
    builder.setFileName(pp_file);
    builder.build();
}

void GeneratorSetQtScript::dumpObjectTree() {
 
}

QString GeneratorSetQtScript::generate() {
    AbstractMetaClassList classes = builder.classesTopologicalSorted();
    QSet<QString> declaredTypeNames = builder.qtMetaTypeDeclaredTypeNames();

    PriGenerator priGenerator;
    priGenerator.setOutputDirectory(outDir);

    SetupGenerator setupGenerator;
    setupGenerator.setOutputDirectory(outDir);
    setupGenerator.setQtMetaTypeDeclaredTypeNames(declaredTypeNames);

    ClassGenerator classGenerator(&priGenerator, &setupGenerator);
    classGenerator.setOutputDirectory(outDir);
    classGenerator.setClasses(classes);
    classGenerator.setQtMetaTypeDeclaredTypeNames(declaredTypeNames);
    classGenerator.generate();

    ShellImplGenerator shellImplGenerator(&priGenerator);
    shellImplGenerator.setOutputDirectory(outDir);
    shellImplGenerator.setClasses(classes);
    shellImplGenerator.setQtMetaTypeDeclaredTypeNames(declaredTypeNames);
    shellImplGenerator.generate();

    ShellHeaderGenerator shellHeaderGenerator(&priGenerator);
    shellHeaderGenerator.setOutputDirectory(outDir);
    shellHeaderGenerator.setClasses(classes);
    shellHeaderGenerator.generate();

    DocGenerator docGenerator;
    docGenerator.setOutputDirectory(outDir);
    docGenerator.setClasses(classes);
    docGenerator.generate();

    priGenerator.generate();
    setupGenerator.generate();

    return QString("Classes in typesystem: %1\n"
                   "Generated:\n"
                   "  - classes...: %2 (%3)\n"
                   "  - header....: %4 (%5)\n"
                   "  - impl......: %6 (%7)\n"
                   "  - modules...: %8 (%9)\n"
                   "  - pri.......: %10 (%11)\n"
                   )
        .arg(builder.classes().size())
        .arg(classGenerator.numGenerated())
        .arg(classGenerator.numGeneratedAndWritten())

        .arg(shellHeaderGenerator.numGenerated())
        .arg(shellHeaderGenerator.numGeneratedAndWritten())

        .arg(shellImplGenerator.numGenerated())
        .arg(shellImplGenerator.numGeneratedAndWritten())

        .arg(setupGenerator.numGenerated())
        .arg(setupGenerator.numGeneratedAndWritten())

        .arg(priGenerator.numGenerated())
        .arg(priGenerator.numGeneratedAndWritten());
}
