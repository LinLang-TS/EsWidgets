/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esIcon.cpp
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esIcon.h"
#include <QIcon>
#include <QList>
#include <QColor>
#include <QFile>
#include <QDomDocument>
#include <QPainter>
#include <QPainterPath>
#include <QSvgRenderer>
#include <QMetaObject>
#include <QMetaEnum>

#include "esConfig.h"
#include "esStyleSheet.h"
#include "esUtils.h"


// region ================= EsIconEngine  =================


EsIconEngine::EsIconEngine(const QIcon &icon, bool reverse)
    : icon(icon), isThemeReversed(reverse)
{
}


EsIconEngine::EsIconEngine(EsIconBase* icon, bool reverse)
    : icon(QVariant::fromValue(icon)), isThemeReversed(reverse)
{
}

void EsIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
    painter->save();
    // 根据 mode 设置透明度
    if (mode == QIcon::Disabled)
    {
        painter->setOpacity(0.5);
    }
    else if (mode == QIcon::Selected)
    {
        painter->setOpacity(0.7);
    }

    // 根据主题更改图标颜色
    QIcon icon_;

    auto theme = (isThemeReversed) ? (EsFunc::isDarkTheme() ? Es::Theme::Theme_DARK : Es::Theme::Theme_LIGHT) : Es::Theme::Theme_AUTO;


    if (icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        icon_ = icon.value<EsIconBase*>()->icon(theme);
    }
    QRect adjustedRect = rect;
    if (rect.x() == 19)
    {
        adjustedRect = rect.adjusted(-1, 0, 0, 0);
    }

    icon_.paint(painter, adjustedRect, Qt::AlignCenter, QIcon::Normal, state);
    painter->restore();
}

QPixmap EsIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
    auto image = QImage(size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    auto pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);

    QPainter painter(&pixmap);
    auto rect = QRect(0, 0, size.width(), size.height());
    paint(&painter, rect, mode, state);
    return pixmap;
    //   return QIconEngine::pixmap(size, mode, state);
}

EsIconEngine* EsIconEngine::clone() const
{
    if (icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        return new EsIconEngine(icon.value<EsIconBase*>(), isThemeReversed);
    }
    return new EsIconEngine(icon.value<QIcon>(), isThemeReversed);
}

// endregion


// region ================= EsIconSvgEngine =================

EsSvgIconEngine::EsSvgIconEngine(const QString& svg)
    : svg(svg)
{
}

void EsSvgIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
    EsFunc::drawSvgIcon(svg.toUtf8(), painter, rect);
}

QPixmap EsSvgIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
    // 创建一个透明背景的 QImage
    QImage image(size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // 从 QImage 创建 QPixmap
    QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);

    // 使用 QPainter 绘制
    QPainter painter(&pixmap);
    QRect rect(0, 0, size.width(), size.height());
    paint(&painter, rect, mode, state);

    return pixmap;
    //  return QIconEngine::pixmap(size, mode, state);
}

EsSvgIconEngine* EsSvgIconEngine::clone() const
{
    return new EsSvgIconEngine(svg);
}

// endregion

// region ================= EsFontIconEngine =================

EsFontIconEngine::EsFontIconEngine(const QString& fontFamily, const QString& character, const QColor& color,
                                   bool isBold) : fontFamily(fontFamily), char_(character), color(color), isBold(isBold)
{
}

void EsFontIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
    QFont font(fontFamily);
    font.setBold(isBold);
    font.setPixelSize(qRound((float)rect.height())); // 设置字体大小为矩形高度
    painter->setFont(font);
    painter->setPen(Qt::NoPen); // 不使用边框
    painter->setBrush(color); // 设置字体颜色
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 开启抗锯齿

    QPainterPath path;
    path.addText(rect.x(), rect.y() + rect.height(), font, char_); // 将文本添加到路径中
    painter->drawPath(path); // 绘制路径
}

EsFontIconEngine* EsFontIconEngine::clone() const
{
    return new EsFontIconEngine(fontFamily, char_, color, isBold);
}

QPixmap EsFontIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
    // 创建一个透明的 QImage
    QImage image(size, QImage::Format_ARGB32);
    image.fill(Qt::transparent); // 填充透明背景

    // 从 QImage 创建 QPixmap
    QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);

    // 使用 QPainter 绘制图标
    QPainter painter(&pixmap);
    QRect rect(0, 0, size.width(), size.height());
    paint(&painter, rect, mode, state); // 绘制图标

    return pixmap;
}

// endregion

// region ================= EsFunc =================

QString EsFunc::getIconColor(Es::Theme theme, bool reverse)
{
    QString lightColor = reverse ? "white" : "black";  // 浅色模式下的颜色
    QString darkColor = reverse ? "black" : "white";   // 深色模式下的颜色

    // 根据主题选择颜色
    if (theme == Es::Theme::Theme_AUTO)
    {
        return isDarkTheme() ? darkColor : lightColor;
    }
    return theme == Es::Theme::Theme_DARK ? darkColor : lightColor;
}

