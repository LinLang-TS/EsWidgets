/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esStyleSheet.cpp
** @date 2025/10/6
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esStyleSheet.h"
#include "esUtils.h"
#include <QEvent>
#include <QMetaEnum>
#include <QWidget>
// region ================= EsStyleSheetManager =================

E_SINGLETON_CREATE_CPP(EsStyleSheetManager)

EsStyleSheetManager::EsStyleSheetManager(QObject* parent)
    : QObject(parent)
{
}

EsStyleSheetManager::~EsStyleSheetManager()
{

}

/**
 * @brief 注册小部件到管理器
 * @param source QSS 源，可以是：
 *        - QString：QSS 文件路径
 *        - StyleSheetBase*：样式表实例指针
 * @param widget 要设置样式表的小部件
 * @param reset 是否重置 QSS 源
 */
void EsStyleSheetManager::registerWidget(const QVariant& source, QWidget* widget, bool reset)
{
    EsStyleSheetBase* qssSource = nullptr;
    if (source.type() == QVariant::String)
    {
        qssSource = new EsStyleSheetFile(source.toString());
    }
    else
    {
        qssSource = source.value<EsStyleSheetBase*>();
    }

    if (!widgets.contains(widget))
    {
        connect(widget, &QObject::destroyed, [this, widget]()
        {
            deregister(widget);
        });
        widget->installEventFilter(new EsCustomStyleSheetWatcher(widget));
        widget->installEventFilter(new EsDirtyStyleSheetWatcher(widget));
        // new EsStyleSheetCompose 由Manager的deregister方法管理
        // new EsCustomStyleSheet(widget)这个指针由EsStyleSheetCompose析构函数内部管理
        // qssSource 这个指针由信号回调deregister方法时管理删除
        widgets[widget] = new EsStyleSheetCompose({qssSource, new EsCustomStyleSheet(widget)});

    }

    if (!reset)
    {
        this->source(widget)->add(qssSource);
    }
    else
    {
        // 重置样式组合, 释放原来的内存, 创建新的
        delete widgets[widget];
        widgets[widget] = new EsStyleSheetCompose({qssSource, new EsCustomStyleSheet(widget)});
    }
}

/**
  * @brief 解除注册小部件
  * 从管理器中移除小部件。
  * @param widget 要解除注册的小部件
 */
void EsStyleSheetManager::deregister(QWidget* widget)
{
    if (!widgets.contains(widget))
    {
        return;
    }
    widgets[widget]->removeAll();
    delete widgets[widget];
    widgets.remove(widget);
}

QMap<QWidget*, EsStyleSheetCompose*> EsStyleSheetManager::items() const
{
    return widgets;
}

/**
 * @brief 获取小部件的 QSS 源
 * @param widget 要查询的小部件
 * @return StyleSheetCompose 返回小部件的样式表，如果没有找到则返回空样式表
 */
EsStyleSheetCompose* EsStyleSheetManager::source(QWidget* widget) const
{
    return widgets.value(widget, nullptr);
}


// endregion


// region ================= QssTemplate  =================

/**
 * @brief 样式表模板类
 * 用于生成 QSS 模板，提供替换功能。
 * @param qss 输入的 QSS 模板字符串
 */
QssTemplate::QssTemplate(const QString& qss) : qssTemplate(qss)
{
}

/**
 * @brief 替换模板中的变量
 *
 * @param mappings 变量映射，键为占位符(qss里`--`开头的变量)，值为替换内容
 * @return QString 替换后的 QSS 字符串
 */
QString QssTemplate::safeSubstitute(const QMap<QString, QString>& mappings) const
{
    QString result = qssTemplate;
    for (auto it = mappings.begin(); it != mappings.end(); ++it)
    {
        result.replace("--" + it.key(), it.value()); // 替换 --变量名 为 对应值
    }
    return result;
}

// endregion


// region ================= EsStyleSheetBase  =================


