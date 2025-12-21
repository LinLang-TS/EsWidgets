/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIndicator.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESINDICATOR_H
#define ES_ESINDICATOR_H
#include "esDef.h"
#include "button/esToolButton.h"
class EsIndicatorPrivate;

/**
 * EsSwitchButton开关指示器
 */
class ES_EXPORT EsIndicator : public EsToolButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsIndicator)
    Q_PROPERTY(float sliderX READ getSliderX WRITE setSliderX)

Q_SIGNALS:
    void checkedChanged(bool);

public:
    explicit EsIndicator(QWidget* parent = nullptr);

    ~EsIndicator();

    Q_SLOT void toggle();

    void setDown(bool isDown);

    void setHover(bool isHover);

    /**
     * @brief 设置选中状态下的颜色
     *
     * @param light 主题模式下的指示器颜色（浅色）todo light, dark: str | QColor | Qt.GlobalColor ;
     * @param dark 主题模式下的指示器颜色（深色）
     */
    void setCheckedColor(const QColor& light, const QColor& dark);

    float getSliderX() const;

    void setSliderX(float x);

protected:
    // 鼠标释放时切换选中状态
    void mouseReleaseEvent(QMouseEvent* e) override;

    void paintEvent(QPaintEvent* e) override;
};


#endif //ES_ESINDICATOR_H
