/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esConfig.cpp
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/


#include "esConfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include "Windows.h"

EsOptionsConfigItem<Es::Theme> EsConfig::themeMode = {
    "EsWidgets", "ThemeMode", QVariant::fromValue(Es::Theme::Theme_LIGHT), new EsOptionsValidator<Es::Theme>(),
    new EsEnumSerializer<Es::Theme>()
};
EsColorConfigItem EsConfig::themeColor = {"EsWidgets", "ThemeColor", "#009faa"};
EsConfigItem EsConfig::fontFamilies = {
    "EsWidgets", "FontFamilies", QVariant::fromValue(QStringList{"Segoe UI", "Microsoft YaHei", "PingFang SC"})
};


// QVariantMap → QJsonObject
QJsonValue variantToJson(const QVariant& variant)
{
    switch (variant.userType())
    {
    case QMetaType::QString:
        return variant.toString();
    case QMetaType::Bool:
        return variant.toBool();
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        return variant.toLongLong();
    case QMetaType::Double:
        return variant.toDouble();
    case QMetaType::QVariantList:
        {
            QJsonArray arr;
            for (const auto& v : variant.toList())
                arr.append(variantToJson(v));
            return arr;
        }
    case QMetaType::QVariantMap:
        {
            QJsonObject obj;
            for (auto it = variant.toMap().constBegin(); it != variant.toMap().constEnd(); ++it)
                obj[it.key()] = variantToJson(it.value());
            return obj;
        }
    default:
        // 其他类型转为字符串（如 QColor, QStringList 已被 serialize 转成 string）
        return variant.toString();
    }
}

QJsonDocument variantMapToJson(const QVariantMap& map)
{
    QJsonObject obj;
    for (auto it = map.constBegin(); it != map.constEnd(); ++it)
    {
        obj[it.key()] = variantToJson(it.value());
    }
    return QJsonDocument(obj);
}

// QJsonValue → QVariant（简化版，满足配置需求）
QVariant jsonToVariant(const QJsonValue& value)
{
    if (value.isString())
    {
        return value.toString();
    }
    else if (value.isBool())
    {
        return value.toBool();
    }
    else if (value.isDouble())
    {
        // 注意：JSON 没有 int，全是 double
        double d = value.toDouble();
        if (d == static_cast<qint64>(d))
            return static_cast<qint64>(d);
        return d;
    }
    else if (value.isArray())
    {
        QVariantList list;
        for (const QJsonValue& v : value.toArray())
            list << jsonToVariant(v);
        return list;
    }
    else if (value.isObject())
    {
        QVariantMap map;
        for (auto it = value.toObject().constBegin(); it != value.toObject().constEnd(); ++it)
            map[it.key()] = jsonToVariant(it.value());
        return map;
    }
    else
    {
        return QVariant(); // null
    }
}

QVariantMap jsonToVariantMap(const QJsonObject& obj)
{
    QVariantMap map;
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it)
    {
        map[it.key()] = jsonToVariant(it.value());
    }
    return map;
}

// 工具函数：将嵌套的 QVariantMap 展平为 "a.b.c" -> value 的形式
QVariantMap flattenMap(const QVariantMap& map, const QString& prefix = QString())
{
    QVariantMap flat;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        QString key = prefix.isEmpty() ? it.key() : prefix + "." + it.key();
        QVariant value = it.value();

        if (value.canConvert<QVariantMap>())
        {
            // 递归展开子对象
            QVariantMap subMap = flattenMap(value.toMap(), key);
            flat.insert(subMap);
        }
        else
        {
            flat[key] = value;
        }
    }
    return flat;
}


bool isDarkMode()
{
    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER,
                               "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                               0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS)
    {
        return false; // 默认返回浅色模式
    }

    DWORD value;
    DWORD dataSize = sizeof(DWORD);
    result = RegQueryValueEx(hKey, "AppsUseLightTheme", 0, NULL, (LPBYTE)&value, &dataSize);
    RegCloseKey(hKey);

    return (result == ERROR_SUCCESS && value == 0); // 0表示深色模式
}

// region ================= ConfigValidator  =================

bool EsConfigValidator::validate(const QVariant& value) const
{
    return true;
}

QVariant EsConfigValidator::correct(const QVariant& value) const
{
    return value;
}

// endregion

// region ================= RangeValidator  =================

EsRangeValidator::EsRangeValidator(int min, int max) : _min(min), _max(max), range(min, max)
{
}