QString EsStyleSheetBase::path(Es::Theme theme) const
{
    qDebug() << "错误调用父类的 EsStyleSheetBase::path, 子类可能未重写,或签名错误没写 const override";
    return "EsStyleSheetBase::path";
}

/**
 * @brief 获取样式表的内容
 *
 * @param theme 主题, 默认 Es::Theme::AUTO
 * @return QString 返回样式表内容
 */
QString EsStyleSheetBase::content(Es::Theme theme) const
{
    return EsFunc::getStyleSheetFromFile(this->path(theme));
}

/**
 * @brief 将样式表应用到 QWidget 上
 *
 * @param widget 要应用样式表的小部件
 * @param theme 主题
 */
void EsStyleSheetBase::apply(QWidget* widget, Es::Theme theme)
{
    EsFunc::setStyleSheet(widget, this, theme);
}

// endregion


// region ================= EsStyleSheet  =================

EsStyleSheet::EsStyleSheet(Es::StyleSheet style)
    : style(style)
{
}

QString EsStyleSheet::path(Es::Theme theme) const
{
    if (theme == Es::Theme::Theme_AUTO)
    {
        theme = esConfig.getTheme();
    }

    // 使用 QMetaEnum 获取主题的字符串
    QString themeValue = EsFunc::getEnumValueStr(theme); // 转换枚举为字符串
    QString styleValue = EsFunc::getEnumValueStr(style);
    return QString(":/esWidgets/qss/%1/%2.qss").arg(themeValue.toLower(), styleValue.toLower());
}


// endregion


// region ================= EsStyleSheetFile  =================

EsStyleSheetFile::EsStyleSheetFile(const QString& path)
    : filePath(path)
{
}

/**
 * @brief 获取样式表的路径
 *
 * @param theme 主题
 * @return QString 返回样式表路径
 */
QString EsStyleSheetFile::path(Es::Theme theme) const
{
    return filePath;
}

// endregion


// region ================= EsCustomStyleSheet  =================

EsCustomStyleSheet::EsCustomStyleSheet(QWidget* widget)
    : _widget(widget)
{
}

QString EsCustomStyleSheet::path(Es::Theme theme) const
{
    return "";
}

QString EsCustomStyleSheet::content(Es::Theme theme) const
{
    if (theme == Es::Theme::Theme_AUTO)
    {
        theme = esConfig.getTheme();
    }

    if (theme == Es::Theme::Theme_LIGHT)
    {
        return lightStyleSheet();
    }
    return darkStyleSheet();
}


QWidget* EsCustomStyleSheet::widget() const
{
    return _widget;
}

EsCustomStyleSheet* EsCustomStyleSheet::setCustomStyleSheet(const QString& lightQss, const QString& darkQss)
{
    setLightStyleSheet(lightQss);
    setDarkStyleSheet(darkQss);
    return this;
}

EsCustomStyleSheet* EsCustomStyleSheet::setLightStyleSheet(const QString& qss)
{
    if (widget())
    {
        widget()->setProperty(LIGHT_QSS_KEY, qss);
    }
    return this;
}

EsCustomStyleSheet* EsCustomStyleSheet::setDarkStyleSheet(const QString& qss)
{
    if (widget())
    {
        widget()->setProperty(DARK_QSS_KEY, qss);
    }
    return this;
}

QString EsCustomStyleSheet::lightStyleSheet() const
{
    if (!widget()) return "";
    return widget()->property(LIGHT_QSS_KEY).toString();
}

QString EsCustomStyleSheet::darkStyleSheet() const
{
    if (!widget()) return "";
    return widget()->property(DARK_QSS_KEY).toString();
}

// endregion

// region ================= EsIconBase  =================


EsCustomStyleSheetWatcher::EsCustomStyleSheetWatcher(QObject* parent)
    : QObject(parent)
{
}

