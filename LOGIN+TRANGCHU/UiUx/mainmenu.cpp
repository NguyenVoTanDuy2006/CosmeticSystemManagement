#include "mainmenu.h"
#include "ui_MainMenu.h"
#include "../app_router.h"

static void relaxFixed(QWidget* w) {
    if (!w) return;
    // bỏ fixed min/max
    w->setMinimumSize(QSize(0,0));
    w->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    // đổi policy sang Expanding nếu đang Fixed
    QSizePolicy sp = w->sizePolicy();
    if (sp.horizontalPolicy() == QSizePolicy::Fixed)   sp.setHorizontalPolicy(QSizePolicy::Expanding);
    if (sp.verticalPolicy()   == QSizePolicy::Fixed)   sp.setVerticalPolicy(QSizePolicy::Expanding);
    w->setSizePolicy(sp);
    // duyệt con
    const auto children = w->findChildren<QWidget*>();
    for (QWidget* c : children) {
        // đừng phá QToolButton nhỏ… tuỳ bạn lọc theo objectName nếu cần
        relaxFixed(c);
    }
}

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    relaxFixed(this);
    ui->goSP->setFixedSize(280, 211);
    ui->goLSNX->setFixedSize(280, 211);
    this->setWindowTitle("The Cosmetic Warehouse Management Application");
    ui->goSP->installEventFilter(this);
    ui->goLSNX->installEventFilter(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

bool MainMenu::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->goSP && event->type() == QEvent::MouseButtonPress) {
        AppRouter::getInstance().goSP();
        qDebug() << "GroupBox được bấm!";
        return true;
    }
    else if (watched == ui->goLSNX && event->type() == QEvent::MouseButtonPress) {
        AppRouter::getInstance().goLSNX();
    }
    return QWidget::eventFilter(watched, event);
}
