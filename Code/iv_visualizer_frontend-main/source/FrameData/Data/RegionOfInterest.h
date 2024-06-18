#ifndef REGIONOFINTEREST_H
#define REGIONOFINTEREST_H

#include <QPolygon>
#include <QString>
#include <QColor>


/**
 * @brief Represents a region of interest(roi).
 *
 * This class is used to define a region of interest with various properies such
 * as the name, the id, the polygon, color and transparency of the region of
 * interest.
 */

class RegionOfInterest {
private:
    QString m_name;
    int m_id;
    QPolygon m_polygon;
    bool m_is_inside;
    QColor m_color;
    float m_opacity;
    const QColor default_color = QColor(255, 255, 255);

public:
    RegionOfInterest();
    /**
   * @brief Construct a new RegionOfInterest object
   *
   * @param a_name The name of the roi.
   * @param a_id The unique identifyer.
   * @param a_polygon The polygon defining the roi.
   * @param a_is_inside true if the roi is inside the polygon, false if the roi
   * is the area outside the polygon
   * @param a_opacity The transparency of the roi (100 = not transparent, 0 =
   * very transparent)
   *
   */
    RegionOfInterest(const QString& a_name, const int a_id,
                     const QPolygon& a_polygon, const bool a_is_inside,
                     const float a_opacity);

    // Destructor
    ~RegionOfInterest();

    /**
   * @brief get true if the roi is inside the polygon, false if the roi is the
   * area outside the polygon
   *
   * @return bool if roi is inside or not
   */
    bool is_inside() const;

    /**
   * @brief get the polygon defining the roi
   *
   * @return QPolygon& the shape of the roi
   */
    const QPolygon& get_polygon() const;

    /**
   * @brief get the name of the roi
   *
   * @return QString& the representation of the roi name
   */
    const QString& get_name() const;

    /**
   * @brief get the id of the roi
   *
   * @return int unique identifyer of the roi
   */
    int get_id() const;

    /**
   * @brief get the color representation of the roi
   *
   * @return QColor& the color of the roi
   */
    const QColor& get_color() const;

    /**
   * @brief get the transparency of the roi
   *
   * @return float (100 = not transparent, 0 = very transparent)
   */
    float get_opacity() const;

    /**
   * @brief set the name of the roi
   *
   * @param a_name the new name of the roi
   */
    void set_name(const QString& a_name);

    /**
   * @brief set the id of the roi
   *
   * @param a_id the new id of the roi
   */
    void set_id(const int a_id);

    /**
   * @brief set the polygon of the roi
   *
   * @param a_polygon the new polygon/shape of the roi
   */
    void set_polygon(const QPolygon& a_polygon);

    /**
   * @brief set if the roi is inside or outside of the roi polygon
   *
   * @param a_is_inside true if the roi is inside the polygon, false if the roi
   * is the area outside the polygon
   */
    void set_is_inside(const bool a_is_inside);

    /**
   * @brief set the color of the roi
   *
   * @param a_color the new color of the roi
   */
    void set_color(const QColor& a_color);

    /**
   * @brief set the transparency of the roi
   *
   * @param polygon the new transparency of the roi
   */
    void set_opacity(const float a_opacity);
};
#endif  // REGIONOFINTEREST_H
