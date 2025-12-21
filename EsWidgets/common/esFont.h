/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esFont.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESFONT_H
#define ES_ESFONT_H

#include <QFont>
#include "esSingleton.h"

namespace EsFunc
{
    /**
     * 给指定widget设置指定字体
     * @param widget 要设置字体的widget
     * @param fontSize 字号
     * @param weight 字体粗细
     */
    void setFont(QWidget* widget, int fontSize = 14, QFont::Weight weight = QFont::Normal);

    // 创建一个QFont
    QFont getFont(int fontSize = 14, QFont::Weight weight = QFont::Normal);

    /**
     * @brief 返回字体的样式表
     * @param font 字体
     * @return 样式表字符串
     */
    QString fontStyleSheet(const QFont& font);
};


#endif //ES_ESFONT_H
