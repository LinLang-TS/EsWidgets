/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esArrowButton_p.h
** @date 2025/10/22
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESARROWBUTTON_P_H
#define ESARROWBUTTON_P_H
#include "esDef.h"
#include "esIcon.h"
class EsArrowButton;

class EsArrowButtonPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsArrowButton);

public:
    explicit EsArrowButtonPrivate(QObject* parent=nullptr);
    EsIcon* icon;
};



#endif //ESARROWBUTTON_P_H
