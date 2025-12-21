/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIcon.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESICON_H
#define ESICON_H
#include <QAction>
#include <QColor>
#include <QIconEngine>

#include "esDef.h"

class QPainter;
class EsColoredIcon;
class EsIconBase;


class ES_EXPORT EsIconEngine : public QIconEngine
{
public:
    /**
     *
     * @param icon QIcon | EsIconBase
     * @param reverse 是否切换图标的主题
     */
    EsIconEngine(const QIcon &icon, bool reverse = false);
    EsIconEngine(EsIconBase* icon, bool reverse = false);

    void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
    EsIconEngine* clone() const override;

public:
    bool isThemeReversed;
    QVariant icon;
};


class ES_EXPORT EsSvgIconEngine : public QIconEngine
{
public:
    EsSvgIconEngine(const QString& svg);

    void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;
    EsSvgIconEngine* clone() const override;

public:
    QString svg;
};


class ES_EXPORT EsFontIconEngine : public QIconEngine
{
public:
    // 构造函数
    EsFontIconEngine(const QString& fontFamily, const QString& character, const QColor& color, bool isBold);

    // 绘制图标
    void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;

    // 克隆引擎
    EsFontIconEngine* clone() const override;

    // 返回一个 pixmap
    QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

private:
    QString fontFamily; // 字体家族
    QString char_; // 字符
    QColor color; // 字符颜色
    bool isBold; // 是否为粗体
};


namespace
ES_EXPORT EsFunc
{
    /**
    * 根据主题获取图标的颜色
    *
    * @param theme 图标的主题
    *  * `Theme.AUTO`: 根据系统主题自动选择
    *  * `Theme.LIGHT`: 浅色主题
    *  * `Theme.DARK`: 深色主题
    * @param reverse 是否反转颜色（即白色和黑色互换）
    * @return 返回根据主题选择的颜色
    */
    QString getIconColor(Es::Theme theme = Es::Theme::Theme_AUTO, bool reverse = false);

    /**
     * 绘制 svg 图标
     *
     * @param icon 图标的路径或 SVG 代码
     *  * 可以是 QString（路径）或 QByteArray（SVG 代码）
     * @param painter 绘制器
     * @param rect 绘制区域
     */
    void drawSvgIcon(const QVariant& icon, QPainter* painter, const QRectF& rect);

    /**
     * 使用指定的属性写入 SVG 文件
     *
     * @param iconPath svg 图标的路径
     * @param indexes 需要修改的路径索引
     * @param attributes 路径的修改属性
     * @return 返回修改后的 SVG 代码
     */
    QString writeSvg(const QString& iconPath, const QList<int>& indexes = QList<int>(),
                     std::initializer_list<std::pair<QString, QString>> attributes = {});

    /**
     * 绘制图标
     * @param variant 可以是 QIcon 或 EsIconBase 或自定义的 Icon 对象 或 str
     * @param painter 绘制器
     * @param rect 绘制区域
     * @param state 图标状态，默认为 QIcon::Off
     * @param attributes SVG 图标的附加属性
     */
    void drawIcon(const QVariant& variant, QPainter* painter, const QRectF& rect, QIcon::State state = QIcon::Off, const QList<int>& indexes = QList<int>(), std::initializer_list<std::pair<QString, QString>> attributes = {});

    QIcon toQIcon(const QVariant& variant);

    QColor validColor(const QColor& color, const QColor& defaultColor);

    QColor fallbackThemeColor(const QColor& color);
    QColor autoFallbackThemeColor(const QColor& light, const QColor& dark);


}


class ES_EXPORT EsIconBase
{
public:
    virtual ~EsIconBase() = default;
    explicit EsIconBase()=default;
    explicit EsIconBase(Es::IconName iconName);

    /**
     * 获取图标的路径
     *
     * @param theme 图标的主题
     *  * `Theme.Light`: 黑色图标
     *  * `Theme.DARK`: 白色图标
     *  * `Theme.AUTO`: 图标颜色取决于 `config.theme`
     * @return 返回图标路径
     */
    virtual QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const;

