/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file EsSmoothScroll.cpp
** @date 2025/11/12
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esSmoothScroll.h"
#include <QDateTime>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>

EsSmoothScroll::EsSmoothScroll(QScrollArea* widget, Qt::Orientation orient)
    : QObject(widget),
      widget(widget),
      orient(orient),
      fps(60),
      duration(400),
      stepsTotal(0),
      stepRatio(1.5),
      acceleration(1),
      lastWheelEvent(nullptr),
      smoothMoveTimer(new QTimer(widget)),
      smoothMode(Es::SmoothMode::LINEAR)
{
    connect(smoothMoveTimer, &QTimer::timeout, this, &EsSmoothScroll::smoothMove);
}

void EsSmoothScroll::setSmoothMode(Es::SmoothMode smoothMode)
{
    this->smoothMode = smoothMode;
}

void EsSmoothScroll::wheelEvent(QWheelEvent* e)
{
    double delta = (e->angleDelta().y() != 0) ? e->angleDelta().y() : e->angleDelta().x();
    if (this->smoothMode == Es::SmoothMode::NO_SMOOTH || static_cast<int>(std::abs(delta)) % 120 != 0) {
        QCoreApplication::sendEvent(widget, e);
        return;
    }

    // 将当前时间推入队列
    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    this->scrollStamps.push_back(now);

    while (now - this->scrollStamps.front() > 500) {
        this->scrollStamps.pop_front();
    }

    // 根据未处理事件调整加速度比例
    double accelerationRatio = std::min(static_cast<double>(this->scrollStamps.size()) / 15.0, 1.0);

    if (!this->lastWheelEvent) {
        this->lastWheelEvent = new QWheelEvent(*e);
    } else {
        this->lastWheelEvent = e;
    }

    // 计算总步数
    this->stepsTotal = this->fps * this->duration / 1000;
    // 获取每次滚动的移动距离
    delta = delta * this->stepRatio;
    if (this->acceleration > 0) {
        delta += delta * this->acceleration * accelerationRatio;
    }

    // 形成移动距离和步数的列表，并将其插入队列进行处理
    this->stepsLeftQueue.push_back({ delta, this->stepsTotal });

    // 定时器的溢出时间：1000ms/帧
    this->smoothMoveTimer->start(1000 / this->fps);
}

void EsSmoothScroll::smoothMove()
{
    double totalDelta = 0;

    // 计算所有未处理事件的滚动距离，
    // 每次定时器超时，步骤数会减少1
    for (auto& step : this->stepsLeftQueue) {
        totalDelta += this->subDelta(step.first, step.second);
        step.second -= 1;
    }

    // 如果事件已经处理完成，从队列中移除
    while (!this->stepsLeftQueue.empty() && this->stepsLeftQueue.front().second == 0) {
        this->stepsLeftQueue.pop_front();
    }

    // 构造滚轮事件
    QPoint p;
    QScrollBar* bar;
    if (this->orient == Qt::Vertical) {
        p = QPoint(0, round(totalDelta));
        bar = this->widget->verticalScrollBar();
    } else {
        p = QPoint(round(totalDelta), 0);
        bar = this->widget->horizontalScrollBar();
    }

    QWheelEvent e(
        this->lastWheelEvent->position(),
        this->lastWheelEvent->globalPosition(),
        QPoint(),
        p,
        round(totalDelta),
        this->orient,
        this->lastWheelEvent->buttons(),
        Qt::NoModifier
    );

    // 发送滚轮事件给应用
    QApplication::sendEvent(bar, &e);

    // 如果队列为空，停止滚动
    if (this->stepsLeftQueue.empty()) {
        this->smoothMoveTimer->stop();
    }
}

double EsSmoothScroll::subDelta(double delta, int stepsLeft)
{

    double m = this->stepsTotal / 2;
    double x = std::abs(this->stepsTotal - stepsLeft - m);

    double res = 0;
    if (this->smoothMode == Es::SmoothMode::NO_SMOOTH) {
        res = 0;
    } else if (this->smoothMode == Es::SmoothMode::CONSTANT) {
        res = delta / this->stepsTotal;
    } else if (this->smoothMode == Es::SmoothMode::LINEAR) {
        res = 2 * delta / this->stepsTotal * (m - x) / m;
    } else if (this->smoothMode == Es::SmoothMode::QUADRATI) {
        res = 3.0 / 4.0 / m * (1 - (x * x) / (m * m)) * delta;
    } else if (this->smoothMode == Es::SmoothMode::COSINE) {
        res = (std::cos(x * 3.141592653589793 / m) + 1) / (2 * m) * delta;
    }
    return res;
}
