/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esFramelessWindow.h
** @date 2025/10/3
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESFRAMELESSWINDOW_H
#define ES_ESFRAMELESSWINDOW_H

#include "esDef.h"
#include "esTitleBar.h"
#include "esWindowsWindowEffect.h"
#include <QWidget>
class EsFramelessWindowPrivate;
class EsAcrylicWindowPrivate;

class ES_EXPORT EsFramelessWindow : public QWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsFramelessWindow)

public:
    explicit EsFramelessWindow(QWidget* parent = nullptr);
    ~EsFramelessWindow() override;

    EsWindowsWindowEffect* windowEffect;
    EsTitleBar* titleBar;

    virtual void updateFrameless();
    // 设置自定义标题栏
    void setTitleBar(EsTitleBar* titleBar);

    // 设置是否启用鼠标在边框调整大小
    void setResizeEnabled(bool isEnabled);

    // 设置窗口置顶
    void setStayOnTop(bool isTop);

    // 切换窗口置顶状态
    void toggleStayOnTop();

    // 系统标题栏按钮是否可见
    bool isSystemButtonVisible() const;

    // 设置系统标题栏按钮的可见性，仅适用于MacOS
    void setSystemTitleBarButtonVisible(bool isVisible);

    /**
     * 返回系统标题栏rect，仅适用于macOS
     * @param size 原始系统标题栏rect
     * @return
     */
    QRect systemTitleBarRect(QSize size);

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

class ES_EXPORT EsAcrylicWindow : public EsFramelessWindow
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsAcrylicWindow)

public:
    explicit EsAcrylicWindow(QWidget* parent = nullptr);
    ~EsAcrylicWindow() override;

    void updateFrameless() override;

protected:
    void closeEvent(QCloseEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
};

#endif //ES_ESFRAMELESSWINDOW_H
