#include "ColorDict.h"
#include <QColor>
#include <gtest/gtest.h>

// Test fixture for ColorDict
class ColorDictTest : public ::testing::Test {
protected:
    void SetUp() override {
        color_dict = ColorDict::get_color_dict();
    }

    void TearDown() override {
    }

    ColorDict* color_dict;
};

// Test the get_color method
TEST_F(ColorDictTest, GetColorTest) {
    // Test getting a color for a label
    QColor color = color_dict->get_color("label");
    // Assert that the returned color is valid
    ASSERT_TRUE(color.isValid());
}

// Test the uniqueness of generated colors
TEST_F(ColorDictTest, UniqueColorsTest) {
    // Generate colors for a set of labels
    QColor color1 = color_dict->get_color("label1");
    QColor color2 = color_dict->get_color("label2");
    QColor color3 = color_dict->get_color("label3");

    // Assert that the generated colors are unique
    ASSERT_NE(color1, color2);
    ASSERT_NE(color1, color3);
    ASSERT_NE(color2, color3);
}

// Test the get_color method for the same label
TEST_F(ColorDictTest, SameLabelTest) {
    // Get colors for the same label multiple times
    QColor color1 = color_dict->get_color("same_label");
    QColor color2 = color_dict->get_color("same_label");
    QColor color3 = color_dict->get_color("same_label");

    // Assert that the generated colors are the same
    ASSERT_EQ(color1, color2);
    ASSERT_EQ(color1, color3);
}

// Test the range of generated colors
TEST_F(ColorDictTest, ColorRangeTest) {
    // Test getting colors for a large number of labels
    const int num_labels = 1000;
    for (int i = 0; i < num_labels; ++i) {
        QColor color = color_dict->get_color(QString("label_%1").arg(i));
        // Assert that the hue is within the range [0, 359]
        ASSERT_GE(color.hue(), 0);
        ASSERT_LE(color.hue(), 359);
    }
}
