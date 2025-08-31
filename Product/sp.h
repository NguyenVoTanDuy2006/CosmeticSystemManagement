#pragma once
#include <QMainWindow>
#include <QVector>
#include <QButtonGroup>
#include <memory>
#include <vector>

// ===== forward declarations cho các class chỉ dùng con trỏ/tham chiếu =====
class QCheckBox;
class QLineEdit;
class QPushButton;
class QButtonGroup;
class FlowLayout;          // nếu không include flowlayout.h trong sp.h
class baseProductFilter;   // từ backend filter
class Product;             // class Product trong backend
// THÊM include để dùng productManager làm member by-value
#include "CosmeticManagement/productManager.h"

// forward
class FlowLayout;

QT_BEGIN_NAMESPACE
namespace Ui { class SP; }
QT_END_NAMESPACE

struct demo_item {
    QString name;
    QString brand;
    QString code;
    QString hsd;
    int     stock = 0;
};

class QWidget;

class sp : public QMainWindow {
    Q_OBJECT
public:
    explicit sp(QWidget *parent = nullptr);
    ~sp();
    void load_from_backend();    // giờ sẽ dùng member pm

protected:
    void resizeEvent(QResizeEvent* e) override;

private slots:
    void on_back_to_normal();  // quay về trang normal
    void on_search_changed(const QString& text);
    void on_delete_search_clicked();
    void on_show_add_page();     // bấm +Thêm
    void on_add_save_clicked();  // bấm THÊM
    void on_show_delete_page();                     // normal -> uiDeleteSP
    void on_delete_search_changed(const QString&);  // filter trong trang xóa
    void on_delete_exec_clicked();                  // bấm XÓA
    //void on_filter_controls_changed();   // tick/un-tick / brand change
    //void on_filter_apply_clicked();      // bấm "Xem N kết quả"

    void on_show_edit_page();          // normal -> uiEditSP
    void on_edit_id_committed();       // người dùng gõ ID xong (Enter / nút Load)
    void on_btnPickPicEditSP_clicked();
    void on_btnSaveEditSP_clicked();

    void on_btnHomeNormal_clicked();
    void on_btnHomeAddSP_clicked();
    void on_btnHomeDeleteSP_clicked();
    void on_btnHomeEditSP_clicked();
    void on_btnHomeFilterSP_clicked();
    void on_btnLSNXNormal_clicked();
    void on_btnLSNXAddSP_clicked();
    void on_btnLSNXDeleteSP_clicked();
    void on_btnLSNXEditSP_clicked();
    void on_btnLSNXFilterSP_clicked();

private:
    Ui::SP* ui = nullptr;

    FlowLayout* flow = nullptr;   // layout cho scrollSPContainer

    // NEW: quản lý backend 1 nơi
    productManager& pm;

    QVector<demo_item> all_items;
    QVector<QWidget*>  cards;

    // sp.h (bên trong class sp)
    int card_min_w = 280;
    int card_min_h = 320;
    int cur_cols   = 1;
    int max_cols   = 4;

    void apply_filter(const QString& key);
    void clear_grid();
    void rebuild_grid();
    void relayout_by_width();
    QWidget* make_card(const demo_item& it) const;

    // NEW: tiện ích cho trang Add
    QString gen_next_product_id_from_pm() const;  // lấy ID kế tiếp dựa trên pm hiện có
    void    reset_add_form_except_code();
    void    refresh_ui_from_pm();                 // đổ pm -> all_items -> grid
    bool    id_exists_in_pm(const QString& id) const;

    // state trang xóa
    FlowLayout* del_flow = nullptr;   // <— NEW
    QVector<QWidget*>  del_cards;
    QVector<QString>   del_ids;     // <<== giữ đúng "vector id" theo yêu cầu
    int                del_cur_cols = 1;
    // helpers
    QVector<demo_item> delete_filtered_items(const QString& key) const;
    void del_clear_grid();
    void del_build_grid(const QVector<demo_item>& items);
    void del_rebuild_grid();
    void del_relayout_by_width();
    QWidget* make_del_card(const demo_item& it);

    //filter
    QButtonGroup *filterHsdGroup  = nullptr;   // HSD: độc quyền 1 tick
    QButtonGroup *filterTypeGroup = nullptr;   // Công dụng: độc quyền 1 tick
    QVector<QCheckBox*> typeChecks;            // 9 checkbox công dụng
    QLineEdit    *brandEdit       = nullptr;   // ô nhập brand
    QPushButton  *btnApply        = nullptr;   // "Xem N kết quả"
    FlowLayout   *filterFlow      = nullptr;   // FlowLayout kết quả (scrollSP_3)
    void setupFilterUI();   // fill UI + wire signal
    std::vector<std::shared_ptr<baseProductFilter>> buildFiltersFromUI() const;
    void updateFilterPreviewCount();  // set text "Xem N kết quả"
    void showFilterResults(const std::vector<Product>& pros);

    //edit
    // --- EDIT state ---
    bool    editMode = false;
    QString editId;
    QString editPicPath;   // ảnh user vừa chọn (nếu có)
    void loadProductToForm(const QString& id);  // fill form từ backend
    bool saveFormToBackend();                   // đọc form -> setter -> saveData
};
