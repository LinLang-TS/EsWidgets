/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esConfig.h
** @date 2025/10/4
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESCONFIG_H
#define ES_ESCONFIG_H
#include <filesystem>
#include <map>
#include <QColor>
#include <QString>
#include <vector>
#include <QDir>
#include <QMetaEnum>

#include "esDef.h"


class ES_EXPORT EsConfigValidator
{
public:
    // 构造函数
    EsConfigValidator() = default;
    virtual ~EsConfigValidator() = default;

    /**
    * @brief 验证值是否合法
    *
    * @param value 要验证的值
    * @return true(值合法) || false(值非法)
    */
    virtual bool validate(const QVariant& value) const;

    /**
     * @brief 修正非法值
     *
     * @param value 需要修正的值
     * @return QVariant 修正后的值
     */
    virtual QVariant correct(const QVariant& value) const;
};

class ES_EXPORT EsRangeValidator : public EsConfigValidator
{
public:
    // 构造函数，初始化最小值和最大值
    EsRangeValidator(int min, int max);

    /**
     * @brief 验证值是否在范围内
     *
     * @param value 要验证的值
     * @return true(在范围内) || false(不在范围内)
     */
    bool validate(const QVariant& value) const override;

    /**
     * @brief 修正不在范围内的值
     *
     * @param value 需要修正的值
     * @return QVariant 修正后的值
     */
    QVariant correct(const QVariant& value) const override;

public:
    QPair<int, int> range; ///< 范围

private:
    int _min; ///< 最小值
    int _max; ///< 最大值
};


template <typename T>
class ES_EXPORT EsOptionsValidator : public EsConfigValidator
{
public:

    /// 构造函数，初始化枚举值
    EsOptionsValidator()
    {
        if constexpr (std::is_enum_v<T>)
        {
            // 用qt的反射往列表存所有的枚举值, QVariant(enum, "enumValue"), 然后就可以用是否包含判断了
            QMetaEnum metaEnum = QMetaEnum::fromType<T>();
            for (int i = 0; i < metaEnum.keyCount(); ++i)
            {
                optionsList.append(metaEnum.value(i));
            }
        }
    }

    /**
     * @brief 构造函数，只支持容器(因为枚举类型不能用引用)
     *
     * @param options Qt容器类型的选项列表
     */
    EsOptionsValidator(const T& options)
    {
        // 如果是容器类型（例如 QList），直接传递
        if (options.isEmpty())
        {
            qCritical() << "OptionsValidator 传入的容器不能为空";
        }
        // 自动将容器元素转换为 QVariant
        for (const auto& item : options)
        {
            optionsList.append(QVariant::fromValue(item));
        }
    }


    /**
     * @brief 验证 value 是否有效(容器内包含)
     *
     * @param value 要判断的值
     * @return true(有效) or false(无效)
     */
    bool validate(const QVariant& value) const override
    {
        if constexpr (std::is_enum_v<T>)
        {
            // 枚举类型验证
            return optionsList.contains(value.value<T>());
        }
        else
        {
            // 容器类型验证
            return optionsList.contains(value);
        }
    }

    /**
     * @brief 验证并获取有效的值
     *
     * @param value 要判断的值
     * @return value无效 返回 QVariant::from(容器中的第一个值) || value有效, 返回QVariant::from(value)
     */
    QVariant correct(const QVariant& value) const override
    {
        if constexpr (std::is_enum_v<T>)
        {
            return validate(value) ? value : QVariant::fromValue(optionsList.first());
        }
        else
        {
            return validate(value) ? value : optionsList.first();
        }
    }


    /**
     * @brief 验证 value 是否有效(容器内包含)
     *
     * @tparam U 支持传递 QVariant 支持的类型
     * @param value 要判断的值
     * @return true(有效) || false(无效)
     */
    template <typename U>
    bool validate(U value) const
    {
        return validate(QVariant::fromValue(value));
    }

