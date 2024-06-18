#include <gtest/gtest.h>
#include "./../../../source/FrameData/Data/RegionOfInterest.h"
#include "./../../../source/Pipeline/RegionOfInterestItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
// Test fixture for RegionOfInterestItem
class RegionOfInterestItemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any objects or variables needed for tests
        item = new RegionOfInterestItem(true, QRectF(0, 0, 100, 100));
    }

    void TearDown() override {
        // Clean up any objects created in SetUp()
        delete item;
    }

    RegionOfInterestItem* item;
};

// Test RegionOfInterestItem::boundingRect
TEST_F(RegionOfInterestItemTest, BoundingRectTest) {
    QRectF boundingRect = item->boundingRect();

    ASSERT_EQ(boundingRect.width(), 100);
    ASSERT_EQ(boundingRect.height(), 100);
}

