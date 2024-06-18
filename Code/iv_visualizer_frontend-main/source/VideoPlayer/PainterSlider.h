#ifndef PAINTERSLIDER_H
#define PAINTERSLIDER_H

#include "qslider.h"
#include <QList>

class PainterSlider : public QSlider {
    Q_OBJECT

public:
    explicit PainterSlider(QWidget *parent = nullptr);

    void set_marks(const QList<uint64_t> &a_positions);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<uint64_t> m_marks;
};


#endif // PAINTERSLIDER_H

