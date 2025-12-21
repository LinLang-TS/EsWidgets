/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esRoundMenu.cpp
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esRoundMenu.h"

#include <qgraphicseffect.h>
#include <QHBoxLayout>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QTextLayout>
#include <QTimer>

#include "esFont.h"
#include "esIcon.h"
#include "esMenuActionListWidget.h"
#include "esScreen.h"
#include "esStyleSheet.h"
#include "delegate/esIndicatorMenuItemDelegate.h"
#include "private/esRoundMenu_p.h"

// region ================= EsMenuAnimationManager  =================

QMap<Es::MenuAnimationType, EsMenuAnimationManager*> EsMenuAnimationManager::managers;

EsMenuAnimationManager::EsMenuAnimationManager(const QVariant& menu, QObject* parent)
    : QObject(parent),
      menu(menu),
      ani(new QPropertyAnimation(menu.value<QObject*>(), "pos", menu.value<QObject*>()))
{
    ani->setDuration(250);
    ani->setEasingCurve(QEasingCurve::OutQuad);

    connect(ani, &QPropertyAnimation::valueChanged,
            this, &EsMenuAnimationManager::onValueChanged);
    connect(ani, &QPropertyAnimation::valueChanged,
            this, &EsMenuAnimationManager::updateMenuViewport);
}

EsMenuAnimationManager::~EsMenuAnimationManager()
{
}

void EsMenuAnimationManager::registerManager(Es::MenuAnimationType name, EsMenuAnimationManager* manager)
{
    if (!managers.contains(name))
        managers.insert(name, manager);
}

EsMenuAnimationManager* EsMenuAnimationManager::make(const QVariant& menu, Es::MenuAnimationType aniType)
{
    if (!managers.contains(aniType))
    {
        // 动态注册内置动画类型
        switch (aniType)
        {
        case Es::MenuAnimationType::NONE:
            EsMenuAnimationManager::registerManager(Es::MenuAnimationType::NONE, new EsDummyMenuAnimationManager({}));
            break;
        case Es::MenuAnimationType::DROP_DOWN:
            EsMenuAnimationManager::registerManager(Es::MenuAnimationType::DROP_DOWN, new EsDropDownMenuAnimationManager({}));
            break;
        case Es::MenuAnimationType::PULL_UP:
            EsMenuAnimationManager::registerManager(Es::MenuAnimationType::PULL_UP, new EsPullUpMenuAnimationManager({}));
            break;
        case Es::MenuAnimationType::FADE_IN_DROP_DOWN:
            EsMenuAnimationManager::registerManager(Es::MenuAnimationType::FADE_IN_DROP_DOWN, new EsFadeInDropDownMenuAnimationManager({}));
            break;
        case Es::MenuAnimationType::FADE_IN_PULL_UP:
            EsMenuAnimationManager::registerManager(Es::MenuAnimationType::FADE_IN_PULL_UP, new EsFadeInPullUpMenuAnimationManager({}));
            break;

        default:
            qWarning() << "无效的 MenuAnimationType:" << (int)aniType;
            return nullptr;
        }
    }

    return managers[aniType]->create(menu);
}


void EsMenuAnimationManager::onValueChanged()
{
}

void EsMenuAnimationManager::updateMenuViewport()
{
    auto menu = this->menu.value<EsRoundMenu*>();
    menu->view->viewport()->update();
    menu->view->setAttribute(Qt::WA_UnderMouse, true);

    QHoverEvent e(QEvent::HoverEnter, QPoint(), QPoint(1, 1));
    QApplication::sendEvent(menu->view, &e);
}

QSize EsMenuAnimationManager::availableViewSize(const QPoint& pos)
{
    QRect ss = EsFunc::getCurrentScreenGeometry();
    int w = ss.width() - 100;
    int h = ss.height() - 100;
    return {w, h};
}

QPoint EsMenuAnimationManager::endPosition(const QPoint& pos)
{
    auto menu = this->menu.value<EsRoundMenu*>();
    QRect rect = EsFunc::getCurrentScreenGeometry();

    QSize s(menu->width() + 5, menu->height());

    int x = qMin(pos.x() - menu->layout()->contentsMargins().left(),
                 rect.right() - s.width());

    int y = qMin(pos.y() - 4,
                 rect.bottom() - s.height() + 10);

    return {x, y};
}

