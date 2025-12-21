/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esFramelessWindow.cpp
** @date 2025/10/3
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esFramelessWindow.h"

#include <QWindow>
#include <QCloseEvent>
#include <QApplication>
#include <QScreen>
#include "esWin32Utils.h"
#include "esFramelessWindow_p.h"

// region ================= EsFramelessWindowPrivate  =================

EsFramelessWindowPrivate::EsFramelessWindowPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsFramelessWindowPrivate::onScreenChanged()
{
    Q_Q(EsFramelessWindow);
    auto hWnd = (HWND)q->windowHandle()->winId();
    ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

// endregion

// region ================= EsAcrylicWindowPrivate =================

EsAcrylicWindowPrivate::EsAcrylicWindowPrivate(QObject* parent)
{
}

// endregion


// region ================= FramelessWindow 无边框窗口 =================

EsFramelessWindow::EsFramelessWindow(QWidget* parent)
    : QWidget(parent), d_ptr(new EsFramelessWindowPrivate())
{
    Q_D(EsFramelessWindow);
    d->q_ptr = this;

    windowEffect = EsWindowsWindowEffect::getInstance();
    titleBar = new EsTitleBar(this);

    d->isSystemButtonVisible = false;
    d->isResizeEnabled = true;
    d->borderWidth = 5;
    EsFramelessWindow::updateFrameless();
    connect(windowHandle(), &QWindow::screenChanged, d, &EsFramelessWindowPrivate::onScreenChanged);
    resize(500, 500);
    titleBar->raise();
}

EsFramelessWindow::~EsFramelessWindow()
{
}

// 更新无框窗口
void EsFramelessWindow::updateFrameless()
{
    Q_D(EsFramelessWindow);


    // 更新无框窗口
    Qt::WindowFlags flags = this->windowFlags(); // 获取当前窗口的标志
    Qt::WindowFlags stayOnTop = (flags & Qt::WindowStaysOnTopHint) ? Qt::WindowStaysOnTopHint : Qt::WindowFlags(0);
    // 检查并设置窗口是否始终在最上层

    QWidget* parentWidget = qobject_cast<QWidget*>(this->parent());

    if (!isWin7())
    {
        // 如果不是 Windows 7，则设置无框窗口
        this->setWindowFlags(flags | Qt::FramelessWindowHint);
    }
    else if (parentWidget)
    {
        // 如果有父窗口，则设置无框窗口，并添加最小化、最大化按钮和保持最上层属性
        this->setWindowFlags(
            parentWidget->windowFlags() | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | stayOnTop);
    }
    else
    {
        // 如果没有父窗口，则仅设置无框窗口，并添加最小化、最大化按钮和保持最上层属性
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | stayOnTop);
    }

    // 添加 DWM（桌面窗口管理器）阴影和窗口动画效果
    EsWindowsWindowEffect::addWindowAnimation((HWND)winId());

    // 如果不是 Acrylic 窗口，添加阴影效果
    if (!dynamic_cast<EsAcrylicWindow*>(this))
    {
        windowEffect->addShadowEffect((HWND)winId());
    }
}

void EsFramelessWindow::setTitleBar(EsTitleBar* titleBar)
{
    // 删除旧的标题栏
    if (this->titleBar)
    {
        this->titleBar->deleteLater(); // 删除旧的标题栏对象
        this->titleBar->hide(); // 隐藏旧的标题栏
    }

    // 更新为新的标题栏
    this->titleBar = titleBar;
    this->titleBar->setParent(this); // 将新的标题栏设置为当前窗口的父控件
    this->titleBar->raise(); // 将新的标题栏提升到最上层
}

void EsFramelessWindow::setResizeEnabled(bool isEnabled)
{
    Q_D(EsFramelessWindow);
    d->isResizeEnabled = isEnabled;
}

