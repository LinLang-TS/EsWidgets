/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esRoundMenu_p.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESESROUNDMENU_P_H
#define ESESROUNDMENU_P_H
#include "esDef.h"

#include <QObject>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QHoverEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QMap>

class QParallelAnimationGroup;
class EsRoundMenu;
class EsAction;
class QListWidgetItem;

/**
 * @class EsMenuAnimationManager
 * @brief 基础菜单动画管理器类，负责管理菜单的动画效果。
 *
 * 该类提供了基础的动画逻辑，并允许不同的菜单类型（如 `EsRoundMenu` 和 `EsMenuActionListWidget`）使用特定的动画效果。
 * 所有继承此类的管理器都需要实现特定的动画执行逻辑。
 */
class ES_EXPORT EsMenuAnimationManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     *
     * @param menu 传入的菜单对象，类型为 QVariant，用于动态存储不同类型的菜单实例。
     * @param parent 父对象（默认值为 nullptr）。
     */
    explicit EsMenuAnimationManager(const QVariant& menu, QObject* parent = nullptr);

    /**
     * @brief 析构函数
     *
     * 在析构时会清理动画对象。
     */
    ~EsMenuAnimationManager() override;

    /**
     * @brief 执行菜单动画
     *
     * 该函数用于执行与菜单相关的动画效果。子类需要实现具体的动画逻辑。
     *
     * @param pos 菜单弹出的位置。
     */
    virtual void exec(const QPoint& pos);

    /**
     * @brief 获取可用的视图大小
     *
     * 该函数返回菜单弹出位置时，菜单可用的视图大小。具体的视图大小计算由子类实现。
     *
     * @param pos 菜单弹出的位置。
     * @return 返回菜单可用的视图大小。
     */
    virtual QSize availableViewSize(const QPoint& pos);

    /**
     * @brief 注册菜单动画管理器
     *
     * 该函数用于注册具体的菜单动画管理器。每个菜单类型都应注册一个动画管理器。
     *
     * @param name 动画类型的标识符。
     * @param manager 动画管理器实例。
     */
    static void registerManager(Es::MenuAnimationType name, EsMenuAnimationManager* manager);

    /**
     * @brief 创建并返回一个菜单动画管理器实例; warning: 此函数返回的指针需要手动管理内存
     *
     * 根据传入的菜单类型和动画类型，创建并返回一个适合该菜单的动画管理器实例。
     *
     * @param menu 菜单实例，可以是 `EsRoundMenu` 或 `EsMenuActionListWidget`。
     * @param aniType 动画类型。
     * @return 返回相应类型的菜单动画管理器实例指针(需手动管理内存)
     */
    static EsMenuAnimationManager* make(const QVariant& menu, Es::MenuAnimationType aniType);

protected:
    /**
     * @brief 创建菜单动画管理器的纯虚函数
     *
     * 该函数由子类实现，用于创建具体的菜单动画管理器实例。
     *
     * @param menu 菜单实例。
     * @return 返回创建的动画管理器实例。
     */
    virtual EsMenuAnimationManager* create(const QVariant& menu) = 0;

    /**
     * @brief 计算并返回菜单弹出位置
     *
     * 根据菜单弹出位置和屏幕边界，计算菜单的结束位置。
     *
     * @param pos 菜单的初始位置。
     * @return 返回计算后的结束位置。
     */
    virtual QPoint endPosition(const QPoint& pos);

    /**
     * @brief 计算菜单的尺寸
     *
     * 该函数用于获取菜单的尺寸，具体的计算逻辑由子类实现。
     *
     * @return 返回菜单的宽度和高度。
     */
    QSize menuSize();

    /**
     * @brief 动画值变化时的槽函数
     *
     * 该函数会在动画的值变化时被调用，子类可以重载此函数以实现特定的行为。
     */
    Q_SLOT virtual void onValueChanged();

    /**
     * @brief 更新菜单视图
     *
     * 当菜单动画发生时，视图会被更新。此函数会在动画过程中被调用。
     */
    Q_SLOT void updateMenuViewport();

