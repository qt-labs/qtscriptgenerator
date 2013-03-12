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


#ifndef DECLARATOR_COMPILER_H
#define DECLARATOR_COMPILER_H

#include "default_visitor.h"
#include "codemodel.h"

#include <QtCore/QString>
#include <QtCore/QList>

class TokenStream;
class Binder;

class DeclaratorCompiler: protected DefaultVisitor
{
public:
  struct Parameter
  {
    TypeInfo type;
    QString name;
    QString defaultValueExpression;
    bool defaultValue;

    Parameter(): defaultValue(false) {}
  };

public:
  DeclaratorCompiler(Binder *binder);

  void run(DeclaratorAST *node);

  inline QString id() const { return _M_id; }
  inline QStringList arrayElements() const { return _M_array; }
  inline bool isFunction() const { return _M_function; }
  inline bool isVariadics() const { return _M_variadics; }
  inline bool isReference() const { return _M_reference; }
  inline int indirection() const { return _M_indirection; }
  inline QList<Parameter> parameters() const { return _M_parameters; }

protected:
  virtual void visitPtrOperator(PtrOperatorAST *node);
  virtual void visitParameterDeclaration(ParameterDeclarationAST *node);

private:
  Binder *_M_binder;
  TokenStream *_M_token_stream;

  bool _M_function;
  bool _M_reference;
  bool _M_variadics;
  int _M_indirection;
  QString _M_id;
  QStringList _M_array;
  QList<Parameter> _M_parameters;
};

#endif // DECLARATOR_COMPILER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