QSize EsMenuAnimationManager::menuSize()
{
    auto menu_ = menu.value<EsRoundMenu*>();
    auto m = menu_->layout()->contentsMargins();
    int w = menu_->view->width() + m.left() + m.right() + 120;
    int h = menu_->view->height() + m.top() + m.bottom() + 20;
    return {w, h};
}

void EsMenuAnimationManager::exec(const QPoint& pos)
{
    qWarning() << "子类未重写 EsMenuAnimationManager::exec";
}


// endregion

// region ================= EsDummyMenuAnimationManager  =================

EsDummyMenuAnimationManager::EsDummyMenuAnimationManager(const QVariant& menu)
    : EsMenuAnimationManager(menu)
{
}

void EsDummyMenuAnimationManager::exec(const QPoint& pos)
{
    auto menu = this->menu.value<EsRoundMenu*>();
    menu->move(endPosition(pos));
}

EsMenuAnimationManager* EsDummyMenuAnimationManager::create(const QVariant& menu)
{
    return new EsDummyMenuAnimationManager(menu);
}

// endregion

// region ================= EsDropDownMenuAnimationManager  =================

EsDropDownMenuAnimationManager::EsDropDownMenuAnimationManager(const QVariant& menu)
    : EsMenuAnimationManager(menu)
{
}


void EsDropDownMenuAnimationManager::exec(const QPoint& pos)
{
    auto menu = this->menu.value<EsRoundMenu*>();
    QPoint endPos = endPosition(pos);
    int h = menu->height() + 5;

    ani->setStartValue(endPos - QPoint(0, h / 2));
    ani->setEndValue(endPos);
    ani->start();
}

QSize EsDropDownMenuAnimationManager::availableViewSize(const QPoint& pos)
{
    QRect ss = EsFunc::getCurrentScreenGeometry();
    return {ss.width() - 100, qMax(ss.bottom() - pos.y() - 10, 1)};
}

void EsDropDownMenuAnimationManager::onValueChanged()
{
    auto menu = this->menu.value<EsRoundMenu*>();
    auto size = menuSize();
    int y = ani->endValue().value<QPoint>().y() - ani->currentValue().value<QPoint>().y();
    menu->setMask(QRegion(0, y, size.width(), size.height()));
}

EsMenuAnimationManager* EsDropDownMenuAnimationManager::create(const QVariant& menu)
{
    return new EsDropDownMenuAnimationManager(menu);
}

// endregion

// region ================= EsPullUpMenuAnimationManager  =================

EsPullUpMenuAnimationManager::EsPullUpMenuAnimationManager(const QVariant& menu)
    : EsMenuAnimationManager(menu)
{
}

EsMenuAnimationManager* EsPullUpMenuAnimationManager::create(const QVariant& menu)
{
    return new EsPullUpMenuAnimationManager(menu);
}

QPoint EsPullUpMenuAnimationManager::endPosition(const QPoint& pos )
{
    auto m = this->menu.value<EsRoundMenu*>();
    auto rect = EsFunc::getCurrentScreenGeometry();
    auto w = m->width() + 5;
    auto h = m->height();
    auto x = qMin(pos.x() - m->layout()->contentsMargins().left(), rect.right() - w);
    auto y = qMax(pos.y() - h + 13, rect.top() + 4);
    return {x, y};
}

void EsPullUpMenuAnimationManager::exec(const QPoint& pos)
{
    auto menu = this->menu.value<EsRoundMenu*>();
    QPoint endPos = endPosition(pos);
    int h = menu->height() + 5;

    ani->setStartValue(endPos + QPoint(0, h / 2));
    ani->setEndValue(endPos);
    ani->start();
}

QSize EsPullUpMenuAnimationManager::availableViewSize(const QPoint& pos)
{
    QRect ss = EsFunc::getCurrentScreenGeometry();
    return {ss.width() - 100, qMax(pos.y() - ss.top() - 28, 1)};
}

void EsPullUpMenuAnimationManager::onValueChanged()
{
    auto menu = this->menu.value<EsRoundMenu*>();
    auto size = menuSize();
    int y = ani->endValue().value<QPoint>().y() - ani->currentValue().value<QPoint>().y();
    menu->setMask(QRegion(0, y, size.width(), size.height() - 28));
}


// endregion

// region ================= EsFadeInDropDownMenuAnimationManager  =================

EsFadeInDropDownMenuAnimationManager::EsFadeInDropDownMenuAnimationManager(const QVariant& menu)
    : EsMenuAnimationManager(menu),
      opacityAni(new QPropertyAnimation(this->menu.value<EsRoundMenu*>(), "windowOpacity", this)),
      aniGroup(new QParallelAnimationGroup(this))
{
    aniGroup->addAnimation(ani);
    aniGroup->addAnimation(opacityAni);
}

