/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWindowsWindowEffect.cpp
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include <QDebug>
#include <dwmapi.h>
#include "esWin32Utils.h"
#include "esWindowsWindowEffect.h"

/**
 * 窗口合成属性
 */
typedef enum _WINDOWCOMPOSITIONATTRIB {
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_HOLOGRAPHIC = 23,
    WCA_EXCLUDED_FROM_DDA = 24,
    WCA_PASSIVEUPDATEMODE = 25,
    WCA_USEDARKMODECOLORS = 26,
    WCA_CORNER_STYLE = 27,
    WCA_PART_COLOR = 28,
    WCA_DISABLE_MOVESIZE_FEEDBACK = 29,
    WCA_LAST = 30,
} WINDOWCOMPOSITIONATTRIB;


/**
 * ACCENT_STATE类型: 窗口边框和背景的视觉效果 (窗口特效的状态)
 */
typedef enum _ACCENT_STATE {
    ACCENT_DISABLED = 0,                    // 表示窗口特效被禁用。
    ACCENT_ENABLE_GRADIENT = 1,             // 表示启用渐变特效。
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,  // 表示启用透明度渐变特效
    ACCENT_ENABLE_BLURBEHIND = 3,           // 表示启用玻璃透明特效（Aero effect），即在窗口后面添加模糊的玻璃效果。
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,    // 表示启用亚克力模糊特效（Acrylic effect），这是 Windows 10 中引入的一种新的模糊效果。
    ACCENT_ENABLE_HOSTBACKDROP = 5,         // 表示启用 Mica (Mica effect)特效
    ACCENT_INVALID_STATE = 6,               // 表示无效的状态，可能用于表示错误状态或占位符。
} ACCENT_STATE;


/**
 * ACCENT_POLICY类型: 描述窗口的边框和背景的视觉效果
 */
typedef struct _ACCENT_POLICY {
    ACCENT_STATE AccentState;   // 边框和背景的视觉状态
    DWORD AccentFlags;          // 控制视觉效果的标志位
    DWORD GradientColor;        // 渐变颜色（ARGB格式）
    DWORD AnimationId;          // 动画效果的ID
} ACCENT_POLICY;


/**
 * WINDOWCOMPOSITIONATTRIBDATA类型: 用于设置或获取窗口合成属性
 */
typedef struct  {
    WINDOWCOMPOSITIONATTRIB Attribute; // 指定要设置或获取的属性
    PVOID pvData; // 指向保存或接收属性值的数据缓冲区
    SIZE_T SizeOfData; // 数据缓冲区的大小（以字节为单位)

} WINDOWCOMPOSITIONATTRIBDATA;



// 声明win32API
using pfnSetWindowCompositionAttribute = BOOL(WINAPI*)(HWND hwnd, const WINDOWCOMPOSITIONATTRIBDATA*);
pfnSetWindowCompositionAttribute SetWindowCompositionAttribute = nullptr;

class WindowsWindowEffectPrivate
{
public:
    ACCENT_POLICY accentPolicy{};
    WINDOWCOMPOSITIONATTRIBDATA winCompAttrData{};
    WindowsWindowEffectPrivate()
    {
        winCompAttrData.Attribute = WCA_ACCENT_POLICY;
        winCompAttrData.SizeOfData = sizeof accentPolicy;
        winCompAttrData.pvData = &accentPolicy;

        if (SetWindowCompositionAttribute == nullptr)
        {
            auto hUser32 = GetModuleHandle("user32.dll");
            SetWindowCompositionAttribute = (pfnSetWindowCompositionAttribute) GetProcAddress(hUser32, "SetWindowCompositionAttribute");
        }

    }


};


EsWindowsWindowEffect::EsWindowsWindowEffect()
{
    impl = new WindowsWindowEffectPrivate;
}

EsWindowsWindowEffect::~EsWindowsWindowEffect()
{
    delete impl;
}

void EsWindowsWindowEffect::setAcrylicEffect(HWND hwnd, const QString& gradientColor, bool enableShadow, DWORD animationId) {
    if (!isGreaterEqualWin10()){
        qDebug() << "亚克力(Acrylic)效果仅在 Win10及以上 可用";
        return;
    }

    //   作用: F2F2F299 转换成 99F2F2F2
    QString formattedColor;
    for (int i = 6; i >= 0; i -= 2) {
        formattedColor += gradientColor.mid(i, 2);
    }
    DWORD dwGradientColor = formattedColor.toUInt(nullptr, 16);
    DWORD accentFlags = enableShadow ? DWORD(0x20 | 0x40 | 0x80 | 0x100) : DWORD(0);

    impl->accentPolicy.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
    impl->accentPolicy.GradientColor = dwGradientColor;
    impl->accentPolicy.AccentFlags = accentFlags;
    impl->accentPolicy.AnimationId = animationId;
    impl->winCompAttrData.Attribute = WCA_ACCENT_POLICY;
    SetWindowCompositionAttribute(hwnd, &impl->winCompAttrData);


}


void EsWindowsWindowEffect::setBorderAccentColor(HWND hWnd, QColor color)
{

    if (!isGreaterEqualWin11()) return;
    // 将 QColor 转换为 DWORD 格式（0xRRGGBB）
    DWORD colorref = DWORD(color.red() | (color.green() << 8) | (color.blue() << 16));

    // 设置窗口的边框颜色
    DwmSetWindowAttribute(hWnd,
                          DWMWA_BORDER_COLOR,
                          &colorref,
                          sizeof(colorref));
}


