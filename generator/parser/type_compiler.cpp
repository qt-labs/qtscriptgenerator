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

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


#include "type_compiler.h"
#include "name_compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "tokens.h"
#include "binder.h"

#include <QtCore/QString>

TypeCompiler::TypeCompiler(Binder *binder)
  : _M_binder (binder), _M_token_stream(binder->tokenStream ())
{
}

void TypeCompiler::run(TypeSpecifierAST *node)
{
  _M_type.clear();
  _M_cv.clear();

  visit(node);

  if (node && node->cv)
    {
      const ListNode<std::size_t> *it = node->cv->toFront();
      const ListNode<std::size_t> *end = it;
      do
        {
          int kind = _M_token_stream->kind(it->element);
          if (! _M_cv.contains(kind))
            _M_cv.append(kind);

          it = it->next;
        }
      while (it != end);
    }
}

void TypeCompiler::visitClassSpecifier(ClassSpecifierAST *node)
{
  visit(node->name);
}

void TypeCompiler::visitEnumSpecifier(EnumSpecifierAST *node)
{
  visit(node->name);
}

void TypeCompiler::visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *node)
{
  visit(node->name);
}

void TypeCompiler::visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node)
{
  if (const ListNode<std::size_t> *it = node->integrals)
    {
      it = it->toFront();
      const ListNode<std::size_t> *end = it;
      QString current_item;
      do
        {
          std::size_t token = it->element;
          current_item += token_name(_M_token_stream->kind(token));
          current_item += " ";
          it = it->next;
        }
      while (it != end);
      _M_type += current_item.trimmed();
    }
  else if (node->type_of)
    {
      // ### implement me
      _M_type += QLatin1String("typeof<...>");
    }

  visit(node->name);
}

void TypeCompiler::visitName(NameAST *node)
{
  NameCompiler name_cc(_M_binder);
  name_cc.run(node);
  _M_type = name_cc.qualifiedName();
}

QStringList TypeCompiler::cvString() const
{
  QStringList lst;

  foreach (int q, cv())
    {
      if (q == Token_const)
        lst.append(QLatin1String("const"));
      else if (q == Token_volatile)
        lst.append(QLatin1String("volatile"));
    }

  return lst;
}

bool TypeCompiler::isConstant() const
{
  return _M_cv.contains(Token_const);
}

bool TypeCompiler::isVolatile() const
{
  return _M_cv.contains(Token_volatile);
}

// kate: space-indent on; indent-width 2; replace-tabs on;
