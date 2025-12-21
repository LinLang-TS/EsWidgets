/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esComboBox.cpp
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esComboBox.h"

#include <QApplication>
#include <QLayout>
#include <QPainter>

#include "esAnimation.h"
#include "esFont.h"
#include "esIcon.h"
#include "esMenuActionListWidget.h"
#include "esRoundMenu.h"
#include "esStyleSheet.h"
#include "private/esComboBox_p.h"
// region ================= EsComboItem  =================

EsComboItem::EsComboItem(const QString& text, const QVariant& icon, const QVariant& userData, bool isEnabled)
    :text(text), userData(userData), isEnabled(isEnabled)
{
    setIcon(icon);
}

EsComboItem::EsComboItem(const QString& text, Es::IconName icon, const QVariant& userData, bool isEnabled)
    : EsComboItem(text, QVariant::fromValue<EsIconBase*>(new EsIcon(icon)), userData, isEnabled)
{
}

EsComboItem::EsComboItem(const QString& text, const QString& icon, const QVariant& userData, bool isEnabled)
    : EsComboItem(text, QVariant::fromValue(icon), userData, isEnabled)
{
}

EsComboItem::EsComboItem(const QString& text, const QIcon& icon, const QVariant& userData, bool isEnabled)
    : EsComboItem(text, QVariant::fromValue(icon), userData, isEnabled)
{
}

EsComboItem::EsComboItem(const QString& text, EsIconBase* icon, const QVariant& userData, bool isEnabled)
    : EsComboItem(text, QVariant::fromValue(icon), userData, isEnabled)
{
}



QIcon EsComboItem::getIcon() const
{
    if (_icon.type() == QVariant::Icon)
    {
        return _icon.value<QIcon>();
    }
    return _icon.value<EsIconBase*>()->icon();
}

void EsComboItem::setIcon(const QVariant& ico)
{
    if (ico.isNull() || !ico.isValid())
    {
        _icon = QIcon();
    }
    else
    {
        if (ico.type() == QVariant::String)
        {
            _icon = QIcon(ico.toString());
        }
        else
        {
            _icon = ico;
        }
    }
}
// endregion

// region ================= EsComboBoxPrivate  =================

EsComboBoxPrivate::EsComboBoxPrivate(QObject* parent)
    : QObject(parent),
      isHover(false),
      isPressed(false),
      items(),
      currentIndex(-1),
      maxVisibleItems(-1),
      placeholderText("")

{
}

void EsComboBoxPrivate::updateTextState(bool isPlaceholder)
{
    Q_Q(EsComboBox);

    if (q->property("isPlaceholderText").toBool() == isPlaceholder) return;

    q->setProperty("isPlaceholderText", isPlaceholder);
    q->setStyle(QApplication::style());
}

// endregion

// region ================= EsComboBox  =================

EsComboBox::EsComboBox(QWidget* parent)
    : QPushButton(parent), dropMenu(nullptr), d_ptr(new EsComboBoxPrivate())
{
    Q_D(EsComboBox);
    d->q_ptr = this;
    d->arrowAni = new EsTranslateYAnimation(this);

    auto style = new EsStyleSheet(Es::StyleSheet::Style_COMBO_BOX);
    style->apply(this);
    installEventFilter(this);

    EsFunc::setFont(this);
}

void EsComboBox::addItem(const QString& text, const QVariant& icon, const QVariant& userData)
{
    Q_D(EsComboBox);
    auto item = new EsComboItem(text, icon, userData);
    d->items.append(item);

    if (d->items.size() == 1)
    {
        setCurrentIndex(0);
    }
}

void EsComboBox::addItems(const QList<QString>& texts)
{
    for (const auto& text : texts) {
        addItem(text);
    }
}

