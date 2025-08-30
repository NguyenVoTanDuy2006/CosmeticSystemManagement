#pragma once
#include <QLayout>
#include <QRect>
#include <QStyle>

class FlowLayout : public QLayout {
public:
    explicit FlowLayout(QWidget *parent = nullptr, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~FlowLayout();

    void addItem(QLayoutItem *item) override;
    int  horizontalSpacing() const;
    int  verticalSpacing() const;
    Qt::Orientations expandingDirections() const override { return {}; }
    bool hasHeightForWidth() const override { return true; }
    int  heightForWidth(int) const override;
    int  count() const override { return itemList.size(); }
    QLayoutItem *itemAt(int index) const override { return itemList.value(index); }
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace = -1;
    int m_vSpace = -1;
};
