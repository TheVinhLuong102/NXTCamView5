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

#ifndef CLANGBACKEND_SKIPPEDSOURCERANGES_H
#define CLANGBACKEND_SKIPPEDSOURCERANGES_H

#include "sourcerange.h"

namespace ClangBackEnd {

class SourceRangeContainer;

class SkippedSourceRanges
{
public:
    SkippedSourceRanges(CXTranslationUnit TranslationUnit, const char *filePath);
    ~SkippedSourceRanges();

    SkippedSourceRanges(const SkippedSourceRanges &) = delete;
    const SkippedSourceRanges &operator=(const SkippedSourceRanges &) = delete;

    SkippedSourceRanges(SkippedSourceRanges &&);
    SkippedSourceRanges &operator=(SkippedSourceRanges &&);

    std::vector<SourceRange> sourceRanges() const;

    QVector<SourceRangeContainer> toSourceRangeContainers() const;

    operator QVector<SourceRangeContainer>() const;

private:
    CXSourceRangeList *cxSkippedSourceRanges;
};

} // namespace ClangBackEnd

#endif // CLANGBACKEND_SKIPPEDSOURCERANGES_H
