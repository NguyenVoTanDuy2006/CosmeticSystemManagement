#include "LOGIN+TRANGCHU/UiUx/loginpage.h"
#include "LOGIN+TRANGCHU/UiUx/mainmenu.h"
#include "CosmeticManagement/productManager.h"
#include "TradeManagement/TradeManager.h"
#include "Router/app_router.h"
#include "LSNX.h"
#include "sp.h"
#include <QApplication>
#include <QStackedWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    productManager::getInstance().readData();
    TradeManager::getInstance().readData();

    // ----- 1) Tạo stack nhưng CHƯA show -----
    auto *stack = new QStackedWidget;
    stack->setStyleSheet("QStackedWidget { background-color: white; }");
    stack->setObjectName("AppStack");

    auto *wMenu = new MainMenu;
    auto *wSP   = new sp;
    auto *wLSNX = new LSNX;

    stack->addWidget(wMenu);  // index 0 = Menu (mặc định)
    stack->addWidget(wSP);    // index 1 = SP
    stack->addWidget(wLSNX);  // index 2 = LSNX

    AppRouter::getInstance().init(stack);
    AppRouter::getInstance().goMenu();   // vào Menu

    // ----- 2) Tạo Login (cửa sổ riêng) -----
    auto *login = new LoginPage;
    login->setAttribute(Qt::WA_DeleteOnClose);
    login->setWindowFlag(Qt::WindowMaximizeButtonHint, false); // không cho maximize
    login->setFixedSize(800, 480);    // cố định nếu bạn muốn
    login->setWindowTitle("The Cosmetic Warehouse Management Application");
    login->show();                                              // hiện Login trước

    // ----- 3) Wiring: login ok -> ẩn login, show stack -----
    QObject::connect(login, &LoginPage::loginSucceeded, [=]{
        login->close();
        // show stack như cửa sổ chính:
        stack->resize(1280, 800);
        stack->setWindowTitle("The Cosmetic Warehouse Management Application");
        stack->show();          // hiển thị app chính (đang ở Menu)
    });

    return a.exec();
}
