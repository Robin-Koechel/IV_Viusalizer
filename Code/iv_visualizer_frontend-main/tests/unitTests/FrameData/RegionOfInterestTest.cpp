#include "gtest/gtest.h"
#include "./../../../source/FrameData/Data/RegionOfInterest.h"

// Verifies the default constructor
TEST(RegionOfInterestTest, DefaultConstructor) {
    RegionOfInterest roi;
    EXPECT_EQ(roi.get_name(), "");
    EXPECT_EQ(roi.get_id(), 0);
    EXPECT_TRUE(roi.get_polygon().isEmpty());
    EXPECT_FALSE(roi.is_inside());
    EXPECT_EQ(roi.get_color(), QColor(Qt::black));
    EXPECT_FLOAT_EQ(roi.get_opacity(), 1.0);
}

// Verifies the parameterized constructor
TEST(RegionOfInterestTest, ParameterizedConstructor) {
    QString name = "Test ROI";
    int id = 1;
    QPolygon polygon;
    polygon << QPoint(0, 0) << QPoint(100, 0) << QPoint(100, 100) << QPoint(0, 100);
    bool is_inside = true;
    float opacity = 0.5;

    RegionOfInterest roi(name, id, polygon, is_inside, opacity);
    EXPECT_EQ(roi.get_name(), name);
    EXPECT_EQ(roi.get_id(), id);
    EXPECT_EQ(roi.get_polygon(), polygon);
    EXPECT_EQ(roi.is_inside(), is_inside);
    EXPECT_FLOAT_EQ(roi.get_opacity(), opacity);
}

// Verifies setters and getters
TEST(RegionOfInterestTest, SettersAndGetters) {
    RegionOfInterest roi;

    QString name = "Test ROI";
    roi.set_name(name);
    EXPECT_EQ(roi.get_name(), name);

    int id = 1;
    roi.set_id(id);
    EXPECT_EQ(roi.get_id(), id);

    QPolygon polygon;
    polygon << QPoint(0, 0) << QPoint(100, 0) << QPoint(100, 100) << QPoint(0, 100);
    roi.set_polygon(polygon);
    EXPECT_EQ(roi.get_polygon(), polygon);

    bool is_inside = true;
    roi.set_is_inside(is_inside);
    EXPECT_EQ(roi.is_inside(), is_inside);

    QColor color(Qt::red);
    roi.set_color(color);
    EXPECT_EQ(roi.get_color(), color);

    float opacity = 0.5;
    roi.set_opacity(opacity);
    EXPECT_FLOAT_EQ(roi.get_opacity(), opacity);
}

