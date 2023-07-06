#pragma once

#include <vector>
#include <iostream>

class UnitTestContext {
public:
    UnitTestContext() = default;

    void add_test() {
        test_results.push_back(true);
    }

    bool last_test_was_ok() {
        return test_results.back();
    }

    void fail_this_test() {
        test_results[test_results.size() - 1] = false;
    }

    void print_results() {
        unsigned int passed = 0;
        unsigned int failed = 0;

        for (bool result : test_results) {
            if (result) {
                passed++;
            } else {
                failed++;
            }
        }

        std::cout << passed << " test(s) passed, " << failed << " test(s) failed out of "
                << test_results.size() << '\n';
    }
private:
    std::vector<bool> test_results;
};

#define INITIALIZE_UNIT_TEST() \
    UnitTestContext* unit_test_context = new UnitTestContext;

#define DEFINE_TEST(name) \
    void name(UnitTestContext* unit_test_context)

#define TEST(name) \
    unit_test_context->add_test(); \
    std::cout << "TESTING " << #name << '\n'; \
    name(unit_test_context); \
    if (unit_test_context->last_test_was_ok()) { \
        std::cout << "OK\n" << '\n'; \
    } else { \
        std::cout << "FAIL\n" << '\n'; \
    }

#define END_UNIT_TEST() \
    unit_test_context->print_results(); \
    delete unit_test_context;

#define ASSERT(expression) \
    if (!(expression)) { \
        unit_test_context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression << "'" \
                << " failed; " << "line " << __LINE__ << '\n'; \
        return; \
    }

#define ASSERT_EQ(expression1, expression2) \
    if (!((expression1) == (expression2))) { \
        unit_test_context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression1 << " == " << #expression2 << "'" \
                << " failed; " << "line " << __LINE__ << '\n'; \
        return; \
    }

#define ASSERT_NEQ(expression1, expression2) \
    if (!((expression1) != (expression2))) { \
        unit_test_context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression1 << " != " << #expression2 << "'" \
                << " failed; " << "line " << __LINE__ << '\n'; \
        return; \
    }
