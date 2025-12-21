/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWin32Utils.cpp
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esWin32Utils.h"
#include <dwmapi.h>
#include <QGuiApplication>
#include <QDebug>
#include <QWindow>


// 取系统强调色
QColor getSystemAccentColor()
{
    COLORREF color = 0;
    BOOL opaque = FALSE;

    // 调用 DwmGetColorizationColor 获取系统色彩
    HRESULT result = DwmGetColorizationColor(&color, &opaque);

    if (FAILED(result)) {
        // 如果函数调用失败，发出警告并返回默认颜色
        qWarning() << "Unable to obtain system accent color.";
        return {};
    }

    // 将 COLORREF 转换为 QColor 对象并返回
    return {GetRValue(color), GetGValue(color), GetBValue(color)};
}

// 检查边框强调是否启用
bool isSystemBorderAccentEnabled() {

    // 如果不是 Windows 11 或更高版本，直接返回 false
    if  (!isGreaterEqualWin11()) return false;


    try {
        // 打开注册表键 HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\DWM
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\DWM", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            return false;
        }

        DWORD value = 0;
        // 查询 ColorPrevalence 键的值
        if (RegQueryValueEx(hKey, "ColorPrevalence", NULL, NULL, (LPBYTE)&value, NULL) != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return false;
        }

        RegCloseKey(hKey);

        // 如果 ColorPrevalence 值为非零，则返回 true，表示启用了边框强调
        return value != 0;

    } catch (...) {
        return false;
    }
}

// 检查窗口是否最大化
bool isMaximized(HWND hWnd) {

    // 获取窗口的位置和状态
    WINDOWPLACEMENT windowPlacement;
    if (!GetWindowPlacement(hWnd, &windowPlacement)) {
        return false;  // 获取窗口状态失败
    }

    // 检查窗口是否最大化
    return windowPlacement.showCmd == SW_MAXIMIZE;
}



// 检查窗口是否为全屏
bool isFullScreen(HWND hWnd) {

    // 如果窗口句柄无效，返回 false
    if (!hWnd) {
        return false;
    }

    // 获取窗口的位置和大小
    RECT winRect;
    if (!GetWindowRect(hWnd, &winRect)) {
        return false;  // 获取窗口矩形失败
    }

    // 获取显示器信息
    MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
    if (!hMonitor || !GetMonitorInfo(hMonitor, &monitorInfo)) {
        return false;  // 获取显示器信息失败
    }

    // 获取显示器的矩形区域
    RECT monitorRect = monitorInfo.rcMonitor;

    // 比较窗口矩形和显示器矩形
    return (winRect.left == monitorRect.left && winRect.top == monitorRect.top &&
            winRect.right == monitorRect.right && winRect.bottom == monitorRect.bottom);
}


// 查找窗口，返回窗口指针，如果未找到则返回 nullptr
QWindow* findTopLevelWindow(HWND hWnd)
{
    if (hWnd == NULL)
    {
        return nullptr;
    }

    // 获取顶层窗口列表
    QWindowList windows = QGuiApplication::topLevelWindows();
    if (windows.isEmpty())
    {
        return nullptr;
    }

    // 遍历窗口列表，匹配窗口ID
    for (auto window : windows)
    {
        if (window && static_cast<DWORD>(window->winId()) == (DWORD)hWnd)
        {
            return window;
        }
    }

    return nullptr; // 未找到匹配窗口时返回 nullptr
}

// 获取窗口的大小边框厚度
int getResizeBorderThickness(HWND hWnd, bool horizontal)
{
    // 查找顶层窗口
    auto window = findTopLevelWindow(hWnd);
    if (window == nullptr)
    {
        return 0;
    }

    // 根据水平还是垂直边框选择适当的系统参数
    int frame = horizontal ? SM_CXSIZEFRAME : SM_CYSIZEFRAME;

    // 获取系统的边框尺寸
    int result = GetSystemMetrics(frame) + GetSystemMetrics(92);

    // 如果返回的尺寸大于零，则返回
    if (result > 0)
    {
        return result;
    }

    // 否则，基于是否启用了 DWM 组合来确定边框厚度
    int thickness = isCompositionEnabled() ? 8 : 4;
    return static_cast<int>(round(thickness * window->devicePixelRatio()));
}

// 取窗口DPI
int getDpiForWindow(HWND hWnd, bool horizontal)
{
    // GetDpiForWindow()最早支持版本 == windows 10 Version 1607 (10.0.14393)
    if (isGreaterEqualVersion(QOperatingSystemVersion::Windows10) && getWindowsVersion() >= 14393)
    {
        return GetDpiForWindow(hWnd);
    }

    // 获取设备上下文
    HDC hdc = GetDC(hWnd);
    if (!hdc)
    {
        return 96;  // 默认 DPI
    }

    // 获取水平和垂直 DPI
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(hWnd, hdc);

    if (dpiX > 0 && horizontal)
    {
        return dpiX;  // 如果是水平 DPI
    }
    else if (dpiY > 0 && !horizontal)
    {
        return dpiY;  // 如果是垂直 DPI
    }

    return 96;  // 默认 DPI
}

void releaseMouseLeftButton(HWND hWnd, int x, int y)
{
    // 计算 LPARAM 参数，通过将 x 和 y 坐标组合成一个 32 位整数
    LPARAM lp = ((y & 0xFFFF) << 16) | (x & 0xFFFF);

    // 发送 WM_LBUTTONUP 消息，模拟释放鼠标左键
    SendMessage(hWnd, WM_LBUTTONUP, 0, lp);
}


int getWindowsVersion() {
    HKEY hKey;
    LPCTSTR subkey = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        TCHAR version[256];
        DWORD size = sizeof(version);

        if (RegGetValue(hKey, NULL, TEXT("CurrentBuildNumber"), RRF_RT_REG_SZ, NULL, version, &size) == ERROR_SUCCESS) {
            std::string versionStr(version);
            int majorVersion = atoi(versionStr.substr(0, versionStr.find('.')).c_str());
            return majorVersion;
        } else {
            qDebug() << "获取版本号失败";
        }

        RegCloseKey(hKey);
    } else {
        qDebug() << "无法打开注册表项";
    }

    return -1;
}


bool isCompositionEnabled() {
    BOOL bResult = FALSE;
    if (SUCCEEDED(DwmIsCompositionEnabled(&bResult)))
    {
        return bResult == TRUE;
    }
    return false;
}



bool isGreaterEqualVersion(QOperatingSystemVersion version) {
    return QOperatingSystemVersion::current() >= version;
}

bool isGreaterEqualWin8_1() {
    return isGreaterEqualVersion(QOperatingSystemVersion::Windows8_1);
}

bool isGreaterEqualWin10() {
    return isGreaterEqualVersion(QOperatingSystemVersion::Windows10);
}

bool isGreaterEqualWin11() {
    return isGreaterEqualVersion(QOperatingSystemVersion::Windows10) && getWindowsVersion() >= 22000;
}
bool isWin7()
{
    OSVERSIONINFOEX osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    // win7的系统版本为NT6.1
    if ( VER_PLATFORM_WIN32_NT == osvi.dwPlatformId &&
         osvi.dwMajorVersion == 6 &&
         osvi.dwMinorVersion == 1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
