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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "metajava.h"
#include "typesystem.h"

#include "codemodel.h"

#include <QObject>
#include <QFile>

class Generator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString outputDirectory READ outputDirectory WRITE setOutputDirectory);

public:
    enum Option {
        NoOption                 = 0x00000000,
        BoxedPrimitive           = 0x00000001,
        ExcludeConst             = 0x00000002,
        ExcludeReference         = 0x00000004,
        UseNativeIds             = 0x00000008,

        EnumAsInts               = 0x00000010,
        SkipName                 = 0x00000020,
        NoCasts                  = 0x00000040,
        SkipReturnType           = 0x00000080,
        OriginalName             = 0x00000100,
        ShowStatic               = 0x00000200,
        UnderscoreSpaces         = 0x00000400,
        ForceEnumCast            = 0x00000800,
        ArrayAsPointer           = 0x00001000,
        VirtualCall              = 0x00002000,
        SkipTemplateParameters   = 0x00004000,
        SkipAttributes           = 0x00008000,
        OriginalTypeDescription  = 0x00010000,
        SkipRemovedArguments     = 0x00020000,
        IncludeDefaultExpression = 0x00040000,
        NoReturnStatement        = 0x00080000,
        NoBlockedSlot            = 0x00100000,

        SuperCall                = 0x00200000,

        GlobalRefJObject         = 0x00100000,

        ForceValueType           = ExcludeReference | ExcludeConst
    };

    Generator();

    void setClasses(const AbstractMetaClassList &classes) { m_classes = classes; }
    AbstractMetaClassList classes() const { return m_classes; }

    QString outputDirectory() const { return m_out_dir; }
    void setOutputDirectory(const QString &outDir) { m_out_dir = outDir; }
    virtual void generate();
    void printClasses();

    int numGenerated() { return m_num_generated; }
    int numGeneratedAndWritten() { return m_num_generated_written; }

    virtual bool shouldGenerate(const AbstractMetaClass *) const { return true; }
    virtual QString subDirectoryForClass(const AbstractMetaClass *java_class) const;
    virtual QString fileNameForClass(const AbstractMetaClass *java_class) const;
    virtual void write(QTextStream &s, const AbstractMetaClass *java_class);

    bool hasDefaultConstructor(const AbstractMetaType *type);

    // QtScript
    void setQtMetaTypeDeclaredTypeNames(const QSet<QString> &names)
        { m_qmetatype_declared_typenames = names; }
    QSet<QString> qtMetaTypeDeclaredTypeNames() const
        { return m_qmetatype_declared_typenames; }

protected:
    void verifyDirectoryFor(const QFile &file);

    AbstractMetaClassList m_classes;
    int m_num_generated;
    int m_num_generated_written;
    QString m_out_dir;

    // QtScript
    QSet<QString> m_qmetatype_declared_typenames;
};

class Indentor {
public:
    Indentor():
        indent(0)
        {}
    int indent;
};

class Indentation {
public:
    Indentation(Indentor &indentor):
        indentor(indentor)
        {
            indentor.indent++;
        }
    ~Indentation()
        {
            indentor.indent--;
        }

private:
    Indentor &indentor;
};

inline QTextStream &operator <<(QTextStream &s, const Indentor &indentor)
{
    for (int i=0; i<indentor.indent; ++i)
        s << "    ";
    return s;
}


#endif // GENERATOR_H
