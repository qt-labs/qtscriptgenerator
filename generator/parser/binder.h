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


#ifndef BINDER_H
#define BINDER_H

#include "default_visitor.h"
#include "codemodel.h"
#include "type_compiler.h"
#include "name_compiler.h"
#include "declarator_compiler.h"

class TokenStream;
class LocationManager;
class Control;
struct NameSymbol;

class Binder: protected DefaultVisitor
{
public:
  Binder(CodeModel *__model, LocationManager &__location, Control *__control = 0);
  virtual ~Binder();

  inline TokenStream *tokenStream() const { return _M_token_stream; }
  inline CodeModel *model() const { return _M_model; }
  ScopeModelItem currentScope();

  FileModelItem run(AST *node);

// utils
  TypeInfo qualifyType(const TypeInfo &type, const QStringList &context) const;

protected:
  virtual void visitAccessSpecifier(AccessSpecifierAST *);
  virtual void visitClassSpecifier(ClassSpecifierAST *);
  virtual void visitEnumSpecifier(EnumSpecifierAST *);
  virtual void visitEnumerator(EnumeratorAST *);
  virtual void visitFunctionDefinition(FunctionDefinitionAST *);
  virtual void visitLinkageSpecification(LinkageSpecificationAST *);
  virtual void visitNamespace(NamespaceAST *);
  virtual void visitSimpleDeclaration(SimpleDeclarationAST *);
  virtual void visitTemplateDeclaration(TemplateDeclarationAST *);
  virtual void visitTypedef(TypedefAST *);
  virtual void visitUsing(UsingAST *);
  virtual void visitUsingDirective(UsingDirectiveAST *);
  virtual void visitQProperty(QPropertyAST *);
  virtual void visitForwardDeclarationSpecifier(ForwardDeclarationSpecifierAST *);
  virtual void visitQEnums(QEnumsAST *);

private:

  int decode_token(std::size_t index) const;
  const NameSymbol *decode_symbol(std::size_t index) const;
  CodeModel::AccessPolicy decode_access_policy(std::size_t index) const;
  CodeModel::ClassType decode_class_type(std::size_t index) const;

  CodeModel::FunctionType changeCurrentFunctionType(CodeModel::FunctionType functionType);
  CodeModel::AccessPolicy changeCurrentAccess(CodeModel::AccessPolicy accessPolicy);
  NamespaceModelItem changeCurrentNamespace(NamespaceModelItem item);
  ClassModelItem changeCurrentClass(ClassModelItem item);
  FunctionDefinitionModelItem changeCurrentFunction(FunctionDefinitionModelItem item);
  TemplateParameterList changeTemplateParameters(TemplateParameterList templateParameters);

  void declare_symbol(SimpleDeclarationAST *node, InitDeclaratorAST *init_declarator);

  void applyStorageSpecifiers(const ListNode<std::size_t> *storage_specifiers, MemberModelItem item);
  void applyFunctionSpecifiers(const ListNode<std::size_t> *it, FunctionModelItem item);

  void updateItemPosition(CodeModelItem item, AST *node);

  QString exceptionSpecToString(ExceptionSpecificationAST *exception_spec);

private:
  CodeModel *_M_model;
  LocationManager &_M_location;
  TokenStream *_M_token_stream;
  Control *_M_control;

  CodeModel::FunctionType _M_current_function_type;
  CodeModel::AccessPolicy _M_current_access;
  FileModelItem _M_current_file;
  NamespaceModelItem _M_current_namespace;
  ClassModelItem _M_current_class;
  FunctionDefinitionModelItem _M_current_function;
  EnumModelItem _M_current_enum;
  QStringList _M_context;
  TemplateParameterList _M_current_template_parameters; // ### check me
  QHash<QString, QString> _M_qualified_types;
  QHash<QString, int> _M_anonymous_enums;

protected:
  TypeCompiler type_cc;
  NameCompiler name_cc;
  DeclaratorCompiler decl_cc;
};

#endif // BINDER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
