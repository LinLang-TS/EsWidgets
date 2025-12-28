/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTableItemDelegate.cpp
** @date 2025/10/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#include "esTableItemDelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QTableWidget>
#include <QScrollBar>

#include "esConfig.h"
#include "esFont.h"
#include "esIcon.h"
#include "widgets/esLineEdit.h"
#include "widgets/esComboBox.h"
#include "widgets/esEditableComboBox.h"
#include "widgets/esRoundMenu.h"
#include "widgets/esTableWidgetComboItem.h"


EsTableItemDelegate::EsTableItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent), margin(2), hoverRow(-1), pressedRow(-1)
{
    // todo 后面写到toolTip在实现他
    // if (qobject_cast<QTableView*> parent)
    // {
    //     tooltipDelegate = ItemViewToolTipDelegate(parent, 100, ItemViewToolTipType.TABLE);
    // }
    // else
    //
    // {
    //     tooltipDelegate = ItemViewToolTipDelegate(parent, 100, ItemViewToolTipType.LIST);
    // }
}

void EsTableItemDelegate::setHoverRow(int row)
{
    hoverRow = row;
}

void EsTableItemDelegate::setPressedRow(int row)
{
    pressedRow = row;
}

void EsTableItemDelegate::setSelectedRows(const QList<QModelIndex>& indexes)
{
    selectedRows.clear();
    for (const QModelIndex& index : indexes)
    {
        selectedRows.insert(index.row());
        if (index.row() == pressedRow)
        {
            pressedRow = -1;
        }
    }
}

QSize EsTableItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size = size.grownBy(QMargins(0, margin, 0, margin)); // 增加上下边距
    return size;
}

QWidget* EsTableItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{

    // EsComboBox单元格: 跳过
    if (auto var = index.data(Es::ComboOptionsRole); var.isValid()) return nullptr;

    // 普通文本单元格: 使用 EsLineEdit 编辑
    auto lineEdit = new EsLineEdit(parent);
    lineEdit->setProperty("transparent", false);
    lineEdit->setStyle(QApplication::style());
    lineEdit->setText(option.text);
    lineEdit->setClearButtonEnabled(true);
    return lineEdit;
}

void EsTableItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                               const QModelIndex& index) const
{
    QRect rect = option.rect;
    int y = rect.top() + (rect.height() - editor->height()) / 2;
    int x = qMax(8, rect.x());
    int w = rect.width();
    if (index.column() == 0)
    {
        w -= 8; // 对于第 0 列，减去 8 像素宽度
    }

    editor->setGeometry(x, y, w, rect.height());
}

void EsTableItemDelegate::setCheckedColor(const QColor& light, const QColor& dark)
{
    lightCheckedColor = light;
    darkCheckedColor = dark;
    // 更新视图
    auto p = (QAbstractItemView*)parent();
    p->viewport()->update();
}

void EsTableItemDelegate::_drawBackground(QPainter* painter, const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    int r = 5;
    QRect rect;
    if (index.column() == 0)
    {
        rect = option.rect.adjusted(4, 0, r + 1, 0);
        painter->drawRoundedRect(rect, r, r);
    }
    else if (index.column() == index.model()->columnCount(index.parent()) - 1)
    {
        rect = option.rect.adjusted(-r - 1, 0, -4, 0);
        painter->drawRoundedRect(rect, r, r);
    }
    else
    {
        rect = option.rect.adjusted(-1, 0, 1, 0);
        painter->drawRect(rect);
    }
}

void EsTableItemDelegate::_drawIndicator(QPainter* painter, const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const
{
    int y = option.rect.y();
    int h = option.rect.height();
    int ph = (pressedRow == index.row()) ? round(0.35 * h) : round(0.257 * h);
    painter->setBrush(EsFunc::autoFallbackThemeColor(darkCheckedColor, lightCheckedColor));
    painter->drawRoundedRect(4, ph + y, 3, h - 2 * ph, 1.5, 1.5);
}

void EsTableItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    // 设置字体
    QFont font;
    if (index.data(Qt::FontRole).isNull() || !index.data(Qt::FontRole).isValid())
    {
        font = EsFunc::getFont(13); // 使用默认字体
    }
    else
    {
        font = index.data(Qt::FontRole).value<QFont>();
    }
    option->font = font;

    // 设置文本颜色
    QColor textColor = EsFunc::isDarkTheme() ? Qt::white : Qt::black;
    auto textBrush = index.data(Qt::TextColorRole);
    if (!textBrush.isNull())
    {
        textColor = textBrush.value<QBrush>().color();
    }

    option->palette.setColor(QPalette::Text, textColor);
    option->palette.setColor(QPalette::HighlightedText, textColor);
}

void EsTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);

    // 设置裁剪矩形，防止绘制超出边框
    painter->setClipping(true);
    painter->setClipRect(option.rect);

    // 调用原始的绘制方法，其中 option.rect 被调整以考虑边框
    QStyleOptionViewItem adjustedOption = option;
    adjustedOption.rect.adjust(0, margin, 0, -margin);

    // 背景高亮绘制
    bool isHover = hoverRow == index.row();
    bool isPressed = pressedRow == index.row();
    auto p = (QAbstractItemView*)parent();
    bool isAlternate = index.row() % 2 == 0 && p->alternatingRowColors();
    bool isDark = EsFunc::isDarkTheme();

    int c = isDark ? 255 : 0;
    int alpha = 0;

    if (!selectedRows.contains(index.row()))
    {
        if (isPressed)
        {
            alpha = isDark ? 9 : 6;
        }
        else if (isHover)
        {
            alpha = 12;
        }
        else if (isAlternate)
        {
            alpha = 5;
        }
    }
    else
    {
        if (isPressed)
        {
            alpha = isDark ? 15 : 9;
        }
        else if (isHover)
        {
            alpha = 25;
        }
        else
        {
            alpha = 17;
        }
    }

    auto var = index.data(Qt::BackgroundRole);
    if (var.isValid() && !var.isNull())
    {
        painter->setBrush(var.value<QColor>());
    }
    else
    {
        painter->setBrush(QColor(c, c, c, alpha));
    }

    _drawBackground(painter, adjustedOption, index);

    // 绘制指示器
    if (selectedRows.contains(index.row()) && index.column() == 0 && p->horizontalScrollBar()->value() == 0)
    {
        _drawIndicator(painter, adjustedOption, index);
    }

    // 绘制复选框
    var = index.data(Qt::CheckStateRole);
    if (var.isValid() && !var.isNull())
    {
        _drawCheckBox(painter, adjustedOption, index);
    }

    // 绘制选择框的箭头
    var = index.data(Es::ComboOptionsRole);
    if (var.isValid() && !var.isNull())
    {
        QRectF rect(
              adjustedOption.rect.right() - 16, // 距右边留 16 像素
              adjustedOption.rect.center().y() - 5, // 垂直居中
              10,
              10
          );
        auto ico = EsIcon(Es::Icon_ChevronDown); // ARROW_DOWN

        if (EsFunc::isDarkTheme())
        {
            ico.render(painter, rect , {});
        } else
        {
            ico.render(painter, rect, {}, {{"fill", "#646464"}});
        }

    }
    painter->restore();

    // 调用父类的绘制方法
    QStyledItemDelegate::paint(painter, adjustedOption, index);
}

void EsTableItemDelegate::_drawCheckBox(QPainter* painter, const QStyleOptionViewItem& option,
                                      const QModelIndex& index) const
{
    painter->save();

    // 获取复选框状态
    Qt::CheckState checkState = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());

    bool isDark = EsFunc::isDarkTheme();

    qreal r = 4.5;
    qreal x = option.rect.x() + 15;
    qreal y = option.rect.center().y() - 9.5;
    QRectF rect(x, y, 19, 19);

    // 绘制未选中状态
    if (checkState == Qt::Unchecked)
    {
        QColor backgroundColor = isDark ? QColor(0, 0, 0, 26) : QColor(0, 0, 0, 6);
        QColor borderColor = isDark ? QColor(255, 255, 255, 142) : QColor(0, 0, 0, 122);
        painter->setBrush(backgroundColor);
        painter->setPen(borderColor);
        painter->drawRoundedRect(rect, r, r);
    }
    else
    {
        QColor color = EsFunc::autoFallbackThemeColor(lightCheckedColor, darkCheckedColor);
        painter->setPen(color);
        painter->setBrush(color);
        painter->drawRoundedRect(rect, r, r);

        // 绘制选中状态
        if (checkState == Qt::Checked) {
            auto ico = EsCheckBoxIcon(Es::IconName::IconCheckBox_Accept);
            ico.render(painter, rect, {});
        }
        else // 部分选中状态
        {
            auto ico = EsCheckBoxIcon(Es::IconName::IconCheckBox_PartialAccept);
            ico.render(painter, rect, {});
        }
    }

    painter->restore();
}

