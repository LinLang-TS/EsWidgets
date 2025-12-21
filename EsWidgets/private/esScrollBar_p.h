/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScrollBar_p.h
** @date 2025/11/11
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef  ESSCROLLBAR_P_H
#define  ESSCROLLBAR_P_H
#include "esDef.h"
#include <QCompleter>
#include <QColor>
class EsScrollBar;


class EsScrollBarPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsScrollBar)

public:
    explicit EsScrollBarPrivate(QObject* parent = nullptr);
    // 初始化控件
    void initWidget(QAbstractScrollArea* parent);

    // 页面上翻
    Q_SLOT void onPageUp();

    // 页面下翻
    Q_SLOT void onPageDown();

    // 处理值变化
    Q_SLOT void onValueChanged(int val);

    // 调整滚动条位置
    void adjustPos(const QSize& size);

    // 调整滑块的大小
    void adjustHandleSize();

    // 调整滑块的位置
    void adjustHandlePos();

    int grooveLength() const;

    int slideLength() const;

    bool isSlideResion(const QPoint& pos) const;

    Q_SLOT void onOpacityAniValueChanged();


    Qt::Orientation orientation; // 滚动条方向
    int singleStep; // 单步步长
    int pageStep; // 页面步长
    int padding; // 内边距
    int minimum; // 最小值
    int maximum; // 最大值
    int value; // 当前值

    bool isPressed; // 滑块是否被按下
    bool isEnter; // 是否鼠标进入
    bool isExpanded; // 滚动条是否扩展
    QPoint pressedPos; // 按下位置
    bool isForceHidden; // 是否强制隐藏
};

class EsSmoothScrollBar;

class EsSmoothScrollBarPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsSmoothScrollBar)

public:
    explicit EsSmoothScrollBarPrivate(QObject* parent = nullptr);
    int value; // 当前值
};

#endif // ESSCROLLBAR_P_H
