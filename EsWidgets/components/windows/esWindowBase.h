/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWindowBase.h
** @date 2025/11/24
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWINDOWBASE_H
#define ESWINDOWBASE_H

#include "esAnimation.h"
#include "widgets/esWidget.h"

class EsStackedWidget;
class EsWindowBasePrivate;

// todo 未完成, 只是初步测试窗口可以根据 EsConfig 正常切换主题色
class ES_EXPORT EsWindowBase : public EsBackgroundAnimationWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsWindowBase)

public:
    explicit EsWindowBase(QWidget* parent = nullptr);
    ~EsWindowBase();

public:
    /**
     * @brief 添加子界面
     *
     * 该方法用于向导航栏添加一个子界面。由于该方法需要在子类中实现，因此在基类中抛出异常。
     *
     * @param iface 子界面，QWidget 类型
     * @param icon 图标，可以是 EsIconBase、QIcon 或者字符串
     * @param text 子界面的文本
     * @param position 子界面在导航栏中的位置，默认为 NavigationItemPosition.TOP
     *
     * @throws NotImplementedError 如果没有在子类中实现该方法
     */
    // virtual void addSubInterface(QWidget* iface, const QVariant& icon, const QString& text,
                                      // Es::NavigationItemPosition position = Es::NavigationItemPosition::TOP) = 0;
    void addSubInterface(QWidget* iface);
    /**
     * @brief 移除子界面
     *
     * 该方法用于从导航栏中移除指定的子界面。
     *
     * @param interface 子界面，QWidget 类型
     * @param isDelete 是否删除子界面，默认值为 false
     */
    // virtual void removeInterface(QWidget* iface, bool isDelete = false) = 0;

    /**
     * @brief 切换到指定的子界面
     *
     * 该方法用于切换到指定的子界面。`popOut` 参数被设置为 `false`，表示在切换时不进行弹出动画效果。
     *
     * @param interface 要切换到的界面，类型为 `QWidget`。
     */
    void switchTo(QWidget* iface);

    /**
     * @brief 设置自定义背景颜色
     *
     * 该方法根据传入的颜色值设置在不同主题模式下的背景颜色。
     *
     * @param light 背景颜色在浅色主题下的颜色，todo 可以是 QColor、Qt.GlobalColor 或者字符串。
     * @param dark 背景颜色在深色主题下的颜色，可以是 QColor、Qt.GlobalColor 或者字符串。
     */
    void setCustomBackgroundColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置是否启用 Mica 特效（仅适用于 Windows 11）
     *
     * 该方法用于启用或禁用 Mica 特效，Mica 特效仅在 Windows 11（版本 22000 或更高）上可用。
     *
     * @param isEnabled 如果为 true，则启用 Mica 特效；如果为 false，则禁用 Mica 特效。
     */
    void setMicaEffectEnabled(bool isEnabled);

    /**
     * @brief 获取 Mica 特效是否启用
     *
     * 该方法返回当前窗口是否启用了 Mica 特效。
     *
     * @return 如果启用了 Mica 特效，则返回 true；否则返回 false。
     */
    bool isMicaEffectEnabled() const;

    /**
     * @brief 获取系统标题栏的矩形区域，仅适用于 macOS
     *
     * 该方法用于返回系统标题栏的矩形区域。此功能仅在 macOS 上有效，其他平台返回默认值。
     *
     * @param size 原始的系统标题栏大小
     * @return 返回标题栏的矩形区域
     */
    QRect systemTitleBarRect(const QSize& size) const;

    /**
     * @brief 设置窗口标题栏
     *
     * 该方法用于设置窗口的标题栏。在 macOS 上，如果系统按钮可见且标题栏是 `TitleBarBase` 类型，
     * 则隐藏标题栏中的最小化、最大化和关闭按钮。
     *
     * @param titleBar 要设置的标题栏
     */
    void setTitleBar(QWidget* titleBar);


protected:
    /**
     * @brief 主题更改完成后的处理函数
     *
     * 该方法在主题更改完成后被调用。它会根据当前的主题是否为深色模式，
     * 启用 Mica 特效（如果该特效已启用）。如果 Mica 特效启用，它会更新窗口的 Mica 特效效果。
     */
    Q_SLOT void onThemeChangedFinished();

    /**
     * @brief 当前界面变化时的槽函数
     *
     * 该方法在当前界面发生变化时被调用。它会更新导航栏中的当前选项，并通过 `qrouter` 跳转到对应的界面。
     * 同时，调用 `_updateStackedBackground()` 方法更新背景。
     *
     * @param index 当前显示的界面索引。
     */
    void onCurrentInterfaceChanged(int index);

    /**
     * @brief 更新堆叠背景样式
     *
     * 该方法用于更新当前堆叠窗口的背景透明度属性。如果当前界面与堆叠窗口的透明属性一致，则不做任何操作。
     * 否则，会设置堆叠窗口的透明度属性，并刷新样式。
     */
    void updateStackedBackground();

    /**
     * @brief 获取正常的背景颜色
     *
     * 该方法根据当前是否启用了 Mica 特效，以及当前系统的主题模式（深色或浅色），返回适当的背景颜色。
     *
     * @return QColor 返回当前主题模式下的背景颜色，如果启用了 Mica 特效，返回透明背景色。
     */
    QColor normalBackgroundColor();

    void paintEvent(QPaintEvent* event) override;

    void showEvent(QShowEvent* event) override;

public:
    EsStackedWidget* stackedWidget;
    QWidget* navigationInterface; // EsNavigationInterface
};


#endif //ESWINDOWBASE_H
