#pragma once
#include <stddef.h>
namespace component::base{

void* GetRightAlign(const void* ptr,size_t alignment);

void* AlignedMalloc(size_t size,size_t alignment);

void AlignedFree(void *mem_block);

template<typename T>
T* GetRightAlign(const T *ptr,size_t alignment){
    return reinterpret_cast<T*>(GetRightAlign(reinterpret_cast<const void*>(ptr),alignment));
}

template<typename T>
T* AlignedMalloc(size_t size,size_t alignment){
    return reinterpret_cast<T*>(AlignedMalloc(size,alignment));
}

struct AlignedFreeDeleter{
    void operator()(void* ptr) const{
         AlignedFree(ptr);
    }
};

}