void EsFunc::drawSvgIcon(const QVariant& icon, QPainter* painter, const QRectF& rect)
{
    QSvgRenderer renderer;

    if (icon.type() == QVariant::String)
    {
        // 如果是路径（QString）
        QString str = icon.toString().trimmed();
        // 判断是否是 SVG 内容（而不是路径）
        if (str.startsWith("<svg", Qt::CaseInsensitive) || str.startsWith("<?xml", Qt::CaseInsensitive))
            renderer.load(str.toUtf8());// 是内联 SVG 文本 → 转为 QByteArray 加载
        else
            renderer.load(str); // 是文件路径（包括 :/ 资源路径）
    }
    else if (icon.type() == QVariant::ByteArray)
    {
        // 如果是二进制数据（QByteArray）
        renderer.load(icon.toByteArray());
    }

    // 渲染到指定区域
    renderer.render(painter, rect);
}

QString EsFunc::writeSvg(const QString& iconPath, const QList<int>& indexes,
                         std::initializer_list<std::pair<QString, QString>> attributes)
{
    if (!iconPath.toLower().endsWith(".svg"))
    {
        return ""; // 如果路径不是 SVG 文件，返回空字符串
    }

    QFile file(iconPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return ""; // 如果文件无法打开，返回空字符串
    }

    QDomDocument dom;
    if (!dom.setContent(&file))
    {
        file.close();
        return ""; // 如果 XML 解析失败，返回空字符串
    }

    file.close();

    // 获取所有的 <path> 元素
    QDomNodeList pathNodes = dom.elementsByTagName("path");

    // 如果没有传递索引，则修改所有路径
    QList<int> pathIndexes = indexes.isEmpty() ? QList<int>{0, pathNodes.length() - 1} : indexes;

    for (int i : pathIndexes)
    {
        if (i < 0 || i >= pathNodes.length())
        {
            continue; // 如果索引无效，跳过
        }

        QDomElement element = pathNodes.at(i).toElement();

        // 设置属性
        for (const auto& pair : attributes)
        {
            element.setAttribute(pair.first, pair.second);
        }
    }

    // 返回修改后的 SVG 字符串
    QString result;
    QTextStream stream(&result);
    dom.save(stream, 4); // 使用 4 个空格缩进保存文件内容
    return result;
}


void EsFunc::drawIcon(const QVariant& variant, QPainter* painter, const QRectF& rect, QIcon::State state, const QList<int>& indexes,
    std::initializer_list<std::pair<QString, QString>> attributes)
{
    if (variant.type() == QVariant::Icon)
    {
        auto icon = QIcon(variant.value<QIcon>());
        icon.paint(painter, QRectF(rect).toRect(), Qt::AlignCenter, QIcon::Normal, state);
    }
    else if (variant.userType() == qMetaTypeId<EsIconBase*>())
    {
        auto icon = variant.value<EsIconBase*>();
        icon->render(painter, rect, indexes, attributes);

    }
    else // if (variant.type() == QVariant::String)
    {
        auto icon = QIcon(variant.value<QString>());
        icon.paint(painter, QRectF(rect).toRect(), Qt::AlignCenter, QIcon::Normal, state);
    }
}


QIcon EsFunc::toQIcon(const QVariant& variant)
{
    if (variant.isNull() || !variant.isValid())
    {
        return {};
    }

    if (variant.type() == QVariant::String)
    {
        return QIcon(variant.value<QString>());
    }

    if (variant.type() == QVariant::Icon)
    {
        return variant.value<QIcon>();
    }

    return variant.value<EsIconBase*>()->icon();

}

QColor EsFunc::validColor(const QColor& color, const QColor& defaultColor) {
    return color.isValid() ? color : defaultColor;
}

QColor EsFunc::fallbackThemeColor(const QColor& color) {
    return color.isValid() ? color : themeColor();
}

QColor EsFunc::autoFallbackThemeColor(const QColor& light, const QColor& dark) {
    QColor color = isDarkTheme() ? dark : light;
    return fallbackThemeColor(color);
}


// endregion


// region ================= EsIconBase  =================

EsIconBase::EsIconBase(Es::IconName iconName)
    : iconName(iconName)
{
    iconNameStr = EsFunc::getEnumValueStr(iconName);
}
QString EsIconBase::path(Es::Theme theme) const
{
    qDebug() << "错误调用父类的 EsIconBase::path, 子类可能未重写,或签名错误没写 const override";
    return "EsIconBase::path";
}



QIcon EsIconBase::icon(Es::Theme theme, const QColor& color) const
{
    QString path = this->path(theme);

    if (!(path.endsWith(".svg"))) // && color.isValid())) // 避雷: qt和pyqt不一样,不支持直接传svg路径,所以这里不能和py一样;
    {
        return QIcon(this->path(theme));
    }

    QString colorName = color.name();
    return QIcon(new EsSvgIconEngine(EsFunc::writeSvg(path, QList<int>(), {{"fill", colorName}})));


}

