#include <gtest/gtest.h>
#include "./../../../source/FrameData/Data/DataFrame.h"

// Fixture for DataFrame tests
class DataFrameTest : public ::testing::Test {
protected:
    // Common setup for all tests
    void SetUp() override {
        // Initialize common resources if needed
    }

    // Common teardown for all tests
    void TearDown() override {
        // Cleanup common resources if needed
    }
};

// Test cases

// Test default constructor
TEST_F(DataFrameTest, DefaultConstructor) {
    DataFrame dataFrame;
    
    // Assert that the URL is empty
    ASSERT_EQ(dataFrame.get_url(), "");
    
    // Assert that the image is initialized properly
    ASSERT_EQ(dataFrame.get_image(), Image());
    
    // Assert that the regions of interest and bounding boxes are empty
    ASSERT_TRUE(dataFrame.get_regions_of_interest().empty());
    ASSERT_TRUE(dataFrame.get_bounding_boxes().empty());
}

// Test constructor with parameters
TEST_F(DataFrameTest, ConstructorWithParameters) {
    QString url = "https://example.com/image.jpg";
    Image image;
    QVector<RegionOfInterest*> rois;
    QVector<BoundingBox*> bboxes;

    // Create DataFrame object with parameters
    DataFrame dataFrame(url, image, rois, bboxes);

    // Assert that the DataFrame is initialized with the provided parameters
    ASSERT_EQ(dataFrame.get_url(), url);
    ASSERT_EQ(dataFrame.get_image().get_timestamp(), image.get_timestamp());
    ASSERT_EQ(dataFrame.get_regions_of_interest(), rois);
    ASSERT_EQ(dataFrame.get_bounding_boxes(), bboxes);
}

// Test setters and getters
TEST_F(DataFrameTest, SettersAndGetters) {
    DataFrame dataFrame;
    QString url = "https://example.com/image.jpg";
    Image image;
    QVector<RegionOfInterest*> rois;
    QVector<BoundingBox*> bboxes;

    // Set values using setters
    dataFrame.setUrl(url);
    dataFrame.set_image(image);
    dataFrame.set_regions_of_interest(rois);
    dataFrame.set_bounding_boxes(bboxes);

    // Assert that getters return the set values
    ASSERT_EQ(dataFrame.get_url(), url);
    ASSERT_EQ(dataFrame.get_image().get_timestamp(), image.get_timestamp());
    ASSERT_EQ(dataFrame.get_regions_of_interest(), rois);
    ASSERT_EQ(dataFrame.get_bounding_boxes(), bboxes);
}