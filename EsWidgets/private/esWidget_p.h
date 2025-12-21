/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWidget_p.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESWIDGET_P_H
#define ES_ESWIDGET_P_H
#include "esDef.h"

class EsWidget;
class EsWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsWidget)

public:
    explicit EsWidgetPrivate(QObject* parent = nullptr);

    bool isHoverMaxBtn();


};


#endif //ES_ESWIDGET_P_H
