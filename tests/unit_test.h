#pragma once

#include <vector>
#include <iostream>

class _UnitTestContext {
public:
    _UnitTestContext() = default;

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
                << test_results.size() << std::endl;
    }
private:
    std::vector<bool> test_results;
};

#define INITIALIZE_UNIT_TEST() \
    _UnitTestContext* _context = new _UnitTestContext;

#define DEFINE_TEST(name) \
    void name(_UnitTestContext* _context) {

#define END_DEFINE_TEST() \
    }

#define TEST(name) \
    _context->add_test(); \
    std::cout << "TESTING " << #name << std::endl; \
    name(_context); \
    if (_context->last_test_was_ok()) { \
        std::cout << "OK\n" << std::endl; \
    } else { \
        std::cout << "FAIL\n" << std::endl; \
    }

#define END_UNIT_TEST() \
    _context->print_results(); \
    delete _context;

#define ASSERT(expression) \
    if (!(expression)) { \
        _context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression << "'" \
                << " failed; " << "line " << __LINE__ << std::endl; \
        return; \
    }

#define ASSERT_EQ(expression1, expression2) \
    if (!((expression1) == (expression2))) { \
        _context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression1 << " == " << #expression2 << "'" \
                << " failed; " << "line " << __LINE__ << std::endl; \
        return; \
    }

#define ASSERT_NEQ(expression1, expression2) \
    if (!((expression1) != (expression2))) { \
        _context->fail_this_test(); \
        std::cout << "Assertion " << "'" << #expression1 << " != " << #expression2 << "'" \
                << " failed; " << "line " << __LINE__ << std::endl; \
        return; \
    }
