#ifndef CUSTOM_MEMORY_RESOURCE_H
#define CUSTOM_MEMORY_RESOURCE_H

#include <memory_resource>
#include <map>
#include <vector>
#include <mutex>

class CustomMemoryResource : public std::pmr::memory_resource {
public:
    CustomMemoryResource();
    ~CustomMemoryResource();

protected:
    virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override;
    virtual bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

    std::map<std::size_t, std::vector<void*>> free_lists_;
    std::vector<void*> allocated_pointers_;
    mutable std::mutex mutex_;
};

#endif // CUSTOM_MEMORY_RESOURCE_H