EsMenuAnimationManager* EsFadeInDropDownMenuAnimationManager::create(const QVariant& menu)
{
    return new EsFadeInDropDownMenuAnimationManager(menu);
}

void EsFadeInDropDownMenuAnimationManager::exec(const QPoint& pos)
{
    QPoint endPos = endPosition(pos);

    // 透明度渐显
    opacityAni->setStartValue(0);
    opacityAni->setEndValue(1);
    opacityAni->setDuration(150);
    opacityAni->setEasingCurve(QEasingCurve::OutQuad);

    // 位置动画：从稍上方落下
    ani->setStartValue(endPos - QPoint(0, 8));
    ani->setEndValue(endPos);
    ani->setDuration(150);
    ani->setEasingCurve(QEasingCurve::OutQuad);

    aniGroup->start();
}

QSize EsFadeInDropDownMenuAnimationManager::availableViewSize(const QPoint& pos)
{
    QRect ss = EsFunc::getCurrentScreenGeometry();
    return { ss.width() - 100, qMax(ss.bottom() - pos.y() - 10, 1) };
}
// endregion

// region ================= EsFadeInPullUpMenuAnimationManager  =================

EsFadeInPullUpMenuAnimationManager::EsFadeInPullUpMenuAnimationManager(const QVariant& menu)
    : EsMenuAnimationManager(menu),
      opacityAni(new QPropertyAnimation(this->menu.value<EsRoundMenu*>(), "windowOpacity", this)),
      aniGroup(new QParallelAnimationGroup(this))
{
    aniGroup->addAnimation(ani);
    aniGroup->addAnimation(opacityAni);
}

EsMenuAnimationManager* EsFadeInPullUpMenuAnimationManager::create(const QVariant& menu)
{
    return new EsFadeInPullUpMenuAnimationManager(menu);
}

QPoint EsFadeInPullUpMenuAnimationManager::endPosition(const QPoint& pos )
{
    auto m = this->menu.value<EsRoundMenu*>();
    auto rect = EsFunc::getCurrentScreenGeometry();
    auto w = m->width() + 5;
    auto h = m->height();
    auto x = qMin(pos.x() - m->layout()->contentsMargins().left(), rect.right() - w);
    auto y = qMax(pos.y() - h + 15, rect.top() + 4);
    return {x, y};
}

void EsFadeInPullUpMenuAnimationManager::exec(const QPoint& pos)
{
    auto menu = this->menu.value<EsRoundMenu*>();
    QPoint endPos = endPosition(pos);

    // 渐显透明度动画
    opacityAni->setStartValue(0);
    opacityAni->setEndValue(1);
    opacityAni->setDuration(150);
    opacityAni->setEasingCurve(QEasingCurve::OutQuad);

    // 上移动画（从最终位置的下方 8px 开始进入）
    ani->setStartValue(endPos + QPoint(0, 8));
    ani->setEndValue(endPos);
    ani->setDuration(200);
    ani->setEasingCurve(QEasingCurve::OutQuad);

    aniGroup->start();
}

QSize EsFadeInPullUpMenuAnimationManager::availableViewSize(const QPoint& pos)
{
    QRect ss = EsFunc::getCurrentScreenGeometry();
    return {
        ss.width() - 100,
        qMax(pos.y() - ss.top() - 28, 1)
    };
}
// endregion

// region ================= EsSubMenuItemWidget  =================


EsSubMenuItemWidget::EsSubMenuItemWidget(EsRoundMenu* menu, QListWidgetItem* item, QWidget* parent)
    : QWidget(parent), menu(menu), item(item)
{
}

void EsSubMenuItemWidget::enterEvent(QEvent* e)
{
    QWidget::enterEvent(e);
    emit showMenuSig(item);
}

void EsSubMenuItemWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect(width() - 10,
                height() / 2.0 - 9.0 / 2.0,
                9.0,
                9.0);

    auto icon = EsIcon(Es::Icon_ChevronRight);
    icon.render(&painter, rect, {});
}

// endregion

// region ================= EsRoundMenuPrivate  =================

EsRoundMenuPrivate::EsRoundMenuPrivate(QObject* parent)
    : QObject(parent),
      isSubMenu(false),
      lastHoverItem(nullptr),
      lastHoverSubMenuItem(nullptr),
      isHideBySystem(true),
      itemHeight(28),
      aniManager(nullptr)
{
}

