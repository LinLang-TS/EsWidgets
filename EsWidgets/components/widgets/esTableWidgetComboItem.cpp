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



void EsTableWidgetComboItem::addOption(const QString& text, const QVariant& icon, const QVariant& userData)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    auto item = new EsComboItem(text, icon, userData);
    cfg.options.append(item);

    if (cfg.options.size() == 1)
    {
        setCurrentIndex(0);
    }
    setComboBoxData(cfg);
}

void EsTableWidgetComboItem::addOptions(const QStringList& texts)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    for (const auto& text : texts) {
        addOption(text);
    }
}

void EsTableWidgetComboItem::removeOption(int index)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index < 0 || index >= cfg.options.size()) {
        return;
    }

    delete cfg.options.takeAt(index);

    if (index < currentIndex())
    {
        setCurrentIndex(currentIndex() - 1);
    }
    else if (index == currentIndex())
    {
        if (index > 0)
        {
            setCurrentIndex(currentIndex() - 1);
        }
        else
        {
            setText(optionText(0));
            // emit currentTextChanged(currentText());
            // emit currentIndexChanged(0);
        }
    }
    if (count() == 0) clear();
    setComboBoxData(cfg);
}

int EsTableWidgetComboItem::currentIndex() const
{
    const EsTableWidgetComboItemData& cfg = comboBoxData();
    return cfg.currentIndex;
}

void EsTableWidgetComboItem::setCurrentIndex(int index)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index >= count() || index == currentIndex()) return;

    if (index < 0)
    {
        cfg.currentIndex = -1;
        setText("");
        setPlaceholderText(cfg.placeholderText);
    }
    else
    {
        cfg.currentIndex = index;
        setText(cfg.options[index]->text);
    }
    setComboBoxData(cfg);
}


QString EsTableWidgetComboItem::currentText() const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (currentIndex() < 0 || currentIndex() >= cfg.options.size()) {
        return {};
    }
    return cfg.options[currentIndex()]->text;
}



QVariant EsTableWidgetComboItem::currentData() const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (!(currentIndex() >= 0 && currentIndex() < cfg.options.size())) {
        return {};
    }

    return cfg.options[currentIndex()]->userData;
}

void EsTableWidgetComboItem::setCurrentText(const QString& text)
{
    if (text == currentText())
    {
        return;
    }

    int index = findText(text);
    if (index >= 0)
    {
        setCurrentIndex(index);
    }
}

void EsTableWidgetComboItem::setOptionText(int index, const QString& text)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index < 0 || index >= cfg.options.size())
    {
        return;
    }

    cfg.options[index]->text = text;
    if (currentIndex() == index)
    {
        setText(text);
    }
    setComboBoxData(cfg);
}

QVariant EsTableWidgetComboItem::optionData(int index) const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index < 0 || index >= cfg.options.size())
    {
        return {};
    }

    return cfg.options[index]->userData;
}

QString EsTableWidgetComboItem::optionText(int index) const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index < 0 || index >= cfg.options.size())
    {
        return QString();
    }

    return cfg.options[index]->text;
}

QIcon EsTableWidgetComboItem::optionIcon(int index) const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index < 0 || index >= cfg.options.size())
    {
        return QIcon();
    }

    return cfg.options[index]->getIcon();
}

void EsTableWidgetComboItem::setOptionData(int index, const QVariant& value)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index >= 0 && index < cfg.options.size())
    {
        cfg.options[index]->userData = value;
    }
    setComboBoxData(cfg);
}
void EsTableWidgetComboItem::setOptionIcon(int index, const QIcon& icon)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index >= 0 && index < cfg.options.size())
    {
        cfg.options[index]->setIcon(icon);
    }
    setComboBoxData(cfg);
}

void EsTableWidgetComboItem::setOptionEnabled(int index, bool isEnabled)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (index >= 0 && index < cfg.options.size())
    {
        cfg.options[index]->isEnabled = isEnabled;
    }
    setComboBoxData(cfg);
}

int EsTableWidgetComboItem::findData(const QVariant& data) const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    for (int i = 0; i < cfg.options.size(); ++i)
    {
        if (cfg.options[i]->userData == data)
        {
            return i;
        }
    }
    return -1;
}

int EsTableWidgetComboItem::findText(const QString& text) const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    for (int i = 0; i < cfg.options.size(); ++i)
    {
        if (cfg.options[i]->text == text)
        {
            return i;
        }
    }
    return -1;
}

void EsTableWidgetComboItem::clear()
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    if (currentIndex() >= 0)
    {
        setText("");
    }
    while (!cfg.options.isEmpty()) {
        delete cfg.options.takeLast();
    }
    cfg.currentIndex = -1;
    setComboBoxData(cfg);
}

int EsTableWidgetComboItem::count() const
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    return cfg.options.size();
}



void EsTableWidgetComboItem::insertOption(int index, const QString& text, const QVariant& icon, const QVariant& userData)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    auto item = new EsComboItem(text, icon, userData);
    cfg.options.insert(index, item);
    setComboBoxData(cfg);

    if (index <= currentIndex())
    {
        setCurrentIndex(currentIndex() + 1);
    }
}

void EsTableWidgetComboItem::insertOptions(int index, const QList<QString>& texts)
{
    EsTableWidgetComboItemData cfg = comboBoxData();
    int pos = index;
    for (const auto& text : texts)
    {
        auto item = new EsComboItem(text);
        cfg.options.insert(cfg.options.begin() + pos, item);
        ++pos;
    }
    setComboBoxData(cfg);

    if (index <= currentIndex())
    {
        setCurrentIndex(currentIndex() + pos - index);
    }
}



// 设置最大显示项数
void EsTableWidgetComboItem::setMaxVisibleOptions(int count)
{
    EsTableWidgetComboItemData data = comboBoxData();
    data.maxVisibleItems = count;
    setComboBoxData(data);
}

// 最大显示项数
int EsTableWidgetComboItem::maxVisibleOptions() const
{
    const EsTableWidgetComboItemData& data = comboBoxData();
    return data.maxVisibleItems;
}


// 设置ComboBox为可编辑
void EsTableWidgetComboItem::setEditableEnabled(bool isEnabled)
{
    EsTableWidgetComboItemData data = comboBoxData();
    data.editable = isEnabled;
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