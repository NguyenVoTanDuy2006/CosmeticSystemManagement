// app_router.h
#pragma once
#include <QObject>
#include <QStackedWidget>

class AppRouter : public QObject {
    Q_OBJECT
public:
    enum Page { Menu=0, SP=1, LSNX=2 };

    static AppRouter& getInstance() { static AppRouter i; return i; }
    void init(QStackedWidget* s) { stack = s; }

public slots:
    void goMenu() { if (stack) stack->setCurrentIndex(Menu); }
    void goSP()   { if (stack) stack->setCurrentIndex(SP); }
    void goLSNX() { if (stack) stack->setCurrentIndex(LSNX); }

private:
    AppRouter(QObject* p=nullptr): QObject(p) {}
    QStackedWidget* stack = nullptr;
};