EsColoredIcon EsIconBase::colored(const QColor& lightColor, const QColor& darkColor)
{
    return EsColoredIcon(iconName, lightColor, darkColor);
}

QIcon EsIconBase::qicon(bool reverse)
{
    return QIcon(new EsIconEngine(this, reverse));
}

void EsIconBase::render(QPainter* painter, const QRectF& rect, Es::Theme theme, const QList<int>& indexes,
                              std::initializer_list<std::pair<QString, QString>> attributes)
{
    QString iconPath = path(theme);

    if (iconPath.endsWith(".svg"))
    {
        if (attributes.size() != 0)
        {
            iconPath = EsFunc::writeSvg(iconPath, indexes, attributes).toUtf8();
        }
        EsFunc::drawSvgIcon(iconPath, painter, rect);
    }
    else
    {
        QIcon icon(iconPath);
        QRect rectToDraw = rect.toRect();
        painter->drawPixmap(rectToDraw, icon.pixmap(rectToDraw.size()));
    }
}

void EsIconBase::render(QPainter* painter, const QRectF& rect, const QList<int>& indexes,
    std::initializer_list<std::pair<QString, QString>> attributes)
{
    render(painter, rect, Es::Theme::Theme_AUTO, indexes, attributes);
}

void EsIconBase::render(QPainter* painter, const QRectF& rect,
                        std::initializer_list<std::pair<QString, QString>> attributes)
{
    render(painter, rect, Es::Theme::Theme_AUTO, QList<int>(), attributes);
}

Es::IconName EsIconBase::getIconName() const
{
    return iconName;
}

QString EsIconBase::getIconNameStr() const
{
    return iconNameStr;
}


// endregion


// region ================= EsColoredIcon  =================

EsColoredIcon::EsColoredIcon(Es::IconName iconName, const QString& lightColor,
                                         const QString& darkColor)
    : EsIconBase(iconName), lightColor(lightColor), darkColor(darkColor)
{
}

EsColoredIcon::EsColoredIcon(Es::IconName iconName, const QColor& lightColor,
                                         const QColor& darkColor)
    : EsIconBase(iconName)
{
}

QString EsColoredIcon::path(Es::Theme theme) const
{
    return "";
}

// endregion

// region ================= EsIcon  =================

QString EsIcon::path(Es::Theme theme) const
{
    // const QMetaObject& metaObj = Es::Theme::staticMetaObject;
    // const QMetaEnum metaEnum = metaObj.enumerator(metaObj.indexOfEnumerator("Theme")); // 获取枚举的元信息

    // 将枚举值转换为对应的字符串
    // auto themeStr = metaEnum.valueToKey(theme);


    return QString(":/esWidgets/images/icons/%1_%2.svg")
           .arg(iconNameStr, EsFunc::getIconColor(theme));
}

// endregion

// region ================= EsCheckBoxIcon  =================

QString EsCheckBoxIcon::path(Es::Theme theme) const
{
    return QString(":/esWidgets/images/check_box/%1_%2.svg").arg(iconNameStr, EsFunc::getIconColor(theme, true));
}
// endregion



// region ================= InfoBarIcon  =================

QString EsInfoBarIcon::path(Es::Theme theme) const
{
    QString color;
    if (theme == Es::Theme::Theme_AUTO)
    {
        color = EsFunc::isDarkTheme() ? "dark" : "light";
    }
    else
    {
        color = EsFunc::getEnumValueStr(theme).toLower();
    }

    return QString(":/esWidgets/images/info_bar/%1_%2.svg").arg(iconNameStr, color);
}

EsAction::EsAction(QObject* parent)
    :QAction(parent)
{
}

EsAction::EsAction(const QString& text, QObject* parent)
    :QAction(text, parent)
{
}

EsAction::EsAction(const QIcon& icon, const QString& text, QObject* parent)
    :QAction(icon, text, parent)
{
}

EsAction::EsAction(EsIconBase* icon, const QString& text, QObject* parent)
    :QAction(icon->icon(), text, parent)
{
    _icon = QVariant::fromValue(icon);
}

QIcon EsAction::icon() const
{

    if (_icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        return _icon.value<EsIconBase*>()->icon(); // 避雷: qt和pyqt不一样,不支持直接传svg路径, 所以这里得用icon复制
    }
    return QAction::icon();
}

void EsAction::setIcon(const QVariant& icon)
{
    _icon = icon;
    if (icon.userType() == qMetaTypeId<EsIconBase*>())
    {
        QAction::setIcon(icon.value<EsIconBase*>()->icon());
        return;
    }
    QAction::setIcon(icon.value<QIcon>());
}

// endregion