bool EsCustomStyleSheetWatcher::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() != QEvent::DynamicPropertyChange)
    {
        return QObject::eventFilter(obj, e);
    }

    // 获取动态属性的名称
    const QDynamicPropertyChangeEvent* dynamicEvent = dynamic_cast<QDynamicPropertyChangeEvent*>(e);
    QByteArray propertyName = dynamicEvent->propertyName();

    // 如果属性名称是 LIGHT_QSS_KEY 或 DARK_QSS_KEY，则重新应用样式表
    if (propertyName == EsCustomStyleSheet::LIGHT_QSS_KEY || propertyName == EsCustomStyleSheet::DARK_QSS_KEY)
    {
        auto ptr = new EsCustomStyleSheet(qobject_cast<QWidget*>(obj)); // 会自动管理内存
        auto value = QVariant::fromValue((EsStyleSheetBase*)ptr);
        EsFunc::addStyleSheet(qobject_cast<QWidget*>(obj), value);
    }

    return QObject::eventFilter(obj, e);
}

// endregion


// region ================= EsDirtyStyleSheetWatcher  =================

EsDirtyStyleSheetWatcher::EsDirtyStyleSheetWatcher(QObject* parent)
    : QObject(parent)
{
}

bool EsDirtyStyleSheetWatcher::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() != QEvent::Paint || !obj->property("dirty-qss").toBool())
    {
        return QObject::eventFilter(obj, e);
    }

    obj->setProperty("dirty-qss", false);

    QWidget* widget = qobject_cast<QWidget*>(obj);
    if (widget && EsStyleSheetManager::getInstance()->widgets.contains(widget))
    {
        widget->setStyleSheet(EsFunc::getStyleSheet(EsStyleSheetManager::getInstance()->source(widget)));
    }

    return QObject::eventFilter(obj, e);
}

// endregion


// region ================= EsStyleSheetCompose  =================

EsStyleSheetCompose::EsStyleSheetCompose(const QList<EsStyleSheetBase*>& sources)
    : sources(sources)
{
}

EsStyleSheetCompose::~EsStyleSheetCompose()
{

    // 只管理列表里EsCustomStyleSheet*的内存释放, 剩下的EsStyleSheetBase*由函数removeAll统一删除
    for (int i=sources.size()-1; i>=0; i--)
    {
        if (dynamic_cast<EsCustomStyleSheet*>(sources[i]))
        {
            delete sources[i];
        }

    }
}


QString EsStyleSheetCompose::content(Es::Theme theme) const
{
    QStringList contentList;
    for (EsStyleSheetBase* source : sources)
    {

        contentList.append(source->content(theme));
    }
    return contentList.join("\n");
}

void EsStyleSheetCompose::add(EsStyleSheetBase* source)
{
    if (source == this || sources.contains(source))
    {
        return;
    }
    sources.append(source);
}

void EsStyleSheetCompose::remove(EsStyleSheetBase* source)
{
    int idx = sources.indexOf(source);
    delete sources[idx];
    sources.removeAt(idx);
}

void EsStyleSheetCompose::removeAll()
{
    for (int i=sources.size()-1; i>=0; i--)
    {
        delete sources[i];
    }
    sources.clear();
}


// endregion


// region ================= EsFunc  =================

/**
 * @brief 应用主题颜色
 * 将主题颜色应用到样式表
 * @param qss 样式表字符串，包含待替换的主题颜色变量
 * @return QString 返回替换后的样式表
 */
QString EsFunc::applyThemeColor(const QString& qss)
{
    QssTemplate templateObj(qss);

    // 定义主题颜色映射
    QMap<QString, QString> mappings;
    mappings["ThemeColorPrimary"] = "#FF5733"; // 示例颜色值
    mappings["ThemeColorSecondary"] = "#33FF57";

    return templateObj.safeSubstitute(mappings);
}

/**
 * @brief 渲染 QSS，包括字体和主题颜色
 * 渲染字体和主题颜色到样式表
 * @param qss 样式表字符串，包含待替换的主题颜色和字体系列变量
 * @return QString 返回渲染后的样式表
 */
