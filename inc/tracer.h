#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <map>
#include <list>

#include "visitor.h"
#include "prims.h"
#include "env.h"

class Tracer : public Visitor {
public:
    Tracer(std::ostream &os)
        : tabs_(0), os_(os), env_(std::make_shared<Env>(nullptr)) { }

    void Visit(const Top &top) override;
    void Visit(const Prog &prog) override;
    void Visit(const Empty &empty) override;
    void Visit(const NInt &num) override;
    void Visit(const NBool &bol) override;
    void Visit(const NFun &fun) override;
    void Visit(const NUnit &unit) override;
    void Visit(const Ident &ident) override;
    void Visit(const Let &let) override;
    void Visit(const LetRec &let) override;
    void Visit(const Seq &seq) override;
    void Visit(const App &app) override;
    void Visit(const If &ifn) override;
    void Visit(const Add &add) override;
    void Visit(const Sub &sub) override;
    void Visit(const Mul &mul) override;
    void Visit(const Div &div) override;
    void Visit(const Less &less) override;
    void Visit(const Great &grt) override;
    void Visit(const Equal &eq) override;

private:
    using PrimPtr = std::shared_ptr<Prim>;

    int tabs_;
    std::ostream &os_;

    PrimPtr ret_;
    std::shared_ptr<Env> env_;

    void ProcessBinary(const Binary &bin,
        std::function<PrimPtr(PrimPtr, PrimPtr)> app);
    void In();
    void Out();
    void Newline();
    void PrintTabs();
};
