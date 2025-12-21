/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPopUpAniStackedWidget_p.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESPOPUPANISTACKEDWIDGET_P_H
#define ESPOPUPANISTACKEDWIDGET_P_H
#include "esDef.h"

class QPropertyAnimation;

struct PopUpAniInfo
{
    QWidget* widget; // 控件指针
    int deltaX; // 水平偏移
    int deltaY; // 垂直偏移
    QPropertyAnimation* ani; // 动画对象指针

    // 构造函数，初始化成员
    PopUpAniInfo(QWidget* widget, int deltaX, int deltaY, QPropertyAnimation* ani)
        : widget(widget), deltaX(deltaX), deltaY(deltaY), ani(ani)
    {
    }
};

class EsPopUpAniStackedWidget;

class EsPopUpAniStackedWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsPopUpAniStackedWidget)

public:
    explicit EsPopUpAniStackedWidgetPrivate(QObject* parent = nullptr);
    /**
     * @brief 设置动画的配置
     *
     * 此方法设置动画的插值曲线、起始值、结束值和持续时间，以便启动时可以应用这些配置。
     *
     * @param ani 要配置的动画对象
     * @param startValue 动画的起始值
     * @param endValue 动画的结束值
     * @param duration 动画的持续时间（毫秒）
     * @param easingCurve 插值曲线，默认为线性插值（Linear）
     */
    void setAnimation(QPropertyAnimation* ani, const QVariant& startValue, const QVariant& endValue, int duration, const QEasingCurve& easingCurve);
    QList<PopUpAniInfo> aniInfos; ///< 动画信息列表
    bool isAnimationEnabled; ///< 是否启用动画
    int nextIndex; ///< 下一个索引
    QPropertyAnimation* ani; ///< 当前动画对象
};


#endif //ESPOPUPANISTACKEDWIDGET_P_H
