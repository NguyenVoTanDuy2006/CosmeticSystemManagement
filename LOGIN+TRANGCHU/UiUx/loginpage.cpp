#include "loginpage.h"
#include <QPushButton>
#include "mainmenu.h"
#include "app_router.h"
#include <QIcon>
#include <QLineEdit>
#include <QAction>
LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
//, mainmenu(m)
{
    QIcon eyeIcon(":/icons/Resources/hide.png");
    QIcon eyeIconHide(":/icons/Resources/show.png");
    ui->setupUi(this);
    ui->accountAnnouncementLabel->setText("");
    ui->wrongPasswordLabel->setText("");
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setClearButtonEnabled(true);


    connect(ui->usernameEdit, &QLineEdit::textEdited, this, [=](const QString &text)
            {
                if (text != this->account)
                {
                    if (text == "")
                    {
                        ui->accountAnnouncementLabel->setText("Tên đăng nhập không được để trống");

                    }
                    else ui->accountAnnouncementLabel->setText("Tên đăng nhập không tồn tại");
                    ui->accountAnnouncementLabel->setStyleSheet("color: red; font-size: 13px;");


                }

                else
                {
                    ui->accountAnnouncementLabel->setText("✔");
                    ui->accountAnnouncementLabel->setStyleSheet("color: #28a745; font-size: 24px; font-weight: bold;");

                }
            });



    connect(ui->usernameEdit, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (text.isEmpty() || text == "")
                {

                    ui->usernameEdit->setStyleSheet("color: gray;");
                }
                else
                {
                    ui->usernameEdit->setStyleSheet("color: #37B606;");
                }
            });

    connect(ui->passwordEdit, &QLineEdit::textChanged, this, [=](const QString &text)
            {
                if (text.isEmpty() || text == "")
                {

                    ui->passwordEdit->setStyleSheet("color: gray;");

                }
                else
                {
                    ui->passwordEdit->setStyleSheet("color: #37B606;");
                }
            });

    connect(ui->pushButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    QAction *eyeAction = new QAction(eyeIcon, "", this);
    ui->passwordEdit->addAction(eyeAction, QLineEdit::TrailingPosition);

    connect(eyeAction, &QAction::triggered, this, [=]()
            {
                if (ui->passwordEdit->echoMode() == QLineEdit::Password)
                {
                    ui->passwordEdit->setEchoMode(QLineEdit::Normal);
                    eyeAction->setIcon(eyeIconHide);
                }
                else
                {
                    ui->passwordEdit->setEchoMode(QLineEdit::Password);
                    eyeAction->setIcon(eyeIcon);
                }
            });


}


LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::handleLogin()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    if (username == this->account && password == this->password)
    {
        //ui->wrongPasswordLabel->setText("");
        //mainMenu = new MainMenu();
        //mainMenu->show();
        //this->close();
        emit loginSucceeded();
    }
    else
    {
        if (username == "")
        {
            ui->accountAnnouncementLabel->setText("Tên đăng nhập không được để trống");
        }
        else ui->accountAnnouncementLabel->setText("Tên đăng nhập không tồn tại");


        if (password != this->password)
        {
            ui->wrongPasswordLabel->setText("Sai mật khẩu");
        }
    }
}
