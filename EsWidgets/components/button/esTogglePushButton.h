/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTogglePushButton.h
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTOGGLEPUSHBUTTON_H
#define ESTOGGLEPUSHBUTTON_H
#include "esToggleButton.h"
class ES_EXPORT EsTogglePushButton : public EsToggleButton
{
    Q_OBJECT
public:
    explicit EsTogglePushButton(QWidget* parent = nullptr);
    explicit EsTogglePushButton(const QString& text, QWidget* parent = nullptr);
    explicit EsTogglePushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTogglePushButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsTogglePushButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);

};

 


#endif //ESTOGGLEPUSHBUTTON_H
