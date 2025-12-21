/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esSmoothScrollDelegate.h
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSMOOTHSCROLLDELEGATE_H
#define ESSMOOTHSCROLLDELEGATE_H
#include <QAbstractScrollArea>

#include "esDef.h"

class EsSmoothScroll;
class EsSmoothScrollBar;

// 平滑滚动委托
class ES_EXPORT EsSmoothScrollDelegate : public QObject
{
public:
    EsSmoothScrollDelegate(QAbstractScrollArea* parent, bool useAni = false);

protected:
    /**
     * @brief 事件过滤器：拦截鼠标滚轮事件
     */
    bool eventFilter(QObject* obj, QEvent* event) override;

public Q_SLOTS:
    /**
     * @brief 设置垂直滚动条策略（重写）
     * @param policy Qt::ScrollBarPolicy
     */
    void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);

    /**
     * @brief 设置水平滚动条策略（重写）
     * @param policy Qt::ScrollBarPolicy
     */
    void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);

public:
    EsSmoothScrollBar* vScrollBar; // 垂直滚动条
    EsSmoothScrollBar* hScrollBar; // 水平滚动条
    EsSmoothScroll* verticalSmoothScroll; // 垂直平滑滚动对象
    EsSmoothScroll* horizontalSmoothScroll; // 水平平滑滚动对象
private:
    bool useAni; // 是否使用动画
    QAbstractScrollArea* parent; // 被代理控件
};


#endif //ESSMOOTHSCROLLDELEGATE_H
