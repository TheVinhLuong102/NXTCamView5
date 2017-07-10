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

#ifndef CUSTOMWIDGETWIZARDDIALOG_H
#define CUSTOMWIDGETWIZARDDIALOG_H

#include "../wizards/qtwizard.h"

#include <QSharedPointer>

namespace QmakeProjectManager {
namespace Internal {

class CustomWidgetWidgetsWizardPage;
class CustomWidgetPluginWizardPage;
struct PluginOptions;
struct FileNamingParameters;

class CustomWidgetWizardDialog : public BaseQmakeProjectWizardDialog
{
    Q_OBJECT
public:
    explicit CustomWidgetWizardDialog(const Core::BaseFileWizardFactory *factory,
                                      const QString &templateName, const QIcon &icon,
                                      QWidget *parent,
                                      const Core::WizardDialogParameters &parameters);

    QSharedPointer<PluginOptions> pluginOptions() const;


    FileNamingParameters fileNamingParameters() const;
    void setFileNamingParameters(const FileNamingParameters &fnp);

private:
    void slotCurrentIdChanged(int id);

    CustomWidgetWidgetsWizardPage *m_widgetsPage;
    CustomWidgetPluginWizardPage *m_pluginPage;
    int m_pluginPageId;
};

} // namespace Internal
} // namespace QmakeProjectManager

#endif // CUSTOMWIDGETWIZARDDIALOG_H
