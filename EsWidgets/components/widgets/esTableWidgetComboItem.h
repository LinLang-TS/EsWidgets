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

#include "esComboBox.h"

struct EsTableWidgetComboItemData
{
    QList<EsComboItem*> options;  // 存储 ComboBox 选项
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

    /**
     * @brief 添加一项
     *
     * @param text 项目文本
     * @param icon 项目图标，可以为 QString、QIcon 或 EsIconBase
     * @param userData 用户数据
     */
    void addOption(const QString& text, const QVariant& icon = {}, const QVariant& userData = {});

    /**
     * @brief 批量添加项目
     *
     * @param texts 字符串可迭代对象，每个元素为项目文本
     */
    void addOptions(const QStringList& texts);


    /**
     * @brief 移除指定索引的项目，并在必要时更新当前索引
     *
     * @param index 要移除的项目索引
     */
    void removeOption(int index);

    /// 当前combo选中的索引
    int currentIndex() const;

    /**
     * @brief 设置当前索引
     *
     * 设置组件的当前索引。如果传入的索引超出范围，或与当前索引相同，
     * 则本函数不会执行任何操作。
     *
     * @param index 当前索引值
     */
    void setCurrentIndex(int index);

    /**
     * @brief 获取当前条目的文本
     *
     * 如果当前索引无效（越界），则返回空字符串；
     * 否则返回当前索引对应条目的文本内容。
     *
     * @return 当前文本
     */
    QString currentText() const;

    /**
     * @brief 获取当前条目的用户数据
     *
     * 若当前索引无效（越界），则返回 nullptr；
     * 否则返回当前索引对应条目的 userData。
     *
     * @return 当前条目的用户数据
     */
    QVariant currentData() const;

    /**
     * @brief 设置当前显示的文本（需存在于条目列表中）
     *
     * 若传入的文本与当前文本相同，则不进行任何操作。
     * 若文本在条目列表中存在，则切换到对应的索引。
     *
     * @param text 要在组合框中显示的文本
     */
    void setCurrentText(const QString& text);

    /**
     * @brief 设置指定索引条目的文本
     *
     * @param index 条目索引
     * @param text 新的文本内容
     */
    void setOptionText(int index, const QString& text);

    /**
     * @brief 获取指定索引条目的用户数据
     *
     * @param index 条目索引
     * @return QVariant 对应的用户数据，如果索引非法则返回空 QVariant
     */
    QVariant optionData(int index) const;

    /**
     * @brief 获取指定索引条目的文本
     *
     * @param index 条目索引
     * @return QString 对应的文本，如果索引非法则返回空字符串
     */
    QString optionText(int index) const;

    /**
     * @brief 获取指定索引条目的图标
     *
     * @param index 条目索引
     * @return QIcon 对应的图标，如果索引非法则返回空图标
     */
    QIcon optionIcon(int index) const;

    /**
     * @brief 设置指定索引条目的用户数据
     *
     * @param index 条目索引
     * @param value 用户数据
     */
    void setOptionData(int index, const QVariant& value);

    /**
     * @brief 设置指定索引条目的图标
     *
     * @param index 条目索引
     * @param icon 条目图标，可为 QIcon 或自定义图标类型
     */
    void setOptionIcon(int index, const QIcon& icon);

    /**
     * @brief 设置指定索引条目的可用状态
     *
     * @param index 条目索引
     * @param isEnabled 是否可用
     */
    void setOptionEnabled(int index, bool isEnabled);

    /**
     * @brief 查找指定数据对应的条目索引
     *
     * @param data 要查找的数据
     * @return 条目索引，未找到返回 -1
     */
    int findData(const QVariant& data) const;

    /**
     * @brief 查找指定文本对应的条目索引
     *
     * @param text 要查找的文本
     * @return 条目索引，未找到返回 -1
     */
    int findText(const QString& text) const;

    /**
     * @brief 清空组合框，移除所有条目
     */
    void clear();

    /**
     * @brief 返回组合框中的条目数量
     * @return 条目数量
     */
    int count() const;

    /**
     * @brief 在指定索引处插入条目
     *
     * @param index 插入位置
     * @param text 条目文本
     * @param icon 条目图标，可以是 QString, QIcon 或 EsIconBase
     * @param userData 用户自定义数据
     */
    void insertOption(int index, const QString& text, const QVariant& icon = QVariant(),
                           const QVariant& userData = QVariant());

    /**
     * @brief 从指定索引开始插入多个条目
     *
     * @param index 插入起始位置
     * @param texts 条目文本列表
     */
    void insertOptions(int index, const QList<QString>& texts);

    /**
     * @brief 设置下拉框最大可见条目数
     *
     * @param num 最大可见条目数
     */
    void setMaxVisibleOptions(int num);

    /**
     * @brief 获取下拉框最大可见条目数
     * @return int 最大可见条目数
     */
    int maxVisibleOptions() const;

    /// 设置ComboBox为可编辑
    void setEditableEnabled(bool isEnabled);


    /// 设置可编辑时的提示文本
    void setPlaceholderText(const QString& text);
private:
    // 设置或获取ComboBox的配置
    void setComboBoxData(const EsTableWidgetComboItemData& data);
    EsTableWidgetComboItemData comboBoxData() const;
};




#endif //ESWIDGETS_ESTABLEWIDGETCOMBOBOXITEM_H