#pragma once

#include <base/error/error.hpp>
#include <base/location.hpp>
#include <optional>
namespace component::base {

class RTCError : public Error<RTCError> {
public:
    enum Code {
        kNone
    };
    RTCError(Code code);

    RTCError(Code code, std::string reason);

    bool HasError() const;

    bool operator=(RTCError::Code code);

    void set_reason(std::string reason);

    const std::string& reason() const;

    Code error_code() const;

public:
    static RTCError Ok();

protected:
    std::string_view Message() const;

    void Swap(RTCError& rhs);

private:
    Code code_;
    std::string reason_;
};

} // namespace component::base