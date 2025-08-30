#pragma once
#include <QWidget>
#include <QString>
#include "sp.h"  // dùng demo_item

class QLabel;
class QFrame;

class spcardwidget : public QWidget {
    Q_OBJECT
public:
    explicit spcardwidget(QWidget *parent = nullptr);
    ~spcardwidget() override = default;

    void set_item(const demo_item& it);

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    // data
    demo_item item_;
    QString   imgPath_;

    // ui blocks
    QFrame *imgFrame  = nullptr;   // khối ảnh (gradient + đường kẻ trắng đáy)
    QFrame *infoFrame = nullptr;   // khối thông tin (gradient)
    QLabel *imgSP     = nullptr;   // label hiển thị ảnh/placeholder

    // value labels
    QLabel *nameVal = nullptr;
    QLabel *brandVal = nullptr;   // <-- thêm
    QLabel *codeVal = nullptr;
    QLabel *hsdVal  = nullptr;
    QLabel *stockVal= nullptr;

    // helpers
    void applyImage();
    static QString findProductImage(const QString& id);
};
