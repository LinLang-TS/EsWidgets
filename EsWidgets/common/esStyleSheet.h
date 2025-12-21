/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esStyleSheet.h
** @date 2025/10/6
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSTYLESHEET_H
#define ESSTYLESHEET_H
#include <QFile>

#include "esConfig.h"
#include "esDef.h"
#include "esSingleton.h"
class EsStyleSheetCompose;

class ES_EXPORT EsStyleSheetManager : public QObject
{
    Q_OBJECT
    E_SINGLETON_CREATE_H(EsStyleSheetManager)

public:
    explicit EsStyleSheetManager(QObject* parent = nullptr);
    ~EsStyleSheetManager() override;

    /**
     * @brief 注册小部件到管理器
     * @param source QSS 源，可以是：
     *        - QString：QSS 文件路径
     *        - StyleSheetBase*：样式表实例指针
     * @param widget 要设置样式表的小部件
     * @param reset 是否重置 QSS 源
     */
    void registerWidget(const QVariant& source, QWidget* widget, bool reset = true);

    /**
      * @brief 解除注册小部件
      * 从管理器中移除小部件。
      * @param widget 要解除注册的小部件
     */
    Q_SLOT void deregister(QWidget* widget);


    QMap<QWidget*, EsStyleSheetCompose*> items() const;

    /**
     * @brief 获取小部件的 QSS 源
     *
     * @param widget 要查询的小部件
     * @return StyleSheetCompose 返回小部件的样式表，如果没有找到则返回空样式表
     */
    EsStyleSheetCompose* source(QWidget* widget) const;

public:
    QMap<QWidget*, EsStyleSheetCompose*> widgets;
};


class ES_EXPORT QssTemplate
{
public:
    ~QssTemplate() = default;
    /**
     * @brief 样式表模板类
     *
     * 用于生成 QSS 模板，提供替换功能。
     *
     * @param qss 输入的 QSS 模板字符串
     */
    QssTemplate(const QString& qss);

    /**
     * @brief 替换模板中的变量
     *
     * @param mappings 变量映射，键为占位符(qss里`--`开头的变量)，值为替换内容
     * @return QString 替换后的 QSS 字符串
     */
    QString safeSubstitute(const QMap<QString, QString>& mappings) const;

private:
    QString qssTemplate; // 存储 QSS 模板字符串
};


// 样式表基类
class ES_EXPORT EsStyleSheetBase
{
public:
    virtual ~EsStyleSheetBase() = default; // 虚析构函数

    /**
     * @brief 获取样式表的路径
     *
     * @param theme 主题
     * @return QString 返回样式表路径
     */
    virtual QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const;

    /**
     * @brief 获取样式表的内容
     *
     * @param theme 主题, 默认 Es::Theme::AUTO
     * @return QString 返回样式表内容
     */
    virtual QString content(Es::Theme theme = Es::Theme::Theme_AUTO) const;

    /**
     * @brief 将样式表应用到 QWidget 上
     *
     * @param widget 要应用样式表的小部件
     * @param theme 主题
     */
    void apply(QWidget* widget, Es::Theme theme = Es::Theme::Theme_AUTO);
};

Q_DECLARE_METATYPE(EsStyleSheetBase*)


// Es样式表
class ES_EXPORT EsStyleSheet : public EsStyleSheetBase
{
public:
    /**
     * 应用Es内置qss样式
     * 必须用指针创建, 内部会自动管理内存; 使用示例:
     *  auto style = new EsStyleSheet(Es::StyleSheet::TABLE_VIEW)
     *  style->apply(QWidget*)
     * @param style
     */
    explicit EsStyleSheet(Es::StyleSheet style);
    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

    Es::StyleSheet style;
};

// 样式表文件类
class ES_EXPORT EsStyleSheetFile : public EsStyleSheetBase
{
public:
    /**
     * @brief 样式表文件类构造函数
     *
     * @param path 样式表文件路径
     */
    explicit EsStyleSheetFile(const QString& path);

    /**
     * @brief 获取样式表的路径
     *
     * @param theme 主题
     * @return QString 返回样式表路径
     */
    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

private:
    QString filePath; // 样式表文件路径
};

