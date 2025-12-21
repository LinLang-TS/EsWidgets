/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPushButton_p.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESPUSHBUTTON_P_H
#define ES_ESPUSHBUTTON_P_H
#include "esDef.h"
class EsIconBase;
class EsPushButton;

class EsPushButtonPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsPushButton);

public:
    explicit EsPushButtonPrivate(QObject* parent = nullptr);

    void setIcon(const QVariant& icon);
    QVariant icon;

};

#endif //ES_ESPUSHBUTTON_P_H
