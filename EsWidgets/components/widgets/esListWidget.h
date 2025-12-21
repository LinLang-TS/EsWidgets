/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esListWidget.h
** @date 2025/11/23
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLISTWIDGET_H
#define ESLISTWIDGET_H

#include <QListWidget>

#include "esDef.h"
class EsListItemDelegate;
class EsListWidgetPrivate;

class ES_EXPORT EsListWidget : public QListWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsListWidget)
    Q_PROPERTY(bool selectRightClickedRow READ isSelectRightClickedRow WRITE setSelectRightClickedRow)

public:
    explicit EsListWidget(QWidget* parent = nullptr);
    ~EsListWidget();


    /**
     * 设置当前单元格项
     *
     * @param item 单元格项
     * @param command 选择标志
     */
    void setCurrentItem(QListWidgetItem* item, QItemSelectionModel::SelectionFlags command = nullptr);

    /**
     * 设置当前行
     *
     * @param row 行号
     * @param command 选择标志
     */
    void setCurrentRow(int row, QItemSelectionModel::SelectionFlags command = nullptr);

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


#endif //ESLISTWIDGET_H
