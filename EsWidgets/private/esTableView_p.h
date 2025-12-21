/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableView_p.h
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTABVIEW_P_H
#define ESTABVIEW_P_H

#include "esDef.h"

class EsTableView;
class EsTableItemDelegate;
class EsSmoothScrollDelegate;

class EsTableViewPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsTableView);

public :
    explicit EsTableViewPrivate(QObject *parent = nullptr);
    void init();
    bool isSelectRightClickedRow;
    EsTableItemDelegate* delegate;
    EsSmoothScrollDelegate* scrollDelegate;

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
};

#endif //ESTABVIEW_P_H
