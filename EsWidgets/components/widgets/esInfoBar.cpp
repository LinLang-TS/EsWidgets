/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esInfoBar.cpp
** @date 2025/11/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esInfoBar.h"

#include <QEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

#include "esIcon.h"
#include "button/esTransparentToolButton.h"
#include "esStyleSheet.h"
#include "esTextWrap.h"
#include "esUtils.h"
#include "private/esInfoBar_p.h"

// region ================= EsInfoBarPrivate =================

EsInfoBarPrivate::EsInfoBarPrivate(QObject* parent)
    : QObject(parent)
{
}

void EsInfoBarPrivate::initWidget()
{
    Q_Q(EsInfoBar);
    opacityEffect->setOpacity(1);
    q->setGraphicsEffect(opacityEffect);

    closeButton->setFixedSize(36, 36);
    closeButton->setIconSize(QSize(12, 12));
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setVisible(isClosable);

    setQss();
    initLayout();
    connect(closeButton, &EsTransparentToolButton::clicked, q, &EsTransparentToolButton::close);
}

void EsInfoBarPrivate::initLayout()
{
    Q_Q(EsInfoBar);

    // 设置水平布局的边距
    hBoxLayout->setContentsMargins(6, 6, 6, 6);
    hBoxLayout->setSizeConstraint(QLayout::SetMinimumSize); // 设置布局的最小尺寸约束

    // 设置文本布局的最小尺寸约束
    textLayout->setSizeConstraint(QLayout::SetMinimumSize);
    textLayout->setAlignment(Qt::AlignTop); // 设置对齐方式为顶部对齐
    textLayout->setContentsMargins(1, 8, 0, 8); // 设置文本布局的内边距

    hBoxLayout->setSpacing(0); // 设置水平布局之间的间距
    textLayout->setSpacing(5); // 设置文本布局的子元素之间的间距

    // 向布局中添加图标
    hBoxLayout->addWidget(iconWidget, 0, Qt::AlignTop | Qt::AlignLeft); // 添加图标到布局，左上对齐

    // 向文本布局中添加标题
    textLayout->addWidget(titleLabel, 1, Qt::AlignTop); // 添加标题标签，顶部对齐
    titleLabel->setVisible(!title.isEmpty()); // 如果标题不为空，则显示标题标签

    // 向文本布局中添加内容标签
    if (orient == Qt::Horizontal)
    {
        textLayout->addSpacing(7); // 如果是水平布局，添加间隔
    }


    textLayout->addWidget(contentLabel, 1, Qt::AlignTop); // 添加内容标签，顶部对齐
    contentLabel->setVisible(!content.isEmpty()); // 如果内容不为空，则显示内容标签
    hBoxLayout->addLayout(textLayout); // 将文本布局添加到水平布局中

    // 根据方向，决定是否需要添加小部件布局
    if (orient == Qt::Horizontal)
    {
        hBoxLayout->addLayout(widgetLayout); // 水平布局时，添加小部件布局
        widgetLayout->setSpacing(10); // 设置小部件布局内元素的间距
    }
    else
    {
        textLayout->addLayout(widgetLayout); // 垂直布局时，添加小部件布局到文本布局
    }

    // 添加关闭按钮到布局
    hBoxLayout->addSpacing(12); // 添加间隔
    hBoxLayout->addWidget(closeButton, 0, Qt::AlignTop | Qt::AlignLeft); // 将关闭按钮添加到布局，左上对齐

    // 调整文本的显示
    adjustText();
}

void EsInfoBarPrivate::setQss()
{
    Q_Q(EsInfoBar);
    // 设置 titleLabel 和 contentLabel 的对象名称，用于在 QSS 中引用
    titleLabel->setObjectName("titleLabel");
    contentLabel->setObjectName("contentLabel");

    // 如果 icon 是枚举类型，设置 "type" 属性为 icon 的值
    if (icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        q->setProperty("type", icon.value<EsIconBase*>()->getIconNameStr());
    }

    auto style = new EsStyleSheet(Es::Style_INFO_BAR);
    style->apply(q);
}

void EsInfoBarPrivate::fadeOut()
{
    Q_Q(EsInfoBar);
    // 淡出效果

    // 设置动画持续时间为 200 毫秒
    opacityAni->setDuration(200);

    // 设置动画的起始值和结束值
    opacityAni->setStartValue(1);
    opacityAni->setEndValue(0);

    // 当动画完成时，连接到关闭窗口的槽函数
    connect(opacityAni, &QPropertyAnimation::finished, q, &EsInfoBar::close);

    // 启动动画
    opacityAni->start();
}

