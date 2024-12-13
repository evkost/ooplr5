#include <iostream>
#include "CustomMemoryResource.h"
#include "Stack.h"

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

int main() {
    CustomMemoryResource custom_resource;
    std::pmr::polymorphic_allocator<int> alloc_int(&custom_resource);
    std::pmr::polymorphic_allocator<Person> alloc_person(&custom_resource);

    Stack int_stack(alloc_int);
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);

    std::cout << "Integer Stack:" << std::endl;
    for (auto it = int_stack.begin(); it != int_stack.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nTop element: " << int_stack.top() << std::endl;

    Stack person_stack(alloc_person);
    person_stack.push(Person{"Alice", 30});
    person_stack.push(Person{"Bob", 25});
    person_stack.push(Person{"Charlie", 35});

    std::cout << "\nPerson Stack:" << std::endl;
    for (auto it = person_stack.begin(); it != person_stack.end(); ++it) {
        std::cout << it->name << ", " << it->age << std::endl;
    }
    std::cout << "Top person: " << person_stack.top().name << ", " << person_stack.top().age << std::endl;

    return 0;
}
