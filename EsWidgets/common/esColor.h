/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esColor.h
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESCOLOR_H
#define ES_ESCOLOR_H
#include "esConfig.h"
#include "esDef.h"


class ES_EXPORT EsSystemColor
{
public:
    enum Color
    {
        // 前景色（字体或图标颜色）
        SUCCESS_FOREGROUND,   // 成功状态的前景色（通常为绿色）
        CAUTION_FOREGROUND,   // 警告状态的前景色（通常为黄色或橙色）
        CRITICAL_FOREGROUND,  // 危急状态的前景色（通常为红色）

        // 背景色
        SUCCESS_BACKGROUND,   // 成功状态的背景色（通常为浅绿色）
        CAUTION_BACKGROUND,   // 警告状态的背景色（通常为浅黄色或浅橙色）
        CRITICAL_BACKGROUND   // 危急状态的背景色（通常为浅红色）
    };
    static QColor color(Color color, Es::Theme theme=Es::Theme::Theme_AUTO);
};


#endif //ES_ESCOLOR_H
