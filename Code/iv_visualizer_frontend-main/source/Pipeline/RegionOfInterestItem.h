#ifndef REGIONOFINTERESTITEM_H
#define REGIONOFINTERESTITEM_H

#include "qgraphicsitem.h"
#include "../FrameData/Data/RegionOfInterest.h"

/**
 * @brief Class representing a region of interest item.
 */
class RegionOfInterestItem : public QGraphicsItem
{
public:
    /**
     * @brief Construct a new RegionOfInterestItem object.
     * 
     * @param a_interest_is_inside Flag indicating if the interest is inside or outside.
     * @param a_bounds Bounds of the region of interest.
     * @param parent Parent item.
     */
    RegionOfInterestItem(bool a_interest_is_inside, const QRectF &a_bounds, QGraphicsItem *parent = nullptr);

    /**
     * @brief Get the bounding rectangle of the item.
     * 
     * @return QRectF Bounding rectangle.
     */
    QRectF boundingRect() const override;

    /**
     * @brief Draw the region of interest.
     * 
     * @param a_polygon Region of interest to draw.
     */
    void draw_region_of_interest(const RegionOfInterest &a_polygon);

    /**
     * @brief Paint the item.
     * 
     * @param a_painter Painter object.
     * @param option Graphics style option.
     * @param widget Widget.
     */
    void paint(QPainter *a_painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    /**
     * @brief Get the color with highest alpha.
     * 
     * @return QColor Color with highest alpha.
     */
    QColor color_with_highest_alpha();

private:
    QRectF m_region_of_interest_rect; /**< Region of interest rectangle. */
    std::vector<std::pair<QPainterPath, QColor>> m_paths_with_colors; /**< Vector of paths with colors. */
    bool m_interest_is_inside; /**< Flag indicating if the interest is inside or outside. */
    QPainterPath m_region_of_interest_path; /**< Region of interest path. */
};

#endif // REGIONOFINTERESTITEM_H
