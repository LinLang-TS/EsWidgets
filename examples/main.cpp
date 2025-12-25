#include <QtCore/qglobal.h>
#include <filesystem>
#include <tuple>
#include <map>
#include <QApplication>
#include <QColor>
#include <QString>
#include <stdexcept>
#include <vector>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QIcon>
#include <QMetaEnum>

#include "../EsWidgets/components/button/esPushButton.h"
#include "../EsWidgets/components/widgets/esWidget.h"
#include "button/esPrimaryPushButton.h"
#include "button/esSwitchButton.h"
#include "button/EsTogglePushButton.h"
#include "button/esToolButton.h"
#include "button/esTransparentPushButton.h"
#include "button/esTransparentTogglePushButton.h"
#include "widgets/esCheckBox.h"
#include "widgets/esInfoBar.h"
#include "widgets/esLineEdit.h"
#include "widgets/esTableWidget.h"
// #include <vld.h>

#include <QCompleter>
#include <QWindow>

#include "esConfig.h"
#include "private/esRoundMenu_p.h"
#include "widgets/esComboBox.h"
#include "widgets/esEditableComboBox.h"
#include "widgets/esListWidget.h"
#include "widgets/esRoundMenu.h"
#include "widgets/esTableWidgetComboItem.h"
#include "windows/esWindowBase.h"

class MyClass : public QWidget
{
    Q_OBJECT

public:
    MyClass(QWidget* parent=nullptr)
        : QWidget(parent)
    {
        setWindowTitle("EsWidgets");

        btn1 = new EsPushButton(Es::Icon_AddTo, "测试按钮1");
        btn2 = new EsTransparentPushButton(Es::Icon_Add, "测试按钮");
        btn3 = new EsPrimaryPushButton(Es::Icon_DictionaryAdd, "测试按钮");
        btn4 = new EsTogglePushButton("测试按钮");
        btn5 = new EsTransparentTogglePushButton("测试按钮");
        btn6 = new EsCheckBox("测试按钮");
        btn6->setTristate(true);
        btn7 = new EsToolButton(Es::Icon_DictionaryAdd);
        btn8 = new EsSwitchButton("关闭", this);
        listWidget = new EsListWidget(this);
        listWidget->addItem(new QListWidgetItem("测试"));
        listWidget->addItem(new QListWidgetItem("测试"));
        listWidget->addItem(new QListWidgetItem("测试"));
        combox = new EsComboBox(this);
        combox->addItem("测试1");
        combox->addItem("测试2");
        combox->addItem("测试3");
        editCombox = new EsEditableComboBox(this);
        editCombox->addItem("测试1");
        editCombox->addItem("测试2");
        editCombox->addItem("测试3");
        auto a = new EsTableWidget(5, 6);
        auto let = new EsLineEdit();
        connect(btn1, SIGNAL(clicked()), SLOT(on_btn1_clicked()));
        connect(btn2, SIGNAL(clicked()), SLOT(on_btn2_clicked()));
        QStringList wordList;
        wordList << "apple" << "banana" << "cherry" << "date" << "fig";
        completer = new QCompleter(wordList, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setMaxVisibleItems(10);
        completer->setCompletionRole(Qt::DisplayRole);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion); // 显示全部item
        let->setCompleter(completer);
        auto layout = new QVBoxLayout();
        layout->addStretch();
        layout->addWidget(btn1);
        layout->addWidget(btn2);
        layout->addWidget(btn3);
        layout->addWidget(btn4);
        layout->addWidget(btn5);
        layout->addWidget(btn6);
        layout->addWidget(btn7);
        layout->addWidget(btn8);
        layout->addWidget(listWidget);
        layout->addWidget(combox);
        layout->addWidget(editCombox);
        layout->addWidget(let);
        layout->addWidget(a);

        setLayout(layout);
        connect(&esConfig, &EsConfig::themeChanged, this, &MyClass::on_themeChanged);

    }

protected:
    void contextMenuEvent(QContextMenuEvent* event) override
    {
        auto menu = new EsRoundMenu("", this);

        auto x1 = new EsAction(new EsIcon(Es::Icon_Copy), "Copy");
        auto x2 = new EsAction("Copy1");
        x2->setShortcut(QKeySequence("Ctrl+A"));
        menu->addAction(x1);
        menu->addAction(x2);

        menu->actions()[0]->setCheckable(true);
        menu->actions()[0]->setChecked(true);


        menu->exec(event->globalPos(), Es::MenuAnimationType::FADE_IN_PULL_UP);
        connect(menu, &EsRoundMenu::closedSignal, [=]()
        {
            qDebug() << "closedSignal";
            menu->deleteLater();
            x1->deleteLater();
            x2->deleteLater();
        });
    };

public:
    Q_SLOT void on_btn2_clicked()
    {
        // auto a = QVariant::fromValue(Es::Theme::Theme_DARK);
        // esConfig.setValue(&EsConfig::themeMode, a);
        esConfig.toMap(true);
        auto msg = EsInfoBar::success(
            "标题",
            "My name ",
            Qt::Vertical,
            true,
            1000,
            Es::InfoBarPosition::BOTTOM_RIGHT,
            this
        );
        // auto btn = new EsPushButton("Action", msg);
        // msg->addWidget(btn);
        // msg->show();
    }

