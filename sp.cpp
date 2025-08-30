#include "sp.h"
#include "ui_SP.h"
#include "spcardwidget.h"
#include "app_router.h"
#include "flowlayout.h"

#include <memory>

#include <QGridLayout>
#include <QScrollArea>
#include <QResizeEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QMargins>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPixmap>
#include <QFileInfo>
#include <QMessageBox>
#include <QImageReader>
#include <QImage>
#include <QStackedLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QButtonGroup>

// backend
#include "CosmeticManagement/productManager.h"


static constexpr int kCardMinW = 280;
static constexpr int kCardMinH = 360;
static constexpr int kMaxCols  = 4;

//==================== helpers ====================//

static demo_item to_demo(const Product& p) {
    demo_item d;
    d.name  = p.getInfo().name;
    d.brand = p.getInfo().brand;
    d.code  = p.getID();
    d.stock = p.getStock();
    d.hsd   = p.getHSD();
    return d;
}

static QString picsDir() {
    const QString dir = QCoreApplication::applicationDirPath() + "/Data/ProductData/Pics";
    QDir().mkpath(dir);
    return dir;
}

static QString copyImageToDataAsPng(const QString& src, const QString& id) {
    const QString dst = picsDir() + "/" + id + ".png";
    QFile::remove(dst);

    QImageReader reader(src);
    reader.setAutoTransform(true);
    QImage img = reader.read();
    if (!img.isNull()) {
        img.save(dst, "PNG");
        return dst;
    }
    const QString ext = QFileInfo(src).suffix().toLower();
    const QString dstRaw = picsDir() + "/" + id + "." + ext;
    QFile::remove(dstRaw);
    QFile::copy(src, dstRaw);
    return dstRaw;
}

QVector<demo_item> sp::delete_filtered_items(const QString& key_raw) const {
    const QString key = key_raw.trimmed();
    if (key.isEmpty()) return all_items;
    QVector<demo_item> out; out.reserve(all_items.size());
    for (const auto& it : all_items)
        if (it.name.contains(key, Qt::CaseInsensitive) ||
            it.code.contains(key, Qt::CaseInsensitive))
            out.push_back(it);
    return out;
}

void sp::del_clear_grid(){
    if (!del_flow) return;
    while (QLayoutItem* it = del_flow->takeAt(0)) {
        if (auto *w = it->widget()) w->deleteLater();
        delete it;
    }
    del_cards.clear();
    del_ids.clear();
    if (ui->btnExecDelSP) ui->btnExecDelSP->setText(tr("XÓA (0)"));
}

void sp::del_build_grid(const QVector<demo_item>& items){
    del_clear_grid();
    del_cards.reserve(items.size());
    for (const auto& it : items) del_cards.push_back(make_del_card(it));
    del_rebuild_grid();
    del_relayout_by_width();
}

void sp::del_rebuild_grid(){
    if (!del_flow) return;
    // nạp lại toàn bộ card
    for (auto* w : del_cards) {
        if (!w) continue;
        del_flow->addWidget(w);
        w->show();
    }
    del_flow->invalidate();   // tính lại & để ScrollArea cập nhật
}


void sp::del_relayout_by_width(){
    if (del_flow) del_flow->invalidate();
}

QWidget* sp::make_del_card(const demo_item& it){
    auto *base = new spcardwidget;
    base->set_item(it);
    base->setMinimumSize(280, 360);
    base->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *overlay = new QWidget;
    overlay->setAutoFillBackground(false);
    overlay->setAttribute(Qt::WA_TranslucentBackground, true);

    auto *h = new QHBoxLayout(overlay);
    h->setContentsMargins(0, 6, 6, 0);
    h->setSpacing(0);
    h->addStretch();

    auto *chk = new QCheckBox(overlay);
    chk->setObjectName("chkDel");
    chk->setFocusPolicy(Qt::NoFocus);
    chk->setText(QString());
    h->addWidget(chk, 0, Qt::AlignRight | Qt::AlignTop);

    QWidget *wrap = new QWidget;
    auto *stack = new QStackedLayout(wrap);
    stack->setStackingMode(QStackedLayout::StackAll);
    stack->setContentsMargins(0,0,0,0);
    stack->addWidget(base);
    stack->addWidget(overlay);

    overlay->raise();
    base->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    wrap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    wrap->setMinimumSize(280, 360);
    wrap->setProperty("code", it.code);

    const bool preChecked = del_ids.contains(it.code);
    chk->blockSignals(true);
    chk->setChecked(preChecked);
    chk->blockSignals(false);

    connect(chk, &QCheckBox::toggled, this, [this, wrap](bool on){
        const QString id = wrap->property("code").toString();
        if (on) { if (!del_ids.contains(id)) del_ids.push_back(id); }
        else     del_ids.removeAll(id);

        if (ui->btnExecDelSP)
            ui->btnExecDelSP->setText(tr("XÓA (%1)").arg(del_ids.size()));
    });

    wrap->setStyleSheet(
        "QCheckBox#chkDel::indicator{ width:22px;height:22px; }"
        "QCheckBox#chkDel::indicator:unchecked{ "
        "  border:2px solid #29a3a3; border-radius:11px; background:transparent; }"
        "QCheckBox#chkDel::indicator:checked{  "
        "  border:2px solid #29a3a3; border-radius:11px; background:#29a3a3; }"
        );
    return wrap;
}


//==================== class sp ====================//

