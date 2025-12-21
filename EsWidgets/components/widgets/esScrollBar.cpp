/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScrollBar.cpp
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esScrollBar.h"
#include "esConfig.h"
#include "esIcon.h"
#include "subButton/esArrowButton.h"

#include <QAbstractScrollArea>
#include <QApplication>
#include <qevent.h>
#include <QEvent>
#include <QScrollBar>
#include <QTimer>
#include <QPainter>
#include <QHBoxLayout>

#include "private/esScrollBar_p.h"
// region ================= EsScrollBarGroove =================

EsScrollBarGroove::EsScrollBarGroove(Qt::Orientation orient, QWidget* parent)
    : QWidget(parent), opacity(1.0f), lightBackgroundColor(252, 252, 252, 217), darkBackgroundColor(44, 44, 44, 245)
{
    if (orient == Qt::Vertical)
    {
        setFixedWidth(12);
        upButton = new EsArrowButton(Es::Icon_CareUpSolid, this);
        downButton = new EsArrowButton(Es::Icon_CareDownSolid, this);
        auto layout = new QVBoxLayout(this);
        setLayout(layout);
        layout->addWidget(upButton, 0, Qt::AlignHCenter);
        layout->addStretch(1);
        layout->addWidget(downButton, 0, Qt::AlignHCenter);
        layout->setContentsMargins(0, 3, 0, 3);
    }
    else
    {
        setFixedHeight(12);
        upButton = new EsArrowButton(Es::Icon_CareLeftSolid, this);
        downButton = new EsArrowButton(Es::Icon_CareRightSolid, this);
        auto layout = new QHBoxLayout(this);
        setLayout(layout);
        layout->addWidget(upButton, 0, Qt::AlignVCenter);
        layout->addStretch(1);
        layout->addWidget(downButton, 0, Qt::AlignVCenter);
        layout->setContentsMargins(3, 0, 3, 0);
    }

    opacityAni = new QPropertyAnimation(this, "opacity", this);
    setOpacity(0.0f);
}

void EsScrollBarGroove::setLightBackgroundColor(const QColor& color)
{
    lightBackgroundColor = color;
    update();
}

void EsScrollBarGroove::setDarkBackgroundColor(const QColor& color)
{
    darkBackgroundColor = color;
    update();
}

void EsScrollBarGroove::fadeIn() const
{
    opacityAni->stop();
    opacityAni->setStartValue(opacity);
    opacityAni->setEndValue(1.0f);
    opacityAni->setDuration(150);
    opacityAni->start();
}

void EsScrollBarGroove::fadeOut() const
{
    opacityAni->stop();
    opacityAni->setStartValue(opacity);
    opacityAni->setEndValue(0.0f);
    opacityAni->setDuration(150);
    opacityAni->start();
}

float EsScrollBarGroove::getOpacity() const
{
    return opacity;
}

void EsScrollBarGroove::setOpacity(float opacity)
{
    this->opacity = opacity;
    upButton->setOpacity(opacity);
    downButton->setOpacity(opacity);
    update();
}

void EsScrollBarGroove::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setOpacity(opacity);
    painter.setPen(Qt::NoPen);

    QColor backgroundColor = EsFunc::isDarkTheme() ? darkBackgroundColor : lightBackgroundColor;
    painter.setBrush(backgroundColor);
    painter.drawRoundedRect(rect(), 6, 6);
}

// endregion


// region ================= ScrollBarHandle =================

EsScrollBarHandle::EsScrollBarHandle(Qt::Orientation orient, QWidget* parent) : QWidget(parent), opacity(1.0f),
    orient(orient), lightColor(0, 0, 0, 114),
    darkColor(255, 255, 255, 139)
{
    if (orient == Qt::Vertical)
    {
        setFixedWidth(3);
    }
    else
    {
        setFixedHeight(3);
    }

    opacityAni = new QPropertyAnimation(this, "opacity", this);
}

void EsScrollBarHandle::setLightColor(const QColor& color)
{
    lightColor = color;
    update();
}

void EsScrollBarHandle::setDarkColor(const QColor& color)
{
    darkColor = color;
    update();
}

float EsScrollBarHandle::getOpacity() const
{
    return opacity;
}

