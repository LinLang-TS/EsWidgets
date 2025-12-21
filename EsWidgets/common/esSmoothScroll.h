/****************************************************************************
** @brief 平滑滚动
** @details 这家伙很懒, 什么都没写
** @file esSmoothScroll.h
** @date 2025/11/12
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSMOOTHSCROLL_H
#define ESSMOOTHSCROLL_H
#include <QScrollArea.h>
#include <deque>
#include <QTimer>
#include <QWidget>

#include "esDef.h"


class ES_EXPORT EsSmoothScroll : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化平滑滚动
     * 
     * @param widget 滚动区域，用于平滑滚动
     * @param orient 滚动方向
     */
    EsSmoothScroll(QScrollArea* widget, Qt::Orientation orient);

    // 设置平滑模式
    void setSmoothMode(Es::SmoothMode smoothMode);

    /**
     * @brief 仅处理由鼠标触发的滚轮事件，修复问题 #75
     *
     * @param e 滚轮事件
     */
    void wheelEvent(QWheelEvent* e);

private:
    /**
     * @brief 当定时器超时时，平滑滚动
     */
    Q_SLOT void smoothMove();

    /**
     * @brief 获取每步的插值
     *
     * @param delta 滚动距离
     * @param stepsLeft 剩余步骤数
     * @return 插值结果
     */
    double subDelta(double delta, int stepsLeft);

public:
    QScrollArea* widget;
    Qt::Orientation orient;                      // 滚动方向（垂直或水平）
    int fps;                                     // 帧率（每秒帧数）
    int duration;                                // 动画持续时间（毫秒）
    int stepsTotal;                              // 总步数
    double stepRatio;                            // 步长比例系数
    double acceleration;                         // 加速系数
    QWheelEvent *lastWheelEvent;                 // 上一次的滚轮事件
    std::deque<qint64> scrollStamps;             // 滚动时间戳队列（用于计算加速）
    std::deque<std::pair<double, int>> stepsLeftQueue; // 待处理的滚动任务队列（距离，剩余步数）
    QTimer *smoothMoveTimer;                     // 平滑滚动定时器
    Es::SmoothMode smoothMode;                   // 当前平滑模式
};


#endif //ESSMOOTHSCROLL_H
