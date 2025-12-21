/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esScrollArea.cpp
** @date 2025/12/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esScrollArea.h"

#include "esSmoothScroll.h"

EsScrollArea::EsScrollArea(QWidget* parent)
    : QScrollArea(parent), scrollDelegate(new EsSmoothScrollDelegate(this))
{
}

void EsScrollArea::setSmoothMode(Es::SmoothMode mode, Qt::Orientation orientation)
{
    if (orientation == Qt::Vertical)
    {
        scrollDelegate->verticalSmoothScroll->setSmoothMode(mode);
    }
    else
    {
        scrollDelegate->horizontalSmoothScroll->setSmoothMode(mode);
    }
}

void EsScrollArea::enableTransparentBackground()
{
    this->setStyleSheet("QScrollArea{border: none; background: transparent}");
    if (this->widget())
    {
        this->widget()->setStyleSheet("QWidget{background: transparent}");
    }
}
