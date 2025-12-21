/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esRouter.h
** @date 2025/11/29
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESROUTER_H
#define ESROUTER_H


#include <QWidget>
#include <QStackedWidget>
#include <QObject>
#include <QList>
#include <QMap>
#include <QGroupBox>
#include <QVariant>

#include "esSingleton.h"

class EsRouteItem {
public:
    /**
     * @brief RouteItem 构造函数
     * @param frame QFrame 子界面堆栈（可以是 QStackedWidget 或自定义的 StackedWidget）
     * @param routeKey 路由的唯一标识符
     */
    EsRouteItem(QFrame* frame, const QString& routeKey);

    bool operator==(const EsRouteItem& other) const;

    QFrame* frame;  // 改为 QFrame*，以便支持 QStackedWidget 和 StackedWidget
    QString routeKey;
};

class EsStackedHistory {
public:
    /**
     * @brief StackedHistory 构造函数
     * @param frame QFrame 子界面堆栈（可以是 QStackedWidget 或自定义的 StackedWidget）
     */
    EsStackedHistory(QFrame* frame);

    int length() const;
    bool isEmpty() const;

    bool push(const QString& routeKey);

    void pop();

    void remove(const QString& routeKey);

    QString top() const;

    void setDefaultRouteKey(const QString& routeKey);

    void goToTop();

private:
    QFrame* frame;  // 改为 QFrame*，以便支持 QStackedWidget 和 StackedWidget
    QString defaultRouteKey;
    QList<QString> history;
};

class EsRouter : public QObject {
    Q_OBJECT

Q_SIGNALS:
    /**
     * @brief 当历史记录为空时发出信号
     * @param empty 是否为空
     */
    void emptyChanged(bool empty);
    E_SINGLETON_CREATE_H(EsRouter)
public:
    /**
     * @brief Router 构造函数
     * @param parent 父对象
     */
    EsRouter(QObject* parent = nullptr);

    /**
     * @brief Router 析构函数
     * 清理分配的 StackedHistory 对象
     */
    ~EsRouter();

    /**
     * @brief 设置堆栈控件的默认路由
     * @param frame QFrame 子界面堆栈（可以是 QStackedWidget 或自定义的 StackedWidget）
     * @param routeKey 默认路由的唯一标识符
     */
    void setDefaultRouteKey(QFrame* frame, const QString& routeKey);

    /**
     * @brief 压入历史记录
     * @param frame QFrame 子界面堆栈（可以是 QStackedWidget 或自定义的 StackedWidget）
     * @param routeKey 路由的唯一标识符
     */
    void push(QFrame* frame, const QString& routeKey);

    /**
     * @brief 弹出历史记录
     */
    Q_SLOT void pop();

    /**
     * @brief 移除特定的路由历史
     * @param routeKey 路由的唯一标识符
     */
    void remove(const QString& routeKey);

private:
    QList<EsRouteItem> history;  // 路由历史
    QMap<QFrame*, EsStackedHistory*> stackHistories; // 各个堆栈的历史记录（支持 QStackedWidget 和 StackedWidget）
};

inline EsRouter esRouter;
#endif //ESROUTER_H
