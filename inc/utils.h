#pragma once

#include <exception>
#include <memory>

class CastFailureException : public std::exception {
    char *what() {
        return (char *)"CastFailureException";
    }
};

class UnsupportedOperatorException : public std::exception {
    char *what() {
        return (char *)"UnsupportedOperatorException";
    }
};

class VariableNotFoundException : public std::exception {
    char *what() {
        return (char *)"VariableNotFoundException";
    }
};

class AmbiguousHandlerException : public std::exception {
    char *what() {
        return (char *)"AmbiguousHandlerException";
    }
};

template <typename T, typename U>
std::shared_ptr<T> Cast(std::shared_ptr<U> before) {
    auto after = std::dynamic_pointer_cast<T>(before);
    if (after == nullptr) {
        throw CastFailureException();
    }
    return after;
}
template <typename T, typename U>
std::shared_ptr<T> TryCast(std::shared_ptr<U> before) {
    auto after = std::dynamic_pointer_cast<T>(before);
    return after;
}
