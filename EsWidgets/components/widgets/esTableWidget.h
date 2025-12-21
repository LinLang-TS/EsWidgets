/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableWidget.h
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTABLEWIDGET_H
#define ESTABLEWIDGET_H

#include "esDef.h"
#include <QTableWidget>

class EsTableWidgetPrivate;
class EsTableItemDelegate;

class ES_EXPORT EsTableWidget : public QTableWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsTableWidget);
    Q_PROPERTY(bool selectRightClickedRow READ isSelectRightClickedRow WRITE setSelectRightClickedRow)
public :
    explicit EsTableWidget(QWidget* parent = nullptr);
    explicit EsTableWidget(int rows, int columns, QWidget* parent = nullptr);
    ~EsTableWidget() override;
    /**
       * 设置当前单元格
       *
       * @param row 行号
       * @param column 列号
       * @param command 选择标志
       */
    void setCurrentCell(int row, int column, QItemSelectionModel::SelectionFlags command = nullptr);

    /**
     * 设置当前单元格项
     *
     * @param item 单元格项
     * @param command 选择标志
     */
    void setCurrentItem(QTableWidgetItem* item, QItemSelectionModel::SelectionFlags command = nullptr);

    /**
     * 设置 item 为 EsComboBox, 并添加选项
     *
     * @param item 单元格项
     * @param options EsComboBox的选项
     * @param defaultText (可选) 单元格项显示的默认文本
     */
    void setItemComBoBox(QTableWidgetItem* item, const QStringList& options, const QString& defaultText={});

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


#endif //ESTABLEWIDGET_H
