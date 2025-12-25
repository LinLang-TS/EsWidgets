/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLineEdit.h
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLINEEDIT_H
#define ESLINEEDIT_H

#include <QLineEdit>
#include "esDef.h"

class EsCompleterMenu;
class QHBoxLayout;
class EsLineEditPrivate;
class EsLineEditButton;

class ES_EXPORT EsLineEdit : public QLineEdit
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsLineEdit)

public:
    explicit EsLineEdit(QWidget* parent = nullptr);
    ~EsLineEdit() override;
    // explicit EsLineEdit(const QString& text, QWidget* parent = nullptr);

    /**
    * 获取当前是否为错误状态
    * @return 错误状态
    */
    bool isError() const;
    /**
     * 设置错误状态
     * @param isError 错误状态
     */
    void setError(bool isError);
    /**
     * 设置聚焦时的边框颜色
     * @param light 浅色模式下的边框颜色
     * @param dark 深色模式下的边框颜色
     */
    void setCustomFocusedBorderColor(const QColor& light, const QColor& dark);

    /**
     * 获取聚焦时的边框颜色
     * @return 当前的边框颜色
     */
    QColor focusedBorderColor() const;

    /**
     * 启用或禁用清除按钮
     * @param enable 是否启用清除按钮
     */
    void setClearButtonEnabled(bool enable);

    /**
     * 获取清除按钮是否启用
     * @return 清除按钮是否启用
     */
    bool isClearButtonEnabled() const;

    /**
     * 设置自动补全器
     * @param completer QCompleter 对象
     */
    void setCompleter(QCompleter* completer);

    /**
     * 获取自动补全器
     * @return 当前的 QCompleter 对象
     */
    QCompleter* completer() const;

    /**
     * 添加单个动作到 LineEdit
     * @param action 要添加的动作
     * @param position 动作的位置（前置或后置）
     */
    void addAction(QAction* action, QLineEdit::ActionPosition position = QLineEdit::TrailingPosition);

    /**
     * 批量添加动作到 LineEdit
     * @param actions 动作列表
     * @param position 动作的位置（前置或后置）
     */
    void addActions(const QList<QAction*>& actions, QLineEdit::ActionPosition position = QLineEdit::TrailingPosition);

   /**
     * 设置自动补全菜单
     * @param menu CompleterMenu 对象
     */
    virtual void setCompleterMenu(EsCompleterMenu* menu);

protected:
    /**
     * 聚焦失去时的事件处理
     * @param e 事件对象
     */
    void focusOutEvent(QFocusEvent* e) override;

    /**
     * 聚焦获取时的事件处理
     * @param e 事件对象
     */
    void focusInEvent(QFocusEvent* e) override;



    /**
     * 右键菜单事件
     * @param e 事件对象
     */
    // TODO void contextMenuEvent(QContextMenuEvent* e) override;

    /**
     * 绘制事件
     * @param e 事件对象
     */
    void paintEvent(QPaintEvent* e) override;

protected:
    QHBoxLayout* hBoxLayout;
    EsLineEditButton* clearButton;
};


#endif //ESLINEEDIT_H
