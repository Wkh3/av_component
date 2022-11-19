#pragma once
#include <atomic>

#include "base/ref_count.hpp"
namespace component::base {

template <typename T>
class RefCountedObject : public T {
public:
    RefCountedObject() {}

    RefCountedObject(const RefCountedObject&) = delete;
    RefCountedObject& operator=(const RefCountedObject&) = delete;

    template <typename U>
    RefCountedObject(U&& obj)
            : T(std::forward<U>(obj)) {
    }

    template <typename P0, typename P1, typename... Args>
    RefCountedObject(P0&& p0, P1&& p1, Args&&... args)
            : T(std::forward<P0>(p0),
                std::forward<P1>(p1),
                std::forward<Args>(args)...) {
    }
    void AddRef() const override {
        ref_count_.fetch_add(1, std::memory_order_relaxed);
    }
    base::RefCountReleaseStatus Release() const override {
        auto count = ref_count_.fetch_sub(1, std::memory_order_acq_rel);
        if (count == 1) {
            delete this;
        }
        return count == 1 ? RefCountReleaseStatus::kDroppedLastRef : RefCountReleaseStatus::kOtherRefsRemained;
    }

    virtual bool HasOneRef() const {
        return ref_count_.load(std::memory_order_acquire) == 1;
    }

protected:
    mutable std::atomic<std::size_t> ref_count_{0};
};

} // namespace component::base