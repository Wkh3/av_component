#pragma once
#include <ostream>
#include <string>
namespace component::base {


class Location {
public:
    Location() = default;

    Location(const char* fn, const char* file, int line)
            : fn_(fn), file_(file), line_(line) {
    }
    std::string_view function_name() const {
        return fn_;
    }

    std::string_view file_name() const {
        return file_;
    }

    int line_number() const {
        return line_;
    }

    std::string ToString() const;

private:
    std::string_view fn_   = "UnKnown";
    std::string_view file_ = "UnKnown";
    int line_              = -1;
};


#define FROM_HERE FROM_HERE_WITH_FUNCTION(__FUNCTION__)

#define FROM_HERE_WITH_FUNCTION(fn) \
    ::component::base::Location(fn, __FILE__, __LINE__)

} // namespace component::base

inline std::ostream& operator<<(std::ostream &os,const component::base::Location &location){
    os << location.ToString();
    return os;
}