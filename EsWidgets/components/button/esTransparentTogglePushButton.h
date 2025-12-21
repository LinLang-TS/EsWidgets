/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTransparentTogglePushButton.h
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTRANSPARENTTOGGLEPUSHBUTTON_H
#define ESTRANSPARENTTOGGLEPUSHBUTTON_H

#include "esTogglePushButton.h"
class ES_EXPORT EsTransparentTogglePushButton : public EsTogglePushButton
{
    Q_OBJECT
public:
    explicit EsTransparentTogglePushButton(QWidget* parent = nullptr);
    explicit EsTransparentTogglePushButton(const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentTogglePushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentTogglePushButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTransparentTogglePushButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);

};




#endif //ESTRANSPARENTTOGGLEPUSHBUTTON_H
