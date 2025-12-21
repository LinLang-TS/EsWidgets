/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esMenuItemDelegate.cpp
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esMenuItemDelegate.h"

#include <QPainter>
#include <QAction>
#include <QTextLayout>

#include "esConfig.h"
#include "esFont.h"

// region ================= EsMenuItemDelegate  =================

EsMenuItemDelegate::EsMenuItemDelegate(QObject* parent)
: QStyledItemDelegate(parent)
{
}

bool EsMenuItemDelegate::isSeparator(const QModelIndex& index) const
{
    return index.model()->data(index, Qt::DecorationRole).toString() == "seperator";
}

void EsMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!isSeparator(index))
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // 绘制分隔符
    painter->save();

    int c = EsFunc::isDarkTheme() ? 255 : 0;
    QPen pen(QColor(c, c, c, 25), 1);
    pen.setCosmetic(true);
    painter->setPen(pen);

    QRect rect = option.rect;
    painter->drawLine(0, rect.y() + 4, rect.width() + 12, rect.y() + 4);

    painter->restore();
}
// endregion

// region ================= EsShortcutMenuItemDelegate  =================

EsShortcutMenuItemDelegate::EsShortcutMenuItemDelegate(QObject* parent): EsMenuItemDelegate(parent)
{
}

void EsShortcutMenuItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    // 先调用父类绘制
    EsMenuItemDelegate::paint(painter, option, index);

    if (isSeparator(index))
        return;

    // 获取 QAction
    QVariant data = index.data(Qt::UserRole);
    QAction* action = qobject_cast<QAction*>(data.value<QObject*>());
    if (!action || action->shortcut().isEmpty())
        return;

    painter->save();

    // disabled 状态透明度
    if (!(option.state & QStyle::State_Enabled))
    {
        painter->setOpacity(EsFunc::isDarkTheme() ? 0.5 : 0.6);
    }

    QFont font = EsFunc::getFont(12);
    painter->setFont(font);
    painter->setPen(EsFunc::isDarkTheme()
                        ? QColor(255, 255, 255, 200)
                        : QColor(0, 0, 0, 153));

    QString shortcut = action->shortcut().toString(QKeySequence::NativeText);

    // text layout 用于计算文本宽度
    QTextLayout layout(shortcut, font);
    layout.beginLayout();
    QTextLine line = layout.createLine();
    line.setNumColumns(1);
    layout.endLayout();

    // 垂直居中
    qreal y = option.rect.y()
        + (option.rect.height() - layout.boundingRect().height()) / 2.0;

    // 靠右绘制，右侧留 20px padding
    qreal x = option.rect.right()
        - layout.boundingRect().width()
        - 20.0;

    layout.draw(painter, QPointF(x, y));

    painter->restore();
}

// endregion
