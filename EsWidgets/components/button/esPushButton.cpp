/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esPushButton.cpp
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esPushButton.h"

#include <QApplication>
#include <QPainter>

#include "esFont.h"
#include "esStyleSheet.h"
#include "private/esPushButton_p.h"


// region ================= EsPushButtonPrivate  =================

EsPushButtonPrivate::EsPushButtonPrivate(QObject* parent)
    : QObject(parent)
{
}


void EsPushButtonPrivate::setIcon(const QVariant& variant)
{
    Q_Q(EsPushButton);
    if (variant.isNull() || variant.type() == QVariant::Icon && icon.isNull())
    {
        q->setProperty("hasIcon", false);
    }
    else
    {
        q->setProperty("hasIcon", true);
    }
    q->setStyle(QApplication::style());
    // 如果之前设置过Es内置图标, 就先释放原本图标的内存;
    // 注意: delete后, icon.isValid()依然会返回true, 但是这里delete后直接就覆盖了icon, 所以没调用icon.clear()清空QVariant;
    // 如果其他地方也需要delete它的值, 且不直接覆盖, 要记得调用clear(), 不然怕出问题; 其他QVariant包含指针的地方同理
    if (icon.userType() == qMetaTypeId<EsIconBase*>()) delete icon.value<EsIconBase*>();
    icon = variant;
    q->update();
}

// endregion


// region ================= EsPushButton  =================

EsPushButton::EsPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new EsPushButtonPrivate())
{
    Q_D(EsPushButton);
    d->q_ptr = this;
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_BUTTON);
    styleSheet->apply(this);
    isPressed = false;
    isHover = false;
    setIconSize(QSize(16, 16));
    setIcon(QIcon());
    EsFunc::setFont(this);
}

EsPushButton::EsPushButton(const QString& text, QWidget* parent)
    : EsPushButton(parent)
{
    setText(text);
}


EsPushButton::EsPushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : EsPushButton(parent)
{
    setText(text);
    setIcon(icon);
}


EsPushButton::EsPushButton(Es::IconName icon, const QString& text, QWidget* parent)
    : EsPushButton(parent)
{
    // 构造函数的调用顺序是：
    // 初始化列表按继承层次和成员声明顺序正向执行;
    // 构造函数体按调用链反向执行。
    // 也就是说 调用这个构造后, 先从这个构造的初始化列表往上依次调用每个构造的初始化列表, 然后反着执行每个构造的函数体
    // 所以依次往上执行完初始化列表后, 第1个构造会先setIcon一个空的QIcon(),然后会被第2个构造的d->setIcon(variant)覆盖;
    // 所以这样写完全没问题
    setText(text);
    setIcon(icon);
}

EsPushButton::EsPushButton(EsIconBase* icon, const QString& text, QWidget* parent)
    : EsPushButton(parent)
{
    setText(text);
    setIcon(icon);
}

EsPushButton::~EsPushButton()
{
    Q_D(EsPushButton);
    if (d->icon.userType() == qMetaTypeId<EsIconBase*>()) delete d->icon.value<EsIconBase*>();

}


void EsPushButton::setIcon(const QIcon& icon)
{
    Q_D(EsPushButton);
    d->setIcon(QVariant::fromValue(icon));
}

void EsPushButton::setIcon(const QString& icon)
{
    Q_D(EsPushButton);
    d->setIcon(QVariant::fromValue(icon));
}

void EsPushButton::setIcon(Es::IconName icon)
{
    Q_D(EsPushButton);
    d->setIcon(QVariant::fromValue<EsIconBase*>(new EsIcon(icon)));
}

void EsPushButton::setIcon(EsIconBase* icon)
{
    Q_D(EsPushButton);
    d->setIcon(QVariant::fromValue(icon));
}

QIcon EsPushButton::icon()
{
    Q_D(EsPushButton);
    return EsFunc::toQIcon(d->icon);
}


void EsPushButton::_drawIcon(const QVariant& icon, QPainter* painter, const QRectF& rect, QIcon::State state)
{
    EsFunc::drawIcon(icon, painter, rect, state);
}


bool EsPushButton::setProperty(const char* name, const QVariant& value)
{
    if (strcmp(name, "icon") != 0)
    {
        // 如果属性名不是 "icon"，调用父类的 setProperty
        return QPushButton::setProperty(name, value);
    }

    // 如果是 "icon" 属性，调用 setIcon 方法
    setIcon(value.toString());
    return true;
}


void EsPushButton::mousePressEvent(QMouseEvent* e)
{
    isPressed = true;
    QPushButton::mousePressEvent(e);
}

void EsPushButton::mouseReleaseEvent(QMouseEvent* e)
{
    isPressed = false;
    QPushButton::mouseReleaseEvent(e);
}

void EsPushButton::enterEvent(QEvent* event)
{
    isHover = true;
    update();
}

void EsPushButton::leaveEvent(QEvent* event)
{
    isHover = false;
    update();
}

void EsPushButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);
    Q_D(EsPushButton);
    if (d->icon.isNull() || !d->icon.isValid()) return;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (!isEnabled())
    {
        painter.setOpacity(0.3628);
    }
    else if (isDown())
    {
        painter.setOpacity(0.786);
    }
    // 获取图标的宽高
    qreal w = iconSize().width();
    qreal h = iconSize().height();

    // 计算图标的纵坐标（垂直居中）
    qreal y = (height() - h) / 2;
    // 获取按钮的最小宽度
    qreal mw = minimumSizeHint().width();
    qreal x = 12; // 默认的左偏移量

    // 如果最小宽度大于0，居中图标
    if (mw > 0)
    {
        x = 12 + (width() - mw) / 2;
    }

    // 如果是右对齐，调整 x 坐标
    if (isRightToLeft())
    {
        x = width() - w - x;
    }
    _drawIcon(d->icon, &painter, QRectF(x, y, w, h));
}

// endregion
