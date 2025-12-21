/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWindowBase_p.h
** @date 2025/11/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWINDOWBASE_P_H
#define ESWINDOWBASE_P_H

#include "esDef.h"
class EsWindowBase;
class EsStackedWidget;
class EsBackgroundColorObject;
class QPropertyAnimation;

class EsWindowBasePrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsWindowBase)
public:
    explicit EsWindowBasePrivate(QObject* parent=nullptr);


    bool isMicaEnabled;
    QColor lightBackgroundColor;
    QColor darkBackgroundColor;

    QHBoxLayout* hBoxLayout;

};



#endif //ESWINDOWBASE_P_H
