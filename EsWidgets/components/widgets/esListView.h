/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListView.h
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLISTVIEW_H
#define ESLISTVIEW_H

#include <QListView>

#include "esDef.h"
class EsListItemDelegate;
class EsListViewPrivate;

class ES_EXPORT EsListView : public QListView
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsListView)
    Q_PROPERTY(bool selectRightClickedRow READ isSelectRightClickedRow WRITE setSelectRightClickedRow)

public:
    explicit EsListView(QWidget* parent = nullptr);
    ~EsListView();

    /**
     * 获取右键点击的行是否被选中
     *
     * @return 是否为右键点击选中的行
     */
    bool isSelectRightClickedRow() const;

    /**
     * 设置右键点击选中的行
     *
     * @param isSelect 是否选中
     */
    void setSelectRightClickedRow(bool isSelect);

    /**
     * 设置项代理
     *
     * @param delegate 委托
     */
    void setItemDelegate(EsListItemDelegate* delegate);

    /**
     * 清除选中状态
     */
    void clearSelection();

    /**
     * 设置当前选中的项
     *
     * @param index 当前项索引
     */
    void setCurrentIndex(const QModelIndex& index);

    /**
     * 更新选中的行
     */
    void updateSelectedRows();

    /**
     * 设置选中状态下的颜色
     *
     * @param light todo 浅色模式颜色
     * @param dark 深色模式颜色
     */
    void setCheckedColor(const QColor& light, const QColor& dark);

protected:
    /**
     * 离开事件
     */
    void leaveEvent(QEvent* e) override;

    /**
     * 调整大小事件
     */
    void resizeEvent(QResizeEvent* e) override;

    /**
     * 键盘按下事件
     */
    void keyPressEvent(QKeyEvent* e) override;

    /**
     * 鼠标按下事件
     */
    void mousePressEvent(QMouseEvent* e) override;

    /**
     * 鼠标释放事件
     */
    void mouseReleaseEvent(QMouseEvent* e) override;
};


#endif //ESLISTVIEW_H
