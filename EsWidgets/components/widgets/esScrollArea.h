/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScrollArea.h
** @date 2025/12/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSCROLLAREA_H
#define ESSCROLLAREA_H
#include <QScrollArea.h>

#include "delegate/esSmoothScrollDelegate.h"


/**
 * @brief 平滑滚动区域
 *
 * 该类表示一个支持平滑滚动效果的滚动区域。
 */
class EsScrollArea : public QScrollArea {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     *
     * 初始化滚动区域并设置平滑滚动委托。
     *
     * @param parent 父窗口部件
     */
    explicit EsScrollArea(QWidget* parent = nullptr);

    /**
     * @brief 设置平滑滚动模式
     *
     * 设置滚动区域的平滑滚动模式及其方向。
     *
     * @param mode 平滑滚动模式
     * @param orientation 滚动方向
     */
    void setSmoothMode(Es::SmoothMode mode, Qt::Orientation orientation);

    /**
     * @brief 启用透明背景
     *
     * 设置滚动区域及其内部小部件为透明背景。
     */
    void enableTransparentBackground();

public:
    EsSmoothScrollDelegate* scrollDelegate; ///< 平滑滚动委托
};



#endif //ESSCROLLAREA_H