void EsRoundMenuPrivate::initWidgets()
{
    Q_Q(EsRoundMenu);
    // 设置窗口属性
    q->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    q->setAttribute(Qt::WA_TranslucentBackground);
    q->setMouseTracking(true);

    // 配置计时器
    timer->setSingleShot(true);
    timer->setInterval(400);
    connect(timer, &QTimer::timeout, this, &EsRoundMenuPrivate::_onShowMenuTimeOut);

    // 设置阴影效果
    q->setShadowEffect();

    // 布局添加 view
    hBoxLayout->addWidget(q->view, 1, Qt::AlignCenter);

    // 设置布局边距
    hBoxLayout->setContentsMargins(12, 8, 12, 20);

    // 应用样式表
    auto style = new EsStyleSheet(Es::StyleSheet::Style_MENU);
    style->apply(q);

    // 连接信号
    connect(q->view, &EsMenuActionListWidget::itemClicked, q, &EsRoundMenu::_onItemClicked);
    connect(q->view, &EsMenuActionListWidget::itemEntered, this, &EsRoundMenuPrivate::_onItemEntered);
}

// endregion

// region ================= EsRoundMenu  =================

EsRoundMenu::EsRoundMenu(const QString& title, QWidget* parent)
    : QMenu(parent),
      menuItem(nullptr),
      parentMenu(nullptr),
      d_ptr(new EsRoundMenuPrivate())
{
    Q_D(EsRoundMenu);
    d->q_ptr = this;

    d->icon = QVariant::fromValue(QIcon());
    d->hBoxLayout = new QHBoxLayout(this);
    d->timer = new QTimer(this);
    view = new EsMenuActionListWidget(this);


    this->setTitle(title);

    d->initWidgets();
}

EsRoundMenu::~EsRoundMenu()
{
    Q_D(EsRoundMenu);
    delete d->aniManager;
}

void EsRoundMenu::setMaxVisibleItems(int num)
{
    view->setMaxVisibleItems(num);
    this->adjustSize();
}

void EsRoundMenu::setItemHeight(int height)
{
    Q_D(EsRoundMenu);
    if (height == d->itemHeight)
        return;

    d->itemHeight = height;
    view->setItemHeight(height);
}

void EsRoundMenu::setShadowEffect(int blurRadius, const QPoint& offset, const QColor& color)
{
    auto shadowEffect = new QGraphicsDropShadowEffect(view);
    shadowEffect->setBlurRadius(blurRadius);
    shadowEffect->setOffset(offset);
    shadowEffect->setColor(color);

    view->setGraphicsEffect(nullptr); // 先清除之前的效果
    view->setGraphicsEffect(shadowEffect);
}

void EsRoundMenu::_setParentMenu(EsRoundMenu* parent, QListWidgetItem* item)
{
    Q_D(EsRoundMenu);
    parentMenu = parent;
    menuItem = item;
    d->isSubMenu = (parent != nullptr);
}

void EsRoundMenu::adjustSize()
{
    QMargins m = this->layout()->contentsMargins();
    int w = view->width() + m.left() + m.right();
    int h = view->height() + m.top() + m.bottom();
    this->setFixedSize(w, h);
}

QIcon EsRoundMenu::icon() const
{
    Q_D(const EsRoundMenu);
    if (d->icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        return QIcon(d->icon.value<EsIconBase*>()->icon());
    }
    return d->icon.value<QIcon>();
}

QString EsRoundMenu::title() const
{
    Q_D(const EsRoundMenu);
    return d->title;
}

void EsRoundMenu::clear()
{
    Q_D(EsRoundMenu);
    // 清除动作
    while (!d->actions.isEmpty())
    {
        EsAction* act = d->actions.takeLast();
        this->removeAction(act);
    }

    // 清除子菜单
    while (!d->subMenus.isEmpty())
    {
        EsRoundMenu* menu = d->subMenus.takeLast();
        this->removeAction((EsAction*)menu->menuAction());
    }
}

void EsRoundMenu::setIcon(const QVariant& icon)
{
    Q_D(EsRoundMenu);
    d->icon = icon;
}

void EsRoundMenu::setTitle(const QString& title)
{
    Q_D(EsRoundMenu);
    d->title = title;
    QMenu::setTitle(title);
}

void EsRoundMenu::addAction(EsAction* action)
{
    QListWidgetItem* item = _createActionItem(action);
    view->addItem(item);
    this->adjustSize();
}

