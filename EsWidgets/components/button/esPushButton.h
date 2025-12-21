/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPushButton.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESPUSHBUTTON_H
#define ES_ESPUSHBUTTON_H

#include <QPushButton>

#include "esGlobal.h"
#include "esIcon.h"

class EsPushButtonPrivate;

class ES_EXPORT EsPushButton : public QPushButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsPushButton)

public:
    explicit EsPushButton(QWidget* parent = nullptr);
    explicit EsPushButton(const QString& text, QWidget* parent = nullptr);
    explicit EsPushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    explicit EsPushButton(Es::IconName icon, const QString& text, QWidget* parent = nullptr);
    explicit EsPushButton(EsIconBase* icon, const QString& text, QWidget* parent = nullptr);
    ~EsPushButton() override;

    /**
     *
     * @param icon 可传 [QIcon, str, EsIconTypes::IconName, 自定义继承了EsIconBase类的指针(内部会管理内存释放)]
     */
    void setIcon(const QIcon& icon);
    void setIcon(const QString& icon);
    void setIcon(Es::IconName icon);
    void setIcon(EsIconBase* icon);

    QIcon icon();
    bool setProperty(const char* name, const QVariant& value);



protected:

    virtual void _drawIcon(const QVariant &icon, QPainter* painter, const QRectF& rect, QIcon::State state=QIcon::Off);

    void mousePressEvent(QMouseEvent* e) override;

    void mouseReleaseEvent(QMouseEvent* e) override;

    void enterEvent(QEvent* event) override;

    void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

public:
    bool isPressed;
    bool isHover;
};



#endif //ES_ESPUSHBUTTON_H
