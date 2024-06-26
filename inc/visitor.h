#pragma once

#include "nodes.h"

class Visitor {
public:
    virtual void Visit(const Top &top) = 0;
    virtual void Visit(const Prog &prog) = 0;
    virtual void Visit(const Empty &empty) = 0;
    virtual void Visit(const NInt &num) = 0;
    virtual void Visit(const NBool &bol) = 0;
    virtual void Visit(const NFun &fun) = 0;
    virtual void Visit(const Ident &ident) = 0;
    virtual void Visit(const Let1 &let) = 0;
    virtual void Visit(const App &app) = 0;
    virtual void Visit(const Add &add) = 0;
    virtual void Visit(const Sub &sub) = 0;
    virtual void Visit(const Mul &mul) = 0;
    virtual void Visit(const Div &div) = 0;
    virtual void Visit(const Less &less) = 0;
    virtual void Visit(const Great &grt) = 0;
    virtual void Visit(const Equal &eq) = 0;
};

