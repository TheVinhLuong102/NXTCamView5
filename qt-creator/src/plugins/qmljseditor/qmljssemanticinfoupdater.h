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

#ifndef SEMANTICINFOUPDATER_H
#define SEMANTICINFOUPDATER_H

#include "qmljseditor.h"

#include <QWaitCondition>
#include <QModelIndex>
#include <QMutex>
#include <QThread>

namespace QmlJSEditor {
namespace Internal {

class SemanticInfoUpdater: public QThread
{
    Q_OBJECT

public:
    SemanticInfoUpdater(QObject *parent = 0);
    virtual ~SemanticInfoUpdater();

    void abort();
    void update(const QmlJS::Document::Ptr &doc, const QmlJS::Snapshot &snapshot);
    void reupdate(const QmlJS::Snapshot &snapshot);

Q_SIGNALS:
    void updated(const QmlJSTools::SemanticInfo &semanticInfo);

protected:
    virtual void run();

private:
    QmlJSTools::SemanticInfo makeNewSemanticInfo(const QmlJS::Document::Ptr &doc,
                                                 const QmlJS::Snapshot &snapshot);

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_wasCancelled;
    QmlJS::Document::Ptr m_sourceDocument;
    QmlJS::Snapshot m_sourceSnapshot;
    QmlJSTools::SemanticInfo m_lastSemanticInfo;
};

} // namespace Internal
} // namespace QmlJSEditor

#endif // SEMANTICINFOUPDATER_H
