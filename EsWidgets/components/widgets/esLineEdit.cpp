/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLineEdit.cpp
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esLineEdit.h"

#include <QHBoxLayout>
#include <QAction>
#include <QPainter>
#include <QTimer>
#include <QMenu>
#include <QPainterPath>

#include "esColor.h"
#include "esCompleterMenu.h"
#include "esFont.h"
#include "esIcon.h"
#include "esStyleSheet.h"
#include "../subButton/esLineEditButton.h"
#include "private/esLinedit_p.h"
// region ================= EsLineEditPrivate =================

EsLineEditPrivate::EsLineEditPrivate(QObject* parent)
    :QObject(parent)
{
}

void EsLineEditPrivate::adjustTextMargins()
{
    Q_Q(EsLineEdit);
    int left = leftButtons.size() * 30;
    int right = rightButtons.size() * 30 + 28 * isClearButtonEnabled;
    QMargins m = q->textMargins();
    q->setTextMargins(left, m.top(), right, m.bottom());
}

void EsLineEditPrivate::onTextChanged(const QString& text)
{
    Q_Q(EsLineEdit);
    if (isClearButtonEnabled)
    {
        q->clearButton->setVisible(!text.isEmpty() && q->hasFocus());
    }
}

void EsLineEditPrivate::onTextEdited(const QString& text)
{
    Q_Q(EsLineEdit);
    if (!completer) return;

    if (!text.isEmpty())
    {
        QTimer::singleShot(50, this, &EsLineEditPrivate::showCompleterMenu);
    }
    else if (completerMenu)
    {
        completerMenu->close();
    }
}

void EsLineEditPrivate::showCompleterMenu()
{
    Q_Q(EsLineEdit);
    if (!completer || q->text().isEmpty()) {
        return;
    }

    // 创建菜单
    if (!completerMenu) {
        q->setCompleterMenu(new EsCompleterMenu(q));
    }

    // 设置菜单项
    completer->setCompletionPrefix(q->text());
    bool changed = completerMenu->setCompletion(completer->completionModel(), completer->completionColumn());
    completerMenu->setMaxVisibleItems(completer->maxVisibleItems());

    // 显示菜单
    if (changed) {
        completerMenu->popup();
    }
}

// endregion

// region ================= EsLineEdit =================

EsLineEdit::EsLineEdit(QWidget* parent)
    : QLineEdit(parent), d_ptr(new EsLineEditPrivate())
{
    Q_D(EsLineEdit);
    d->q_ptr = this;
    d->isClearButtonEnabled = false;
    d->completer = nullptr;
    d->completerMenu = nullptr;
    d->isError = false;


    setProperty("transparent", true);
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_LINE_EDIT);
    styleSheet->apply(this);

    setFixedHeight(33);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    EsFunc::setFont(this);

    hBoxLayout = new QHBoxLayout(this);
    clearButton = new EsLineEditButton(Es::Icon_Close, this);
    clearButton->setFixedSize(29, 25);
    clearButton->hide();

    hBoxLayout->setSpacing(3);
    hBoxLayout->setContentsMargins(4, 4, 4, 4);
    hBoxLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hBoxLayout->addWidget(clearButton, 0, Qt::AlignRight);

    connect(clearButton, &EsLineEditButton::clicked, this, &EsLineEdit::clear);
    connect(this, &QLineEdit::textChanged, d, &EsLineEditPrivate::onTextChanged);
    connect(this, &QLineEdit::textEdited, d, &EsLineEditPrivate::onTextEdited);
}

EsLineEdit::~EsLineEdit()
{
}

bool EsLineEdit::isError() const
{
    Q_D(const EsLineEdit);
    return d->isError;
}

void EsLineEdit::setError(bool isError)
{
    Q_D(EsLineEdit);
    if (isError == d->isError) return;

    d->isError = isError;
    update();
}

