#include "mu_test.h"

#include "object_pool.hpp"
#include "policy_expand.hpp"
#include "policy_throw.hpp"
#include "policy_back_to_normal.hpp"
#include "token.hpp"
#include "factory_method.hpp"
#include "object_pool_exepction.hpp"

#include <memory>


template <typename ObjectType>
class DummyCreator : public dp::Creator<ObjectType> {
public:
    virtual std::unique_ptr<ObjectType> create() override;
};

template <typename ObjectType>
std::unique_ptr<ObjectType> DummyCreator<ObjectType>::create() 
{
    return std::make_unique<ObjectType>();
}

class DummyObject : public dp::Object {
public:
	DummyObject() = default;
	~DummyObject() = default;
	void use() override {};
};



BEGIN_TEST(regular_usage)

	constexpr size_t N = 3;
	dp::PolicyThrow<DummyObject> ramp_up_policy{};
	dp::PolicyBackToNormal<DummyObject> back_to_normal_policy{N};

	DummyCreator<DummyObject> factory;
	auto obj_pool = dp::ObjectPool<DummyObject, dp::PolicyThrow<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::create_pool(N, ramp_up_policy, back_to_normal_policy, factory);

	ASSERT_EQUAL(obj_pool->size(), N);
	
	auto obj1 = obj_pool->take();
	ASSERT_EQUAL(obj_pool->size(), N-1);
	auto obj2 = obj_pool->take();
	ASSERT_EQUAL(obj_pool->size(), N-2);
	auto obj3 = obj_pool->take();

	ASSERT_THAT(obj1 != obj2 && obj2 != obj3 && obj1 != obj3);
	ASSERT_EQUAL(obj_pool->size(), 0);

END_TEST


BEGIN_TEST(take_more_than_available)

    constexpr size_t N = 2;
	dp::PolicyThrow<DummyObject> ramp_up_policy{};
	dp::PolicyBackToNormal<DummyObject> back_to_normal_policy{N};

	DummyCreator<DummyObject> factory;
	auto obj_pool = dp::ObjectPool<DummyObject, dp::PolicyThrow<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::create_pool(N, ramp_up_policy, back_to_normal_policy, factory);

    ASSERT_EQUAL(obj_pool->size(), N);

    auto obj1 = obj_pool->take();
    auto obj2 = obj_pool->take();
    ASSERT_EQUAL(obj_pool->size(), 0);

    bool check = false;
    try {
        auto obj3 = obj_pool->take();
    } catch (dp::ObjectPoolException const& e) {
        check = true;
    }

    ASSERT_THAT(check);

END_TEST


BEGIN_TEST(regular_expansion)

	constexpr size_t N = 3;
	dp::PolicyExpand<DummyObject> ramp_up_policy{1};
	dp::PolicyBackToNormal<DummyObject> back_to_normal_policy{N};

	DummyCreator<DummyObject> factory;
	auto obj_pool = dp::ObjectPool<DummyObject, dp::PolicyExpand<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::create_pool(N, ramp_up_policy, back_to_normal_policy, factory);

	ASSERT_EQUAL(obj_pool->size(), N);

	{
		auto obj1 = obj_pool->take();
		auto obj2 = obj_pool->take();
		ASSERT_EQUAL(obj_pool->size(), N-2);
		ASSERT_THAT(obj1 != obj2);

		auto obj3 = obj_pool->take();
		auto obj4 = obj_pool->take();
	}

	ASSERT_EQUAL(obj_pool->size(), N);

END_TEST


BEGIN_TEST(multiple_expansions)

    constexpr size_t N = 2;
	dp::PolicyExpand<DummyObject> ramp_up_policy{N};
	dp::PolicyBackToNormal<DummyObject> back_to_normal_policy{N};

	DummyCreator<DummyObject> factory;
	auto obj_pool = dp::ObjectPool<DummyObject, dp::PolicyExpand<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::create_pool(N, ramp_up_policy, back_to_normal_policy, factory);

    ASSERT_EQUAL(obj_pool->size(), N);

    auto obj1 = obj_pool->take();
    auto obj2 = obj_pool->take();
    auto obj3 = obj_pool->take();
    ASSERT_EQUAL(obj_pool->size(), 1);

    auto obj4 = obj_pool->take();
    auto obj5 = obj_pool->take();
    ASSERT_EQUAL(obj_pool->size(), 1); // still 1 left

END_TEST


BEGIN_TEST(out_of_scope_pool)

	dp::ObjectPool<DummyObject, dp::PolicyExpand<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::CustomDeleterUptr dummy_uptr;
	{
		constexpr size_t N = 1;
		dp::PolicyExpand<DummyObject> ramp_up_policy{N};
		dp::PolicyBackToNormal<DummyObject> back_to_normal_policy{N};

		DummyCreator<DummyObject> factory;
		auto obj_pool = dp::ObjectPool<DummyObject, dp::PolicyExpand<DummyObject>, dp::PolicyBackToNormal<DummyObject>>::create_pool(N, ramp_up_policy, back_to_normal_policy, factory);

		ASSERT_EQUAL(obj_pool->size(), N);
		dummy_uptr = std::move(obj_pool->take());
		ASSERT_EQUAL(obj_pool->size(), N-1);
	}

	ASSERT_THAT(dummy_uptr);

	dummy_uptr.reset();
	ASSERT_THAT(!dummy_uptr);


END_TEST



TEST_SUITE(Object Pool Tests)

	TEST(regular_usage)
	TEST(take_more_than_available)
	TEST(regular_expansion)
	TEST(multiple_expansions)
	TEST(out_of_scope_pool)	

END_SUITE
