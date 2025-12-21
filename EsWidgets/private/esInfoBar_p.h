/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esInfoBar_p.h
** @date 2025/11/15
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESINFOBAR_P_H
#define ESINFOBAR_P_H
#include <QColor>

#include "esDef.h"
#include "esSingleton.h"
#include <QSize>
class EsInfoIconWidget;
class EsInfoBar;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QLabel;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
class EsTransparentToolButton;
class QGraphicsOpacityEffect;


class EsInfoBarPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsInfoBar)

public:
    explicit EsInfoBarPrivate(QObject* parent = nullptr);
    void initWidget();
    void initLayout();

    void setQss();
    Q_SLOT void fadeOut();
    void adjustText();

    QString title; // 标题
    QString content; // 内容
    Qt::Orientation orient; // 排版方向
    QVariant icon; // 图标
    int duration; // 显示时长
    bool isClosable; // 是否可关闭
    Es::InfoBarPosition position; // 信息条的位置

    QLabel* titleLabel; // 标题标签
    QLabel* contentLabel; // 内容标签
    EsTransparentToolButton* closeButton; // 关闭按钮
    EsInfoIconWidget* iconWidget; // 图标组件

    QHBoxLayout* hBoxLayout; // 水平布局
    QBoxLayout* textLayout; // 文本布局（根据方向不同可能是水平或垂直）
    QBoxLayout* widgetLayout; // 小部件布局（根据方向不同可能是水平或垂直）

    QGraphicsOpacityEffect* opacityEffect; // 透明度效果
    QPropertyAnimation* opacityAni; // 透明度动画

    QColor lightBackgroundColor; // 亮色背景色
    QColor darkBackgroundColor; // 暗色背景色
};

/**
 * @brief 管理信息条的基类，负责管理不同位置的信息条以及相应的动画效果。
 *
 * 自定义位置信息条教程:\n
 *      1.继承此类 重写 pos slideStartPos 接口实现;\n
 *      2.在 Es::InfoBarPosition 枚举中添加自定义枚举;\n
 *      3.在程序入口像下面这样注册你的自定义类\n
 *          EsTopInfoBarManager::registerManager(Es::InfoBarPosition::TOP, new EsTopInfoBarManager());
 */
class ES_EXPORT EsInfoBarManager : public QObject
{
    Q_OBJECT
    E_SINGLETON_CREATE_H(EsInfoBarManager)

public:
    explicit EsInfoBarManager();
    // 添加信息条
    void add(EsInfoBar* infoBar);
    // 移除信息条
    void remove(EsInfoBar* infoBar);
    bool eventFilter(QObject* obj, QEvent* e) override;

    /**
     * @brief 注册一个自定义位置的信息条管理器。
     *
     * @param name     要注册的位置枚举值(添加自定义的新枚举)
     * @param manager  new 自定义Manager类()
     */
    static void registerManager(Es::InfoBarPosition name, EsInfoBarManager* manager);

    // 获取已注册的 EsInfoBarManager 实例
    static EsInfoBarManager* make(Es::InfoBarPosition position);

protected:

    // 计算位置信息, 必须重写
    virtual QPoint pos(EsInfoBar* infoBar, QSize parentSize = {});
    // 计算动画开始位置, 必须重写
    virtual QPoint slideStartPos(EsInfoBar* infoBar);

private:
    // 创建滑动动画
    QPropertyAnimation* createSlideAni(EsInfoBar* infoBar);
    // 更新掉落动画
    void updateDropAni(QWidget* parent);

protected:
    int spacing;
    int margin;
    QList<QPropertyAnimation*> dropAnis; // 掉落动画
    QList<QPropertyAnimation*> slideAnis; // 滑动动画
public:
    static QHash<QWidget*, QList<EsInfoBar*>> infoBars; // 父组件和信息条的映射
    static QHash<QWidget*, QParallelAnimationGroup*> aniGroups; // 父组件和动画组的映射
    static QMap<Es::InfoBarPosition, EsInfoBarManager*> managers; // 管理器注册
};

/// 顶部位置信息条管理器
class ES_EXPORT EsTopInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsTopInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};

/// 右上角位置信息条管理器
class ES_EXPORT EsTopRightInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsTopRightInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};

/// 右下角位置信息条管理器
class ES_EXPORT EsBottomRightInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsBottomRightInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};

/// 左上角位置信息条管理器
class ES_EXPORT EsTopLeftInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsTopLeftInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};

/// 左下角位置信息条管理器
class ES_EXPORT EsBottomLeftInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsBottomLeftInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};


/// 底部位置信息条管理器
class ES_EXPORT EsBottomInfoBarManager : public EsInfoBarManager
{
public:
    explicit EsBottomInfoBarManager()=default;

protected:
    QPoint pos(EsInfoBar* infoBar, QSize parentSize = {}) override;

    QPoint slideStartPos(EsInfoBar* infoBar) override;
};

#endif //ESINFOBAR_P_H
