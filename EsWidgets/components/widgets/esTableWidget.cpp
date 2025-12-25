/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableWidget.cpp
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTableWidget.h"

#include <QApplication>

#include "esStyleSheet.h"
#include "delegate/esTableItemDelegate.h"
#include "private/esTableWidget_p.h"
#include <QHeaderView>
#include <QMouseEvent>

#include "delegate/esSmoothScrollDelegate.h"

// region ================= EsTableWidgetPrivate =================

EsTableWidgetPrivate::EsTableWidgetPrivate(QObject* parent)
    : QObject(parent)
{
}

EsTableWidgetPrivate::~EsTableWidgetPrivate()
{
}

void EsTableWidgetPrivate::init()
{
    Q_Q(EsTableWidget);
    delegate = new EsTableItemDelegate(q);  // 注意 在私有类里创建对象且要指定父类别传this, 要传q, 他妈的 研究半天 传错父类了
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

    connect(q, &QTableWidget::entered, this, [=](const QModelIndex& index)
    {
        setHoverRow(index.row());
    });
    connect(q, &QTableWidget::pressed, this, [=](const QModelIndex& index)
    {
        setPressedRow(index.row());
    });
    connect(q->verticalHeader(), &QHeaderView::sectionClicked, q, &EsTableWidget::selectRow);
}

void EsTableWidgetPrivate::setHoverRow(int row)
{
    Q_Q(EsTableWidget);
    delegate->setHoverRow(row);
    q->viewport()->update();
}

void EsTableWidgetPrivate::setPressedRow(int row)
{
    Q_Q(EsTableWidget);
    if (q->selectionMode() == QTableView::NoSelection) {
        return;
    }

    delegate->setPressedRow(row);
    q->viewport()->update();
}

void EsTableWidgetPrivate::setSelectedRows(const QList<QModelIndex>& indexes)
{
    Q_Q(EsTableWidget);
    if (q->selectionMode() == QTableView::NoSelection) {
        return;
    }

    delegate->setSelectedRows(indexes);
    q->viewport()->update();
}


// endregion

// region ================= EsTableWidget  =================

EsTableWidget::EsTableWidget(QWidget* parent)
    : QTableWidget(parent), d_ptr(new EsTableWidgetPrivate())
{
    Q_D(EsTableWidget);
    d->q_ptr = this;

    d->init();

}

EsTableWidget::EsTableWidget(int rows, int columns, QWidget* parent)
    :QTableWidget(rows, columns, parent), d_ptr(new EsTableWidgetPrivate())
{
    Q_D(EsTableWidget);
    d->q_ptr = this;
    d->init();
}

EsTableWidget::~EsTableWidget()
{
}

void EsTableWidget::setCurrentCell(int row, int column, QItemSelectionModel::SelectionFlags command)
{
    setCurrentItem(item(row, column), command);
}

void EsTableWidget::setCurrentItem(QTableWidgetItem* item, QItemSelectionModel::SelectionFlags command)
{
    if (!command) {
        QTableWidget::setCurrentItem(item);
    } else {
        QTableWidget::setCurrentItem(item, command);
    }

    updateSelectedRows();
}

EsTableWidgetComboItem* EsTableWidget::itemCombo(int row, int column)
{
    return dynamic_cast<EsTableWidgetComboItem*>(item(row, column));
}


bool EsTableWidget::isSelectRightClickedRow() const
{
    Q_D(const EsTableWidget);
    return d->isSelectRightClickedRow;
}

void EsTableWidget::setSelectRightClickedRow(bool isSelect)
{
    Q_D(EsTableWidget);
    d->isSelectRightClickedRow = isSelect;
}


void EsTableWidget::setBorderVisible(bool isVisible)
{
    Q_D(EsTableWidget);
    setProperty("isBorderVisible", isVisible);
    setStyle(QApplication::style());
}

void EsTableWidget::setBorderRadius(int radius)
{
    Q_D(EsTableWidget);
    QString qss = QString("QTableView { border-radius: %1px }").arg(radius);
    EsFunc::setCustomStyleSheet(this, qss, qss);
}

void EsTableWidget::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsTableWidget);
    d->delegate->setCheckedColor(light, dark);
}

void EsTableWidget::leaveEvent(QEvent* e)
{
    Q_D(EsTableWidget);
    QTableView::leaveEvent(e);
    d->setHoverRow(-1);
}

void EsTableWidget::resizeEvent(QResizeEvent* e)
{
    QTableView::resizeEvent(e);
    viewport()->update();
}

void EsTableWidget::keyPressEvent(QKeyEvent* e)
{
    QTableView::keyPressEvent(e);
    updateSelectedRows();
}

void EsTableWidget::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsTableWidget);
    if (e->button() == Qt::LeftButton || d->isSelectRightClickedRow) {
        return QTableView::mousePressEvent(e);
    }

    QModelIndex index = indexAt(e->pos());
    if (index.isValid()) {
        d->setPressedRow(index.row());
    }

    QTableWidget::mousePressEvent(e);
}

void EsTableWidget::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsTableWidget);
    QTableView::mouseReleaseEvent(e);
    updateSelectedRows();

    if (indexAt(e->pos()).row() < 0 || e->button() == Qt::RightButton) {
        d->setPressedRow(-1);
    }
}

void EsTableWidget::setItemDelegate(EsTableItemDelegate* delegate)
{
    Q_D(EsTableWidget);
    d->delegate = delegate;
    QTableView::setItemDelegate(delegate);
}

void EsTableWidget::selectAll()
{
    QTableView::selectAll();
    updateSelectedRows();
}

void EsTableWidget::selectRow(int row)
{
    QTableView::selectRow(row);
    updateSelectedRows();
}

void EsTableWidget::clearSelection()
{
    QTableView::clearSelection();
    updateSelectedRows();
}

void EsTableWidget::setCurrentIndex(const QModelIndex& index)
{
    QTableView::setCurrentIndex(index);
    updateSelectedRows();
}

void EsTableWidget::updateSelectedRows()
{
    Q_D(EsTableWidget);
    d->setSelectedRows(selectedIndexes());
}


// endregion
