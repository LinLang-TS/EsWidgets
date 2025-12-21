/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListItemDelegate.h
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLISTITEMDELEGATE_H
#define ESLISTITEMDELEGATE_H

#include "esDef.h"
#include "esTableItemDelegate.h"

class ES_EXPORT EsListItemDelegate : public EsTableItemDelegate
{
    Q_OBJECT

public:
    explicit EsListItemDelegate(QObject* parent);

    /**
 * 绘制行背景
 *
 * @param painter 绘制器
 * @param option 视图项选项
 * @param index 视图项的索引
 */
    void _drawBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * 绘制指示器
     *
     * @param painter 绘制器
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    void _drawIndicator(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;


};


#endif //ESLISTITEMDELEGATE_H
