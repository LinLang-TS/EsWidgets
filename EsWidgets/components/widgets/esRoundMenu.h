/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esRoundMenu.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESROUNDMENU_H
#define ESROUNDMENU_H

#include <QMenu>
#include <QListWidgetItem>

#include "esDef.h"
#include "esGlobal.h"
class EsAction;
class QHBoxLayout;
class EsMenuActionListWidget;
class EsRoundMenuPrivate;
Q_DECLARE_METATYPE(QListWidgetItem*)

/**
 * 由于 EsAction 继承了QAction隐藏了它的icon()方法(), 而如果这个类传入或返回QAction在调用icon方法就会执行原Qt的icon方法,
 * 导致出问题, 所以 这里的方法只接收和返回 EsAction
 */
class ES_EXPORT EsRoundMenu : public QMenu
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsRoundMenu)
Q_SIGNALS:
    /**
     * @brief 菜单关闭信号
     */
    void closedSignal();

public:
    /**
     * @brief 构造函数
     * @param title 菜单标题
     * @param parent 父对象
     */
    explicit EsRoundMenu(const QString& title = "", QWidget* parent = nullptr);

    ~EsRoundMenu() override;

    /**
     * @brief 设置最大可见项数
     * @param num 最大可见项数
     */
    void setMaxVisibleItems(int num);
    /**
     * @brief 设置菜单项高度
     * @param height 高度值
     */
    void setItemHeight(int height);
    /**
     * @brief 为菜单添加阴影效果
     * @param blurRadius 模糊半径，默认 30
     * @param offset 阴影偏移，默认 (0, 8)
     * @param color 阴影颜色，默认 QColor(0, 0, 0, 30)
     */
    void setShadowEffect(int blurRadius = 30, const QPoint& offset = QPoint(0, 8),
                         const QColor& color = QColor(0, 0, 0, 30));
    /**
     * @brief 设置父菜单和对应的菜单项
     * @param parent 父菜单
     * @param item 对应的菜单项
     */
    void _setParentMenu(EsRoundMenu* parent, QListWidgetItem* item);

    /**
     * @brief 调整 RoundMenu 的大小以适应内容
     */
    void adjustSize();

    /**
     * @brief 获取菜单图标
     * @return 菜单图标
     */
    QIcon icon() const;

    /**
     * @brief 获取菜单标题
     * @return 菜单标题
     */
    QString title() const;

    /**
     * @brief 清除所有动作和子菜单
     */
    void clear();

    /**
     * @brief 设置菜单图标
     * @param icon 菜单图标，可以是 QIcon 或自定义 EsIconBase
     */
    void setIcon(const QVariant& icon);

    /**
     * @brief 设置菜单标题
     * @param title 菜单标题
     */
    void setTitle(const QString& title);

    /**
     * @brief 向菜单添加动作
     * @param action 菜单动作，可以是 EsAction 或自定义 Action
     */
    void addAction(EsAction* action);

    /**
     * @brief 向菜单添加自定义控件
     * @param widget 自定义控件
     * @param selectable 是否可选
     * @param onClick 点击回调槽函数，可为 nullptr
     */
    void addWidget(QWidget* widget, bool selectable = true, const std::function<void()>& onClick = nullptr);


    /**
     * @brief 在指定动作前插入新动作
     * @param before 插入位置前的动作
     * @param action 要插入的动作
     */

    void insertAction(EsAction* before, EsAction* action);
    /**
     * @brief 批量添加动作到菜单
     * @param actions 动作列表
     */

    void addActions(const QList<EsAction*>& actions);

    /**
     * @brief 在指定动作前批量插入动作
     * @param before 插入位置前的动作
     * @param actions 要插入的动作列表
     */
    void insertActions(EsAction* before, const QList<EsAction*>& actions);

    /**
     * @brief 从菜单中移除指定动作
     * @param action 要移除的动作
     */
    void removeAction(EsAction* action);

    /**
     * @brief 移除子菜单
     * @param menu 要移除的子菜单
     */
    void removeMenu(EsRoundMenu* menu);

    /**
     * @brief 设置默认选中动作
     * @param action 要设置为默认的动作
     */
    void setDefaultAction(EsAction* action);

    /**
     * 取所有的 action;
     * @return
     */
    QList<EsAction*> actions() const;

    /**
     * @brief 添加子菜单
     * @param menu 要添加的子菜单
     * @throws std::invalid_argument 如果 menu 不是 RoundMenu 实例
     */
    void addMenu(EsRoundMenu* menu);

    /**
     * @brief 在指定动作前插入子菜单
     * @param before 插入位置前的动作
     * @param menu 要插入的子菜单
     * @throws std::invalid_argument 如果 menu 不是 RoundMenu 或 before 不在动作列表中
     */
    void insertMenu(EsAction* before, EsRoundMenu* menu);


    /**
     * @brief 向菜单添加分隔符
     */
    void addSeparator();


    /**
     * @brief 菜单隐藏事件处理
     * @param e QHideEvent 指针
     */
    void hideEvent(QHideEvent* e) override;

    /**
     * @brief 菜单关闭事件处理
     * @param e QCloseEvent 指针
     */
    void closeEvent(QCloseEvent* e) override;

    /**
     * @brief 获取菜单中的动作列表
     * @return 菜单动作列表
     */
    QList<EsAction*> menuActions() const;

    /**
     * @brief 鼠标按下事件处理
     * @param e QMouseEvent 指针
     */
    void mousePressEvent(QMouseEvent* e) override;

    /**
     * @brief 鼠标移动事件处理（用于子菜单防抖隐藏）
     * @param e QMouseEvent 指针
     */
    void mouseMoveEvent(QMouseEvent* e) override;


    /**
     * @brief 显示菜单
     * @param pos 弹出位置
     * @param ani 是否显示动画
     * @param aniType 菜单动画类型
     */
    virtual void exec(const QPoint& pos, Es::MenuAnimationType aniType = Es::MenuAnimationType::DROP_DOWN);

    /**
     * @brief 显示菜单（兼容 exec_ 命名）
     * @param pos 弹出位置
     * @param ani 是否显示动画
     * @param aniType 菜单动画类型
     */
    void exec_(const QPoint& pos, Es::MenuAnimationType aniType = Es::MenuAnimationType::DROP_DOWN);

    /**
     * @brief 调整菜单位置，使其不超出屏幕边界
     */
    void adjustPosition();

