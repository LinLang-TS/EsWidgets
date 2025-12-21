/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleBar.cpp
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/
#include "esTitleBar.h"
#include "esTitleBarButtons.h"

#include <QMouseEvent>
#include <QSysInfo>
#include <QLabel>
#include <QHBoxLayout>

#include "esTitleBar_p.h"


// region ================= 标题栏Base Private类 =================

void EsTitleBarBasePrivate::toggleMaxState()
{
    Q_Q(EsTitleBarBase);
    if (q->window()->isMaximized())
    {
        q->window()->showNormal();
    }
    else
    {
        q->window()->showMaximized();
    }
#ifdef Q_OS_WIN
    releaseMouseLeftButton((HWND)q->window()->winId());
#endif
}

bool EsTitleBarBasePrivate::isDragRegion(QPoint pos)
{
    Q_Q(EsTitleBarBase);
    int width = 0;

    // 查找所有 TitleBarButton 类型的子控件
    const auto buttons = q->findChildren<EsTitleBarButtonBase*>();

    for (auto button : buttons)
    {
        if (button->isVisible())
        {
            width += button->width(); // 累加可见按钮的宽度
        }
    }

    // 判断鼠标位置是否在可拖动的区域内
    return (pos.x() > 0 && pos.x() < q->width() - width);
}

bool EsTitleBarBasePrivate::hasButtonPressed()
{
    Q_Q(EsTitleBarBase);
    // 查找所有 EsTitleBarButtonBase 类型的子控件
    const auto buttons = q->findChildren<EsTitleBarButtonBase*>();

    // 遍历按钮列表，检查是否有按钮被按下
    for (auto button : buttons)
    {
        if (button->isPressed())
        {
            return true;
        }
    }
    return false;
}

// endregion


// region ================= 标题栏基类 =================

EsTitleBarBase::EsTitleBarBase(QWidget* parent)
    : QWidget(parent), d_ptr(new EsTitleBarBasePrivate())
{
    Q_D(EsTitleBarBase);
    d->q_ptr = this;
    closeBtn = new EsCloseButton(this);
    minBtn = new EsMinimizeButton(this);
    maxBtn = new EsMaximizeButton(this);
    resize(200, 32);
    setFixedHeight(32);

    connect(minBtn, &QAbstractButton::clicked, window(), &QWidget::showMinimized);
    connect(maxBtn, &QAbstractButton::clicked, d, &EsTitleBarBasePrivate::toggleMaxState);
    connect(closeBtn, &QAbstractButton::clicked, window(), &QWidget::close);
    window()->installEventFilter(this);
}


bool EsTitleBarBase::eventFilter(QObject* obj, QEvent* e)
{
    if (obj == window())
    {
        if (e->type() == QEvent::WindowStateChange)
        {
            maxBtn->setMaxState(window()->isMaximized());
            return false;
        }
    }


    return QWidget::eventFilter(obj, e);
}

bool EsTitleBarBase::canDrag(QPoint pos)
{
    Q_D(EsTitleBarBase);
    return d->isDragRegion(pos) && !d->hasButtonPressed();
}

void EsTitleBarBase::setDoubleClickEnabled(bool isEnabled)
{
    Q_D(EsTitleBarBase);
    d->isDoubleClickEnabled = isEnabled;
}

// 切换窗口最大化的状态
void EsTitleBarBase::mouseDoubleClickEvent(QMouseEvent* e)
{
    Q_D(EsTitleBarBase);
    if (e->button() != Qt::LeftButton || !d->isDoubleClickEnabled) return;
    d->toggleMaxState();
    // QWidget::mouseDoubleClickEvent(e);
}

// Windows移动窗口事件
void EsTitleBarBase::mouseMoveEvent(QMouseEvent* e)
{
    if (QSysInfo::productType() != "windows" || !canDrag(e->pos())) return;
    ReleaseCapture();
    SendMessage(
        (HWND)window()->winId(),
        WM_SYSCOMMAND,
        SC_MOVE | HTCAPTION,
        0
    );


    // 移动窗口
   // move(window()->pos() + QPoint(1,1)); // 通过 move 来实现拖动
   //  QWidget::mouseMoveEvent(e);
}

// mac移动窗口事件
void EsTitleBarBase::mousePressEvent(QMouseEvent* e)
{
    if (QSysInfo::productType() == "windows" || !canDrag(e->pos())) return;
    // todo startSystemMove() mac,linux兼容没写

    //  QWidget::mousePressEvent(e);
}

// endregion


// region ================= 标题栏 =================
EsTitleBar::EsTitleBar(QWidget* parent)
    : EsTitleBarBase(parent)
{
    Q_D(EsTitleBarBase);

    d->hBoxLayout = new QHBoxLayout(this);

    // add buttons to layout
    d->hBoxLayout->setSpacing(0);
    d->hBoxLayout->setContentsMargins(0, 0, 0, 0);
    d->hBoxLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    d->hBoxLayout->addStretch(1);
    d->hBoxLayout->addWidget(minBtn, 0, Qt::AlignRight);
    d->hBoxLayout->addWidget(maxBtn, 0, Qt::AlignRight);
    d->hBoxLayout->addWidget(closeBtn, 0, Qt::AlignRight);
}

// endregion


// region ================= 标题栏 =================
EsTitleBarStandard::EsTitleBarStandard(QWidget* parent)
    : EsTitleBar(parent)
{
    Q_D(EsTitleBarBase);
    // add window icon
    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(20, 20);
    d->hBoxLayout->insertSpacing(0, 10);
    d->hBoxLayout->insertWidget(1, iconLabel, 0, Qt::AlignLeft);
    connect(window(), &QWidget::windowIconChanged, this, &EsTitleBarStandard::setIcon);

    // # add title label
     titleLabel = new QLabel(this);
    d->hBoxLayout->insertWidget(2, titleLabel, 0, Qt::AlignLeft);
    titleLabel->setStyleSheet(R"EOF(
        QLabel{
            background: transparent;
            font: 13px 'Segoe UI', 'Microsoft YaHei', 'PingFang SC';
            padding: 0 4px
        }
    )EOF");
    connect(window(), &QWidget::windowTitleChanged, this, &EsTitleBarStandard::setTitle);
}

void EsTitleBarStandard::setIcon(const QIcon &icon)
{
    iconLabel->setPixmap(QIcon(icon).pixmap(20, 20));
}

void EsTitleBarStandard::setTitle(const QString& title)
{
    titleLabel->setText(title);
    titleLabel->adjustSize();
}

// endregion