sp::sp(QWidget *parent) : QMainWindow(parent), ui(new Ui::SP),  pm(productManager::getInstance()) {
    ui->setupUi(this);
    this->setWindowTitle("The Cosmetic Warehouse Management Application");

    // trong ctor sp::sp(...)
    if (auto *btn = ui->uiAddSP->findChild<QPushButton*>("btnBackAddSP"))
        connect(btn, &QPushButton::clicked, this, &sp::on_back_to_normal);

    if (auto *btn = ui->uiEditSP->findChild<QPushButton*>("btnBackEditSP"))
        connect(btn, &QPushButton::clicked, this, &sp::on_back_to_normal);

    if (auto *btn = ui->uiDeleteSP->findChild<QPushButton*>("btnBackDeleteSP"))
        connect(btn, &QPushButton::clicked, this, &sp::on_back_to_normal);

    if (auto *btn = ui->uiFilterSP->findChild<QPushButton*>("btnBackFilterSP"))
        connect(btn, &QPushButton::clicked, this, &sp::on_back_to_normal);


    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP")) {
        // Khi rời ô (editingFinished) cũng auto load
        connect(ed, &QLineEdit::editingFinished, this, &sp::on_edit_id_committed);
        // Và giữ cái cũ: Enter để load
        connect(ed, &QLineEdit::returnPressed,    this, &sp::on_edit_id_committed);
    }

    // ScrollArea
    ui->scrollSP->setWidgetResizable(true);
    ui->scrollSP->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Lấy container bên trong ScrollArea
    QWidget* container = ui->scrollSP->widget();   // scrollSPContainer từ .ui
    container->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    // Gắn FlowLayout vào container (không dùng gridSP nữa)
    flow = new FlowLayout(container, /*margin*/24, /*hSpacing*/24, /*vSpacing*/24);
    container->setLayout(flow);

    // Scroll + grid cấu hình chắc chắn
    if (ui->scrollSP) {
        ui->scrollSP->setWidgetResizable(true);
        ui->scrollSP->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        if (auto *cont = ui->scrollSP->widget())
            cont->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    // === KẾT NỐI SIGNAL -> SLOT ===
    // tìm kiếm ở trang normal
    // Nút kính lúp -> tìm đúng tên qua backend và hiển thị 1 card
    if (auto *btn = ui->normal->findChild<QPushButton*>("btnSearchSP")) {
        connect(btn, &QPushButton::clicked, this, [this]{
            const QString name = ui->searchEditSP ? ui->searchEditSP->text().trimmed() : QString();
            if (name.isEmpty()) { apply_filter(QString()); return; }

            Product &p = pm.findProductByName(name);
            const bool notFound =
                p.getID().isEmpty() ||
                p.getInfo().name == "null" ||
                p.getInfo().name != name;

            clear_grid();
            if (notFound) {
                if (flow) {
                    auto *msg = new QLabel(tr("Không tìm thấy sản phẩm tên \"%1\"").arg(name));
                    msg->setStyleSheet("color:#666; padding:8px;");
                    flow->addWidget(msg);
                    flow->invalidate();
                }
                return;
            }
            cards = { make_card(to_demo(p)) };
            rebuild_grid();
            relayout_by_width();
        });
    }

    // Nhấn Enter trong ô nhập -> reuse logic của nút
    if (ui->searchEditSP) {
        connect(ui->searchEditSP, &QLineEdit::returnPressed, this, [this]{
            if (auto *btn = ui->normal->findChild<QPushButton*>("btnSearchSP"))
                emit btn->clicked();
        });
    }

    // if (ui->searchEditSP)
    //     connect(ui->searchEditSP, &QLineEdit::textChanged,
    //             this, &sp::on_search_changed);

    // nút +Thêm (trang normal -> trang add)
    if (ui->themSP)
        connect(ui->themSP, &QPushButton::clicked,
                this, &sp::on_show_add_page);

    // nút –Xóa (trang normal -> trang delete)
    if (auto *btnXoa = ui->normal->findChild<QPushButton*>("xoaSP"))
        connect(btnXoa, &QPushButton::clicked,
                this, &sp::on_show_delete_page);

    // nút THÊM trên trang add
    if (auto *btnSave = ui->uiAddSP->findChild<QPushButton*>("btnSaveAddSP"))
        connect(btnSave, &QPushButton::clicked,
                this, &sp::on_add_save_clicked);
    // --- nút CHỌN ẢNH trên trang add ---
    auto hookPickBtn = [this](const char* name){
        if (auto *btnPick = ui->uiAddSP->findChild<QPushButton*>(name)) {
            connect(btnPick, &QPushButton::clicked, this, [this]{
                // lấy ID hiện thời để đặt tên file ảnh
                auto *edCode = ui->uiAddSP->findChild<QLineEdit*>("codeAddSP");
                const QString id = edCode ? edCode->text().trimmed() : QString();
                if (id.isEmpty()) {
                    QMessageBox::warning(this, tr("Thiếu mã SP"),
                                         tr("Chưa có mã sản phẩm để lưu ảnh."));
                    return;
                }

                // chọn file ảnh
                const QString f = QFileDialog::getOpenFileName(
                    this, tr("Chọn ảnh sản phẩm"),
                    QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                    tr("Ảnh (*.png *.jpg *.jpeg *.bmp *.webp)")
                    );
                if (f.isEmpty()) return;

                // copy/convert về thư mục dữ liệu, tên theo <id>.png nếu đọc được
                const QString savedPath = copyImageToDataAsPng(f, id);

                // lưu path tạm vào property để nếu cần dùng về sau
                ui->uiAddSP->setProperty("copied_pic_path", savedPath);

                // set preview
                if (auto *lbl = ui->uiAddSP->findChild<QLabel*>("picPreviewAddSP")) {
                    QPixmap pm(savedPath);
                    if (pm.isNull()) {
                        QImageReader r(savedPath);
                        r.setAutoTransform(true);
                        QImage im = r.read();
                        if (!im.isNull()) pm = QPixmap::fromImage(im);
                    }
                    if (!pm.isNull()) {
                        // scale theo kích thước hiện tại của label
                        QPixmap scaled = pm.scaled(lbl->size(),
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation);
                        lbl->setAlignment(Qt::AlignCenter);
                        lbl->setPixmap(scaled);
                        lbl->setText(QString());
                        lbl->setToolTip(savedPath);
                    } else {
                        lbl->setPixmap(QPixmap());
                        lbl->setText(tr("Không đọc được ảnh"));
                    }
                }
            });
        }
    };

    // tùy dự án, nút có thể đặt tên khác nhau – thử lần lượt:
    hookPickBtn("btnPickPicAddSP");
    hookPickBtn("btnUploadPicAddSP");
    hookPickBtn("btnAddPicSP");

    // ô tìm + nút kính lúp ở trang delete
    if (auto *ed = ui->uiDeleteSP->findChild<QLineEdit*>("searchEditSP_2"))
        connect(ed, &QLineEdit::returnPressed,
                this, &sp::on_delete_search_clicked);
    if (auto *btn = ui->uiDeleteSP->findChild<QPushButton*>("btnSearchSP_2"))
        connect(btn, &QPushButton::clicked,
                this, &sp::on_delete_search_clicked);

    // nút XÓA ở trang delete
    if (ui->btnExecDelSP)
        connect(ui->btnExecDelSP, &QPushButton::clicked,
                this, &sp::on_delete_exec_clicked);

    // === DELETE PAGE (FlowLayout giống trang normal) ===
    if (ui->uiDeleteSP) {
        auto* delScroll = ui->uiDeleteSP->findChild<QScrollArea*>("scrollSP_2");
        if (delScroll) {
            delScroll->setWidgetResizable(true);
            delScroll->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            if (auto* cont = delScroll->widget()) {   // scrollSP_2 container từ .ui
                cont->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
                del_flow = new FlowLayout(cont, /*margin*/24, /*hSpacing*/24, /*vSpacing*/24);
                cont->setLayout(del_flow);
            }
        }
    }
    // nút "Bộ lọc" (trang normal -> trang uiFilterSP)
    if (auto *btnFilter = ui->normal->findChild<QPushButton*>("filter")) {
        connect(btnFilter, &QPushButton::clicked, this, [this]{
            if (ui->stackedWidget && ui->uiFilterSP)
                ui->stackedWidget->setCurrentWidget(ui->uiFilterSP);
            // cập nhật trước khi hiển thị
            QTimer::singleShot(0, this, &sp::updateFilterPreviewCount);
        });
    }
    // === NÚT "Sửa" ở trang normal ===
    if (auto *btn = ui->normal->findChild<QPushButton*>("editSP")) {
        connect(btn, &QPushButton::clicked, this, &sp::on_show_edit_page);
    }
    // === TRANG uiEditSP: điền ID rồi load thông tin ===
    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP")) {
        connect(ed, &QLineEdit::returnPressed, this, &sp::on_edit_id_committed);
    }
    // nếu bạn có thêm 1 nút riêng để load theo ID (tùy .ui), thì hook luôn:
    if (auto *btnLoad = ui->uiEditSP->findChild<QPushButton*>("btnLoadFromIdEditSP")) {
        connect(btnLoad, &QPushButton::clicked, this, &sp::on_edit_id_committed);
    }
    // === TRANG uiEditSP: chọn ảnh & lưu (nút “THÊM” nhưng thực chất là CẬP NHẬT) ===
    if (auto *btnPick = ui->uiEditSP->findChild<QPushButton*>("btnPickPicEditSP"))
        connect(btnPick, &QPushButton::clicked, this, &sp::on_btnPickPicEditSP_clicked);

    if (auto *btnSave = ui->uiEditSP->findChild<QPushButton*>("btnSaveEditSP"))
        connect(btnSave, &QPushButton::clicked, this, &sp::on_btnSaveEditSP_clicked);


    // === FILTER PAGE ===
    setupFilterUI();

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    //pm.readData();
    refresh_ui_from_pm();


    // Sau khi UI có size thật, tính cột và dựng lưới
    QTimer::singleShot(0, this, [this]{ relayout_by_width(); });
}


