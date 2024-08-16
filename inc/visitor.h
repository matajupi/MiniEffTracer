#pragma once

#include "nodes.h"

#define VISIT_DEC(cls) void Visit(cls *node)

class Visitor {
public:
    virtual VISIT_DEC(NTop) = 0;
    virtual VISIT_DEC(NInt) = 0;
    virtual VISIT_DEC(NBool) = 0;
    virtual VISIT_DEC(NFun) = 0;
    virtual VISIT_DEC(NUnit) = 0;
    virtual VISIT_DEC(NPair) = 0;
    virtual VISIT_DEC(NIdent) = 0;
    virtual VISIT_DEC(NLet) = 0;
    virtual VISIT_DEC(NLetRec) = 0;
    virtual VISIT_DEC(NSeq) = 0;
    virtual VISIT_DEC(NApp) = 0;
    virtual VISIT_DEC(NBinaryApp) = 0;
    virtual VISIT_DEC(NUnaryApp) = 0;
    virtual VISIT_DEC(NCond) = 0;
    virtual VISIT_DEC(NHandler) = 0;
    virtual VISIT_DEC(NOpCase) = 0;
    virtual VISIT_DEC(NWithHandle) = 0;
};

