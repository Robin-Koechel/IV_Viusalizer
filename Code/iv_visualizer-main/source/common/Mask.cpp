#include "Mask.h"

// Constructors
Mask::Mask() : intersectsInside(false), id(0) {}

Mask::Mask(bool intersects, const QPolygon &poly, const QString &name, int id)
    : intersectsInside(intersects), polygon(poly), name(name), id(id) {}

// Destructor
Mask::~Mask() {}

// Getters
bool Mask::getIntersectsInside() const { return intersectsInside; }
QPolygon Mask::getPolygon() const { return polygon; }
QString Mask::getName() const { return name; }
QColor Mask::getColor() const { return color; }
int Mask::getId() const { return id; }

// Setters
void Mask::setIntersectsInside(bool intersects) { intersectsInside = intersects; }
void Mask::setPolygon(const QPolygon &poly) { polygon = poly; }
void Mask::setName(const QString &name) { this->name = name; }
void Mask::setColor(const QColor &color) { this->color = color; }
void Mask::setId(int id) { this->id = id; }
