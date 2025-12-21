/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableView.cpp
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTableView.h"



#include <QApplication>

#include "esStyleSheet.h"
#include "delegate/esTableItemDelegate.h"
#include "private/esTableView_p.h"
#include <QHeaderView>
#include <QMouseEvent>
#include "delegate/esSmoothScrollDelegate.h"
// region ================= EsTableViewPrivate =================

EsTableViewPrivate::EsTableViewPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsTableViewPrivate::init()
{
    Q_Q(EsTableView);
    delegate = new EsTableItemDelegate(q);
    scrollDelegate = new EsSmoothScrollDelegate(q);
    isSelectRightClickedRow = false;
    // 设置样式表
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_TABLE_VIEW);
    styleSheet->apply(q);
    q->setShowGrid(false);
    q->setMouseTracking(true);
    q->setAlternatingRowColors(true);
    q->setItemDelegate(delegate);
    q->setSelectionBehavior(QTableView::SelectRows);
    q->horizontalHeader()->setHighlightSections(false);
    q->verticalHeader()->setHighlightSections(false);
    q->verticalHeader()->setDefaultSectionSize(38);

    connect(q, &EsTableView::entered, this, [=](const QModelIndex& index)
    {
        setHoverRow(index.row());
    });
    connect(q, &EsTableView::pressed, this, [=](const QModelIndex& index)
    {
        setPressedRow(index.row());
    });
    connect(q->verticalHeader(), &QHeaderView::sectionClicked, q, &EsTableView::selectRow);
}

void EsTableViewPrivate::setHoverRow(int row)
{
    Q_Q(EsTableView);
    delegate->setHoverRow(row);
    q->viewport()->update();
}

void EsTableViewPrivate::setPressedRow(int row)
{
    Q_Q(EsTableView);
    if (q->selectionMode() == QTableView::NoSelection) {
        return;
    }

    delegate->setPressedRow(row);
    q->viewport()->update();
}

void EsTableViewPrivate::setSelectedRows(const QList<QModelIndex>& indexes)
{
    Q_Q(EsTableView);
    if (q->selectionMode() == QTableView::NoSelection) {
        return;
    }

    delegate->setSelectedRows(indexes);
    q->viewport()->update();
}


// endregion

// region ================= EsTableView  =================

EsTableView::EsTableView(QWidget* parent)
    : QTableView(parent), d_ptr(new EsTableViewPrivate())
{
    Q_D(EsTableView);
    d->q_ptr = this;
    d->init();
}

EsTableView::~EsTableView()
{
}

bool EsTableView::isSelectRightClickedRow() const
{
    Q_D(const EsTableView);
    return d->isSelectRightClickedRow;
}

void EsTableView::setSelectRightClickedRow(bool isSelect)
{
    Q_D(EsTableView);
    d->isSelectRightClickedRow = isSelect;
}

void EsTableView::setBorderVisible(bool isVisible)
{
    Q_D(EsTableView);
    setProperty("isBorderVisible", isVisible);
    setStyle(QApplication::style());
}

void EsTableView::setBorderRadius(int radius)
{
    Q_D(EsTableView);
    QString qss = QString("QTableView { border-radius: %1px }").arg(radius);
    EsFunc::setCustomStyleSheet(this, qss, qss);
}

void EsTableView::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsTableView);
    d->delegate->setCheckedColor(light, dark);
}

void EsTableView::leaveEvent(QEvent* e)
{
    Q_D(EsTableView);
    QTableView::leaveEvent(e);
    d->setHoverRow(-1);
}

void EsTableView::resizeEvent(QResizeEvent* e)
{
    QTableView::resizeEvent(e);
    viewport()->update();
}

void EsTableView::keyPressEvent(QKeyEvent* e)
{
    QTableView::keyPressEvent(e);
    updateSelectedRows();
}

void EsTableView::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsTableView);
    if (e->button() == Qt::LeftButton || d->isSelectRightClickedRow) {
        return QTableView::mousePressEvent(e);
    }

    QModelIndex index = indexAt(e->pos());
    if (index.isValid()) {
        d->setPressedRow(index.row());
    }

    QTableView::mousePressEvent(e);
}

void EsTableView::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsTableView);
    QTableView::mouseReleaseEvent(e);
    updateSelectedRows();

    if (indexAt(e->pos()).row() < 0 || e->button() == Qt::RightButton) {
        d->setPressedRow(-1);
    }
}

void EsTableView::setItemDelegate(EsTableItemDelegate* delegate)
{
    Q_D(EsTableView);
    d->delegate = delegate;
    QTableView::setItemDelegate(delegate);
}

void EsTableView::selectAll()
{
    QTableView::selectAll();
    updateSelectedRows();
}

void EsTableView::selectRow(int row)
{
    QTableView::selectRow(row);
    updateSelectedRows();
}

void EsTableView::clearSelection()
{
    QTableView::clearSelection();
    updateSelectedRows();
}

void EsTableView::setCurrentIndex(const QModelIndex& index)
{
    QTableView::setCurrentIndex(index);
    updateSelectedRows();
}

void EsTableView::updateSelectedRows()
{
    Q_D(EsTableView);
    d->setSelectedRows(selectedIndexes());
}


// endregion