void EsComboBox::removeItem(int index)
{
    Q_D(EsComboBox);
    if (index < 0 || index >= d->items.size()) {
        return;
    }

    d->items.removeAt(index);

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

int EsComboBox::currentIndex() const
{
    Q_D(const EsComboBox);
    return d->currentIndex;
}

void EsComboBox::setCurrentIndex(int index)
{
    Q_D(EsComboBox);
    // 索引越界或与当前索引相同则直接返回
    if (index < 0 || index >= d->items.size() || index == currentIndex()) {
        return;
    }

    QString oldText = currentText();

    // 更新当前索引
    d->currentIndex = index;
    setText(d->items[index]->text);

    // 如果当前文本发生变化则发射文本变化信号
    if (oldText != currentText()) {
        emit currentTextChanged(currentText());
    }

    // 发射索引变化信号
    emit currentIndexChanged(index);
}

void EsComboBox::setText(const QString& text)
{
    QPushButton::setText(text);
    adjustSize();
}

QString EsComboBox::currentText() const
{
    Q_D(const EsComboBox);
    if (currentIndex() < 0 || currentIndex() >= d->items.size()) {
        return QString();
    }
    return d->items[currentIndex()]->text;
}

QVariant EsComboBox::currentData() const
{
    Q_D(const EsComboBox);
    if (!(currentIndex() >= 0 && currentIndex() < d->items.size())) {
        return {};
    }

    return d->items[currentIndex()]->userData;
}

void EsComboBox::setCurrentText(const QString& text)
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

void EsComboBox::setItemText(int index, const QString& text)
{
    Q_D(EsComboBox);
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

QVariant EsComboBox::itemData(int index) const
{
    Q_D(const EsComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QVariant();
    }

    return d->items[index]->userData;
}

QString EsComboBox::itemText(int index) const
{
    Q_D(const EsComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QString();
    }

    return d->items[index]->text;
}

QIcon EsComboBox::itemIcon(int index) const
{
    Q_D(const EsComboBox);
    if (index < 0 || index >= d->items.size())
    {
        return QIcon();
    }

    return d->items[index]->getIcon();
}

void EsComboBox::setItemData(int index, const QVariant& value)
{
    Q_D(EsComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->userData = value;
    }
}

void EsComboBox::setItemIcon(int index, const QIcon& icon)
{
    Q_D(EsComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->getIcon() = icon;
    }
}

void EsComboBox::setItemEnabled(int index, bool isEnabled)
{
    Q_D(EsComboBox);
    if (index >= 0 && index < d->items.size())
    {
        d->items[index]->isEnabled = isEnabled;
    }
}

int EsComboBox::findData(const QVariant& data) const
{
    Q_D(const EsComboBox);
    for (int i = 0; i < d->items.size(); ++i)
    {
        if (d->items[i]->userData == data)
        {
            return i;
        }
    }
    return -1;
}

int EsComboBox::findText(const QString& text) const
{
    Q_D(const EsComboBox);
    for (int i = 0; i < d->items.size(); ++i)
    {
        if (d->items[i]->text == text)
        {
            return i;
        }
    }
    return -1;
}

void EsComboBox::clear()
{
    Q_D(EsComboBox);
    if (currentIndex() >= 0)
    {
        setText("");
    }

    d->items.clear();
    d->currentIndex = -1;
}

int EsComboBox::count() const
{
    Q_D(const EsComboBox);
    return static_cast<int>(d->items.size());
}

void EsComboBox::insertItem(int index, const QString& text, const QVariant& icon, const QVariant& userData)
{
    Q_D(EsComboBox);
    auto item = new EsComboItem(text, icon, userData);
    d->items.insert(index, item);

    if (index <= currentIndex())
    {
        setCurrentIndex(currentIndex() + 1);
    }
}

void EsComboBox::insertItems(int index, const QList<QString>& texts)
{
    Q_D(EsComboBox);
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

void EsComboBox::setMaxVisibleItems(int num)
{
    Q_D(EsComboBox);
    d->maxVisibleItems = num;
}

int EsComboBox::maxVisibleItems() const
{
    Q_D(const EsComboBox);
    return d->maxVisibleItems;
}

void EsComboBox::_closeComboMenu()
{
    Q_D(EsComboBox);
    if (!dropMenu)
        return;


    dropMenu = nullptr;
}

EsComboBoxMenu* EsComboBox::_createComboMenu()
{
    return new EsComboBoxMenu({}, this);
}

void EsComboBox::_onDropMenuClosed()
{
    Q_D(EsComboBox);
#ifdef _WIN32
    QPoint pos = mapFromGlobal(QCursor::pos());
    if (!rect().contains(pos))
    {
        dropMenu = nullptr;
    }
#else
    dropMenu = nullptr;
#endif
}

void EsComboBox::_showComboMenu()
{
    Q_D(EsComboBox);
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
    connect(menu, &EsComboBoxMenu::closedSignal, this, &EsComboBox::_onDropMenuClosed);
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

void EsComboBox::toggleComboMenu()
{
    Q_D(EsComboBox);
    if (dropMenu)
    {
        _closeComboMenu();
    }
    else
    {
        _showComboMenu();
    }
}

void EsComboBox::_onItemClicked(int index)
{
    if (index != currentIndex())
    {
        setCurrentIndex(index);
    }

    emit activated(index);
    emit textActivated(currentText());
}

bool EsComboBox::eventFilter(QObject* obj, QEvent* e)
{
    Q_D(EsComboBox);
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
    return QPushButton::eventFilter(obj, e);
}

void EsComboBox::setPlaceholderText(const QString& text)
{
    Q_D(EsComboBox);
    d->placeholderText = text;
    if (currentIndex() <= 0)
    {
        _updateTextState(true);
        setText(text);
    }

}

void EsComboBox::_updateTextState(bool isPlaceholder)
{
    Q_D(EsComboBox);
    if (property("isPlaceholderText").toBool() == isPlaceholder)
    {
        return;
    }

    setProperty("isPlaceholderText", isPlaceholder);
    setStyle(QApplication::style());
}

void EsComboBox::mouseReleaseEvent(QMouseEvent* e)
{
    QPushButton::mouseReleaseEvent(e);
    toggleComboMenu();
}

void EsComboBox::paintEvent(QPaintEvent* e)
{
    Q_D(EsComboBox);
    QPushButton::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    if (d->isHover) {
        painter.setOpacity(0.8);
    } else if (d->isPressed) {
        painter.setOpacity(0.7);
    }

    QRectF rect(
        width() - 22,
        height() / 2.0 - 5 + d->arrowAni->getY(),
        10,
        10
    );

    auto ico = EsIcon(Es::Icon_ChevronDown); // ARROW_DOWN

    if (EsFunc::isDarkTheme())
    {
        ico.render(&painter, rect , {});
    } else
    {
        ico.render(&painter, rect, {}, {{"fill", "#646464"}});
    }
}

// endregion
