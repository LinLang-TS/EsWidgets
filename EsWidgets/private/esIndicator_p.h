/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIndicator_p.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESINDICATOR_P_H
#define ESINDICATOR_P_H
#include <QColor>

#include "esDef.h"

class EsIndicator;
class QPropertyAnimation;
class EsIndicatorPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsIndicator)
public:
    explicit EsIndicatorPrivate(QObject *parent=nullptr);
    Q_SLOT void toggleSlider();

    // 绘制背景
    void drawBackground(QPainter* painter);

    // 绘制圆形
    void drawCircle(QPainter* painter);

    // 获取背景颜色
    QColor backgroundColor();

    // 获取边框颜色
    QColor borderColor();

    // 获取滑块的颜色;
    QColor sliderColor();
private:
    QColor lightCheckedColor;
    QColor darkCheckedColor;
    float sliderX;
    QPropertyAnimation* slideAni;
};

#endif //ESINDICATOR_P_H
