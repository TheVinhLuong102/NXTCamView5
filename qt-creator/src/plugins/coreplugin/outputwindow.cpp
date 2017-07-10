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

#include "outputwindow.h"

#include "actionmanager/actionmanager.h"
#include "coreconstants.h"
#include "icore.h"

#include <utils/outputformatter.h>
#include <utils/synchronousprocess.h>
//OPENMV-DIFF//
#include <utils/fadingindicator.h>
//OPENMV-DIFF//

#include <QAction>
#include <QScrollBar>
//OPENMV-DIFF//
#include <QApplication>
#include <QRegularExpression>
//OPENMV-DIFF//

using namespace Utils;

namespace Core {

namespace Internal {

class OutputWindowPrivate
{
public:
    OutputWindowPrivate(QTextDocument *document)
        : outputWindowContext(0)
        , formatter(0)
        , enforceNewline(false)
        , scrollToBottom(false)
        , linksActive(true)
        , mousePressed(false)
        , m_zoomEnabled(false)
        , m_originalFontSize(0)
        , maxLineCount(100000)
        , cursor(document)
    {
    }

    ~OutputWindowPrivate()
    {
        ICore::removeContextObject(outputWindowContext);
        delete outputWindowContext;
    }

    IContext *outputWindowContext;
    Utils::OutputFormatter *formatter;

    bool enforceNewline;
    bool scrollToBottom;
    bool linksActive;
    bool mousePressed;
    bool m_zoomEnabled;
    float m_originalFontSize;
    int maxLineCount;
    QTextCursor cursor;
};

} // namespace Internal

/*******************/

OutputWindow::OutputWindow(Context context, QWidget *parent)
    : QPlainTextEdit(parent)
    , d(new Internal::OutputWindowPrivate(document()))
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //setCenterOnScroll(false);
    setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    setUndoRedoEnabled(false);

    d->outputWindowContext = new IContext;
    d->outputWindowContext->setContext(context);
    d->outputWindowContext->setWidget(this);
    ICore::addContextObject(d->outputWindowContext);

    QAction *undoAction = new QAction(this);
    QAction *redoAction = new QAction(this);
    QAction *cutAction = new QAction(this);
    QAction *copyAction = new QAction(this);
    QAction *pasteAction = new QAction(this);
    QAction *selectAllAction = new QAction(this);

    ActionManager::registerAction(undoAction, Constants::UNDO, context);
    ActionManager::registerAction(redoAction, Constants::REDO, context);
    ActionManager::registerAction(cutAction, Constants::CUT, context);
    ActionManager::registerAction(copyAction, Constants::COPY, context);
    ActionManager::registerAction(pasteAction, Constants::PASTE, context);
    ActionManager::registerAction(selectAllAction, Constants::SELECTALL, context);

    connect(undoAction, &QAction::triggered, this, &QPlainTextEdit::undo);
    connect(redoAction, &QAction::triggered, this, &QPlainTextEdit::redo);
    connect(cutAction, &QAction::triggered, this, &QPlainTextEdit::cut);
    connect(copyAction, &QAction::triggered, this, &QPlainTextEdit::copy);
    connect(pasteAction, &QAction::triggered, this, &QPlainTextEdit::paste);
    connect(selectAllAction, &QAction::triggered, this, &QPlainTextEdit::selectAll);

    connect(this, &QPlainTextEdit::undoAvailable, undoAction, &QAction::setEnabled);
    connect(this, &QPlainTextEdit::redoAvailable, redoAction, &QAction::setEnabled);
    connect(this, &QPlainTextEdit::copyAvailable, cutAction, &QAction::setEnabled);  // OutputWindow never read-only
    connect(this, &QPlainTextEdit::copyAvailable, copyAction, &QAction::setEnabled);

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    cutAction->setEnabled(false);
    copyAction->setEnabled(false);

    m_scrollTimer.setInterval(10);
    m_scrollTimer.setSingleShot(true);
    connect(&m_scrollTimer, &QTimer::timeout,
            this, &OutputWindow::scrollToBottom);
    m_lastMessage.start();

