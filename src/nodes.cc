#include "nodes.h"

#include <iostream>

#include "visitor.h"
#include "utils.h"

#define ACCEPT_IMPL(cls) void cls::Accept(Visitor &visitor) { visitor.Visit(this); }


void NTop::Dump(std::ostream &os) const {
    dec_->Dump(os);
    std::cout << std::endl;
}
ACCEPT_IMPL(NTop)

void NInt::Dump(std::ostream &os) const {
    os << value_;
}
ACCEPT_IMPL(NInt)

void NBool::Dump(std::ostream &os) const {
    os << value_;
}
ACCEPT_IMPL(NBool)

void NFun::Dump(std::ostream &os) const {
    os << "(" << "fun " << var_ << " -> ";
    body_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NFun)

void NUnit::Dump(std::ostream &os) const {
    os << "()";
}
ACCEPT_IMPL(NUnit)

void NPair::Dump(std::ostream &os) const {
    os << "(";
    expr1_->Dump(os);
    os << ", ";
    expr2_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NPair)

void NIdent::Dump(std::ostream &os) const {
    os << str_;
}
ACCEPT_IMPL(NIdent)

void NLet::Dump(std::ostream &os) const {
    os << "(" << "let " << var_ << " = ";
    bexpr_->Dump(os);
    if (!IsDec()) {
        os << " in ";
        cexpr_->Dump(os);
    }
    os << ")";
}
ACCEPT_IMPL(NLet)

void NLetRec::Dump(std::ostream &os) const {
    os << "(" << "let rec " << var_ << " = ";
    bexpr_->Dump(os);
    if (!IsDec()) {
        os << " in ";
        cexpr_->Dump(os);
    }
    os << ")";
}
ACCEPT_IMPL(NLetRec)

void NSeq::Dump(std::ostream &os) const {
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

void NApp::Dump(std::ostream &os) const {
    os << "(";
    if (IsPrimApp()) {
        os << "prim";
    }
    else {
        fun_->Dump(os);
    }
    os << " ";
    arg_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NApp)

void NCond::Dump(std::ostream &os) const {
    os << "(" << "if ";
    cond_->Dump(os);
    os << " then ";
    conseq_->Dump(os);
    os << " else ";
    alter_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NCond)

void NHandler::Dump(std::ostream &os) const {
    os << "(" << "handler ";
    opcs_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NHandler)

void NOpCase::Dump(std::ostream &os) const {
    if (IsReturnCase()) {
        os << var_ << " -> ";
    }
    else {
        os << opname_ << " " << var_ << " " << cont_ << " -> ";
    }
    body_->Dump(os);
}
ACCEPT_IMPL(NOpCase)

void NWithHandle::Dump(std::ostream &os) const {
    os << "(" << "with ";
    handler_->Dump(os);
    os << " handle ";
    cexpr_->Dump(os);
    os << ")";
}
ACCEPT_IMPL(NWithHandle)

