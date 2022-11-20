#include <base/error/rtc_error.hpp>
#include <unordered_map>
namespace component::base {

namespace {

std::unordered_map<int, std::string_view> errors_{
    {RTCError::kRTCError, "RTC util error"},
};

}

RTCError::RTCError(int code, std::string reason, Location location)
        : Error(code, location), reason_(std::move(reason)) {
}

RTCError::RTCError(const RTCError& rhs)
        : RTCError(rhs.code_, rhs.reason_, rhs.location_) {
}

RTCError::RTCError(RTCError&& rhs) {
}

RTCError& RTCError::operator=(RTCError rhs) {
    rhs.swap(*this);
    return *this;
}

std::string_view RTCError::Message() const {
    auto it = errors_.find(code_);
    return it == errors_.end() ? Error::Message() : it->second;
}

std::string RTCError::Detail() const {
    char buf[256];
    snprintf(buf, sizeof(buf), "[%s] code = %d desc = %s, error reason = %s",
             location_.ToString().c_str(), code_, Message().data(), reason_.data());
    return buf;
}
RTCError RTCError::Ok() {
    return RTCError(Error::kNoneError);
}

bool RTCError::HasError() const {
    return code_ != Error::kNoneError;
}

void RTCError::swap(RTCError& rhs) {
    std::swap(code_, rhs.code_);
    std::swap(reason_, rhs.reason_);
}
void RTCError::set_reason(std::string reason) {
    reason_ = std::move(reason);
}

const std::string& RTCError::reason() const {
    return reason_;
}

} // namespace component::base