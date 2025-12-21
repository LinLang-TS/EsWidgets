/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTogglePushButton.cpp
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTogglePushButton.h"

EsTogglePushButton::EsTogglePushButton(QWidget* parent)
    : EsToggleButton(parent)
{
}

EsTogglePushButton::EsTogglePushButton(const QString& text, QWidget* parent)
    : EsToggleButton(text, parent)
{
}

EsTogglePushButton::EsTogglePushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsToggleButton(icon, text, parent)
{
}

EsTogglePushButton::EsTogglePushButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsToggleButton(icon, text, parent)
{
}

EsTogglePushButton::EsTogglePushButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsToggleButton(icon, text, parent)
{
}
