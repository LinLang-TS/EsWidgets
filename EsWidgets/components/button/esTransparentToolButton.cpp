/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTransparentToolButton.cpp
** @date 2025/11/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTransparentToolButton.h"

EsTransparentToolButton::EsTransparentToolButton(QWidget* parent)
    : EsToolButton(parent)
{
}

EsTransparentToolButton::EsTransparentToolButton(Es::IconName icon, QWidget* parent)
    : EsToolButton(icon, parent)
{
}

EsTransparentToolButton::EsTransparentToolButton(EsIconBase* icon, QWidget* parent)
    : EsToolButton(icon, parent)
{
}

EsTransparentToolButton::EsTransparentToolButton(const QString& icon, QWidget* parent)
    : EsToolButton(icon, parent)
{
}

EsTransparentToolButton::EsTransparentToolButton(const QIcon& icon, QWidget* parent)
    : EsToolButton(icon, parent)
{
}
