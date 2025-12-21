/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIndicatorMenuItemDelegate.cpp
** @date 2025/12/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esIndicatorMenuItemDelegate.h"

#include <QPainter>

#include "esStyleSheet.h"

EsIndicatorMenuItemDelegate::EsIndicatorMenuItemDelegate(QObject* parent)
    : EsMenuItemDelegate(parent)
{
}

void EsIndicatorMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    EsMenuItemDelegate::paint(painter, option, index);
    if (!(option.state & QStyle::State_Selected)) return;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    painter->setPen(Qt::NoPen);
    painter->setBrush(EsFunc::themeColor());
    painter->drawRoundedRect(6, 11 + option.rect.y(), 3, 15, 1.5, 1.5);

    painter->restore();
}
