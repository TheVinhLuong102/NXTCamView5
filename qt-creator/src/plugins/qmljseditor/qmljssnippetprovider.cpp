/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "qmljssnippetprovider.h"
#include "qmljshighlighter.h"
#include "qmljseditor.h"
#include "qmljsautocompleter.h"
#include "qmljseditorconstants.h"

#include <texteditor/texteditorsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/snippets/snippeteditor.h>

#include <qmljstools/qmljsindenter.h>

#include <QLatin1String>
#include <QCoreApplication>

using namespace QmlJSEditor;
using namespace Internal;

QmlJSSnippetProvider::QmlJSSnippetProvider() :
    TextEditor::ISnippetProvider()
{}

QmlJSSnippetProvider::~QmlJSSnippetProvider()
{}

QString QmlJSSnippetProvider::groupId() const
{
    return QLatin1String(Constants::QML_SNIPPETS_GROUP_ID);
}

QString QmlJSSnippetProvider::displayName() const
{
    return QCoreApplication::translate("QmlJSEditor::Internal::QmlJSSnippetProvider", "QML");
}

void QmlJSSnippetProvider::decorateEditor(TextEditor::SnippetEditorWidget *editor) const
{
    editor->textDocument()->setSyntaxHighlighter(new QmlJSHighlighter);
    editor->textDocument()->setIndenter(new Indenter);
    editor->setAutoCompleter(new AutoCompleter);
}
