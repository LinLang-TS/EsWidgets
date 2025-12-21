/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esToolButton.cpp
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esToolButton.h"

#include <QPainter>

#include "esFont.h"
#include "esStyleSheet.h"
#include "esIcon.h"
#include "private/esToolButton_p.h"

// region ================= EsPushButton  =================

EsToolButtonPrivate::EsToolButtonPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsToolButtonPrivate::setIcon(const QVariant& variant)
{
    Q_Q(EsToolButton);
    if (icon.userType() == qMetaTypeId<EsIconBase*>()) delete icon.value<EsIconBase*>();
    icon = variant;
    q->update();
}

// endregion


// region ================= EsPushButton  =================

EsToolButton::EsToolButton(QWidget* parent)
    : QToolButton(parent),
      isPressed(false),
      isHover(false),
      d_ptr(new EsToolButtonPrivate())
{
    Q_D(EsToolButton);
    d->q_ptr = this;

    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_BUTTON);
    styleSheet->apply(this);
    setIconSize(QSize(16, 16));
    setIcon(QIcon());
    EsFunc::setFont(this);
}

EsToolButton::EsToolButton(Es::IconName icon, QWidget* parent)
    : EsToolButton(parent)
{
    setIcon(icon);
}

EsToolButton::EsToolButton(EsIconBase* icon, QWidget* parent)
    : EsToolButton(parent)
{
    setIcon(icon);
}

EsToolButton::EsToolButton(const QString& icon, QWidget* parent)
    : EsToolButton(parent)
{
    setIcon(icon);
}

EsToolButton::EsToolButton(const QIcon& icon, QWidget* parent)
    : EsToolButton(parent)
{
    setIcon(icon);
}

EsToolButton::~EsToolButton()
{
    Q_D(EsToolButton);
    if (d->icon.userType() == qMetaTypeId< EsIconBase*>()) delete d->icon.value< EsIconBase*>();
}

void EsToolButton::setIcon(const QIcon& icon)
{
    Q_D(EsToolButton);
    d->setIcon(QVariant::fromValue(icon));
}

void EsToolButton::setIcon(const QString& icon)
{
    Q_D(EsToolButton);
    d->setIcon(QVariant::fromValue(icon));
}

void EsToolButton::setIcon(Es::IconName icon)
{
    setIcon(new EsIcon(icon));
}

void EsToolButton::setIcon(EsIconBase* icon)
{
    Q_D(EsToolButton);
    d->setIcon(QVariant::fromValue(icon));
}

QIcon EsToolButton::icon()
{
    Q_D(EsToolButton);
    return EsFunc::toQIcon(d->icon);
}


bool EsToolButton::setProperty(const char* name, const QVariant& value)
{
    if (strcmp(name, "icon") != 0)
    {
        // 如果属性名不是 "icon"，调用父类的 setProperty
        return QToolButton::setProperty(name, value);
    }

    // 如果是 "icon" 属性，调用 setIcon 方法
    setIcon(value.toString());
    return true;
}


void EsToolButton::_drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state)
{
    EsFunc::drawIcon(icon, painter, rect, state);
}

void EsToolButton::mousePressEvent(QMouseEvent* e)
{
    isPressed = true;
    QToolButton::mousePressEvent(e);
}

void EsToolButton::mouseReleaseEvent(QMouseEvent* e)
{
    isPressed = false;
    QToolButton::mouseReleaseEvent(e);
}

void EsToolButton::enterEvent(QEvent* event)
{
    isHover = true;
    update();
}

void EsToolButton::leaveEvent(QEvent* event)
{
    isHover = false;
    update();
}

void EsToolButton::paintEvent(QPaintEvent* event)
{
    Q_D(EsToolButton);
    QToolButton::paintEvent(event);

    // 如果图标为空，则不进行绘制
    if (d->icon.isNull() || !d->icon.isValid()) return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);  // 启用反锯齿和光滑变换

    // 设置透明度
    if (!isEnabled()) {
        painter.setOpacity(0.43);  // 禁用时设置透明度为 0.43
    } else if (isPressed) {
        painter.setOpacity(0.63);  // 按下时设置透明度为 0.63
    }

    // 获取图标的宽高
    qreal w = iconSize().width();
    qreal h = iconSize().height();

    // 计算图标的位置，居中显示
    qreal y = (height() - h) / 2;
    qreal x = (width() - w) / 2;

    // 绘制图标
    _drawIcon(d->icon, &painter, QRectF(x, y, w, h));
}
// endregion