    Q_SLOT void on_btn1_clicked()
    {
        QMessageBox::information(this, "123", "456");
    }

    Q_SLOT void on_themeChanged()
    {
        qDebug() << "主题变化";
    }


    EsPushButton* btn1;
    EsPushButton* btn2;
    EsPushButton* btn3;
    EsPushButton* btn4;
    EsPushButton* btn5;
    EsCheckBox* btn6;
    EsToolButton* btn7;
    EsSwitchButton* btn8;
    EsListWidget* listWidget;
    EsComboBox* combox;
    EsEditableComboBox* editCombox;

    ~MyClass() override
    {
        qDebug() << "~MyClass()";
    }

    QCompleter* completer;
};

class TableDemo : public EsWidget
{
    Q_OBJECT

public:
    TableDemo()
    {
        tableView = new EsTableWidget(this);
        auto btn = new EsPushButton("测试按钮", this);
        auto hBoxLayout = new QVBoxLayout(this);
        hBoxLayout->addWidget(btn);
        connect(btn, &EsPushButton::clicked, this, &TableDemo::on_btn_clickwed);
        tableView->setBorderVisible(true);
        tableView->setBorderRadius(8);

        tableView->setWordWrap(false);
        tableView->setRowCount(60);
        tableView->setColumnCount(5);
        QList<QList<QString>> songInfos = {
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"かばん", "aiko", "かばん", "2004", "5:04"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
            {"爱你", "王心凌", "爱你", "2004", "3:39"},
        };
        for (int i = 0; i < songInfos.size(); ++i)
        {
            for (int j = 0; j < songInfos[i].size(); ++j)
            {
                auto item = new QTableWidgetItem(songInfos[i][j]);
                if (i == 1 && j == 1)
                {
                    auto itemCombo = new EsTableWidgetComboItem(QStringList{"item1", "item2", "item3"});
                    itemCombo->setPlaceholderText("测试");
                    // itemCombo->setEditable(true);
                    tableView->setItem(i, j, itemCombo);
                    continue;
                }
                if (i == 1 && j == 2)
                {
                    auto itemCombo = new EsTableWidgetComboItem(QStringList{"item1", "item2", "item3"});
                    itemCombo->setPlaceholderText("测试");
                    itemCombo->setEditable(true);
                    tableView->setItem(i, j, itemCombo);
                    continue;
                }
                tableView->setItem(i, j, item);
            }
        }
        tableView->verticalHeader()->hide();
        tableView->setHorizontalHeaderLabels(QList<QString>{"Title", "Artist", "Album", "Year", "Duration"});
        tableView->resizeColumnsToContents();
        hBoxLayout->setContentsMargins(50, 30, 50, 30);
        hBoxLayout->addWidget(tableView);
    }
    Q_SLOT void on_btn_clickwed()
    {
        auto item = (EsTableWidgetComboItem*)tableView->item(1,1);
        qDebug()<< "index: " << item->currentIndex();
        item->disableOption(1);
    }
private:
    EsTableWidget* tableView;
};

class MyClass2:public EsWindowBase
{
    Q_OBJECT
    public:
    MyClass2(QWidget* parent=nullptr)
    {
        MyClass* wid = new MyClass(this);
        addSubInterface(wid);
    };
};
int main(int argc, char* argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    // auto wid = new MyClass();
    // wid->show();

    esConfig.load("config/config.json");
    MyClass wid;
    wid.setFixedSize(400,600);
    wid.show();
    qDebug()<<wid.size();


    TableDemo demo;
    demo.show();
    return QApplication::exec();
}


#include "main.moc"