void EsRoundMenu::addWidget(QWidget* widget, bool selectable, const std::function<void()>& onClick)
{
    EsAction* action = new EsAction(this);
    action->setProperty("selectable", selectable);

    QListWidgetItem* item = _createActionItem(action);
    item->setSizeHint(widget->size());

    view->addItem(item);
    view->setItemWidget(item, widget);

    if (!selectable)
    {
        item->setFlags(Qt::NoItemFlags);
    }

    if (onClick)
    {
        // 使用 lambda 连接 EsAction::triggered 信号到 std::function
        connect(action, &EsAction::triggered, this, [onClick]() { onClick(); });
    }

    this->adjustSize();
}

QListWidgetItem* EsRoundMenu::_createActionItem(EsAction* action, EsAction* before)
{
    Q_D(EsRoundMenu);
    if (!before)
    {
        d->actions.append(action);
        QMenu::addAction(action);
    }
    else if (d->actions.contains(before))
    {
        int index = d->actions.indexOf(before);
        d->actions.insert(index, action);
        QMenu::insertAction(before, action);
    }
    else
    {
        throw std::invalid_argument("`before` is not in the action list");
    }

    QListWidgetItem* item = new QListWidgetItem(d->_createItemIcon(action), action->text());
    _adjustItemText(item, action);

    // 如果动作不可用，则禁用列表项
    if (!action->isEnabled())
    {
        item->setFlags(Qt::NoItemFlags);
    }

    if (action->text() != action->toolTip())
    {
        item->setToolTip(action->toolTip());
    }

    item->setData(Qt::UserRole, QVariant::fromValue(action));
    action->setProperty("item", QVariant::fromValue(item));
    connect(action, &EsAction::changed, d, &EsRoundMenuPrivate::_onActionChanged);

    return item;
}

bool EsRoundMenuPrivate::_hasItemIcon() const
{
    for (EsAction* act : actions)
    {
        if (!act->icon().isNull())
            return true;
    }

    for (EsRoundMenu* menu : subMenus)
    {
        if (!menu->icon().isNull())
            return true;
    }

    return false;
}

int EsRoundMenu::_adjustItemText(QListWidgetItem* item, EsAction* action)
{
    Q_D(EsRoundMenu);
    int sw = 0;

    // 如果当前委托是 ShortcutMenuItemDelegate，计算最宽快捷键的宽度
    if (qobject_cast<EsShortcutMenuItemDelegate*>(this->view->itemDelegate()))
    {
        sw = d->_longestShortcutWidth();
        if (sw > 0)
            sw += 22;
    }

    int w = 0;
    QFontMetrics fm = this->view->fontMetrics();

    // 调整item宽度
    if (!d->_hasItemIcon())
    {
        // 不含图标时
        item->setText(action->text());
        w = 40 + fm.width(action->text()) + sw;
    }
    else
    {
        // 含图标：前面加一个空格
        item->setText(" " + action->text());

        int space = 4 - fm.width(" ");

        w = 60 + fm.width(item->text()) + sw + space;
    }

    item->setSizeHint(QSize(w, d->itemHeight));
    return w;
}

int EsRoundMenuPrivate::_longestShortcutWidth() const
{
    Q_Q(const EsRoundMenu);
    QFont font = EsFunc::getFont(12);
    qreal maxWidth = 0;

    for (EsAction* action : q->menuActions())
    {
        QString shortcut = action->shortcut().toString(QKeySequence::NativeText);

        // 使用 QTextLayout 计算文本宽度
        QTextLayout layout(shortcut, font);
        layout.beginLayout();
        QTextLine line = layout.createLine();
        line.setNumColumns(1);
        layout.endLayout();

        QRectF rect = layout.boundingRect();
        if (rect.width() > maxWidth)
            maxWidth = rect.width();
    }

    return static_cast<int>(std::ceil(maxWidth));
}

QIcon EsRoundMenuPrivate::_createItemIcon(EsRoundMenu* w) const
{
    Q_Q(const EsRoundMenu);
    bool hasIcon = _hasItemIcon();

    QIcon icon = w->icon();
    // QIcon icon(new EsIconEngine(ico));

    if (hasIcon && icon.isNull())
    {
        QPixmap pixmap(q->view->iconSize());
        pixmap.fill(Qt::transparent);
        icon = QIcon(pixmap);
    }
    else if (!hasIcon)
    {
        icon = QIcon();
    }

    return icon;
}

