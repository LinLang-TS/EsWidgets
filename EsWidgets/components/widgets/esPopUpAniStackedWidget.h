/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPopUpAniStackedWidget.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESPOPUPANISTACKEDWIDGET_H
#define ESPOPUPANISTACKEDWIDGET_H

#include <qeasingcurve.h>
#include <QStackedWidget>

#include "esGlobal.h"

class QPropertyAnimation;

class EsPopUpAniStackedWidgetPrivate;
/**
 * @brief 带有弹出动画的堆叠小部件
 *
 * 该类是一个堆叠小部件，具有弹出动画功能。它允许将多个小部件堆叠在一起，并且能够在切换小部件时显示弹出动画效果。
 */
class ES_EXPORT EsPopUpAniStackedWidget : public QStackedWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsPopUpAniStackedWidget)
Q_SIGNALS:
    void aniFinished();  ///< 动画完成时发射的信号
    void aniStart();     ///< 动画开始时发射的信号
public:

    explicit EsPopUpAniStackedWidget(QWidget* parent = nullptr);

    ~EsPopUpAniStackedWidget();

    /**
     * @brief 向堆叠小部件中添加一个小部件并设置动画
     *
     * 此方法将一个新的小部件添加到堆叠小部件中，并为该小部件设置弹出动画。动画基于指定的 X 和 Y 偏移量进行。
     *
     * @param widget 要添加的小部件
     * @param deltaX X轴偏移量，用于动画的起始位置和结束位置之间的位移
     * @param deltaY Y轴偏移量，用于动画的起始位置和结束位置之间的位移
     */
    void addWidget(QWidget* widget, int deltaX=0, int deltaY=76);


    /**
     * @brief 从堆叠小部件中移除一个小部件
     *
     * 此方法会从堆叠小部件中移除一个已添加的小部件，并清除与该小部件相关的动画信息。
     * 如果小部件不存在于堆叠小部件中，方法将不会执行任何操作。
     *
     * @param widget 要移除的小部件
     */
    void removeWidget(QWidget* widget);

    /**
     * @brief 设置是否启用弹出动画
     *
     * 此方法设置是否启用弹出动画。如果启用，窗口在切换时会应用动画效果；如果禁用，动画效果将被禁用，窗口直接切换。
     *
     * @param isEnabled 如果为 true，启用动画；如果为 false，禁用动画
     */
    void setAnimationEnabled(bool isEnabled);

    /**
     * @brief 设置当前显示的窗口
     *
     * 此方法设置当前显示的窗口，并根据是否启用动画执行相应的动画效果。可以选择启用弹出动画、设置动画持续时间和插值模式。
     *
     * @param index 要显示的窗口的索引
     * @param needPopOut 是否需要执行弹出动画
     * @param showNextWidgetDirectly 动画开始时是否直接显示下一个窗口
     * @param duration 动画持续时间（毫秒）
     * @param easingCurve 动画的插值模式（默认是 OutQuad）
     */
    void setCurrentIndex(int index, bool needPopOut=false, bool showNextWidgetDirectly=true, int duration=250, QEasingCurve easingCurve=QEasingCurve::OutQuad);

    /**
     * @brief 设置当前显示的窗口
     *
     * 此方法设置当前显示的窗口，并根据是否启用动画执行相应的弹出动画效果。可以选择设置动画持续时间、插值模式等参数。
     *
     * @param widget 要显示的窗口
     * @param needPopOut 是否需要执行弹出动画
     * @param showNextWidgetDirectly 动画开始时是否直接显示下一个窗口
     * @param duration 动画持续时间（毫秒）
     * @param easingCurve 动画的插值模式（默认是 OutQuad）
     */
    void setCurrentWidget(QWidget* widget, bool needPopOut=false, bool showNextWidgetDirectly=true, int duration=250, QEasingCurve easingCurve=QEasingCurve::OutQuad);

    bool isAnimationEnabled() const;

private:
    /**
     * @brief 动画完成时的槽函数
     *
     * 该方法会在动画结束时被调用，执行必要的清理工作，并切换到下一个 widget。
     * 它会断开动画连接并设置当前索引为下一个索引，同时发出动画完成的信号。
     */
    Q_SLOT void onAniFinished();

};
#endif //ESPOPUPANISTACKEDWIDGET_H
