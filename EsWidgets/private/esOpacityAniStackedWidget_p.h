/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esOpacityAniStackedWidget_p.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESOPACITYANISTACKEDWIDGET_P_H
#define ESOPACITYANISTACKEDWIDGET_P_H

#include "esDef.h"

class QPropertyAnimation;
class QGraphicsOpacityEffect;
class EsOpacityAniStackedWidget;

class EsOpacityAniStackedWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsOpacityAniStackedWidget)
    explicit EsOpacityAniStackedWidgetPrivate(QObject* parent = nullptr);


    int nextIndex; ///< 下一索引
    QList<QPropertyAnimation*> animations; ///< 动画列表
    QList<QGraphicsOpacityEffect*> effects; ///< 透明度效果列表
};


#endif //ESOPACITYANISTACKEDWIDGET_P_H
