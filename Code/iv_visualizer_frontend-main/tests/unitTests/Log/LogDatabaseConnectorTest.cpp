#include <gtest/gtest.h>
#include "LogDatabaseConnector.h"
#include <unordered_set>
#include <QSettings>
#include "./../../../source/Log/Log.h"
#include "./../../../source/main/MainWindow.h"

// Test fixture for LogDatabaseConnector class
class LogDatabaseConnectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any objects or variables needed for tests
        connector = new LogDatabaseConnector();
    }

    void TearDown() override {
        // Clean up any objects created in SetUp()
        delete connector;
    }

    LogDatabaseConnector* connector;
};

// Test LogDatabaseConnector::get_base_url
TEST_F(LogDatabaseConnectorTest, GetBaseUrlTest) {
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    ASSERT_EQ(connector->get_base_url(), settings.value("loki/url").toString());
}

/*// Test LogDatabaseConnector::get_log_data
TEST_F(LogDatabaseConnectorTest, GetLogDataTest) {
    // Set up any necessary data for the test
    std::unordered_set<LogType> types;
    types.insert(LogType::DEBUG);
    types.insert(LogType::ERROR);
    uint64_t start_timestamp = 1234567890;

    // Call the get_log_data method
    QVector<Log*> logs = connector->get_log_data(types, start_timestamp);

    for(Log* log:logs){
        ASSERT_EQ(log->get_timestamp(), start_timestamp);
        ASSERT_EQ(log->get_description().isEmpty(), std::string("-test-output").empty());
        ASSERT_EQ(log->get_log_type(), LogType::DEBUG);
    }

}
*/
