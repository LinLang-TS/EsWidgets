/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPopUpAniStackedWidget.cpp
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esPopUpAniStackedWidget.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "private/esPopUpAniStackedWidget_p.h"



// region ================= EsPopUpAniStackedWidgetPrivate  =================

EsPopUpAniStackedWidgetPrivate::EsPopUpAniStackedWidgetPrivate(QObject* parent)
    : QObject(parent), isAnimationEnabled(true), nextIndex(0), ani(nullptr)
{
}

void EsPopUpAniStackedWidgetPrivate::setAnimation(QPropertyAnimation* ani, const QVariant& startValue,
    const QVariant& endValue, int duration, const QEasingCurve& easingCurve)
{
    ani->setEasingCurve(easingCurve);  // 设置插值曲线
    ani->setStartValue(startValue);    // 设置动画的起始值
    ani->setEndValue(endValue);        // 设置动画的结束值
    ani->setDuration(duration);        // 设置动画的持续时间
}

// endregion

// region ================= EsPopUpAniStackedWidget  =================

EsPopUpAniStackedWidget::EsPopUpAniStackedWidget(QWidget* parent)
    : QStackedWidget(parent), d_ptr(new EsPopUpAniStackedWidgetPrivate())
{
    Q_D(EsPopUpAniStackedWidget);
    d->q_ptr = this;
}

EsPopUpAniStackedWidget::~EsPopUpAniStackedWidget()
{
}

void EsPopUpAniStackedWidget::addWidget(QWidget* widget, int deltaX, int deltaY)
{
    Q_D(EsPopUpAniStackedWidget);
    // 调用父类的方法添加小部件
    QStackedWidget::addWidget(widget);

    // 创建动画信息对象并将其添加到动画信息列表
    PopUpAniInfo aniInfo = {
        widget,
        deltaX,
        deltaY,
        new QPropertyAnimation(widget, "pos", this)
    };

    d->aniInfos.append(aniInfo);
}

void EsPopUpAniStackedWidget::removeWidget(QWidget* widget)
{
    Q_D(EsPopUpAniStackedWidget);
    // 获取小部件的索引
    int index = indexOf(widget);

    // 如果小部件不在堆叠小部件中，则返回
    if (index == -1) {
        return;
    }

    // 移除与该小部件关联的动画信息
    d->aniInfos.removeAt(index);

    // 调用父类的 removeWidget 方法移除小部件
    QStackedWidget::removeWidget(widget);
}

void EsPopUpAniStackedWidget::setAnimationEnabled(bool isEnabled)
{
    Q_D(EsPopUpAniStackedWidget);
    d->isAnimationEnabled = isEnabled;
}

void EsPopUpAniStackedWidget::setCurrentIndex(int index, bool needPopOut, bool showNextWidgetDirectly, int duration,
    QEasingCurve easingCurve)
{
    Q_D(EsPopUpAniStackedWidget);
    if (index < 0 || index >= count()) {
        return;
    }

    if (index == currentIndex()) {
        return;
    }

    if (!d->isAnimationEnabled) {
        return QStackedWidget::setCurrentIndex(index);
    }

    if (d->ani && d->ani->state() == QAbstractAnimation::Running) {
        d->ani->stop();
        onAniFinished();
    }

    // 获取要显示的窗口的索引
    d->nextIndex = index;

    // 获取动画信息
    PopUpAniInfo& nextAniInfo = d->aniInfos[index];
    PopUpAniInfo& currentAniInfo = d->aniInfos[currentIndex()];

    QWidget* currentWidget = this->currentWidget();
    QWidget* nextWidget = nextAniInfo.widget;
    QPropertyAnimation* ani = needPopOut ? currentAniInfo.ani : nextAniInfo.ani;
    d->ani = ani;

    if (needPopOut) {
        int deltaX = currentAniInfo.deltaX;
        int deltaY = currentAniInfo.deltaY;
        QPoint pos = currentWidget->pos() + QPoint(deltaX, deltaY);
        d->setAnimation(ani, currentWidget->pos(), pos, duration, easingCurve);
        nextWidget->setVisible(showNextWidgetDirectly);
    } else {
        int deltaX = nextAniInfo.deltaX;
        int deltaY = nextAniInfo.deltaY;
        QPoint pos = nextWidget->pos() + QPoint(deltaX, deltaY);
        d->setAnimation(ani, pos, QPoint(nextWidget->x(), 0), duration, easingCurve);
        QStackedWidget::setCurrentIndex(index);
    }

    // 启动动画
    connect(ani, &QPropertyAnimation::finished, this, &EsPopUpAniStackedWidget::onAniFinished);
    ani->start();
    emit aniStart();
}

void EsPopUpAniStackedWidget::setCurrentWidget(QWidget* widget, bool needPopOut, bool showNextWidgetDirectly,
    int duration, QEasingCurve easingCurve)
{
    setCurrentIndex(indexOf(widget), needPopOut, showNextWidgetDirectly, duration, easingCurve);
}

bool EsPopUpAniStackedWidget::isAnimationEnabled() const
{
    Q_D(const EsPopUpAniStackedWidget);
    return d->isAnimationEnabled;
}

void EsPopUpAniStackedWidget::onAniFinished()
{
    Q_D(EsPopUpAniStackedWidget);
    d->ani->disconnect();                // 断开动画的连接
    QStackedWidget::setCurrentIndex(d->nextIndex);       // 切换到下一个 widget
    emit aniFinished();                // 发出动画完成信号
}

// endregion
