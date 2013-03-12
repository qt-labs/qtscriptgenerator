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

#ifndef PP_ITERATOR_H
#define PP_ITERATOR_H

#include <iterator>

namespace rpp {

class pp_null_output_iterator
  : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
  pp_null_output_iterator() {}

  template <typename _Tp>
  pp_null_output_iterator &operator=(_Tp const &)
  { return *this; }

  inline pp_null_output_iterator &operator * () { return *this; }
  inline pp_null_output_iterator &operator ++ () { return *this; }
  inline pp_null_output_iterator operator ++ (int) { return *this; }
};

template <typename _Container>
class pp_output_iterator
  : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
  std::string &_M_result;

public:
  explicit pp_output_iterator(std::string &__result):
    _M_result (__result) {}

  inline pp_output_iterator &operator=(typename _Container::const_reference __v)
  {
    if (_M_result.capacity () == _M_result.size ())
      _M_result.reserve (_M_result.capacity () << 2);

    _M_result.push_back(__v);
    return *this;
  }

  inline pp_output_iterator &operator * () { return *this; }
  inline pp_output_iterator &operator ++ () { return *this; }
  inline pp_output_iterator operator ++ (int) { return *this; }
};

} // namespace rpp

#endif // PP_ITERATOR_H

// kate: space-indent on; indent-width 2; replace-tabs on;
