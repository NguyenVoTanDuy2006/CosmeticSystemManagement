// lsnx.cpp
#include "LSNX.h"
#include "ui_LSNX.h"
#include "TradeCardWidget.h"
#include "app_router.h"

// ==== BACKEND ====
#include "TradeManagement/TradeManager.h"
#include "Trade/TradeIn.h"
#include "Trade/TradeOut.h"
#include "TradeManagement/TradeFilterConditions.h"
#include "CosmeticManagement/productManager.h"

// Qt
#include <QApplication>
#include <QSignalBlocker>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>
#include <QCheckBox>
#include <QFileInfo>
#include <QDebug>
// validators + regex + completer
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QCompleter>


// std
#include <memory>

namespace {
static QVBoxLayout* ensureVBox(QWidget* container) {
    auto *lay = qobject_cast<QVBoxLayout*>(container->layout());
    if (!lay) {
        lay = new QVBoxLayout(container);
        lay->setContentsMargins(8,8,8,8);
        lay->setSpacing(8);
        lay->addStretch();                // spacer ở cuối
    }
    return lay;
}
static void ensureScroll(QScrollArea* sc, QWidget* container){
    if (sc->widget() != container) sc->setWidget(container);
    sc->setWidgetResizable(true);
    sc->setFrameShape(QFrame::NoFrame);
    sc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
} // anon

LSNX::LSNX(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LSNX), tm_(TradeManager::getInstance())
{
    ui->setupUi(this);

    this->setWindowTitle("The Cosmetic Warehouse Management Application");

    // ====== signals ======
    connect(ui->btnAddTradeIn,     &QPushButton::clicked, this, &LSNX::onAddTradeInClicked);
    connect(ui->btnSaveAddTradeIn, &QPushButton::clicked, this, &LSNX::onSaveAddTradeIn);

    // ====== chuẩn hoá input (QLineEdit) ======
    // ngày: dd/MM/yyyy
    ui->nsxAddTradeIn->setInputMask("99/99/9999;_");
    ui->hsdAddTradeIn->setInputMask("99/99/9999;_");
    ui->nsxAddTradeIn->setPlaceholderText("dd/MM/yyyy");
    ui->hsdAddTradeIn->setPlaceholderText("dd/MM/yyyy");

    // qty: số nguyên dương
    ui->quantityAddTradeIn->setValidator(new QIntValidator(1, 100000000, ui->quantityAddTradeIn));
    // tiền: số nguyên dương
    ui->tienHangAddTradeIn->setValidator(
        new QRegularExpressionValidator(QRegularExpression("^\\d{1,12}$"), ui->tienHangAddTradeIn));

    // autocomplete mã SP
    QStringList ids;
    for (const auto& p : productManager::getInstance().getProducts()) ids << p.getID();
    if (!ids.isEmpty()) {
        auto *cmp = new QCompleter(ids, this);
        cmp->setCaseSensitivity(Qt::CaseInsensitive);
        ui->maSPAddTradeIn->setCompleter(cmp);
    }
    // ----- mở trang AddOut từ trang normal -----
    connect(ui->btnAddTradeOut,    &QPushButton::clicked, this, &LSNX::onAddTradeOutClicked);
    // ----- lưu trên trang AddOut -----
    connect(ui->btnSaveAddTradeOut,&QPushButton::clicked, this, &LSNX::onSaveAddTradeOut);

    // ----- validator -----
    ui->quantityAddTradeOut->setValidator(new QIntValidator(1, 100000000, ui->quantityAddTradeOut));

    // Tổng hóa đơn là ô tự tính -> không cho sửa tay (nếu là QLineEdit)
    ui->tienHangAddTradeOut->setReadOnly(true);

    // ----- autocomplete Mã SP -----
    QStringList idsOut;
    for (const auto& p : productManager::getInstance().getProducts()) idsOut << p.getID();
    if (!idsOut.isEmpty()) {
        auto *cmp = new QCompleter(idsOut, this);
        cmp->setCaseSensitivity(Qt::CaseInsensitive);
        ui->maSPAddTradeOut->setCompleter(cmp);
    }

    // ----- tự tính tổng tiền khi thay đổi Mã SP hoặc Số lượng -----
    auto recalcTotal = [this]{
        const QString pid = ui->maSPAddTradeOut->text().trimmed();
        bool okQty=false;
        const int qty = ui->quantityAddTradeOut->text().toInt(&okQty);
        if (pid.isEmpty() || !okQty || qty <= 0) {
            ui->tienHangAddTradeOut->setText("// tự động: đơn giá * SL");
            return;
        }
        int price = -1;
        for (const auto& p : productManager::getInstance().getProducts()) {
            if (p.getID() == pid) { price = p.getInfo().price; break; }
        }
        if (price < 0) {
            ui->tienHangAddTradeOut->setText("// SP không tồn tại");
            return;
        }
        const qlonglong total = 1LL * price * qty;
        ui->tienHangAddTradeOut->setText(QString::number(total)); // chỉ số, chưa format VND
    };
    connect(ui->maSPAddTradeOut,     &QLineEdit::textChanged, this, recalcTotal);
    connect(ui->quantityAddTradeOut, &QLineEdit::textChanged, this, recalcTotal);

    initScrollAreas();
    initDeleteScrollAreas();
    connect(ui->btnDeleteTrade,     &QPushButton::clicked, this, &LSNX::onEnterDeletePage);
    connect(ui->btnSaveDeleteTrade, &QPushButton::clicked, this, &LSNX::onConfirmDeleteClicked);

    //filter
    connect(ui->btnFilter,   &QPushButton::clicked, this, &LSNX::onOpenFilter);
    connect(ui->btnApply,    &QPushButton::clicked, this, &LSNX::onApplyFilter);
    connect(ui->btnClose,    &QPushButton::clicked, this, &LSNX::onCloseFilter);
    ui->dateFrom->setDisplayFormat("dd/MM/yyyy");
    ui->dateTo->setDisplayFormat("dd/MM/yyyy");
    ui->dateFrom->setDate(QDate::currentDate().addMonths(-1));
    ui->dateTo->setDate(QDate::currentDate());


    // Kiểm tra file Data/TradeData/TradeList.txt ở cạnh .exe
    const QString dataPath = QCoreApplication::applicationDirPath() + "/Data/TradeData/TradeList.txt";
    qDebug() << "[DATA] expecting:" << dataPath;
    if (!QFileInfo::exists(dataPath)) {
        QMessageBox::warning(this, "Thiếu dữ liệu",
                             "Không tìm thấy Data/TradeData/TradeList.txt cạnh file .exe.\n"
                             "Hãy đặt đúng thư mục Data vào: \n" + dataPath);
    }

    if (auto *btn = ui->uiAddTradeIn->findChild<QPushButton*>("btnBackAddTradeIn"))
        connect(btn, &QPushButton::clicked, this, &LSNX::on_back_to_normal);

    if (auto *btn = ui->uiAddTradeOut->findChild<QPushButton*>("btnBackAddTradeOut"))
        connect(btn, &QPushButton::clicked, this, &LSNX::on_back_to_normal);

    if (auto *btn = ui->uiDeleteTrade->findChild<QPushButton*>("btnBackDeleteTrade"))
        connect(btn, &QPushButton::clicked, this, &LSNX::on_back_to_normal);

    seedFromManager();
}

LSNX::~LSNX(){ delete ui; }

void LSNX::initScrollAreas(){
    // Các objectName đúng với .ui bạn đang dùng
    ensureScroll(ui->scrollIn,  ui->scrollInContainer);
    ensureScroll(ui->scrollOut, ui->scrollOutContainer);
}

QString LSNX::fmtTime(const QDateTime& dt){
    // theo mockup: dd/MM/yyyy  hh:mm
    return dt.toString("dd/MM/yyyy  hh:mm");
}

void LSNX::addCardTo(QWidget* container, TradeCardWidget* card){
    auto *lay = ensureVBox(container);
    lay->insertWidget(lay->count()-1, card);
    connect(card, &TradeCardWidget::toggled, this,
            [this](const QString& id, bool on){
                if (on) selectedIds_.insert(id); else selectedIds_.remove(id);
            });
}

void LSNX::clearLists(){
    auto wipe = [](QWidget* container){
        if (auto *lay = qobject_cast<QVBoxLayout*>(container->layout())){
            while (lay->count() > 1){
                auto *it = lay->takeAt(0);
                if (auto *w = it->widget()) w->deleteLater();
                delete it;
            }
        }
    };
    wipe(ui->scrollInContainer);
    wipe(ui->scrollOutContainer);
    selectedIds_.clear();
}

void LSNX::seedFromManager() {
    clearLists();

    std::vector<std::shared_ptr<baseTradeFilterCondition>> conds;   // rỗng = không lọc gì
    auto trades = tm_.getFilteredTrades(conds); // trả về toàn bộ

    for (const auto& sp : trades) {
        if (!sp) continue;
        const QString qid  = sp->getID();
        const QString pid  = sp->getProductID();
        const int     qty  = sp->getQuantity();
        const QString time = fmtTime(sp->getTradeTime());

        if (auto inT = std::dynamic_pointer_cast<TradeIn>(sp)) {
            auto *card = new TradeCardWidget;
            card->setTradeId(qid);
            card->setInCard(time, pid, inT->getLotID(), qty,
                            static_cast<qlonglong>(inT->getCapital()));
            addCardTo(ui->scrollInContainer, card);
        } else if (auto outT = std::dynamic_pointer_cast<TradeOut>(sp)) {
            auto *card = new TradeCardWidget;
            card->setTradeId(qid);
            card->setOutCard(time, pid, outT->getClient().name, qty,
                             static_cast<qlonglong>(outT->getRevenue()));
            addCardTo(ui->scrollOutContainer, card);
        }
    }
}

//addTradeIn
QDate LSNX::parseDate(const QString& s, const QString& fmt) {
    const QDate d = QDate::fromString(s.trimmed(), fmt);
    return d.isValid() ? d : QDate(); // invalid => QDate()
}

static QDateTime makeDT(const QString& s) {
    QDate d = QDate::fromString(s.trimmed(), "dd/MM/yyyy");
    return QDateTime(d, QTime(0,0,0));
}

void LSNX::onAddTradeInClicked() {
    resetAddInForm();
    // chỉ hiển thị, thời gian trade thật sẽ lấy lúc bấm THÊM
    ui->timeAddTradeIn->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy  HH:mm"));
    ui->stackedWidget->setCurrentWidget(ui->uiAddTradeIn);
}

void LSNX::resetAddInForm() {
    ui->maSPAddTradeIn->clear();
    ui->nsxAddTradeIn->clear();
    ui->hsdAddTradeIn->clear();
    ui->quantityAddTradeIn->clear();
    ui->tienHangAddTradeIn->clear();
    ui->timeAddTradeIn->clear();
}

bool LSNX::validateAddInForm(QString& err) const {
    const QString pid = ui->maSPAddTradeIn->text().trimmed();
    if (pid.isEmpty()) { err = "Chưa nhập Mã sản phẩm."; return false; }

    const QDate nsx = parseDate(ui->nsxAddTradeIn->text());
    const QDate hsd = parseDate(ui->hsdAddTradeIn->text());
    if (!nsx.isValid()) { err = "Ngày sản xuất (NSX) không hợp lệ (dd/MM/yyyy)."; return false; }
    if (!hsd.isValid()) { err = "Hạn sử dụng (HSD) không hợp lệ (dd/MM/yyyy)."; return false; }
    if (hsd <= nsx)     { err = "HSD phải lớn hơn NSX."; return false; }

    bool okQty=false, okMoney=false;
    const int qty     = ui->quantityAddTradeIn->text().toInt(&okQty);
    const int capital = ui->tienHangAddTradeIn->text().toInt(&okMoney);
    if (!okQty || qty <= 0)     { err = "Số lượng phải là số nguyên > 0."; return false; }
    if (!okMoney || capital < 0){ err = "Tổng tiền hàng phải là số ≥ 0."; return false; }

    // kiểm tra tồn tại Mã SP
    bool exists = false;
    for (const auto& p : productManager::getInstance().getProducts())
        if (p.getID() == pid) { exists = true; break; }
    if (!exists) { err = "Mã sản phẩm không tồn tại."; return false; }

    return true;
}

bool LSNX::submitAddIn() {
    const QString pid = ui->maSPAddTradeIn->text().trimmed();

    LotInfo lot(
        makeDT(ui->nsxAddTradeIn->text()),
        makeDT(ui->hsdAddTradeIn->text()),
        ui->quantityAddTradeIn->text().toInt()
        );

    const int capital = ui->tienHangAddTradeIn->text().toInt();

    if (!tm_.addTrade(pid, lot, capital)) {
        return false;
    }
    tm_.writeData();
    productManager::getInstance().saveData();
    return true;
}

void LSNX::onSaveAddTradeIn() {
    QString err;
    if (!validateAddInForm(err)) {
        QMessageBox::warning(this, "Thiếu thông tin", err);
        return;
    }
    if (!submitAddIn()) {
        QMessageBox::critical(this, "Lỗi", "Không thể thêm lịch sử nhập hàng (kiểm tra Mã SP / dữ liệu).");
        return;
    }
    //QMessageBox::information(this, "Thành công", "Đã thêm giao dịch nhập & cập nhật lô sản phẩm.");
    ui->stackedWidget->setCurrentWidget(ui->normal);
    seedFromManager(); // render lại thẻ
}

//addTradeOut
void LSNX::onAddTradeOutClicked() {
    resetAddOutForm();
    ui->timeAddTradeOut->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy  HH:mm"));
    ui->stackedWidget->setCurrentWidget(ui->uiAddTradeOut);
}

void LSNX::resetAddOutForm() {
    ui->maSPAddTradeOut->clear();
    ui->khachHangAddTradeOut->clear();
    ui->quantityAddTradeOut->clear();
    ui->tienHangAddTradeOut->clear();
    ui->timeAddTradeOut->clear();
}

bool LSNX::validateAddOutForm(QString& err) const {
    const QString pid = ui->maSPAddTradeOut->text().trimmed();
    if (pid.isEmpty()) { err = "Chưa nhập Mã sản phẩm."; return false; }

    const QString customer = ui->khachHangAddTradeOut->text().trimmed();
    if (customer.isEmpty()) { err = "Chưa nhập Khách hàng."; return false; }

    bool okQty=false;
    const int qty = ui->quantityAddTradeOut->text().toInt(&okQty);
    if (!okQty || qty <= 0) { err = "Số lượng bán phải là số nguyên > 0."; return false; }

    return true;
}

bool LSNX::submitAddOut() {
    const QString pid      = ui->maSPAddTradeOut->text().trimmed();
    const QString customer = ui->khachHangAddTradeOut->text().trimmed();
    const int qty          = ui->quantityAddTradeOut->text().toInt();

    // Lấy đơn giá để tính revenue (đồng bộ với label tổng tiền)
    int price = -1;
    for (const auto& p : productManager::getInstance().getProducts())
        if (p.getID() == pid) { price = p.getInfo().price; break; }

    if (price < 0) {
        QMessageBox::critical(this, "Lỗi", "Mã sản phẩm không tồn tại.");
        return false;
    }
    const int revenue = static_cast<int>(1LL * price * qty);

    Client cl;
    cl.name = customer;

    // Gọi backend tạo TradeOut (FIFO xuất kho sẽ do backend xử)
    if (!tm_.addTrade(pid, qty, revenue, cl)) {
        QMessageBox::critical(this, "Lỗi",
                              "Không thể tạo giao dịch bán.\n"
                              "• Tồn kho không đủ hoặc Mã SP không hợp lệ.");
        return false;
    }

    // Lưu dữ liệu
    tm_.writeData();
    productManager::getInstance().saveData();
    return true;
}

void LSNX::onSaveAddTradeOut() {
    QString err;
    if (!validateAddOutForm(err)) {
        if (err.isEmpty()) err = "Dữ liệu không hợp lệ.";
        QMessageBox::warning(this, "Thiếu thông tin", err);
        return;
    }
    if (!submitAddOut()) return;

    //QMessageBox::information(this, "Thành công", "Đã thêm hóa đơn bán hàng.");
    ui->stackedWidget->setCurrentWidget(ui->normal);
    seedFromManager(); // render lại danh sách thẻ
}

//Delete Trade
void LSNX::initDeleteScrollAreas() {
    // đổi đúng objectName theo .ui của bạn
    ensureScroll(ui->scrollIn_3,  ui->scrollInContainer_3);
    ensureScroll(ui->scrollOut_3, ui->scrollOutContainer_3);
}

void LSNX::clearDeleteLists(){
    auto wipe = [](QWidget* c){
        if (auto *lay = qobject_cast<QVBoxLayout*>(c->layout())){
            while (lay->count() > 1){
                auto *it = lay->takeAt(0);
                if (auto *w = it->widget()) w->deleteLater();
                delete it;
            }
        }
    };
    delClickMap_.clear();
    selectedIds_.clear();
    wipe(ui->scrollInContainer_3);
    wipe(ui->scrollOutContainer_3);
}

void LSNX::onEnterDeletePage() {
    seedForDelete();
    ui->stackedWidget->setCurrentWidget(ui->uiDeleteTrade);
}

void LSNX::seedForDelete(){
    clearDeleteLists();
    ui->btnSaveDeleteTrade->setEnabled(false);

    std::vector<std::shared_ptr<baseTradeFilterCondition>> conds;
    auto trades = tm_.getFilteredTrades(conds);

    for (const auto& sp : trades){
        if (!sp) continue;
        const QString id   = sp->getID();
        const QString pid  = sp->getProductID();
        const int     qty  = sp->getQuantity();
        const QString time = fmtTime(sp->getTradeTime());

        if (auto inT = std::dynamic_pointer_cast<TradeIn>(sp)) {
            auto *card = new TradeCardWidget;
            card->setTradeId(id);
            card->setInCard(time, pid, inT->getLotID(), qty,
                            static_cast<qlonglong>(inT->getCapital()));
            addWrappedDeleteCard(ui->scrollInContainer_3, card, id);

        } else if (auto outT = std::dynamic_pointer_cast<TradeOut>(sp)) {
            auto *card = new TradeCardWidget;
            card->setTradeId(id);
            card->setOutCard(time, pid, outT->getClient().name, qty,
                             static_cast<qlonglong>(outT->getRevenue()));
            addWrappedDeleteCard(ui->scrollOutContainer_3, card, id);
        }
    }
}

void LSNX::addWrappedDeleteCard(QWidget* container,
                                TradeCardWidget* card,
                                const QString& tradeId)
{
    auto *wrap = new QFrame(container);
    wrap->setObjectName("delWrap");
    wrap->setStyleSheet(
        "#delWrap{"
        "  border:1px solid #b7d3ff; border-radius:10px;"
        "  background:qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #eafdf3, stop:1 #c6e6fa);"
        "}"
        "QCheckBox#delChk::indicator{ width:26px;height:26px; }"
        "QCheckBox#delChk::indicator:unchecked{"
        "  border:3px solid #25c158; border-radius:13px; background:transparent;"
        "}"
        "QCheckBox#delChk::indicator:checked{"
        "  border:3px solid #25c158; border-radius:13px; background:#aef0c7;"
        "}"
        );
    auto *h = new QHBoxLayout(wrap);
    h->setContentsMargins(10,8,10,8);
    h->setSpacing(10);

    auto *chk = new QCheckBox(wrap);
    chk->setObjectName("delChk");
    h->addWidget(chk, 0, Qt::AlignTop);

    h->addWidget(card, 1);

    auto *lay = qobject_cast<QVBoxLayout*>(container->layout());
    if (!lay) { lay = new QVBoxLayout(container); lay->addStretch(); }
    lay->insertWidget(lay->count()-1, wrap);

    // tick -> thêm/bớt ID
    connect(chk, &QCheckBox::toggled, this, [this, tradeId](bool on){
        if (on) selectedIds_.insert(tradeId); else selectedIds_.remove(tradeId);
        ui->btnSaveDeleteTrade->setEnabled(!selectedIds_.isEmpty());
    });

    // click wrap/card -> toggle
    delClickMap_.insert(wrap, chk);
    delClickMap_.insert(card, chk);
    wrap->installEventFilter(this);
    card->installEventFilter(this);
}

bool LSNX::eventFilter(QObject* obj, QEvent* ev) {
    if (ev->type() == QEvent::MouseButtonPress) {
        if (auto it = delClickMap_.find(obj); it != delClickMap_.end()) {
            if (QCheckBox* chk = it.value()) chk->toggle();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, ev);
}

#include <QTimer>
#include <QPointer>
#include <QSignalBlocker>
#include <QApplication>

void LSNX::onConfirmDeleteClicked() {
    if (selectedIds_.isEmpty()){
        QMessageBox::information(this,"Xóa","Chọn giao dịch cần xóa trước đã.");
        return;
    }

    std::vector<QString> ids;
    ids.reserve(selectedIds_.size());
    for (const auto& s : selectedIds_) ids.push_back(s);

    // defer để slot click kết thúc hẳn rồi mới đụng UI/backend
    QPointer<LSNX> that(this);
    QTimer::singleShot(0, this, [that, ids]() {
        if (!that) return;

        // gỡ filter & ngắt toàn bộ signal checkbox
        QSet<QCheckBox*> uniqChks;
        for (auto it = that->delClickMap_.cbegin(); it != that->delClickMap_.cend(); ++it) {
            if (QObject* obj = it.key()) obj->removeEventFilter(that);
            if (QCheckBox* chk = it.value()) uniqChks.insert(chk);
        }
        for (QCheckBox* chk : uniqChks) {
            QSignalBlocker blocker(chk);
            QObject::disconnect(chk, nullptr, that, nullptr);
            chk->setEnabled(false);
        }
        that->delClickMap_.clear();

        // dọn UI trang delete
        that->clearDeleteLists();
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

        // backend
        qDebug() << "[DEL] ids:" << ids.size();
        that->tm_.deleteTrade(ids);   // chữ ký B: vector<QString>
        qDebug() << "[DEL] backend ok";
        that->tm_.writeData();
        qDebug() << "[DEL] write ok";

        // render lại & quay về trang normal
        that->seedFromManager();
        that->ui->stackedWidget->setCurrentWidget(that->ui->normal);
    });
}

//filter
void LSNX::onOpenFilter() {
    // reset nhẹ nếu muốn
    // ui->cbIn->setChecked(false);
    // ui->cbOut->setChecked(false);
    clearFilterLists();
    ui->stackedWidget->setCurrentWidget(ui->uiFilterTrade);
}

void LSNX::onCloseFilter() {
    ui->stackedWidget->setCurrentWidget(ui->normal);
}

#include "TradeManagement/TradeFilterConditions.h"

static QDateTime beginOfDay(const QDate& d) { return QDateTime(d, QTime(0,0,0)); }
static QDateTime endOfDay(const QDate& d)   { return QDateTime(d, QTime(23,59,59)); }

void LSNX::onApplyFilter() {
    using std::make_shared;
    std::vector<std::shared_ptr<baseTradeFilterCondition>> conds;

    // --- Lọc theo ngày (chỉ dùng DateTradeCondition; không cần From/End riêng lẻ) ---
    const QDate df = ui->dateFrom->date();
    const QDate dt = ui->dateTo->date();
    if (df.isValid() && dt.isValid()) {
        const auto from = beginOfDay(df);
        const auto to   = endOfDay(dt);
        if (from <= to)
            conds.push_back(make_shared<DateTradeCondition>(from, to));
    }
    // (Nếu muốn hỗ trợ chỉ từ hoặc chỉ đến mà backend KHÔNG có From/End filter:
    //  ta có thể không push gì – coi như không lọc ngày trong case đó.)

    // --- Gọi backend lấy danh sách ---
    auto trades = tm_.getFilteredTrades(conds);

    // --- Đổ ra 2 cột theo tick cbIn / cbOut ---
    renderFiltered(trades);
}

void LSNX::clearFilterLists() {
    auto wipe = [](QWidget* c){
        if (!c) return;

        // Lấy (hoặc tạo) QVBoxLayout đúng 1 lần
        QVBoxLayout* lay = qobject_cast<QVBoxLayout*>(c->layout());
        if (!lay) {
            lay = new QVBoxLayout(c);
            lay->setContentsMargins(8,8,8,8);
            lay->setSpacing(8);
        }

        // Dọn các widget con
        while (lay->count() > 0) {
            QLayoutItem* it = lay->takeAt(0);
            if (QWidget* w = it->widget()) w->deleteLater();
            delete it;
        }
    };

    wipe(ui->scrollInContainer_2);
    wipe(ui->scrollOutContainer_2);
}

void LSNX::renderFiltered(const std::vector<std::shared_ptr<Trade>>& trades) {
    const bool showIn  = ui->cbIn->isChecked();
    const bool showOut = ui->cbOut->isChecked();

    clearFilterLists();

    auto add = [&](QWidget* container, TradeCardWidget* card){
        QVBoxLayout* lay = qobject_cast<QVBoxLayout*>(container->layout());
        if (!lay) {
            lay = new QVBoxLayout(container);
            lay->setContentsMargins(8,8,8,8);
            lay->setSpacing(8);
        }
        lay->addWidget(card);

        // (tùy) tắt chọn-tick ở trang filter
        if (card->metaObject()->indexOfMethod("setSelectable(bool)") != -1)
            QMetaObject::invokeMethod(card, "setSelectable", Q_ARG(bool, false));
    };

    for (const auto& sp : trades) {
        if (!sp) continue;

        const QString id   = sp->getID();
        const QString pid  = sp->getProductID();
        const int     qty  = sp->getQuantity();
        const QString time = fmtTime(sp->getTradeTime());

        if (auto inT = std::dynamic_pointer_cast<TradeIn>(sp)) {
            if (!showIn) continue;
            auto *card = new TradeCardWidget;
            card->setTradeId(id);
            card->setInCard(time, pid, inT->getLotID(), qty,
                            static_cast<qlonglong>(inT->getCapital()));
            add(ui->scrollInContainer_2, card);

        } else if (auto outT = std::dynamic_pointer_cast<TradeOut>(sp)) {
            if (!showOut) continue;
            auto *card = new TradeCardWidget;
            card->setTradeId(id);
            card->setOutCard(time, pid, outT->getClient().name, qty,
                             static_cast<qlonglong>(outT->getRevenue()));
            add(ui->scrollOutContainer_2, card);
        }
    }

    ui->scrollIn_2->setWidgetResizable(true);
    ui->scrollOut_2->setWidgetResizable(true);
}

void LSNX::on_back_to_normal() {
    if (ui->stackedWidget && ui->normal)
        ui->stackedWidget->setCurrentWidget(ui->normal);
}

void LSNX::on_btnHomeNormal_clicked() { AppRouter::getInstance().goMenu(); }
void LSNX::on_btnHomeAddTradeIn_clicked(){AppRouter::getInstance().goMenu();}
void LSNX::on_btnHomeAddTradeOut_clicked(){AppRouter::getInstance().goMenu();}
void LSNX::on_btnHomeDeleteTrade_clicked(){AppRouter::getInstance().goMenu();}
void LSNX::on_btnHomeFilterTrade_clicked(){AppRouter::getInstance().goMenu();}
void LSNX::on_btnSPNormal_clicked() { AppRouter::getInstance().goSP(); }
void LSNX::on_btnSPAddTradeIn_clicked() { AppRouter::getInstance().goSP(); }
void LSNX::on_btnSPAddTradeOut_clicked() { AppRouter::getInstance().goSP(); }
void LSNX::on_btnSPDeleteTrade_clicked() { AppRouter::getInstance().goSP(); }
void LSNX::on_btnSPFilterTrade_clicked() { AppRouter::getInstance().goSP(); }

