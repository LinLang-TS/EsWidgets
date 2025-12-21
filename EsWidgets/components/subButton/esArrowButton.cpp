/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esArrowButton.cpp
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esArrowButton.h"

#include <QPainter>

#include "esConfig.h"
#include "esIcon.h"
#include "private/esArrowButton_p.h"

EsArrowButtonPrivate::EsArrowButtonPrivate(QObject* parent)
: QObject(parent)
{
}

EsArrowButton::EsArrowButton(Es::IconName icon, QWidget* parent)
    : QToolButton(parent), d_ptr(new EsArrowButtonPrivate())
{
    Q_D(EsArrowButton);
    d->q_ptr = this;

    d->icon = new EsIcon(icon);
    opacity = 1.0;

    setFixedSize(10, 10);
    lightColor = QColor(0, 0, 0, 114); // 浅色模式下的箭头颜色
    darkColor = QColor(255, 255, 255, 139); // 深色模式下的箭头颜色
}

EsArrowButton::~EsArrowButton()
{
    Q_D(EsArrowButton);
    delete d->icon;
}

void EsArrowButton::setOpacity(qreal opacity)
{
    this->opacity = opacity;
    update();
}

void EsArrowButton::setLightColor(const QColor& color)
{
    lightColor = color;
    update();
}

void EsArrowButton::setDarkColor(const QColor& color)
{
    darkColor = color;
    update();
}

void EsArrowButton::paintEvent(QPaintEvent* event)
{
    Q_D(EsArrowButton);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    // 根据主题选择颜色
    QColor color = EsFunc::isDarkTheme() ? darkColor : lightColor;

    // 设置不透明度; 避雷: 用alphaF会导致透明底太低不显示
    painter.setOpacity(opacity * color.alpha() / 255.0);

    // 按按钮按下状态调整图标大小
    int s = isDown() ? 7 : 8;
    qreal x = (width() - s) / 2.0;

    // 绘制 EsIcon
    d->icon->render(&painter, QRectF(x, x, s, s), {{"fill", color.name()}});
}