void EsScrollBarHandle::setOpacity(float opacity)
{
    this->opacity = opacity;
    update();
}

void EsScrollBarHandle::fadeIn() const
{
    opacityAni->stop();
    opacityAni->setStartValue(opacity);
    opacityAni->setEndValue(1.0f);
    opacityAni->setDuration(150);
    opacityAni->start();
}

void EsScrollBarHandle::fadeOut() const
{
    opacityAni->stop();
    opacityAni->setStartValue(opacity);
    opacityAni->setEndValue(0.0f);
    opacityAni->setDuration(150);
    opacityAni->start();
}

void EsScrollBarHandle::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    float radius = (orient == Qt::Vertical) ? (float)width() / 2 : (float)height() / 2;
    painter.setOpacity(opacity);

    QColor color = (EsFunc::isDarkTheme()) ? darkColor : lightColor;
    painter.setBrush(color);
    painter.drawRoundedRect(rect(), radius, radius);
}

// region ================= EsScrollBarPrivate =================

EsScrollBarPrivate::EsScrollBarPrivate(QObject* parent)
    : QObject(parent),
      singleStep(1),
      pageStep(50),
      padding(14),
      minimum(0),
      maximum(0),
      value(0),
      isPressed(false),
      isEnter(false),
      isExpanded(false),
      isForceHidden(false)
{
}


void EsScrollBarPrivate::initWidget(QAbstractScrollArea* parent)
{
    Q_Q(EsScrollBar);
    connect(q->groove->upButton, &QToolButton::clicked, this, &EsScrollBarPrivate::onPageUp);
    connect(q->groove->downButton, &QToolButton::clicked, this, &EsScrollBarPrivate::onPageDown);
    connect(q->groove->opacityAni, &QVariantAnimation::valueChanged, this,
            &EsScrollBarPrivate::onOpacityAniValueChanged);

    connect(q->partnerBar, &QAbstractSlider::rangeChanged, q, &EsScrollBar::setRange);
    connect(q->partnerBar, &QAbstractSlider::valueChanged, this, &EsScrollBarPrivate::onValueChanged);
    connect(q, &EsScrollBar::valueChanged, q->partnerBar, &QScrollBar::setValue);


    parent->installEventFilter(q);

    q->setRange(q->partnerBar->minimum(), q->partnerBar->maximum());
    q->setVisible(q->maximum() > 0 && !isForceHidden);
    adjustPos(parent->size());
}


void EsScrollBarPrivate::onPageUp()
{
    Q_Q(EsScrollBar);
    q->setValue(q->value() - q->pageStep());
}

void EsScrollBarPrivate::onPageDown()
{
    Q_Q(EsScrollBar);
    q->setValue(q->value() + q->pageStep());
}

void EsScrollBarPrivate::onValueChanged(int val)
{
    Q_Q(EsScrollBar);
    q->setVal(val);
}

void EsScrollBarPrivate::adjustPos(const QSize& size)
{
    Q_Q(EsScrollBar);
    if (q->orientation() == Qt::Vertical)
    {
        q->resize(12, size.height() - 2);
        q->move(size.width() - 13, 1);
    }
    else
    {
        q->resize(size.width() - 2, 12);
        q->move(1, size.height() - 13);
    }
}


void EsScrollBarPrivate::adjustHandleSize()
{
    Q_Q(EsScrollBar);
    auto p = dynamic_cast<QWidget*>(q->parent());
    if (q->orientation() == Qt::Vertical)
    {
        int total = q->maximum() - q->minimum() + p->height();
        int s = (float)grooveLength() * p->height() / qMax(total, 1);
        q->handle->setFixedHeight(qMax(30, s));
    }
    else
    {
        int total = q->maximum() - q->minimum() + p->width();
        int s = (float)grooveLength() * p->width() / qMax(total, 1);
        q->handle->setFixedWidth(qMax(30, s));
    }
}

