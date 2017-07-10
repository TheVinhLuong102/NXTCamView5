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

#pragma once

#include "vcsbase_global.h"

#include <QVariant>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QToolButton;
class QStringList;
QT_END_NAMESPACE

namespace VcsBase {

namespace Internal { class VcsBaseEditorParameterWidgetPrivate; }

// Documentation->inside.
class VCSBASE_EXPORT VcsBaseEditorParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VcsBaseEditorParameterWidget(QWidget *parent = 0);
    ~VcsBaseEditorParameterWidget() override;

    class VCSBASE_EXPORT ComboBoxItem
    {
    public:
        ComboBoxItem() = default;
        ComboBoxItem(const QString &text, const QVariant &val);
        QString displayText;
        QVariant value;
    };

    QStringList baseArguments() const;
    void setBaseArguments(const QStringList &);

    QToolButton *addToggleButton(const QString &option, const QString &label,
                                 const QString &tooltip = QString());
    QToolButton *addToggleButton(const QStringList &options, const QString &label,
                                 const QString &tooltip = QString());
    QComboBox *addComboBox(const QStringList &options, const QList<ComboBoxItem> &items);

    void mapSetting(QToolButton *button, bool *setting);
    void mapSetting(QComboBox *comboBox, QString *setting);
    void mapSetting(QComboBox *comboBox, int *setting);

    // Return the effective arguments according to setting.
    virtual QStringList arguments() const;

public slots:
    void handleArgumentsChanged();
    void executeCommand();

signals:
    void commandExecutionRequested();

    // Trigger a re-run to show changed output according to new argument list.
    void argumentsChanged();

protected:
    class OptionMapping
    {
    public:
        OptionMapping() = default;
        OptionMapping(const QString &option, QWidget *w);
        OptionMapping(const QStringList &optionList, QWidget *w);
        QStringList options;
        QWidget *widget = nullptr;
    };

    const QList<OptionMapping> &optionMappings() const;
    virtual QStringList argumentsForOption(const OptionMapping &mapping) const;
    void updateMappedSettings();

private:
    friend class Internal::VcsBaseEditorParameterWidgetPrivate;
    Internal::VcsBaseEditorParameterWidgetPrivate *const d;
};

} // namespace VcsBase
