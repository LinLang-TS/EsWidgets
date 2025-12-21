/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esMenuItemDelegate.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESMENUITEMDELEGATE_H
#define ESMENUITEMDELEGATE_H
#include "esDef.h"
#include <QStyledItemDelegate>

class ES_EXPORT EsMenuItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit EsMenuItemDelegate(QObject* parent = nullptr);

protected:
    /**
     * @brief 判断某项是否为分隔符
     * @param index 模型索引
     * @return 是否为分隔符
     */
    bool isSeparator(const QModelIndex& index) const;

public:
    /**
     * @brief 绘制项
     * @param painter 画笔
     * @param option 绘制参数
     * @param index 模型索引
     */
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    /**
     * @brief 处理帮助事件（Tooltip）
     * @param event 帮助事件
     * @param view 视图
     * @param option 绘制选项
     * @param index 模型索引
     * @return 是否处理成功
     */
    // todo
    // bool helpEvent(QHelpEvent* event,
    //                QAbstractItemView* view,
    //                const QStyleOptionViewItem& option,
    //                const QModelIndex& index) override
    // {
    //     if (!tooltipDelegate)
    //     {
    //         tooltipDelegate = new ItemViewToolTipDelegate(
    //             view, 100, ItemViewToolTipType::LIST);
    //     }
    //
    //     return tooltipDelegate->helpEvent(event, view, option, index);
    // }

private:
    /// todo Tooltip 委托
    // ItemViewToolTipDelegate* tooltipDelegate;
};


/**
 * @class 快捷菜单项委托代理类
 */
class ES_EXPORT EsShortcutMenuItemDelegate : public EsMenuItemDelegate
{
    Q_OBJECT

public:
    explicit EsShortcutMenuItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif //ESMENUITEMDELEGATE_H
