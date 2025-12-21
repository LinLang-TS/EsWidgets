/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esToolButton.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESTOOLBUTTON_H
#define ES_ESTOOLBUTTON_H

#include "esDef.h"
#include <QToolButton>

class EsIconBase;
class EsToolButtonPrivate;

class ES_EXPORT EsToolButton : public QToolButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsToolButton)

public:
    explicit EsToolButton(QWidget* parent = nullptr);
    explicit EsToolButton(Es::IconName icon, QWidget* parent = nullptr);
    explicit EsToolButton(EsIconBase* icon, QWidget* parent = nullptr);
    explicit EsToolButton(const QString& icon, QWidget* parent = nullptr);
    explicit EsToolButton(const QIcon& icon, QWidget* parent = nullptr);
    ~EsToolButton() override;

    /**
     * 设置图标
     * @param icon 可传 [QIcon, str, EsIconTypes::IconName, EsIconBase*(内部会管理内存释放)]
     */
    void setIcon(const QIcon& icon);
    void setIcon(const QString& icon);
    void setIcon(Es::IconName icon);
    void setIcon(EsIconBase* icon);
    QIcon icon();
    bool setProperty(const char* name, const QVariant& value);

protected:
    virtual void _drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state = QIcon::Off);

    void mousePressEvent(QMouseEvent* e) override;

    void mouseReleaseEvent(QMouseEvent* e) override;

    void enterEvent(QEvent* event) override;

    void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

protected:
    bool isPressed;
    bool isHover;
};


#endif //ES_ESTOOLBUTTON_H