QIcon EsRoundMenuPrivate::_createItemIcon(EsAction* w) const
{
    Q_Q(const EsRoundMenu);
    bool hasIcon = _hasItemIcon();


    auto icon = w->icon();
    // QIcon icon(new EsIconEngine(ico)); // qt和pyqt不一样,不支持直接传svg路径

    if (hasIcon && icon.isNull())
    {
        QPixmap pixmap(q->view->iconSize());
        pixmap.fill(Qt::transparent);
        icon = QIcon(pixmap);
    }
    else if (!hasIcon)
    {
        icon = QIcon();
    }

    return icon;
}

void EsRoundMenu::insertAction(EsAction* before, EsAction* action)
{
    Q_D(EsRoundMenu);
    if (!d->actions.contains(before))
        return;

    QVariant beforeItemVar = before->property("item");
    if (!beforeItemVar.isValid())
        return;

    QListWidgetItem* beforeItem = beforeItemVar.value<QListWidgetItem*>();
    int index = view->row(beforeItem);

    QListWidgetItem* item = _createActionItem(action, before);
    view->insertItem(index, item);
    this->adjustSize();
}

void EsRoundMenu::addActions(const QList<EsAction*>& actions)
{
    for (EsAction* action : actions)
    {
        this->addAction(action);
    }
}

void EsRoundMenu::insertActions(EsAction* before, const QList<EsAction*>& actions)
{
    for (EsAction* action : actions)
    {
        this->insertAction(before, action);
    }
}

void EsRoundMenu::removeAction(EsAction* action)
{
    Q_D(EsRoundMenu);
    if (!d->actions.contains(action))
        return;

    // 移除动作
    QVariant itemVar = action->property("item");
    d->actions.removeOne(action);
    action->setProperty("item", QVariant());

    if (!itemVar.isValid())
        return;

    QListWidgetItem* item = itemVar.value<QListWidgetItem*>();

    // 移除列表项
    d->_removeItem(item);

    // 调用基类 removeAction
    QMenu::removeAction(action);
}

void EsRoundMenu::removeMenu(EsRoundMenu* menu)
{
    Q_D(EsRoundMenu);
    if (!d->subMenus.contains(menu))
        return;

    QListWidgetItem* item = menu->menuItem;
    d->subMenus.removeOne(menu);

    // 移除对应的列表项
    d->_removeItem(item);
}

void EsRoundMenu::setDefaultAction(EsAction* action)
{
    Q_D(EsRoundMenu);
    if (!d->actions.contains(action))
        return;

    QVariant itemVar = action->property("item");
    if (itemVar.isValid() || itemVar.isNull())
    {
        QListWidgetItem* item = itemVar.value<QListWidgetItem*>();
        view->setCurrentItem(item);
    }
}

QList<EsAction*> EsRoundMenu::actions() const
{
    QList<EsAction*> result;
    for (QAction* action : QMenu::actions()) {
        if (auto esAction = qobject_cast<EsAction*>(action)) {
            result.append(esAction);
        }
    }
    return result;
}

void EsRoundMenu::addMenu(EsRoundMenu* menu)
{
    Q_D(EsRoundMenu);
    if (!qobject_cast<EsRoundMenu*>(menu))
        throw std::invalid_argument("`menu` should be a valid RoundMenu instance.");

    QListWidgetItem* item;
    QWidget* w;
    auto pair = d->_createSubMenuItem(menu);
    item = pair.first;
    w = pair.second;
    view->addItem(item);
    view->setItemWidget(item, w);
    this->adjustSize();
}

void EsRoundMenu::insertMenu(EsAction* before, EsRoundMenu* menu)
{
    Q_D(EsRoundMenu);
    if (!qobject_cast<EsRoundMenu*>(menu))
        throw std::invalid_argument("`menu` should be a valid RoundMenu instance.");

    if (!d->actions.contains(before))
        throw std::invalid_argument("`before` should be in menu action list");

    QListWidgetItem* item;
    QWidget* w;
    auto pair = d->_createSubMenuItem(menu);
    item = pair.first;
    w = pair.second;

    QVariant beforeItemVar = before->property("item");
    QListWidgetItem* beforeItem = beforeItemVar.value<QListWidgetItem*>();
    view->insertItem(view->row(beforeItem), item);
    view->setItemWidget(item, w);
    this->adjustSize();
}

