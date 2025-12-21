/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esUtils.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESUTILS_H
#define ES_ESUTILS_H
#include <QString>
#include <QMetaEnum>
#include <QDebug>

namespace EsFunc
{
    /**
     * @brief 将 Qt 元对象系统注册的枚举值转换为字符串
     * @tparam Enum 枚举类型
     * @param value 枚举值; 枚举必须已通过 Q_ENUM 或 Q_ENUM_NS 注册
     * @return 第一个下划线 '_' 之后的枚举值字符串(如 "Theme_DARK" → "DARK"); 如果没下划线就直接返回对应字符串
     */
    template<typename Enum>
    QString getEnumValueStr(Enum value)
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();
        if (!metaEnum.isValid()) {
            qWarning() << "getEnumValueStr: 无效的枚举类型, 枚举未使用 Q_ENUM/Q_ENUM_NS 注册";
            return {};
        }

        const char* key = metaEnum.valueToKey(static_cast<int>(value));
        if (!key) {
            qWarning() << "getEnumValueStr: 无效的枚举值:" << static_cast<int>(value);
            return {};
        }

        QString str = QString::fromLatin1(key);

        // 提取下划线后部分
        int pos = str.indexOf('_');
        if (pos != -1) {
            str = str.mid(pos + 1);
        }

        return str;
    }
}

#endif //ES_ESUTILS_H
