/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esWin32Utils.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWIN32UTILS_H
#define ESWIN32UTILS_H

#include <QOperatingSystemVersion>
#include <QColor>

#include <Windows.h>

/**
 * 取系统强调色
 * @return
 */
QColor getSystemAccentColor();

/**
 * 检查边框强调是否启用
 * @return
 */
bool isSystemBorderAccentEnabled();

/**
 * 检查窗口是否最大化
 * @param hWnd 窗口句柄
 * @return
 */
bool isMaximized(HWND hWnd);

// 检查窗口是否为全屏
bool isFullScreen(HWND hWnd);


/**
 * 获取窗口的大小边框厚度
 * @param hWnd 窗口句柄
 * @param horizontal true(默认,取水平边框) or false(取垂直边框)
 * @return
 */
int getResizeBorderThickness(HWND hWnd, bool horizontal = true);


// 取指定窗口的 DPI
/**
 * @brief 获取指定窗口的 DPI
 * @param hWnd 目标窗口句柄
 * @param horizontal true(默认,取水平 DPI) or false(取垂直 DPI)
 *
 * @return 返回 DPI 值，默认 DPI 为 96。
 */
int getDpiForWindow(HWND hWnd, bool horizontal = true);


/**
 * @brief 在指定窗口句柄的窗口上释放鼠标左键，鼠标位置为 (x, y)。
 *
 * 该函数模拟发送 WM_LBUTTONUP 消息，释放鼠标左键并将鼠标移动到指定的 (x, y) 坐标位置。
 *
 * @param hWnd 窗口句柄，表示目标窗口的标识。
 * @param x 鼠标释放时的 x 坐标，默认为 0。
 * @param y 鼠标释放时的 y 坐标，默认为 0。
 */
void releaseMouseLeftButton(HWND hWnd, int x = 0, int y = 0);

/**
 * 获取Windows操作系统版本号
 * @return 操作系统版本号 例如: 版本号为22631.2715 返回 22631
 */
int getWindowsVersion();


/**
 * 检测 DWM 组合是否已启用
 */
bool isCompositionEnabled();


/**
 * 判断当前 Windows 版本是否大于等于指定版本
 * @param version 指定的 Windows 版本
 * @return true(大于等于) or false(小于)
 */
bool isGreaterEqualVersion(QOperatingSystemVersion version);


/**
 * 判断当前 Windows 版本是否大于等于 Windows 8.1
 * @return
 */
bool isGreaterEqualWin8_1();


/**
 * 判断当前 Windows 版本是否大于等于 Windows 10
 * @return
 */
bool isGreaterEqualWin10();

/**
 * 判断当前 Windows 版本是否大于等于 Windows 11
 * @return
 */
bool isGreaterEqualWin11();

bool isWin7();




#endif //ESWIN32_UTILS_H
