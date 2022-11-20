#pragma once

#include <base/error/error.hpp>
#include <base/location.hpp>
#include <optional>
namespace component::base {

class RTCError : public Error<RTCError> {
public:
    enum Code : int {
        kRTCError = 100,
    };
    using Error::Error;

    RTCError(int code, std::string reason, Location location);

    RTCError(const RTCError& rhs);

    RTCError(RTCError&& rhs);

    RTCError& operator=(RTCError rhs);

    bool HasError() const;

    void set_reason(std::string reason);

    const std::string& reason() const;

    std::string Detail() const;

    void swap(RTCError& rhs);

public:
    static RTCError Ok();

protected:
    std::string_view Message() const;

private:
    std::string reason_;
};

} // namespace component::base