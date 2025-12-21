/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableView.h
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTABLEVIEW_H
#define ESTABLEVIEW_H


#include "esDef.h"
#include <QTableView>

class EsTableViewPrivate;
class EsTableItemDelegate;

class ES_EXPORT EsTableView : public QTableView
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsTableView);
    Q_PROPERTY(bool selectRightClickedRow READ isSelectRightClickedRow WRITE setSelectRightClickedRow)

public :
    explicit EsTableView(QWidget* parent = nullptr);
    ~EsTableView() override;
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
    * 设置边框的可见性
    *
    * @param isVisible 是否可见
    */
    void setBorderVisible(bool isVisible);

    /**
     * 设置边框的圆角半径
     *
     * @param radius 圆角半径
     */
    void setBorderRadius(int radius);

    /**
     * 设置选中状态下的颜色
     *
     * @param light 浅色模式颜色
     * @param dark 深色模式颜色
     */
    void setCheckedColor(const QColor& light, const QColor& dark);


    /**
     * 设置项代理
     *
     * @param delegate 委托
     */
    void setItemDelegate(EsTableItemDelegate* delegate);

    /**
     * 全选
     */
    void selectAll() override;

    /**
     * 选中某一行
     *
     * @param row 行号
     */
    Q_SLOT void selectRow(int row);

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

#endif //ESTABLEVIEW_H
