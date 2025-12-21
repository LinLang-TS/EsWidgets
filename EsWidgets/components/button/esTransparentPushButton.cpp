/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTransparentPushButton.cpp
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTransparentPushButton.h"

EsTransparentPushButton::EsTransparentPushButton(QWidget* parent)
    : EsPushButton(parent)
{
}


EsTransparentPushButton::EsTransparentPushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}

EsTransparentPushButton::EsTransparentPushButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}

EsTransparentPushButton::EsTransparentPushButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}