void EsScrollBarPrivate::adjustHandlePos()
{
    Q_Q(EsScrollBar);

    int total = qMax(q->maximum() - q->minimum(), 1);
    // 避雷, C++必须把其中一个操作数转换为float, 会自动转换另外2个; 全用int计算结果会一直为0
    int delta = (float)q->value() / total * slideLength();


    if (q->orientation() == Qt::Vertical)
    {
        int x = q->width() - q->handle->width() - 3;
        q->handle->move(x, padding + delta);
    }
    else
    {
        int y = q->height() - q->handle->height() - 3;
        q->handle->move(padding + delta, y);
    }
}

int EsScrollBarPrivate::grooveLength() const
{
    Q_Q(const EsScrollBar);
    if (q->orientation() == Qt::Vertical)
    {
        return q->height() - 2 * padding;
    }

    return q->width() - 2 * padding;
}

int EsScrollBarPrivate::slideLength() const
{
    Q_Q(const EsScrollBar);
    if (q->orientation() == Qt::Vertical)
    {
        return grooveLength() - q->handle->height();
    }

    return grooveLength() - q->handle->width();
}

bool EsScrollBarPrivate::isSlideResion(const QPoint& pos) const
{
    Q_Q(const EsScrollBar);
    if (q->orientation() == Qt::Vertical)
    {
        return padding <= pos.y() && pos.y() <= q->height() - padding;
    }

    return padding <= pos.x() && pos.x() <= q->width() - padding;
}

void EsScrollBarPrivate::onOpacityAniValueChanged()
{
    Q_Q(EsScrollBar);
    float opacity = q->groove->getOpacity();
    if (q->orientation() == Qt::Vertical)
    {
        q->handle->setFixedWidth(int(3 + opacity * 3));
    }
    else
    {
        q->handle->setFixedHeight(int(3 + opacity * 3));
    }

    adjustHandlePos();
}

// endregion

// region ================= EsSmoothScrollBarPrivate =================

EsSmoothScrollBarPrivate::EsSmoothScrollBarPrivate(QObject* parent)
: QObject(parent)
{
}

// endregion


// region ================= EsScrollBar =================

