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

#ifndef AUTOCOMPLETER_H
#define AUTOCOMPLETER_H

#include "texteditor_global.h"

#include <QString>

QT_BEGIN_NAMESPACE
class QTextCursor;
QT_END_NAMESPACE

namespace TextEditor {

class TabSettings;

class TEXTEDITOR_EXPORT AutoCompleter
{
public:
    AutoCompleter();
    virtual ~AutoCompleter();

    void setAutoParenthesesEnabled(bool b);
    bool isAutoParenthesesEnabled() const;

    void setSurroundWithEnabled(bool b);
    bool isSurroundWithEnabled() const;

    // Returns the text to complete at the cursor position, or an empty string
    virtual QString autoComplete(QTextCursor &cursor, const QString &text) const;

    // Handles backspace. When returning true, backspace processing is stopped
    virtual bool autoBackspace(QTextCursor &cursor);

    // Hook to insert special characters on enter. Returns the number of extra blocks inserted.
    virtual int paragraphSeparatorAboutToBeInserted(QTextCursor &cursor,
                                                    const TabSettings &tabSettings);

    virtual bool contextAllowsAutoParentheses(const QTextCursor &cursor,
                                              const QString &textToInsert = QString()) const;
    virtual bool contextAllowsElectricCharacters(const QTextCursor &cursor) const;

    // Returns true if the cursor is inside a comment.
    virtual bool isInComment(const QTextCursor &cursor) const;

    // Returns true if the cursor is inside a string.
    virtual bool isInString(const QTextCursor &cursor) const;

    virtual QString insertMatchingBrace(const QTextCursor &cursor, const
                                        QString &text,
                                        QChar la,
                                        int *skippedChars) const;

    // Returns the text that needs to be inserted
    virtual QString insertParagraphSeparator(const QTextCursor &cursor) const;

protected:
    static void countBracket(QChar open, QChar close, QChar c, int *errors, int *stillopen);
    static void countBrackets(QTextCursor cursor, int from, int end, QChar open, QChar close,
                              int *errors, int *stillopen);

private:
    mutable bool m_allowSkippingOfBlockEnd;
    bool m_surroundWithEnabled;
    bool m_autoParenthesesEnabled;
};

} // TextEditor

#endif // AUTOCOMPLETER_H
