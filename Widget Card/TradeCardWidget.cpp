#include "TradeCardWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QLocale>
#include <QMouseEvent>

namespace {
static QLabel* makeKey(const QString& s, QWidget* p){
    auto *l = new QLabel(s, p);
    l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l->setStyleSheet("font-weight:800; font-size:18px; color:#0B49C4;");
    return l;
}
static QLabel* makeVal(QWidget* p){
    auto *l = new QLabel(p);
    l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l->setStyleSheet("font-weight:800; font-size:22px; color:#0B49C4;");
    return l;
}
}

TradeCardWidget::TradeCardWidget(QWidget* parent) : QFrame(parent) {
    setObjectName("tradeCard");
    setAttribute(Qt::WA_StyledBackground, true); // đảm bảo nền theo stylesheet

    setStyleSheet(R"(
      /* Card */
      QFrame#tradeCard {
        border-radius: 22px;
        padding: 14px;
        background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                                    stop:0    #CDEFD6,
                                    stop:0.48 #A6D0FF,
                                    stop:1    #7AA2FF);
        border: 1px solid rgba(13,87,208,0.18);
      }
      QFrame#tradeCard[selected="true"] {
        border: 2px solid #0B57D0;
      }


      QFrame#tradeCard QLabel,
      QFrame#tradeCard QLineEdit {
        background: transparent;   /* bỏ nền trắng */
        border: none;              /* bỏ viền */
        color: #0A37B8;            /* xanh như mock */
        font-weight: 700;
        padding: 2px 6px;
      }
    )");

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0,0,0,35));
    setGraphicsEffect(shadow);

    buildUI();
}


void TradeCardWidget::buildUI(){
    auto *g = new QGridLayout(this);
    g->setContentsMargins(18,16,18,16);
    g->setHorizontalSpacing(18);
    g->setVerticalSpacing(10);

    int r = 0;
    g->addWidget(makeKey("Thời gian:",   this), r,0); vTime  = makeVal(this); g->addWidget(vTime,  r,1); r++;
    g->addWidget(makeKey("Mã sản phẩm:", this), r,0); vPid   = makeVal(this); g->addWidget(vPid,   r,1); r++;
    kExtra = makeKey("Số lô:", this);                      g->addWidget(kExtra, r,0);
    vExtra = makeVal(this);                                g->addWidget(vExtra, r,1); r++;
    g->addWidget(makeKey("Số lượng:",    this), r,0); vQty   = makeVal(this); g->addWidget(vQty,   r,1); r++;
    g->addWidget(makeKey("Số tiền:",     this), r,0); vMoney = makeVal(this); g->addWidget(vMoney, r,1);
}

QString TradeCardWidget::moneyVi(qlonglong v){
    QLocale vi(QLocale::Vietnamese, QLocale::Vietnam);
    return vi.toString(v) + QChar(0x0111); // ký tự 'đ'
}

void TradeCardWidget::setInCard(const QString& time, const QString& pid,
                                const QString& lot, int qty, qlonglong money){
    kExtra->setText("Số lô:");
    vTime->setText(time);
    vPid->setText(pid);
    vExtra->setText(lot);
    vQty->setText(QString::number(qty));
    vMoney->setText(moneyVi(money));
}

void TradeCardWidget::setOutCard(const QString& time, const QString& pid,
                                 const QString& client, int qty, qlonglong money){
    kExtra->setText("Khách hàng:");
    vTime->setText(time);
    vPid->setText(pid);
    vExtra->setText(client);
    vQty->setText(QString::number(qty));
    vMoney->setText(moneyVi(money));
}

void TradeCardWidget::setSelected(bool on){
    selected_ = on;
    setProperty("selected", on ? "true" : "false");
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void TradeCardWidget::mousePressEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton){
        setSelected(!selected_);
        emit toggled(id_, selected_);
    }
    QFrame::mousePressEvent(e);
}
