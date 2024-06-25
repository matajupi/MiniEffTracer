#pragma once

#include <exception>

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

