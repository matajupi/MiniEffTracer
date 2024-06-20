#pragma once

#include <iostream>
#include <functional>
#include "visitor.h"

class Tracer : public Visitor {
public:
    Tracer(std::ostream &os) : tabs_(0), os_(os) { }

    void Visit(const Top &top) override;
    void Visit(const Number &num) override;
    void Visit(const Add &add) override;
    void Visit(const Sub &sub) override;
    void Visit(const Mul &mul) override;
    void Visit(const Div &div) override;

private:
    int ret_;
    int tabs_;
    std::ostream &os_;
    void ProcessBinary(const Binary &bin, std::function<int(int, int)> app);
    void PrintTabs();
};
