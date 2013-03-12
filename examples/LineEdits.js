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

function Window(parent) {
    QWidget.call(this, parent);

    var echoGroup = new QGroupBox(tr("Echo"));

    var echoLabel = new QLabel(tr("Mode:"));
    var echoComboBox = new QComboBox();
    echoComboBox.addItem(tr("Normal"));
    echoComboBox.addItem(tr("Password"));
    echoComboBox.addItem(tr("PasswordEchoOnEdit"));
    echoComboBox.addItem(tr("No Echo"));

    this.echoLineEdit = new QLineEdit();
    this.echoLineEdit.setFocus();

    var validatorGroup = new QGroupBox(tr("Validator"));

    var validatorLabel = new QLabel(tr("Type:"));
    var validatorComboBox = new QComboBox();
    validatorComboBox.addItem(tr("No validator"));
    validatorComboBox.addItem(tr("Integer validator"));
    validatorComboBox.addItem(tr("Double validator"));

    this.validatorLineEdit = new QLineEdit();

    var alignmentGroup = new QGroupBox(tr("Alignment"));

    var alignmentLabel = new QLabel(tr("Type:"));
    var alignmentComboBox = new QComboBox();
    alignmentComboBox.addItem(tr("Left"));
    alignmentComboBox.addItem(tr("Centered"));
    alignmentComboBox.addItem(tr("Right"));

    this.alignmentLineEdit = new QLineEdit();

    var inputMaskGroup = new QGroupBox(tr("Input mask"));

    var inputMaskLabel = new QLabel(tr("Type:"));
    var inputMaskComboBox = new QComboBox;
    inputMaskComboBox.addItem(tr("No mask"));
    inputMaskComboBox.addItem(tr("Phone number"));
    inputMaskComboBox.addItem(tr("ISO date"));
    inputMaskComboBox.addItem(tr("License key"));

    this.inputMaskLineEdit = new QLineEdit();

    var accessGroup = new QGroupBox(tr("Access"));

    var accessLabel = new QLabel(tr("Read-only:"));
    var accessComboBox = new QComboBox;
    accessComboBox.addItem(tr("False"));
    accessComboBox.addItem(tr("True"));

    this.accessLineEdit = new QLineEdit();

    echoComboBox["activated(int)"].connect(
        this, "echoChanged");
    validatorComboBox["activated(int)"].connect(
        this, "validatorChanged");
    alignmentComboBox["activated(int)"].connect(
        this, "alignmentChanged");
    inputMaskComboBox["activated(int)"].connect(
        this, "inputMaskChanged");
    accessComboBox["activated(int)"].connect(
        this, "accessChanged");

    var echoLayout = new QGridLayout;
    echoLayout.addWidget(echoLabel, 0, 0);
    echoLayout.addWidget(echoComboBox, 0, 1);
    echoLayout.addWidget(this.echoLineEdit, 1, 0, 1, 2);
    echoGroup.setLayout(echoLayout);

    var validatorLayout = new QGridLayout;
    validatorLayout.addWidget(validatorLabel, 0, 0);
    validatorLayout.addWidget(validatorComboBox, 0, 1);
    validatorLayout.addWidget(this.validatorLineEdit, 1, 0, 1, 2);
    validatorGroup.setLayout(validatorLayout);

    var alignmentLayout = new QGridLayout;
    alignmentLayout.addWidget(alignmentLabel, 0, 0);
    alignmentLayout.addWidget(alignmentComboBox, 0, 1);
    alignmentLayout.addWidget(this.alignmentLineEdit, 1, 0, 1, 2);
    alignmentGroup.setLayout(alignmentLayout);

    var inputMaskLayout = new QGridLayout;
    inputMaskLayout.addWidget(inputMaskLabel, 0, 0);
    inputMaskLayout.addWidget(inputMaskComboBox, 0, 1);
    inputMaskLayout.addWidget(this.inputMaskLineEdit, 1, 0, 1, 2);
    inputMaskGroup.setLayout(inputMaskLayout);

    var accessLayout = new QGridLayout;
    accessLayout.addWidget(accessLabel, 0, 0);
    accessLayout.addWidget(accessComboBox, 0, 1);
    accessLayout.addWidget(this.accessLineEdit, 1, 0, 1, 2);
    accessGroup.setLayout(accessLayout);

    var layout = new QGridLayout;
    layout.addWidget(echoGroup, 0, 0);
    layout.addWidget(validatorGroup, 1, 0);
    layout.addWidget(alignmentGroup, 2, 0);
    layout.addWidget(inputMaskGroup, 0, 1);
    layout.addWidget(accessGroup, 1, 1);
    this.setLayout(layout);

    this.setWindowTitle(tr("Line Edits"));
}

Window.prototype = new QWidget();

Window.prototype.echoChanged = function(index) {
    switch (index) {
    case 0:
        this.echoLineEdit.echoMode = QLineEdit.Normal;
        break;
    case 1:
        this.echoLineEdit.echoMode = QLineEdit.Password;
        break;
    case 2:
    	this.echoLineEdit.echoMode = QLineEdit.PasswordEchoOnEdit;
        break;
    case 3:
        this.echoLineEdit.echoMode = QLineEdit.NoEcho;
        break;
    }
};

Window.prototype.validatorChanged = function(index) {
    switch (index) {
    case 0:
        this.validatorLineEdit.setValidator(null);
        break;
    case 1:
        this.validatorLineEdit.setValidator(
            new QIntValidator(this.validatorLineEdit));
        break;
    case 2:
        this.validatorLineEdit.setValidator(
            new QDoubleValidator(-999.0, 999.0, 2, this.validatorLineEdit));
        break;
    }
    this.validatorLineEdit.clear();
};

Window.prototype.alignmentChanged = function(index) {
    switch (index) {
    case 0:
        this.alignmentLineEdit.alignment = Qt.Alignment(Qt.AlignLeft);
        break;
    case 1:
        this.alignmentLineEdit.alignment = Qt.Alignment(Qt.AlignCenter);
        break;
    case 2:
    	this.alignmentLineEdit.alignment = Qt.Alignment(Qt.AlignRight);
    }
};

Window.prototype.inputMaskChanged = function(index)
{
    switch (index) {
    case 0:
        this.inputMaskLineEdit.inputMask = "";
        break;
    case 1:
        this.inputMaskLineEdit.inputMask = "+99 99 99 99 99;_";
        break;
    case 2:
        this.inputMaskLineEdit.inputMask = "0000-00-00";
        this.inputMaskLineEdit.text = "00000000";
        this.inputMaskLineEdit.cursorPosition = 0;
        break;
    case 3:
        this.inputMaskLineEdit.inputMask = ">AAAAA-AAAAA-AAAAA-AAAAA-AAAAA;#";
    }
};

Window.prototype.accessChanged = function(index) {
    switch (index) {
    case 0:
        this.accessLineEdit.readOnly = false;
        break;
    case 1:
        this.accessLineEdit.readOnly = true;
    }
};

var win = new Window(null);
win.show();

QCoreApplication.exec();
