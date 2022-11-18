#include "base/aligned_malloc.hpp"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace component::base{

uintptr_t GetRightAlign(uintptr_t start_pos,size_t alignment){
    return (start_pos + alignment - 1) & ~(alignment - 1);
}

// Alignment must be an integer power of two.
bool ValidAlignment(size_t alignment){
    if(alignment == 0){
        return false;
    }

    return (alignment & (alignment - 1)) == 0;
}

void* AlignedMalloc(size_t size,size_t alignment){
    if(size == 0 || !ValidAlignment(alignment)){
       return NULL;
    }

    void* memory_pointer = malloc(size + sizeof(uintptr_t) + alignment - 1);
    assert(memory_pointer != NULL);
    uintptr_t align_start_pos = reinterpret_cast<uintptr_t>(memory_pointer);
    align_start_pos += sizeof(uintptr_t);
    uintptr_t aligned_pos = GetRightAlign(align_start_pos,alignment);
    void *aligned_pointer = reinterpret_cast<void*>(aligned_pos);

    uintptr_t header_pos = aligned_pos - sizeof(uintptr_t);
    void *header_pointer = reinterpret_cast<void*>(header_pos);
    uintptr_t memory_start = reinterpret_cast<uintptr_t>(memory_pointer);
    memcpy(header_pointer,&memory_start,sizeof(uintptr_t));

    return aligned_pointer;
}

void* GetRightAlign(const void* ptr,size_t alignment){
    if(!ptr || !ValidAlignment(alignment)){
        return NULL;
    }

    uintptr_t start_pos = reinterpret_cast<uintptr_t>(ptr);
    return reinterpret_cast<void*>(GetRightAlign(start_pos,alignment));
}

void AlignedFree(void *mem_block){
    if(!mem_block){
        return;
    }

    uintptr_t aligned_pos = reinterpret_cast<uintptr_t>(mem_block);
    uintptr_t header_pos  = aligned_pos - sizeof(uintptr_t);

    uintptr_t memory_start_pos = *reinterpret_cast<uintptr_t*>(header_pos);
    void *memory_start = reinterpret_cast<void*>(memory_start_pos);
    free(memory_start);
}

}
