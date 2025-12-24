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
#include "widgets/esRoundMenu.h"
#include "widgets/esTableWidgetComboBoxItem.h"


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

    // EsComboBox 单元格
    if (auto var = index.data(Es::ComboOptionsRole); var.isValid())
    {
        // 禁止进入编辑态
        if (var.value<TableWidgetComboBoxItemData>().editable == false) return nullptr;

        // todo 创建 EsEditableComboBox并显示

    }


    // 普通文本单元格：使用 EsLineEdit 编辑
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


    const auto cfg = var.value<TableWidgetComboBoxItemData>();

    // 如果 combo单元格 可编辑, 就走 createEditor 方法创建 editableComboBox
    // 返回false和调用基类差不多, 但是如果表格里有 checkbox , 返回false就无法切换他的选中状态;
    // 但是上面已经判断了他是combobox单元格, 没checkbox, 所以直接返回false
    if (cfg.editable) return false;

    if (event->type() == QEvent::MouseButtonRelease)
    {

        auto* me = dynamic_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton)
        {

            if (cfg.options.isEmpty()) return false;

            auto* view = qobject_cast<QAbstractItemView*>(parent());
            if (!view) return false;

            // 创建 ComboBox
            auto* combo = new EsComboBox(view->viewport());
            combo->addItems(cfg.options);

            // 提示文本; 目前单击时方案是不显示combo, 所以只有在双击可编辑时才需要设置这个
            // if (!cfg.placeholderText.isEmpty())
            // {
            //     combo->setPlaceholderText(cfg.placeholderText);
            // }

            // 最大显示项数
            if (cfg.maxVisibleItems > 0)
            {
                combo->setMaxVisibleItems(cfg.maxVisibleItems);
            }

            // 当前选中项同步; 优先 cfg.currentIndex，其次 DisplayRole
            int indexToSelect = cfg.currentIndex;

            if (indexToSelect < 0)
            {
                const QString text = index.data(Qt::DisplayRole).toString();
                indexToSelect = cfg.options.indexOf(text);
            }

            if (indexToSelect >= 0)
            {
                combo->setCurrentIndex(indexToSelect);
            }
            else
            {
                // 不强制选中第一个
                combo->setCurrentIndex(-1);
            }

            // 禁用选项
            for (int idx : cfg.disabledIndexes)
            {
                combo->setItemEnabled(idx, false);
            }


            // 选择后写回 Model
            connect(combo, &EsComboBox::activated, combo, [model, index, combo](int idx){
                    model->setData(index, combo->itemText(idx), Qt::DisplayRole);
                    combo->deleteLater();
                });
            // 关闭选择框后销毁
            connect(combo->dropMenu, &EsRoundMenu::closedSignal, combo, &QObject::deleteLater);
            // 单击时显示combo的方案; 目前使用的方案是单击时只显示选择框
            // QString originalText = index.data(Qt::DisplayRole).toString();
            // model->setData(index, QString(), Qt::DisplayRole);
            // connect(combo->dropMenu, &EsRoundMenu::closedSignal, combo, [model, index, originalText]()
            // {
            //     if (index.data(Qt::DisplayRole).toString().isEmpty())
            //         model->setData(index, originalText, Qt::DisplayRole);
            // });

            // 定位并弹出
            combo->setGeometry(option.rect);
            // combo->show();
            combo->toggleComboMenu();
            return true;

        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

