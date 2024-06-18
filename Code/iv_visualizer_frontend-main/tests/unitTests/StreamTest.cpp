#include <gtest/gtest.h>
#include "Stream.h"

// Test fixture for Stream class
class StreamTest : public ::testing::Test {
protected:
    // Set up before each test
    void SetUp() override {
        // Create a Stream object with sample URL and gRPC URL
        sample_url_stream = "http://example.com/stream";
        sample_url_grpc = "http://example.com/grpc";
        TimeRange* time = new TimeRange(0,18000ULL * 1000000000ULL);
        MockReader* mockreader = new MockReader(sample_url_stream, time);
        MockWriter* mockwriter = new MockWriter(sample_url_stream, time, mockreader);
        stream = new Stream(mockreader, mockwriter);
    }

    // Clean up after each test
    void TearDown() override {
        delete stream;
    }

    // Declare member variables
    Stream* stream;
    QString sample_url_stream;
    QString sample_url_grpc;
};

// Test case to verify the getter for name
TEST_F(StreamTest, GetName) {
    // Set a sample name and check if the getter returns it
    QString sample_name = "SampleStream";
    stream->set_name(sample_name);
    EXPECT_EQ(sample_name, stream->get_name());
}

// Test case to verify the stop_stream method
TEST_F(StreamTest, StopStream) {
    // Verify that the method does not cause a crash
    stream->stop_stream();
    // Additional assertions could be added here based on the expected behavior of stop_stream
}