sp::~sp() { delete ui; }

//==================== backend → UI ====================//

void sp::refresh_ui_from_pm() {
    const auto& list = pm.getProducts();

    QVector<demo_item> items;
    items.reserve(static_cast<int>(list.size()));
    for (const auto& p : list) {
        if (p.getID().isEmpty() || p.getInfo().name == "null") continue;
        items.push_back(to_demo(p));
    }
    all_items = std::move(items);
    apply_filter(ui->searchEditSP ? ui->searchEditSP->text() : QString());
}

bool sp::id_exists_in_pm(const QString& id) const {
    const auto& list = pm.getProducts();
    for (const auto& p : list) if (p.getID() == id) return true;
    return false;
}

QString sp::gen_next_product_id_from_pm() const {
    int maxNum = 0, pad = 5;
    QRegularExpression re("(\\D*)(\\d+)");
    for (const auto& p : pm.getProducts()) {
        auto m = re.match(p.getID());
        if (!m.hasMatch()) continue;
        const QString digits = m.captured(2);
        pad = qMax(pad, digits.size());
        bool ok=false; int n = digits.toInt(&ok);
        if (ok) maxNum = qMax(maxNum, n);
    }
    const int next = maxNum + 1;
    return QString("SP%1").arg(next, pad, 10, QChar('0'));
}

