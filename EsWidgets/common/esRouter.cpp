/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esRouter.cpp
** @date 2025/11/29
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esRouter.h"
#include "esRouter.h"

#include "windows/esStackedWidget.h"

// region ================= EsRouteItem  =================
EsRouteItem::EsRouteItem(QFrame* frame, const QString& routeKey)
    : frame(frame), routeKey(routeKey)
{
}

bool EsRouteItem::operator==(const EsRouteItem& other) const
{
    return (this->frame == other.frame) && (this->routeKey == other.routeKey);
}
// endregion

// region ================= EsStackedHistory  =================

EsStackedHistory::EsStackedHistory(QFrame* frame)
    : frame(frame), defaultRouteKey(""), history({defaultRouteKey})
{
}

int EsStackedHistory::length() const
{
    return history.length();
}

bool EsStackedHistory::isEmpty() const
{
    return length() <= 1;
}

bool EsStackedHistory::push(const QString& routeKey)
{
    if (history.last() == routeKey)
    {
        return false; // 路由已经存在，不重复添加
    }
    history.append(routeKey);
    return true;
}

void EsStackedHistory::pop()
{
    if (isEmpty())
    {
        return;
    }
    history.removeLast();
    goToTop();
}

void EsStackedHistory::remove(const QString& routeKey)
{
    // 删除所有与 routeKey 相等的项
    history.removeAll(routeKey);

    // 使用 std::unique 去重
    history.sort(); // 排序，确保相同项是相邻的
    history.erase(std::unique(history.begin(), history.end()), history.end()); // 去重

    goToTop(); // 更新界面
}

QString EsStackedHistory::top() const
{
    return history.last();
}

void EsStackedHistory::setDefaultRouteKey(const QString& routeKey)
{
    defaultRouteKey = routeKey;
    history[0] = routeKey;
}

void EsStackedHistory::goToTop()
{
    auto widget = frame->findChild<QWidget*>(top());
    if (widget)
    {
        if (auto stackedWidget = qobject_cast<QStackedWidget*>(frame))
        {
            stackedWidget->setCurrentWidget(widget);
        }
        else if (auto customWidget = qobject_cast<EsStackedWidget*>(frame))
        {
            customWidget->setCurrentWidget(widget);
        }
    }
}

// endregion

// region ================= EsRouter  =================
E_SINGLETON_CREATE_CPP(EsRouter)

EsRouter::EsRouter(QObject* parent)
    : QObject(parent)
{
}

EsRouter::~EsRouter()
{
    qDeleteAll(stackHistories); // 删除所有堆栈历史
    stackHistories.clear();
}

void EsRouter::setDefaultRouteKey(QFrame* frame, const QString& routeKey)
{
    if (!stackHistories.contains(frame))
    {
        stackHistories[frame] = new EsStackedHistory(frame);
    }
    stackHistories[frame]->setDefaultRouteKey(routeKey);
}

void EsRouter::push(QFrame* frame, const QString& routeKey)
{
    EsRouteItem item(frame, routeKey);

    if (!stackHistories.contains(frame))
    {
        stackHistories[frame] = new EsStackedHistory(frame);
    }

    bool success = stackHistories[frame]->push(routeKey);
    if (success)
    {
        history.append(item);
    }

    emit emptyChanged(history.isEmpty());
}

void EsRouter::pop()
{
    if (history.isEmpty())
    {
        return;
    }

    EsRouteItem item = history.takeLast();
    stackHistories[item.frame]->pop();

    emit emptyChanged(history.isEmpty());
}

void EsRouter::remove(const QString& routeKey)
{
    // 删除历史记录中的所有对应项
    history.erase(std::remove_if(history.begin(), history.end(),
                                 [routeKey](const EsRouteItem& item)
                                 {
                                     return item.routeKey == routeKey;
                                 }),
                  history.end());

    // 清理所有相应的 StackedHistory 对象
    foreach(auto tmpHistory, stackHistories)
    {
        tmpHistory->remove(routeKey);
    }

    // 如果某个 QFrame 对应的 StackedHistory 没有任何历史记录了，删除它
    for (auto it = stackHistories.begin(); it != stackHistories.end();)
    {
        if (it.value()->length() <= 1)
        {
            delete it.value(); // 删除对应的 StackedHistory 对象
            it = stackHistories.erase(it); // 从 QMap 中移除该项
        }
        else
        {
            ++it;
        }
    }

    emit emptyChanged(history.isEmpty());
}

// endregion
