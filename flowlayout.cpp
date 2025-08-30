#include "flowlayout.h"
#include <QWidgetItem>
#include <QWidget>
#include <algorithm>

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}
FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}
FlowLayout::~FlowLayout() {
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}
void FlowLayout::addItem(QLayoutItem *item) { itemList.append(item); }

int FlowLayout::horizontalSpacing() const {
    if (m_hSpace >= 0) return m_hSpace;
    else return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}
int FlowLayout::verticalSpacing() const {
    if (m_vSpace >= 0) return m_vSpace;
    else return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::heightForWidth(int width) const {
    return doLayout(QRect(0, 0, width, 0), true);
}
QSize FlowLayout::sizeHint() const { return minimumSize(); }

QSize FlowLayout::minimumSize() const {
    QSize size;
    for (auto *item : itemList)
        size = size.expandedTo(item->minimumSize());
    const QMargins m = contentsMargins();
    size += QSize(m.left()+m.right(), m.top()+m.bottom());
    return size;
}

QLayoutItem *FlowLayout::takeAt(int index) {
    return (index >= 0 && index < itemList.size()) ? itemList.takeAt(index) : nullptr;
}
void FlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

// core wrap
int FlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effective = rect.adjusted(+left, +top, -right, -bottom);

    int x = effective.x();
    int y = effective.y();
    int lineHeight = 0;

    const int spaceX = horizontalSpacing();
    const int spaceY = verticalSpacing();

    for (auto *item : itemList) {
        QWidget *wid = item->widget();
        if (wid && !wid->isVisible()) continue;

        const QSize hint = item->sizeHint();
        int nextX = x + hint.width() + spaceX;
        if (nextX - spaceX > effective.right() && lineHeight > 0) {
            // wrap to next line
            x = effective.x();
            y += lineHeight + spaceY;
            nextX = x + hint.width() + spaceX;
            lineHeight = 0;
        }
        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), hint));
        x = nextX;
        lineHeight = std::max(lineHeight, hint.height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
    QObject *par = parent();
    if (!par) return -1;
    if (par->isWidgetType()) {
        QWidget *pw = static_cast<QWidget*>(par);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    return static_cast<QLayout*>(par)->spacing();
}
