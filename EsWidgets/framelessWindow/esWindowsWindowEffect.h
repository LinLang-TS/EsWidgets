/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWindowsWindowEffect.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWINDOWSWINDOWEFFECT_H
#define ESWINDOWSWINDOWEFFECT_H


#include <Windows.h>
#include "esSingleton.h"
#include "esDef.h"

class WindowsWindowEffectPrivate;

class ES_EXPORT EsWindowsWindowEffect
{
    E_SINGLETON_CREATE(EsWindowsWindowEffect)

private:
    explicit EsWindowsWindowEffect();
    ~EsWindowsWindowEffect();
    WindowsWindowEffectPrivate* impl;


public:
    /**
     * 给窗口添加亚克力效果 (仅在 Win10及以上 可用) \n
     * 半透明的毛(磨砂)玻璃效果
     * @param hwnd 窗口句柄
     * @param gradientColor 十六进制亚克力混合颜色，对应四个 RGBA 通道
     * @param enableShadow 是否启用窗口阴影
     * @param animationId 开启动画效果的 ID
     */
    void setAcrylicEffect(HWND hwnd, const QString& gradientColor = "F2F2F299", bool enableShadow = true, DWORD animationId = 0);

    /**
     * 设置窗口的边框颜色(仅在 Windows 11 及以上可用)
     * @param hWnd 窗口句柄。
     * @param color 边框的颜色。
     */
    void setBorderAccentColor(HWND hWnd, QColor color);

    /**
     * 移除窗口的边框颜色
     * @param hWnd 窗口句柄
     */
    void removeBorderAccentColor(HWND hWnd);


    /**
     *给窗口添加云母效果（仅适用于Win11）
     * @param hwnd 窗口句柄
     * @param isDarkMode 是否使用深色模式
     * @param isAlt 是否启用替代的仿玻璃效果
     */
    void setMicaEffect(HWND hwnd, bool isDarkMode = false, bool isAlt = false);

    /**
     * 为窗口添加Aero效果
     * @param hwnd 窗口句柄
     */
    void setAeroEffect(HWND hwnd);


    /**
     * 移除背景效果
     * @param hwnd 窗口句柄
     */
    void removeBackgroundEffect(HWND hwnd);


    /**
     * 为窗口添加DWM阴影效果
     * @param hwnd 窗口句柄
     */
    void addShadowEffect(HWND hwnd);


    /**
     * 为菜单添加DWM阴影效果
     * @param hwnd 窗口句柄
     */
    void addMenuShadowEffect(HWND hwnd);


    /**
     * 从窗口中移除DWM阴影效果
     * @param hwnd 窗口句柄
     */
    void removeShadowEffect(HWND hwnd);


    /**
     * 从弹出式菜单中移除阴影效果
     * @param hwnd 窗口句柄
     */
    static void removeMenuShadowEffect(HWND hwnd);


    /**
     * 启用窗口的最大化和最小化动画; (自定义标题栏的时候用, 调用它会去掉系统默认的标题栏)
     * @param hwnd 窗口句柄
     */
    static void addWindowAnimation(HWND hwnd);


    /**
     * 禁用窗口的最大化按钮
     * @param hwnd 窗口句柄
     */
    static void disableMaximizeButton(HWND hwnd);


    /**
     * 启用窗口后面的模糊效果
     * @param hwnd 窗口句柄
     */
    void enableBlurBehindWindow(HWND hwnd);
};


#endif //ESWINDOWSWINDOWEFFECT_H
