/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScreen.h
** @date 2025/12/6
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSCREEN_H
#define ESSCREEN_H
#include <QRect>

class QScreen;

namespace EsFunc
{

    /**
     * @brief 获取鼠标所在的屏幕
     *
     * 遍历所有屏幕，查找其 geometry 是否包含当前鼠标位置。
     * 若找到，则返回对应的 QScreen 指针；若未找到则返回 nullptr。
     *
     * @return QScreen* 鼠标所在的屏幕；若无匹配则为 nullptr。
     */
     QScreen* getCurrentScreen();


    /**
     * @brief 获取当前屏幕的几何区域
     *
     * 根据鼠标所在位置获取当前屏幕，返回其 availableGeometry()
     * 或 geometry()。若无法找到屏幕，则返回一个默认大小为 1920x1080 的 QRect。
     *
     * @param available 是否返回可用区域（去除任务栏）。为 false 时返回屏幕全部区域。
     * @return QRect 当前屏幕的矩形区域。
     */
    QRect getCurrentScreenGeometry(bool available = true);
}



#endif //ESSCREEN_H
