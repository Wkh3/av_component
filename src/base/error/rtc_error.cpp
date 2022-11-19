#include <base/error/rtc_error.hpp>
#include <unordered_map>
namespace component::base {

namespace {
std::unordered_map<RTCError::Code, std::string_view> errors_{
    {RTCError::kNone, "no error"},
};

}

RTCError::RTCError(Code code)
        : code_(code) {
}

RTCError::RTCError(Code code, std::string reason)
        : code_(code), reason_(std::move(reason)) {
}

std::string_view RTCError::Message() const {
    auto it = errors_.find(code_);
    return it == errors_.end() ? "unknown-error" : it->second;
}

RTCError RTCError::Ok() {
    return RTCError(Code::kNone);
}

bool RTCError::HasError() const {
    return code_ != Code::kNone;
}

void RTCError::Swap(RTCError& rhs) {
    std::swap(code_, rhs.code_);
    std::swap(reason_, rhs.reason_);
}
void RTCError::set_reason(std::string reason) {
    reason_ = std::move(reason);
}

const std::string& RTCError::reason() const {
    return reason_;
}

RTCError::Code RTCError::error_code() const {
    return code_;
}
bool RTCError::operator=(RTCError::Code code) {
    return code_ == code;
}

} // namespace component::base