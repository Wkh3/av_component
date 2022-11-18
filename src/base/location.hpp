#pragma once
#include <string>
namespace component::base{

class Location{
public:
    Location() = default;

    Location(const char *fn,const char* file,int line) : fn_(fn),file_(file),line_(line){
    }
    const char *function_name() const {
        return fn_;
    }
    
    const char *file_name() const {
        return file_;
    }

    int line_number() const{
        return line_;
    }

    std::string ToString() const;
private:
    const char *fn_   = "UnKnown";
    const char *file_ = "UnKnown";
    int         line_ = -1;
};

#define FROM_HERE FROM_HERE_WITH_FUNCTION(__FUNCTION__)

#define FROM_HERE_WITH_FUNCTION(fn)\
    ::component::base::Location(fn,__FILE__,__LINE__)

}