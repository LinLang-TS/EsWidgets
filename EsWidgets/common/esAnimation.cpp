/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esAnimation.cpp
** @date 2025/11/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esAnimation.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QEvent>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QLineEdit>

// region ================= AnimationBase  =================
EsAnimationBase::EsAnimationBase(QWidget* parent) : QObject(parent)
{
    parent->installEventFilter(this);
}

bool EsAnimationBase::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == parent())
    {
        switch (e->type())
        {
        case QEvent::MouseButtonPress:
            onPress(dynamic_cast<QMouseEvent*>(e));
            break;

        case QEvent::MouseButtonRelease:
            onRelease(dynamic_cast<QMouseEvent*>(e));
            break;

        case QEvent::Enter:
            onHover(dynamic_cast<QEnterEvent*>(e));
            break;

        case QEvent::Leave:
            onLeave(e);
            break;

        default:
            break;
        }
    }

    return QObject::eventFilter(obj, e);
}


// endregion


// region ================= TranslateYAnimation  =================

EsTranslateYAnimation::EsTranslateYAnimation(QWidget* parent, float offset)
    : EsAnimationBase(parent),
      y(0.0f),
      maxOffset(offset)
{
    ani = new QPropertyAnimation(this, "y", this);
}

float EsTranslateYAnimation::getY() const
{
    return y;
}

void EsTranslateYAnimation::setY(float value)
{
    if (qFuzzyCompare(y, value))
        return;

    y = value;
    auto p = (QWidget*)parent();
    p->update();
    emit valueChanged(value);
}

void EsTranslateYAnimation::onPress(QMouseEvent* e)
{
    Q_UNUSED(e);
    ani->stop();
    ani->setEndValue(maxOffset);
    ani->setDuration(150);
    ani->setEasingCurve(QEasingCurve::OutQuad);
    ani->start();
}

void EsTranslateYAnimation::onRelease(QMouseEvent* e)
{
    Q_UNUSED(e);
    ani->stop();
    ani->setEndValue(0.0f);
    ani->setDuration(500);
    ani->setEasingCurve(QEasingCurve::OutElastic);
    ani->start();
}
// endregion

// region ================= EsBackgroundAnimationWidget  =================
EsBackgroundAnimationWidget::EsBackgroundAnimationWidget(QWidget* parent)
    : EsWidget(parent)
{
    bgColorObject = new EsBackgroundColorObject(this);
    backgroundColorAni = new QPropertyAnimation(bgColorObject, "backgroundColor", this);
    backgroundColorAni->setDuration(120);
    installEventFilter(this);
    connect(&esConfig, &EsConfig::themeChanged, this, &EsBackgroundAnimationWidget::updateBackgroundColor);
}

void EsBackgroundAnimationWidget::updateBackgroundColor()
{
    QColor color;
    if (!this->isEnabled())
    {
        color = disabledBackgroundColor();
    }
    else if (qobject_cast<QLineEdit*>(this) && this->hasFocus())
    {
        color = focusInBackgroundColor();
    }
    else if (isPressed)
    {
        color = pressedBackgroundColor();
    }
    else if (isHover)
    {
        color = hoverBackgroundColor();
    }
    else
    {
        color = normalBackgroundColor();
    }

    backgroundColorAni->stop();
    backgroundColorAni->setEndValue(color);
    backgroundColorAni->start();
}

void EsBackgroundAnimationWidget::setBackgroundColor(const QColor& color)
{
    bgColorObject->setBackgroundColor(color);
}

QColor EsBackgroundAnimationWidget::normalBackgroundColor() const
{
    return {0, 0, 0, 0};
}

QColor EsBackgroundAnimationWidget::hoverBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationWidget::pressedBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationWidget::focusInBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationWidget::disabledBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationWidget::getBackgroundColor() const
{
    return bgColorObject->getBackgroundColor();
}

bool EsBackgroundAnimationWidget::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == this)
    {
        if (e->type() == QEvent::EnabledChange)
        {
            if (this->isEnabled())
            {
                setBackgroundColor(normalBackgroundColor());
            }
            else
            {
                setBackgroundColor(disabledBackgroundColor());
            }
        }
    }
    return EsWidget::eventFilter(obj, e);
}

void EsBackgroundAnimationWidget::mousePressEvent(QMouseEvent* e)
{
    isPressed = true;
    updateBackgroundColor();
    EsWidget::mousePressEvent(e);
}