protected:
    QVariant menu;  ///< 存储菜单实例的 QVariant，支持多种菜单类型。
    QPropertyAnimation* ani;  ///< 动画对象，用于管理菜单动画效果。

public:
    /**
     * @brief 动画管理器映射表
     *
     * 该映射表保存了每种动画类型对应的菜单动画管理器实例。通过动画类型可以找到相应的管理器。
     */
    static QMap<Es::MenuAnimationType, EsMenuAnimationManager*> managers;
};


/**
 * 无动画的菜单
 */
class ES_EXPORT EsDummyMenuAnimationManager : public EsMenuAnimationManager
{
    Q_OBJECT

public:
    explicit EsDummyMenuAnimationManager(const QVariant& menu);

    void exec(const QPoint& pos) override;

protected:
    EsMenuAnimationManager* create(const QVariant& menu) override;
};

/**
 * @class EsPullUpMenuAnimationManager
 * @brief 下拉式菜单动画管理器，实现菜单从底部向下弹出的显示动画。
 *
 * 该类继承自 EsMenuAnimationManager，负责管理 EsRoundMenu 的下拉动画效果。
 * 主要负责动画创建、执行位置计算、可视区域计算以及动画过程中动态 Mask 更新。
 */
class ES_EXPORT EsDropDownMenuAnimationManager : public EsMenuAnimationManager
{
    Q_OBJECT

public:
    explicit EsDropDownMenuAnimationManager(const QVariant& menu);

    void exec(const QPoint& pos) override;
protected:

    QSize availableViewSize(const QPoint& pos) override;

    Q_SLOT void onValueChanged() override;

    EsMenuAnimationManager* create(const QVariant& menu) override;
};


/**
 * @class EsPullUpMenuAnimationManager
 * @brief 上拉式菜单动画管理器，实现菜单从底部向上弹出的显示动画。
 *
 * 该类继承自 EsMenuAnimationManager，负责管理 EsRoundMenu 的上拉动画效果。
 * 主要负责动画创建、执行位置计算、可视区域计算以及动画过程中动态 Mask 更新。
 */
class ES_EXPORT EsPullUpMenuAnimationManager : public EsMenuAnimationManager
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param menu QVariant 中存储的菜单对象（通常为 EsRoundMenu*）
     */
    explicit EsPullUpMenuAnimationManager(const QVariant& menu);

    /**
     * @brief 执行动画
     * @param pos 菜单最终出现的位置（菜单左上角坐标）
     *
     * 此函数将设置动画起始与结束坐标，并启动从下向上的弹出动画。
     */
    void exec(const QPoint& pos) override;
protected:
    /**
     * @brief 创建当前类型的动画管理器实例
     * @param menu 传入的菜单对象
     * @return 新的 EsPullUpMenuAnimationManager 实例
     */
    EsMenuAnimationManager* create(const QVariant& menu) override;

    QPoint endPosition(const QPoint& pos) override;

    /**
     * @brief 计算当前菜单可用显示区域大小
     * @param pos 菜单基准位置
     * @return 可用区域尺寸
     *
     * 返回基于当前屏幕位置 pos 能够提供的最大显示区域，主要用于限制菜单高度。
     */
    QSize availableViewSize(const QPoint& pos) override;

    /**
     * @brief 动画值变化回调
     *
     * 在动画过程中更新菜单的 Mask，实现菜单从下向上逐步揭开的视觉效果。
     */
    void onValueChanged() override;
};


/**
 * @class EsFadeInDropDownMenuAnimationManager
 * @brief 下拉渐隐式菜单动画管理器，提供同时包含位移动画与透明度渐显的复合效果。
 *
 * 该类继承自 EsMenuAnimationManager，通过 QParallelAnimationGroup 实现菜单
 * 从上向下出现，并伴随透明度由 0 增至 1 的渐入动画。适用于下拉菜单类组件。
 */
