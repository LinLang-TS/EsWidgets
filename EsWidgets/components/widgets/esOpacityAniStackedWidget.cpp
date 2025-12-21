/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esOpacityAniStackedWidget.cpp
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esOpacityAniStackedWidget.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "private/esOpacityAniStackedWidget_p.h"

// region ================= EsOpacityAniStackedWidgetPrivate  =================

EsOpacityAniStackedWidgetPrivate::EsOpacityAniStackedWidgetPrivate(QObject* parent)
    : QObject(parent), nextIndex(0)
{
}

// endregion


// region ================= EsOpacityAniStackedWidget  =================

EsOpacityAniStackedWidget::EsOpacityAniStackedWidget(QWidget* parent)
    : QStackedWidget(parent), d_ptr(new EsOpacityAniStackedWidgetPrivate())
{
}

EsOpacityAniStackedWidget::~EsOpacityAniStackedWidget()
{
}

void EsOpacityAniStackedWidget::addWidget(QWidget* w)
{
    Q_D(EsOpacityAniStackedWidget);
    QStackedWidget::addWidget(w);

    // 创建并设置透明度效果
    auto effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(1);

    // 创建动画并设置持续时间为220毫秒
    auto ani = new QPropertyAnimation(effect, "opacity", this);
    ani->setDuration(220);
    connect(ani, &QPropertyAnimation::finished, this, &EsOpacityAniStackedWidget::onAniFinished);

    // 将动画和效果添加到各自的列表中
    d->animations.append(ani);
    d->effects.append(effect);

    w->setGraphicsEffect(effect);
}

void EsOpacityAniStackedWidget::setCurrentIndex(int index)
{
    Q_D(EsOpacityAniStackedWidget);
    int currentIndex = this->currentIndex();

    // 如果目标索引和当前索引相同，则无需更改
    if (index == currentIndex)
    {
        return;
    }

    QPropertyAnimation* ani = nullptr;

    // 如果目标索引大于当前索引，则执行淡入动画
    if (index > currentIndex)
    {
        ani = d->animations.at(index);
        ani->setStartValue(0);
        ani->setEndValue(1);
        QStackedWidget::setCurrentIndex(index); // 调用基类的 setCurrentIndex
    }
    // 如果目标索引小于当前索引，则执行淡出动画
    else
    {
        ani = d->animations.at(currentIndex);
        ani->setStartValue(1);
        ani->setEndValue(0);
    }

    // 显示当前小部件
    widget(currentIndex)->show();

    d->nextIndex = index;

    // 启动动画
    ani->start();
}

void EsOpacityAniStackedWidget::setCurrentWidget(QWidget* w)
{
    setCurrentIndex(indexOf(w));
}

void EsOpacityAniStackedWidget::onAniFinished()
{
    Q_D(EsOpacityAniStackedWidget);
    QStackedWidget::setCurrentIndex(d->nextIndex);
}


// endregion
