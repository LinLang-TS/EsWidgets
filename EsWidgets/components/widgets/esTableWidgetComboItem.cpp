/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableWidgetComboItem.cpp
** @date 2025/12/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTableWidgetComboItem.h"

#include "esDef.h"

EsTableWidgetComboItem::EsTableWidgetComboItem()
{
    setComboBoxData(EsTableWidgetComboItemData());
}

EsTableWidgetComboItem::EsTableWidgetComboItem(const QStringList& options, const QString& defaultText)
    : QTableWidgetItem(defaultText)
{
    EsTableWidgetComboItemData cfg;
    cfg.options = options;

    if (!defaultText.isEmpty())
    {
        setText(defaultText);
        cfg.currentIndex = options.indexOf(defaultText);
    }

    setComboBoxData(cfg);
}

void EsTableWidgetComboItem::setOptions(const QStringList& options, const QString& defaultText)
{
    EsTableWidgetComboItemData cfg = comboBoxData();

    cfg.options = options;

    if (!defaultText.isEmpty())
    {
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

QStringList EsTableWidgetComboItem::options() const
{
    return comboBoxData().options;
}

QString EsTableWidgetComboItem::currentText() const
{
    return text(); // 由于选中comboBox的选项后，会把数据更新到单元格上，所以直接返回文本
}

int EsTableWidgetComboItem::currentIndex() const
{
    const EsTableWidgetComboItemData& cfg = comboBoxData();
    if (cfg.currentIndex >= 0)
    {
        return cfg.currentIndex;
    }
    return cfg.options.indexOf(text());
}

void EsTableWidgetComboItem::disableOption(int index)
{
    if (index < 0)
        return;

    EsTableWidgetComboItemData cfg = comboBoxData();
    cfg.disabledIndexes.insert(index);
    setComboBoxData(cfg);
}

QSet<int> EsTableWidgetComboItem::disabledOptions() const
{
    return comboBoxData().disabledIndexes;
}

// 设置ComboBox为可编辑
void EsTableWidgetComboItem::setEditable(bool editable)
{
    EsTableWidgetComboItemData data = comboBoxData();
    data.editable = editable;
    setComboBoxData(data);
}

// 设置最大显示项数
void EsTableWidgetComboItem::setMaxVisibleItems(int count)
{
    EsTableWidgetComboItemData data = comboBoxData();
    data.maxVisibleItems = count;
    setComboBoxData(data);
}

// 设置占位符文本
void EsTableWidgetComboItem::setPlaceholderText(const QString& text)
{
    EsTableWidgetComboItemData data = comboBoxData();
    data.placeholderText = text;
    setComboBoxData(data);
}

// 设置ComboBox的配置数据
void EsTableWidgetComboItem::setComboBoxData(const EsTableWidgetComboItemData& data)
{
    setData(Es::ItemDataRole::ComboOptionsRole, QVariant::fromValue(data));  // 将数据存储到QTableWidgetItem中
}

// 获取ComboBox的配置数据
EsTableWidgetComboItemData EsTableWidgetComboItem::comboBoxData() const
{
    QVariant var = data(Es::ItemDataRole::ComboOptionsRole);
    if (!var.isValid())
    {
        return {};
    }

    return var.value<EsTableWidgetComboItemData>();
}