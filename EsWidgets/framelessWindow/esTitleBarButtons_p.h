/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleButtons_p.h
** @date 2025/10/2
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTITLEBUTTONS_P_H
#define ESTITLEBUTTONS_P_H
#include <QtXml/QDomDocument>
#include "esDef.h"


class EsTitleBar;
class EsTitleBarButtonBase;
class EsTitleBarButtonSvg;
class EsMaximizeButton;

// 标题栏按钮基类
class EsTitleBarButtonBasePrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsTitleBarButtonBase)

public:
    explicit EsTitleBarButtonBasePrivate(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    Es::TitleBarButtonState state;

    QColor normalColor;
    QColor hoverColor;
    QColor pressedColor;

    QColor normalBgColor;
    QColor hoverBgColor;
    QColor pressedBgColor;
};

// 标题栏Svg按钮
class EsTitleBarButtonSvgPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsTitleBarButtonSvg)

public:
    explicit EsTitleBarButtonSvgPrivate(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    QDomDocument svgDom;
};

// 最大化按钮
class EsMaximizeButtonPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsMaximizeButton)

public:
    explicit EsMaximizeButtonPrivate(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    bool isMax;
};


#endif //ESTITLEBUTTONS_P_H
