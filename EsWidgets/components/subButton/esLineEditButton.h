/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esLineEditButton.h
** @date 2025/10/20
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESLINEEDITBUTTON_H
#define ESLINEEDITBUTTON_H
#include <QToolButton>
#include <qvariant.h>

#include "esDef.h"
#include "esGlobal.h"

class EsLineEditButtonPrivate;
class EsIconBase;
class QIcon;
class ES_EXPORT EsLineEditButton:public QToolButton
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsLineEditButton)
public:
    explicit EsLineEditButton(const QVariant& icon, QWidget *parent = nullptr);
    explicit EsLineEditButton(const QString& icon, QWidget *parent = nullptr);
    explicit EsLineEditButton(const QIcon& icon, QWidget *parent = nullptr);
    explicit EsLineEditButton(Es::IconName icon, QWidget *parent = nullptr);
    explicit EsLineEditButton(EsIconBase* icon, QWidget *parent = nullptr);
    ~EsLineEditButton() override;

    /**
    * 设置按钮的操作
    */
    void setAction(QAction* action);

    /**
     * 获取当前的操作
     */
    QAction* action() const;

    /**
     * 设置图标 icon: Union[str, EsIconBase, QIcon]
     */
    void setIcon(const QVariant& icon);

protected:
    /**
     * 处理鼠标按下事件
     */
    void mousePressEvent(QMouseEvent* e) override;

    /**
     * 处理鼠标释放事件
     */
    void mouseReleaseEvent(QMouseEvent* e) override;

    /**
     * 绘制事件
     */
    void paintEvent(QPaintEvent* e) override;
};



#endif //ESLINEEDITBUTTON_H
