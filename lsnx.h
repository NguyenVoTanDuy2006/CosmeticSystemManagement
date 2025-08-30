#pragma once
#include <QMainWindow>
#include <QSet>
#include <QDateTime>
#include <memory>
#include <qcheckbox.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LSNX; }
QT_END_NAMESPACE

class TradeCardWidget;

// BACKEND
class TradeManager;
class Trade;
class TradeIn;
class TradeOut;

class LSNX : public QMainWindow {
    Q_OBJECT
public:
    explicit LSNX(QWidget *parent = nullptr);
    ~LSNX();

private:
    Ui::LSNX *ui;
    TradeManager& tm_;
    QSet<QString> selectedIds_;

    //normal page
    void initScrollAreas();
    void seedFromManager();
    void clearLists();
    void addCardTo(QWidget* container, TradeCardWidget* card);
    static QString fmtTime(const QDateTime& dt);

    // Add TradeIn page
    void showAddInPage();
    void resetAddInForm();
    bool validateAddInForm(QString& err) const;
    bool submitAddIn();
    static QDate parseDate(const QString& s, const QString& fmt = "dd/MM/yyyy");

    // Add TradeOut
    void resetAddOutForm();
    bool validateAddOutForm(QString& err) const;
    bool submitAddOut();
    static QDateTime dtFromDdMMyyyy(const QString& s) {
        // (để sau dùng nếu muốn mở rộng ngày tháng)
        return QDateTime(QDate::fromString(s.trimmed(), "dd/MM/yyyy"), QTime(0,0,0));
    }

    //Delete trade
    QHash<QObject*, QCheckBox*> delClickMap_;
    // tạo ScrollArea cho trang Xóa
    void initDeleteScrollAreas();
    // render lại danh sách thẻ trên trang Xóa
    void seedForDelete();
    // bọc card vào 1 khung có checkbox, click wrap/card sẽ toggle
    void addWrappedDeleteCard(QWidget* container, TradeCardWidget* card, const QString& tradeId);
    // dọn UI trang xóa
    void clearDeleteLists();
    bool eventFilter(QObject* obj, QEvent* ev) override;

    //filter
    void clearFilterLists(); // dọn 2 cột của trang filter
    void renderFiltered(const std::vector<std::shared_ptr<Trade>>& trades);

private slots:
    void onAddTradeInClicked();   // từ trang normal
    void onSaveAddTradeIn();      // nút THÊM
    // mở trang add out từ trang normal
    void onAddTradeOutClicked();
    // bấm THÊM trên trang add out
    void onSaveAddTradeOut();
    void onEnterDeletePage();      // từ trang normal → trang Xóa
    void onConfirmDeleteClicked(); // nút Xoa trên trang Xóa
    void onOpenFilter();     // btnFilter (normal)
    void onApplyFilter();    // btnApply (Xem kết quả)
    void onCloseFilter();    // btnClose (X)

    void on_back_to_normal();  // quay về trang normal

    void on_btnHomeNormal_clicked();
    void on_btnHomeAddTradeIn_clicked();
    void on_btnHomeAddTradeOut_clicked();
    void on_btnHomeDeleteTrade_clicked();
    void on_btnHomeFilterTrade_clicked();
    void on_btnSPNormal_clicked();
    void on_btnSPAddTradeIn_clicked();
    void on_btnSPAddTradeOut_clicked();
    void on_btnSPDeleteTrade_clicked();
    void on_btnSPFilterTrade_clicked();
};
