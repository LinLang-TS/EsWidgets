/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esStackedWidget.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSTACKEDWIDGET_H
#define ESSTACKEDWIDGET_H
#include <QFrame>

#include "esGlobal.h"


class EsStackedWidgetPrivate;

class ES_EXPORT EsStackedWidget : public QFrame
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsStackedWidget)
Q_SIGNALS:
    /**
     * 信号：当前显示的 widget 改变时发出
     * @param index widget 的索引
     */
    void currentChanged(int index);

public:
    explicit EsStackedWidget(QWidget* parent = nullptr);
    ~EsStackedWidget() override;

    /**
     * @brief 获取动画是否启用
     *
     * 检查当前堆叠窗口部件是否启用了动画效果。
     *
     * @return bool
     * 返回是否启用动画的状态。
     */
    bool isAnimationEnabled() const;

    /**
     * @brief 设置是否启用弹出动画
     *
     * 设置当前堆叠窗口部件是否启用弹出动画效果。
     *
     * @param isEnabled 是否启用动画
     */
    void setAnimationEnabled(bool isEnabled);

    /**
     * @brief 向视图中添加一个小部件
     *
     * 将一个新的小部件添加到视图中。
     *
     * @param widget 要添加的小部件
     */
    void addWidget(QWidget* widget);

    /**
     * @brief 从视图中移除一个小部件
     *
     * 将指定的小部件从视图中移除。
     *
     * @param widget 要移除的小部件
     */
    void removeWidget(QWidget* widget);

    /**
     * @brief 获取指定索引的小部件
     *
     * 获取指定索引的视图中的小部件。
     *
     * @param index 索引
     * @return 返回指定索引的小部件
     */
    QWidget* widget(int index) const;

    /**
     * @brief 设置当前显示的小部件
     *
     * 设置当前显示的小部件，并根据需要执行弹出动画。
     *
     * @param widget 要显示的小部件
     * @param popOut 是否启用弹出动画
     */
    void setCurrentWidget(QWidget* widget, bool popOut=true);

    /**
     * @brief 设置当前显示的索引
     *
     * 设置显示的当前小部件，基于给定的索引，并可选择是否启用弹出动画。
     *
     * @param index 要显示的小部件的索引
     * @param popOut 是否启用弹出动画
     */
    void setCurrentIndex(int index, bool popOut=true);

    /**
     * @brief 获取当前显示的小部件的索引
     * 
     * 获取当前显示的小部件在堆叠小部件中的索引。
     * 
     * @return 当前小部件的索引
     */
    int currentIndex() const;

    /**
     * @brief 获取当前显示的小部件
     * 
     * 获取当前显示的小部件。
     * 
     * @return 当前小部件
     */
    QWidget* currentWidget() const;

    /**
     * @brief 获取小部件的索引
     * 
     * 获取给定小部件在堆叠小部件中的索引。
     * 
     * @param widget 要查找的 widget
     * @return widget 的索引
     */
    int indexOf(QWidget* widget) const;

    /**
     * @brief 获取堆叠小部件中的小部件数量
     * 
     * 获取当前堆叠小部件中的小部件数量。
     * 
     * @return 小部件数量
     */
    int count() const;
};


#endif //ESSTACKEDWIDGET_H
