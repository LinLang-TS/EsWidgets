/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esStackedWidget.cpp
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esStackedWidget.h"

#include <QAbstractScrollArea>
#include <QHBoxLayout>
#include <QScrollBar>

#include "widgets/esPopUpAniStackedWidget.h"
#include "private/esStackedWidget_p.h"


// region ================= EsStackedWidgetPrivate  =================
EsStackedWidgetPrivate::EsStackedWidgetPrivate(QObject* parent)
{
}
// endregion


// region ================= EsStackedWidget  =================

EsStackedWidget::EsStackedWidget(QWidget* parent)
    : QFrame(parent), d_ptr(new EsStackedWidgetPrivate())
{
    Q_D(EsStackedWidget);
    d->q_ptr = this;

    d->hBoxLayout = new QHBoxLayout(this);
    d->view = new EsPopUpAniStackedWidget(this);

    // 设置布局属性
    d->hBoxLayout->setContentsMargins(0, 0, 0, 0);
    d->hBoxLayout->addWidget(d->view);

    // 连接信号与槽
    connect(d->view, &EsPopUpAniStackedWidget::currentChanged, this, &EsStackedWidget::currentChanged);

    // 设置背景样式
    this->setAttribute(Qt::WA_StyledBackground);
}

EsStackedWidget::~EsStackedWidget()
{
}

bool EsStackedWidget::isAnimationEnabled() const
{
    Q_D(const EsStackedWidget);
    return d->view->isAnimationEnabled();
}

void EsStackedWidget::setAnimationEnabled(bool isEnabled)
{
    Q_D(EsStackedWidget);
    d->view->setAnimationEnabled(isEnabled);
}

void EsStackedWidget::addWidget(QWidget* widget)
{
    Q_D(EsStackedWidget);
    d->view->addWidget(widget);
}

void EsStackedWidget::removeWidget(QWidget* widget)
{
    Q_D(EsStackedWidget);
    d->view->removeWidget(widget);
}

QWidget* EsStackedWidget::widget(int index) const
{
    Q_D(const EsStackedWidget);
    return d->view->widget(index);
}

void EsStackedWidget::setCurrentWidget(QWidget* widget, bool popOut)
{
    Q_D(EsStackedWidget);
    if (qobject_cast<QAbstractScrollArea*>(widget))
    {
        // 如果是 QAbstractScrollArea 类型的 widget，重置其垂直滚动条
        auto scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
        scrollArea->verticalScrollBar()->setValue(0);
    }

    if (!popOut)
    {
        d->view->setCurrentWidget(widget, false, true, 300);
    }
    else
    {
        d->view->setCurrentWidget(widget, true, false, 200, QEasingCurve::InQuad);
    }
}

void EsStackedWidget::setCurrentIndex(int index, bool popOut)
{
    Q_D(EsStackedWidget);
    setCurrentWidget(d->view->widget(index), popOut);
}

int EsStackedWidget::currentIndex() const
{
    Q_D(const EsStackedWidget);
    return d->view->currentIndex();
}

QWidget* EsStackedWidget::currentWidget() const
{
    Q_D(const EsStackedWidget);
    return d->view->currentWidget();
}

int EsStackedWidget::indexOf(QWidget* widget) const
{
    Q_D(const EsStackedWidget);
    return d->view->indexOf(widget);
}

int EsStackedWidget::count() const
{
    Q_D(const EsStackedWidget);
    return d->view->count();
}

// endregion
