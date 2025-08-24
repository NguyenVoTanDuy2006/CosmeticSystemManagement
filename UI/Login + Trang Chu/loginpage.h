#ifndef LOGINPAGE_H
#define LOGINPAGE_H
#include <QString>
#include "mainmenu.h"
#include "./ui_login_form_gradient.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class LoginForm;
}
QT_END_NAMESPACE

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private:
    MainMenu *mainMenu = nullptr;
    Ui::LoginForm *ui;
    QString password = "password";
    QString account = "account";
    void handleLogin();
};
#endif // LOGINPAGE_H