//==================== UI logic ====================//

void sp::load_from_backend() {
    pm.readData();
    refresh_ui_from_pm();
}

void sp::on_search_changed(const QString& text) {
    apply_filter(text);
}

void sp::apply_filter(const QString& key_raw) {
    clear_grid();

    const QString key = key_raw.trimmed();
    QVector<demo_item> filtered;
    filtered.reserve(all_items.size());

    if (key.isEmpty()) {
        filtered = all_items;
    } else {
        for (const auto& it : all_items) {
            if (it.name.contains(key, Qt::CaseInsensitive) ||
                it.code.contains(key, Qt::CaseInsensitive)) {
                filtered.push_back(it);
            }
        }
    }

    cards.reserve(filtered.size());
    for (const auto& it : filtered) {
        QWidget* w = make_card(it);
        cards.push_back(w);
    }
    rebuild_grid();
    relayout_by_width();
}

void sp::clear_grid() {
    if (!flow) return;
    // bỏ hết item trong layout + delete widget
    while (QLayoutItem* it = flow->takeAt(0)) {
        if (auto* w = it->widget()) w->deleteLater();
        delete it;
    }
    cards.clear();
}

void sp::rebuild_grid() {
    if (!flow) return;
    // add lại toàn bộ card
    for (QWidget* w : cards) {
        if (!w) continue;
        flow->addWidget(w);
        w->show();
    }
    flow->invalidate(); // ScrollArea tự cập nhật scroll
}

// sp.cpp
void sp::relayout_by_width() {
    if (!flow) return;        // flow là FlowLayout* của trang normal
    flow->invalidate();       // bảo layout tính lại khi cửa sổ đổi kích thước
}

void sp::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
    relayout_by_width();     // normal page
    del_relayout_by_width(); // delete page
}


QWidget* sp::make_card(const demo_item& it) const {
    auto* card = new spcardwidget;
    card->set_item(it);
    card->setMinimumSize(280, 360);                // giống kCardMinW/H
    card->setSizePolicy(QSizePolicy::Preferred,    // chiều ngang auto
                        QSizePolicy::Fixed);        // cao cố định
    return card;
}



//==================== Trang thêm SP ====================//

void sp::reset_add_form_except_code() {
    if (!ui || !ui->uiAddSP) return;

    if (auto* ed = ui->uiAddSP->findChild<QLineEdit*>("nameAddSP"))   ed->clear();
    if (auto* ed = ui->uiAddSP->findChild<QLineEdit*>("brandAddSP"))  ed->clear();
    if (auto* ed = ui->uiAddSP->findChild<QLineEdit*>("priceAddSP"))  ed->clear();

    if (auto* cb = ui->uiAddSP->findChild<QComboBox*>("typeAddSP"))   cb->setCurrentIndex(0);
    if (auto* cb = ui->uiAddSP->findChild<QComboBox*>("unitAddSP"))   cb->setCurrentIndex(0);

    ui->uiAddSP->setProperty("copied_pic_path", QVariant());
    if (auto* lbl = ui->uiAddSP->findChild<QLabel*>("picPreviewAddSP")) lbl->clear();
}

void sp::on_show_add_page() {
    const QString id = gen_next_product_id_from_pm();
    if (auto* edCode = ui->uiAddSP->findChild<QLineEdit*>("codeAddSP"))
        edCode->setText(id);

    reset_add_form_except_code();

    if (auto *cbT = ui->uiAddSP->findChild<QComboBox*>("typeAddSP")) {
        if (cbT->count() == 0) {
            cbT->addItems({ tr("Sữa rửa mặt"), tr("Kem"), tr("Serum"),
                           tr("Son"), tr("Nước"), tr("Mặt nạ"),
                           tr("Nước hoa"), tr("Gel"), tr("Phấn") });
        }
    }
    if (auto *cbU = ui->uiAddSP->findChild<QComboBox*>("unitAddSP")) {
        if (cbU->count() == 0) {
            cbU->addItems({ tr("Chai"), tr("Tuýp"), tr("Hộp") });
        }
    }

    QFile::remove(picsDir() + "/" + id + ".png");
    QFile::remove(picsDir() + "/" + id + ".jpg");
    QFile::remove(picsDir() + "/" + id + ".jpeg");

    if (ui->stackedWidget && ui->uiAddSP)
        ui->stackedWidget->setCurrentWidget(ui->uiAddSP);
}

