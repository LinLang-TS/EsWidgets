/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esFramelessWindow_p.h
** @date 2025/10/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESFRAMELESSWINDOW_P_H
#define ESFRAMELESSWINDOW_P_H
#include "esDef.h"

class EsFramelessWindow;
class EsAcrylicWindow;

class EsFramelessWindowPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsFramelessWindow)

public:
    explicit EsFramelessWindowPrivate(QObject* parent = nullptr);

    Q_SLOT void onScreenChanged();

    bool isSystemButtonVisible;
    bool isResizeEnabled;
    int borderWidth;
};


class EsAcrylicWindowPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsAcrylicWindow)

public:
    explicit EsAcrylicWindowPrivate(QObject* parent = nullptr);
    bool closedByKey;

};

#endif //ESFRAMELESSWINDOW_P_H