QPair<QListWidgetItem*, EsSubMenuItemWidget*> EsRoundMenuPrivate::_createSubMenuItem(EsRoundMenu* menu)
{
    Q_Q(EsRoundMenu);
    subMenus.append(menu);

    QListWidgetItem* item = new QListWidgetItem(_createItemIcon(menu), menu->title());
    int wWidth = 0;

    if (!_hasItemIcon())
    {
        wWidth = 60 + q->view->fontMetrics().horizontalAdvance(menu->title());
    }
    else
    {
        // 在文本前加空格，增加图标与文字的间距
        item->setText(" " + item->text());
        wWidth = 72 + q->view->fontMetrics().horizontalAdvance(item->text());
    }

    // 设置父菜单和列表项关联
    menu->_setParentMenu(q, item);

    item->setSizeHint(QSize(wWidth, itemHeight));
    item->setData(Qt::UserRole, QVariant::fromValue(menu));

    EsSubMenuItemWidget* w = new EsSubMenuItemWidget(menu, item, q);
    connect(w, &EsSubMenuItemWidget::showMenuSig, this, &EsRoundMenuPrivate::_showSubMenu);
    w->resize(item->sizeHint());

    return {item, w};
}

void EsRoundMenuPrivate::_removeItem(QListWidgetItem* item)
{
    Q_Q(EsRoundMenu);
    q->view->takeItem(q->view->row(item));
    item->setData(Qt::UserRole, QVariant());

    // 删除关联的控件
    QWidget* widget = q->view->itemWidget(item);
    if (widget)
    {
        widget->deleteLater();
    }
}

void EsRoundMenuPrivate::_showSubMenu(QListWidgetItem* item)
{
    lastHoverItem = item;
    lastHoverSubMenuItem = item;

    // 防抖延迟 400ms
    timer->stop();
    timer->start();
}

void EsRoundMenuPrivate::_onShowMenuTimeOut()
{
    Q_Q(EsRoundMenu);
    if (!lastHoverSubMenuItem || lastHoverItem != lastHoverSubMenuItem)
        return;

    EsSubMenuItemWidget* w = (EsSubMenuItemWidget*)q->view->itemWidget(lastHoverSubMenuItem);
    EsRoundMenu* m = (EsRoundMenu*)w->menu;
    if (!w || m->parentMenu->isHidden())
        return;

    QRect itemRect(w->mapToGlobal(w->rect().topLeft()), w->size());
    int x = itemRect.right() + 5;
    int y = itemRect.y() - 5;

    QRect screenRect = EsFunc::getCurrentScreenGeometry();
    QSize subMenuSize = w->menu->sizeHint();

    if ((x + subMenuSize.width()) > screenRect.right())
    {
        x = qMax(itemRect.left() - subMenuSize.width() - 5, screenRect.left());
    }

    if ((y + subMenuSize.height()) > screenRect.bottom())
    {
        y = screenRect.bottom() - subMenuSize.height();
    }

    y = std::max(y, screenRect.top());

    w->menu->exec(QPoint(x, y));
}

void EsRoundMenu::addSeparator()
{
    QMargins m = view->viewportMargins();

    int w = view->width() - m.left() - m.right();

    // 创建分隔符列表项
    QListWidgetItem* item = new QListWidgetItem();
    item->setFlags(Qt::NoItemFlags);
    item->setSizeHint(QSize(w, 9));
    item->setData(Qt::DecorationRole, "seperator");

    view->addItem(item);
    this->adjustSize();
}

void EsRoundMenu::_onItemClicked(QListWidgetItem* item)
{
    Q_D(EsRoundMenu);
    QVariant var = item->data(Qt::UserRole);
    EsAction* action = qobject_cast<EsAction*>(var.value<QObject*>());
    if (!action || !d->actions.contains(action) || !action->isEnabled())
        return;

    QWidget* widget = view->itemWidget(item);
    if (widget && !action->property("selectable").toBool())
        return;

    _hideMenu(false);

    if (!d->isSubMenu)
    {
        action->trigger();
        return;
    }

    // 关闭父菜单
    d->_closeParentMenu();
    action->trigger();
}

void EsRoundMenuPrivate::_closeParentMenu()
{
    Q_Q(EsRoundMenu);
    EsRoundMenu* menu = q;
    while (menu)
    {
        menu->close();
        menu = menu->parentMenu;
    }
}

void EsRoundMenuPrivate::_onItemEntered(QListWidgetItem* item)
{
    lastHoverItem = item;

    QVariant var = item->data(Qt::UserRole);
    EsRoundMenu* submenu = qobject_cast<EsRoundMenu*>(var.value<QObject*>());
    if (!submenu)
        return;

    _showSubMenu(item);
}

