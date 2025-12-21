/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esMenuActionListWidget.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESMENUACTIONLISTWIDGET_H
#define ESMENUACTIONLISTWIDGET_H

#include <QListWidget>

#include "esDef.h"
#include "esGlobal.h"
#include "delegate/esMenuItemDelegate.h"

class EsMenuActionListWidgetPrivate;
class ES_EXPORT EsMenuActionListWidget : public QListWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsMenuActionListWidget)

public:
    explicit EsMenuActionListWidget(QWidget* parent = nullptr);

    ~EsMenuActionListWidget();

    /**
     * @brief 在指定行插入菜单项
     * @param row 行号
     * @param item 列表项
     */
    void insertItem(int row, QListWidgetItem* item);

    /**
     * @brief 在列表末尾添加菜单项
     * @param item 列表项
     */
    void addItem(QListWidgetItem* item);

    /**
     * @brief 从列表中删除指定行的项
     * @param row 行号
     * @return 被移除的列表项
     */
    QListWidgetItem* takeItem(int row);

    /**
     * @brief 调整列表控件大小
     * @param pos 可选位置（默认为 nullptr）
     * @param aniType 动画类型，默认为 MenuAnimationType::NONE
     */
    void adjustSize(const QPoint& pos = {}, Es::MenuAnimationType aniType = Es::MenuAnimationType::NONE);

    /**
     * @brief 设置列表项的高度
     * @param height 高度值
     */
    void setItemHeight(int height);

    /**
     * @brief 设置最大可见项数
     * @param num 最大可见项数
     */
    void setMaxVisibleItems(int num);

    /**
     * @brief 获取最大可见项数
     * @return 最大可见项数
     */
    int maxVisibleItems() const;
    /**
     * @brief 获取动画时的高度
     * @param pos 位置
     * @param aniType 动画类型
     * @return 动画高度
     */
    int heightForAnimation(const QPoint& pos, Es::MenuAnimationType aniType);// const;
    /**
     * @brief 获取所有列表项的总高度
     * @return 总高度
     */
    int itemsHeight() const;

    QMargins viewportMargins() const;
    void setViewportMargins(int left, int top, int right, int bottom);


};
Q_DECLARE_METATYPE(EsMenuActionListWidget*)


#endif //ESMENUACTIONLISTWIDGET_H