/**
 * @brief 调整文本内容的显示
 *
 * 该函数根据父控件的宽度来调整标题和内容的显示方式，确保文本的适配。
 * 标题和内容的文本会被自动换行，以便在给定宽度下正确显示。
 */
void EsInfoBarPrivate::adjustText()
{
    Q_Q(EsInfoBar);
    // 获取宽度，默认值为 900，若有父控件则计算父控件的宽度减去 50
    int w = 900;
    if (q->parentWidget())
    {
        w = q->parentWidget()->width() - 50;
    }

    // 调整标题文本
    int chars = std::max(std::min(w / 10, 120), 30);
    this->titleLabel->setText(EsTextWrap::wrap(this->title, chars, false).first);

    // 调整内容文本
    chars = std::max(std::min(w / 9, 120), 30);
    this->contentLabel->setText(EsTextWrap::wrap(this->content, chars, false).first);

    // 调整控件大小
    q->adjustSize();
}

// endregion


// region ================= EsInfoBarManager =================

E_SINGLETON_CREATE_CPP(EsInfoBarManager)
QHash<QWidget*, QList<EsInfoBar*>> EsInfoBarManager::infoBars;
QHash<QWidget*, QParallelAnimationGroup*> EsInfoBarManager::aniGroups;
QMap<Es::InfoBarPosition, EsInfoBarManager*> EsInfoBarManager::managers;

EsInfoBarManager::EsInfoBarManager()
    : spacing(16), margin(24)
{
}

void EsInfoBarManager::add(EsInfoBar* infoBar)
{
    auto p = infoBar->parentWidget();
    if (!p) return;

    if (!infoBars.contains(p))
    {
        p->installEventFilter(this);
        infoBars[p] = QList<EsInfoBar*>();
        aniGroups[p] = new QParallelAnimationGroup(this);
    }

    if (infoBars[p].contains(infoBar))
    {
        return;
    }

    // 如果正在播放掉落动画，先停止; 不然此时继续往下执行会输出警告 "starting an animation without end value" 且 信息位置会偶尔从错误的地方飞过来
    if (aniGroups[p]->state() == QAbstractAnimation::Running)
    {
        auto ani = aniGroups[p];
        ani->setCurrentTime(ani->duration());
        ani->stop();
    }


    // 添加掉落动画
    if (!infoBars[p].isEmpty())
    {
        auto dropAni = new QPropertyAnimation(infoBar, "pos", infoBar);
        dropAni->setDuration(200);
        aniGroups[p]->addAnimation(dropAni);
        dropAnis.append(dropAni);
        infoBar->setProperty("dropAni", QVariant::fromValue(dropAni));
    }

    // 添加滑动动画
    infoBars[p].append(infoBar);
    QPropertyAnimation* slideAni = createSlideAni(infoBar);
    slideAnis.append(slideAni);
    infoBar->setProperty("slideAni", QVariant::fromValue(slideAni));

    connect(infoBar, &EsInfoBar::closedSignal, this, [this, infoBar]()
    {
        remove(infoBar);
    });

    slideAni->start();
}

void EsInfoBarManager::remove(EsInfoBar* infoBar)
{
    QWidget* p = infoBar->parentWidget();
    if (!infoBars.contains(p)) return;

    if (!infoBars[p].contains(infoBar)) return;

    infoBars[p].removeAll(infoBar);


    // 移除掉落动画
    if (auto dropAni = infoBar->property("dropAni").value<QPropertyAnimation*>())
    {
        aniGroups[p]->removeAnimation(dropAni);
        dropAnis.removeAll(dropAni);
        dropAni->deleteLater();
    }

    // 移除滑动动画
    if (auto slideAni = infoBar->property("slideAni").value<QPropertyAnimation*>())
    {
        slideAnis.removeAll(slideAni);
        slideAni->deleteLater();
    }


    // 更新剩余信息条的位置
    updateDropAni(p);
    aniGroups[p]->start();
}

bool EsInfoBarManager::eventFilter(QObject* obj, QEvent* e)
{
    QWidget* p = qobject_cast<QWidget*>(obj);
    if (!p || !infoBars.contains(p)) return false;

    if (e->type() == QEvent::Resize || e->type() == QEvent::WindowStateChange)
    {
        QSize size = (e->type() == QEvent::Resize) ? static_cast<QResizeEvent*>(e)->size() : QSize();
        for (EsInfoBar* bar : infoBars[p])
        {
            bar->move(pos(bar, size));
        }
    }
    return QObject::eventFilter(obj, e);
}