QString EsFunc::renderQss(const QString& qss)
{
    QssTemplate templateObj(qss);

    QMap<QString, QString> mappings = {
        // {"ThemeColorPrimary", "#009faa"},
        // {"ThemeColorDark1", "#007780"},
        // {"ThemeColorDark2", "#005055"},
        // {"ThemeColorDark3", "#004044"},
        // {"ThemeColorLight1", "#00a7b3"},
        // {"ThemeColorLight2", "#2daab3"},
        // {"ThemeColorLight3", "#3eabb3"},
        // {"FontFamilies", "'Segoe UI','Microsoft YaHei','PingFang SC'"}


    };
    QMetaEnum metaEnum = QMetaEnum::fromType<Es::ThemeColor>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        auto e = static_cast<Es::ThemeColor>(metaEnum.value(i));
        mappings[EsFunc::getEnumValueStr(e)] = EsThemeColor::getName(e);

    }

    // 添加字体系列的替换
    QStringList fontFamilies = esConfig.get(&EsConfig::fontFamilies).toStringList();
    mappings["FontFamilies"] = "'" + fontFamilies.join("', '") + "'"; // 转为字符串格式
    // qDebug() << templateObj.safeSubstitute(mappings); // 输出替换后的qss
    // qDebug() << mappings;

    return templateObj.safeSubstitute(mappings); // 替换qss里的`--`开头的主题变量,并返回
}

QString EsFunc::getStyleSheetFromFile(const QString& file)
{
    QFile f(file);
    if (f.open(QFile::ReadOnly))
    {
        QTextStream in(&f);
        return in.readAll();
    }
    return "";
}

