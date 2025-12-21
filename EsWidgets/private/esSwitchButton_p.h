/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esSwitchButton_p.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSWITCHBUTTON_P_H
#define ESSWITCHBUTTON_P_H
#include <QLabel>

#include "esDef.h"
class EsIndicator;
class EsSwitchButton;
class QHBoxLayout;

class EsSwitchButtonPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsSwitchButton)

public:
    explicit EsSwitchButtonPrivate(QObject* parent = nullptr);

    void initWidget();

    Q_SLOT void updateText();

    QString text;
    QString offText;
    QString onText;
    int spacing;
    QColor lightTextColor;
    QColor darkTextColor;
    Es::IndicatorPosition indicatorPos;
    QHBoxLayout* hBox;
    EsIndicator* indicator;
    QLabel* label;
};


#endif //ESSWITCHBUTTON_P_H
