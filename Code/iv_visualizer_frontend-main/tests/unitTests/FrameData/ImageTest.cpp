#include "gtest/gtest.h"
#include "./../../../source/FrameData/Data/Image.h"
#include <QSettings>

class ImageTest : public ::testing::Test {
protected:
    // Test data
    const uchar* test_data = nullptr;
    int test_width = 100;
    int test_height = 100;
    uint64_t test_timestamp = 123456789;

    void SetUp() override {
        // Set up test data
        test_data = new uchar[test_width * test_height * 3]; // RGB888 format
    }

    void TearDown() override {
        // Tear down test data
        delete[] test_data;
    }
};

TEST_F(ImageTest, ConstructorWithData) {
    QSettings settings;

    settings.setValue("grpc_mock/default_image_background_color_red", 0);
    settings.setValue("grpc_mock/default_image_background_color_green", 255);
    settings.setValue("grpc_mock/default_image_background_color_blue", 0);

    Image image(test_data, test_width, test_height, test_timestamp);
    EXPECT_EQ(image.width(), test_width);
    EXPECT_EQ(image.height(), test_height);
    EXPECT_EQ(image.get_timestamp(), test_timestamp);
}

TEST_F(ImageTest, ConstructorWithDimensions) {
    QSettings settings;
    
    settings.setValue("grpc_mock/default_image_background_color_red", 0);
    settings.setValue("grpc_mock/default_image_background_color_green", 255);
    settings.setValue("grpc_mock/default_image_background_color_blue", 0);

    Image image(test_width, test_height, test_timestamp);
    EXPECT_EQ(image.width(), test_width);
    EXPECT_EQ(image.height(), test_height);
    EXPECT_EQ(image.get_timestamp(), test_timestamp);
}

TEST_F(ImageTest, DefaultConstructor) {
    QSettings settings;
    
    settings.setValue("grpc_mock/default_image_background_color_red", 0);
    settings.setValue("grpc_mock/default_image_background_color_green", 255);
    settings.setValue("grpc_mock/default_image_background_color_blue", 0);

    Image image;
    EXPECT_EQ(image.width(), 0);
    EXPECT_EQ(image.height(), 0);
    EXPECT_EQ(image.get_timestamp(), 0);
}

TEST_F(ImageTest, SetAndGetTimestamp) {
    Image image;
    image.set_timestamp(test_timestamp);
    EXPECT_EQ(image.get_timestamp(), test_timestamp);
}

TEST_F(ImageTest, AssignmentOperator) {
    Image image1(test_data, test_width, test_height, test_timestamp);
    Image image2;
    image2 = image1;
    EXPECT_EQ(image1.width(), image2.width());
    EXPECT_EQ(image1.height(), image2.height());
    EXPECT_EQ(image1.get_timestamp(), image2.get_timestamp());
    // Add more checks if needed
}

TEST_F(ImageTest, GetQImage) {
    QSettings settings;
    
    settings.setValue("grpc_mock/default_image_background_color_red", 0);
    settings.setValue("grpc_mock/default_image_background_color_green", 255);
    settings.setValue("grpc_mock/default_image_background_color_blue", 0);

    Image image(test_data, test_width, test_height, test_timestamp);
    QImage qimage = image.get_qimage();
    EXPECT_EQ(qimage.width(), test_width);
    EXPECT_EQ(qimage.height(), test_height);
    // Add more checks if needed
}