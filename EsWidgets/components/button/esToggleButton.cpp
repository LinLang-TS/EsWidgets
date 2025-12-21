/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esToggleButton.cpp
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esToggleButton.h"

#include <QPainter>

#include "esConfig.h"

EsToggleButton::EsToggleButton(QWidget* parent)
    : EsPushButton(parent)
{
    _postInit();
}

EsToggleButton::EsToggleButton(const QString& text, QWidget* parent)
    : EsPushButton(text, parent)
{
    _postInit();
}

EsToggleButton::EsToggleButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
    _postInit();
}

EsToggleButton::EsToggleButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
    _postInit();
}

EsToggleButton::EsToggleButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsPushButton(icon, text, parent)
{
    _postInit();
}

void EsToggleButton::_postInit()
{
    setCheckable(true);
    setChecked(false);
}

void EsToggleButton::_drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state)
{
    if (!isChecked())
        return EsPushButton::_drawIcon(icon, painter, rect);


    //  下面的是对应PrimaryPushButton._drawIcon(self, icon, painter, rect, QIcon.On), 直接copy的EsPrimaryPushButton, 没办法像py那样直接调用
    QVariant variant;
    if (icon.userType() == qMetaTypeId<EsIconBase*>() && isEnabled())
    {
        // 反转图标颜色
        auto theme =  !EsFunc::isDarkTheme() ? Es::Theme::Theme_DARK : Es::Theme::Theme_LIGHT;
        variant = QVariant::fromValue(icon.value<EsIconBase*>()->icon(theme));
        EsPushButton::_drawIcon(variant, painter, rect, QIcon::On);
        return;
    }
    else if (!isEnabled())
    {
        painter->setOpacity(EsFunc::isDarkTheme() ? 0.786 : 0.9);
        if (icon.userType() == qMetaTypeId<EsIconBase*>())
        {
            variant = QVariant::fromValue(icon.value<EsIconBase*>()->icon(Es::Theme::Theme_DARK));
            EsPushButton::_drawIcon(variant, painter, rect, QIcon::On);
            return;
        }
    }
    EsPushButton::_drawIcon(icon, painter, rect, state);

}
