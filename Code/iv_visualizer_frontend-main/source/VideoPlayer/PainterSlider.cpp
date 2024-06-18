#include "PainterSlider.h"
#include <QPainter>
#include <QMouseEvent>

PainterSlider::PainterSlider(QWidget *parent) : QSlider(parent) {
    setOrientation(Qt::Horizontal);
}

void PainterSlider::paintEvent(QPaintEvent *event) {
    QSlider::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_marks.size() == 2) {
        uint64_t pos1 = (m_marks[0] - minimum()) * width() / (maximum() - minimum());
        uint64_t pos2 = (m_marks[1] - minimum()) * width() / (maximum() - minimum());

        if (pos1 > pos2) {
            std::swap(pos1, pos2);
        }

        QRect rect(pos1, 0, pos2 - pos1, height());

        QColor color = m_marks[0] < m_marks[1] ? Qt::green : Qt::red;
        color.setAlpha(127);

        painter.fillRect(rect, color);
    }

    for (int mark : m_marks) {
        int pos = (mark - minimum()) * width() / (maximum() - minimum());
        painter.setPen(Qt::black);
        painter.drawLine(pos, 0, pos, height());
    }
}



void PainterSlider::set_marks(const QList<uint64_t> &a_positions) {
    int mark1 = a_positions[0];
    int mark2 = a_positions[1];
    m_marks = a_positions;
    update();
}