    d->m_originalFontSize = font().pointSizeF();
}

OutputWindow::~OutputWindow()
{
    delete d;
}

void OutputWindow::mousePressEvent(QMouseEvent * e)
{
    d->mousePressed = true;
    QPlainTextEdit::mousePressEvent(e);
}

void OutputWindow::mouseReleaseEvent(QMouseEvent *e)
{
    d->mousePressed = false;

    if (d->linksActive) {
        const QString href = anchorAt(e->pos());
        if (d->formatter)
            d->formatter->handleLink(href);
    }

    // Mouse was released, activate links again
    d->linksActive = true;

    QPlainTextEdit::mouseReleaseEvent(e);
}

void OutputWindow::mouseMoveEvent(QMouseEvent *e)
{
    // Cursor was dragged to make a selection, deactivate links
    if (d->mousePressed && textCursor().hasSelection())
        d->linksActive = false;

    if (!d->linksActive || anchorAt(e->pos()).isEmpty())
        viewport()->setCursor(Qt::IBeamCursor);
    else
        viewport()->setCursor(Qt::PointingHandCursor);
    QPlainTextEdit::mouseMoveEvent(e);
}

void OutputWindow::resizeEvent(QResizeEvent *e)
{
    //Keep scrollbar at bottom of window while resizing, to ensure we keep scrolling
    //This can happen if window is resized while building, or if the horizontal scrollbar appears
    bool atBottom = isScrollbarAtBottom();
    QPlainTextEdit::resizeEvent(e);
    if (atBottom)
        scrollToBottom();
}

void OutputWindow::keyPressEvent(QKeyEvent *ev)
{
    QPlainTextEdit::keyPressEvent(ev);

    //Ensure we scroll also on Ctrl+Home or Ctrl+End
    if (ev->matches(QKeySequence::MoveToStartOfDocument))
        verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMinimum);
    else if (ev->matches(QKeySequence::MoveToEndOfDocument))
        verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
}

OutputFormatter *OutputWindow::formatter() const
{
    return d->formatter;
}

void OutputWindow::setFormatter(OutputFormatter *formatter)
{
    d->formatter = formatter;
    d->formatter->setPlainTextEdit(this);
}

void OutputWindow::showEvent(QShowEvent *e)
{
    QPlainTextEdit::showEvent(e);
    if (d->scrollToBottom)
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    d->scrollToBottom = false;
}

void OutputWindow::wheelEvent(QWheelEvent *e)
{
    if (d->m_zoomEnabled) {
        if (e->modifiers() & Qt::ControlModifier) {
            float delta = e->angleDelta().y() / 120.f;
            zoomInF(delta);
            emit wheelZoom();
            //OPENMV-DIFF//
            Utils::FadingIndicator::showText(this, tr("Zoom: %1%").arg(int(100 * (font().pointSizeF() / d->m_originalFontSize))), Utils::FadingIndicator::SmallText);
            //OPENMV-DIFF//
            return;
        }
    }
    QAbstractScrollArea::wheelEvent(e);
    updateMicroFocus();
}

void OutputWindow::setBaseFont(const QFont &newFont)
{
    float zoom = fontZoom();
    d->m_originalFontSize = newFont.pointSizeF();
    QFont tmp = newFont;
    float newZoom = qMax(d->m_originalFontSize + zoom, 4.0f);
    tmp.setPointSizeF(newZoom);
    setFont(tmp);
}

float OutputWindow::fontZoom() const
{
    return font().pointSizeF() - d->m_originalFontSize;
}

void OutputWindow::setFontZoom(float zoom)
{
    QFont f = font();
    if (f.pointSizeF() == d->m_originalFontSize + zoom)
        return;
    float newZoom = qMax(d->m_originalFontSize + zoom, 4.0f);
    f.setPointSizeF(newZoom);
    setFont(f);
}

void OutputWindow::setWheelZoomEnabled(bool enabled)
{
    d->m_zoomEnabled = enabled;
}