EsScrollBar::EsScrollBar(Qt::Orientation orient, QAbstractScrollArea* parent)
    : QWidget(parent), handleDisplayMode(Es::ScrollBarHandleDisplayMode::ALWAYS), d_ptr(new EsScrollBarPrivate())
{
    Q_D(EsScrollBar);
    d->q_ptr = this;
    d->orientation = orient;
    groove = new EsScrollBarGroove(orient, this);
    handle = new EsScrollBarHandle(orient, this);
    if (orient == Qt::Vertical)
    {
        partnerBar = parent->verticalScrollBar();
        parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        partnerBar = parent->horizontalScrollBar();
        parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    d->initWidget(parent);
}

EsScrollBar::~EsScrollBar()
{
}

int EsScrollBar::val() const
{
    Q_D(const EsScrollBar);
    return d->value;
}

int EsScrollBar::value() const
{
    Q_D(const EsScrollBar);
    return d->value;
}

int EsScrollBar::minimum() const
{
    Q_D(const EsScrollBar);
    return d->minimum;
}

int EsScrollBar::maximum() const
{
    Q_D(const EsScrollBar);
    return d->maximum;
}

Qt::Orientation EsScrollBar::orientation() const
{
    Q_D(const EsScrollBar);
    return d->orientation;
}

int EsScrollBar::pageStep() const
{
    Q_D(const EsScrollBar);
    return d->pageStep;
}

int EsScrollBar::singleStep() const
{
    Q_D(const EsScrollBar);
    return d->singleStep;
}

bool EsScrollBar::isSliderDown() const
{
    Q_D(const EsScrollBar);
    return d->isPressed;
}

void EsScrollBar::setVal(int value)
{
    Q_D(EsScrollBar);
    if (this->value() == value) return;
    value = qMax(minimum(), qMin(value, maximum()));
    d->value = value;
    emit valueChanged(value);

    // 调整滑块位置
    d->adjustHandlePos();
}

void EsScrollBar::setValue(int value, bool useAni)
{
    setVal(value);
}

void EsScrollBar::setMinimum(int min)
{
    Q_D(EsScrollBar);
    if (min == minimum()) return;

    d->minimum = min;
    emit rangeChanged(min, maximum());
}

void EsScrollBar::setMaximum(int max)
{
    Q_D(EsScrollBar);
    if (max == maximum()) return;

    d->maximum = max;
    emit rangeChanged(minimum(), max);
}

void EsScrollBar::setRange(int min, int max)
{
    Q_D(EsScrollBar);
    if (min > max || (min == minimum() && max == maximum())) return;

    setMinimum(min);
    setMaximum(max);

    d->adjustHandleSize();
    d->adjustHandlePos();
    setVisible(max > 0 && !d->isForceHidden);

    emit rangeChanged(min, max);
}

void EsScrollBar::setPageStep(int step)
{
    Q_D(EsScrollBar);
    if (step >= 1)
    {
        d->pageStep = step;
    }
}

void EsScrollBar::setSingleStep(int step)
{
    Q_D(EsScrollBar);
    if (step >= 1)
    {
        d->singleStep = step;
    }
}

void EsScrollBar::setSliderDown(bool isDown)
{
    Q_D(EsScrollBar);
    d->isPressed = true;
    if (isDown)
        emit sliderPressed();
    else
        emit sliderReleased();
}


void EsScrollBar::setHandleColor(const QColor& light, const QColor& dark)
{
    Q_D(EsScrollBar);
    handle->setLightColor(light);
    handle->setDarkColor(dark);
}

void EsScrollBar::setArrowColor(const QColor& light, const QColor& dark)
{
    Q_D(EsScrollBar);
    groove->upButton->setLightColor(light);
    groove->upButton->setDarkColor(dark);
    groove->downButton->setLightColor(light);
    groove->downButton->setDarkColor(dark);
}

void EsScrollBar::setGrooveColor(const QColor& light, const QColor& dark)
{
    Q_D(EsScrollBar);

    groove->setLightBackgroundColor(light);
    groove->setDarkBackgroundColor(dark);
}

void EsScrollBar::setHandleDisplayMode(Es::ScrollBarHandleDisplayMode mode)
{
    Q_D(EsScrollBar);

    if (mode == handleDisplayMode)
    {
        return;
    }

    handleDisplayMode = mode;

    if (mode == Es::ScrollBarHandleDisplayMode::ON_HOVER && !d->isEnter)
    {
        handle->fadeOut();
    }
    else if (mode == Es::ScrollBarHandleDisplayMode::ALWAYS)
    {
        handle->fadeIn();
    }
}

void EsScrollBar::expand()
{
    Q_D(EsScrollBar);

    if (d->isExpanded || !d->isEnter)
    {
        return;
    }

    d->isExpanded = true;
    groove->fadeIn();
    handle->fadeIn();
}

void EsScrollBar::collapse()
{
    Q_D(EsScrollBar);

    if (!d->isExpanded || d->isEnter)
    {
        return;
    }

    d->isExpanded = false;
    groove->fadeOut();

    if (handleDisplayMode == Es::ScrollBarHandleDisplayMode::ON_HOVER)
    {
        handle->fadeOut();
    }
}

void EsScrollBar::setForceHidden(bool isHidden)
{
    Q_D(EsScrollBar);

    d->isForceHidden = isHidden;
    setVisible(maximum() > 0 && !isHidden);
}

void EsScrollBar::wheelEvent(QWheelEvent* e)
{
    QApplication::sendEvent(dynamic_cast<QAbstractScrollArea*>(parent())->viewport(), e);
}

void EsScrollBar::enterEvent(QEvent* e)
{
    Q_D(EsScrollBar);
    d->isEnter = true;
    QTimer::singleShot(200, this, &EsScrollBar::expand);
}

void EsScrollBar::leaveEvent(QEvent* e)
{
    Q_D(EsScrollBar);
    d->isEnter = false;
    QTimer::singleShot(200, this, &EsScrollBar::collapse);
}

bool EsScrollBar::eventFilter(QObject* obj, QEvent* e)
{
    Q_D(EsScrollBar);
    if (obj != parent())
    {
        return QObject::eventFilter(obj, e);
    }

    // 调整滑块的位置
    if (e->type() == QEvent::Resize)
    {
        d->adjustPos(dynamic_cast<QResizeEvent*>(e)->size());
    }

    return QObject::eventFilter(obj, e);
}

void EsScrollBar::resizeEvent(QResizeEvent* event)
{
    groove->resize(size());
}

void EsScrollBar::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsScrollBar);
    QWidget::mousePressEvent(e);
    d->isPressed = true;
    d->pressedPos = e->pos();

    if (childAt(e->pos()) == handle || !d->isSlideResion(e->pos()))
    {
        return;
    }

    int value = 0;
    if (orientation() == Qt::Vertical)
    {
        if (e->pos().y() > handle->geometry().bottom())
        {
            value = e->pos().y() - handle->height() - d->padding;
        }
        else
        {
            value = e->pos().y() - d->padding;
        }
    }
    else
    {
        if (e->pos().x() > handle->geometry().right())
        {
            value = e->pos().x() - handle->width() - d->padding;
        }
        else
        {
            value = e->pos().x() - d->padding;
        }
    }

    setValue((float)value / qMax(d->slideLength(), 1) * maximum());
    emit sliderPressed();
}

