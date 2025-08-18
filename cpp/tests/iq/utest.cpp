#include "mu_test.h"
#include "containers.hpp"
#include <memory>


struct Person{
    int id;
    int age;

    Person(int id, int age)
    : id{id}
    , age{age}
    {}

    bool operator==(Person const& other){
        return id == other.id && age == other.age;
    }

    bool operator<(Person const& other){
        return id < other.id;
    }
};

BEGIN_TEST(min_stack_reg_ints)

    iq::MinStack<int> stack{};

    int value = 5;
    
    stack.push(std::make_shared<int>(value));

    ASSERT_EQUAL(*stack.min(), value);

END_TEST

BEGIN_TEST(min_stack_cmp_ints)

    auto cmp = [](int a, int b){ return a < b; };

    iq::MinStack<int> stack{cmp};

    int value = 20;
    
    stack.push(std::make_shared<int>(value));

    ASSERT_EQUAL(*stack.min(), value);

END_TEST

BEGIN_TEST(min_stack_cmp_persons)

    auto cmp_by_id = [](Person const& a, Person const& b){ return a.id < b.id; };
    auto cmp_by_age = [](Person const& a, Person const& b){ return a.age < b.age; };

    iq::MinStack<Person> id_stack{cmp_by_id};
    iq::MinStack<Person> age_stack{cmp_by_age};

    Person a{123456, 30};
    Person b{223344, 20};
    
    id_stack.push(std::make_shared<Person>(a));
    id_stack.push(std::make_shared<Person>(b));

    ASSERT_THAT(*id_stack.min() == a);

    age_stack.push(std::make_shared<Person>(a));
    age_stack.push(std::make_shared<Person>(b));

    ASSERT_THAT(*age_stack.min() == b);

END_TEST


BEGIN_TEST(resettable_slots_basic_usage)

    iq::ResettableSlots<int> slots(3, std::make_shared<int>(0));

    ASSERT_EQUAL(*slots.get(0), 0);
    ASSERT_EQUAL(*slots.get(1), 0);
    ASSERT_EQUAL(*slots.get(2), 0);

    slots.set(0, std::make_shared<int>(10));
    slots.set(1, std::make_shared<int>(20));

    ASSERT_EQUAL(*slots.get(0), 10);
    ASSERT_EQUAL(*slots.get(1), 20);
    ASSERT_EQUAL(*slots.get(2), 0); 
END_TEST

BEGIN_TEST(resettable_slots_reset)
    iq::ResettableSlots<int> slots(3, std::make_shared<int>(1));

    slots.set(0, std::make_shared<int>(10));
    slots.set(1, std::make_shared<int>(20));
    slots.set(2, std::make_shared<int>(30));

    auto resetVal = std::make_shared<int>(99);
    slots.reset(resetVal);

    ASSERT_EQUAL(*slots.get(0), 99);
    ASSERT_EQUAL(*slots.get(1), 99);
    ASSERT_EQUAL(*slots.get(2), 99);

    slots.set(1, std::make_shared<int>(777));
    ASSERT_EQUAL(*slots.get(0), 99);
    ASSERT_EQUAL(*slots.get(1), 777); 
    ASSERT_EQUAL(*slots.get(2), 99);
END_TEST

BEGIN_TEST(resettable_slots_out_of_bounds)
    iq::ResettableSlots<int> slots(2, std::make_shared<int>(0));

    bool exception_thrown = false;
    try {
        slots.get(10); // out of range
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    ASSERT_THAT(exception_thrown);

    exception_thrown = false;
    try {
        slots.set(10, std::make_shared<int>(5)); // out of range
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }
    ASSERT_THAT(exception_thrown);
END_TEST

BEGIN_TEST(resettable_slots_nullptr)
    iq::ResettableSlots<int> slots(2, std::make_shared<int>(100));

    bool exception_thrown = false;
    try {
        std::shared_ptr<int> nullPtr;
        slots.set(0, nullPtr); // Should throw
    } catch(const std::runtime_error&) {
        exception_thrown = true;
    }
    ASSERT_THAT(exception_thrown);


    exception_thrown = false;
    try {
        std::shared_ptr<int> nullPtr;
        slots.reset(nullPtr);
    } catch(const std::runtime_error&) {
        exception_thrown = true;
    }
    ASSERT_THAT(exception_thrown);
END_TEST

BEGIN_TEST(resettable_slots_multiple_operations)

    iq::ResettableSlots<int> slots(3, std::make_shared<int>(-1));

    ASSERT_EQUAL(*slots.get(0), -1);
    ASSERT_EQUAL(*slots.get(1), -1);
    ASSERT_EQUAL(*slots.get(2), -1);

    slots.set(0, std::make_shared<int>(10));
    slots.set(2, std::make_shared<int>(30));
    ASSERT_EQUAL(*slots.get(0), 10);
    ASSERT_EQUAL(*slots.get(1), -1);
    ASSERT_EQUAL(*slots.get(2), 30);

    slots.reset(std::make_shared<int>(999));
    ASSERT_EQUAL(*slots.get(0), 999);
    ASSERT_EQUAL(*slots.get(1), 999);
    ASSERT_EQUAL(*slots.get(2), 999);

    slots.set(1, std::make_shared<int>(555));
    ASSERT_EQUAL(*slots.get(1), 555);
    ASSERT_EQUAL(*slots.get(0), 999);
END_TEST

TEST_SUITE(containers tests)
    TEST(min_stack_reg_ints)
    TEST(min_stack_cmp_ints)
    TEST(min_stack_cmp_persons)


    TEST(resettable_slots_basic_usage)
    TEST(resettable_slots_reset)
    TEST(resettable_slots_out_of_bounds)
    TEST(resettable_slots_nullptr)
    TEST(resettable_slots_multiple_operations)

END_SUITE
