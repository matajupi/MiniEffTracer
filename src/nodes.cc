#include "nodes.h"

#include <iostream>

#include "visitor.h"
#include "utils.h"

#define ACCEPT_IMPL(cls) void cls::Accept(Visitor &visitor) { visitor.Visit(this); }
#define DUMP_HEADER(cls) void cls::Dump(std::ostream &os) const


DUMP_HEADER(NTop) {
    dec_->Dump(os);
    std::cout << std::endl;
}
ACCEPT_IMPL(NTop)

DUMP_HEADER(NInt) {
    os << value_;
}
ACCEPT_IMPL(NInt)

DUMP_HEADER(NBool) {
    os << value_;
}
ACCEPT_IMPL(NBool)

DUMP_HEADER(NFun) {
    os << "(" << "fun " << var_ << " -> ";
    body_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NFun)

DUMP_HEADER(NUnit) {
    os << "()";
}
ACCEPT_IMPL(NUnit)

DUMP_HEADER(NPair) {
    os << "(";
    expr1_->Dump(os);
    os << ", ";
    expr2_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NPair)

DUMP_HEADER(NIdent) {
    os << str_;
}
ACCEPT_IMPL(NIdent)

DUMP_HEADER(NLet) {
    os << "(" << "let " << var_ << " = ";
    bexpr_->Dump(os);
    if (!IsDec()) {
        os << " in ";
        cexpr_->Dump(os);
    }
    os << ")";
}
ACCEPT_IMPL(NLet)

DUMP_HEADER(NLetRec) {
    os << "(" << "let rec " << var_ << " = ";
    bexpr_->Dump(os);
    if (!IsDec()) {
        os << " in ";
        cexpr_->Dump(os);
    }
    os << ")";
}
ACCEPT_IMPL(NLetRec)

DUMP_HEADER(NSeq) {
    expr1_->Dump(os);
    switch (sk_) {
        case SeqKind::Dec: {
            os << " ;;" << std::endl;
            break;
        }
        case SeqKind::Expr: {
            os << "; ";
            break;
        }
        case SeqKind::OpCase: {
            os << " | ";
        }
    }
    expr2_->Dump(os);
}
ACCEPT_IMPL(NSeq)

DUMP_HEADER(NApp) {
    os << "(";
    fun_->Dump(os);
    os << " ";
    arg_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NApp)

DUMP_HEADER(NBinaryApp) {
    os << "(";
    left_->Dump(os);
    os << " " << opsym_ << " ";
    right_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NBinaryApp)

DUMP_HEADER(NUnaryApp) {
    os << opsym_;
    expr_->Dump(os);
}
ACCEPT_IMPL(NUnaryApp)

DUMP_HEADER(NCond) {
    os << "(" << "if ";
    cond_->Dump(os);
    os << " then ";
    conseq_->Dump(os);
    os << " else ";
    alter_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NCond)

DUMP_HEADER(NHandler) {
    os << "(" << "handler ";
    opcs_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NHandler)

DUMP_HEADER(NOpCase) {
    if (IsReturnCase()) {
        os << var_ << " -> ";
    }
    else {
        os << opname_ << " " << var_ << " " << cont_ << " -> ";
    }
    body_->Dump(os);
}
ACCEPT_IMPL(NOpCase)

DUMP_HEADER(NWithHandle) {
    os << "(" << "with ";
    handler_->Dump(os);
    os << " handle ";
    cexpr_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NWithHandle)

