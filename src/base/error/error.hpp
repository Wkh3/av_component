#pragma once

#include <base/location.hpp>
#include <string_view>
#include <unordered_map>
namespace component::base {

template <typename T>
class Error {
public:
    Error()  = default;
    ~Error() = default;

    enum Code : int {
        kNoneError
    };

    Error(Location location)
            : location_(location) {
    }
    Error(int code, Location location)
            : code_(code), location_(location) {
    }
    Error(int code)
            : code_(code) {
    }
    operator bool() const {
        return static_cast<const T*>(this)->HasError();
    }

    std::string_view Description() const {
        return static_cast<const T*>(this)->Message();
    }

    std::string Detail() const {
        return static_cast<const T*>(this)->Detail();
    }

    const Location& location() const {
        return location_;
    }

    int error_code() const {
        return code_;
    }

    bool operator==(int code) const {
        return code_ == code;
    }

public:
    static T Ok() {
        return T(kNoneError);
    }

protected:
    std::string_view Message() const {
        auto it = util_errors_.find(code_);
        return it == util_errors_.end() ? error_ : it->second;
    }

    static inline std::unordered_map<int, std::string_view> util_errors_{
        {kNoneError, "no error"},
    };
    static inline std::string_view error_ = "unknown_error";

protected:
    int code_;
    Location location_;
};

template <typename T>
T MakeError(int code, std::string reason, Location location) {
    return T(code, reason, location);
}

} // namespace component::base