// CustomStyleSheet 类
class ES_EXPORT EsCustomStyleSheet : public EsStyleSheetBase
{
public:
    // 常量键名
    static constexpr const char* DARK_QSS_KEY = "darkCustomQss";
    static constexpr const char* LIGHT_QSS_KEY = "lightCustomQss";

    /**
     * @brief 构造函数，初始化 CustomStyleSheet
     *
     * @param widget QWidget 对象
     */
    explicit EsCustomStyleSheet(QWidget* widget);

    /**
     * @brief 获取样式表的路径
     *
     * @param theme 主题
     * @return QString 返回空字符串
     */
    QString path(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

    /**
     * @brief 获取样式表的内容
     *
     * @param theme 主题, 默认 Es::Theme::AUTO
     * @return QString 返回样式表内容
     */
    QString content(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

    /**
     * @brief 获取 widget 属性
     *
     * @return QWidget* 返回 widget 对象
     */
    QWidget* widget() const;

    /**
     * @brief 比较两个 CustomStyleSheet 对象
     *
     * @param other 另一个对象
     * @return bool 是否相等
     */
    bool operator==(const EsCustomStyleSheet& other) const
    {
        return this->widget() == other.widget();
    }

    /**
     * @brief 设置自定义样式表
     *
     * @param lightQss Light 模式下的样式表
     * @param darkQss Dark 模式下的样式表
     * @return CustomStyleSheet* 当前对象指针
     */
    EsCustomStyleSheet* setCustomStyleSheet(const QString& lightQss, const QString& darkQss);

    /**
     * @brief 设置 Light 模式下的样式表
     *
     * @param qss 样式表
     * @return CustomStyleSheet* 当前对象指针
     */
    EsCustomStyleSheet* setLightStyleSheet(const QString& qss);

    /**
     * @brief 设置 Dark 模式下的样式表
     *
     * @param qss 样式表
     * @return CustomStyleSheet* 当前对象指针
     */
    EsCustomStyleSheet* setDarkStyleSheet(const QString& qss);

    /**
     * @brief 获取 Light 模式下的样式表
     *
     * @return QString 返回样式表
     */
    QString lightStyleSheet() const;

    /**
     * @brief 获取 Dark 模式下的样式表
     *
     * @return QString 返回样式表
     */
    QString darkStyleSheet() const;



private:
    QWidget* _widget = nullptr;
};

// 自定义样式表观察器
class ES_EXPORT EsCustomStyleSheetWatcher : public QObject
{
    Q_OBJECT

public:
    explicit EsCustomStyleSheetWatcher(QObject* parent = nullptr);

    /**
     * @brief 事件过滤器，用于处理样式表变化事件
     *
     * @param obj 事件对象
     * @param e 事件
     * @return bool 返回事件是否被处理
     */
    bool eventFilter(QObject* obj, QEvent* e) override;
};

// DirtyStyleSheetWatcher 类
class ES_EXPORT EsDirtyStyleSheetWatcher : public QObject
{
    Q_OBJECT

public:
    explicit EsDirtyStyleSheetWatcher(QObject* parent = nullptr);

    /**
     * @brief 事件过滤器，用于处理样式表的脏标记事件
     *
     * @param obj 事件对象
     * @param e 事件
     * @return bool 返回事件是否被处理
     */
    bool eventFilter(QObject* obj, QEvent* e) override;
};

// StyleSheetCompose 类
class ES_EXPORT EsStyleSheetCompose : public EsStyleSheetBase
{
public:
    /**
     * @brief 样式表组合类构造函数
     *
     * @param sources 样式表源列表
     */
    explicit EsStyleSheetCompose(const QList<EsStyleSheetBase*>& sources);
    ~EsStyleSheetCompose() override;

    /**
     * @brief 获取组合后的样式表内容
     *
     * @param theme 主题, 默认 EsThemes::AUTO
     * @return QString 返回组合后的样式表内容
     */
    QString content(Es::Theme theme = Es::Theme::Theme_AUTO) const override;

    /**
     * @brief 添加样式表源
     *
     * @param source 样式表源
     */
    void add(EsStyleSheetBase* source);

    /**
     * @brief 移除样式表源
     *
     * @param source 样式表源
     */
    void remove(EsStyleSheetBase* source);

    void removeAll();


private:
    QList<EsStyleSheetBase*> sources; // 存储样式表源列表
};
// Q_DECLARE_METATYPE(EsStyleSheetCompose*)

namespace EsFunc
{
    /**
     * @brief 应用主题颜色
     * 将主题颜色应用到样式表
     * @param qss 样式表字符串，包含待替换的主题颜色变量
     * @return QString 返回替换后的样式表
     */
    QString applyThemeColor(const QString& qss);


    /**
     * @brief 渲染字体和主题颜色到样式表中
     * @param qss 样式表字符串，用于应用主题颜色。被替换的变量应与 `ThemeColor` 的值相同，
     *            并以 `--` 开头，例如 `--ThemeColorPrimary`
     * @return QString 返回应用主题颜色后的样式表内容
     */
    QString renderQss(const QString& qss);


    /**
     * @brief 从文件获取样式表
     *
     * @param file 样式表文件路径或 QFile 对象
     * @return QString 返回样式表内容
     */
    QString getStyleSheetFromFile(const QString& file);
    QString getStyleSheetFromFile(QFile& file);

    /**
     * @brief 获取样式表
     *
     * @param source 样式表源，可能是文件路径或 StyleSheetBase* 指针
     * @param theme 主题
     * @return QString 返回渲染后的样式表
     */
    QString getStyleSheet(const QVariant& source, Es::Theme theme = Es::Theme::Theme_AUTO);
    QString getStyleSheet(const QString& source, Es::Theme theme = Es::Theme::Theme_AUTO);
    QString getStyleSheet(EsStyleSheetBase* source, Es::Theme theme = Es::Theme::Theme_AUTO);

    /**
     * @brief 设置 QWidget 的样式表
     *
     * @param widget 要设置样式表的 QWidget
     * @param source 样式表源，可能是文件路径或 StyleSheetBase 对象
     * @param theme 主题
     * @param register_ 是否注册控件到样式表管理器
     */
    void setStyleSheet(QWidget* widget, const QVariant &source, Es::Theme theme = Es::Theme::Theme_AUTO, bool register_ = true);
    void setStyleSheet(QWidget* widget, const QString &source, Es::Theme theme = Es::Theme::Theme_AUTO, bool register_ = true);
    void setStyleSheet(QWidget* widget, EsStyleSheetBase* source, Es::Theme theme = Es::Theme::Theme_AUTO, bool register_ = true);



    /**
     * @brief 设置自定义样式表
     *
     * @param widget 控件
     * @param lightQss Light 模式下的样式表
     * @param darkQss Dark 模式下的样式表
     */
    void setCustomStyleSheet(QWidget* widget, const QString& lightQss, const QString& darkQss);

    /**
     * @brief 向控件添加样式表
     *
     * @param widget 控件
     * @param source 样式表源，可以是文件路径或StyleSheetBase*
     * @param theme 主题
     * @param register 是否注册控件到样式表管理器
     */
    void addStyleSheet(QWidget* widget, const QVariant &source, Es::Theme theme = Es::Theme::Theme_AUTO,
                       bool register_ = true);

    /**
     * @brief 更新所有控件的样式表
     *
     * @param lazy 是否延迟更新样式表，如果为 true，会加速主题切换
     */
    void updateStyleSheet(bool lazy = false);


    /**
     * @brief 设置应用程序的主题
     *
     * @param theme 主题模式
     * @param save 是否保存更改到配置文件
     * @param lazy 是否延迟更新样式表
     */
    void setTheme(Es::Theme theme, bool save = false, bool lazy = false);

    /**
     * @brief 切换应用程序的主题
     *
     * @param save 是否保存更改到配置文件
     * @param lazy 是否延迟更新样式表
     */
    void toggleTheme(bool save = false, bool lazy = false);

    // 获取当前主题颜色
    QColor themeColor();

    // 设置主题颜色
    void setThemeColor(const QColor& color, bool save = false, bool lazy = false);
}


class ES_EXPORT EsThemeColor
{
public:
    static QString getName(Es::ThemeColor color);


    static QColor getColor(Es::ThemeColor color);


};

#endif //ESSTYLESHEET_H
