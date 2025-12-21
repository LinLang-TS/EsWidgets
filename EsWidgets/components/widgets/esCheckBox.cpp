/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esCheckBox.cpp
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esCheckBox.h"

#include <QPainter>
#include <QStyleOptionButton>

#include "esFont.h"
#include "esStyleSheet.h"
#include "private/esCheckBox_p.h"

// region ================= EsCheckBoxPrivate  =================

EsCheckBoxPrivate::EsCheckBoxPrivate(QObject* parent)
    : QObject(parent),
      isPressed(false),
      isHover(false),
      lightTextColor(0, 0, 0),
      darkTextColor(255, 255, 255),
      iconAccept(new EsCheckBoxIcon(Es::IconCheckBox_Accept)),
      iconPartialAccept(new EsCheckBoxIcon(Es::IconCheckBox_PartialAccept))
{
}

QColor EsCheckBoxPrivate::borderColor()
{
    static const QMap<Es::CheckBoxState, QColor> darkMap{
        {Es::CheckBoxState::NORMAL, QColor(255, 255, 255, 141)},
        {Es::CheckBoxState::HOVER, QColor(255, 255, 255, 141)},
        {Es::CheckBoxState::PRESSED, QColor(255, 255, 255, 40)},
        {Es::CheckBoxState::CHECKED, EsFunc::fallbackThemeColor(darkCheckedColor)},
        {Es::CheckBoxState::CHECKED_HOVER, EsFunc::validColor(darkCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorDark1))},
        {Es::CheckBoxState::CHECKED_PRESSED, EsFunc::validColor(darkCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorDark2))},
        {Es::CheckBoxState::DISABLED, QColor(255, 255, 255, 41)},
        {Es::CheckBoxState::CHECKED_DISABLED, QColor(0, 0, 0, 0)}
    };
    static const QMap<Es::CheckBoxState, QColor> lightMap{
        {Es::CheckBoxState::NORMAL, QColor(0, 0, 0, 122)},
        {Es::CheckBoxState::HOVER, QColor(0, 0, 0, 143)},
        {Es::CheckBoxState::PRESSED, QColor(0, 0, 0, 69)},
        {Es::CheckBoxState::CHECKED, EsFunc::fallbackThemeColor(lightCheckedColor)},
        {Es::CheckBoxState::CHECKED_HOVER, EsFunc::validColor(lightCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorLight1))},
        {Es::CheckBoxState::CHECKED_PRESSED, EsFunc::validColor(lightCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorLight2))},
        {Es::CheckBoxState::DISABLED, QColor(0, 0, 0, 56)},
        {Es::CheckBoxState::CHECKED_DISABLED, QColor(0, 0, 0, 0)}
    };
    const auto& map = EsFunc::isDarkTheme() ? darkMap : lightMap;
    return map.value(state());
}

QColor EsCheckBoxPrivate::backgroundColor()
{
    static const QMap<Es::CheckBoxState, QColor> darkMap = {
        {Es::CheckBoxState::NORMAL, QColor(0, 0, 0, 26)},
        {Es::CheckBoxState::HOVER, QColor(255, 255, 255, 11)},
        {Es::CheckBoxState::PRESSED, QColor(255, 255, 255, 18)},
        {Es::CheckBoxState::CHECKED, EsFunc::fallbackThemeColor(darkCheckedColor)},
        {Es::CheckBoxState::CHECKED_HOVER,EsFunc::validColor(darkCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorDark1))},
        {Es::CheckBoxState::CHECKED_PRESSED,EsFunc::validColor(darkCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorDark2))},
        {Es::CheckBoxState::DISABLED, QColor(0, 0, 0, 0)},
        {Es::CheckBoxState::CHECKED_DISABLED, QColor(255, 255, 255, 41)}
    };

    static const QMap<Es::CheckBoxState, QColor> lightMap = {
        {Es::CheckBoxState::NORMAL, QColor(0, 0, 0, 6)},
        {Es::CheckBoxState::HOVER, QColor(0, 0, 0, 13)},
        {Es::CheckBoxState::PRESSED, QColor(0, 0, 0, 31)},
        {Es::CheckBoxState::CHECKED, EsFunc::fallbackThemeColor(lightCheckedColor)},
        {
            Es::CheckBoxState::CHECKED_HOVER,
            EsFunc::validColor(lightCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorLight1))
        },
        {
            Es::CheckBoxState::CHECKED_PRESSED,
            EsFunc::validColor(lightCheckedColor, EsThemeColor::getColor(Es::ThemeColor::ThemeColorLight2))
        },
        {Es::CheckBoxState::DISABLED, QColor(0, 0, 0, 0)},
        {Es::CheckBoxState::CHECKED_DISABLED, QColor(0, 0, 0, 56)}
    };

    // 根据当前主题选择对应的 map
    const auto& map = EsFunc::isDarkTheme() ? darkMap : lightMap;

    // 返回对应状态的颜色值
    return map.value(state());
}

