#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include "Encoder.h"

class EncoderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary resources before each test
        QCoreApplication app();
    }

    void TearDown() override {
        // Clean up any allocated resources after each test
    }
};

TEST_F(EncoderTest, AddImageAndFinishWriting) {
    // Create an Encoder object
    QString file_name = "unit_test_video";
    int fps = 30;
    Encoder encoder(file_name, fps);

    for(int i = 0; i < fps*4;i++){
        // Create a sample QImage (e.g., with random pixel values)
        QImage image(640, 480, QImage::Format_RGB888);
        image.fill(Qt::white); // Fill with white color (you can modify this as needed)

        // Add the image to the encoder
        encoder.add_image(&image);
    }
    // Finish writing the video
    encoder.finish_writing_mp4();

    // Assert that the video file has been created
    QFile video_file(file_name + ".mp4");
    ASSERT_TRUE(video_file.exists());
}

TEST_F(EncoderTest, CancelWriting) {
    // Create an Encoder object
    QString file_name = "unit_test_video";
    int fps = 30;
    Encoder encoder(file_name, fps);

    // Create a sample QImage (e.g., with random pixel values)
    QImage image(640, 480, QImage::Format_RGB888);
    image.fill(Qt::white); // Fill with white color (you can modify this as needed)

    // Add the image to the encoder
    encoder.add_image(&image);

    // Cancel writing the video
    encoder.cancel_writing_mp4();

    // Assert that the video file has been removed
    QFile video_file(file_name + ".mp4");
    ASSERT_FALSE(video_file.exists());
}