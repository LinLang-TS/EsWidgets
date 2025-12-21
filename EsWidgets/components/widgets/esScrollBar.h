/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScrollBar.h
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSCROLLBAR_H
#define ESSCROLLBAR_H

#include "esDef.h"
#include <QWidget>
#include <QPropertyAnimation>
#include <QColor>
#include <QScrollBar>

class EsArrowButton;
class QAbstractScrollArea;
class EsScrollBarPrivate;
class EsSmoothScrollBarPrivate;


/**
 * @brief 滚动条轨道（ScrollBarGroove）
 *
 * 用于实现带有上下箭头的滚动条轨道，支持淡入淡出动画。
 */
class ES_EXPORT EsScrollBarGroove : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float opacity READ getOpacity WRITE setOpacity)

public:
    /**
     * @brief 构造函数
     * @param orient 滚动条的方向（Qt::Vertical 或 Qt::Horizontal）
     * @param parent 父控件
     */
    explicit EsScrollBarGroove(Qt::Orientation orient, QWidget* parent = nullptr);

    /**
     * @brief 设置浅色背景色
     * @param color 浅色背景色
     */
    void setLightBackgroundColor(const QColor& color);

    /**
     * @brief 设置深色背景色
     * @param color 深色背景色
     */
    void setDarkBackgroundColor(const QColor& color);

    /**
     * @brief 执行淡入动画
     */
    void fadeIn() const;

    /**
     * @brief 执行淡出动画
     */
    void fadeOut() const;

    /**
     * @brief 获取当前的透明度
     * @return 当前透明度
     */
    float getOpacity() const;

    /**
     * @brief 设置透明度
     * @param opacity 透明度
     */
    void setOpacity(float opacity);

protected:
    /**
     * @brief 绘制事件
     *
     * 用于绘制滚动条轨道的背景
     */
    void paintEvent(QPaintEvent* event) override;

public:
    EsArrowButton* upButton; // 上箭头按钮
    EsArrowButton* downButton; // 下箭头按钮
    QPropertyAnimation* opacityAni; // 透明度动画
    QColor lightBackgroundColor; // 浅色背景颜色
    QColor darkBackgroundColor; // 深色背景颜色
private:
    float opacity; // 当前透明度
};


/**
 * @brief 滚动条滑块
 *
 * 用于实现滚动条的滑块，支持淡入淡出动画。
 */
class ES_EXPORT EsScrollBarHandle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float opacity READ getOpacity WRITE setOpacity)

public:
    /**
     * @brief 构造函数
     * @param orient 滚动条的方向（Qt::Vertical 或 Qt::Horizontal）
     * @param parent 父控件
     */
    explicit EsScrollBarHandle(Qt::Orientation orient, QWidget* parent = nullptr);

    /**
     * @brief 设置浅色颜色
     * @param color 浅色颜色
     */
    void setLightColor(const QColor& color);

    /**
     * @brief 设置深色颜色
     * @param color 深色颜色
     */
    void setDarkColor(const QColor& color);

    /**
     * @brief 获取当前透明度
     * @return 当前透明度
     */
    float getOpacity() const;

    /**
     * @brief 设置透明度
     * @param opacity 透明度
     */
    void setOpacity(float opacity);

    /**
     * @brief 执行淡入动画
     */
    void fadeIn() const;

    /**
     * @brief 执行淡出动画
     */
    void fadeOut() const;

protected:
    /**
     * @brief 绘制事件
     *
     * 用于绘制滑块的外观
     */
    void paintEvent(QPaintEvent* event) override;

private:
    QPropertyAnimation* opacityAni; // 透明度动画
    float opacity; // 当前透明度
    Qt::Orientation orient; // 滚动条方向（垂直或水平）
    QColor lightColor; // 浅色
    QColor darkColor; // 深色
};


/**
 * @brief 滚动条类
 *
 * 提供流畅的滚动条动画，支持自定义颜色、显示模式等功能。
 */
class ES_EXPORT EsScrollBar : public QWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsScrollBar)
    Q_PROPERTY(int val READ value WRITE setVal NOTIFY valueChanged)

Q_SIGNALS:
    // 滚动条范围变化时发射信号
    void rangeChanged(int min, int max);

    // 滚动条值变化时发射信号
    void valueChanged(int value);

    // 滑块按下时发射信号
    void sliderPressed();

    // 滑块释放时发射信号
    void sliderReleased();

    // 滑块移动时发射信号
    void sliderMoved();

