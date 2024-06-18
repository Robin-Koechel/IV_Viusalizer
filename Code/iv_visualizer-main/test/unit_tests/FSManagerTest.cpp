#include "FSManager.h"
#include "gtest/gtest.h"

class FSManagerTest : public ::testing::Test {
 protected:
};

// TODO more tests
TEST_F(FSManagerTest, UserConstructor) {
  fsm::FSManager fsm = fsm::FSManager("rtsp://admin:passwort@181.112.101.124:554/Streaming/Channels/103");
}