class ES_EXPORT EsFadeInDropDownMenuAnimationManager : public EsMenuAnimationManager
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param menu QVariant 中保存的菜单对象（通常为 EsRoundMenu*）
     *
     * 初始化位移动画 ani（来自基类）与透明度动画 opacityAni，
     * 并将两者加入并行动画组 aniGroup。
     */
    explicit EsFadeInDropDownMenuAnimationManager(const QVariant& menu);

protected:
    /**
     * @brief 工厂方法，用于基类 clone 时创建同类型实例
     * @param menu 菜单对象
     * @return EsFadeInDropDownMenuAnimationManager 新实例
     */
    EsMenuAnimationManager* create(const QVariant& menu) override;

public:
    /**
     * @brief 执行动画
     * @param pos 菜单目标位置（左上角坐标）
     *
     * 位移动画向下偏移 8 像素进入最终位置，透明度从 0 到 1。
     * 两个动画并行执行。
     */
    void exec(const QPoint& pos) override;

    /**
     * @brief 获取基于 pos 可用的最大显示区域尺寸
     * @param pos 菜单基准坐标
     * @return 可视区域大小
     *
     * 限定下拉菜单允许的最大高度（与屏幕底部保持 10px 间距）。
     */
    QSize availableViewSize(const QPoint& pos) override;

private:
    QPropertyAnimation* opacityAni;              ///< 控制透明度变化的动画
    QParallelAnimationGroup* aniGroup;           ///< 位移动画与透明度动画的并行动画组
};

/**
 * @class EsFadeInPullUpMenuAnimationManager
 * @brief 上拉渐显式菜单动画管理器，实现菜单从下方向上浮现并伴随透明度渐入的动画。
 *
 * 本类继承自 EsMenuAnimationManager，通过 QParallelAnimationGroup 将位置上移的
 * 动画与透明度从 0 到 1 的渐显动画组合，实现更平滑的上拉展示效果。
 */
class ES_EXPORT EsFadeInPullUpMenuAnimationManager : public EsMenuAnimationManager
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param menu 通过 QVariant 传入的菜单实例（一般为 EsRoundMenu*）
     *
     * 初始化透明度动画 opacityAni 以及并行动画组 aniGroup，
     * 并将透明度动画与基类中的位移动画 ani 同步加入并行动画组中。
     */
    explicit EsFadeInPullUpMenuAnimationManager(const QVariant& menu);

protected:
    /**
     * @brief 工厂创建方法，用于基类克隆动画管理器实例
     * @param menu 菜单对象 QVariant
     * @return 新的 EsFadeInPullUpMenuAnimationManager 实例
     */
    EsMenuAnimationManager* create(const QVariant& menu) override;

    QPoint endPosition(const QPoint& pos) override;
public:
    /**
     * @brief 执行动画
     * @param pos 菜单最终位置（左上角坐标）
     *
     * 动画从最终位置向下偏移 8 像素开始上移，并伴随透明度从 0 增加到 1 的渐显。
     * 位移动画持续 200ms，透明度动画持续 150ms。
     */
    void exec(const QPoint& pos) override;

    /**
     * @brief 计算菜单在上拉方向可用的最大可视显示区域
     * @param pos 菜单基准位置
     * @return 可用显示区域尺寸
     *
     * 主要限制菜单向上展开的最大高度，基于屏幕顶部到 pos 的距离计算。
     */
    QSize availableViewSize(const QPoint& pos) override;

private:
    QPropertyAnimation* opacityAni;              ///< 透明度渐显动画
    QParallelAnimationGroup* aniGroup;           ///< 位移动画与透明度动画的并行动画组
};


/**
 * @class EsSubMenuItemWidget
 * @brief 子菜单条目控件
 *
 * 该控件用于在圆角菜单（RoundMenu）中显示子菜单的入口。
 * 当鼠标进入该控件时，会发射 showMenuSig 信号，通知父菜单显示子菜单。
 * 控件右侧会绘制一个箭头图标（chevron）。
 */
