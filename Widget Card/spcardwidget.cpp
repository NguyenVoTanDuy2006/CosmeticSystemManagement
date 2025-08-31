#include "spcardwidget.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPixmap>
#include <QImageReader>
#include <QFontMetrics>
#include <QSizePolicy>

static inline QString picsBaseDir() {
    return QCoreApplication::applicationDirPath() + "/Data/ProductData/Pics";
}

spcardwidget::spcardwidget(QWidget *parent)
    : QWidget(parent) {
    // khoảng cách xung quanh card khi đặt vào grid
    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(6,6,6,6);
    outer->setSpacing(0);

    // ======= KHỐI ẢNH (trên) =======
    imgFrame = new QFrame(this);
    imgFrame->setObjectName("imgFrame");
    imgFrame->setStyleSheet(
        // gradient mint -> blue, đường kẻ trắng đáy
        "#imgFrame {"
        "  background: qlineargradient(x1:0,y1:0, x2:0,y2:1,"
        "             stop:0  #E8FBEF, stop:1 #D7E7FF);"
        "  border-bottom: 2px solid #FFFFFF;"
        "}"
        );
    auto *imgLay = new QVBoxLayout(imgFrame);
    imgLay->setContentsMargins(20,18,20,18);
    imgLay->setSpacing(0);

    imgSP = new QLabel(imgFrame);
    // trong ctor sau khi tạo imgSP:
    imgSP->setMinimumHeight(190);
    imgSP->setMaximumHeight(220);                   // <- quan trọng
    imgSP->setSizePolicy(QSizePolicy::Expanding,    // giãn ngang
                         QSizePolicy::Fixed);       // cố định cao
    imgSP->setObjectName("imgSP");
    imgSP->setAlignment(Qt::AlignCenter);
    imgSP->setMinimumHeight(190);                 // cố định chiều cao ảnh → card bằng nhau
    imgSP->setText("// Ảnh sản phẩm");
    imgSP->setStyleSheet("#imgSP { color: rgba(0,0,0,0.40); font-style: italic; }");
    imgLay->addWidget(imgSP);

    // ======= KHỐI THÔNG TIN (dưới) =======
    infoFrame = new QFrame(this);
    infoFrame->setObjectName("infoFrame");
    infoFrame->setStyleSheet(
        // gradient lam nhạt -> lam hơn
        "#infoFrame {"
        "  background: qlineargradient(x1:0,y1:0, x2:0,y2:1,"
        "             stop:0  #DDE8FF, stop:1 #AFC2FF);"
        "}"
        );

    auto *grid = new QGridLayout(infoFrame);
    // thu margin/spacing để nhường chỗ cho cột giá trị
    grid->setContentsMargins(16,12,16,14);
    grid->setVerticalSpacing(8);
    grid->setHorizontalSpacing(8);

    // Tính bề rộng cột nhãn theo nhãn dài nhất ("Tồn kho:")
    QFont boldF(font());
    boldF.setBold(true);
    QFontMetrics lfm(boldF);
    int labelW = lfm.horizontalAdvance(tr("Tồn kho:")) + 6; // buffer 6px
    grid->setColumnMinimumWidth(0, labelW);
    grid->setColumnStretch(0, 0); // nhãn không giãn
    grid->setColumnStretch(1, 1); // giá trị giãn hết phần còn lại

    auto mkBold = [boldF](const QString& t){
        auto *l = new QLabel(t);
        l->setFont(boldF);
        l->setStyleSheet(
            "background: transparent; border: none;"
            "color: #0A2FA0;"   // xanh dương đậm
            );
        l->setAttribute(Qt::WA_TranslucentBackground, true);
        return l;
    };
    auto mkVal = [](){
        auto *l = new QLabel();
        l->setWordWrap(true);
        l->setStyleSheet(
            "background: transparent; border: none;"
            "color: #102EAD;"   // xanh dương cho value
            );
        l->setAttribute(Qt::WA_TranslucentBackground, true);
        l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        l->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        return l;
    };

    // Tên SP (khóa 2 dòng để card đồng đều)
    grid->addWidget(mkBold("Tên SP:"), 0, 0);
    nameVal = mkVal();
    // ... ngay sau khi tạo nameVal
    {
        constexpr int NAME_MAX_LINES = 3;   // ← đổi 3 hoặc 4 tùy bạn
        QFontMetrics fm(nameVal->font());
        const int h = fm.lineSpacing() * NAME_MAX_LINES + 6; // +6 cho thoáng
        nameVal->setMinimumHeight(h);
        nameVal->setMaximumHeight(h);       // giữ card cao đều; bỏ dòng này nếu muốn auto cao
    }
    grid->addWidget(nameVal, 0, 1);

    grid->addWidget(mkBold("Thương hiệu:"), 1, 0);
    brandVal = mkVal();
    grid->addWidget(brandVal, 1, 1);

    grid->addWidget(mkBold("Mã SP:"), 2, 0);
    codeVal = mkVal();  grid->addWidget(codeVal, 2, 1);

    grid->addWidget(mkBold("HSD:"),   3, 0);
    hsdVal  = mkVal();  grid->addWidget(hsdVal, 3, 1);

    grid->addWidget(mkBold("Tồn kho:"), 4, 0);
    stockVal = mkVal(); grid->addWidget(stockVal, 4, 1);

    // ghép 2 khối
    outer->addWidget(imgFrame);
    outer->addWidget(infoFrame);

}

QString spcardwidget::findProductImage(const QString& id) {
    const QString base = picsBaseDir() + "/" + id;
    const QStringList tries = { base + ".jpg", base + ".jpeg", base + ".png" };
    for (const QString& p : tries) if (QFileInfo::exists(p)) return p;
    return {};
}

void spcardwidget::set_item(const demo_item& it) {
    item_ = it;

    if (nameVal)  {nameVal->setText(it.name);  nameVal->setToolTip(it.name);   }
    if (brandVal)   brandVal->setText(it.brand);       // <-- thêm
    if (codeVal)  codeVal->setText(it.code);
    if (hsdVal)   hsdVal->setText(it.hsd.isEmpty()? tr("(No date)") : it.hsd);
    if (stockVal) stockVal->setText(QString::number(it.stock));

    imgPath_ = findProductImage(it.code);
    applyImage();
}

void spcardwidget::applyImage() {
    if (!imgSP) return;
    if (imgPath_.isEmpty()) { imgSP->setPixmap(QPixmap()); imgSP->setText("// Ảnh sản phẩm"); return; }

    QPixmap pm(imgPath_);
    if (pm.isNull()) {
        QImageReader r(imgPath_); r.setAutoTransform(true);
        QImage im = r.read();
        if (!im.isNull()) pm = QPixmap::fromImage(im);
    }
    if (pm.isNull()) { imgSP->setPixmap(QPixmap()); imgSP->setText("Image error"); return; }

    QSize box = imgSP->size();
    // clamp để không bao giờ scale vượt quá 512px mỗi chiều
    box.setWidth (qBound(80,  box.width(),  512));
    box.setHeight(qBound(80,  box.height(), 512));

    QPixmap scaled = pm.scaled(box, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imgSP->setText(QString());
    imgSP->setPixmap(scaled);
    imgSP->setToolTip(imgPath_);
}


void spcardwidget::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    applyImage();
}