    /**
     * 创建一个流畅的图标
     * @param theme 图标的主题
     *  * `Theme.Light`: 黑色图标
     *  * `Theme.DARK`: 白色图标
     *  * `Theme.AUTO`: 图标颜色取决于 `qconfig.theme`
     * @param color 图标颜色，仅适用于 svg 图标
     * @return 返回创建的图标
     */
    QIcon icon(Es::Theme theme = Es::Theme::Theme_AUTO, const QColor& color = QColor()) const;

    /**
     * 创建一个带有颜色的流畅图标
     *
     * @param lightColor 浅色模式下的图标颜色
     * @param darkColor 深色模式下的图标颜色
     * @return 返回一个带有颜色的流畅图标对象
     */
    EsColoredIcon colored(const QColor& lightColor, const QColor& darkColor);

    /**
     * 转换为 QIcon，图标的主题将会与应用同步更新
     *
     * @param reverse 是否反转图标的主题
     * @return 返回转换后的 QIcon 对象
     */
    QIcon qicon(bool reverse = false);

    /**
     * 绘制 svg 图标
     *
     * @param painter 绘制器
     * @param rect 绘制区域
     * @param theme 图标的主题
     *  * `EsTheme::Light`: 黑色图标
     *  * `EsTheme::DARK`: 白色图标
     *  * `EsTheme::AUTO`: 图标颜色取决于 `config.theme`
     * @param indexes 需要修改的 svg 路径索引
     * @param attributes 路径的修改属性
     */
    void render(QPainter* painter, const QRectF& rect, Es::Theme theme = Es::Theme::Theme_AUTO,
                const QList<int>& indexes = QList<int>(),
                std::initializer_list<std::pair<QString, QString>> attributes = {});
    void render(QPainter* painter, const QRectF& rect, const QList<int>& indexes = QList<int>(), std::initializer_list<std::pair<QString, QString>> attributes = {});
    void render(QPainter* painter, const QRectF& rect,  std::initializer_list<std::pair<QString, QString>> attributes);

    Es::IconName getIconName() const;

    QString getIconNameStr() const;

protected:
    Es::IconName iconName;
    QString iconNameStr;
};
Q_DECLARE_METATYPE(EsIconBase*)


class ES_EXPORT EsColoredIcon : public EsIconBase
{
public:
    explicit EsColoredIcon(Es::IconName iconName, const QString &lightColor, const QString &darkColor);
    explicit EsColoredIcon(Es::IconName iconName, const QColor &lightColor, const QColor &darkColor);

    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

public:
    QColor lightColor;
    QColor darkColor;
};

class ES_EXPORT EsIcon : public EsIconBase
{
public:

    explicit EsIcon()=default;
    explicit EsIcon(Es::IconName iconName)
        : EsIconBase(iconName)
    {
    }

    QString path(Es::Theme theme= Es::Theme::Theme_AUTO) const override;
};
Q_DECLARE_METATYPE(EsIcon)


class ES_EXPORT EsCheckBoxIcon : public EsIconBase
{
public:
    explicit EsCheckBoxIcon() = default;

    explicit EsCheckBoxIcon(Es::IconName iconName)
        : EsIconBase(iconName){}

    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;
};

class ES_EXPORT EsInfoBarIcon: public EsIconBase
{
public:
    explicit EsInfoBarIcon()=default;
    explicit EsInfoBarIcon(Es::IconName iconName)
        : EsIconBase(iconName){}
    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;
};

class ES_EXPORT EsAction : public QAction
{
    Q_OBJECT
public:
    explicit EsAction(QObject *parent = nullptr);
    explicit EsAction(const QString &text, QObject *parent = nullptr);
    explicit EsAction(const QIcon &icon, const QString &text, QObject *parent = nullptr);
    explicit EsAction(EsIconBase* icon, const QString &text, QObject *parent = nullptr);

    QIcon icon() const;
    void setIcon(const QVariant &icon);
private:
    QVariant _icon;
};
#endif //ESICON_H