void sp::on_add_save_clicked() {
    auto *edCode  = ui->uiAddSP->findChild<QLineEdit*>("codeAddSP");
    auto *edName  = ui->uiAddSP->findChild<QLineEdit*>("nameAddSP");
    auto *edBrand = ui->uiAddSP->findChild<QLineEdit*>("brandAddSP");
    auto *cbType  = ui->uiAddSP->findChild<QComboBox*>("typeAddSP");
    auto *cbUnit  = ui->uiAddSP->findChild<QComboBox*>("unitAddSP");
    auto *edPrice = ui->uiAddSP->findChild<QLineEdit*>("priceAddSP");

    if (!edName || edName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Thiếu dữ liệu", "Tên sản phẩm không được để trống.");
        return;
    }

    productInfo info;
    info.name  = edName->text().trimmed();
    info.brand = edBrand ? edBrand->text().trimmed() : QString();

    constexpr int TYPE_OFFSET = 0;
    constexpr int UNIT_OFFSET = 0;
    info.type  = cbType ? static_cast<TYPE>(cbType->currentIndex() + TYPE_OFFSET) : static_cast<TYPE>(0);
    info.unit  = cbUnit ? static_cast<UNIT>(cbUnit->currentIndex() + UNIT_OFFSET) : static_cast<UNIT>(0);

    long long price = 0;
    if (edPrice) {
        QString raw = edPrice->text();
        raw.remove(QRegularExpression("[^\\d]"));
        if (!raw.isEmpty()) price = raw.toLongLong();
    }
    info.price = price;

    const QString desiredID = edCode ? edCode->text().trimmed() : gen_next_product_id_from_pm();
    if (id_exists_in_pm(desiredID)) {
        QMessageBox::warning(this, "Trùng mã", "Mã sản phẩm đã tồn tại.");
        return;
    }

    pm.addProduct(info);
    if (!pm.saveData()) {
        QMessageBox::warning(this, "Lỗi lưu", "Không thể lưu dữ liệu sản phẩm.");
        return;
    }

    refresh_ui_from_pm();

    if (ui->stackedWidget && ui->normal)
        ui->stackedWidget->setCurrentWidget(ui->normal);
}

//==================== XÓA SP ====================//

void sp::on_delete_search_clicked() {
    auto *ed = ui->uiDeleteSP->findChild<QLineEdit*>("searchEditSP_2");
    const QString name = ed ? ed->text().trimmed() : QString();

    if (name.isEmpty()) { del_build_grid(all_items); return; }

    Product &p = pm.findProductByName(name);
    const bool notFound =
        p.getID().isEmpty() ||
        p.getInfo().name == "null" ||
        p.getInfo().name != name;

    if (notFound) {
        del_clear_grid();
        if (del_flow) {
            auto *msg = new QLabel(tr("Không tìm thấy sản phẩm tên \"%1\"").arg(name));
            msg->setStyleSheet("color:#666; padding:8px;");
            del_flow->addWidget(msg);
        }
        return;
    }

    QVector<demo_item> one{ to_demo(p) };
    del_build_grid(one);
}

void sp::on_show_delete_page(){
    del_build_grid(all_items);
    QTimer::singleShot(0, this, [this]{ del_relayout_by_width(); });
    if (ui->stackedWidget && ui->uiDeleteSP)
        ui->stackedWidget->setCurrentWidget(ui->uiDeleteSP);
}

void sp::on_delete_search_changed(const QString& text){
    del_build_grid(delete_filtered_items(text));
}

void sp::on_delete_exec_clicked(){
    if (del_ids.isEmpty()){
        QMessageBox::information(this, tr("Chưa chọn"), tr("Hãy chọn ít nhất một sản phẩm."));
        return;
    }
    if (QMessageBox::question(this, tr("Xóa sản phẩm"),
                              tr("Bạn có chắc muốn xóa %1 sản phẩm?").arg(del_ids.size()))
        != QMessageBox::Yes) return;

    std::vector<QString> ids; ids.reserve(del_ids.size());
    for (const auto& id : del_ids) ids.push_back(id);

    pm.removeProduct(ids);
    pm.saveData();

    refresh_ui_from_pm();
    if (ui->stackedWidget && ui->normal)
        ui->stackedWidget->setCurrentWidget(ui->normal);
}

