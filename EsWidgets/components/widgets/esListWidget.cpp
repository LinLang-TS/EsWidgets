/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListWidget.cpp
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esListWidget.h"

#include "esStyleSheet.h"
#include "delegate/esListItemDelegate.h"
#include "delegate/esSmoothScrollDelegate.h"

#include <QMouseEvent>
#include "private/esListWidget_p.h"

// region ================= EsListWidgetPrivate =================

EsListWidgetPrivate::EsListWidgetPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsListWidgetPrivate::init()
{
    Q_Q(EsListWidget);
    delegate = new EsListItemDelegate(q);
    scrollDelegate = new EsSmoothScrollDelegate(q);
    isSelectRightClickedRow = false;

    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_LIST_VIEW);
    styleSheet->apply(q);

    q->setItemDelegate(delegate);
    q->setMouseTracking(true);
    connect(q, &EsListWidget::entered, this, [=](const QModelIndex& index)
    {
        setHoverRow(index.row());
    });
    connect(q, &EsListWidget::pressed, this, [=](const QModelIndex& index)
    {
        setPressedRow(index.row());
    });

}

void EsListWidgetPrivate::setHoverRow(int row)
{
    Q_Q(EsListWidget);
    delegate->setHoverRow(row);
    q->viewport()->update();
}

void EsListWidgetPrivate::setPressedRow(int row)
{
    Q_Q(EsListWidget);
    if (q->selectionMode() == EsListWidget::NoSelection) {
        return;
    }

    delegate->setPressedRow(row);
    q->viewport()->update();
}

void EsListWidgetPrivate::setSelectedRows(const QList<QModelIndex>& indexes)
{
    Q_Q(EsListWidget);
    if (q->selectionMode() == EsListWidget::NoSelection) {
        return;
    }

    delegate->setSelectedRows(indexes);
    q->viewport()->update();
}
// endregion

// region ================= EsListWidgetPrivate =================

EsListWidget::EsListWidget(QWidget* parent)
    : QListWidget(parent), d_ptr(new EsListWidgetPrivate())
{
    Q_D(EsListWidget);
    d->q_ptr = this;
    d->init();
}

EsListWidget::~EsListWidget()
{
}


void EsListWidget::setCurrentItem(QListWidgetItem* item, QItemSelectionModel::SelectionFlags command)
{
    setCurrentRow(row(item), command);
}

void EsListWidget::setCurrentRow(int row, QItemSelectionModel::SelectionFlags command)
{
    if (!command) {
        QListWidget::setCurrentRow(row);
    } else {
        QListWidget::setCurrentRow(row, command);
    }

    updateSelectedRows();
}

bool EsListWidget::isSelectRightClickedRow() const
{
    Q_D(const EsListWidget);
    return d->isSelectRightClickedRow;
}

void EsListWidget::setSelectRightClickedRow(bool isSelect)
{
    Q_D(EsListWidget);
    d->isSelectRightClickedRow = isSelect;
}

void EsListWidget::setItemDelegate(EsListItemDelegate* delegate)
{
    Q_D(EsListWidget);
    d->delegate = delegate;
    QListWidget::setItemDelegate(delegate);
}



void EsListWidget::clearSelection()
{
    QListWidget::clearSelection();
    updateSelectedRows();
}

void EsListWidget::setCurrentIndex(const QModelIndex& index)
{
    QListWidget::setCurrentIndex(index);
    updateSelectedRows();
}

void EsListWidget::updateSelectedRows()
{
    Q_D(EsListWidget);
    d->setSelectedRows(selectedIndexes());
}

void EsListWidget::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsListWidget);
    d->delegate->setCheckedColor(light, dark);
}

void EsListWidget::leaveEvent(QEvent* e)
{
    Q_D(EsListWidget);
    QListWidget::leaveEvent(e);
    d->setHoverRow(-1);
}

void EsListWidget::resizeEvent(QResizeEvent* e)
{
    QListWidget::resizeEvent(e);
    viewport()->update();
}

void EsListWidget::keyPressEvent(QKeyEvent* e)
{
    QListWidget::keyPressEvent(e);
    updateSelectedRows();
}

void EsListWidget::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsListWidget);
    if (e->button() == Qt::LeftButton || d->isSelectRightClickedRow) {
        return QListWidget::mousePressEvent(e);
    }

    QModelIndex index = indexAt(e->pos());
    if (index.isValid()) {
        d->setPressedRow(index.row());
    }

    QListWidget::mousePressEvent(e);
}

void EsListWidget::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsListWidget);
    QListWidget::mouseReleaseEvent(e);
    updateSelectedRows();

    if (indexAt(e->pos()).row() < 0 || e->button() == Qt::RightButton) {
        d->setPressedRow(-1);
    }
}
// endregion