protected:
    /**
     * @brief 创建菜单动作对应的列表项
     * @param action 菜单动作
     * @param before 可选，插入到此动作前
     * @return 创建的 QListWidgetItem
     * @throws std::invalid_argument 如果 before 不在动作列表中
     */
    QListWidgetItem* _createActionItem(EsAction* action, EsAction* before = nullptr);

    /**
     * @brief 调整菜单项文字与宽度
     * @param item  QListWidgetItem*
     * @param action EsAction*
     * @return int 计算后的宽度
     */
    int _adjustItemText(QListWidgetItem* item, EsAction* action);
    /**
     * @brief 处理菜单项点击事件
     * @param item 被点击的 QListWidgetItem
     */
    Q_SLOT void _onItemClicked(QListWidgetItem* item);


    /**
     * @brief 隐藏菜单
     * @param isHideBySystem 是否由系统触发隐藏
     */
    void _hideMenu(bool isHideBySystem = false);

    void paintEvent(QPaintEvent*) override;

public:
    QListWidgetItem* menuItem;
    EsRoundMenu* parentMenu;

    EsMenuActionListWidget* view;
};

class ES_EXPORT EsComboBoxMenu: public EsRoundMenu
{
    Q_OBJECT
public:
    explicit EsComboBoxMenu(const QString& title = "", QWidget* parent = nullptr);
    void exec(const QPoint& pos, Es::MenuAnimationType aniType = Es::MenuAnimationType::DROP_DOWN);

};

#endif //ESROUNDMENU_H
