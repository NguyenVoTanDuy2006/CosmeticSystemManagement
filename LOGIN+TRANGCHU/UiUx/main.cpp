#include "loginpage.h"
#include "mainmenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginPage w;  // nếu chưa cần
    MainMenu h;
    w.show();

    return a.exec();
}
