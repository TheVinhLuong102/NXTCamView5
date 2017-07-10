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

#ifndef CLANGBACKEND_DIAGNOSTICSET_H
#define CLANGBACKEND_DIAGNOSTICSET_H

#include "diagnostic.h"
#include "diagnosticsetiterator.h"

#include <clang-c/Index.h>

#include <QVector>

#include <functional>

namespace ClangBackEnd {

class DiagnosticSetIterator;

class DiagnosticSet
{
    friend class TranslationUnit;
    friend class Diagnostic;

public:
    using ConstIterator = DiagnosticSetIterator;

public:
    ~DiagnosticSet();

    DiagnosticSet(const DiagnosticSet &) = delete;
    const DiagnosticSet &operator=(const DiagnosticSet &) = delete;

    DiagnosticSet(DiagnosticSet &&other);
    DiagnosticSet &operator=(DiagnosticSet &&other);

    uint size() const;
    bool isNull() const;

    Diagnostic at(uint index) const;

    Diagnostic front() const;
    Diagnostic back() const;

    ConstIterator begin() const;
    ConstIterator end() const;

    using IsAcceptedDiagnostic = std::function<bool (const Diagnostic &)>;
    QVector<DiagnosticContainer> toDiagnosticContainers() const;
    QVector<DiagnosticContainer> toDiagnosticContainers(
            const IsAcceptedDiagnostic &isAcceptedDiagnostic) const;

private:
    DiagnosticSet(CXDiagnosticSet cxDiagnosticSet);

private:
    CXDiagnosticSet cxDiagnosticSet;
};

} // namespace ClangBackEnd

#endif // CLANGBACKEND_DIAGNOSTICSET_H