void EsInfoBarManager::registerManager(Es::InfoBarPosition name, EsInfoBarManager* manager)
{
    if (!managers.contains(name))
    {
        managers.insert(name, manager);
    }
}

EsInfoBarManager* EsInfoBarManager::make(Es::InfoBarPosition position)
{
    if (!managers.contains(position))
    {
        // 动态注册内置信息条管理器
        switch (position)
        {
        case Es::InfoBarPosition::TOP:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::TOP, new EsTopInfoBarManager());
            break;
        case Es::InfoBarPosition::BOTTOM:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::BOTTOM, new EsBottomInfoBarManager());
            break;
        case Es::InfoBarPosition::TOP_LEFT:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::TOP_LEFT, new EsTopLeftInfoBarManager());
            break;
        case Es::InfoBarPosition::TOP_RIGHT:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::TOP_RIGHT, new EsTopRightInfoBarManager());
            break;
        case Es::InfoBarPosition::BOTTOM_LEFT:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::BOTTOM_LEFT, new EsBottomLeftInfoBarManager());
            break;
        case Es::InfoBarPosition::BOTTOM_RIGHT:
            EsTopInfoBarManager::registerManager(Es::InfoBarPosition::BOTTOM_RIGHT, new EsBottomRightInfoBarManager());
            break;
        default:
            qDebug() << "无效的 Es::InfoBarPosition:" << (int)position <<
                ", 自定义信息条位置时, 应在使用前调用 EsTopInfoBarManager::registerManager 注册";
            return nullptr;
        }
    }
    return managers[position];
}



QPoint EsInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    qWarning() << "子类未重写 EsInfoBarManager::pos";
    return {};
}

QPoint EsInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    qWarning() << "子类未重写 EsInfoBarManager::slideStartPos";
    return {};
}

QPropertyAnimation* EsInfoBarManager::createSlideAni(EsInfoBar* infoBar)
{
    auto slideAni = new QPropertyAnimation(infoBar, "pos", infoBar);
    slideAni->setEasingCurve(QEasingCurve::OutQuad);
    slideAni->setDuration(200);

    slideAni->setStartValue(slideStartPos(infoBar));
    slideAni->setEndValue(pos(infoBar));
    return slideAni;
}

void EsInfoBarManager::updateDropAni(QWidget* parent)
{
    for (EsInfoBar* bar : infoBars[parent])
    {
        auto ani = bar->property("dropAni").value<QPropertyAnimation*>();
        if (!ani) continue;
        ani->setStartValue(bar->pos());
        ani->setEndValue(pos(bar));
    }
}

// endregion


// region ================= EsTopInfoBarManager =================


QPoint EsTopInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();

    int x = (infoBar->parentWidget()->width() - infoBar->width()) / 2.f;
    int y = margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y += bar->height() + spacing;
    }

    return {x, y};
}

QPoint EsTopInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    QPoint p = pos(infoBar);
    return {p.x(), p.y() - 16};
}

// endregion

// region ================= TopRightInfoBarManager =================

QPoint EsTopRightInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();
    parentSize = parentSize.isEmpty() ? p->size() : parentSize;

    int x = parentSize.width() - infoBar->width() - margin;
    int y = margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y += bar->height() + spacing;
    }

    return {x, y};
}

QPoint EsTopRightInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    return {infoBar->parentWidget()->width(), pos(infoBar).y()};
}

// endregion

// region ================= EsBottomRightInfoBarManager =================



QPoint EsBottomRightInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();
    parentSize = parentSize.isEmpty() ? p->size() : parentSize;

    int x = parentSize.width() - infoBar->width() - margin;
    int y = parentSize.height() - infoBar->height() - margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y -= bar->height() + spacing;
    }

    return {x, y};
}

QPoint EsBottomRightInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    return {infoBar->parentWidget()->width(), pos(infoBar).y()};
}

// endregion

// region ================= EsTopLeftInfoBarManager =================


QPoint EsTopLeftInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();
    parentSize = parentSize.isEmpty() ? p->size() : parentSize;

    int y = margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y += bar->height() + spacing;
    }

    return {margin, y};
}

QPoint EsTopLeftInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    return {-infoBar->parentWidget()->width(), pos(infoBar).y()};
}

// endregion

// region ================= EsBottomLeftInfoBarManager =================

