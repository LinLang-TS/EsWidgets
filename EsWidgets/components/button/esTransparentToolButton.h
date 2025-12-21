/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTransparentToolButton.h
** @date 2025/11/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTRANSPARENTTOOLBUTTON_H
#define ESTRANSPARENTTOOLBUTTON_H
#include "esToolButton.h"

class ES_EXPORT EsTransparentToolButton : public EsToolButton
{
    Q_OBJECT

public:
    explicit EsTransparentToolButton(QWidget* parent = nullptr);
    explicit EsTransparentToolButton(Es::IconName icon, QWidget* parent = nullptr);
    explicit EsTransparentToolButton(EsIconBase* icon, QWidget* parent = nullptr);
    explicit EsTransparentToolButton(const QString& icon, QWidget* parent = nullptr);
    explicit EsTransparentToolButton(const QIcon& icon, QWidget* parent = nullptr);
};


#endif //ESTRANSPARENTTOOLBUTTON_H