QString EsFunc::getStyleSheetFromFile(QFile& file)
{
    if (file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}

QString EsFunc::getStyleSheet(const QString& source, Es::Theme theme)
{
    auto source_ = EsStyleSheetFile(source);
    return renderQss(source_.content(theme));
}

QString EsFunc::getStyleSheet(EsStyleSheetBase* source, Es::Theme theme)
{
    return renderQss(source->content(theme));
}


QString EsFunc::getStyleSheet(const QVariant& source, Es::Theme theme)
{
    if (source.canConvert<QString>())
    {
        getStyleSheet(source.toString(), theme);
    }
    return getStyleSheet(source.value<EsStyleSheetBase*>());
}


void EsFunc::setStyleSheet(QWidget* widget, const QVariant& source, Es::Theme theme, bool register_)
{
    if (register_)
    {
        EsStyleSheetManager::getInstance()->registerWidget(source, widget);
    }
    widget->setStyleSheet(getStyleSheet(source, theme));
}


void EsFunc::setStyleSheet(QWidget* widget, const QString& source, Es::Theme theme, bool register_)
{
    if (register_)
    {
        EsStyleSheetManager::getInstance()->registerWidget(QVariant::fromValue(source), widget);
    }

    widget->setStyleSheet(getStyleSheet(source, theme));
}

void EsFunc::setStyleSheet(QWidget* widget, EsStyleSheetBase* source, Es::Theme theme, bool register_)
{
    if (register_)
    {
        EsStyleSheetManager::getInstance()->registerWidget(QVariant::fromValue(source), widget);
    }
    widget->setStyleSheet(getStyleSheet(source, theme));
}

void EsFunc::setCustomStyleSheet(QWidget* widget, const QString& lightQss, const QString& darkQss)
{
    EsCustomStyleSheet(widget).setCustomStyleSheet(lightQss, darkQss);

}

void EsFunc::addStyleSheet(QWidget* widget,const QVariant& source, Es::Theme theme, bool register_)
{
    QString qss;
    if (register_)
    {
        auto styleSheetManager = EsStyleSheetManager::getInstance();

        styleSheetManager->registerWidget(source, widget, false);
        qss = getStyleSheet(styleSheetManager->source(widget), theme);
    }
    else
    {
        qss = widget->styleSheet() + "\n" + getStyleSheet(source, theme);
    }

    if (qss.trimmed() != widget->styleSheet().trimmed())
    {
        widget->setStyleSheet(qss);
    }
}

void EsFunc::updateStyleSheet(bool lazy)
{
    QList<QWidget*> removes;
    auto styleSheetManager = EsStyleSheetManager::getInstance();
    QList<QWidget*> widgets = styleSheetManager->items().keys(); // 避免Map在迭代期间更改了大小
    for (auto widget : widgets)
    {
        try
        {
            auto param = QVariant::fromValue((EsStyleSheetBase*)styleSheetManager->source(widget));
            if (!(lazy && widget->visibleRegion().isNull()))
            {
                setStyleSheet(widget, param, esConfig.getTheme());
            }
            else
            {
                styleSheetManager->registerWidget(param, widget);
                widget->setProperty("dirty-qss", true);
            }
        }
        catch (const std::runtime_error& e)
        {
            removes.append(widget);
        }
    }

    for (auto widget : removes)
    {
        styleSheetManager->deregister(widget);
    }
}

void EsFunc::setTheme(Es::Theme theme, bool save, bool lazy)
{

    esConfig.set(&EsConfig::themeMode, theme, save);
    updateStyleSheet(lazy);
    emit esConfig.themeChangedFinished();
}

void EsFunc::toggleTheme(bool save, bool lazy)
{
    auto theme =  isDarkTheme() ? Es::Theme::Theme_LIGHT  : Es::Theme::Theme_DARK;
    setTheme(theme, save, lazy);
}

QColor EsFunc::themeColor()
{
    return EsThemeColor::getColor(Es::ThemeColor::ThemeColorPrimary);
}

void EsFunc::setThemeColor(const QColor& color, bool save, bool lazy)
{
    esConfig.set(&EsConfig::themeColor, color, save);
    updateStyleSheet(lazy);
}


// endregion


// region ================= EsThemeColor  =================

QString EsThemeColor::getName(Es::ThemeColor color)
{
    // return EsFunc::getEnumValueStr(color); // 转换枚举为字符串
    return getColor(color).name();
}

QColor EsThemeColor::getColor(Es::ThemeColor color)
{

    QColor baseColor = esConfig.get(&EsConfig::themeColor).value<QColor>();
    // 转换颜色到 HSV 空间
    qreal h, s, v;
    baseColor.getHsvF(&h, &s, &v);

    if (EsFunc::isDarkTheme())
    {
        s *= 0.84;
        v = 1.0;
        if (color == Es::ThemeColor::ThemeColorDark1)
        {
            v *= 0.9;
        }
        else if (color == Es::ThemeColor::ThemeColorDark2)
        {
            s *= 0.977;
            v *= 0.82;
        }
        else if (color == Es::ThemeColor::ThemeColorDark3)
        {
            s *= 0.95;
            v *= 0.7;
        }
        else if (color == Es::ThemeColor::ThemeColorLight1)
        {
            s *= 0.92;
        }
        else if (color == Es::ThemeColor::ThemeColorLight2)
        {
            s *= 0.78;
        }
        else if (color == Es::ThemeColor::ThemeColorLight3)
        {
            s *= 0.65;
        }
    }
    else
    {
        if (color == Es::ThemeColor::ThemeColorDark1)
        {
            v *= 0.75;
        }
        else if (color == Es::ThemeColor::ThemeColorDark2)
        {
            s *= 1.05;
            v *= 0.5;
        }
        else if (color == Es::ThemeColor::ThemeColorDark3)
        {
            s *= 1.1;
            v *= 0.4;
        }
        else if (color == Es::ThemeColor::ThemeColorLight1)
        {
            v *= 1.05;
        }
        else if (color == Es::ThemeColor::ThemeColorLight2)
        {
            s *= 0.75;
            v *= 1.05;
        }
        else if (color == Es::ThemeColor::ThemeColorLight3)
        {
            s *= 0.65;
            v *= 1.05;
        }
    }

    return QColor::fromHsvF(h, qMin(s, 1.0), qMin(v, 1.0));
}

// endregion