QPoint EsBottomLeftInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();
    parentSize = parentSize.isEmpty() ? p->size() : parentSize;

    int y = parentSize.height() - infoBar->height() - margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y -= bar->height() + spacing;
    }

    return {margin, y};
}

QPoint EsBottomLeftInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    return {-infoBar->parentWidget()->width(), pos(infoBar).y()};
}

// endregion

// region ================= EsBottomInfoBarManager =================

QPoint EsBottomInfoBarManager::pos(EsInfoBar* infoBar, QSize parentSize)
{
    QWidget* p = infoBar->parentWidget();
    parentSize = parentSize.isEmpty() ? p->size() : parentSize;

    int x = (parentSize.width() - infoBar->width()) / 2.f;
    int y = parentSize.height() - infoBar->height() - margin;
    int index = infoBars[p].indexOf(infoBar);
    for (EsInfoBar* bar : infoBars[p].mid(0, index))
    {
        y -= bar->height() + spacing;
    }

    return {x, y};
}

QPoint EsBottomInfoBarManager::slideStartPos(EsInfoBar* infoBar)
{
    auto p = pos(infoBar);
    return {p.x(), p.y() + 16};
}

// endregion

// region ================= EsInfoIconWidget =================

EsInfoIconWidget::EsInfoIconWidget(const QVariant& icon, QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(36, 36);
    this->icon = icon;
}

EsInfoIconWidget::~EsInfoIconWidget()
{
    // icon会在EsInfoBar的析构释放, 所以不需要释放
}

void EsInfoIconWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    auto rect = QRectF(10, 10, 15, 15);
    if (icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        auto ico = icon.value<EsIconBase*>();
        if (ico->getIconName() != Es::IconInfoBar_Info)
        {
            EsFunc::drawIcon(icon, &painter, rect);
            return;
        }
    }
    EsFunc::drawIcon(icon, &painter, rect, QIcon::Off, {0}, {{"fill", EsFunc::themeColor().name()}});
}

// endregion


// region ================= EsInfoBar =================

EsInfoBar::EsInfoBar(QWidget* parent, const QVariant& icon, const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position)
    : QFrame(parent), d_ptr(new EsInfoBarPrivate())
{
    Q_D(EsInfoBar);
    d->q_ptr = this;

    d->title = title;
    d->content = content;
    d->orient = orient;
    d->icon = icon;
    d->duration = duration;
    d->isClosable = isClosable;
    d->position = position;

    // 创建控件
    d->titleLabel = new QLabel(title, this);
    d->contentLabel = new QLabel(content, this);
    d->closeButton = new EsTransparentToolButton(Es::Icon_Close, this);
    d->iconWidget = new EsInfoIconWidget(icon, this);

    // 设置布局
    d->hBoxLayout = new QHBoxLayout(this);
    if (orient == Qt::Horizontal)
    {
        d->textLayout = new QHBoxLayout();
        d->widgetLayout = new QHBoxLayout();
    }
    else
    {
        d->textLayout = new QVBoxLayout();
        d->widgetLayout = new QVBoxLayout();
    }


    // 设置透明效果
    d->opacityEffect = new QGraphicsOpacityEffect(this);
    d->opacityAni = new QPropertyAnimation(d->opacityEffect, "opacity", this);

    d->initWidget();
}

