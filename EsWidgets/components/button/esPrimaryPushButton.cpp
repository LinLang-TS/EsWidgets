/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPrimaryPushButton.cpp
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/
#include "esPrimaryPushButton.h"

#include <QPainter>

#include "esConfig.h"


EsPrimaryPushButton::EsPrimaryPushButton(QWidget* parent)
    : EsPushButton(parent)
{
}

EsPrimaryPushButton::EsPrimaryPushButton(const QString& text, QWidget* parent)
    : EsPushButton(text, parent)
{
}

EsPrimaryPushButton::EsPrimaryPushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}

EsPrimaryPushButton::EsPrimaryPushButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}

EsPrimaryPushButton::EsPrimaryPushButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
}

void EsPrimaryPushButton::_drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state)
{
    QVariant variant;
    if (icon.userType() == qMetaTypeId<EsIconBase*>() && isEnabled())
    {
        // 反转图标颜色
        auto theme =  !EsFunc::isDarkTheme() ? Es::Theme::Theme_DARK : Es::Theme::Theme_LIGHT;
        variant = QVariant::fromValue(icon.value<EsIconBase*>()->icon(theme));
        EsPushButton::_drawIcon(variant, painter, rect, state);
        return;
    }
    else if (!isEnabled())
    {
        painter->setOpacity(EsFunc::isDarkTheme() ? 0.786 : 0.9);
        if (icon.userType() == qMetaTypeId<EsIconBase*>())
        {
            variant = QVariant::fromValue(icon.value<EsIconBase*>()->icon(Es::Theme::Theme_DARK));
            EsPushButton::_drawIcon(variant, painter, rect, state);
            return;
        }
    }
    EsPushButton::_drawIcon(icon, painter, rect, state);
}
