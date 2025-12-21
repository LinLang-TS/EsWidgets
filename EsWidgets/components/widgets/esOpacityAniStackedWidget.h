/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esOpacityAniStackedWidget.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESOPACITYANISTACKEDWIDGET_H
#define ES_ESOPACITYANISTACKEDWIDGET_H


#include <qeasingcurve.h>
#include <QStackedWidget>

#include "esGlobal.h"

class QPropertyAnimation;
class EsOpacityAniStackedWidgetPrivate;

class ES_EXPORT EsOpacityAniStackedWidget : public QStackedWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsOpacityAniStackedWidget)

public:
    explicit EsOpacityAniStackedWidget(QWidget* parent = nullptr);
    ~EsOpacityAniStackedWidget();

    /**
    * @brief 添加一个小部件并为其设置动画
    *
    * 此函数将一个小部件添加到堆叠小部件中，并为该小部件设置淡入淡出动画。
    *
    * @param w 需要添加的小部件
    */
    void addWidget(QWidget* w);


    /**
     * @brief 设置当前显示的索引并执行淡入淡出动画
     *
     * 该函数设置堆叠小部件的当前索引，并根据需要执行淡入或淡出动画。
     * 如果目标索引大于当前索引，则执行淡入动画；如果目标索引小于当前索引，则执行淡出动画。
     *
     * @param index 新的索引
     */
    void setCurrentIndex(int index);

    /**
     * @brief 设置当前显示的小部件
     *
     * 该函数设置堆叠小部件中显示的小部件，依据小部件的索引来更新当前显示的内容。
     * 它通过调用 `indexOf` 来获取小部件的索引，并使用 `setCurrentIndex` 来切换显示。
     *
     * @param w 需要设置为当前显示的小部件
     */
    void setCurrentWidget(QWidget* w);

private:
    Q_SLOT void onAniFinished();
};



#endif //ES_ESOPACITYANISTACKEDWIDGET_H