void EsLineEdit::setCustomFocusedBorderColor(const QColor& light, const QColor& dark)
{
    Q_D(EsLineEdit);
    d->lightFocusedBorderColor = light;
    d->darkFocusedBorderColor = dark;
    update();
}

QColor EsLineEdit::focusedBorderColor() const
{
    Q_D(const EsLineEdit);
    if (isError())
    {
        return EsSystemColor::color(EsSystemColor::CRITICAL_FOREGROUND); // 错误时使用警告颜色
    }
    return EsFunc::autoFallbackThemeColor(d->lightFocusedBorderColor, d->darkFocusedBorderColor); // 根据主题颜色返回合适的颜色
}

void EsLineEdit::setClearButtonEnabled(bool enable)
{
    Q_D(EsLineEdit);
    d->isClearButtonEnabled = enable;
    d->adjustTextMargins();
}

bool EsLineEdit::isClearButtonEnabled() const
{
    Q_D(const EsLineEdit);
    return d->isClearButtonEnabled;
}

void EsLineEdit::setCompleter(QCompleter* completer)
{
    Q_D(EsLineEdit);
    d->completer = completer;
}

QCompleter* EsLineEdit::completer() const
{
    Q_D(const EsLineEdit);
    return d->completer;
}

void EsLineEdit::addAction(QAction* action, QLineEdit::ActionPosition position)
{
    Q_D(EsLineEdit);
    QWidget::addAction(action);

    auto button = new EsLineEditButton(action->icon());
    button->setAction(action);
    button->setFixedWidth(29);

    if (position == QLineEdit::LeadingPosition)
    {
        hBoxLayout->insertWidget(d->leftButtons.size(), button, 0, Qt::AlignLeading);
        if (d->leftButtons.empty())
        {
            hBoxLayout->insertStretch(1, 1);
        }
        d->leftButtons.append(button);
    }
    else
    {
        d->rightButtons.append(button);
        hBoxLayout->addWidget(button, 0, Qt::AlignRight);
    }

    d->adjustTextMargins();
}

void EsLineEdit::addActions(const QList<QAction*>& actions, QLineEdit::ActionPosition position)
{
    Q_D(EsLineEdit);
    for (QAction* action : actions)
    {
        addAction(action, position);
    }
}

void EsLineEdit::setCompleterMenu(EsCompleterMenu* menu)
{
    Q_D(EsLineEdit);
    connect(menu, &EsCompleterMenu::activated, d->completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::activated));
    connect(menu, &EsCompleterMenu::indexActivated, this, [=](const QModelIndex& idx) {
        emit d->completer->activated(idx);
    });
    d->completerMenu = menu;
}


void EsLineEdit::focusOutEvent(QFocusEvent* e)
{
    QLineEdit::focusOutEvent(e);
    clearButton->hide();
}

void EsLineEdit::focusInEvent(QFocusEvent* e)
{
    QLineEdit::focusInEvent(e);
    if (isClearButtonEnabled())
    {
        clearButton->setVisible(!text().isEmpty());
    }
}

// todo
// void EsLineEdit::contextMenuEvent(QContextMenuEvent* e)
// {
// QMenu menu(this);
// LineEditMenu* lineEditMenu = new LineEditMenu(this);  // 假设 LineEditMenu 类已经定义
// lineEditMenu->exec(e->globalPos());
// }

void EsLineEdit::paintEvent(QPaintEvent* e)
{
    QLineEdit::paintEvent(e);

    if (!hasFocus())
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QMargins margins = contentsMargins();
    QPainterPath path;
    int w = width() - margins.left() - margins.right();
    int h = height();

    // 绘制圆角矩形路径
    path.addRoundedRect(QRectF(margins.left(), h - 10, w, 10), 5, 5);

    // 绘制矩形路径
    QPainterPath rectPath;
    rectPath.addRect(margins.left(), h - 10, w, 8);

    // 从圆角矩形路径中减去矩形路径
    path = path.subtracted(rectPath);

    // 填充路径颜色
    painter.fillPath(path, focusedBorderColor());
}


// endregion
