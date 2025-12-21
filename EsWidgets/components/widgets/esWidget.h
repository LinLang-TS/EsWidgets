/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWidget.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWIDGET_H
#define ESWIDGET_H
#include "esFramelessWindow.h"
class EsWidgetPrivate;

class ES_EXPORT EsWidget : public EsAcrylicWindow
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsWidget)

public:
    explicit EsWidget(QWidget* parent = nullptr);
    ~EsWidget();

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};


#endif //ESWIDGET_H
