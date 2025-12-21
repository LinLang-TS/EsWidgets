/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIndicator.cpp
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esIndicator.h"

#include <QPainter>

#include "private/esIndicator_p.h"
#include <QPropertyAnimation>

#include "esConfig.h"
#include "esIcon.h"
#include "esStyleSheet.h"
// region ================= EsIndicatorPrivate =================

EsIndicatorPrivate::EsIndicatorPrivate(QObject* parent)
    : QObject(parent), sliderX(5.f)
{
}

void EsIndicatorPrivate::toggleSlider()
{
    Q_Q(EsIndicator);
    slideAni->setEndValue(q->isChecked() ? 25 : 5);
    slideAni->start();
}

void EsIndicatorPrivate::drawBackground(QPainter* painter)
{
    Q_Q(EsIndicator);
    // 获取按钮的高度的一半，用于圆角半径
    qreal r = q->height() / 2.0;

    // 设置边框颜色
    painter->setPen(borderColor());

    // 设置背景颜色
    painter->setBrush(backgroundColor());

    // 绘制圆角矩形，调整矩形的位置，避免画出边界
    painter->drawRoundedRect(q->rect().adjusted(1, 1, -1, -1), r, r);
}

void EsIndicatorPrivate::drawCircle(QPainter* painter)
{
    // 设置画笔为无边框
    painter->setPen(Qt::NoPen);

    // 设置圆形填充颜色
    painter->setBrush(sliderColor());

    // 绘制圆形，圆的圆心位置由 sliderX 确定，大小为 12x12
    painter->drawEllipse(static_cast<int>(this->sliderX), 5, 12, 12);
}

QColor EsIndicatorPrivate::backgroundColor()
{
    Q_Q(EsIndicator);
    bool isDark = EsFunc::isDarkTheme();

    // 如果当前是选中状态
    if (q->isChecked())
    {
        // 根据主题选择不同的颜色
        QColor color = isDark ? this->darkCheckedColor : this->lightCheckedColor;


        if (!q->isEnabled())
        {
            return isDark ? QColor(255, 255, 255, 41) : QColor(0, 0, 0, 56);
        }

        if (q->isPressed)
        {
            return EsFunc::validColor(color, EsThemeColor::getColor(Es::ThemeColorLight2));
        }

        else if (q->isHover)
        {
            return EsFunc::validColor(color, EsThemeColor::getColor(Es::ThemeColorLight1));
        }

        // 如果没有按下或悬停，返回默认的主题颜色
        return EsFunc::fallbackThemeColor(color);
    }
    // 如果当前不是选中状态
    else
    {
        if (!q->isEnabled())
        {
            return {0, 0, 0, 0};
        }

        if (q->isPressed)
        {
            return isDark ? QColor(255, 255, 255, 18) : QColor(0, 0, 0, 23);
        }
        else if (q->isHover)
        {
            return isDark ? QColor(255, 255, 255, 10) : QColor(0, 0, 0, 15);
        }

        // 否则返回透明色
        return {0, 0, 0, 0};
    }
}

QColor EsIndicatorPrivate::borderColor()
{
    Q_Q(EsIndicator);
    bool isDark = EsFunc::isDarkTheme();


    if (q->isChecked())
    {
        return q->isEnabled() ? backgroundColor() : QColor(0, 0, 0, 0);
    }
    else
    {
        if (q->isEnabled())
        {
            return isDark ? QColor(255, 255, 255, 153) : QColor(0, 0, 0, 133);
        }
        // 如果不可用，返回对应的禁用边框颜色
        return isDark ? QColor(255, 255, 255, 41) : QColor(0, 0, 0, 56);
    }
}

QColor EsIndicatorPrivate::sliderColor()
{
    Q_Q(EsIndicator);
    bool isDark = EsFunc::isDarkTheme();

    if (q->isChecked())
    {

        if (q->isEnabled())
        {
            return isDark ? QColor(Qt::black) : QColor(Qt::white);
        }

        return isDark ? QColor(255, 255, 255, 77) : QColor(255, 255, 255);
    }
    else
    {
        if (q->isEnabled())
        {
            return isDark ? QColor(255, 255, 255, 201) : QColor(0, 0, 0, 156);
        }
        return isDark ? QColor(255, 255, 255, 96) : QColor(0, 0, 0, 91);
    }
}

// endregion

// region ================= EsIndicator =================
EsIndicator::EsIndicator(QWidget* parent)
    : EsToolButton(parent), d_ptr(new EsIndicatorPrivate())
{
    Q_D(EsIndicator);
    d->q_ptr = this;
    setCheckable(true);
    setFixedSize(42, 22);
    d->slideAni = new QPropertyAnimation(this, "sliderX", this);
    d->slideAni->setDuration(120);
    connect(this, &EsToolButton::toggled, d, &EsIndicatorPrivate::toggleSlider);
}

EsIndicator::~EsIndicator()
{
}

void EsIndicator::toggle()
{
    setChecked(!isChecked());
}

void EsIndicator::setDown(bool isDown)
{
    isPressed = isDown;
    EsToolButton::setDown(isDown);
}

void EsIndicator::setHover(bool isHover)
{
    this->isHover = isHover;
    update();
}

void EsIndicator::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsIndicator);
    d->lightCheckedColor = light;
    d->darkCheckedColor = dark;
    update();
}

float EsIndicator::getSliderX() const
{
    Q_D(const EsIndicator);
    return d->sliderX;
}

void EsIndicator::setSliderX(float x)
{
    Q_D(EsIndicator);
    d->sliderX = qMax(x, 5.f);
    update();
}

void EsIndicator::mouseReleaseEvent(QMouseEvent* e)
{
    EsToolButton::mouseReleaseEvent(e);
    emit checkedChanged(isChecked());
}

void EsIndicator::paintEvent(QPaintEvent* e)
{
    Q_D(EsIndicator);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    d->drawBackground(&painter);
    d->drawCircle(&painter);
}

// endregion
