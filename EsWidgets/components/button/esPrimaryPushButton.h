/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPrimaryPushButton.h
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESPRIMARYPUSHBUTTON_H
#define ESPRIMARYPUSHBUTTON_H
#include <QObject>

#include "esPushButton.h"

class ES_EXPORT EsPrimaryPushButton : public EsPushButton
{
    Q_OBJECT

public:
    explicit EsPrimaryPushButton(QWidget* parent = nullptr);
    explicit EsPrimaryPushButton(const QString& text, QWidget* parent = nullptr);
    explicit EsPrimaryPushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsPrimaryPushButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsPrimaryPushButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);

protected:
    void _drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state=QIcon::Off) override;
};


#endif //ES_ESPRIMARYPUSHBUTTON_H