bool EsTableItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
    const QModelIndex& index)
{
    // 是否为 ComboBox Item
    const QVariant var = index.data(Es::ComboOptionsRole);
    if (!var.isValid() || var.isNull())
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }


    auto cfg = var.value<EsTableWidgetComboItemData>();

    // combo单元格可编辑(双击)
    if (cfg.editable)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (cfg.options.isEmpty()) return false;

            auto* view = qobject_cast<QAbstractItemView*>(parent());
            if (!view) return false;

            auto editableCombo = new EsEditableComboBox(view->viewport()); // 必须传 view->viewport(), 直接传parent()会显示在上一行的单元格
            QString itemText = index.data().toString(); // 取当前单元格文本; 这个事件里option参数还没填充具体数据, 所以要从index里取文本
            initComboBox(editableCombo, cfg, itemText);
            editableCombo->setFocus();
            editableCombo->setText(itemText);


            connect(editableCombo, &EsEditableComboBox::editingFinished, [model, index, editableCombo, cfg]
            {
                // 避雷: 失去焦点后也会触发该信号; 当我点击option时, lineEdit失去焦点, 会触发一个空的完成, 导致直接执行删除combo;
                // 所以这里判断如果失去焦点 且 下拉菜单对象不为null(正在显示), 就过滤掉
                if (!editableCombo->hasFocus() && editableCombo->dropMenu) return;


                auto tmpCfg = cfg; // 只能拷贝一份, []里捕获的都是const，没办法修改里面的值
                tmpCfg.currentIndex = editableCombo->currentIndex();
                model->setData(index, editableCombo->currentText(), Qt::DisplayRole); // 设置单元格文本
                model->setData(index, QVariant::fromValue(tmpCfg), Es::ItemDataRole::ComboOptionsRole);
                editableCombo->deleteLater();
            });
            // 定位并弹出
            editableCombo->setGeometry(option.rect);
            editableCombo->show();
            return true;
        }

        return false;
    }

    // combo单元格不可编辑(单击)
    if (event->type() == QEvent::MouseButtonRelease)
    {
        auto* me = dynamic_cast<QMouseEvent*>(event);
        if (me->button() != Qt::LeftButton) return false;
        if (cfg.options.isEmpty()) return false;

        auto* view = qobject_cast<QAbstractItemView*>(parent());
        if (!view) return false;

        // 创建 ComboBox
        auto* combo = new EsComboBox(view->viewport());
        auto itemText = index.data().toString();
        initComboBox(combo, cfg, itemText);
        // 选择后写回 Model
        connect(combo, &EsComboBox::currentIndexChanged, [model, index, combo, cfg](int idx){
                auto tmpCfg = cfg;
                tmpCfg.currentIndex = idx;
                model->setData(index, combo->itemText(idx), Qt::DisplayRole);
                model->setData(index, QVariant::fromValue(tmpCfg), Es::ItemDataRole::ComboOptionsRole);
                combo->deleteLater();
            });

        // 关闭下拉菜单后销毁
        if (combo->dropMenu)
        {
            connect(combo->dropMenu, &EsRoundMenu::closedSignal, &QObject::deleteLater);
        }

        // 目前使用的方案是单击时只显示下拉菜单
        // 单击时显示combo的时, 需要先清空单元格文本(因为combo的背景是有一丢丢透明 会穿透文本);
        // 因为paint里绘制了下箭头的icon, 也会穿透, 如果显示combo会看着有2个下箭头, 这里我就不处理了, 直接不显示combo
        // model->setData(index, QString(), Qt::DisplayRole);

        // 定位并弹出
        combo->setGeometry(option.rect);
        // combo->show();
        combo->toggleComboMenu();
        return true;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}



