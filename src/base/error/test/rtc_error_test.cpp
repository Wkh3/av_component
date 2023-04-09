#include <gtest/gtest.h>
#include <base/error/rtc_error.hpp>
TEST(RTCError,Error){
    using namespace component::base;
    auto err = MakeError<RTCError>(RTCError::kRTCError, "init failed",FROM_HERE);
    EXPECT_EQ(err,RTCError::kRTCError);
}
