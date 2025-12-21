/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esAnimation.h
** @date 2025/11/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESANIMATION_H
#define ESANIMATION_H
#include <QColor>
#include <QEvent>
#include <QLineEdit>
#include <QPropertyAnimation>

#include "esConfig.h"
#include "esDef.h"
#include "widgets/esWidget.h"

class QPropertyAnimation;
class EsBackgroundColorObject;
class QMouseEvent;
class QFocusEvent;
class QEnterEvent;


class ES_EXPORT EsAnimationBase : public QObject
{
    Q_OBJECT
public:
    explicit EsAnimationBase(QWidget* parent);

protected:
    // ---- 可被子类重写的事件处理函数 ----
    virtual void onHover(QEnterEvent* e) { Q_UNUSED(e); }
    virtual void onLeave(QEvent* e) { Q_UNUSED(e); }
    virtual void onPress(QMouseEvent* e) { Q_UNUSED(e); }
    virtual void onRelease(QMouseEvent* e) { Q_UNUSED(e); }

    // ---- 事件过滤器 ----
    bool eventFilter(QObject* obj, QEvent* e) override;
};

class ES_EXPORT EsTranslateYAnimation : public EsAnimationBase
{
    Q_OBJECT
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY valueChanged)

public:
    explicit EsTranslateYAnimation(QWidget* parent, float offset = 2.0f);

    float getY() const;

    void setY(float value);

signals:
        void valueChanged(float value);

protected:

    void onPress(QMouseEvent* e) override;

    void onRelease(QMouseEvent* e) override;

private:
    float y;
    float maxOffset;
    QPropertyAnimation* ani;
};

class ES_EXPORT EsBackgroundAnimationBase
{
public:
    virtual ~EsBackgroundAnimationBase() = default;
    virtual QColor normalBackgroundColor() const = 0;

};
/**
 * @class EsBackgroundAnimationWidget
 * @brief 提供背景颜色动画的控件基类。
 *
 * 该类通过对鼠标事件、焦点事件和启用状态的检测，动态更新背景颜色。
 * 支持通过 QPropertyAnimation 实现平滑的背景色变化动画。
 *
 * @tparam T 父类控件类型，如 QWidget 或 QFrame。
 */
class ES_EXPORT EsBackgroundAnimationWidget:public EsWidget, public EsBackgroundAnimationBase
{
    Q_OBJECT
public:
    explicit EsBackgroundAnimationWidget(QWidget* parent=nullptr);

    /**
     * @brief 更新背景颜色。
     * 根据控件的启用状态、焦点状态、鼠标按下状态等更新背景颜色。
     * 使用 QPropertyAnimation 平滑过渡背景颜色变化。
     */
    Q_SLOT void updateBackgroundColor();

    /**
     * @brief 设置背景颜色。
     *
     * @param color 要设置的背景颜色。
     */
    void setBackgroundColor(const QColor& color);
    /**
     * @brief 获取默认背景颜色。
     *
     * @return QColor 默认背景颜色。
     */
    virtual QColor normalBackgroundColor() const override;
protected:
    /**
     * @brief 获取鼠标悬停时的背景颜色。
     *
     * @return QColor 鼠标悬停时的背景颜色，默认为 normalBackgroundColor。
     */
    virtual QColor hoverBackgroundColor() const;

    /**
     * @brief 获取鼠标按下时的背景颜色。
     *
     * @return QColor 鼠标按下时的背景颜色，默认为 normalBackgroundColor。
     */
    virtual QColor pressedBackgroundColor() const;

    /**
     * @brief 获取获得焦点时的背景颜色。
     *
     * @return QColor 焦点时的背景颜色，默认为 normalBackgroundColor。
     */
    QColor focusInBackgroundColor() const;

    /**
     * @brief 获取禁用状态下的背景颜色。
     *
     * @return QColor 禁用状态下的背景颜色，默认为 normalBackgroundColor。
     */
    QColor disabledBackgroundColor() const;

    /**
     * @brief 获取当前背景颜色。
     *
     * @return QColor 当前背景颜色。
     */
    QColor getBackgroundColor() const;

    bool eventFilter(QObject* obj, QEvent* e) override;

protected:
     void mousePressEvent(QMouseEvent* e) override;


    void mouseReleaseEvent(QMouseEvent* e) override;

    void enterEvent(QEvent* e) override;

    void leaveEvent(QEvent* e) override;

    void focusInEvent(QFocusEvent* e) override;

private:
    EsBackgroundColorObject* bgColorObject;  ///< 背景颜色对象，用于管理背景颜色
    QPropertyAnimation* backgroundColorAni;  ///< 背景颜色动画对象
    bool isHover;  ///< 鼠标是否悬停
    bool isPressed;  ///< 鼠标是否按下
};


class ES_EXPORT EsBackgroundAnimationFrame:public QFrame, public EsBackgroundAnimationBase
{
    Q_OBJECT
public:
    explicit EsBackgroundAnimationFrame(QWidget* parent=nullptr);

    Q_SLOT void updateBackgroundColor();

    void setBackgroundColor(const QColor& color);

    virtual QColor normalBackgroundColor() const override;
    
protected:
    virtual QColor hoverBackgroundColor() const;
    virtual QColor pressedBackgroundColor() const;
    QColor focusInBackgroundColor() const;
    QColor disabledBackgroundColor() const;
    QColor getBackgroundColor() const;

    bool eventFilter(QObject* obj, QEvent* e) override;

protected:
     void mousePressEvent(QMouseEvent* e) override;

    void mouseReleaseEvent(QMouseEvent* e) override;

    void enterEvent(QEvent* e) override;

    void leaveEvent(QEvent* e) override;

    void focusInEvent(QFocusEvent* e) override;

private:
    EsBackgroundColorObject* bgColorObject;
    QPropertyAnimation* backgroundColorAni;
    bool isHover;
    bool isPressed;
};

/**
 * @brief 背景颜色对象
 *
 * 该类用于管理背景颜色，提供 `backgroundColor` 属性，可以获取和设置背景颜色。
 * 设置新的背景颜色时，会触发父级部件的更新。
 */
class ES_EXPORT EsBackgroundColorObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)

public:
    explicit EsBackgroundColorObject(QObject* parent = nullptr);

    QColor getBackgroundColor() const;

public slots:
    void setBackgroundColor(const QColor& color);

private:
    QColor backgroundColor;
};


#endif //ESANIMATION_H
