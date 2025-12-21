/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListWidget_p.h
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLISTWIDGET_P_H
#define ESLISTWIDGET_P_H

#include "esDef.h"
class EsSmoothScrollDelegate;
class EsListItemDelegate;
class EsListWidget;

class EsListWidgetPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsListWidget)

public:
    explicit EsListWidgetPrivate(QObject* parent = nullptr);

    void init();

    /**
     * 设置悬停的行
     *
     * @param row 行号
     */
    Q_SLOT void setHoverRow(int row);

    /**
     * 设置按下的行
     *
     * @param row 行号
     */
    Q_SLOT void setPressedRow(int row);

    /**
     * 设置选中的行
     *
     * @param indexes 选中的行索引
     */
    void setSelectedRows(const QList<QModelIndex>& indexes);

    EsListItemDelegate* delegate;
    EsSmoothScrollDelegate* scrollDelegate;
    bool isSelectRightClickedRow;
};


#endif //ESLISTWIDGET_P_H
