#include "base/location.hpp"

namespace component::base {

std::string Location::ToString() const {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s@%s:%d", fn_.data(), file_.data(), line_);
    return buf;
}

} // namespace component::base