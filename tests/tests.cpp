#include <gtest/gtest.h>
#include "CustomMemoryResource.h"
#include "Stack.h"
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

TEST(StackTest, IntStackOperations) {
    CustomMemoryResource custom_resource;
    std::pmr::polymorphic_allocator<int> alloc_int(&custom_resource);
    Stack stack(alloc_int);

    EXPECT_TRUE(stack.empty());
    stack.push(1);
    stack.push(2);
    stack.push(3);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.top(), 3);

    stack.pop();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 2);
}

TEST(StackTest, PersonStackOperations) {
    CustomMemoryResource custom_resource;
    std::pmr::polymorphic_allocator<Person> alloc_person(&custom_resource);
    Stack stack(alloc_person);

    EXPECT_TRUE(stack.empty());

    Person alice{"Alice", 30};
    Person bob{"Bob", 25};
    Person charlie{"Charlie", 35};

    stack.push(alice);
    stack.push(bob);
    stack.push(charlie);

    EXPECT_EQ(stack.size(), 3);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.top(), charlie);

    stack.pop();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), bob);
}

TEST(StackTest, IteratorTraversal) {
    CustomMemoryResource custom_resource;
    std::pmr::polymorphic_allocator<int> alloc_int(&custom_resource);
    Stack stack(alloc_int);

    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::vector expected = {10, 20, 30};
    std::vector<int> result;

    for(Stack<int>::iterator it = stack.begin(); it != stack.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(CustomMemoryResourceTest, MemoryReuse) {
    CustomMemoryResource custom_resource;
    std::pmr::polymorphic_allocator<int> alloc_int(&custom_resource);

    int* a = alloc_int.allocate(1);
    int* b = alloc_int.allocate(1);
    int* c = alloc_int.allocate(1);

    alloc_int.deallocate(b, 1);
    alloc_int.deallocate(c, 1);

    int* d = alloc_int.allocate(1);

    EXPECT_EQ(d, c);

    alloc_int.deallocate(a, 1);
    alloc_int.deallocate(d, 1);
}

TEST(CustomMemoryResourceTest, CleanupOnDestruction) {
    class TrackingMemoryResource : public CustomMemoryResource {
    public:
        ~TrackingMemoryResource() override = default;

        std::size_t get_allocated_blocks() const {
            std::lock_guard lock(mutex_);
            return allocated_pointers_.size();
        }
    };

    std::size_t initial_blocks = 0;
    {
        TrackingMemoryResource custom_resource;
        std::pmr::polymorphic_allocator<int> alloc_int(&custom_resource);

        int* a = alloc_int.allocate(1);
        int* b = alloc_int.allocate(1);

        alloc_int.deallocate(a, 1);

        initial_blocks = custom_resource.get_allocated_blocks();
        EXPECT_EQ(initial_blocks, 2);
    }

    SUCCEED();
}