#pragma once

#include "nodes.h"

class Visitor {
public:
    virtual void Visit(const NTop &top) = 0;
    virtual void Visit(const NInt &num) = 0;
    virtual void Visit(const NBool &bol) = 0;
    virtual void Visit(const NFun &fun) = 0;
    virtual void Visit(const NUnit &unit) = 0;
    virtual void Visit(const NProduct &prod) = 0;
    virtual void Visit(const NIdent &ident) = 0;
    virtual void Visit(const NLet &let) = 0;
    virtual void Visit(const NLetRec &let) = 0;
    virtual void Visit(const NLetDef &let) = 0;
    virtual void Visit(const NLetRecDef &let) = 0;
    virtual void Visit(const NSeq &seq) = 0;
    virtual void Visit(const NApp &app) = 0;
    virtual void Visit(const NIf &ifn) = 0;
    virtual void Visit(const NOpC &opc) = 0;
    virtual void Visit(const NHandler &handler) = 0;
    virtual void Visit(const NWithHandle &with) = 0;
    virtual void Visit(const NAdd &add) = 0;
    virtual void Visit(const NSub &sub) = 0;
    virtual void Visit(const NMul &mul) = 0;
    virtual void Visit(const NDiv &div) = 0;
    virtual void Visit(const NLess &less) = 0;
    virtual void Visit(const NGreat &grt) = 0;
    virtual void Visit(const NEqual &eq) = 0;
};

