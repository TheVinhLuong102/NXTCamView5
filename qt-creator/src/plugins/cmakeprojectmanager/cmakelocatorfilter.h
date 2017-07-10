/****************************************************************************
**
** Copyright (C) 2016 Kläralvdalens Datakonsult AB, a KDAB Group company.
** Contact: Kläralvdalens Datakonsult AB (info@kdab.com)
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

#pragma once

#include <coreplugin/locator/ilocatorfilter.h>

#include <QIcon>

namespace CMakeProjectManager {
namespace Internal {

class CMakeLocatorFilter : public Core::ILocatorFilter
{
    Q_OBJECT

public:
    CMakeLocatorFilter();

    void prepareSearch(const QString &entry) override;
    QList<Core::LocatorFilterEntry> matchesFor(QFutureInterface<Core::LocatorFilterEntry> &future,
                                               const QString &entry) override;
    void accept(Core::LocatorFilterEntry selection) const override;
    void refresh(QFutureInterface<void> &future) override;

private:
    void slotProjectListUpdated();

    QList<Core::LocatorFilterEntry> m_result;
};

} // namespace Internal
} // namespace CMakeProjectManager
