/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLinedit_p.h
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLINEDIT_P_H
#define ESLINEDIT_P_H
#include "esDef.h"
#include <QCompleter>
#include <QColor>
class EsLineEdit;
class EsCompleterMenu;
class EsLineEditButton;

class EsLineEditPrivate : public QObject
{
    Q_OBJECT
    E_D_CREATE_PIMPL(EsLineEdit)

public:
    explicit EsLineEditPrivate(QObject* parent = nullptr);

    /**
     * 调整文本的内边距，以适应按钮的显示
     */
    void adjustTextMargins();

    /**
     * 当文本内容改变时的槽函数
     * @param text 当前的文本内容
     */
    Q_SLOT void onTextChanged(const QString& text);

    /**
     * 当文本被编辑时的槽函数
     * @param text 当前编辑的文本内容
     */
    Q_SLOT void onTextEdited(const QString& text);

    /**
    * 显示自动补全菜单
    */
    void showCompleterMenu();
    bool isClearButtonEnabled;
    QCompleter* completer;
    EsCompleterMenu* completerMenu;
    bool isError;

    QColor lightFocusedBorderColor;
    QColor darkFocusedBorderColor;
    QList<EsLineEditButton*> leftButtons;
    QList<EsLineEditButton*> rightButtons;
};

#endif //ESLINEDIT_P_H