//for filter page
void sp::setupFilterUI()
{
    // ===== Scroll "Công dụng": QScrollArea objectName=scrollType =====
    if (auto *sc = ui->uiFilterSP->findChild<QScrollArea*>("scrollType")) {
        sc->setWidgetResizable(true);
        sc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sc->setFrameShape(QFrame::NoFrame);

        QWidget *content = new QWidget(sc);
        auto *v = new QVBoxLayout(content);
        v->setContentsMargins(8, 8, 8, 8);
        v->setSpacing(6);

        const QStringList typeNames{
            tr("Sữa rửa mặt"), tr("Kem"),  tr("Serum"),
            tr("Son"),         tr("Nước"), tr("Mặt nạ"),
            tr("Nước hoa"),    tr("Gel"),  tr("Phấn")
        };

        if (!filterTypeGroup) filterTypeGroup = new QButtonGroup(this);
        filterTypeGroup->setExclusive(true);
        typeChecks.clear();

        for (int i = 0; i < typeNames.size(); ++i) {
            auto *cb = new QCheckBox(typeNames[i], content);
            cb->setObjectName(QStringLiteral("type_%1").arg(i));
            cb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            v->addWidget(cb);

            typeChecks.push_back(cb);
            filterTypeGroup->addButton(cb, i);    // id = index TYPE
        }
        v->addStretch();
        sc->setWidget(content);
    }

    // ===== HSD group: 3 checkbox đã đặt sẵn trên .ui =====
    if (!filterHsdGroup) filterHsdGroup = new QButtonGroup(this);
    filterHsdGroup->setExclusive(true);
    if (auto *cb = ui->uiFilterSP->findChild<QCheckBox*>("cbFresh"))
        filterHsdGroup->addButton(cb, 0);
    if (auto *cb = ui->uiFilterSP->findChild<QCheckBox*>("cbNear"))
        filterHsdGroup->addButton(cb, 1);
    if (auto *cb = ui->uiFilterSP->findChild<QCheckBox*>("cbExpired"))
        filterHsdGroup->addButton(cb, 2);

    // ===== brand + nút "Xem ... kết quả" =====
    brandEdit = ui->uiFilterSP->findChild<QLineEdit*>("brandEdit");
    btnApply  = ui->uiFilterSP->findChild<QPushButton*>("btnApply");

    // Preview count khi thay đổi tiêu chí
    if (brandEdit)
        connect(brandEdit, &QLineEdit::textChanged,
                this, &sp::updateFilterPreviewCount);

    if (filterHsdGroup)
        connect(filterHsdGroup, &QButtonGroup::idClicked,
                this, [this](int){ updateFilterPreviewCount(); });

    if (filterTypeGroup)
        connect(filterTypeGroup, &QButtonGroup::idClicked,
                this, [this](int){ updateFilterPreviewCount(); });

    // Nút "Xem kết quả" → build filters → backend → render sang scrollSP_3
    if (btnApply)
        connect(btnApply, &QPushButton::clicked, this, [this]{
            auto filters = buildFiltersFromUI();                 // AND logic
            const auto pros = pm.getFilteredProducts(filters);
            showFilterResults(pros);
        });

    // Nút đóng filter về normal (nếu có)
    if (auto *btnClose = ui->uiFilterSP->findChild<QPushButton*>("btnClose"))
        connect(btnClose, &QPushButton::clicked, this, [this]{
            if (ui->stackedWidget && ui->normal)
                ui->stackedWidget->setCurrentWidget(ui->normal);
        });

    // FlowLayout cho vùng kết quả (scrollSP_3) – tạo 1 lần
    if (!filterFlow) {
        if (auto *sc3 = ui->uiFilterSP->findChild<QScrollArea*>("scrollSP_3")) {
            sc3->setWidgetResizable(true);
            QWidget *cont = sc3->widget();
            if (!cont) { cont = new QWidget(sc3); sc3->setWidget(cont); }
            // bỏ mọi layout cũ (nếu có) rồi gắn FlowLayout mới
            if (cont->layout()) { delete cont->layout(); }
            filterFlow = new FlowLayout(cont, 24, 24, 24);
            cont->setLayout(filterFlow);
        }
    }
    QTimer::singleShot(0, this, &sp::updateFilterPreviewCount);
    // Tìm kiếm nhanh trong trang filter (đổ ra panel kết quả bên phải - scrollSP_3)
    if (auto *btn = ui->uiFilterSP->findChild<QPushButton*>("btnSearchSP_3")) {
        connect(btn, &QPushButton::clicked, this, [this]{
            auto *ed = ui->uiFilterSP->findChild<QLineEdit*>("searchEditSP_3");
            const QString name = ed ? ed->text().trimmed() : QString();
            if (!filterFlow) return;

            // clear panel kết quả cũ
            while (QLayoutItem *it = filterFlow->takeAt(0)) {
                if (auto *w = it->widget()) w->deleteLater();
                delete it;
            }

            if (name.isEmpty()) {
                auto *msg = new QLabel(tr("Nhập tên sản phẩm để tìm."));
                msg->setStyleSheet("color:#666; padding:8px;");
                filterFlow->addWidget(msg);
                filterFlow->invalidate();
                return;
            }

            Product &p = pm.findProductByName(name);
            const bool notFound =
                p.getID().isEmpty() ||
                p.getInfo().name == "null" ||
                p.getInfo().name != name;

            if (notFound) {
                auto *msg = new QLabel(tr("Không tìm thấy sản phẩm tên \"%1\"").arg(name));
                msg->setStyleSheet("color:#666; padding:8px;");
                filterFlow->addWidget(msg);
                filterFlow->invalidate();
                return;
            }

            // có hàng -> add đúng 1 card vào panel bên phải
            auto *card = new spcardwidget;
            card->set_item(to_demo(p));
            card->setMinimumSize(280, 360);
            card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            filterFlow->addWidget(card);
            filterFlow->invalidate();
        });
    }
    if (auto *ed = ui->uiFilterSP->findChild<QLineEdit*>("searchEditSP_3")) {
        connect(ed, &QLineEdit::returnPressed, this, [this]{
            if (auto *btn = ui->uiFilterSP->findChild<QPushButton*>("btnSearchSP_3"))
                emit btn->clicked();
        });
    }

}

std::vector<std::shared_ptr<baseProductFilter>> sp::buildFiltersFromUI() const
{
    std::vector<std::shared_ptr<baseProductFilter>> filters;

    // ===== HSD =====
    if (filterHsdGroup) {
        const int hsdId = filterHsdGroup->checkedId(); // -1 nếu chưa chọn
        switch (hsdId) {
        case 0: // Còn hạn
            filters.emplace_back(std::make_shared<ProductUnexpiredFilter>());
            break;
        case 1: // Sắp hết hạn
            filters.emplace_back(std::make_shared<ProductNearExpiryFilter>());
            break;
        case 2: // Đã hết hạn
            filters.emplace_back(std::make_shared<ProductExpiredFilter>());
            break;
        default: break;
        }
    }

    // ===== TYPE (Công dụng) =====
    if (filterTypeGroup) {
        const int typeId = filterTypeGroup->checkedId(); // 0..8
        if (typeId >= 0) {
            // enum TYPE: 1..9  => cộng bù +1
            const TYPE ty = static_cast<TYPE>(typeId + 1);
            filters.emplace_back(std::make_shared<ProductTypeFilter>(ty));
        }
    }

    // ===== BRAND =====
    if (brandEdit) {
        const QString brand = brandEdit->text().trimmed();
        if (!brand.isEmpty()) {
            // ProductBrandFilter đang so sánh == (phân biệt hoa/thường). Nếu muốn ignore-case
            // hãy điều chỉnh ở backend.
            filters.emplace_back(std::make_shared<ProductBrandFilter>(brand));
        }
    }

    return filters;
}


