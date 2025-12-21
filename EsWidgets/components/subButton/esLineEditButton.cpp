/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLineEditButton.cpp
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "../subbutton/esLineEditButton.h"

#include <QPainter>

#include "esIcon.h"
#include "esStyleSheet.h"

#include "private/esLineEditButton_p.h"
// region ================= EsLineEditButtonPrivate  =================

EsLineEditButtonPrivate::EsLineEditButtonPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsLineEditButtonPrivate::onActionChanged()
{
    Q_Q(EsLineEditButton);
    if (action)
    {
        q->setIcon(action->icon());
        q->setToolTip(action->toolTip());
        q->setEnabled(action->isEnabled());
        q->setCheckable(action->isCheckable());
        q->setChecked(action->isChecked());
    }
}

// endregion

// region ================= EsLineEditButton  =================

EsLineEditButton::EsLineEditButton(const QVariant& icon, QWidget* parent)
    : QToolButton(parent), d_ptr(new EsLineEditButtonPrivate())
{
    Q_D(EsLineEditButton);
    d->q_ptr = this;
    d->icon = icon;
    d->action = nullptr;
    d->isPressed = false;
    setFixedSize(31, 23);
    setIconSize(QSize(10, 10));
    setCursor(Qt::PointingHandCursor);
    setObjectName("lineEditButton");
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_LINE_EDIT);
    styleSheet->apply(this);
}

EsLineEditButton::EsLineEditButton(const QString& icon, QWidget* parent)
    :EsLineEditButton(QVariant::fromValue(icon), parent)
{
}

EsLineEditButton::EsLineEditButton(const QIcon& icon, QWidget* parent)
    :EsLineEditButton(QVariant::fromValue(icon), parent)
{
}

EsLineEditButton::EsLineEditButton(Es::IconName icon, QWidget* parent)
    : EsLineEditButton(new EsIcon(icon), parent)
{
}

EsLineEditButton::EsLineEditButton(EsIconBase* icon, QWidget* parent)
    :EsLineEditButton(QVariant::fromValue(icon), parent)
{
}

EsLineEditButton::~EsLineEditButton()
{
    Q_D(EsLineEditButton);
    if (d->icon.userType() == qMetaTypeId<EsIconBase*>()) delete d->icon.value<EsIconBase*>();
}

void EsLineEditButton::setAction(QAction* action)
{
    Q_D(EsLineEditButton);

    d->action = action;
    d->onActionChanged();

    connect(this, &QToolButton::clicked, action, &QAction::trigger);
    connect(action, &QAction::toggled, this, &QToolButton::setChecked);
    connect(action, &QAction::changed, d, &EsLineEditButtonPrivate::onActionChanged);

    // installEventFilter(new ToolTipFilter(this, 700));  // todo ToolTipFilter should be defined elsewhere
}

QAction* EsLineEditButton::action() const
{
    Q_D(const EsLineEditButton);
    return d->action;
}

void EsLineEditButton::setIcon(const QVariant& icon)
{
    Q_D(EsLineEditButton);
    if (icon.userType() == qMetaTypeId<EsIconBase*>()) delete icon.value<EsIconBase*>();
    d->icon = icon;
    update();
}

void EsLineEditButton::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsLineEditButton);
    d->isPressed = true;
    QToolButton::mousePressEvent(e);
}

void EsLineEditButton::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsLineEditButton);
    d->isPressed = false;
    QToolButton::mouseReleaseEvent(e);
}

void EsLineEditButton::paintEvent(QPaintEvent* e)
{
    Q_D(EsLineEditButton);
    QToolButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int iw = iconSize().width();
    int ih = iconSize().height();
    int w = width();
    int h = height();

    QRectF rect((w - iw) / 2, (h - ih) / 2, iw, ih);

    if (d->isPressed)
    {
        painter.setOpacity(0.7);
    }

    if (EsFunc::isDarkTheme())
    {
        EsFunc::drawIcon(d->icon, &painter, rect);
    }
    else
    {
        EsFunc::drawIcon(d->icon, &painter, rect, QIcon::Off, {}, {{"fill", "#656565"}});
    }
}