    /**
     * @brief 验证并获取有效的值
     *
     * @tparam U 支持传递 QVariant 支持的类型
     * @param value 要判断的值
     * @return value无效 返回 QVariant::from(容器中的第一个值) || value有效, 返回QVariant::from(value)
     */
    template <typename U>
    QVariant correct(U value) const
    {
        return correct(QVariant::fromValue(value));
    }

    /**
     * @brief 验证并获取有效的值
     *
     * @tparam U 支持传递 QVariant 支持的类型, 但前提是你要验证的value必须是和容器是相同的类型;
     * 如果你不能确定传给这个函数的 value 参数类型, 那么我不建议你用这个模板函数
     * @param value 要判断的值
     * @return value无效 返回容器中的第一个值 || value有效, 返回value自身
     */
    template <typename U>
    U correctPro(U value) const
    {
        // 简单的参数类型验证, 没啥用, QString和int,bool之类的都会判断能互相转, 所以在用这个函数之前, 一定要确定value的类型和容器内类型一致
        if (!optionsList.first().canConvert<U>())
        {
            qDebug() << "OptionsValidator.correctPro() error: value和容器内第一个元素类型不一致";
        }
        QVariant corrected = correct(QVariant::fromValue(value));
        return corrected.value<U>();
    }
    /**
      * @brief 获取选项列表
      *
      * @return 选项列表
      */
    QVariantList getOptions() { return optionsList; };

private:
    QVariantList optionsList; ///< 存储所有有效的选项

};

class ES_EXPORT EsBoolValidator : public EsOptionsValidator<QList<bool>>
{
public:
    /**
      * @brief 构造函数，初始化选项列表为 [true, false]
      */
    EsBoolValidator();

    QList<bool> boolOptions; ///< 存储布尔类型选项
};

class ES_EXPORT EsFolderValidator : public EsConfigValidator
{
public:
    /**
     * @brief 验证文件夹是否存在
     *
     * @param value 要验证的文件夹路径
     * @return true(存在) || false(不存在)
     */
    bool validate(const QVariant& value) const override;


    /**
     * @brief 修正文件夹路径，若不存在则创建
     *
     * @param value 需要修正的文件夹路径
     * @return 修正后的文件夹路径
     */
    QVariant correct(const QVariant& value) const override;
};


class ES_EXPORT EsFolderListValidator : public EsConfigValidator
{
public:

    /**
     * @brief 验证所有路径是否存在
     *
     * @param value 要验证的文件夹路径列表
     * @return true(都存在) || false (不是都存在)
     */
    bool validate(const QVariant& value) const override;

    /**
     * @brief 修正路径，将存在的文件夹路径转换为绝对路径，并替换反斜杠为正斜杠
     *
     * @param value 需要修正的文件夹路径列表
     * @return 修正后的路径列表
     */
    QVariant correct(const QVariant& value) const override;
};


class ES_EXPORT EsColorValidator : public EsConfigValidator
{
public:
    /**
     * @brief 构造函数，接受默认颜色
     *
     * @param color 默认颜色
     */
    EsColorValidator(const QVariant& color);

    /**
     * @brief 验证颜色是否有效
     *
     * @param value 要验证的颜色
     * @return true(有效) || false(无效)
     */
    bool validate(const QVariant& value) const override;

    /**
     * @brief 修正颜色，返回有效颜色，如果无效则返回默认颜色
     *
     * @param value 需要修正的颜色
     * @return 修正后的颜色
     */
    QVariant correct(const QVariant& value) const override;

private:
    QVariant defaultColor; ///< 默认颜色
};


class ES_EXPORT EsConfigSerializer
{
public:
    /**
     * @brief 序列化配置值
     *
     * @param value 要序列化的配置值
     * @return 序列化后的值
     */
    virtual QString serialize(QVariant value);

    /**
     * @brief 反序列化配置值
     *
     * @param value 配置文件中的值
     * @return 反序列化后的值
     */
    virtual QVariant deserialize(QVariant value);
};


