/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTitleBar.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESTITLEBAR_H
#define ESTITLEBAR_H
#include "esDef.h"
#include <QWidget>

class QHBoxLayout;
class QLabel;

class EsTitleBarBasePrivate;
class EsMinimizeButton;
class EsCloseButton;
class EsMaximizeButton;

// 标题栏基类
class ES_EXPORT EsTitleBarBase : public QWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsTitleBarBase)

public:
    explicit EsTitleBarBase(QWidget* parent = nullptr);

public:
    EsMinimizeButton* minBtn;
    EsCloseButton* closeBtn;
    EsMaximizeButton* maxBtn;

    bool eventFilter(QObject* obj, QEvent* e) override;

    // 判断点击指定坐标后是否能拖动
    bool canDrag(QPoint pos);

    // 设置是否启用双击标题栏最大化窗口
    void setDoubleClickEnabled(bool isEnabled);

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
};


// 标题栏，带最小化，最大和关闭按钮
class ES_EXPORT EsTitleBar : public EsTitleBarBase
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(EsTitleBarBase);
    Q_DISABLE_COPY(EsTitleBar)

public:
    explicit EsTitleBar(QWidget* parent = nullptr);
};

// 带有图标和标题的标题栏
class ES_EXPORT EsTitleBarStandard : public EsTitleBar
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(EsTitleBarBase);
    Q_DISABLE_COPY(EsTitleBarStandard)

public:
    explicit EsTitleBarStandard(QWidget* parent = nullptr);
    Q_SLOT void setIcon(const QIcon &icon);
    Q_SLOT void setTitle(const QString& icon);

    QLabel* iconLabel;
    QLabel* titleLabel;

};
#endif //ESTITLEBAR_H
