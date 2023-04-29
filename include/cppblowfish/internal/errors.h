#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdexcept>

namespace cppblowfish {
    class AlreadyInitializedError : public std::logic_error {
    public:
        explicit AlreadyInitializedError(const std::string& message)
            : std::logic_error(message) {}
        explicit AlreadyInitializedError(const char* message)
            : std::logic_error(message) {}
    };

    class AllocationError : public std::logic_error {
    public:
        explicit AllocationError(const std::string& message)
            : std::logic_error(message) {}
        explicit AllocationError(const char* message)
            : std::logic_error(message) {}
    };

    class KeyError : public std::logic_error {
    public:
        explicit KeyError(const std::string& message)
            : std::logic_error(message) {}
        explicit KeyError(const char* message)
            : std::logic_error(message) {}
    };

    class PlatformError : public std::runtime_error {
    public:
        explicit PlatformError(const std::string& message)
            : std::runtime_error(message) {}
        explicit PlatformError(const char* message)
            : std::runtime_error(message) {}
    };
}

#endif  // _ERRORS_H_
