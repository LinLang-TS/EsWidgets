/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esColor.cpp
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esColor.h"

QColor EsSystemColor::color(Color color, Es::Theme theme)
{
    static QMap<Color, std::pair<QString, QString>> colorMap = {
        {SUCCESS_FOREGROUND, {"#0f7b0f", "#6ccb5f"}},
        {CAUTION_FOREGROUND, {"#9d5d00", "#fce100"}},
        {CRITICAL_FOREGROUND, {"#c42b1c", "#ff99a4"}},
        {SUCCESS_BACKGROUND, {"#dff6dd", "#393d1b"}},
        {CAUTION_BACKGROUND, {"#fff4ce", "#433519"}},
        {CRITICAL_BACKGROUND, {"#fde7e9", "#442726"}}
    };
    return EsFunc::isDarkThemeMode(theme) ? colorMap.value(color).first : colorMap.value(color).second;
}
