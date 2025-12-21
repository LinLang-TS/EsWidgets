/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esToolButton_p.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTOOLBUTTON_P_H
#define ESTOOLBUTTON_P_H
#include "esDef.h"
class EsToolButton;
class EsToolButtonPrivate:QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsToolButton)
public:
    EsToolButtonPrivate(QObject* parent=nullptr);
    void setIcon(const QVariant& icon);
    QVariant icon;
};



#endif //ESTOOLBUTTON_P_H
