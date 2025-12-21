/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esSmoothScrollDelegate.cpp
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esSmoothScrollDelegate.h"
#include "widgets/esScrollBar.h"
#include "common/esSmoothScroll.h"
#include <QAbstractItemView>
#include <qevent.h>
#include <QEvent>
#include <QListView>
#include <QScrollBar>

EsSmoothScrollDelegate::EsSmoothScrollDelegate(QAbstractScrollArea* parent, bool useAni)
: QObject(parent), useAni(useAni)
{
    // 这里parent必须用 static_cast , 动态转换或 qobject_cast 都转换失败
    auto parent_ = static_cast<QScrollArea*>(parent);
    // 创建垂直与水平平滑滚动条
    vScrollBar = new EsSmoothScrollBar(Qt::Vertical, parent);
    hScrollBar = new EsSmoothScrollBar(Qt::Horizontal, parent);

    // 创建平滑滚动处理器;
    verticalSmoothScroll = new EsSmoothScroll(parent_, Qt::Vertical);
    horizontalSmoothScroll = new EsSmoothScroll(parent_, Qt::Horizontal);

    // 针对 QAbstractItemView 类型，启用像素级滚动
    if (auto itemView = qobject_cast<QAbstractItemView*>(parent)) {
        itemView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        itemView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

    // 针对 QListView，隐藏水平滚动条（通过高度 0px 样式）
    if (auto listView = qobject_cast<QListView*>(parent)) {
        listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{height: 0px}");
    }

    // 安装事件过滤器
    parent_->viewport()->installEventFilter(this);
    // todo 强制隐藏滚动条 这里无法像py那样替换父类函数, 后面想办法hook
    // parent_->setVerticalScrollBarPolicy = setVerticalScrollBarPolicy;
    // parent_->setHorizontalScrollBarPolicy = setHorizontalScrollBarPolicy;

}

bool EsSmoothScrollDelegate::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::Wheel) {
        auto* wheelEvent = dynamic_cast<QWheelEvent*>(event);

        // 判断垂直滚动是否到达边界
        bool verticalAtEnd =
            (wheelEvent->angleDelta().y() < 0 && vScrollBar->value() == vScrollBar->maximum()) ||
            (wheelEvent->angleDelta().y() > 0 && vScrollBar->value() == vScrollBar->minimum());

        // 判断水平滚动是否到达边界
        bool horizontalAtEnd =
            (wheelEvent->angleDelta().x() < 0 && hScrollBar->value() == hScrollBar->maximum()) ||
            (wheelEvent->angleDelta().x() > 0 && hScrollBar->value() == hScrollBar->minimum());

        if (verticalAtEnd || horizontalAtEnd)
            return false;

        // 垂直滚动事件
        if (wheelEvent->angleDelta().y() != 0) {
            if (!useAni)
                verticalSmoothScroll->wheelEvent(wheelEvent);
            else
                vScrollBar->scrollValue(-wheelEvent->angleDelta().y());
        }
        // 水平滚动事件
        else {
            if (!useAni)
                horizontalSmoothScroll->wheelEvent(wheelEvent);
            else
                hScrollBar->scrollValue(-wheelEvent->angleDelta().x());
        }

        wheelEvent->accept();
        return true;
    }

    return QObject::eventFilter(obj, event);
}

void EsSmoothScrollDelegate::setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    if (!parent)
        return;

    parent->QAbstractScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vScrollBar->setForceHidden(policy == Qt::ScrollBarAlwaysOff);
}

void EsSmoothScrollDelegate::setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy)
{
    if (!parent)
        return;

    parent->QAbstractScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    hScrollBar->setForceHidden(policy == Qt::ScrollBarAlwaysOff);
}
