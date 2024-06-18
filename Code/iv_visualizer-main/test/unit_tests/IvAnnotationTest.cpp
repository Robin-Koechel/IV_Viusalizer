#include <gtest/gtest.h>
#include <BoundingBox.h>

using namespace iv_annotation;

TEST(BoundingBoxTest, CheckCoordinates) {
    // Generate random values for the bounding box
    std::vector<int> expectedCoords = {10, 20, 50, 30};  // Example coordinates (x, y, width, height)
    float confidence = 0.85;  // Example confidence value
    QString label = "Object";  // Example label
    cv::Size2i imageSize(640, 480);  // Example image size (width, height)
    BoundingBox::Format format = BoundingBox::Format::TLWH;  // Example format
    int rotation = 45;  // Example rotation angle

    // Create BoundingBox object
    BoundingBox boundingBox(expectedCoords, confidence, label, imageSize, format, rotation);

    // Get coordinates in the expected format
    std::vector<int> actualCoords = boundingBox.get_coords(format);

    // Check if the actual coordinates match the expected coordinates
    ASSERT_EQ(actualCoords, expectedCoords);
}
