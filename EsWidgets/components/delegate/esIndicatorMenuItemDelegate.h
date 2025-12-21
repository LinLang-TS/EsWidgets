/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIndicatorMenuItemDelegate.h
** @date 2025/12/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESINDICATORMENUITEMDELEGATE_H
#define ESINDICATORMENUITEMDELEGATE_H
#include "esMenuItemDelegate.h"

/**
 * 带指示器的菜单项委托
 */
class ES_EXPORT EsIndicatorMenuItemDelegate : public EsMenuItemDelegate
{
    Q_OBJECT

public:
    explicit EsIndicatorMenuItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif //ESINDICATORMENUITEMDELEGATE_H
