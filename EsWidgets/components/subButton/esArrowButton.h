/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esArrowButton.h
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESARROWBUTTON_H
#define ESARROWBUTTON_H


#include <QToolButton>

#include "esDef.h"
#include "esGlobal.h"

class EsArrowButtonPrivate;
class EsIcon;

/**
 * @brief 箭头按钮（ArrowButton）
 * 
 * 一个带方向箭头图标的按钮
 * 支持浅色 / 深色主题、透明度调节等。
 */
class ES_EXPORT EsArrowButton : public QToolButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsArrowButton)

public:
    /**
     * @brief 构造函数
     * @param icon EsIconTypes 图标
     * @param parent 父控件
     */
    explicit EsArrowButton(Es::IconName icon, QWidget* parent = nullptr);
    ~EsArrowButton() override;
    /** 设置透明度（范围 0.0~1.0） */
    void setOpacity(qreal opacity);

    /** 设置浅色模式颜色 */
    void setLightColor(const QColor& color);

    /** 设置深色模式颜色 */
    void setDarkColor(const QColor& color);

protected:
    /**
     * @brief 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

private:
    QColor lightColor; // 浅色主题颜色
    QColor darkColor; // 深色主题颜色
    qreal opacity; // 透明度（0.0 ~ 1.0）
};
#endif //ESARROWBUTTON_H