Es::CheckBoxState EsCheckBoxPrivate::state()
{
    Q_Q(EsCheckBox);
    // 如果 CheckBox 被禁用，返回禁用状态
    if (!q->isEnabled())
    {
        return q->isChecked() ? Es::CheckBoxState::CHECKED_DISABLED : Es::CheckBoxState::DISABLED;
    }

    // 如果 CheckBox 被选中，检查是否按下或悬停
    if (q->isChecked())
    {
        if (isPressed)
        {
            return Es::CheckBoxState::CHECKED_PRESSED; // 被选中且按下
        }
        if (isHover)
        {
            return Es::CheckBoxState::CHECKED_HOVER; // 被选中且悬停
        }

        return Es::CheckBoxState::CHECKED; // 只是被选中
    }
    else
    {
        // 如果 CheckBox 没有被选中，检查是否按下或悬停
        if (isPressed)
        {
            return Es::CheckBoxState::PRESSED; // 没有选中但按下
        }
        if (isHover)
        {
            return Es::CheckBoxState::HOVER; // 没有选中但悬停
        }

        return Es::CheckBoxState::NORMAL; // 默认状态
    }
}

// endregion


// region ================= EsCheckBox  =================

EsCheckBox::EsCheckBox(QWidget* parent)
    : QCheckBox(parent), d_ptr(new EsCheckBoxPrivate())
{
    Q_D(EsCheckBox);
    d->q_ptr = this;

    EsFunc::setFont(this);
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_CHECK_BOX);
    styleSheet->apply(this);
}

EsCheckBox::EsCheckBox(const QString& text, QWidget* parent)
    : EsCheckBox(parent)
{
    setText(text);
}

EsCheckBox::~EsCheckBox()
{
    Q_D(EsCheckBox);
    delete d->iconAccept;
    delete d->iconPartialAccept;
}

void EsCheckBox::setCheckedColor(const QColor& light, const QColor& dark)
{
    Q_D(EsCheckBox);
    d->lightCheckedColor = light;
    d->darkCheckedColor = dark;
    update();
}

void EsCheckBox::setTextColor(const QColor& light, const QColor& dark)
{
    Q_D(EsCheckBox);
    d->lightTextColor = light;
    d->darkTextColor = dark;

    QString lightColorHex = d->lightTextColor.name(QColor::HexArgb); // 获取浅色文本颜色的 HEX 值
    QString darkColorHex = d->darkTextColor.name(QColor::HexArgb); // 获取深色文本颜色的 HEX 值

    // 设置自定义样式表
    EsFunc::setCustomStyleSheet(
        this,
        QString("EsCheckBox { color: %1; }").arg(lightColorHex),
        QString("EsCheckBox { color: %1; }").arg(darkColorHex)
    );
}

void EsCheckBox::mousePressEvent(QMouseEvent* e)
{
    Q_D(EsCheckBox);
    d->isPressed = true;
    QCheckBox::mousePressEvent(e);
}

void EsCheckBox::mouseReleaseEvent(QMouseEvent* e)
{
    Q_D(EsCheckBox);
    d->isPressed = false;
    QCheckBox::mouseReleaseEvent(e);
}

void EsCheckBox::enterEvent(QEvent* event)
{
    Q_D(EsCheckBox);
    d->isHover = true;
    update();
}

void EsCheckBox::leaveEvent(QEvent* event)
{
    Q_D(EsCheckBox);
    d->isHover = false;
    update();
}

void EsCheckBox::paintEvent(QPaintEvent* paint_event)
{
    Q_D(EsCheckBox);
    QCheckBox::paintEvent(paint_event);


    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing); // 启用反锯齿

    // 获取指示器的矩形区域
    QStyleOptionButton opt;
    opt.initFrom(this);
    QRect rect = this->style()->subElementRect(QStyle::SE_CheckBoxIndicator, &opt, this);

    // 绘制边框和背景
    painter.setPen(d->borderColor()); // 设置边框颜色
    painter.setBrush(d->backgroundColor()); // 设置背景颜色
    painter.drawRoundedRect(rect, 4.5, 4.5); // 绘制圆角矩形

    // 如果 CheckBox 被禁用，设置透明度
    if (!this->isEnabled())
    {
        painter.setOpacity(0.8);
    }

    // 绘制图标，根据状态选择不同的图标
    if (this->checkState() == Qt::Checked)
    {
        d->iconAccept->render(&painter, rect, {}); // 绘制选中图标
    }
    else if (this->checkState() == Qt::PartiallyChecked)
    {
        d->iconPartialAccept->render(&painter, rect, {}); // 绘制部分选中图标
    }
}


// endregion
