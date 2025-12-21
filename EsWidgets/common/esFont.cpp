/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esFont.cpp
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esFont.h"

#include <QWidget>

#include "esConfig.h"


void EsFunc::setFont(QWidget* widget, int fontSize, QFont::Weight weight)
{
    widget->setFont(getFont(fontSize, weight));
}

QFont EsFunc::getFont(int fontSize, QFont::Weight weight)
{
    auto font = QFont();
    font.setFamilies(esConfig.get(&EsConfig::fontFamilies).toStringList());
    font.setPixelSize(fontSize);
    font.setWeight(weight);
    return font;
}

QString EsFunc::fontStyleSheet(const QFont& font)
{
    QStringList families;
    for (const QString& family : font.families())
    {
        families.append(QString("'%1'").arg(family));
    }

    return QString("font: %1px %2")
        .arg(font.pixelSize())
        .arg(families.join(","));
}