void EsFramelessWindow::setStayOnTop(bool isTop)
{
    Qt::WindowFlags flags = windowFlags(); // 获取当前窗口的标志

    if (isTop)
    {
        // 设置窗口为始终在最上层
        setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    }
    else
    {
        // 取消窗口的始终在最上层
        setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    }

    // 更新无框窗口设置
    updateFrameless();

    // 显示窗口
    show();
}

void EsFramelessWindow::toggleStayOnTop()
{
    if (windowFlags() & Qt::WindowStaysOnTopHint)
    {
        setStayOnTop(false);
    }
    else
    {
        setStayOnTop(true);
    }
}

bool EsFramelessWindow::isSystemButtonVisible() const
{
    Q_D(const EsFramelessWindow);
    return d->isSystemButtonVisible;
}

void EsFramelessWindow::setSystemTitleBarButtonVisible(bool isVisible)
{
}

QRect EsFramelessWindow::systemTitleBarRect(QSize size)
{
    return {0, 0, size.width(), size.height()};
}

void EsFramelessWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    titleBar->resize(width(), titleBar->height());
}

bool EsFramelessWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_D(EsFramelessWindow);
    MSG* msg = reinterpret_cast<MSG*>(message);
    if (!msg->hwnd)
    {
        return QWidget::nativeEvent(eventType, message, result);
    }

    // 处理 WM_NCHITTEST 消息
    if (msg->message == WM_NCHITTEST && d->isResizeEnabled)
    {
        // 获取鼠标位置并转换为客户区域坐标
        POINT pt;
        pt.x = LOWORD(msg->lParam);
        pt.y = HIWORD(msg->lParam);
        ScreenToClient(msg->hwnd, &pt);

        RECT clientRect;
        GetClientRect(msg->hwnd, &clientRect);

        int w = clientRect.right - clientRect.left;
        int h = clientRect.bottom - clientRect.top;

        // 判断是否为最大化或全屏模式
        int bw = (::isMaximized(msg->hwnd) || ::isFullScreen(msg->hwnd)) ? 0 : d->borderWidth;

        // 判断鼠标位置是否处于可调整大小的区域
        bool lx = pt.x < bw; // 左边
        bool rx = pt.x > w - bw; // 右边
        bool ty = pt.y < bw; // 上边
        bool by = pt.y > h - bw; // 下边

        if (lx && ty)
        {
            *result = HTTOPLEFT;
            return true;
        }
        else if (rx && by)
        {
            *result = HTBOTTOMRIGHT;
            return true;
        }
        else if (rx && ty)
        {
            *result = HTTOPRIGHT;
            return true;
        }
        else if (lx && by)
        {
            *result = HTBOTTOMLEFT;
            return true;
        }
        else if (ty)
        {
            *result = HTTOP;
            return true;
        }
        else if (by)
        {
            *result = HTBOTTOM;
            return true;
        }
        else if (lx)
        {
            *result = HTLEFT;
            return true;
        }
        else if (rx)
        {
            *result = HTRIGHT;
            return true;
        }
    }
    // 处理 WM_NCCALCSIZE 消息
    else if (msg->message == WM_NCCALCSIZE)
    {
        NCCALCSIZE_PARAMS* nccsParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        RECT& rect = nccsParams->rgrc[0];

        bool isMax = ::isMaximized(msg->hwnd);
        bool isFull = ::isFullScreen(msg->hwnd);

        // 如果是最大化状态，调整客户区的大小
        if (isMax && !isFull)
        {
            int ty = getResizeBorderThickness(msg->hwnd, false);
            rect.top += ty;
            rect.bottom -= ty;

            int tx = getResizeBorderThickness(msg->hwnd, true);
            rect.left += tx;
            rect.right -= tx;
        }

        // todo 处理自动隐藏任务栏的情况
        // if ((isMax || isFull) && isTaskbarAutoHide()) {
        //     int position = getTaskbarPosition(msg->hwnd);
        //     if (position == LEFT) {
        //         rect.top += AUTO_HIDE_THICKNESS;
        //     } else if (position == BOTTOM) {
        //         rect.bottom -= AUTO_HIDE_THICKNESS;
        //     } else if (position == RIGHT) {
        //         rect.right -= AUTO_HIDE_THICKNESS;
        //     }
        // }

        *result = msg->wParam ? WVR_REDRAW : 0;
        return true;
    }
    // 处理 WM_SETFOCUS 消息
    else if (msg->message == WM_SETFOCUS && isSystemBorderAccentEnabled())
    {
        windowEffect->setBorderAccentColor(msg->hwnd, ::getSystemAccentColor());
        *result = 0;
        return true;
    }
    // 处理 WM_KILLFOCUS 消息
    else if (msg->message == WM_KILLFOCUS)
    {
        windowEffect->removeBorderAccentColor(msg->hwnd);
        *result = 0;
        return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}

