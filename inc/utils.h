#pragma once

#include <exception>
#include <tuple>

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
class UnknownOperatorException : public std::exception {
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
T *Cast(U *before) {
    auto after = dynamic_cast<T *>(before);
    if (after == nullptr) {
        throw CastFailureException();
    }
    return after;
}
template <typename T1, typename T2, typename U>
std::tuple<T1 *, T2 *> Cast(U *before) {
    auto after1 = dynamic_cast<T1 *>(before);
    auto after2 = dynamic_cast<T2 *>(before);
    if (after1 == nullptr && after2 == nullptr) {
        throw CastFailureException();
    }
    return { after1, after2 };
}
template <typename T1, typename T2, typename T3, typename U>
std::tuple<T1 *, T2 *, T3 *> Cast(U *before) {
    auto after1 = dynamic_cast<T1 *>(before);
    auto after2 = dynamic_cast<T2 *>(before);
    auto after3 = dynamic_cast<T3 *>(before);
    if (after1 == nullptr && after2 == nullptr && after3 == nullptr) {
        throw CastFailureException();
    }
    return { after1, after2, after3 };
}
template <typename T, typename U>
T *TryCast(U *before) {
    auto after = dynamic_cast<T *>(before);
    return after;
}

