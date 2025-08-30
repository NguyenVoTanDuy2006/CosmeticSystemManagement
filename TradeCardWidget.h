#pragma once
#include <QFrame>

class QLabel;

class TradeCardWidget : public QFrame {
    Q_OBJECT
public:
    explicit TradeCardWidget(QWidget* parent = nullptr);

    // Card cho giao dịch nhập (IN)
    void setInCard (const QString& time, const QString& pid,
                   const QString& lot, int qty, qlonglong money);
    // Card cho giao dịch bán ra (OUT)
    void setOutCard(const QString& time, const QString& pid,
                    const QString& client, int qty, qlonglong money);

    // Meta
    void    setTradeId(const QString& id) { id_ = id; }
    QString tradeId()   const             { return id_; }

    // chọn/bỏ chọn
    void setSelected(bool on);
    bool isSelected() const { return selected_; }

signals:
    void toggled(const QString& id, bool selected);

protected:
    void mousePressEvent(QMouseEvent* e) override;

private:
    void buildUI();
    static QString moneyVi(qlonglong v);

    QLabel *vTime{}, *vPid{}, *vExtra{}, *vQty{}, *vMoney{}, *kExtra{};
    QString id_;
    bool selected_ = false;
};