void sp::updateFilterPreviewCount()
{
    if (!btnApply) return;
    auto filters = buildFiltersFromUI();
    const auto pros = pm.getFilteredProducts(filters);  // <-- tên hàm đúng & dùng shared_ptr
    btnApply->setText(tr("Xem %1 kết quả").arg(static_cast<int>(pros.size())));
}

void sp::showFilterResults(const std::vector<Product>& pros)
{
    if (!filterFlow) return;

    // clear widgets cũ
    while (QLayoutItem *it = filterFlow->takeAt(0)) {
        if (auto *w = it->widget()) w->deleteLater();
        delete it;
    }

    // add card mới
    for (const auto &p : pros) {
        if (p.getID().isEmpty() || p.getInfo().name == "null") continue;

        demo_item d;
        d.name  = p.getInfo().name;
        d.brand = p.getInfo().brand;
        d.code  = p.getID();
        d.stock = p.getStock();
        d.hsd   = p.getHSD();

        auto *card = new spcardwidget;
        card->set_item(d);
        card->setMinimumSize(280, 360);
        card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        filterFlow->addWidget(card);
    }

    filterFlow->invalidate();
}

//edit
void sp::on_show_edit_page() {
    editMode = true;
    editId.clear();
    editPicPath.clear();

    // clear form
    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP"))    { ed->clear(); ed->setReadOnly(false); }
    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("nameEditSP"))  ed->clear();
    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("brandEditSP")) ed->clear();
    if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("typeEditSP"))  cb->setCurrentIndex(0);
    if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("unitEditSP"))  cb->setCurrentIndex(0);
    if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("priceEditSP")) ed->clear();
    if (auto *lbl = ui->uiEditSP->findChild<QLabel*>("picPreviewEditSP")) {
        lbl->setPixmap(QPixmap());
        lbl->setText(tr("// ảnh minh hoạ"));
    }

    if (ui->stackedWidget && ui->uiEditSP)
        ui->stackedWidget->setCurrentWidget(ui->uiEditSP);
}

void sp::on_edit_id_committed() {
    auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP");
    const QString id = ed ? ed->text().trimmed() : QString();
    if (id.isEmpty()) {
        QMessageBox::warning(this, tr("Thiếu mã"), tr("Hãy nhập Mã SP để tải thông tin."));
        return;
    }
    editPicPath.clear(); // reset ảnh tạm khi đổi ID
    loadProductToForm(id);
}

static void setPreviewImage(QWidget* uiEditSP, const QString& path){
    auto *lbl = uiEditSP->findChild<QLabel*>("picPreviewEditSP");
    if (!lbl) return;

    QString p = path;
    // nếu path tương đối mà chưa tồn tại, thử ghép với ApplicationDir
    if (!QFileInfo::exists(p) && QFileInfo(p).isRelative())
        p = QCoreApplication::applicationDirPath() + "/" + p;

    QPixmap pm;
    if (QFileInfo::exists(p)) {
        pm.load(p);
        if (pm.isNull()) { // đọc lại qua QImageReader để auto-rotate
            QImageReader r(p); r.setAutoTransform(true);
            const QImage im = r.read();
            if (!im.isNull()) pm = QPixmap::fromImage(im);
        }
    }

    if (!pm.isNull()){
        lbl->setText(QString());
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setPixmap(pm.scaled(lbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lbl->setToolTip(p);
    } else {
        lbl->setPixmap(QPixmap());
        lbl->setText(QObject::tr("// ảnh minh hoạ"));
        lbl->setToolTip(QString());
    }
}
void sp::loadProductToForm(const QString& id) {
    try {
        Product &p = pm.findProductByID(id);
        if (p.getID().isEmpty() || p.getInfo().name == "null")
            throw std::runtime_error("dummy");

        const auto inf = p.getInfo();
        editId = id;
        editPicPath.clear();                 // rất quan trọng: chưa chọn ảnh mới → giữ ảnh cũ

        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP")) { ed->setText(p.getID()); ed->setReadOnly(true); }
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("nameEditSP"))   ed->setText(inf.name);
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("brandEditSP"))  ed->setText(inf.brand);

        if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("typeEditSP")) {
            if (cb->count() == 0) cb->addItems({ tr("Sữa rửa mặt"), tr("Kem"), tr("Serum"),
                              tr("Son"), tr("Nước"), tr("Mặt nạ"),
                              tr("Nước hoa"), tr("Gel"), tr("Phấn") });
            cb->setCurrentIndex(qMax(0, int(inf.type) - 1));
        }
        if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("unitEditSP")) {
            if (cb->count() == 0) cb->addItems({ tr("Chai"), tr("Tuýp"), tr("Hộp") });
            cb->setCurrentIndex(qMax(0, int(inf.unit) - 1));
        }

        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("priceEditSP"))
            ed->setText(QLocale().toString(qlonglong(inf.price)));

        // === ẢNH: lấy ảnh SẴN CÓ của product và hiển thị ===
        const QString picPath = p.getPicturePath();      // backend đã lưu đường dẫn ảnh
        qDebug() << "pic path = " << picPath;
        setPreviewImage(ui->uiEditSP, picPath);

        // (tuỳ chọn) nếu bạn có ô đường dẫn ảnh (QLineEdit)
        if (auto *edPath = ui->uiEditSP->findChild<QLineEdit*>("picPathEditSP")) {
            edPath->setText(picPath);
            edPath->setReadOnly(true);
        }
    } catch (...) {
        QMessageBox::warning(this, tr("Không tìm thấy"), tr("Không tìm thấy sản phẩm mã %1").arg(id));
    }
}

