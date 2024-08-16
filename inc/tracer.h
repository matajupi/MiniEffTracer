#pragma once

#include <iostream>
#include <functional>

#include "visitor.h"
#include "values.h"
#include "env.h"

class Tracer : public Visitor {
public:
    Tracer(std::ostream &os);

    VISIT_DEC(NTop) override;
    VISIT_DEC(NInt) override;
    VISIT_DEC(NBool) override;
    VISIT_DEC(NFun) override;
    VISIT_DEC(NUnit) override;
    VISIT_DEC(NPair) override;
    VISIT_DEC(NIdent) override;
    VISIT_DEC(NLet) override;
    VISIT_DEC(NLetRec) override;
    VISIT_DEC(NSeq) override;
    VISIT_DEC(NApp) override;
    VISIT_DEC(NBinaryApp) override;
    VISIT_DEC(NUnaryApp) override;
    VISIT_DEC(NCond) override;
    VISIT_DEC(NHandler) override;
    VISIT_DEC(NOpCase) override;
    VISIT_DEC(NWithHandle) override;

private:
    std::ostream &os_;
    int tabs_;

    Value *ret_;
    Env *env_;

    void In();
    void Out();
    void Newline();
    void ArrowLine();
    void PrintTabs();
    void RegisterPrimFun(std::string name, VPrimFun::FunType fun);
};
