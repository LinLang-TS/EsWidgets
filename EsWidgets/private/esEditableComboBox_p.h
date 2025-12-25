/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esEditableComboBox_p.h
** @date 2025/12/25
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESWIDGETS_ESEDITABLECOMBOBOX_P_H
#define ESWIDGETS_ESEDITABLECOMBOBOX_P_H

#include "esDef.h"
class EsComboBoxMenu;
class EsComboItem;
class EsTranslateYAnimation;
class EsEditableComboBox;


class EsEditableComboBoxPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsEditableComboBox);

public:
    explicit EsEditableComboBoxPrivate(QObject* parent=nullptr);

    void updateTextState(bool isPlaceholder);

    bool isHover;
    bool isPressed;
    QList<EsComboItem*> items;
    int maxVisibleItems;
    QString placeholderText;
    int currentIndex;
};


#endif //ESWIDGETS_ESEDITABLECOMBOBOX_P_H