template <typename T>
class ES_EXPORT EsEnumSerializer : public EsConfigSerializer
{
public:
    /**
     * @brief 序列化配置值
     *
     * @param value 要序列化的配置值, 必须是注册到Qt元系统的枚举值
     * @return 序列化后的值
     */
    QString serialize(QVariant value) override
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        QString themeValue = metaEnum.valueToKey(value.value<T>()); // 转换枚举为字符串
        return themeValue;
    }

    /**
     * @brief 反序列化配置值
     *
     * @param value 配置文件中的值
     * @return 反序列化后的值
     */
    QVariant deserialize(QVariant value) override
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        auto enumValue = metaEnum.keyToValue(value.toString().toUtf8().constData()); // 将字符串转换为枚举值
        if (enumValue == -1)
        {
            qWarning() << "Invalid value for enum deserialization:" << value;
            return T(); // 返回默认值
        }
        T enumTypedValue = static_cast<T>(enumValue);
        return QVariant::fromValue<T>(enumTypedValue); // 返回对应的枚举值
    }
};


class ES_EXPORT EsColorSerializer : public EsConfigSerializer
{
public:
    /**
     * @brief 序列化 QColor 对象
     *
     * @param value QColor 对象
     * @return QString 返回 QColor 的十六进制 ARGB 字符串表示
     */
    QString serialize(QVariant value) override;

    /**
     * @brief 反序列化 QColor 对象
     *
     * @param value 反序列化的值，可以是 字符串 || 列表(QList<int>)
     * @return QColor 对象
     */
    QVariant deserialize(QVariant value) override;
};



class ES_EXPORT EsConfigItem : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    /**
     * @brief 当配置项的值发生变化时发出的信号
     *
     * @param value 配置项的新值
     */
    void valueChanged(QVariant value);

public:

    /**
     * @brief 获取所有实例
     *
     * @return 所有ConfigItem实例的列表
     */
    static QList<EsConfigItem*>& allInstances();

    /**
     * @brief 构造函数，初始化配置项
     *
     * @param group 配置项的分组
     * @param name 配置项的名称，可以为空
     * @param defaultValue 默认值
     * @param validator 配置项的验证器
     * @param serializer 配置项的序列化器
     * @param restart 是否在更新配置值后重启应用程序
     */
    EsConfigItem(const QString& group,
               const QString& name,
               const QVariant& defaultValue,
               EsConfigValidator* validator = nullptr,
               EsConfigSerializer* serializer = nullptr,
               bool restart = false);

    ~EsConfigItem() override;

    /**
     * @brief 获取配置项的当前值
     *
     * @return 配置项当前的值
     */
    QVariant getValue() const;

    /**
    * @brief 设置配置项的值并发射信号
    *
    * @param v 要设置的值
    */
    void setValue(QVariant v);

    /**
     * @brief 获取配置项的键名
     *
     * @return QString 配置项的键名（以 `.` 分隔）
     */
    QString key() const;

    /**
     * @brief 序列化配置项的值
     *
     * @return QString 序列化后的配置值
     */
    QString serialize();

    /**
     * @brief 从序列化的值中反序列化并更新配置项的值
     *
     * @param value_ 反序列化的值
     */
    void deserializeFrom(QVariant& value_);


    virtual QString toString() const;


public:
    bool restart; ///< 是否在更新配置值后重启应用程序
    QString group; ///< 配置项所属的分组
    QString name; ///< 配置项的名称

protected:
    EsConfigValidator* validator; ///< 配置项的验证器
    EsConfigSerializer* serializer; ///< 配置项的序列化器
    QVariant value; ///< 配置项的当前值
    QVariant defaultValue; ///< 配置项的默认值
};

class ES_EXPORT EsRangeConfigItem : public EsConfigItem
{
public:
    EsRangeConfigItem(const QString& group,
                    const QString& name,
                    const QVariant& defaultValue,
                    EsConfigValidator* validator = nullptr,
                    bool restart = false);
    /**
     * @brief 获取配置项的范围
     *
     * @return 配置项的范围
     */
    QPair<int, int> range() const;

    QString toString() const override;
};

