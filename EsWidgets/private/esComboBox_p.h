/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esComboBox_p.h
** @date 2025/12/5
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESCOMBOBOX_P_H
#define ESCOMBOBOX_P_H
#include "esDef.h"
class EsComboBoxMenu;
class EsComboItem;
class EsTranslateYAnimation;
class EsComboBox;


class EsComboBoxPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsComboBox);

public:
    explicit EsComboBoxPrivate(QObject* parent=nullptr);

    void updateTextState(bool isPlaceholder);

    bool isHover;
    bool isPressed;
    QList<EsComboItem*> items;
    int maxVisibleItems;
    EsTranslateYAnimation* arrowAni;
    QString placeholderText;
    int currentIndex;
};



#endif //ESCOMBOBOX_P_H