// endregion


// region ================= EsAcrylicWindow  =================

EsAcrylicWindow::EsAcrylicWindow(QWidget* parent)
    : EsFramelessWindow(parent), d_ptr(new EsAcrylicWindowPrivate())
{
    Q_D(EsAcrylicWindow);
    d->q_ptr = this;
    d->closedByKey = false;
    setStyleSheet("EsAcrylicWindow{background:transparent}");
}

EsAcrylicWindow::~EsAcrylicWindow()
{
}

void EsAcrylicWindow::updateFrameless()
{
    EsFramelessWindow::updateFrameless();

    windowEffect->enableBlurBehindWindow((HWND)winId());

    // 获取窗口的 "始终在最上层" 标志
    Qt::WindowFlags stayOnTop = (this->windowFlags() & Qt::WindowStaysOnTopHint)
                                    ? Qt::WindowStaysOnTopHint
                                    : Qt::WindowFlags(0);
    // 判断是否为 Windows 7 且有父窗口
    if (::isWin7() && this->parent())
    {
        // 设置无框窗口和最小化、最大化按钮，并保留 "始终在最上层" 标志
        this->setWindowFlags(
            qobject_cast<QWidget*>(parent())->windowFlags() | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint |
            stayOnTop);
    }
    else
    {
        // 设置无框窗口和最小化、最大化按钮，并保留 "始终在最上层" 标志
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | stayOnTop);
    }

    // 添加窗口动画
    EsWindowsWindowEffect::addWindowAnimation((HWND)winId());

    // Windows 7 相关设置
    if (isWin7())
    {
        windowEffect->addShadowEffect((HWND)winId()); // 添加阴影效果
        windowEffect->setAeroEffect((HWND)winId()); // 设置 Aero 效果
    }
    else
    {
        // 设置 Acrylic 效果
        windowEffect->setAcrylicEffect((HWND)winId());

        // 如果是 Windows 11 或更高版本，添加阴影效果
        if (isGreaterEqualWin11())
        {
            windowEffect->addShadowEffect((HWND)winId());
        }
    }
}

// 处理Windows消息
bool EsAcrylicWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_D(EsAcrylicWindow);
    MSG* msg = reinterpret_cast<MSG*>(message);

    // 处理 Alt+F4
    if (msg->message == WM_SYSKEYDOWN)
    {
        if (msg->wParam == VK_F4)
        {
            d->closedByKey = true;
            QApplication::sendEvent(this, new QCloseEvent());
            *result = 0;
            return false;
        }
    }


    return EsFramelessWindow::nativeEvent(eventType, message, result);
}

void EsAcrylicWindow::closeEvent(QCloseEvent* event)
{
    Q_D(EsAcrylicWindow);
    if (!d->closedByKey || QApplication::quitOnLastWindowClosed())
    {
        d->closedByKey = false;
        return EsFramelessWindow::closeEvent(event);
    }
    // system tray icon
    d->closedByKey = false;
    hide();
}

// endregion
