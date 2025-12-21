/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWindowBase.cpp
** @date 2025/11/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esWindowBase.h"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPropertyAnimation>

#include "esAnimation.h"
#include "esRouter.h"
#include "esStackedWidget.h"
#include "esStyleSheet.h"
#include "esWin32Utils.h"

#include "private/esWindowBase_p.h"

// region ================= EsWindowBasePrivate  =================

EsWindowBasePrivate::EsWindowBasePrivate(QObject* parent)
    : QObject(parent),
      isMicaEnabled(false),
      lightBackgroundColor(240, 244, 249),
      darkBackgroundColor(32, 32, 32)
{
}

// endregion

// region ================= EsWindowBase  =================

EsWindowBase::EsWindowBase(QWidget* parent)
    : EsBackgroundAnimationWidget(parent), d_ptr(new EsWindowBasePrivate())
{
    Q_D(EsWindowBase);
    d->q_ptr = this;


    d->hBoxLayout = new QHBoxLayout(this);
    stackedWidget = new EsStackedWidget(this);
    navigationInterface = nullptr;

    // 初始化布局设置
    d->hBoxLayout->setSpacing(0);
    d->hBoxLayout->setContentsMargins(0, 0, 0, 0);

    // 应用样式
    auto styleSheet = new EsStyleSheet(Es::StyleSheet::Style_FLUENT_WINDOW);
    styleSheet->apply(stackedWidget);
    // 启用 Windows 11 上的 Mica 效果
    setMicaEffectEnabled(true);

    // 在 macOS 上显示系统标题栏按钮
    if (QSysInfo::productType() == "osx") // if sys.platform == "darwin":
    {
        setSystemTitleBarButtonVisible(true);
    }

    // 连接主题变化信号
    connect(&esConfig, &EsConfig::themeChangedFinished, this, &EsWindowBase::onThemeChangedFinished);
}

EsWindowBase::~EsWindowBase()
{
}

void EsWindowBase::addSubInterface(QWidget* iface)
{
    stackedWidget->addWidget(iface);
}

void EsWindowBase::switchTo(QWidget* iface)
{
    stackedWidget->setCurrentWidget(iface, false);
}

void EsWindowBase::setCustomBackgroundColor(const QColor& light, const QColor& dark)
{
    Q_D(EsWindowBase);
    // 设置浅色主题下的背景颜色
    d->lightBackgroundColor = light;

    // 设置深色主题下的背景颜色
    d->darkBackgroundColor = dark;

    // 更新背景颜色
    updateBackgroundColor();
}


void EsWindowBase::setMicaEffectEnabled(bool isEnabled)
{
    Q_D(EsWindowBase);

    // 检查是否是 Windows 11 系统，并且 Windows 版本大于或等于 22000
    if (!isGreaterEqualWin11()) return;

    d->isMicaEnabled = isEnabled;

    // 启用或禁用 Mica 特效
    if (isEnabled)
    {
        windowEffect->setMicaEffect((HWND)winId(), EsFunc::isDarkTheme());
    }
    else
    {
        windowEffect->removeBackgroundEffect((HWND)winId());
    }

    // 设置背景颜色
    setBackgroundColor(normalBackgroundColor());
}

bool EsWindowBase::isMicaEffectEnabled() const
{
    Q_D(const EsWindowBase);
    return d->isMicaEnabled;
}

QRect EsWindowBase::systemTitleBarRect(const QSize& size) const
{
    // 如果是全屏，则标题栏在窗口顶部；否则标题栏稍微向下偏移 9 像素
    return {size.width() - 75, isFullScreen() ? 0 : 9, 75, size.height()};
}

void EsWindowBase::setTitleBar(QWidget* titleBar)
{
    // todo mac标题栏
    // EsBackgroundAnimationWidget::setTitleBar(titleBar);  // 调用基类的 setTitleBar
    //
    // // 在 macOS 上隐藏系统标题栏按钮
    // if (QSysInfo::productType() == "osx" && isSystemButtonVisible() && qobject_cast<TitleBarBase*>(titleBar)) {
    //     TitleBarBase* baseTitleBar = qobject_cast<TitleBarBase*>(titleBar);
    //     baseTitleBar->minBtn->hide();
    //     baseTitleBar->maxBtn->hide();
    //     baseTitleBar->closeBtn->hide();
    // }
}


void EsWindowBase::onCurrentInterfaceChanged(int index)
{
    // 获取当前界面
    QWidget* widget = stackedWidget->widget(index);

    // todo 更新导航栏当前项目
    // navigationInterface->setCurrentItem(widget->objectName());

    // 使用 qrouter 跳转到当前界面
    esRouter.push(stackedWidget, widget->objectName());

    // 更新堆叠背景
    updateStackedBackground();
}

void EsWindowBase::updateStackedBackground()
{
    // 获取当前堆叠窗口的透明属性
    bool isTransparent = stackedWidget->currentWidget()->property("isStackedTransparent").toBool();

    // 如果当前堆叠窗口的透明属性与目标透明度一致，则直接返回
    if (stackedWidget->property("isTransparent").toBool() == isTransparent)
    {
        return;
    }

    // 设置堆叠窗口的透明属性
    stackedWidget->setProperty("isTransparent", isTransparent);

    // 刷新堆叠窗口样式
    stackedWidget->setStyle(QApplication::style());
}


QColor EsWindowBase::normalBackgroundColor()
{
    Q_D(EsWindowBase);
    // 如果没有启用 Mica 特效，则根据当前主题返回相应的背景颜色
    if (!isMicaEffectEnabled())
    {
        return EsFunc::isDarkTheme() ? d->darkBackgroundColor : d->lightBackgroundColor;
    }

    // 启用 Mica 特效时，返回透明背景
    return {0, 0, 0, 0};
}

void EsWindowBase::onThemeChangedFinished()
{
    // 如果启用了 Mica 特效，更新窗口的 Mica 特效效果
    if (isMicaEffectEnabled())
    {
        windowEffect->setMicaEffect((HWND)winId(), EsFunc::isDarkTheme());
    }
}


void EsWindowBase::paintEvent(QPaintEvent* event)
{
    Q_D(EsWindowBase);
    EsWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(getBackgroundColor());
    painter.drawRect(rect());
}

void EsWindowBase::showEvent(QShowEvent* event)
{
    EsBackgroundAnimationWidget::showEvent(event);
    // 窗口完全初始化后重新应用云母效果
    if (isMicaEffectEnabled())
        windowEffect->setMicaEffect((HWND)winId(), EsFunc::isDarkTheme());
}


// endregion