void EsWindowsWindowEffect::removeBorderAccentColor(HWND hWnd)
{
    if (!isGreaterEqualWin11()) return;

    // 设置边框颜色为默认值（0xFFFFFFFF，即透明）
    DWORD colorref = 0xFFFFFFFF;
    // 移除边框颜色
    DwmSetWindowAttribute(hWnd,
                          DWMWA_BORDER_COLOR,
                          &colorref,
                          sizeof(colorref));
}

void EsWindowsWindowEffect::setMicaEffect(HWND hwnd, bool isDarkMode, bool isAlt) {
    if (!isGreaterEqualWin11()){
        qDebug() << "云母(Mica)效果仅在Windows 11上可用。";
        return;
    }
    // 扩展窗口客户区域，以便在窗口周围创建一个玻璃边框
    MARGINS margins = {16777215, 16777215, 0, 0};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    // 设置窗口组合属性为WCA_ACCENT_POLICY，从而启用透明效果
    impl->winCompAttrData.Attribute = WCA_ACCENT_POLICY;
    impl->accentPolicy.AccentState = ACCENT_ENABLE_HOSTBACKDROP;
    SetWindowCompositionAttribute(hwnd, &impl->winCompAttrData);

    // 是否使用深色模式
    if (isDarkMode) {
        impl->winCompAttrData.Attribute = WCA_USEDARKMODECOLORS;
        SetWindowCompositionAttribute(hwnd, &impl->winCompAttrData);
    }

    // 设置DWM窗口属性
    int v = getWindowsVersion();
    if (v < 22523) {
        // 在早期版本的Windows 11中使用1029属性
        int attribute = 1;
        DwmSetWindowAttribute(hwnd, 1029, &attribute, sizeof attribute);
    } else {
        // 在Windows 11 Build 22523及更高版本中使用38属性启用/禁用玻璃效果
        int attribute = isAlt ? 4 : 2;
        DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &attribute, sizeof attribute);
    }
    // 启用或禁用深色模式的颜色
    int attribute = 1 * isDarkMode;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &attribute, sizeof attribute);

}


void EsWindowsWindowEffect::setAeroEffect(HWND hwnd) {
    impl->winCompAttrData.Attribute = WCA_ACCENT_POLICY;  // 设置属性为窗口合成属性-玻璃效果策略
    impl->accentPolicy.AccentState = ACCENT_ENABLE_BLURBEHIND;  // 设置玻璃效果状态为启用模糊背景
    SetWindowCompositionAttribute(hwnd, &impl->winCompAttrData);  // 调用设置窗口合成属性的函数，应用玻璃效果策略

}


void EsWindowsWindowEffect::removeBackgroundEffect(HWND hwnd) {
    impl->accentPolicy.AccentState = ACCENT_DISABLED; // 设置玻璃效果状态为禁用
    SetWindowCompositionAttribute(hwnd, &impl->winCompAttrData);  // 调用设置窗口合成属性的函数，移除窗口的背景效果

}

void EsWindowsWindowEffect::addShadowEffect(HWND hwnd) {
    // 检查DWM合成是否启用，如果未启用则返回
    if (!isCompositionEnabled()) {
        return;
    }

    MARGINS margins = {-1, -1, -1, -1};  // 创建MARGINS对象并设置边距为-1，表示将阴影扩展到整个客户区域
    DwmExtendFrameIntoClientArea(hwnd, &margins);  // 调用DWM扩展窗口框架函数，将阴影效果应用到窗口的客户区域

}

void EsWindowsWindowEffect::addMenuShadowEffect(HWND hwnd) {
    if (!isCompositionEnabled()) {
        return;
    }
    int enabled = DWMNCRP_ENABLED;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &enabled, sizeof enabled);  // 设置菜单的非客户区渲染策略为启用
    MARGINS margins = {-1, -1, -1, -1}; // 创建MARGINS对象并设置边距为-1，表示将阴影扩展到整个客户区域
    DwmExtendFrameIntoClientArea(hwnd, &margins);  // 调用DWM扩展窗口框架函数，将阴影效果应用到菜单的客户区域

}

void EsWindowsWindowEffect::removeShadowEffect(HWND hwnd) {
    int disabled = DWMNCRP_DISABLED;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &disabled, sizeof disabled); // 设置窗口的非客户区渲染策略为禁用，即移除阴影效果
}

void EsWindowsWindowEffect::removeMenuShadowEffect(HWND hwnd) {
    int newStyle;
    LONG_PTR style = GetClassLong(hwnd, GCL_STYLE);
    newStyle = style & ~CS_DROPSHADOW;  // CS_DROPSHADOW，即移除窗口的阴影效果
    SetClassLongPtr(hwnd, GCL_STYLE, newStyle); // 设置窗口的新样式

}

void EsWindowsWindowEffect::addWindowAnimation(HWND hwnd) {
    LONG_PTR style = GetClassLong(hwnd, GCL_STYLE);
    // 设置窗口的新样式，包括最小化、最大化按钮、标题栏、双击标题栏最大化、调整窗口大小的边框效果
    SetWindowLong(
            hwnd,
            GWL_STYLE,
            style | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | CS_DBLCLKS | WS_THICKFRAME);
}

void EsWindowsWindowEffect::disableMaximizeButton(HWND hwnd) {
    LONG_PTR style = GetClassLong(hwnd, GCL_STYLE);
    // 设置窗口的新样式
    SetWindowLong(
            hwnd,
            GWL_STYLE,
            style & ~WS_MAXIMIZEBOX  // 移除最大化按钮
    );
}

void EsWindowsWindowEffect::enableBlurBehindWindow(HWND hwnd) {
    DWM_BLURBEHIND blurBehind = {1, true, 0, false};
    DwmEnableBlurBehindWindow(hwnd, &blurBehind);
}

