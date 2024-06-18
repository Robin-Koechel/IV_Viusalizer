#include <gtest/gtest.h>
#include "./../../../source/DataStreams/DataFrameRingBuffer.h"

class DataFrameRingbufferTest : public ::testing::Test {
protected:
    DataFrameRingbuffer* buffer;

    virtual void SetUp() {
        buffer = new DataFrameRingbuffer(10); // 10 ist ein Beispielwert für die Größe
    }

    virtual void TearDown() {
        delete buffer;
    }
};


TEST_F(DataFrameRingbufferTest, PushIncreasesSize) {
    DataFrame* frame = new DataFrame();
    buffer->push(frame);
    ASSERT_EQ(1, buffer->get_size()); 
}

TEST_F(DataFrameRingbufferTest, PushOverCapacity) {
    for (int i = 0; i < buffer->get_capacity() + 1; ++i) {
        DataFrame* frame = new DataFrame();
        buffer->push(frame);
    }
    ASSERT_EQ(buffer->get_capacity(), buffer->get_size()); 
}


TEST_F(DataFrameRingbufferTest, SearchForExactMatch) {
    DataFrame* frame = new DataFrame();
    Image* image = new Image(100, 100, 11);
    frame->set_image(*image);
    buffer->push(frame); 
    for(int i = 0; i < 5; i++){
        DataFrame* framei = new DataFrame();
        Image* imagei = new Image(100, 100, i*2+20);
        framei->set_image(*imagei);
        buffer->push(framei); 
    }
    ASSERT_EQ((*frame).get_image().get_timestamp(), (*buffer).search_ring_buffer(11)->get_image().get_timestamp());
}

TEST_F(DataFrameRingbufferTest, SearchForClosestMatch) {
    DataFrame* frame = new DataFrame();
    Image* image = new Image(100, 100, 11);
    frame->set_image(*image);
    buffer->push(frame); 
    for(int i = 0; i < 6; i++){
        DataFrame* framei = new DataFrame();
        Image* imagei = new Image(100, 100, i*3+20);
        framei->set_image(*imagei);
        buffer->push(framei); 
    }
    ASSERT_EQ((*frame).get_image().width(), (*buffer).search_ring_buffer(10)->get_image().width());
}

