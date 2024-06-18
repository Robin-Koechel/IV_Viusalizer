#include <gtest/gtest.h>
#include "JsonSettingsSerializer.h"
#include "./../../../source/BackendConnector/Stream.h"
#include "./../../../source/BackendStub/MockReader.h"
#include "./../../../source/BackendStub/MockWriter.h"
#include "./../../../source/BackendConnector/TimeRange.h"

class JsonSettingsSerializerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize necessary variables or objects before each test case
    }

    void TearDown() override {
        // Clean up any allocated resources after each test case
    }
};

TEST_F(JsonSettingsSerializerTest, ExportAndImportStreams) {
    MockReader* reader = new MockReader("myUrl", new TimeRange(10000, 20000));
    MockWriter* writer = new MockWriter("myUrl", new TimeRange(10000, 20000), reader);

    // Create some sample streams
    QVector<Stream*> streams;
    // Populate streams vector with sample Stream objects
    for(int i = 0; i < 3;i++){
        streams.append(new Stream(reader, writer));
        streams.at(i)->set_name(QString::number(i));
    }

    // Set up file name for export and import
    QString file_name = "unit_test_streams.json";

    // Export the streams to JSON file
    JsonSettingsSerializer serializer;
    serializer.export_streams(streams, file_name);

    // Import the streams from JSON file
    QVector<Stream*> imported_streams = serializer.import_streams(file_name, false);

    // Assert that imported streams are not empty and contain the same data as exported streams
    ASSERT_FALSE(imported_streams.empty());
    for(int i = 0; i < 3; i++){
        ASSERT_EQ(streams.at(0)->get_name(), imported_streams.at(0)->get_name());
    }
    
}