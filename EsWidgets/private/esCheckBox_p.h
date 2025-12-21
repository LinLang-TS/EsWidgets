/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esCheckBox_p.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESCHECKBOX_P_H
#define ESCHECKBOX_P_H

#include "esDef.h"
#include <QColor>
class EsCheckBoxIcon;
class EsCheckBox;

class EsCheckBoxPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsCheckBox)
public:
    explicit EsCheckBoxPrivate(QObject* parent = nullptr);

    /**
     * @brief 获取边框颜色，根据当前的主题和状态返回不同的颜色
     *
     * 根据当前是否为暗黑主题以及勾选框的不同状态，返回对应的边框颜色。
     *
     * @return QColor 返回适用于当前状态的边框颜色
     */
    QColor borderColor();

        /**
     * @brief 获取背景颜色，根据当前的主题和状态返回不同的背景颜色
     *
     * 根据当前是否为暗黑主题以及勾选框的不同状态，返回对应的背景颜色。
     *
     * @return QColor 返回适用于当前状态的背景颜色
     */
    QColor backgroundColor();

    /**
     * @brief 获取当前 CheckBox 的状态
     *
     * 该方法根据 CheckBox 的当前状态（是否选中、是否按下、是否悬停、是否启用）返回相应的状态。
     *
     * @return CheckBoxState 返回当前的状态
     */
    Es::CheckBoxState state();

    bool isPressed;
    bool isHover;
    QColor lightCheckedColor;
    QColor darkCheckedColor;
    QColor lightTextColor;
    QColor darkTextColor;
    EsCheckBoxIcon* iconAccept;
    EsCheckBoxIcon* iconPartialAccept;
    QMap<int, bool> states;
};

#endif //ESCHECKBOX_P_H