bool EsRangeValidator::validate(const QVariant& value) const
{
    return _min <= value.toInt() && value.toInt() <= _max;
}

QVariant EsRangeValidator::correct(const QVariant& value) const
{
    return qMin(qMax(_min, value.toInt()), _max);
}

// endregion

// region ================= BoolValidator  =================

EsBoolValidator::EsBoolValidator()
    : EsOptionsValidator(boolOptions), boolOptions({true, false})
{
}

// endregion

// region ================= FolderValidator  =================

bool EsFolderValidator::validate(const QVariant& value) const
{
    QDir dir(value.toString());
    return dir.exists(); // 使用 QDir 来检查目录是否存在
}

QVariant EsFolderValidator::correct(const QVariant& value) const
{
    QDir dir(value.toString());

    // 如果路径不存在，则创建目录
    if (!dir.exists())
    {
        dir.mkpath("."); // mkpath 会创建多级目录
    }

    // 获取绝对路径
    QString absolutePath = dir.absolutePath();

    // 替换反斜杠为正斜杠
    absolutePath.replace("\\", "/");

    return absolutePath;
}

// endregion


// region ================= FolderListValidator  =================

bool EsFolderListValidator::validate(const QVariant& value) const
{
    for (auto& folder : value.toStringList())
    {
        QFileInfo folderInfo(folder);
        if (!folderInfo.exists() || !folderInfo.isDir())
        {
            return false; // 如果路径不存在或不是目录，返回 false
        }
    }
    return true; // 所有路径都有效
}

QVariant EsFolderListValidator::correct(const QVariant& value) const
{
    QStringList validFolders;

    for (const QString& folder : value.toStringList())
    {
        QFileInfo folderInfo(folder);
        if (folderInfo.exists() && folderInfo.isDir())
        {
            // 获取绝对路径并转换为字符串
            QString absolutePath = folderInfo.absoluteFilePath();

            // 替换反斜杠为正斜杠
            absolutePath.replace("\\", "/");
            validFolders.append(absolutePath);
        }
    }

    return validFolders;
}

// endregion

// region ================= ColorValidator  =================

EsColorValidator::EsColorValidator(const QVariant& color)
    : defaultColor(color)
{
}

bool EsColorValidator::validate(const QVariant& value) const
{
    return QColor(value.toString()).isValid();
}

QVariant EsColorValidator::correct(const QVariant& value) const
{
    return validate(value) ? value : defaultColor;
}

// endregion

// region ================= ConfigSerializer  =================

QString EsConfigSerializer::serialize(QVariant value)
{
    if (value.canConvert<QStringList>())
    {
        QJsonArray arr;
        for (auto& s : value.toStringList())
        {
            arr.append(s);
        }
        return QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact));
    }
    return value.toString();
}

QVariant EsConfigSerializer::deserialize(QVariant value)
{
    // 如果输入已经是 QStringList，直接返回（避免重复处理）
    if (value.canConvert<QStringList>() && value.userType() == QMetaType::QStringList)
    {
        return value;
    }

    // 否则尝试当作 JSON 字符串解析
    QString str = value.toString();
    if (str.isEmpty())
    {
        return QVariant::fromValue(QStringList{});
    }

    // 检查是否以 [ 开头、] 结尾，快速判断可能是 JSON 数组
    if (str.trimmed().startsWith('[') && str.trimmed().endsWith(']'))
    {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &err);

        if (err.error == QJsonParseError::NoError && doc.isArray())
        {
            QStringList list;
            for (const QJsonValue& v : doc.array())
            {
                if (v.isString())
                {
                    list << v.toString();
                }
                // 可选：忽略非字符串项，或报错
            }
            return QVariant::fromValue(list);
        }
    }

    // 如果不是有效 JSON 数组，按普通字符串处理
    // （或者根据需求 fallback 到 split(",")，但不推荐）
    return value;
}

// endregion

// region ================= ColorSerializer  =================

QString EsColorSerializer::serialize(QVariant value)
{
    return value.value<QColor>().name(QColor::HexArgb); // 返回 QColor 的十六进制 ARGB 表示
}

QVariant EsColorSerializer::deserialize(QVariant value)
{
    if (value.canConvert<QList<int>>())
    {
        auto v = value.value<QList<int>>();
        if (v.size() == 4)
        {
            return QColor(v[0], v[1], v[2], v[3]);
        }
        return QColor(v[0], v[1], v[2]);
    }
    return QColor(value.toString());
}

// endregion

