#include "CustomMemoryResource.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <malloc.h>
#endif

CustomMemoryResource::CustomMemoryResource() = default;

CustomMemoryResource::~CustomMemoryResource() {
    std::lock_guard lock(mutex_);
    for (void* ptr : allocated_pointers_) {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        std::free(ptr);
#endif
    }
    allocated_pointers_.clear();
    free_lists_.clear();
}

void* CustomMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    std::lock_guard lock(mutex_);

#ifndef _WIN32
    if (bytes % alignment != 0) {
        bytes += alignment - (bytes % alignment);
    }
#endif

    auto it = free_lists_.find(bytes);
    if (it != free_lists_.end() && !it->second.empty()) {
        void* ptr = it->second.back();
        it->second.pop_back();
        std::cout << "Reusing freed memory at " << ptr << std::endl; // Отладочный вывод
        return ptr;
    }

    void* ptr = nullptr;
#ifdef _WIN32
    ptr = _aligned_malloc(bytes, alignment);
#else
    ptr = std::aligned_alloc(alignment, bytes);
#endif

    if (!ptr) {
        throw std::bad_alloc();
    }

    allocated_pointers_.push_back(ptr);
    return ptr;
}

void CustomMemoryResource::do_deallocate(void* p, std::size_t bytes, std::size_t alignment) {
    std::lock_guard lock(mutex_);
    free_lists_[bytes].push_back(p);
}

bool CustomMemoryResource::do_is_equal(const memory_resource& other) const noexcept {
    return this == &other;
}
