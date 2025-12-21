/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esSwitchButton.cpp
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esSwitchButton.h"

#include <QEvent>
#include <QHBoxLayout>

#include "esStyleSheet.h"
#include "subButton/esIndicator.h"

#include "private/esSwitchButton_p.h"
// region ================= EsSwitchButtonPrivate =================

EsSwitchButtonPrivate::EsSwitchButtonPrivate(QObject* parent)
    : QObject(parent),
      text("关"),
      offText("关"),
      onText("开"),
      spacing(12),
      lightTextColor(0, 0, 0),
      darkTextColor(255, 255, 255)
{
}

void EsSwitchButtonPrivate::initWidget()
{
    Q_Q(EsSwitchButton);

    q->setAttribute(Qt::WA_StyledBackground);
    q->installEventFilter(q);
    q->setFixedHeight(22);

    // 设置布局
    hBox->setSpacing(spacing);
    hBox->setContentsMargins(2, 0, 0, 0);

    // 根据指示器的位置决定控件的布局
    if (indicatorPos == Es::IndicatorPosition::LEFT)
    {
        hBox->addWidget(indicator);
        hBox->addWidget(label);
        hBox->setAlignment(Qt::AlignLeft);
    }
    else
    {
        hBox->addWidget(label, 0, Qt::AlignRight);
        hBox->addWidget(indicator, 0, Qt::AlignRight);
        hBox->setAlignment(Qt::AlignRight);
    }

    // 应用默认的样式表
    auto styleSheet1 = new EsStyleSheet(Es::StyleSheet::Style_SWITCH_BUTTON);
    auto styleSheet2 = new EsStyleSheet(Es::StyleSheet::Style_SWITCH_BUTTON);
    styleSheet1->apply(q);
    styleSheet2->apply(label);
    // 连接信号与槽
    connect(indicator, &EsIndicator::toggled, this, &EsSwitchButtonPrivate::updateText);
    connect(indicator, &EsIndicator::toggled, q, &EsSwitchButton::checkedChanged);
}

void EsSwitchButtonPrivate::updateText()
{
    Q_Q(EsSwitchButton);
    q->setText(q->isChecked() ? onText : offText);
    q->adjustSize();
}

// endregion


// region ================= EsSwitchButton =================


EsSwitchButton::EsSwitchButton(QWidget* parent, Es::IndicatorPosition indicatorPos)
    : QWidget(parent), d_ptr(new EsSwitchButtonPrivate())

{
    Q_D(EsSwitchButton);
    d->q_ptr = this;
    d->indicatorPos = indicatorPos;
    d->hBox = new QHBoxLayout(this);
    d->indicator = new EsIndicator(this);
    d->label = new QLabel(this);
    d->initWidget();
}


EsSwitchButton::EsSwitchButton(const QString& text, QWidget* parent, Es::IndicatorPosition indicatorPos)
    : EsSwitchButton(parent, indicatorPos)
{
    Q_D(EsSwitchButton);
    d->offText = text;
    setText(text);
}

EsSwitchButton::~EsSwitchButton()
{
}

bool EsSwitchButton::eventFilter(QObject* obj, QEvent* e)
{
    Q_D(EsSwitchButton);
    if (obj == this && this->isEnabled())
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            d->indicator->setDown(true);
        }
        else if (e->type() == QEvent::MouseButtonRelease)
        {
            d->indicator->setDown(false);
            d->indicator->toggle();
        }
        else if (e->type() == QEvent::Enter)
        {
            d->indicator->setHover(true);
        }
        else if (e->type() == QEvent::Leave)
        {
            d->indicator->setHover(false);
        }
    }
    return QWidget::eventFilter(obj, e);
}

bool EsSwitchButton::isChecked() const
{
    Q_D(const EsSwitchButton);
    return d->indicator->isChecked();
}

void EsSwitchButton::setChecked(bool isChecked)
{
    Q_D(EsSwitchButton);
    d->updateText();
    d->indicator->setChecked(isChecked);
}

void EsSwitchButton::setTextColor(const QColor& light, const QColor& dark)
{
    Q_D(EsSwitchButton);
    // 设置颜色
    d->lightTextColor = light;
    d->darkTextColor = dark;

    // 设置自定义样式表
    QString lightColor = d->lightTextColor.name(QColor::HexArgb);
    QString darkColor = d->darkTextColor.name(QColor::HexArgb);

    QString styleSheet = QString("EsSwitchButton>QLabel{color:%1}").arg(lightColor);
    QString darkStyleSheet = QString("EsSwitchButton>QLabel{color:%1}").arg(darkColor);

    EsFunc::setCustomStyleSheet(d->label, styleSheet, darkStyleSheet);
}

void EsSwitchButton::setCheckedIndicatorColor(const QColor& light, const QColor& dark)
{
    Q_D(EsSwitchButton);
    d->indicator->setCheckedColor(light, dark);
}

void EsSwitchButton::toggleChecked()
{
    Q_D(EsSwitchButton);
    d->indicator->setChecked(!d->indicator->isChecked());
}

QString EsSwitchButton::getText() const
{
    Q_D(const EsSwitchButton);
    return d->text;
}

void EsSwitchButton::setText(const QString& text)
{
    Q_D(EsSwitchButton);
    d->text = text;
    d->label->setText(text);
    adjustSize();
}

int EsSwitchButton::getSpacing() const
{
    Q_D(const EsSwitchButton);
    return d->spacing;
}

void EsSwitchButton::setSpacing(int spacing)
{
    Q_D(EsSwitchButton);
    d->spacing = spacing;
    d->hBox->setSpacing(spacing);
    update();
}

QString EsSwitchButton::getOnText() const
{
    Q_D(const EsSwitchButton);
    return d->onText;
}

void EsSwitchButton::setOnText(const QString& text)
{
    Q_D(EsSwitchButton);
    d->onText = text;
    d->updateText();
}

QString EsSwitchButton::getOffText() const
{
    Q_D(const EsSwitchButton);
    return d->offText;
}

void EsSwitchButton::setOffText(const QString& text)
{
    Q_D(EsSwitchButton);
    d->offText = text;
    d->updateText();
}


// endregion
