#pragma once

#include "nodes.h"

class Visitor {
public:
    virtual void Visit(const Top &top) = 0;
    virtual void Visit(const Number &num) = 0;
    virtual void Visit(const Add &add) = 0;
    virtual void Visit(const Sub &sub) = 0;
    virtual void Visit(const Mul &mul) = 0;
    virtual void Visit(const Div &div) = 0;
};

