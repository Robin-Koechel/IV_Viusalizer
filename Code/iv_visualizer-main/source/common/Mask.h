#ifndef MASK_H
#define MASK_H

#include <QPolygon>
#include <QString>
#include <QColor>

class Mask {
  private:
    bool intersectsInside;
    QPolygon polygon;
    QString name;
    QColor color;
    int id;

  public:
    // Constructors
    Mask();
    Mask(bool intersects, const QPolygon &poly, const QString &name, int id);

    // Destructor
    ~Mask();

    // Getters
    bool getIntersectsInside() const;
    QPolygon getPolygon() const;
    QString getName() const;
    QColor getColor() const;
    int getId() const;

    // Setters
    void setIntersectsInside(bool intersects);
    void setPolygon(const QPolygon &poly);
    void setName(const QString &name);
    void setColor(const QColor &color);
    void setId(int id);
};

#endif // MASK_H
