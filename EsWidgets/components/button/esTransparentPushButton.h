/****************************************************************************
** @brief 透明按钮
** @details 这家伙很懒, 什么都没写
** @file esTransparentPushButton.h
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTRANSPARENTPUSHBUTTON_H
#define ESTRANSPARENTPUSHBUTTON_H

#include "esPushButton.h"

class ES_EXPORT EsTransparentPushButton : public EsPushButton
{
    Q_OBJECT
public:
    explicit EsTransparentPushButton(QWidget* parent = nullptr);
    explicit EsTransparentPushButton(const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentPushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentPushButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentPushButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);
};


#endif //ESTRANSPARENTPUSHBUTTON_H