QString OutputWindow::doNewlineEnforcement(const QString &out)
{
    d->scrollToBottom = true;
    QString s = out;
    if (d->enforceNewline) {
        s.prepend(QLatin1Char('\n'));
        d->enforceNewline = false;
    }

    if (s.endsWith(QLatin1Char('\n'))) {
        d->enforceNewline = true; // make appendOutputInline put in a newline next time
        s.chop(1);
    }

    return s;
}

void OutputWindow::setMaxLineCount(int count)
{
    d->maxLineCount = count;
    setMaximumBlockCount(d->maxLineCount);
}

int OutputWindow::maxLineCount() const
{
    return d->maxLineCount;
}

void OutputWindow::appendMessage(const QString &output, OutputFormat format)
{
    const QString out = SynchronousProcess::normalizeNewlines(output);
    setMaximumBlockCount(d->maxLineCount);
    const bool atBottom = isScrollbarAtBottom() || m_scrollTimer.isActive();

    if (format == ErrorMessageFormat || format == NormalMessageFormat) {

        d->formatter->appendMessage(doNewlineEnforcement(out), format);

    } else {

        bool sameLine = format == StdOutFormatSameLine
                     || format == StdErrFormatSameLine;

        if (sameLine) {
            d->scrollToBottom = true;

            int newline = -1;
            bool enforceNewline = d->enforceNewline;
            d->enforceNewline = false;

            if (!enforceNewline) {
                newline = out.indexOf(QLatin1Char('\n'));
                moveCursor(QTextCursor::End);
                if (newline != -1)
                    d->formatter->appendMessage(out.left(newline), format);// doesn't enforce new paragraph like appendPlainText
            }

            QString s = out.mid(newline+1);
            if (s.isEmpty()) {
                d->enforceNewline = true;
            } else {
                if (s.endsWith(QLatin1Char('\n'))) {
                    d->enforceNewline = true;
                    s.chop(1);
                }
                d->formatter->appendMessage(QLatin1Char('\n') + s, format);
            }
        } else {
            d->formatter->appendMessage(doNewlineEnforcement(out), format);
        }
    }

    if (atBottom) {
        if (m_lastMessage.elapsed() < 5) {
            m_scrollTimer.start();
        } else {
            m_scrollTimer.stop();
            scrollToBottom();
        }
    }

    m_lastMessage.start();
    enableUndoRedo();
}

