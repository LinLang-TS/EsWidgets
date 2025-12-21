/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esStackedWidget_p.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSTACKEDWIDGET_P_H
#define ESSTACKEDWIDGET_P_H
#include "esDef.h"
class EsStackedWidget;
class EsPopUpAniStackedWidget;

class EsStackedWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsStackedWidget)

public:
    explicit EsStackedWidgetPrivate(QObject* parent = nullptr);

    QHBoxLayout* hBoxLayout; ///< 水平布局管理器
    EsPopUpAniStackedWidget* view; ///< 弹出动画堆叠窗口部件
};


#endif //ESSTACKEDWIDGET_P_H
