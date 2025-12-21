/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLineEditButton_p.h
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLINEEDITBUTTON_P_H
#define ESLINEEDITBUTTON_P_H

#include "esDef.h"
#include <QAction>
#include <QPushButton>

class EsLineEditButton;

class EsLineEditButtonPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsLineEditButton)

public:
    explicit EsLineEditButtonPrivate(QObject *parent=nullptr);
    QVariant icon;
    QAction* action;
    bool isPressed;

    /**
     * 响应动作变化
     */
    Q_SLOT void onActionChanged();
};



#endif //ESLINEEDITBUTTON_P_H
