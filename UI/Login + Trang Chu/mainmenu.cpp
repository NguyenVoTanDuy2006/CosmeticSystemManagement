#include "mainmenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    ui->groupBox->installEventFilter(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

bool MainMenu::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->groupBox && event->type() == QEvent::MouseButtonPress) {
        // Xử lý click tại đây
        qDebug() << "GroupBox được bấm!";
        return true;
    }
    return QWidget::eventFilter(watched, event);
}
