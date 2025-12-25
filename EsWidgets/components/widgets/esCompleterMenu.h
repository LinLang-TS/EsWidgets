/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esCompleterMenu.h
** @date 2025/12/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWIDGETS_ESCOMPLETERMENU_H
#define ESWIDGETS_ESCOMPLETERMENU_H
#include "esDef.h"
#include "esRoundMenu.h"


class EsLineEdit;

/**
 * @brief EsCompleterMenu 类用于显示补全菜单，继承自 RoundMenu
 *
 * 该菜单用于显示并选择补全项，支持激活项并通过信号通知外部。
 */
class EsCompleterMenu : public EsRoundMenu
{
    Q_OBJECT

Q_SIGNALS:
    void activated(const QString& text);
    void indexActivated(const QModelIndex& index);

public:
    /**
     * @brief 构造函数
     *
     * @param lineEdit 关联的 LineEdit 控件
     */
    explicit EsCompleterMenu(EsLineEdit* lineEdit, QWidget* parent = nullptr);

    /**
     * @brief 设置补全模型
     *
     * 该函数用于设置补全菜单的模型，并根据模型更新菜单项。
     *
     * @param model 用于补全的模型，通常是 QAbstractItemModel 的子类
     * @param column 用于补全的列，默认为第 0 列
     * @return 如果模型中的项没有变化则返回 false，否则返回 true
     */
    bool setCompletion(QAbstractItemModel* model, int column = 0);


    /**
     * @brief 设置补全项
     *
     * 该函数用于设置补全菜单的项，并更新视图。
     *
     * @param items 补全项的列表
     */
    void setItems(const QStringList& items);

    /**
     * @brief 事件过滤器
     *
     * 处理按键事件，判断按键类型并根据不同的按键执行不同的操作，
     * 比如将输入重定向到 LineEdit 或者关闭菜单等。
     *
     * @param obj 事件源
     * @param e 事件对象
     * @return 返回父类的事件处理结果
     */
    bool eventFilter(QObject* obj, QEvent* e) override;

    /**
     * @brief 处理选中的补全项
     *
     * 当一个补全项被选中时，更新 LineEdit 的文本并发出信号通知外部。
     * 如果选中的项在索引范围内，还会发出索引变化的信号。
     *
     * @param text 选中项的文本
     * @param row 选中项在视图中的行号
     */
    void _onCompletionItemSelected(const QString& text, int row);

    /**
     * @brief 显示补全菜单
     *
     * 该方法展示补全菜单，调整菜单的大小，并根据输入框的位置确定动画类型。
     * 根据视图的高度来选择合适的动画类型，确保菜单显示的平滑过渡。
     */
    void popup();

protected:
    /**
     * @brief 处理补全项点击事件
     *
     * 当用户点击某个补全项时，调用该函数来隐藏菜单并触发项选择事件。
     *
     * @param item 被点击的补全项
     */
    void _onItemClicked(QListWidgetItem* item) override;

private:
    /**
     * @brief 存储菜单项列表
     */
    QStringList items;

    /**
     * @brief 存储菜单项索引
     */
    QList<QModelIndex> indexes;

    /**
     * @brief 关联的 LineEdit 控件
     */
    EsLineEdit* lineEdit;
};


#endif //ESWIDGETS_ESCOMPLETERMENU_H
