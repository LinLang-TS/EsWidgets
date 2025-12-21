/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleBarButtons.h
** @date 2025/10/2
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTITLEBARBUTTONS_H
#define ESTITLEBARBUTTONS_H
#include <QAbstractButton>
#include "esDef.h"


class EsTitleBarButtonBasePrivate;
class EsTitleBarButtonSvgPrivate;
class EsMaximizeButtonPrivate;

class ES_EXPORT EsTitleBarButtonBase : public QAbstractButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsTitleBarButtonBase)

    Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor)
    Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
    Q_PROPERTY(QColor pressedColor READ getPressedColor WRITE setPressedColor)

    Q_PROPERTY(QColor normalBackgroundColor READ getNormalBackgroundColor WRITE setNormalBackgroundColor)
    Q_PROPERTY(QColor hoverBackgroundColor READ getHoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor pressedBackgroundColor READ getPressedBackgroundColor WRITE setPressedBackgroundColor)

public:
    explicit EsTitleBarButtonBase(QWidget* parent=nullptr);

    // 设置按钮状态
    void setState(Es::TitleBarButtonState state);

    // 是否被按下
    bool isPressed() const;

    // 取正常状态下颜色
    QColor getNormalColor() const;
    QColor getHoverColor() const;
    QColor getPressedColor() const;
    QColor getNormalBackgroundColor() const;
    QColor getHoverBackgroundColor() const;
    QColor getPressedBackgroundColor() const;

    void setNormalColor(const QColor &color);
    void setHoverColor(const QColor &color);
    void setPressedColor(const QColor &color);
    void setNormalBackgroundColor(const QColor &color);
    void setHoverBackgroundColor(const QColor &color);
    void setPressedBackgroundColor(const QColor &color);

protected:
    void enterEvent(QEvent* event) override;

    void leaveEvent(QEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    QPair<QColor, QColor> getColors();
};


class ES_EXPORT EsTitleBarButtonSvg : public EsTitleBarButtonBase
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsTitleBarButtonSvg)

public:
    explicit EsTitleBarButtonSvg(const QString& iconPath, QWidget* parent=nullptr);


    void setIcon(const QString& iconPath);

protected:
    void paintEvent(QPaintEvent* e) override;
};


// 最小化按钮
class ES_EXPORT EsMinimizeButton : public EsTitleBarButtonBase
{
    Q_OBJECT

public:
    explicit EsMinimizeButton(QWidget* parent=nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;
};

// 最大化按钮
class ES_EXPORT EsMaximizeButton : public EsTitleBarButtonBase
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsMaximizeButton)

public:
    explicit EsMaximizeButton(QWidget* parent=nullptr);

    void setMaxState(bool isMax);

protected:
    void paintEvent(QPaintEvent* e) override;
};

// 关闭按钮
class ES_EXPORT EsCloseButton : public EsTitleBarButtonSvg
{
    Q_OBJECT
public:

    explicit EsCloseButton(const QString& iconPath, QWidget* parent=nullptr);
    explicit EsCloseButton(QWidget* parent=nullptr);

};


#endif //ESTITLEBARBUTTONS_H
