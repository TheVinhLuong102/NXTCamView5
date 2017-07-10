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

#include "debugmessagesmodel.h"

using namespace QmlProfiler;

namespace QmlProfilerExtension {
namespace Internal {

bool DebugMessagesModel::accepted(const QmlProfilerDataModel::QmlEventTypeData &event) const
{
    return event.message == QmlDebug::DebugMessage;
}

DebugMessagesModel::DebugMessagesModel(QmlProfilerModelManager *manager, QObject *parent) :
    QmlProfilerTimelineModel(manager, QmlDebug::DebugMessage, QmlDebug::MaximumRangeType,
                             QmlDebug::ProfileDebugMessages, parent), m_maximumMsgType(-1)
{
}

int DebugMessagesModel::typeId(int index) const
{
    return m_data[index].typeId;
}

QColor DebugMessagesModel::color(int index) const
{
    return colorBySelectionId(index);
}

static const char *messageTypes[] = {
    QT_TRANSLATE_NOOP("DebugMessagesModel", "Debug Message"),
    QT_TRANSLATE_NOOP("DebugMessagesModel", "Warning Message"),
    QT_TRANSLATE_NOOP("DebugMessagesModel", "Critical Message"),
    QT_TRANSLATE_NOOP("DebugMessagesModel", "Fatal Message"),
    QT_TRANSLATE_NOOP("DebugMessagesModel", "Info Message"),
};

QString DebugMessagesModel::messageType(uint i)
{
    return i < sizeof(messageTypes) / sizeof(char *) ? tr(messageTypes[i]) :
                                                       tr("Unknown Message %1").arg(i);
}

QVariantList DebugMessagesModel::labels() const
{
    QVariantList result;

    for (int i = 0; i <= m_maximumMsgType; ++i) {
        QVariantMap element;
        element.insert(QLatin1String("description"), messageType(i));
        element.insert(QLatin1String("id"), i);
        result << element;
    }
    return result;
}

QVariantMap DebugMessagesModel::details(int index) const
{
    const QmlProfilerDataModel::QmlEventTypeData &type =
            modelManager()->qmlModel()->getEventTypes()[m_data[index].typeId];

    QVariantMap result;
    result.insert(QLatin1String("displayName"), messageType(type.detailType));
    result.insert(tr("Timestamp"), QmlProfilerDataModel::formatTime(startTime(index)));
    result.insert(tr("Message"), m_data[index].text);
    result.insert(tr("Location"), type.displayName);
    return result;
}

int DebugMessagesModel::expandedRow(int index) const
{
    return selectionId(index) + 1;
}

int DebugMessagesModel::collapsedRow(int index) const
{
    Q_UNUSED(index);
    return 1;
}

void DebugMessagesModel::loadData()
{
    QmlProfilerDataModel *simpleModel = modelManager()->qmlModel();
    if (simpleModel->isEmpty())
        return;

    const QVector<QmlProfilerDataModel::QmlEventTypeData> &types = simpleModel->getEventTypes();

    foreach (const QmlProfilerDataModel::QmlEventData &event, simpleModel->getEvents()) {
        const QmlProfilerDataModel::QmlEventTypeData &type = types[event.typeIndex()];
        if (!accepted(type) || event.startTime() < 0)
            continue;

        m_data.insert(insert(event.startTime(), 0, type.detailType),
                      MessageData(event.stringData(), event.typeIndex()));
        if (type.detailType > m_maximumMsgType)
            m_maximumMsgType = event.typeIndex();
        updateProgress(count(), simpleModel->getEvents().count());
    }
    setCollapsedRowCount(2);
    setExpandedRowCount(m_maximumMsgType + 2);
    updateProgress(1, 1);
}

void DebugMessagesModel::clear()
{
    m_data.clear();
    m_maximumMsgType = -1;
    QmlProfilerTimelineModel::clear();
}

QVariantMap DebugMessagesModel::location(int index) const
{
    return locationFromTypeId(index);
}

}
}