void EsScrollBar::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    Q_D(EsScrollBar);

    d->isPressed = false;
    emit sliderReleased();
}

void EsScrollBar::mouseMoveEvent(QMouseEvent* e)
{
    Q_D(EsScrollBar);
    int dv;

    if (orientation() == Qt::Vertical)
    {
        dv = e->pos().y() - d->pressedPos.y();
    }
    else
    {
        dv = e->pos().x() - d->pressedPos.x();
    }
    dv = (float)dv / qMax(d->slideLength(), 1) * (maximum() - minimum());
    EsScrollBar::setValue(value() + dv);
    d->pressedPos = e->pos();
    emit sliderMoved();
}

// endregion


// region ================= EsSmoothScrollBar =================
EsSmoothScrollBar::EsSmoothScrollBar(Qt::Orientation orient, QAbstractScrollArea* parent)
    : EsScrollBar(orient, parent), d_ptr(new EsSmoothScrollBarPrivate())
{
    Q_D(EsSmoothScrollBar);
    d->q_ptr = this;

    duration = 500;
    ani = new QPropertyAnimation(this);
    ani->setTargetObject(this);
    ani->setPropertyName("val");
    ani->setEasingCurve(QEasingCurve::OutCubic);
    ani->setDuration(duration);
}

EsSmoothScrollBar::~EsSmoothScrollBar()
{
}

void EsSmoothScrollBar::setValue(int value, bool useAni)
{
    if (value == this->value())
        return;

    // 停止正在运行的动画
    ani->stop();

    if (!useAni)
    {
        setVal(value);
        return;
    }

    // 调整动画的持续时间
    int dv = qAbs(value - this->value());
    if (dv < 50)
    {
        ani->setDuration(duration * (float)dv / 70);
    }
    else
    {
        ani->setDuration(duration);
    }

    ani->setStartValue(this->value());
    ani->setEndValue(value);
    ani->start();
}

void EsSmoothScrollBar::scrollValue(int value, bool useAni)
{
    Q_D(EsSmoothScrollBar);
    d->value += value;
    d->value = qMax(this->minimum(), d->value);
    d->value = qMin(this->maximum(), d->value);
    setValue(d->value, useAni);
}

void EsSmoothScrollBar::scrollTo(int value, bool useAni)
{
    Q_D(EsSmoothScrollBar);
    d->value = value;
    d->value = qMax(this->minimum(), d->value);
    d->value = qMin(this->maximum(), d->value);
    setValue(d->value, useAni);
}

void EsSmoothScrollBar::resetValue(int value)
{
    Q_D(EsSmoothScrollBar);
    d->value = value;
}

void EsSmoothScrollBar::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsSmoothScrollBar);
    ani->stop();
    EsScrollBar::mousePressEvent(e);
    d->value = value();
}

void EsSmoothScrollBar::mouseMoveEvent(QMouseEvent* e)
{
    Q_D(EsSmoothScrollBar);
    ani->stop();
    EsScrollBar::mouseMoveEvent(e);
    d->value = value();
}

void EsSmoothScrollBar::setScrollAnimation(int duration, QEasingCurve easing)
{
    this->duration = duration;
    ani->setDuration(duration);
    ani->setEasingCurve(easing);
}


// endregion
