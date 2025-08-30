#ifndef MAINMENU_H
#define MAINMENU_H
//#include sp lsnx.h
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);

    ~MainMenu();

private:
    Ui::MainMenu *ui;

};

#endif // MAINMENU_H