EsInfoBar::EsInfoBar(Es::IconName icon, const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
    : EsInfoBar(parent, QVariant::fromValue<EsIconBase*>(new EsInfoBarIcon(icon)), title, content, orient, isClosable, duration,
                position)
{
}

EsInfoBar::EsInfoBar(const QString& icon, const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
    : EsInfoBar(parent, QVariant::fromValue(icon), title, content, orient, isClosable, duration, position)
{
}

EsInfoBar::EsInfoBar(EsIconBase* icon, const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
    : EsInfoBar(parent, QVariant::fromValue(icon), title, content, orient, isClosable, duration, position)
{
}

EsInfoBar::EsInfoBar(const QIcon& icon, const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
    : EsInfoBar(parent, QVariant::fromValue(icon), title, content, orient, isClosable, duration, position)
{
}

EsInfoBar::~EsInfoBar()
{
    Q_D(EsInfoBar);
    if (d->icon.userType() == qMetaTypeId<EsIconBase*>()) delete d->icon.value<EsIconBase*>();

}

void EsInfoBar::addWidget(QWidget* widget, int stretch)
{
    Q_D(EsInfoBar);
    // 添加间距
    d->widgetLayout->addSpacing(6);

    // 设置控件的对齐方式
    Qt::Alignment align = (d->orient == Qt::Vertical) ? Qt::AlignTop : Qt::AlignVCenter;

    // 向布局中添加控件，使用指定的对齐方式
    d->widgetLayout->addWidget(widget, stretch, Qt::AlignLeft | align);
}

void EsInfoBar::setCustomBackgroundColor(const QColor& light, const QColor& dark)
{
    Q_D(EsInfoBar);
    // 设置浅色和深色模式下的背景颜色
    d->lightBackgroundColor = light;
    d->darkBackgroundColor = dark;

    // 更新控件外观
    this->update();
}

bool EsInfoBar::eventFilter(QObject* obj, QEvent* e)
{
    Q_D(EsInfoBar);
    // 只处理父控件的 Resize 和 WindowStateChange 事件
    if (obj == this->parent())
    {
        if (e->type() == QEvent::Resize || e->type() == QEvent::WindowStateChange)
        {
            d->adjustText(); // 调整文本
        }
    }

    // 调用父类的事件过滤器
    return QFrame::eventFilter(obj, e);
}

EsInfoBar* EsInfoBar::newShow(const QVariant& icon, const QString& title, const QString& content, Qt::Orientation orient,
                              bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
{
    auto w = new EsInfoBar(parent, icon, title, content, orient, isClosable, duration, position);
    w->show();
    return w;
}

EsInfoBar* EsInfoBar::info(const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
{
    auto icon = new EsInfoBarIcon(Es::IconName::IconInfoBar_Info);
    return EsInfoBar::newShow(QVariant::fromValue<EsIconBase*>(icon), title, content, orient, isClosable, duration, position, parent);
}


EsInfoBar* EsInfoBar::success(const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
{
    auto icon = new EsInfoBarIcon(Es::IconName::IconInfoBar_Success);
    return EsInfoBar::newShow(QVariant::fromValue<EsIconBase*>(icon), title, content, orient, isClosable, duration, position, parent);
}

EsInfoBar* EsInfoBar::warning(const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
{
    auto icon = new EsInfoBarIcon(Es::IconName::IconInfoBar_Warning);
    return EsInfoBar::newShow(QVariant::fromValue<EsIconBase*>(icon), title, content, orient, isClosable, duration, position, parent);
}

EsInfoBar* EsInfoBar::error(const QString& title, const QString& content, Qt::Orientation orient, bool isClosable, int duration, Es::InfoBarPosition position, QWidget* parent)
{
    auto icon = new EsInfoBarIcon(Es::IconName::IconInfoBar_Error);
    return EsInfoBar::newShow(QVariant::fromValue<EsIconBase*>(icon), title, content, orient, isClosable, duration, position, parent);
}

void EsInfoBar::closeEvent(QCloseEvent* e)
{
    emit closedSignal();
    this->deleteLater();

    // 忽略事件，防止控件被立即关闭
    e->ignore();
}

void EsInfoBar::showEvent(QShowEvent* e)
{
    Q_D(EsInfoBar);
    d->adjustText();
    QFrame::showEvent(e);

    // 如果有持续时间，设置定时器执行淡出效果
    if (d->duration >= 0)
    {
        QTimer::singleShot(d->duration, d, &EsInfoBarPrivate::fadeOut);
    }

    // 如果信息条位置不为 NONE，添加到管理器
    if (d->position != Es::InfoBarPosition::NONE)
    {
        auto manager = EsInfoBarManager::make(d->position);
        manager->add(this);
    }

    if (parent())
    {
        parent()->installEventFilter(this);
    }
}

void EsInfoBar::paintEvent(QPaintEvent* e)
{
    Q_D(EsInfoBar);
    QFrame::paintEvent(e);

    // 如果没有设置自定义背景颜色，就用qss默认颜色;
    if (!d->lightBackgroundColor.isValid())
        // 避雷: 就算这样初始化 lightBackgroundColor={} 它也会认为有效(返回true); 想用 QColor.isValid() 判断非空, 要么不初始化, 要么初始化为nullptr
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing); // 启用抗锯齿
    painter.setPen(Qt::NoPen); // 禁用边框绘制

    // 根据当前主题选择背景颜色
    if (EsFunc::isDarkTheme())
    {
        painter.setBrush(d->darkBackgroundColor); // 深色主题背景
    }
    else
    {
        painter.setBrush(d->lightBackgroundColor); // 浅色主题背景
    }

    // 绘制圆角矩形背景
    QRect rect = this->rect().adjusted(1, 1, -1, -1);
    painter.drawRoundedRect(rect, 6, 6); // 圆角半径为6
}

// endregion
