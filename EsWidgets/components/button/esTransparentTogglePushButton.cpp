/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTransparentTogglePushButton.cpp
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTransparentTogglePushButton.h"


EsTransparentTogglePushButton::EsTransparentTogglePushButton(QWidget* parent)
    : EsTogglePushButton(parent)
{
}

EsTransparentTogglePushButton::EsTransparentTogglePushButton(const QString& text, QWidget* parent)
    : EsTogglePushButton(text, parent)
{
}

EsTransparentTogglePushButton::EsTransparentTogglePushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsTogglePushButton(icon, text, parent)
{
}

EsTransparentTogglePushButton::EsTransparentTogglePushButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsTogglePushButton(icon, text, parent)
{
}

EsTransparentTogglePushButton::EsTransparentTogglePushButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsTogglePushButton(icon, text, parent)
{
}
