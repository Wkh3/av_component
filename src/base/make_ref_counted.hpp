#pragma once
#include <type_traits>
#include <utility>

#include "base/ref_count.hpp"
#include "base/ref_counted_object.hpp"
#include "base/scoped_refptr.hpp"
namespace component::base {

template <typename T, typename... Args,
          typename = std::enable_if_t<std::is_convertible_v<T*, RefCountInterface*> && std::is_abstract_v<T>>>
scoped_refptr<T> make_ref_counted(Args&&... args) {
    return scoped_refptr<T>(new RefCountedObject<T>(std::forward<Args>(args)...));
}

} // namespace component::base