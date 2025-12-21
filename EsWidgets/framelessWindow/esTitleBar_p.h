/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleBarPrivate.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTITLEBARPRIVATE_H
#define ESTITLEBARPRIVATE_H
#include "esDef.h"
#include "esWin32Utils.h"
class EsTitleBarBase;

class EsTitleBarBasePrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsTitleBarBase)

public:
    EsTitleBarBasePrivate(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    // 切换最大状态
    Q_SLOT void toggleMaxState();

    // 检查该位置是否属于允许拖动的区域
    bool isDragRegion(QPoint pos);

    // 检查是否有标题栏按钮被按下
    bool hasButtonPressed();

public:

    bool isDoubleClickEnabled;
    QHBoxLayout* hBoxLayout;
};





#endif //ESTITLEBARPRIVATE_H
