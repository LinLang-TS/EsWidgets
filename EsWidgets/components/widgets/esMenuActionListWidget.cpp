/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esMenuActionListWidget.cpp
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esMenuActionListWidget.h"

#include "esFont.h"
#include "delegate/esSmoothScrollDelegate.h"
#include "private/esMenuActionListWidget_p.h"
#include "private/esRoundMenu_p.h"

// region ================= EsMenuActionListWidgetPrivate  =================

EsMenuActionListWidgetPrivate::EsMenuActionListWidgetPrivate(QObject* parent)
    : QObject(parent),
      itemHeight(28),
      maxVisibleItems(-1)
{
}

// endregion


// region ================= EsMenuActionListWidget  =================

EsMenuActionListWidget::EsMenuActionListWidget(QWidget* parent)
    : QListWidget(parent),
      d_ptr(new EsMenuActionListWidgetPrivate())
{
    Q_D(EsMenuActionListWidget);
    d->q_ptr = this;

    setViewportMargins(0, 6, 0, 6);
    setTextElideMode(Qt::ElideNone);
    setDragEnabled(false);
    setMouseTracking(true);
    setIconSize(QSize(14, 14));
    setItemDelegate(new EsShortcutMenuItemDelegate(this));
    d->scrollDelegate = new EsSmoothScrollDelegate(this);

    setStyleSheet(
        QStringLiteral("EsMenuActionListWidget{%1}")
        .arg(EsFunc::fontStyleSheet(EsFunc::getFont()))
    );

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

EsMenuActionListWidget::~EsMenuActionListWidget()
{
}

void EsMenuActionListWidget::insertItem(int row, QListWidgetItem* item)
{
    QListWidget::insertItem(row, item);
    this->adjustSize();
}

void EsMenuActionListWidget::addItem(QListWidgetItem* item)
{
    QListWidget::addItem(item);
    this->adjustSize();
}

QListWidgetItem* EsMenuActionListWidget::takeItem(int row)
{
    QListWidgetItem* item = QListWidget::takeItem(row);
    this->adjustSize();
    return item;
}

void EsMenuActionListWidget::adjustSize(const QPoint& pos, Es::MenuAnimationType aniType)
{
    Q_D(EsMenuActionListWidget);
    QSize size;

    // 计算总尺寸
    for (int i = 0; i < this->count(); ++i)
    {
        QSize s = this->item(i)->sizeHint();
        size.setWidth(std::max({s.width(), size.width(), 1}));
        size.setHeight(std::max(1, size.height() + s.height()));
    }

    // 调整 viewport 的高度
    auto manager = EsMenuAnimationManager::make(QVariant::fromValue(this), aniType);
    QSize available = manager->availableViewSize(pos);
    delete manager;
    int w = available.width();
    int h = available.height();

    // 调整 QListWidget 的大小
    QMargins m = this->viewportMargins();
    size += QSize(m.left() + m.right() + 2, m.top() + m.bottom());
    size.setHeight(std::min(h, size.height() + 3));
    size.setWidth(std::max(std::min(w, size.width()), this->minimumWidth()));

    if (this->maxVisibleItems() > 0)
    {
        size.setHeight(std::min(
            size.height(),
            this->maxVisibleItems() * d->itemHeight + m.top() + m.bottom() + 3
        ));
    }

    this->setFixedSize(size);
}

void EsMenuActionListWidget::setItemHeight(int height)
{
    Q_D(EsMenuActionListWidget);
    if (height == d->itemHeight)
        return;

    for (int i = 0; i < this->count(); ++i)
    {
        QListWidgetItem* item = this->item(i);
        if (!this->itemWidget(item))
        {
            QSize s = item->sizeHint();
            item->setSizeHint(QSize(s.width(), height));
        }
    }

    d->itemHeight = height;
    this->adjustSize();
}

void EsMenuActionListWidget::setMaxVisibleItems(int num)
{
    Q_D(EsMenuActionListWidget);
    d->maxVisibleItems = num;
    this->adjustSize();
}

int EsMenuActionListWidget::maxVisibleItems() const
{
    Q_D(const EsMenuActionListWidget);
    return d->maxVisibleItems;
}

int EsMenuActionListWidget::heightForAnimation(const QPoint& pos, Es::MenuAnimationType aniType)
{

    int ih = this->itemsHeight();

    auto manager = EsMenuAnimationManager::make(QVariant::fromValue(this), aniType);
    QSize available = manager->availableViewSize(pos);
    delete manager;

    int sh = available.height();
    return std::min(ih, sh);
}

int EsMenuActionListWidget::itemsHeight() const
{
    int N = (maxVisibleItems() < 0) ? this->count() : std::min(maxVisibleItems(), this->count());
    int h = 0;
    for (int i = 0; i < N; ++i)
    {
        h += this->item(i)->sizeHint().height();
    }

    QMargins m = this->viewportMargins();
    return h + m.top() + m.bottom();
}

QMargins EsMenuActionListWidget::viewportMargins() const
{
    return QListWidget::viewportMargins();
}

void EsMenuActionListWidget::setViewportMargins(int left, int top, int right, int bottom)
{
    QListWidget::setViewportMargins(left, top, right, bottom);
}

bool EsMenuActionListWidget::event(QEvent* e)
{
    return QListWidget::event(e);
}

// endregion
