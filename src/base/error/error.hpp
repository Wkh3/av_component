#pragma once

#include <string_view>
#include <base/location.hpp>
namespace component::base{

template<typename T>
class Error{
public:
    Error()  = default;
    ~Error() = default;

    Error(Location location) : location_(location){

    }
    operator bool(){
        return static_cast<T>(this).HasError();
    }

    std::string_view Detail(){
        return static_cast<T>(this).Message();
    }

    const Location &location(){
        return location_;
    }

    std::string Dump() const{
       return static_cast<T>(this).Dump(); 
    }
protected:
    Location location_;
};


}