class ES_EXPORT EsSubMenuItemWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     *
     * @param menu 子菜单对象（EsRoundMenu 或 EsRoundMenu）
     * @param item 对应的 QListWidgetItem
     * @param parent 父控件
     */
    explicit EsSubMenuItemWidget(EsRoundMenu* menu,
                                 QListWidgetItem* item,
                                 QWidget* parent = nullptr);

signals:
    /**
     * @brief 当鼠标进入该控件时发射信号，用于显示子菜单
     *
     * @param item 当前条目对应的 QListWidgetItem
     */
    void showMenuSig(QListWidgetItem* item);

protected:
    /**
     * @brief 鼠标进入事件
     *
     * 鼠标进入控件时，发射 showMenuSig 信号让父菜单显示子菜单。
     *
     * @param e QEvent 事件对象
     */
    void enterEvent(QEvent* e) override;

    /**
     * @brief 绘制控件
     *
     * 在控件右侧绘制一个 9x9 的 Chevron 图标。
     *
     * @param e QPaintEvent 事件对象
     */
    void paintEvent(QPaintEvent* e) override;

public:
    EsRoundMenu* menu; ///< 子菜单对象
    QListWidgetItem* item; ///< 对应的菜单项
};


class EsRoundMenu;

class EsRoundMenuPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsRoundMenu);

public:
    explicit EsRoundMenuPrivate(QObject* parent = nullptr);
    /**
     * @brief 初始化控件与布局
     */
    void initWidgets();

    /**
     * @brief 创建子菜单对应的列表项和控件
     * @param menu 要创建的子菜单
     * @return QPair<QListWidgetItem*, EsSubMenuItemWidget*> 列表项和控件
     */
    QPair<QListWidgetItem*, EsSubMenuItemWidget*> _createSubMenuItem(EsRoundMenu* menu);

    /**
     * @brief 检查菜单是否有图标
     * @return 如果有任意动作或子菜单有图标则返回 true
     */
    bool _hasItemIcon() const;

    /**
     * @brief 获取最长快捷键文本的宽度
     * @return 最长宽度（向上取整）
     */
    int _longestShortcutWidth() const;

    /**
     * @brief 创建菜单项的图标
     * @param w EsAction 或 EsRoundMenu
     * @return QIcon 对象
     */
    QIcon _createItemIcon(EsRoundMenu* w) const;
    QIcon _createItemIcon(EsAction* w) const;

    /**
     * @brief 从列表中移除指定项并删除其控件
     * @param item 要移除的 QListWidgetItem
     */
    void _removeItem(QListWidgetItem* item);

    /**
     * @brief 显示子菜单（带防抖延迟）
     * @param item 当前悬停的子菜单项
     */
    void _showSubMenu(QListWidgetItem* item);

    /**
     * @brief 延迟显示子菜单（菜单防抖定时器触发）
     */
    void _onShowMenuTimeOut();

    /**
     * @brief 关闭当前菜单及其所有父菜单
     */
    void _closeParentMenu();

    /**
     * @brief 处理鼠标进入菜单项事件
     * @param item 当前悬停的 QListWidgetItem
     */
    void _onItemEntered(QListWidgetItem* item);

    /**
     * @brief 动作状态变化槽函数
     */
    void _onActionChanged();

    QVariant icon;
    QString title;
    QList<EsAction*> actions;
    QList<EsRoundMenu*> subMenus;
    bool isSubMenu; ///< 是否为子菜单
    QListWidgetItem* lastHoverItem; ///< 上次悬停的项
    QListWidgetItem* lastHoverSubMenuItem; ///< 上次悬停的子菜单项
    bool isHideBySystem;
    QHBoxLayout* hBoxLayout;
    QTimer* timer;
    EsMenuAnimationManager* aniManager;
};


#endif //ESESROUNDMENU_P_H
