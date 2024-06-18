#include "MessageHandler.h"
#include <gtest/gtest.h>

// Define a test fixture for the MessageHandler tests
class MessageHandlerTest : public ::testing::Test {
protected:
    // SetUp() is called before each test
    void SetUp() override {
        // Get an instance of MessageHandler before each test
        MessageHandler::getInstance();
    }

    // TearDown() is called after each test
    void TearDown() override {
        // Clean up any resources after each test if needed
    }
};

// Test the Singleton pattern by ensuring only one instance is created
TEST_F(MessageHandlerTest, SingletonPattern) {
    // Get two instances of MessageHandler
    MessageHandler* instance1 = MessageHandler::getInstance();
    MessageHandler* instance2 = MessageHandler::getInstance();

    // Check that both instances point to the same memory address
    EXPECT_EQ(instance1, instance2);
}
