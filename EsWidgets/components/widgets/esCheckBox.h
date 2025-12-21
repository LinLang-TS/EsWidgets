/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esCheckBox.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESCHECKBOX_H
#define ES_ESCHECKBOX_H

#include "esDef.h"
#include "esIcon.h"
#include <QCheckBox>


class EsCheckBoxPrivate;

class ES_EXPORT EsCheckBox : public QCheckBox
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsCheckBox)

public:
    explicit EsCheckBox(QWidget* parent = nullptr);
    explicit EsCheckBox(const QString& text, QWidget* parent = nullptr);
    ~EsCheckBox();


    /**
     * @brief 设置选中状态下指示器的颜色
     *
     * @param light 主题模式下的指示器颜色（浅色）todo light, dark: str | QColor | Qt.GlobalColor ;
     * @param dark 主题模式下的指示器颜色（深色）
     */
    void setCheckedColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置文本的颜色
     *
     * @param light 主题模式下的文本颜色（浅色）todo light, dark: str | QColor | Qt.GlobalColor ;
     * @param dark 主题模式下的文本颜色（深色）
     */
    void setTextColor(const QColor& light, const QColor& dark);



protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent*) override;
};

#endif //ES_ESCHECKBOX_H
