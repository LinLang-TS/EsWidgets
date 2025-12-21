/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esToggleButton.h
** @date 2025/10/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTOGGLEBUTTON_H
#define ESTOGGLEBUTTON_H


#include "esPushButton.h"

class ES_EXPORT EsToggleButton : public EsPushButton
{
    Q_OBJECT
public:
    explicit EsToggleButton(QWidget* parent = nullptr);
    explicit EsToggleButton(const QString& text, QWidget* parent = nullptr);
    explicit EsToggleButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsToggleButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsToggleButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);
protected:
    void _drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state) override;
private:
    void _postInit();

};



#endif //ESTOGGLEBUTTON_H
