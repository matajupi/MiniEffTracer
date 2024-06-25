#pragma once

#include <iostream>
#include <functional>
#include <memory>

#include "visitor.h"
#include "prim.h"

class Tracer : public Visitor {
public:
    Tracer(std::ostream &os) : tabs_(0), os_(os) { }

    void Visit(const Top &top) override;
    void Visit(const Prog &prog) override;
    void Visit(const Empty &empty) override;
    void Visit(const NInt &num) override;
    void Visit(const NBool &bol) override;
    void Visit(const Add &add) override;
    void Visit(const Sub &sub) override;
    void Visit(const Mul &mul) override;
    void Visit(const Div &div) override;
    void Visit(const Less &less) override;
    void Visit(const Great &grt) override;
    void Visit(const Equal &eq) override;

private:
    using PrimPtr = std::shared_ptr<Prim>;

    PrimPtr ret_;
    int tabs_;
    std::ostream &os_;
    void ProcessBinary(const Binary &bin,
        std::function<PrimPtr(PrimPtr, PrimPtr)> app);
    void PrintTabs();
};
