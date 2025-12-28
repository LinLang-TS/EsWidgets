/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esEditableComboBox.cpp
** @date 2025/12/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esEditableComboBox.h"

#include <QApplication>
#include <QLayout>
#include <QPainter>

#include "esAnimation.h"
#include "esComboBox.h"
#include "esCompleterMenu.h"
#include "esFont.h"
#include "esIcon.h"
#include "esMenuActionListWidget.h"
#include "esRoundMenu.h"
#include "esStyleSheet.h"
#include "private/esEditableComboBox_p.h"
#include "subButton/esLineEditButton.h"

// region ================= EsEditableComboBoxPrivate  =================

EsEditableComboBoxPrivate::EsEditableComboBoxPrivate(QObject* parent)
    : QObject(parent),
      isHover(false),
      isPressed(false),
      items(),
      currentIndex(-1),
      maxVisibleItems(-1),
      placeholderText("")

{
}

void EsEditableComboBoxPrivate::updateTextState(bool isPlaceholder)
{
    Q_Q(EsEditableComboBox);

    if (q->property("isPlaceholderText").toBool() == isPlaceholder) return;

    q->setProperty("isPlaceholderText", isPlaceholder);
    q->setStyle(QApplication::style());
}

// endregion

// region ================= EsEditableComboBox  =================

EsEditableComboBox::EsEditableComboBox(QWidget* parent)
    : EsLineEdit(parent), dropMenu(nullptr), d_ptr(new EsEditableComboBoxPrivate())
{
    Q_D(EsEditableComboBox);
    d->q_ptr = this;
    dropButton = new EsLineEditButton(Es::Icon_ChevronDown, this);

    setTextMargins(0, 0, 29, 0);
    dropButton->setFixedSize(30, 25);
    hBoxLayout->addWidget(dropButton, 0, Qt::AlignRight);

    connect(dropButton, &EsLineEditButton::clicked, this, &EsEditableComboBox::toggleComboMenu);
    connect(this, &EsLineEdit::textChanged, this, &EsEditableComboBox::_onComboTextChanged);
    connect(this, &EsLineEdit::returnPressed, this, &EsEditableComboBox::_onReturnPressed);

    clearButton->disconnect();
    connect(clearButton, &EsLineEditButton::clicked, this, &EsEditableComboBox::_onClearButtonClicked);

    installEventFilter(this);

}

void EsEditableComboBox::addItem(const QString& text, const QVariant& icon, const QVariant& userData)
{
    Q_D(EsEditableComboBox);
    auto item = new EsComboItem(text, icon, userData);
    d->items.append(item);

    if (d->items.size() == 1)
    {
        setCurrentIndex(0);
    }
}

void EsEditableComboBox::addItems(const QList<QString>& texts)
{
    for (const auto& text : texts) {
        addItem(text);
    }
}

void EsEditableComboBox::removeItem(int index)
{
    Q_D(EsEditableComboBox);
    if (index < 0 || index >= d->items.size()) {
        return;
    }

    delete d->items.takeAt(index);

    if (index < currentIndex()) {
        setCurrentIndex(currentIndex() - 1);
    } else if (index == currentIndex()) {
        if (index > 0) {
            setCurrentIndex(currentIndex() - 1);
        } else {
            setText(itemText(0));
            emit currentTextChanged(currentText());
            emit currentIndexChanged(0);
        }
    }

    if (count() == 0) {
        clear();
    }
}

int EsEditableComboBox::currentIndex() const
{
    Q_D(const EsEditableComboBox);
    return d->currentIndex;
}

void EsEditableComboBox::setCurrentIndex(int index)
{
    Q_D(EsEditableComboBox);
    if (index >= count() || index == currentIndex()) return;

    if (index < 0)
    {
        d->currentIndex = -1;
        setText("");
        setPlaceholderText(d->placeholderText);
    }
    else
    {
        d->currentIndex = index;
        setText(d->items[index]->text);
    }
}

void EsEditableComboBox::setText(const QString& text)
{
    EsLineEdit::setText(text);
    adjustSize();
}

QString EsEditableComboBox::currentText() const
{
    Q_D(const EsEditableComboBox);
    if (currentIndex() < 0 || currentIndex() >= d->items.size()) {
        return QString();
    }
    return d->items[currentIndex()]->text;
}

