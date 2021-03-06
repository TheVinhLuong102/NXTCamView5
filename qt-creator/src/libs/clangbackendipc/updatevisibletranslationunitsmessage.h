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

#ifndef CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H
#define CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H

#include <clangbackendipc_global.h>

#include <utf8stringvector.h>

namespace ClangBackEnd {

class CMBIPC_EXPORT UpdateVisibleTranslationUnitsMessage
{
    friend CMBIPC_EXPORT QDataStream &operator<<(QDataStream &out, const UpdateVisibleTranslationUnitsMessage &message);
    friend CMBIPC_EXPORT QDataStream &operator>>(QDataStream &in, UpdateVisibleTranslationUnitsMessage &message);
    friend CMBIPC_EXPORT bool operator==(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);
public:
    UpdateVisibleTranslationUnitsMessage() = default;
    UpdateVisibleTranslationUnitsMessage(const Utf8String &currentEditorFilePath,
                                         const Utf8StringVector &visibleEditorFilePaths);

    const Utf8String &currentEditorFilePath() const;
    const Utf8StringVector &visibleEditorFilePaths() const;

private:
    Utf8String currentEditorFilePath_;
    Utf8StringVector visibleEditorFilePaths_;
};

CMBIPC_EXPORT QDataStream &operator<<(QDataStream &out, const UpdateVisibleTranslationUnitsMessage &message);
CMBIPC_EXPORT QDataStream &operator>>(QDataStream &in, UpdateVisibleTranslationUnitsMessage &message);
CMBIPC_EXPORT bool operator==(const UpdateVisibleTranslationUnitsMessage &first, const UpdateVisibleTranslationUnitsMessage &second);

CMBIPC_EXPORT QDebug operator<<(QDebug debug, const UpdateVisibleTranslationUnitsMessage &message);
void PrintTo(const UpdateVisibleTranslationUnitsMessage &message, ::std::ostream* os);

DECLARE_MESSAGE(UpdateVisibleTranslationUnitsMessage)
} // namespace ClangBackEnd

#endif // CLANGBACKEND_UPDATEVISIBLETRANSLATIONUNITSMESSAGE_H
