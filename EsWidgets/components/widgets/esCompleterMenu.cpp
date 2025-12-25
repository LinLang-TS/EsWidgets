/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esCompleterMenu.cpp
** @date 2025/12/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esCompleterMenu.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QLayout>

#include "esLineEdit.h"
#include "esMenuActionListWidget.h"
#include "delegate/esIndicatorMenuItemDelegate.h"


EsCompleterMenu::EsCompleterMenu(EsLineEdit *lineEdit, QWidget *parent)
       : EsRoundMenu({}, parent), lineEdit(lineEdit)
{
    setObjectName("CompleterMenu");
    setItemHeight(33);

    // 设置视图
    view->setViewportMargins(0, 2, 0, 6);
    view->setObjectName("completerListWidget");
    view->setItemDelegate(new EsIndicatorMenuItemDelegate());
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 安装事件过滤器
    installEventFilter(this);
}

bool EsCompleterMenu::setCompletion(QAbstractItemModel* model, int column)
{
    // 存储新的项和索引
    QStringList items;
    indexes.clear();

    // 遍历模型的所有行，并将数据添加到项列表和索引列表中
    for (int i = 0; i < model->rowCount(); ++i)
    {
        items.append(model->data(model->index(i, column)).toString());
        indexes.append(model->index(i, column));
    }

    // 如果当前项和新的项相同且菜单可见，则不需要更新
    if (this->items == items && this->isVisible())
    {
        return false;
    }

    // 更新菜单项
    setItems(items);
    return true;
}

void EsCompleterMenu::setItems(const QStringList& items)
{
    // 清空当前视图
    view->clear();

    // 更新补全项
    this->items = items;
    view->addItems(items);

    // 设置每个项的高度
    for (int i = 0; i < view->count(); ++i)
    {
        QListWidgetItem *item = view->item(i);
        item->setSizeHint(QSize(1, itemHeight));  // 设置项的高度
    }
}

void EsCompleterMenu::_onItemClicked(QListWidgetItem* item)
{
    // 隐藏菜单
    _hideMenu(false);

    // 触发项选择事件
    _onCompletionItemSelected(item->text(), view->row(item));
}

bool EsCompleterMenu::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() != QEvent::KeyPress)
    {
        return QMenu::eventFilter(obj, e);
    }

    // 将输入事件重定向到 LineEdit 和视图
    lineEdit->event(e);
    view->event(e);

    if (dynamic_cast<QKeyEvent*>(e)->key() == Qt::Key_Escape)
    {
        close();  // 按下 Esc 键关闭菜单
    }
    else if (dynamic_cast<QKeyEvent*>(e)->key() == Qt::Key_Enter || dynamic_cast<QKeyEvent*>(e)->key() == Qt::Key_Return)
    {
        // 按下 Enter 或 Return 键并且选中项有效时，触发项选择事件
        if (view->currentRow() >= 0)
        {
            _onCompletionItemSelected(view->currentItem()->text(), view->currentRow());
            close();
        }
    }

    return EsRoundMenu::eventFilter(obj, e);
}

void EsCompleterMenu::_onCompletionItemSelected(const QString& text, int row)
{
    lineEdit->setText(text);  // 设置 LineEdit 的文本为选中的项

    emit activated(text);  // 发出激活的信号

    if (row >= 0 && row < indexes.size())
    {
        emit indexActivated(indexes[row]);  // 如果行号有效，发出对应的索引激活信号
    }
}

void EsCompleterMenu::popup()
{
    if (items.isEmpty())
    {
        close();  // 如果没有项，则关闭菜单
        return;
    }

    // 调整菜单的宽度
    QWidget *p = lineEdit;
    if (view->width() < p->width())
    {
        view->setMinimumWidth(p->width());
        adjustSize();
    }

    // 根据最大视图高度选择动画类型
    int x = -width() / 2 + layout()->contentsMargins().left() + p->width() / 2;
    int y = p->height() - layout()->contentsMargins().top() + 2;
    QPoint pd = p->mapToGlobal(QPoint(x, y));
    int hd = view->heightForAnimation(pd, Es::MenuAnimationType::FADE_IN_DROP_DOWN);

    QPoint pu = p->mapToGlobal(QPoint(x, 7));
    int hu = view->heightForAnimation(pu, Es::MenuAnimationType::FADE_IN_PULL_UP);

    QPoint pos;
    Es::MenuAnimationType aniType;
    if (hd >= hu)
    {
        pos = pd;
        aniType = Es::MenuAnimationType::FADE_IN_DROP_DOWN;
    }
    else
    {
        pos = pu;
        aniType = Es::MenuAnimationType::FADE_IN_PULL_UP;
    }

    view->adjustSize(pos, aniType);  // 调整视图大小

    // 更新视图的边框样式
    view->setProperty("dropDown", aniType == Es::MenuAnimationType::FADE_IN_DROP_DOWN);
    view->setStyle(QApplication::style());
    view->update();

    adjustSize();
    exec(pos, aniType);  // 执行动画并显示菜单

    // 移除菜单的焦点
    view->setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::NoFocus);
    p->setFocus();  // 重新设置输入框为焦点
}