QVariant EsEditableComboBox::currentData() const
{
    Q_D(const EsEditableComboBox);
    if (!(currentIndex() >= 0 && currentIndex() < d->items.size())) {
        return {};
    }

    return d->items[currentIndex()]->userData;
}

void EsEditableComboBox::setCurrentText(const QString& text)
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

void EsEditableComboBox::setItemText(int index, const QString& text)
{
    Q_D(EsEditableComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return;
    }

    d->items[index]->text = text;
    if (currentIndex() == index)
    {
        setText(text);
    }
}

QVariant EsEditableComboBox::itemData(int index) const
{
    Q_D(const EsEditableComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QVariant();
    }

    return d->items[index]->userData;
}

QString EsEditableComboBox::itemText(int index) const
{
    Q_D(const EsEditableComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QString();
    }

    return d->items[index]->text;
}

QIcon EsEditableComboBox::itemIcon(int index) const
{
    Q_D(const EsEditableComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QIcon();
    }

    return d->items[index]->getIcon();
}

void EsEditableComboBox::setItemData(int index, const QVariant& value)
{
    Q_D(EsEditableComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->userData = value;
    }
}

void EsEditableComboBox::setItemIcon(int index, const QIcon& icon)
{
    Q_D(EsEditableComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->setIcon(icon);
    }
}

void EsEditableComboBox::setItemEnabled(int index, bool isEnabled)
{
    Q_D(EsEditableComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->isEnabled = isEnabled;
    }
}

int EsEditableComboBox::findData(const QVariant& data) const
{
    Q_D(const EsEditableComboBox);
    for (int i = 0; i < d->items.size(); ++i)
    {
        if (d->items[i]->userData == data)
        {
            return i;
        }
    }
    return -1;
}

int EsEditableComboBox::findText(const QString& text) const
{
    Q_D(const EsEditableComboBox);
    for (int i = 0; i < d->items.size(); ++i)
    {
        if (d->items[i]->text == text)
        {
            return i;
        }
    }
    return -1;
}

void EsEditableComboBox::clear()
{
    Q_D(EsEditableComboBox);
    if (currentIndex() >= 0)
    {
        setText("");
    }
    while (!d->items.isEmpty()) {
        delete d->items.takeLast();
    }
    d->currentIndex = -1;
}

int EsEditableComboBox::count() const
{
    Q_D(const EsEditableComboBox);
    return static_cast<int>(d->items.size());
}

void EsEditableComboBox::insertItem(int index, const QString& text, const QVariant& icon, const QVariant& userData)
{
    Q_D(EsEditableComboBox);
    auto item = new EsComboItem(text, icon, userData);
    d->items.insert(index, item);

    if (index <= currentIndex())
    {
        setCurrentIndex(currentIndex() + 1);
    }
}

void EsEditableComboBox::insertItems(int index, const QList<QString>& texts)
{
    Q_D(EsEditableComboBox);
    int pos = index;
    for (const auto& text : texts)
    {
        auto item = new EsComboItem(text);
        d->items.insert(d->items.begin() + pos, item);
        ++pos;
    }

    if (index <= currentIndex())
    {
        setCurrentIndex(currentIndex() + pos - index);
    }
}

void EsEditableComboBox::setMaxVisibleItems(int num)
{
    Q_D(EsEditableComboBox);
    d->maxVisibleItems = num;
}

int EsEditableComboBox::maxVisibleItems() const
{
    Q_D(const EsEditableComboBox);
    return d->maxVisibleItems;
}

void EsEditableComboBox::_closeComboMenu()
{
    Q_D(EsEditableComboBox);
    if (!dropMenu)
        return;


    dropMenu = nullptr;
}

EsComboBoxMenu* EsEditableComboBox::_createComboMenu()
{
    return new EsComboBoxMenu({}, this);
}

void EsEditableComboBox::_onDropMenuClosed()
{
    Q_D(EsEditableComboBox);
    dropMenu = nullptr;
}

