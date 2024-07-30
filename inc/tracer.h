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
    Tracer(std::ostream &os);

    void Visit(const NTop &top) override;
    void Visit(const NInt &num) override;
    void Visit(const NBool &bol) override;
    void Visit(const NFun &fun) override;
    void Visit(const NUnit &unit) override;
    void Visit(const NProduct &prod) override;
    void Visit(const NIdent &ident) override;
    void Visit(const NLet &let) override;
    void Visit(const NLetRec &let) override;
    void Visit(const NLetDef &let) override;
    void Visit(const NLetRecDef &let) override;
    void Visit(const NSeq &seq) override;
    void Visit(const NApp &app) override;
    void Visit(const NIf &ifn) override;
    void Visit(const NOpC &opc) override;
    void Visit(const NHandler &handler) override;
    void Visit(const NWith &with) override;
    void Visit(const NAdd &add) override;
    void Visit(const NSub &sub) override;
    void Visit(const NMul &mul) override;
    void Visit(const NDiv &div) override;
    void Visit(const NLess &less) override;
    void Visit(const NGreat &grt) override;
    void Visit(const NEqual &eq) override;

private:
    using PrimPtr = std::shared_ptr<Prim>;

    int tabs_;
    std::ostream &os_;

    PrimPtr ret_;
    std::shared_ptr<Env> env_;

    void ProcessBinary(const NBinary &bin,
        std::function<PrimPtr(PrimPtr, PrimPtr)> app);
    void In();
    void Out();
    void Newline();
    void PrintTabs();

    void RegisterPrim(std::string name, PPrimFun::FunType fun);
};
