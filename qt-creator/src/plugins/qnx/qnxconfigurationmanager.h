/****************************************************************************
**
** Copyright (C) 2016 BlackBerry Limited. All rights reserved.
** Contact: BlackBerry (qt@blackberry.com)
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

#ifndef QNXCONFIGURATIONMANAGER_H
#define QNXCONFIGURATIONMANAGER_H

#include <utils/fileutils.h>

namespace Utils { class PersistentSettingsWriter; }
namespace Qnx {
namespace Internal {

class QnxConfiguration;
class QnxPlugin;

class QnxConfigurationManager: public QObject
{
    Q_OBJECT
public:
    static QnxConfigurationManager *instance();
    ~QnxConfigurationManager();
    QList<QnxConfiguration*> configurations() const;
    void removeConfiguration(QnxConfiguration *config);
    bool addConfiguration(QnxConfiguration *config);
    QnxConfiguration* configurationFromEnvFile(const Utils::FileName &envFile) const;

protected slots:
    void saveConfigs();

signals:
    void configurationsListUpdated();

private:
    QnxConfigurationManager(QObject *parent = 0);
    static QnxConfigurationManager *m_instance;
    QList<QnxConfiguration*> m_configurations;
    Utils::PersistentSettingsWriter *m_writer;
    void restoreConfigurations();

    friend class QnxPlugin;
};

}
}

#endif // QNXCONFIGURATIONMANAGER_H