// TODO rename
void OutputWindow::appendText(const QString &textIn, const QTextCharFormat &format)
{
    //OPENMV-DIFF//
    //const QString text = SynchronousProcess::normalizeNewlines(textIn);
    //if (d->maxLineCount > 0 && document()->blockCount() >= d->maxLineCount)
    //    return;
    //OPENMV-DIFF//
    QString text = SynchronousProcess::normalizeNewlines(textIn);
    int index = text.indexOf(QStringLiteral("Traceback (most recent call last):\n"));
    if(index != -1)
    {
        appendText(text.left(index));
        grayOutOldContent();
        text = text.mid(index);
    }
    //OPENMV-DIFF//
    //OPENMV-DIFF//

    QChar lastChar = QChar();

    QString string;
    int column = 0;

    for(int i = 0, j = text.size(); i < j; i++)
    {
        switch(text.at(i).unicode())
        {
            case 15:
            case 17: // XON
            case 19: // XOFF
            case 23:
            case 24:
            case 25:
            case 26:
            case 27: // Escape - AnsiEscapeCodeHandler
            case 28:
            case 29:
            case 30:
            case 31:
            {
                break;
            }

            case 0: // Null
            {
                break;
            }

            case 1: // Home Cursor
            {
                break;
            }

            case 2: // Move Cursor Left
            {
                break;
            }

            case 3: // Clear Screen
            {
                break;
            }

            case 4:
            case 127: // Delete
            {
                break;
            }

            case 5: // End Cursor
            {
                break;
            }

            case 6: // Move Cursor Right
            {
                break;
            }

            case 7: // Beep Speaker
            {
                QApplication::beep();
                break;
            }

            case 8: // Backspace
            {
                break;
            }

            case 9: // Tab
            {
                for(int k = 8 - (column % 8); k > 0; k--)
                {
                    string.append(text.at(i));
                    column += 1;
                }

                break;
            }

            case 10: // Line Feed
            {
                if(lastChar.unicode() != '\r')
                {
                    string.append(QLatin1Char('\n'));
                    column = 0;
                }

                break;
            }

            case 11: // Clear to end of line.
            {
                break;
            }

            case 12: // Clear lines below.
            {
                break;
            }

            case 13: // Carriage Return
            {
                string.append(QLatin1Char('\n'));
                column = 0;
                break;
            }

            case 14: // Move Cursor Down
            {
                break;
            }

            case 16: // Move Cursor Up
            {
                break;
            }

            case 18: // Move to start of line.
            {
                break;
            }

            case 20: // Move to end of line.
            {
                break;
            }

            case 21: // Clear to start of line.
            {
                break;
            }

            case 22: // Clear lines above.
            {
                break;
            }

            default:
            {
                string.append(text.at(i));
                column += 1;
                break;
            }
        }

        lastChar = text.at(i);
    }

    if(d->cursor.document()->isEmpty())
    {
        string.remove(QRegularExpression(QStringLiteral("^\\s+")));
    }

    text = string;

    //OPENMV-DIFF//
    const bool atBottom = isScrollbarAtBottom();
    if (!d->cursor.atEnd())
        d->cursor.movePosition(QTextCursor::End);
    d->cursor.beginEditBlock();
    d->cursor.insertText(doNewlineEnforcement(text), format);

    //OPENMV-DIFF//
    //if (d->maxLineCount > 0 && document()->blockCount() >= d->maxLineCount) {
    //    QTextCharFormat tmp;
    //    tmp.setFontWeight(QFont::Bold);
    //    d->cursor.insertText(doNewlineEnforcement(tr("Additional output omitted") + QLatin1Char('\n')), tmp);
    //}
    //OPENMV-DIFF//

    d->cursor.endEditBlock();
    if (atBottom)
        scrollToBottom();
}

bool OutputWindow::isScrollbarAtBottom() const
{
    return verticalScrollBar()->value() == verticalScrollBar()->maximum();
}

void OutputWindow::clear()
{
    d->enforceNewline = false;
    QPlainTextEdit::clear();
}

void OutputWindow::scrollToBottom()
{
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    // QPlainTextEdit destroys the first calls value in case of multiline
    // text, so make sure that the scroll bar actually gets the value set.
    // Is a noop if the first call succeeded.
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void OutputWindow::grayOutOldContent()
{
    //OPENMV-DIFF//
    if(document()->isEmpty())
        return;
    d->enforceNewline = true;
    const bool atBottom = isScrollbarAtBottom();
    //OPENMV-DIFF//
    if (!d->cursor.atEnd())
        d->cursor.movePosition(QTextCursor::End);
    QTextCharFormat endFormat = d->cursor.charFormat();

    d->cursor.select(QTextCursor::Document);

    QTextCharFormat format;
    const QColor bkgColor = palette().base().color();
    const QColor fgdColor = palette().text().color();
    double bkgFactor = 0.50;
    double fgdFactor = 1.-bkgFactor;
    format.setForeground(QColor((bkgFactor * bkgColor.red() + fgdFactor * fgdColor.red()),
                             (bkgFactor * bkgColor.green() + fgdFactor * fgdColor.green()),
                             (bkgFactor * bkgColor.blue() + fgdFactor * fgdColor.blue()) ));
    d->cursor.mergeCharFormat(format);

    d->cursor.movePosition(QTextCursor::End);
    d->cursor.setCharFormat(endFormat);
    d->cursor.insertBlock(QTextBlockFormat());
    //OPENMV-DIFF//
    if (atBottom)
        scrollToBottom();
    //OPENMV-DIFF//
}

void OutputWindow::enableUndoRedo()
{
    setMaximumBlockCount(0);
    setUndoRedoEnabled(true);
}

void OutputWindow::setWordWrapEnabled(bool wrap)
{
    if (wrap)
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    else
        setWordWrapMode(QTextOption::NoWrap);
}

} // namespace Core
