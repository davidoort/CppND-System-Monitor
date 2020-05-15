#include "gtest/gtest.h"
#include "../include/format.h"
#include <string>

//--------------------------------//
//   Helper Functions
//--------------------------------//


//--------------------------------//
//   Beginning SystemMonitor Tests.
//--------------------------------//

// class SystemMonitorTest : public ::testing::Test {

// };

// Test the Format methods.
TEST(SystemMonitorTest, TestFormatting) {
    EXPECT_EQ(Format::ElapsedTime(120), "00:01:00");
}