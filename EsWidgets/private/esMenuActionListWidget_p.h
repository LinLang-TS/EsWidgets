/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esMenuActionListWidget_p.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESMENUACTIONLISTWIDGET_P_H
#define ESMENUACTIONLISTWIDGET_P_H
#include "esDef.h"
class EsSmoothScrollDelegate;
class EsMenuActionListWidget;

class EsMenuActionListWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsMenuActionListWidget);

public:
    explicit EsMenuActionListWidgetPrivate(QObject* parent=nullptr);

    int itemHeight;
    int maxVisibleItems;    // 根据屏幕大小调整可见项目
    EsSmoothScrollDelegate* scrollDelegate;
};



#endif //ESMENUACTIONLISTWIDGET_P_H
