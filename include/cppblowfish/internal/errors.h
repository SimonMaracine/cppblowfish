#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdexcept>

namespace cppblowfish {
    class AlreadyInitializedError : public std::logic_error {
    public:
        AlreadyInitializedError(const std::string& message)
            : std::logic_error(message) {}
        AlreadyInitializedError(const char* message)
            : std::logic_error(message) {}
    };

    class AllocationError : public std::logic_error {
    public:
        AllocationError(const std::string& message)
            : std::logic_error(message) {}
        AllocationError(const char* message)
            : std::logic_error(message) {}
    };

    class KeyError : public std::logic_error {
    public:
        KeyError(const std::string& message)
            : std::logic_error(message) {}
        KeyError(const char* message)
            : std::logic_error(message) {}
    };

    class PlatformError : public std::runtime_error {
    public:
        PlatformError(const std::string& message)
            : std::runtime_error(message) {}
        PlatformError(const char* message)
            : std::runtime_error(message) {}
    };
}

#endif  // _ERRORS_H_