void sp::on_btnPickPicEditSP_clicked() {
    const QString f = QFileDialog::getOpenFileName(
        this, tr("Chọn ảnh sản phẩm"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        tr("Ảnh (*.png *.jpg *.jpeg *.bmp *.webp)")
        );
    if (f.isEmpty()) return;

    editPicPath = f;
    if (auto *lbl = ui->uiEditSP->findChild<QLabel*>("picPreviewEditSP")) {
        QPixmap pm(f);
        if (pm.isNull()) {
            QImageReader r(f); r.setAutoTransform(true);
            QImage im = r.read();
            if (!im.isNull()) pm = QPixmap::fromImage(im);
        }
        if (!pm.isNull())
            lbl->setPixmap(pm.scaled(lbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

bool sp::saveFormToBackend() {
    // Lấy ID từ state nếu có, nếu chưa thì lấy trực tiếp trong ô nhập
    if (editId.isEmpty()) {
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP"))
            editId = ed->text().trimmed();
    }
    if (editId.isEmpty()) {
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("idEditSP")) ed->setFocus();
        QMessageBox::warning(this, tr("Chưa chọn sản phẩm"),
                             tr("Hãy nhập Mã SP và nhấn Enter để tải thông tin trước khi lưu."));
        return false;
    }

    try {
        Product &p = pm.findProductByID(editId);
        if (p.getID().isEmpty() || p.getInfo().name == "null") {
            QMessageBox::warning(this, tr("Không hợp lệ"),
                                 tr("Mã %1 không tồn tại.").arg(editId));
            return false;
        }

        // lấy info cũ làm gốc → tránh vô tình overwrite khi người dùng để trống
        productInfo info = p.getInfo();

        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("nameEditSP")) {
            const QString s = ed->text().trimmed();
            if (s.isEmpty()) {
                QMessageBox::warning(this, tr("Thiếu dữ liệu"), tr("Tên sản phẩm không được để trống."));
                return false;
            }
            info.name = s;
        }
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("brandEditSP"))
            info.brand = ed->text().trimmed();

        if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("typeEditSP"))
            info.type = static_cast<TYPE>(cb->currentIndex() + 1);
        if (auto *cb = ui->uiEditSP->findChild<QComboBox*>("unitEditSP"))
            info.unit = static_cast<UNIT>(cb->currentIndex() + 1);

        // parse price: chỉ đổi nếu có số
        if (auto *ed = ui->uiEditSP->findChild<QLineEdit*>("priceEditSP")) {
            QString raw = ed->text();
            raw.remove(QRegularExpression("[^\\d]"));
            if (!raw.isEmpty())
                info.price = raw.toLongLong();
        }

        // setter + save
        p.setInfo(info);
        if (!editPicPath.isEmpty())
            p.setPicture(editPicPath);

        if (!pm.saveData()) {
            QMessageBox::warning(this, tr("Lỗi lưu"), tr("Không thể lưu dữ liệu."));
            return false;
        }
        return true;
    } catch (...) {
        QMessageBox::critical(this, tr("Lỗi"), tr("Không thể cập nhật sản phẩm."));
        return false;
    }
}

void sp::on_btnSaveEditSP_clicked() {
    if (!saveFormToBackend()) return;

    // về normal + refresh danh sách
    refresh_ui_from_pm();
    editMode = false;
    editId.clear();
    editPicPath.clear();

    if (ui->stackedWidget && ui->normal)
        ui->stackedWidget->setCurrentWidget(ui->normal);

    //QMessageBox::information(this, tr("Thành công"), tr("Đã cập nhật sản phẩm."));
}

//back to normal
void sp::on_back_to_normal() {
    // Dọn state nhẹ nhàng cho các mode
    editMode = false;
    editId.clear();
    editPicPath.clear();
    del_ids.clear();

    // (tuỳ chọn) clear preview filter bên phải
    if (filterFlow) {
        while (QLayoutItem *it = filterFlow->takeAt(0)) {
            if (auto *w = it->widget()) w->deleteLater();
            delete it;
        }
        filterFlow->invalidate();
    }

    if (ui->stackedWidget && ui->normal)
        ui->stackedWidget->setCurrentWidget(ui->normal);
}

void sp::on_btnHomeNormal_clicked() { AppRouter::getInstance().goMenu(); }
void sp::on_btnHomeAddSP_clicked(){AppRouter::getInstance().goMenu();}
void sp::on_btnHomeDeleteSP_clicked(){AppRouter::getInstance().goMenu();}
void sp::on_btnHomeEditSP_clicked(){AppRouter::getInstance().goMenu();}
void sp::on_btnHomeFilterSP_clicked(){AppRouter::getInstance().goMenu();}
void sp::on_btnLSNXNormal_clicked() { AppRouter::getInstance().goLSNX(); }
void sp::on_btnLSNXAddSP_clicked() { AppRouter::getInstance().goLSNX(); }
void sp::on_btnLSNXDeleteSP_clicked() { AppRouter::getInstance().goLSNX(); }
void sp::on_btnLSNXEditSP_clicked() { AppRouter::getInstance().goLSNX(); }
void sp::on_btnLSNXFilterSP_clicked() { AppRouter::getInstance().goLSNX(); }




