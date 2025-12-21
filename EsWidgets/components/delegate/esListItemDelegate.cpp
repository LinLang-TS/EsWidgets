/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListItemDelegate.cpp
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esListItemDelegate.h"
#include <QPainter>

#include "esIcon.h"

EsListItemDelegate::EsListItemDelegate(QObject* parent)
    :EsTableItemDelegate(parent)
{
}

void EsListItemDelegate::_drawBackground(QPainter* painter, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    painter->drawRoundedRect(option.rect, 5, 5);
}

void EsListItemDelegate::_drawIndicator(QPainter* painter, const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    int y = option.rect.y();
    int h = option.rect.height();
    qreal ph = round(pressedRow == index.row() ? 0.35 * h : 0.257 * h);
    painter->setBrush(EsFunc::autoFallbackThemeColor(lightCheckedColor, darkCheckedColor));
    painter->drawRoundedRect(0, ph + y, 3, h - 2 * ph, 1.5, 1.5);
}