template <typename T>
class ES_EXPORT EsOptionsConfigItem : public EsConfigItem
{
public:
    EsOptionsConfigItem(const QString& group,
                      const QString& name,
                      const QVariant& defaultValue,
                      EsConfigValidator* validator = nullptr,
                      EsConfigSerializer* serializer = nullptr,
                      bool restart = false)
        : EsConfigItem(group,
                     name,
                     defaultValue,
                     validator,
                     serializer,
                     restart)
    {
    }

    /**
      * @brief 获取可选值列表
      *
      * @return 配置项的所有选项列表
      */
    QVariantList options() const
    {
        // 假设 validator 是一个带有 options 属性的类
        auto a = (EsOptionsValidator<T>*)validator;
        return a->getOptions();
    }

    QString toString() const override
    {
        // 格式化 options 列表
        QStringList opts;
        for (const QVariant& v : options()) {
            opts << v.toString();
        }
        QString optionsStr = "[" + opts.join(", ") + "]";

        return QString("%1[options=%2, value=%3]")
            .arg(QString::fromUtf8(this->metaObject()->className()))
            .arg(optionsStr)
            .arg(this->getValue().toString());
    }
};

class ES_EXPORT EsColorConfigItem : public EsConfigItem
{
public:
    EsColorConfigItem(const QString& group,
                    const QString& name,
                    const QVariant& defaultValue,
                    bool restart = false);

    QString toString() const override;
};


class ES_EXPORT EsConfig : public QObject
{
    Q_OBJECT
Q_SIGNALS:
    /**
     * @brief 应用重启信号
     */
    void appRestartSig();

    /**
     * @brief 主题改变信号
     *
     * @param theme 改变后的主题
     */
    void themeChanged(Es::Theme theme);

    /**
     * @brief 主题改变完成信号
     */
    void themeChangedFinished();

    /**
     * @brief 主题颜色改变信号
     *
     * @param color 改变后的颜色
     */
    void themeColorChanged(QColor color);

public:
    static EsOptionsConfigItem<Es::Theme> themeMode; ///< 主题配置项
    static EsColorConfigItem themeColor; ///< 主题颜色配置项
    static EsConfigItem fontFamilies; ///< 字体配置项


    EsConfig();


    /**
     * @brief 获取配置项的值。
     *
     * @param item 要查询的配置项。
     * @return 配置项当前的值，以 QVariant 形式返回。
     */
    QVariant get(EsConfigItem* item);

    /**
     * @brief 设置配置项的值。
     *
     * @param item   要更新的配置项。
     * @param newValue  配置项的新值。
     * @param save   是否将更改保存到配置文件中。默认为 true。
     */
    void set(EsConfigItem* item, QVariant newValue, bool save = true);

    /**
     * @brief 加载配置。
     *
     * @param file   JSON 配置文件的路径（字符串或路径对象）。
     */
    void load(const QString& file = {});

    /// 保存配置到文件
    void save();

    /**
     * @brief 获取当前的主题模式。
     *
     * 主题模式可以是 @c Es::Theme::Theme_LIGHT 或 @c Es::Theme::Theme_DARK
     *
     * @return 当前主题枚举值。
     */
    Es::Theme getTheme();

    /**
     * @brief 临时切换主题，不修改配置文件。
     *
     * 此操作仅影响当前应用程序的 UI 主题，不会将更改写入配置文件。
     * 重启后将恢复为配置文件中保存的主题。
     *
     * @param t 要切换到的主题（Light / Dark / Auto）。
     */
    void setTheme(Es::Theme t);

    /// 将配置项转换为 QVariantMap
    QVariantMap toMap(bool serialize = true);

public:
    QString cfgFile;
    Es::Theme theme = Es::Theme::Theme_LIGHT;
    static QList<EsConfigItem*> getAllConfigItems();
};


inline EsConfig esConfig;

namespace EsFunc
{
    /// 主题是否为深色模式
    bool isDarkTheme();

    /// 获取主题模式
    bool getCurrentTheme();

    /// 主题是否为深色模式
    bool isDarkThemeMode(Es::Theme theme = Es::Theme::Theme_AUTO);
}

#endif //ES_ESCONFIG_H
