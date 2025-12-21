/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListView.cpp
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esListView.h"

#include "esStyleSheet.h"
#include "delegate/esListItemDelegate.h"
#include "delegate/esSmoothScrollDelegate.h"

#include <QMouseEvent>
#include "private/esListView_p.h"

// region ================= EsListViewPrivate =================

EsListViewPrivate::EsListViewPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsListViewPrivate::init()
{
    Q_Q(EsListView);
    delegate = new EsListItemDelegate(q);
    scrollDelegate = new EsSmoothScrollDelegate(q);
    isSelectRightClickedRow = false;

    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_LIST_VIEW);
    styleSheet->apply(q);

    q->setItemDelegate(delegate);
    q->setMouseTracking(true);
    connect(q, &EsListView::entered, this, [=](const QModelIndex& index)
    {
        setHoverRow(index.row());
    });
    connect(q, &EsListView::pressed, this, [=](const QModelIndex& index)
    {
        setPressedRow(index.row());
    });
}

void EsListViewPrivate::setHoverRow(int row)
{
    Q_Q(EsListView);
    delegate->setHoverRow(row);
    q->viewport()->update();
}

void EsListViewPrivate::setPressedRow(int row)
{
    Q_Q(EsListView);
    if (q->selectionMode() == EsListView::NoSelection)
    {
        return;
    }

    delegate->setPressedRow(row);
    q->viewport()->update();
}

void EsListViewPrivate::setSelectedRows(const QList<QModelIndex>& indexes)
{
    Q_Q(EsListView);
    if (q->selectionMode() == EsListView::NoSelection)
    {
        return;
    }

    delegate->setSelectedRows(indexes);
    q->viewport()->update();
}

// endregion

// region ================= EsListViewPrivate =================

EsListView::EsListView(QWidget* parent)
    : QListView(parent), d_ptr(new EsListViewPrivate())
{
    Q_D(EsListView);
    d->q_ptr = this;
    d->init();
}

EsListView::~EsListView()
{
}

bool EsListView::isSelectRightClickedRow() const
{
    Q_D(const EsListView);
    return d->isSelectRightClickedRow;
}

void EsListView::setSelectRightClickedRow(bool isSelect)
{
    Q_D(EsListView);
    d->isSelectRightClickedRow = isSelect;
}

void EsListView::setItemDelegate(EsListItemDelegate* delegate)
{
    Q_D(EsListView);
    d->delegate = delegate;
    QListView::setItemDelegate(delegate);
}


void EsListView::clearSelection()
{
    QListView::clearSelection();
    updateSelectedRows();
}

void EsListView::setCurrentIndex(const QModelIndex& index)
{
    QListView::setCurrentIndex(index);
    updateSelectedRows();
}

void EsListView::updateSelectedRows()
{
    Q_D(EsListView);
    d->setSelectedRows(selectedIndexes());
}

void EsListView::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsListView);
    d->delegate->setCheckedColor(light, dark);
}

void EsListView::leaveEvent(QEvent* e)
{
    Q_D(EsListView);
    QListView::leaveEvent(e);
    d->setHoverRow(-1);
}

void EsListView::resizeEvent(QResizeEvent* e)
{
    QListView::resizeEvent(e);
    viewport()->update();
}

void EsListView::keyPressEvent(QKeyEvent* e)
{
    QListView::keyPressEvent(e);
    updateSelectedRows();
}

void EsListView::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsListView);
    if (e->button() == Qt::LeftButton || d->isSelectRightClickedRow)
    {
        return QListView::mousePressEvent(e);
    }

    QModelIndex index = indexAt(e->pos());
    if (index.isValid())
    {
        d->setPressedRow(index.row());
    }

    QListView::mousePressEvent(e);
}

void EsListView::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsListView);
    QListView::mouseReleaseEvent(e);
    updateSelectedRows();

    if (indexAt(e->pos()).row() < 0 || e->button() == Qt::RightButton)
    {
        d->setPressedRow(-1);
    }
}

// endregion