public:
    /**
     * @brief 构造函数
     * @param orient 滚动条方向（Qt::Vertical 或 Qt::Horizontal）
     * @param parent 父控件
     */
    EsScrollBar(Qt::Orientation orient, QAbstractScrollArea* parent);
    ~EsScrollBar() override;

    int val() const;
    // 获取和设置滚动条的值
    int value() const;

    // 获取和设置滚动条的最小值
    int minimum() const;
    int maximum() const;
    // 获取滚动条的方向
    Qt::Orientation orientation() const;
    int pageStep() const;

    int singleStep() const;

    bool isSliderDown() const;

    void setVal(int value);

    /**
     * 设置滚动条的值
     * @param value 滚动条的目标值
     * @param useAni 此类的这个参数无效, 只是为了给子类重写提供的
     */
    virtual void setValue(int value, bool useAni = true);

    void setMinimum(int min);

    // 获取和设置滚动条的最大值
    void setMaximum(int max);

    // 设置滚动条的范围
    Q_SLOT void setRange(int min, int max);

    // 设置步长
    void setPageStep(int step);

    void setSingleStep(int step);


    void setSliderDown(bool isDown);


    /**
     * 设置滑块颜色
     * @param light todo QColor | 字符串 | Qt.GlobalColor
     * @param dark  颜色值，分别用于亮色模式和暗色模式
     */
    void setHandleColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置箭头按钮的颜色
     *
     * @param light 亮色模式下的颜色
     * @param dark 暗色模式下的颜色
     */
    void setArrowColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置轨道的颜色
     * @param light 亮色模式下的颜色
     * @param dark 暗色模式下的颜色
     */
    void setGrooveColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置滑块的显示模式
     *
     * @param mode 滑块的显示模式
     */
    void setHandleDisplayMode(Es::ScrollBarHandleDisplayMode mode);

    /**
     * @brief 展开滚动条
     */
    void expand();

    /**
     * @brief 收起滚动条
     */
    void collapse();

    /**
     * @brief 设置是否强制隐藏滚动条
     * @param isHidden 是否隐藏
     */
    void setForceHidden(bool isHidden);

    /**
     * @brief 处理鼠标滚轮事件
     * 将事件转发到父组件的视口
     * @param e 滚轮事件
     */
    void wheelEvent(QWheelEvent* e) override;

protected:
    // 当鼠标进入滚动条区域时触发
    void enterEvent(QEvent* e) override;

    // 当鼠标离开滚动条区域时触发
    void leaveEvent(QEvent* e) override;

    bool eventFilter(QObject* obj, QEvent* e) override;

    void resizeEvent(QResizeEvent* event) override;

    void mousePressEvent(QMouseEvent* e) override;

    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

protected:
    EsScrollBarGroove* groove; // 滚动条槽
    EsScrollBarHandle* handle; // 滑块

    Es::ScrollBarHandleDisplayMode handleDisplayMode; // 滑块显示模式
    QScrollBar* partnerBar; // 与此滚动条配对的滚动条
};

class ES_EXPORT EsSmoothScrollBar : public EsScrollBar
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsSmoothScrollBar)

public:
    explicit EsSmoothScrollBar(Qt::Orientation orient, QAbstractScrollArea* parent = nullptr);
    ~EsSmoothScrollBar() override;

    /**
     * @brief 设置滚动条的值
     *
     * @param value 滚动条的目标值
     * @param useAni 是否使用动画
     */
    void setValue(int value, bool useAni = true) override;

    /**
     * @brief 滚动指定的距离
     *
     * @param value 滚动的值
     * @param useAni 是否使用动画
     */
    void scrollValue(int value, bool useAni = true);

    /**
     * @brief 滚动到指定的位置
     *
     * @param value 滚动到的目标位置
     * @param useAni 是否使用动画
     */
    void scrollTo(int value, bool useAni = true);

    /**
     * @brief 重置滑块的值
     *
     * @param value 新的值
     */
    void resetValue(int value);

    /**
     * @brief 设置滚动动画
     *
     * @param duration 动画持续时间
     * @param easing 动画的缓动曲线类型
     */
    void setScrollAnimation(int duration, QEasingCurve easing = QEasingCurve::OutCubic);

protected:
    // 停止动画并记录当前值
    void mousePressEvent(QMouseEvent* e) override;

    // 停止动画并记录当前值
    void mouseMoveEvent(QMouseEvent* e) override;

public:
    int duration;
    QPropertyAnimation* ani;
};
#endif //ESSCROLLBAR_H
