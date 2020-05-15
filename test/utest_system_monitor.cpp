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
TEST(FormattingTest, TestStringTime) {
    EXPECT_EQ(Format::StringTime(0), "00");
    EXPECT_EQ(Format::StringTime(9), "09");
    EXPECT_EQ(Format::StringTime(14), "14");
    EXPECT_EQ(Format::StringTime(120), "120");
}

TEST(FormattingTest, TestElapsedTime) {
    EXPECT_EQ(Format::ElapsedTime(0), "00:00:00");
    EXPECT_EQ(Format::ElapsedTime(1), "00:00:01");
    EXPECT_EQ(Format::ElapsedTime(120), "00:02:00");
    EXPECT_EQ(Format::ElapsedTime(3601), "01:00:01");
    EXPECT_EQ(Format::ElapsedTime(3600*260+60*44+15), "260:44:15");
}