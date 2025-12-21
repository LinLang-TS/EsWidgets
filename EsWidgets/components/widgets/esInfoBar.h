/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esInfoBar.h
** @date 2025/11/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESINFOBAR_H
#define ESINFOBAR_H

#include "esDef.h"
#include <QWidget>
#include <QMap>
#include <QFrame>

#include "esIcon.h"


class EsIconBase;
class EsInfoBarIcon;
class EsInfoBarPrivate;

class ES_EXPORT EsInfoIconWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EsInfoIconWidget(const QVariant& icon, QWidget* parent = nullptr);
    ~EsInfoIconWidget() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVariant icon;
};

class ES_EXPORT EsInfoBar : public QFrame
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsInfoBar)
Q_SIGNALS:
    void closedSignal();

public:
    /**
      * @brief 构造一个信息提示栏（Info Bar）。
      *
      * @param icon 图标，支持以下类型：
      *             - InfoBarIcon 枚举值（内置图标），
      *             - EsIconBase*（自定义图标），
      *             - QIcon，
      *             - QString（图标文件路径或 Qt 资源路径）。
      * @param title 信息栏的标题文本。
      * @param content 信息栏的主要内容文本。
      * @param orient 布局方向：Qt::Horizontal（默认）适用于简短内容，Qt::Vertical 适用于较长内容。
      * @param isClosable 是否显示关闭按钮（默认为 true）。
      * @param duration 显示持续时间（毫秒）。若小于等于 0，则信息栏不会自动消失。
      * @param position 信息栏在屏幕上的显示位置（默认为右上角）。
      * @param parent 父级窗口部件（默认为 nullptr）。
      */
    explicit EsInfoBar(
        QWidget* parent,
        const QVariant& icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient,
        bool isClosable,
        int duration,
        Es::InfoBarPosition position
    );

    explicit EsInfoBar(
        Es::IconName icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );

    explicit EsInfoBar(
        const QString& icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );
    explicit EsInfoBar(
        EsIconBase* icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );
    explicit EsInfoBar(
        const QIcon& icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );


    ~EsInfoBar() override;

    /**
     * @brief 向信息条中添加一个控件
     *
     * Tip:
     *    - 必须是自己实例化 EsInfoBar 时, 调用此函数才不会显示异常;
     *    - 如果用内置的 EsInfoBar::info() 返回的对象指针调用此函数, 然后位置信息指定为 底部(包括左底和右底)时, 信息条底部边距会贴着父窗口底部;
     *    - 因为 newShow() 函数执行show()后, 就开始播放动画了, 此时添加就是动态添加组件到 EsInfoBar 里, 会导致的动画尺寸计算错误
     * @param widget 要添加的控件
     * @param stretch 控件的伸展因子，默认为 0
     */
    void addWidget(QWidget* widget, int stretch = 0);
    /**
     * @brief 设置自定义背景颜色
     *
     * 此函数允许设置控件在不同主题模式下的自定义背景颜色。在浅色模式下使用 `light` 颜色，在深色模式下使用 `dark` 颜色。
     * 设置后，控件的背景颜色将根据当前的主题自动更新。
     *
     * @param light 浅色模式下的背景颜色，可以是颜色名称; todo（字符串）、`Qt.GlobalColor` 或 `QColor` 对象
     * @param dark 深色模式下的背景颜色，可以是颜色名称（字符串）、`Qt.GlobalColor` 或 `QColor` 对象
     */
    void setCustomBackgroundColor(const QColor& light, const QColor& dark);


    bool eventFilter(QObject* obj, QEvent* e) override;


    static EsInfoBar* newShow(
        const QVariant& icon,
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr

    );

    static EsInfoBar* info(
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );
    static EsInfoBar* success(
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );

    static EsInfoBar* warning(
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );

    static EsInfoBar* error(
        const QString& title,
        const QString& content,
        Qt::Orientation orient = Qt::Horizontal,
        bool isClosable = true,
        int duration = 2000,
        Es::InfoBarPosition position = Es::InfoBarPosition::TOP_RIGHT,
        QWidget* parent = nullptr
    );

protected:
    void closeEvent(QCloseEvent* e) override;

    void showEvent(QShowEvent* e) override;

    void paintEvent(QPaintEvent* e) override;
};


#endif //ESINFOBAR_H