void EsBackgroundAnimationWidget::mouseReleaseEvent(QMouseEvent* e)
{
    isPressed = false;
    updateBackgroundColor();
    EsWidget::mouseReleaseEvent(e);
}

void EsBackgroundAnimationWidget::enterEvent(QEvent* e)
{
    isHover = true;
    updateBackgroundColor();
}

void EsBackgroundAnimationWidget::leaveEvent(QEvent* e)
{
    isHover = false;
    updateBackgroundColor();
}

void EsBackgroundAnimationWidget::focusInEvent(QFocusEvent* e)
{
    EsWidget::focusInEvent(e);
    updateBackgroundColor();
}

// endregion

// region ================= EsBackgroundAnimationFrame  =================

EsBackgroundAnimationFrame::EsBackgroundAnimationFrame(QWidget* parent)
    : QFrame(parent)
{
    bgColorObject = new EsBackgroundColorObject(this);
    backgroundColorAni = new QPropertyAnimation(bgColorObject, "backgroundColor", this);
    backgroundColorAni->setDuration(120);
    installEventFilter(this);
    connect(&esConfig, &EsConfig::themeChanged, this, &EsBackgroundAnimationFrame::updateBackgroundColor);
}

void EsBackgroundAnimationFrame::updateBackgroundColor()
{
    QColor color;
    if (!this->isEnabled())
    {
        color = disabledBackgroundColor();
    }
    else if (qobject_cast<QLineEdit*>(this) && this->hasFocus())
    {
        color = focusInBackgroundColor();
    }
    else if (isPressed)
    {
        color = pressedBackgroundColor();
    }
    else if (isHover)
    {
        color = hoverBackgroundColor();
    }
    else
    {
        color = normalBackgroundColor();
    }

    backgroundColorAni->stop();
    backgroundColorAni->setEndValue(color);
    backgroundColorAni->start();
}

void EsBackgroundAnimationFrame::setBackgroundColor(const QColor& color)
{
    bgColorObject->setBackgroundColor(color);
}

QColor EsBackgroundAnimationFrame::normalBackgroundColor() const
{
    return {0, 0, 0, 0};
}

QColor EsBackgroundAnimationFrame::hoverBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationFrame::pressedBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationFrame::focusInBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationFrame::disabledBackgroundColor() const
{
    return normalBackgroundColor();
}

QColor EsBackgroundAnimationFrame::getBackgroundColor() const
{
    return bgColorObject->getBackgroundColor();
}

bool EsBackgroundAnimationFrame::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == this)
    {
        if (e->type() == QEvent::EnabledChange)
        {
            if (this->isEnabled())
            {
                setBackgroundColor(normalBackgroundColor());
            }
            else
            {
                setBackgroundColor(disabledBackgroundColor());
            }
        }
    }
    return QFrame::eventFilter(obj, e);
}

void EsBackgroundAnimationFrame::mousePressEvent(QMouseEvent* e)
{
    isPressed = true;
    updateBackgroundColor();
    QFrame::mousePressEvent(e);
}

void EsBackgroundAnimationFrame::mouseReleaseEvent(QMouseEvent* e)
{
    isPressed = false;
    updateBackgroundColor();
    QFrame::mouseReleaseEvent(e);
}

void EsBackgroundAnimationFrame::enterEvent(QEvent* e)
{
    isHover = true;
    updateBackgroundColor();
}

void EsBackgroundAnimationFrame::leaveEvent(QEvent* e)
{
    isHover = false;
    updateBackgroundColor();
}

void EsBackgroundAnimationFrame::focusInEvent(QFocusEvent* e)
{
    QFrame::focusInEvent(e);
    updateBackgroundColor();
}

// endregion


// region ================= BackgroundColorObject  =================

EsBackgroundColorObject::EsBackgroundColorObject(QObject* parent)
    : QObject(parent)
{
    auto p1 = dynamic_cast<EsBackgroundAnimationBase*>(parent);
    backgroundColor = p1->normalBackgroundColor();
}

QColor EsBackgroundColorObject::getBackgroundColor() const
{
    return backgroundColor;
}

void EsBackgroundColorObject::setBackgroundColor(const QColor& color)
{
    if (backgroundColor != color)
    {
        backgroundColor = color;
        qobject_cast<QWidget*>(parent())->update();
    }
}

// endregion