void EsRoundMenu::_hideMenu(bool isHideBySystem)
{
    Q_D(EsRoundMenu);
    d->isHideBySystem = isHideBySystem;
    view->clearSelection();

    if (d->isSubMenu)
        this->hide();
    else
        this->close();
}

void EsRoundMenu::paintEvent(QPaintEvent* paint_event)
{
    // 屏蔽QMenu原来的绘制
}

void EsRoundMenu::hideEvent(QHideEvent* e)
{
    Q_D(EsRoundMenu);
    if (d->isHideBySystem && d->isSubMenu)
    {
        d->_closeParentMenu();
    }

    d->isHideBySystem = true;
    e->accept();
}

void EsRoundMenu::closeEvent(QCloseEvent* e)
{
    e->accept();
    emit closedSignal();
    view->clearSelection();
}

QList<EsAction*> EsRoundMenu::menuActions() const
{
    Q_D(const EsRoundMenu);
    return d->actions;
}

void EsRoundMenu::mousePressEvent(QMouseEvent* e)
{
    QWidget* w = this->childAt(e->pos());
    if (w != view && !view->isAncestorOf(w))
    {
        _hideMenu(true);
    }
}

void EsRoundMenu::mouseMoveEvent(QMouseEvent* e)
{
    Q_D(EsRoundMenu);
    if (!d->isSubMenu)
        return;

    // 鼠标移出子菜单项时隐藏子菜单
    QPoint pos = e->globalPos();
    EsMenuActionListWidget* parentView = parentMenu->view;

    // 获取菜单项的全局矩形
    QMargins margin = parentView->viewportMargins();
    QRect rect = parentView->visualItemRect(menuItem).translated(parentView->mapToGlobal(QPoint()));
    rect.translate(margin.left(), margin.top() + 2);

    if (parentMenu->geometry().contains(pos) && !rect.contains(pos) && !geometry().contains(pos))
    {
        parentView->clearSelection();
        _hideMenu(false);
    }
}

void EsRoundMenuPrivate::_onActionChanged()
{
    Q_Q(EsRoundMenu);
    EsAction* action = qobject_cast<EsAction*>(sender());
    if (!action)
        return;

    QVariant itemVar = action->property("item");
    QListWidgetItem* item = itemVar.value<QListWidgetItem*>();
    if (!item)
        return;

    item->setIcon(_createItemIcon(action));

    if (action->text() != action->toolTip())
        item->setToolTip(action->toolTip());

    q->_adjustItemText(item, action);

    if (action->isEnabled())
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    else
        item->setFlags(Qt::NoItemFlags);

    q->view->adjustSize();
    q->adjustSize();
}

void EsRoundMenu::exec(const QPoint& pos, Es::MenuAnimationType aniType)
{
    Q_D(EsRoundMenu);
    // if (this->isVisible())
    //     aniType = MenuAnimationType::NONE;

    d->aniManager = EsMenuAnimationManager::make(QVariant::fromValue(this), aniType);
    d->aniManager->exec(pos);

    this->show();

    if (d->isSubMenu && menuItem)
        menuItem->setSelected(true);
}

void EsRoundMenu::exec_(const QPoint& pos, Es::MenuAnimationType aniType)
{
    exec(pos, aniType);
}

void EsRoundMenu::adjustPosition()
{
    QMargins m = layout()->contentsMargins();
    QRect screenRect = EsFunc::getCurrentScreenGeometry();
    int w = layout()->sizeHint().width() + 5;
    int h = layout()->sizeHint().height();

    int x = std::min(this->x() - m.left(), screenRect.right() - w);
    int y = this->y();
    if (y > screenRect.bottom() - h)
    {
        y = this->y() - h + m.bottom();
    }

    this->move(x, y);
}

// endregion


EsComboBoxMenu::EsComboBoxMenu(const QString& title, QWidget* parent)
    :EsRoundMenu(title, parent)
{
    view->setViewportMargins(0, 2, 0, 6);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    view->setItemDelegate(new EsIndicatorMenuItemDelegate());
    view->setObjectName("comboListWidget");

    setItemHeight(33);
}

void EsComboBoxMenu::exec(const QPoint& pos, Es::MenuAnimationType aniType)
{
    view->adjustSize(pos, aniType);
    adjustSize();
    return EsRoundMenu::exec(pos, aniType);
}
