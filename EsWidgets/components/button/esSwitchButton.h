/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esSwitchButton.h
** @date 2025/11/14
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSWITCHBUTTON_H
#define ESSWITCHBUTTON_H
#include "esDef.h"
#include <QWidget>

class EsSwitchButtonPrivate;

class ES_EXPORT EsSwitchButton : public QWidget
{
    Q_OBJECT
    E_Q_CREATE_PIMPL(EsSwitchButton)
    Q_PROPERTY(int spacing READ getSpacing WRITE setSpacing)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)
    Q_PROPERTY(QString text READ getText WRITE setText)
    Q_PROPERTY(QString onText READ getOnText WRITE setOnText)
    Q_PROPERTY(QString offText READ getOffText WRITE setOffText)

Q_SIGNALS:
    void checkedChanged(bool);

public:
    explicit EsSwitchButton(QWidget* parent = nullptr, Es::IndicatorPosition indicatorPos = Es::IndicatorPosition::LEFT);
    explicit EsSwitchButton(const QString& text, QWidget* parent = nullptr, Es::IndicatorPosition indicatorPos = Es::IndicatorPosition::LEFT);
    ~EsSwitchButton() override;

    bool eventFilter(QObject* obj, QEvent* e) override;

    bool isChecked() const;

    void setChecked(bool isChecked);

    /**
     * @brief 设置文本颜色
     *
     * 根据光明模式和暗黑模式设置文本颜色。
     *
     * @param light 亮色模式下的文本颜色 todo light, dark: str | QColor | Qt.GlobalColor ;
     * @param dark 暗色模式下的文本颜色
     */
    void setTextColor(const QColor& light, const QColor& dark);

    /**
     * @brief 设置已选中状态下指示器的颜色
     *
     * 根据光明模式和暗黑模式设置已选中状态下指示器的颜色。
     *
     * @param light 亮色模式下的指示器颜色 todo light, dark: str | QColor | Qt.GlobalColor ;
     * @param dark 暗色模式下的指示器颜色
     */
    void setCheckedIndicatorColor(const QColor& light, const QColor& dark);

    // 切换选中状态
    void toggleChecked();

    QString getText() const;

    void setText(const QString& text);

    int getSpacing() const;

    void setSpacing(int spacing);

    QString getOnText() const;

    void setOnText(const QString& text);

    QString getOffText() const;

    void setOffText(const QString& text);
};


#endif //ESSWITCHBUTTON_H
