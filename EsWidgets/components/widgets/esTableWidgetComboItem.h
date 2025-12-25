/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableWidgetComboItem.h
** @date 2025/12/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWIDGETS_ESTABLEWIDGETCOMBOBOXITEM_H
#define ESWIDGETS_ESTABLEWIDGETCOMBOBOXITEM_H

#include <QTableWidgetItem>

struct EsTableWidgetComboItemData
{
    QStringList options;         // 存储 ComboBox 选项
    QSet<int> disabledIndexes;   // 存储禁用的选项索引
    int maxVisibleItems = -1;    // 最大显示项数
    bool editable = false;       // 是否允许用户输入自定义文本
    int currentIndex = -1;       // 当前选中选项索引
    QString placeholderText;     // 可编辑时的提示文本
};
Q_DECLARE_METATYPE(EsTableWidgetComboItemData)

class EsTableWidgetComboItem  : public QTableWidgetItem
{
public:
    EsTableWidgetComboItem();
    EsTableWidgetComboItem(const QStringList& options = {}, const QString& defaultText = nullptr);

    /// 设置ComboBox的选项和默认文本
    void setOptions(const QStringList& options, const QString& defaultText = nullptr);

    /// 获取ComboBox的所有选项
    QStringList options() const;

    /// 获取当前选中的文本
    QString currentText() const;

    /// 获取当前选中的索引，如果没有选中，则返回-1
    int currentIndex() const;

    /// 禁用某个选项
    void disableOption(int index);

    /// 获取所有已禁用的选项索引
    QSet<int> disabledOptions() const;

    /// 设置ComboBox为可编辑
    void setEditable(bool editable);

    /// 设置最大显示项数
    void setMaxVisibleItems(int count);

    /// 设置可编辑时的提示文本
    void setPlaceholderText(const QString& text);
private:
    // 设置或获取ComboBox的配置
    void setComboBoxData(const EsTableWidgetComboItemData& data);
    EsTableWidgetComboItemData comboBoxData() const;
};




#endif //ESWIDGETS_ESTABLEWIDGETCOMBOBOXITEM_H