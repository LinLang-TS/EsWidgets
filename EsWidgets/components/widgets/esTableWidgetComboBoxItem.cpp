/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableWidgetComboBoxItem.cpp
** @date 2025/12/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTableWidgetComboBoxItem.h"

#include "esDef.h"

EsTableWidgetComboBoxItem::EsTableWidgetComboBoxItem()
{
    setComboBoxData(TableWidgetComboBoxItemData());
}

EsTableWidgetComboBoxItem::EsTableWidgetComboBoxItem(const QStringList& options, const QString& defaultText)
    : QTableWidgetItem(defaultText)
{
    TableWidgetComboBoxItemData cfg;
    cfg.options = options;
    cfg.defaultText = defaultText;

    if (!defaultText.isEmpty())
    {
        setText(defaultText);
        cfg.currentIndex = options.indexOf(defaultText);
    }

    setComboBoxData(cfg);
}

void EsTableWidgetComboBoxItem::setOptions(const QStringList& options, const QString& defaultText)
{
    TableWidgetComboBoxItemData cfg = comboBoxData();

    cfg.options = options;

    if (!defaultText.isEmpty())
    {
        cfg.defaultText = defaultText;
        cfg.currentIndex = options.indexOf(defaultText);
        setText(defaultText);
    }
    else
    {
        // 如果当前文本仍然合法，保持当前选中
        const int idx = options.indexOf(text());
        cfg.currentIndex = idx;
        if (idx < 0)
        {
            cfg.currentIndex = -1;
            setText(QString());
        }
    }

    setComboBoxData(cfg);
}

QStringList EsTableWidgetComboBoxItem::options() const
{
    return comboBoxData().options;
}

QString EsTableWidgetComboBoxItem::currentText() const
{
    return text(); // 由于选中comboBox的选项后，会把数据更新到单元格上，所以直接返回文本
}

int EsTableWidgetComboBoxItem::currentIndex() const
{
    const TableWidgetComboBoxItemData& cfg = comboBoxData();
    if (cfg.currentIndex >= 0)
    {
        return cfg.currentIndex;
    }
    return cfg.options.indexOf(text());
}

void EsTableWidgetComboBoxItem::disableOption(int index)
{
    if (index < 0)
        return;

    TableWidgetComboBoxItemData cfg = comboBoxData();
    cfg.disabledIndexes.insert(index);
    setComboBoxData(cfg);
}

QSet<int> EsTableWidgetComboBoxItem::disabledOptions() const
{
    return comboBoxData().disabledIndexes;
}

// 设置ComboBox为可编辑
void EsTableWidgetComboBoxItem::setEditable(bool editable)
{
    TableWidgetComboBoxItemData data = comboBoxData();
    data.editable = editable;
    setComboBoxData(data);
}

// 设置最大显示项数
void EsTableWidgetComboBoxItem::setMaxVisibleItems(int count)
{
    TableWidgetComboBoxItemData data = comboBoxData();
    data.maxVisibleItems = count;
    setComboBoxData(data);
}

// 设置占位符文本
void EsTableWidgetComboBoxItem::setPlaceholderText(const QString& text)
{
    TableWidgetComboBoxItemData data = comboBoxData();
    data.placeholderText = text;
    setComboBoxData(data);
}

// 设置ComboBox的配置数据
void EsTableWidgetComboBoxItem::setComboBoxData(const TableWidgetComboBoxItemData& data)
{
    setData(Es::ItemDataRole::ComboOptionsRole, QVariant::fromValue(data));  // 将数据存储到QTableWidgetItem中
}

// 获取ComboBox的配置数据
TableWidgetComboBoxItemData EsTableWidgetComboBoxItem::comboBoxData() const
{
    QVariant var = data(Es::ItemDataRole::ComboOptionsRole);
    if (!var.isValid())
    {
        return {};
    }

    return var.value<TableWidgetComboBoxItemData>();
}