/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScreen.cpp
** @date 2025/12/6
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esScreen.h"

#include <QApplication>
#include <QCursor>
#include <QGuiApplication>
#include <QScreen>


QScreen* EsFunc::getCurrentScreen()
{
    const QPoint cursorPos = QCursor::pos();

    for (QScreen* s : QGuiApplication::screens()) {
        if (s->geometry().contains(cursorPos)) {
            return s;
        }
    }

    return nullptr;
}

QRect EsFunc::getCurrentScreenGeometry(bool available)
{
    QScreen* screen = getCurrentScreen();
    if (!screen) {
        screen = QApplication::primaryScreen();
    }

    if (!screen) {
        // 理论上不会发生的情况
        return QRect(0, 0, 1920, 1080);
    }

    return available ? screen->availableGeometry()
                     : screen->geometry();
}