void EsEditableComboBox::_showComboMenu()
{
    Q_D(EsEditableComboBox);
    if (d->items.empty())
        return;

    EsComboBoxMenu* menu = _createComboMenu();
    for (int i = 0; i < d->items.size(); ++i)
    {
        const auto& item = d->items[i];
        EsAction* action = new EsAction(item->getIcon(), item->text, menu);
        action->setEnabled(item->isEnabled);
        connect(action, &EsAction::triggered, [this, i]() { _onItemClicked(i); });
        menu->addAction(action);
    }

    if (menu->view->width() < width())
    {
        menu->view->setMinimumWidth(width());
        menu->adjustSize();
    }

    menu->setMaxVisibleItems(maxVisibleItems());
    menu->setAttribute(Qt::WA_DeleteOnClose);
    connect(menu, &EsComboBoxMenu::closedSignal, this, &EsEditableComboBox::_onDropMenuClosed);
    dropMenu = menu;

    // 设置当前选中项
    if (currentIndex() >= 0 && !d->items.empty())
    {
        menu->setDefaultAction(menu->actions()[currentIndex()]);
    }

    // 根据菜单最大高度选择动画类型
    int x = -menu->width() / 2 + menu->layout()->contentsMargins().left() + width() / 2;
    QPoint pd = mapToGlobal(QPoint(x, height()));
    int hd = menu->view->heightForAnimation(pd, Es::MenuAnimationType::DROP_DOWN);

    QPoint pu = mapToGlobal(QPoint(x, 0));
    int hu = menu->view->heightForAnimation(pu, Es::MenuAnimationType::PULL_UP);

    if (hd >= hu)
    {
        menu->view->adjustSize(pd, Es::MenuAnimationType::DROP_DOWN);
        menu->exec(pd, Es::MenuAnimationType::DROP_DOWN);
    }
    else
    {
        menu->view->adjustSize(pu, Es::MenuAnimationType::PULL_UP);
        menu->exec(pu, Es::MenuAnimationType::PULL_UP);
    }
}

void EsEditableComboBox::toggleComboMenu()
{
    Q_D(EsEditableComboBox);
    if (dropMenu)
    {
        _closeComboMenu();
    }
    else
    {
        _showComboMenu();
    }
}

void EsEditableComboBox::_onItemClicked(int index)
{
    if (index != currentIndex())
    {
        setCurrentIndex(index);
    }

    emit activated(index);
    emit textActivated(currentText());
}

bool EsEditableComboBox::eventFilter(QObject* obj, QEvent* e)
{
    Q_D(EsEditableComboBox);
    if (obj == this)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            d->isPressed = true;
        }

        else if (e->type() == QEvent::MouseButtonRelease)
        {
            d->isPressed = false;
        }
        else if (e->type() == QEvent::Enter)
        {
            d->isHover = true;
        }
        else if (e->type() == QEvent::Leave)
        {
            d->isHover = false;
        }
    }
    return EsLineEdit::eventFilter(obj, e);
}

void EsEditableComboBox::setCompleterMenu(EsCompleterMenu* menu)
{
    EsLineEdit::setCompleterMenu(menu);
    connect(menu, &EsCompleterMenu::activated, this, &EsEditableComboBox::onActivated);
}

void EsEditableComboBox::onActivated(const QString& text)
{
    int index = findText(text);
    if (index >= 0) setCurrentIndex(index);
}

QString EsEditableComboBox::currentText()
{
    return text();
}

void EsEditableComboBox::setPlaceholderText(const QString& text)
{
    Q_D(EsEditableComboBox);
    d->placeholderText = text;
    EsLineEdit::setPlaceholderText(text);
}


void EsEditableComboBox::_onReturnPressed()
{
    Q_D(EsEditableComboBox);
    if (text().isEmpty()) {
        return;
    }

    int index = findText(text());
    if (index >= 0 && index != currentIndex()) {
        d->currentIndex = index;
        emit currentIndexChanged(index);
    } else if (index == -1) {
        addItem(text());
        setCurrentIndex(count() - 1);
    }
}


void EsEditableComboBox::_onComboTextChanged(const QString &text)
{
    Q_D(EsEditableComboBox);
    d->currentIndex = -1;
    emit currentTextChanged(text);

    for (int i = 0; i < d->items.size(); ++i) {
        if (d->items[i]->text == text) {
            d->currentIndex = i;
            emit currentIndexChanged(i);
            return;
        }
    }
}


void EsEditableComboBox::_onClearButtonClicked()
{
    Q_D(EsEditableComboBox);
    clear();
    d->currentIndex = -1;
}

// endregion