// region ================= ConfigItem  =================

QList<EsConfigItem*>& EsConfigItem::allInstances()
{
    static QList<EsConfigItem*> instances;
    return instances;
}

EsConfigItem::EsConfigItem(const QString& group, const QString& name, const QVariant& defaultValue,
                       EsConfigValidator* validator, EsConfigSerializer* serializer, bool restart) : group(group),
    name(name),
    validator(validator ? validator : new EsConfigValidator()),
    serializer(serializer ? serializer : new EsConfigSerializer()),
    value(defaultValue),
    restart(restart),
    defaultValue(this->validator->correct(defaultValue))
{
    setValue(defaultValue);
    allInstances().append(this);
}

EsConfigItem::~EsConfigItem()
{
    allInstances().removeOne(this);
    delete validator;
    delete serializer;
}

QVariant EsConfigItem::getValue() const
{
    return this->value;
}

void EsConfigItem::setValue(QVariant v)
{
    v = validator->correct(v);
    QVariant ov = value;
    value = v;
    // qDebug() << "ov: " << ov << " v: " <<v << "bool: " << (ov != v);
    if (ov != v)
    {
        emit valueChanged(v);
    }
}

QString EsConfigItem::key() const
{
    return name.isEmpty() ? group : group + "." + name;
}

QString EsConfigItem::serialize()
{
    return serializer->serialize(this->value);
}

void EsConfigItem::deserializeFrom(QVariant& value_)
{
    auto v = serializer->deserialize(value_);
    this->setValue(v);
}

QString EsConfigItem::toString() const
{
    return QString("%1[value=%2]").arg(this->metaObject()->className()).arg(this->value.toString());
}

// endregion

// region ================= RangeConfigItem  =================

EsRangeConfigItem::EsRangeConfigItem(const QString& group, const QString& name, const QVariant& defaultValue,
                                 EsConfigValidator* validator, bool restart) : EsConfigItem(group,
    name,
    defaultValue,
    validator,
    nullptr,
    restart)
{
}

QPair<int, int> EsRangeConfigItem::range() const
{
    auto p = dynamic_cast<EsRangeValidator*>(validator);
    if (p)
    {
        return p->range;
    }
    qWarning() << "RangeConfigItem::range validator错误, 必须为RangeValidator*";
    return {0, 0};  // 返回默认范围
}

QString EsRangeConfigItem::toString() const
{
    auto p = dynamic_cast<EsRangeValidator*>(validator);
    if (p)
    {
        return QString("%1[range=(%2, %3), value=%4]")
               .arg(this->metaObject()->className())
               .arg(p->range.first)
               .arg(p->range.second)
               .arg(this->getValue().toString());
    }
    qWarning() << "RangeConfigItem::toString validator错误, 必须为RangeValidator*";
    return QString("%1[range=(0, 0), value=%2]")
       .arg(this->metaObject()->className())
       .arg(this->getValue().toString());

}

// endregion

// region ================= ColorConfigItem  =================

EsColorConfigItem::EsColorConfigItem(const QString& group, const QString& name, const QVariant& defaultValue,
                                 bool restart) : EsConfigItem(group,
                                                            name,
                                                            defaultValue,
                                                            new EsColorValidator(defaultValue),
                                                            new EsColorSerializer(),
                                                            restart)
{
}

QString EsColorConfigItem::toString() const
{
    return QString("%1[value=%2]").arg(this->metaObject()->className()).
                                   arg(this->getValue().value<QColor>().name());
}

// endregion

// region ================= EsConfig  =================

EsConfig::EsConfig()
    : cfgFile("config/config.json")
{
}

QVariant EsConfig::get(EsConfigItem* item)
{
    return item->getValue();
}

void EsConfig::set(EsConfigItem* item, QVariant newValue, bool save)
{
    if (item->getValue() == newValue) return;
    item->setValue(newValue);
    if (save) this->save();
    if (item->restart)
    {
        emit appRestartSig();
        if (item == &themeMode)
        {
            theme = newValue.value<Es::Theme>();
            emit themeChanged(theme);
        }
    }
    if (item == &themeColor)
    {
        emit themeColorChanged(newValue.value<QColor>());
    }
}


