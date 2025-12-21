/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWidget.cpp
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esWidget.h"

#include <QApplication>

#include "esTitleBarButtons.h"
#include <QCursor>
#include <QMouseEvent>

#include "framelessWindow/esFramelessWindow_p.h"
#include "private/esWidget_p.h"

// region ================= EsWidgetPrivate  =================

EsWidgetPrivate::EsWidgetPrivate(QObject* parent)
    : QObject(parent)
{
}

bool EsWidgetPrivate::isHoverMaxBtn()
{
    Q_Q(EsWidget);
    auto pos = QCursor::pos() - q->geometry().topLeft() - q->titleBar->pos();
    return qobject_cast<EsMaximizeButton*>(q->titleBar->childAt(pos)) == q->titleBar->maxBtn;
}
// endregion

// region ================= EsWidget  =================

EsWidget::EsWidget(QWidget* parent)
    : EsAcrylicWindow(parent), d_ptr(new EsWidgetPrivate())
{
    Q_D(EsWidget);
    d->q_ptr = this;
    windowEffect->setMicaEffect((HWND)winId());
}

EsWidget::~EsWidget()
{
}


// windows11 鼠标放到最大化窗口时, 显示选择分屏布局的弹窗
bool EsWidget::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_D(EsWidget);
    MSG* msg = reinterpret_cast<MSG*>(message); // 转换消息指针

    if (!msg->hwnd)
    {
        return EsAcrylicWindow::nativeEvent(eventType, message, result);
    }

    // 处理 WM_NCHITTEST 消息，判断是否悬停在最大化按钮上
    if (msg->message == WM_NCHITTEST && EsFramelessWindow::d_ptr->isResizeEnabled)
    {
        if (d->isHoverMaxBtn())
        {
            titleBar->maxBtn->setState(Es::TitleBarButtonState::HOVER);
            *result = HTMAXBUTTON;
            return true;
        }
    }
    // 处理鼠标离开消息，恢复最大化按钮状态
    else if (msg->message == WM_MOUSELEAVE || msg->message == 0x2A2)
    {
        titleBar->maxBtn->setState(Es::TitleBarButtonState::NORMAL);
    }
    // 处理鼠标按下（左键或右键）消息
    else if ((msg->message == WM_NCLBUTTONDOWN || msg->message == WM_NCLBUTTONDBLCLK) && d->isHoverMaxBtn())
    {
        auto e = new QMouseEvent(QEvent::MouseButtonPress, QPoint(), Qt::LeftButton, Qt::LeftButton,
                                  Qt::NoModifier);
        QApplication::sendEvent(titleBar->maxBtn, e);
        *result = 0;
        return true;
    }
    // 处理鼠标松开（左键或右键）消息
    else if ((msg->message == WM_NCLBUTTONUP || msg->message == WM_NCRBUTTONUP) && d->isHoverMaxBtn())
    {
        auto e = new QMouseEvent(QEvent::MouseButtonRelease, QPoint(), Qt::LeftButton, Qt::LeftButton,
                                  Qt::NoModifier);
        QApplication::sendEvent(titleBar->maxBtn, e);
        *result = 0;
        return true;
    }

    return EsAcrylicWindow::nativeEvent(eventType, message, result);
}
// endregion
