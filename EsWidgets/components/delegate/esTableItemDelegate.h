/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableItemDelegate.h
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTABLEITEMDELEGATE_H
#define ESTABLEITEMDELEGATE_H

#include "esDef.h"
#include <QStyledItemDelegate>
#include <QSet>


class ES_EXPORT EsTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit EsTableItemDelegate(QObject* parent);
    int margin;
    int hoverRow;
    int pressedRow;
    QSet<int> selectedRows;
    QColor lightCheckedColor;
    QColor darkCheckedColor;
    // ItemViewToolTipDelegate* tooltipDelegate;  // todo 后面写到toolTip在实现他

    void setHoverRow(int row);

    void setPressedRow(int row);

    /**
     * 设置选中的行
     *
     * @param indexes 要设置的选中项的模型索引列表
     */
    void setSelectedRows(const QList<QModelIndex>& indexes);

    /**
     * 计算项的推荐大小，增加边框所需的空间
     *
     * @param option 视图项选项
     * @param index 视图项的索引
     * @return 增加边框空间后的大小
     */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * 创建编辑器（QLineEdit）
     *
     * @param parent 父窗口部件
     * @param option 视图项选项
     * @param index 视图项的索引
     * @return 编辑器控件（QLineEdit）
     */
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * 更新编辑器的几何位置
     *
     * @param editor 编辑器控件
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * 设置选中状态下的指示器颜色
     *
     * @param light 浅色模式下的颜色
     * @param dark 深色模式下的颜色
     */
    void setCheckedColor(const QColor& light, const QColor& dark);

    /**
     * 绘制行背景
     *
     * @param painter 绘制器
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    virtual void _drawBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    /**
     * 绘制指示器
     *
     * @param painter 绘制器
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    virtual void _drawIndicator(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    /**
     * 初始化视图项的样式选项
     *
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
    /**
     * 绘制项
     *
     * @param painter 绘制器
     * @param option 视图项选项
     * @param index 视图项的索引
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
   * 绘制复选框
   *
   * @param painter 绘制器
   * @param option 视图项选项
   * @param index 视图项的索引
   */
    void _drawCheckBox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    /**
     * 帮助事件
     *
     * @param event 事件
     * @param view 视图
     * @param option 视图项选项
     * @param index 视图项的索引
     * @return 是否处理了事件
     */
    // todo 帮助事件
    // bool helpEvent(QHelpEvent* event, QAbstractItemView* view, const QStyleOptionViewItem& option, const QModelIndex& index) override {
    //     // 调用tooltipDelegate的helpEvent方法
    //     return tooltipDelegate->helpEvent(event, view, option, index);
    // }

};


#endif //ESTABLEITEMDELEGATE_H