void EsConfig::load(const QString& file)
{
    // 1. 确定文件路径
    QString actualFile = file.isEmpty() ? cfgFile : file;
    QFile configFile(actualFile);

    QVariantMap loadedData;

    if (configFile.exists())
    {
        if (!configFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "无法打开配置文件:" << configFile.errorString();
            return;
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll(), &err);
        configFile.close();

        if (err.error != QJsonParseError::NoError || !doc.isObject())
        {
            qWarning() << "无效JSON:" << err.errorString();
            return;
        }

        // 2. 转为 QVariantMap 并展平
        loadedData = flattenMap(jsonToVariantMap(doc.object()));
    }
    // 如果文件不存在，loadedData 保持为空，后续用默认值

    // 3. 自动加载：遍历所有 ConfigItem，按 key 匹配
    for (EsConfigItem* item : getAllConfigItems())
    {
        // ← 获取所有配置项（见第2部分）
        QString k = item->key();
        if (loadedData.contains(k))
        {
            item->deserializeFrom(loadedData[k]);
        }
    }

    // 4. 同步 theme 等缓存值
    theme = get(&themeMode).value<Es::Theme>();

    if (!file.isEmpty())
    {
        this->cfgFile = actualFile;
    }
}

void EsConfig::save()
{
    // 1. 生成 QVariantMap（已序列化）
    QVariantMap data = toMap(true); // serialize = true

    // 2. 转为 QJsonDocument
    QJsonDocument doc = variantMapToJson(data);

    // 3. 写入文件
    QDir().mkpath(QFileInfo(cfgFile).absolutePath());
    QFile file(cfgFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(doc.toJson(QJsonDocument::Indented)); // Indented 可读性好
        file.close();
    }
}

Es::Theme EsConfig::getTheme()
{
    return theme;
}

void EsConfig::setTheme(Es::Theme t)
{
    if (t == Es::Theme::Theme_AUTO)
    {
        t = isDarkMode() ? Es::Theme::Theme_DARK : Es::Theme::Theme_LIGHT;
    }
    theme = t;
}

QVariantMap EsConfig::toMap(bool serialize)
{
    QVariantMap items;
    // 遍历所有配置项
    for (auto* item : getAllConfigItems())
    {
        QVariant value;
        if (serialize)
        {
            // 序列化：得到 QString（用于保存到文件）
            value = item->serialize(); // 注意：serialize() 返回 QString，会被隐式转为 QVariant
        }
        else
        {
            // 非序列化：保留原始类型
            value = item->getValue(); // 返回 QVariant（含 QColor / QStringList / Es::Theme 等）
        }
        // 判断是否存在这个组（group）
        if (!items.contains(item->group))
        {
            if (item->name.isEmpty())
            {
                items[item->group] = value;
            }
            else
            {
                items[item->group] = QVariantMap(); // 如果没有name，就初始化为map
            }
        }

        // 根据group和name设置值
        if (!item->name.isEmpty())
        {
            QVariantMap groupMap = items[item->group].toMap(); // 获取 group 对应的 QVariantMap
            groupMap[item->name] = value; // 修改 groupMap 中的项
            items[item->group] = groupMap; // 将修改后的 groupMap 放回 items 中
        }
        else
        {
            items[item->group] = value;
        }
    }
    // qDebug() << "================配置map=======================";
    // if (serialize)
    // {
    //     // 输出最终的 items
    //     for (auto it = items.begin(); it != items.end(); ++it)
    //     {
    //         // 如果值是 QVariantMap，递归输出
    //         if (it.value().canConvert<QVariantMap>())
    //         {
    //             qDebug() << it.key() << ": {";
    //             QVariantMap subMap = it.value().toMap();
    //             for (auto subIt = subMap.begin(); subIt != subMap.end(); ++subIt)
    //             {
    //                 qDebug() << "  " << subIt.key() << ": " << subIt.value().toString();
    //             }
    //             qDebug() << "}";
    //         }
    //         else
    //         {
    //             // 输出单值
    //             qDebug() << it.key() << ": " << it.value().toString();
    //         }
    //     }
    //
    // }
    // else
    // {
    //     qDebug() << items;
    // }

    return items;
}

QList<EsConfigItem*> EsConfig::getAllConfigItems()
{
    return EsConfigItem::allInstances();
}

// endregion


// region ================= EsFunc  =================

bool EsFunc::isDarkTheme()
{
    return esConfig.theme == Es::Theme::Theme_DARK;
}

bool EsFunc::getCurrentTheme()
{
    return esConfig.theme;
}

bool EsFunc::isDarkThemeMode(Es::Theme theme)
{
    return theme != Es::Theme::Theme_AUTO ? theme == Es::Theme::Theme_DARK : isDarkTheme();
}

// endregion
