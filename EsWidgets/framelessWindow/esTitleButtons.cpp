/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleButtons.cpp
** @date 2025/10/2
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/


#include "esTitleBarButtons.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtSvg/QSvgRenderer>
#include "esTitleBarButtons_p.h"


// region ================= 标题栏按钮Base =================
EsTitleBarButtonBase::EsTitleBarButtonBase(QWidget* parent)
    : QAbstractButton(parent), d_ptr(new EsTitleBarButtonBasePrivate())
{
    Q_D(EsTitleBarButtonBase);
    d->q_ptr = this;
    setCursor(Qt::ArrowCursor);
    setFixedSize(46, 32);

    d->state = Es::TitleBarButtonState::NORMAL;

    // icon color
    d->normalColor = QColor(0, 0, 0);
    d->hoverColor = QColor(0, 0, 0);
    d->pressedColor = QColor(0, 0, 0);
    // background color
    d->normalBgColor = QColor(0, 0, 0, 0);
    d->hoverBgColor = QColor(0, 0, 0, 26);
    d->pressedBgColor = QColor(0, 0, 0, 51);
}

E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, NormalColor, normalColor);
E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, HoverColor, hoverColor);
E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, PressedColor, pressedColor);
E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, NormalBackgroundColor, normalBgColor);
E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, HoverBackgroundColor, hoverBgColor);
E_Q_CREATE_GET_SET_UPDATE_CPP(EsTitleBarButtonBase, QColor, PressedBackgroundColor, pressedBgColor);

void EsTitleBarButtonBase::setState(Es::TitleBarButtonState state)
{
    Q_D(EsTitleBarButtonBase);
    d->state = state;
    update();
}

bool EsTitleBarButtonBase::isPressed() const
{
    Q_D(const EsTitleBarButtonBase);
    return d->state == Es::TitleBarButtonState::PRESSED;
}


void EsTitleBarButtonBase::enterEvent(QEvent* event)
{
    setState(Es::TitleBarButtonState::HOVER);
    QAbstractButton::enterEvent(event);
}

void EsTitleBarButtonBase::leaveEvent(QEvent* event)
{
    setState(Es::TitleBarButtonState::NORMAL);
    QAbstractButton::leaveEvent(event);
}

void EsTitleBarButtonBase::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    setState(Es::TitleBarButtonState::PRESSED);
    QAbstractButton::mousePressEvent(event);
}

QPair<QColor, QColor> EsTitleBarButtonBase::getColors()
{
    Q_D(EsTitleBarButtonBase);
    if (d->state == Es::TitleBarButtonState::NORMAL)
        return {d->normalColor, d->normalBgColor};

    else if (d->state == Es::TitleBarButtonState::HOVER)
        return {d->hoverColor, d->hoverBgColor};

    return {d->pressedColor, d->pressedBgColor};
}

// endregion

// region ================= 标题栏svg按钮 =================
EsTitleBarButtonSvg::EsTitleBarButtonSvg(const QString& iconPath, QWidget* parent)
    : EsTitleBarButtonBase(parent), d_ptr(new EsTitleBarButtonSvgPrivate())
{
    Q_D(EsTitleBarButtonSvg);
    d->q_ptr = this;
    setIcon(iconPath);
}

void EsTitleBarButtonSvg::setIcon(const QString& iconPath)
{
    Q_D(EsTitleBarButtonSvg);
    auto f = QFile(iconPath);
    f.open(QFile::ReadOnly);
    d->svgDom.setContent(f.readAll());
    f.close();
}

void EsTitleBarButtonSvg::paintEvent(QPaintEvent* e)
{
    Q_D(EsTitleBarButtonSvg);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    auto tmp = getColors();
    auto color = tmp.first;
    auto bgColor = tmp.second;

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    auto colorName = color.name();
    auto pathNodes = d->svgDom.elementsByTagName("path");
    for (int i = 0; i < pathNodes.length(); ++i)
    {
        auto element = pathNodes.at(i).toElement();
        element.setAttribute("stroke", colorName);
    }

    auto renderer = QSvgRenderer(d->svgDom.toByteArray());
    renderer.render(&painter, QRectF(rect()));
}

// endregion


// region ================= 标题栏最小化按钮 =================
EsMinimizeButton::EsMinimizeButton(QWidget* parent)
    : EsTitleBarButtonBase(parent)
{
}

void EsMinimizeButton::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    auto tmp = getColors();
    auto color = tmp.first;
    auto bgColor = tmp.second;

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    painter.setBrush(Qt::NoBrush);
    auto pen = QPen(color, 1);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.drawLine(18, 16, 28, 16);
}

// endregion


// region ================= 标题栏最大化按钮 =================
EsMaximizeButton::EsMaximizeButton(QWidget* parent)
    : EsTitleBarButtonBase(parent), d_ptr(new EsMaximizeButtonPrivate())
{
    Q_D(EsMaximizeButton);
    d->q_ptr = this;
}

void EsMaximizeButton::setMaxState(bool isMax)
{
    Q_D(EsMaximizeButton);
    if (d->isMax == isMax) return;
    d->isMax = isMax;
    setState(Es::TitleBarButtonState::NORMAL);

}

void EsMaximizeButton::paintEvent(QPaintEvent* e)
{
    Q_D(EsMaximizeButton);
    QPainter painter(this);
    auto tmp = getColors();
    auto color = tmp.first;
    auto bgColor = tmp.second;

    // draw background
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    // draw icon
    painter.setBrush(Qt::NoBrush);
    auto pen = QPen(color, 1);
    pen.setCosmetic(true);
    painter.setPen(pen);

    auto r = devicePixelRatioF();
    painter.scale(1 / r, 1 / r);
    if (d->isMax)
    {
        painter.drawRect(int(18 * r), int(11 * r), int(10 * r), int(10 * r));
    }
    else
    {
        painter.drawRect(int(18 * r), int(13 * r), int(8 * r), int(8 * r));
        auto x0 = int(18 * r) + int(2 * r);
        auto y0 = 13 * r;
        auto dw = int(2 * r);
        auto path = QPainterPath(QPointF(x0, y0));
        path.lineTo(x0, y0 - dw);
        path.lineTo(x0 + 8 * r, y0 - dw);
        path.lineTo(x0 + 8 * r, y0 - dw + 8 * r);
        path.lineTo(x0 + 8 * r - dw, y0 - dw + 8 * r);
        painter.drawPath(path);
    }
}


// endregion

// region ================= 标题栏关闭按钮 =================
EsCloseButton::EsCloseButton(const QString& iconPath, QWidget* parent)
    : EsTitleBarButtonSvg(iconPath, parent)
{
    setHoverColor(Qt::white);
    setPressedColor(Qt::white);
    setHoverBackgroundColor(QColor(232, 17, 35));
    setPressedBackgroundColor(QColor(241, 112, 122));
}

EsCloseButton::EsCloseButton(QWidget* parent)
    :EsCloseButton(":/esWidgets/images/close.svg", parent)
{
}

// endregion

