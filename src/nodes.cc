#include "nodes.h"

#include <set>

#include "visitor.h"
#include "utils.h"

void NTop::Dump(std::ostream &os) const {
    for (auto p : *prog_) {
        p->Dump(os);
        os << " ;;" << std::endl;
    }
}
void NTop::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NInt::Dump(std::ostream &os) const {
    os << value_;
}
void NInt::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NBool::Dump(std::ostream &os) const {
    os << value_;
}
void NBool::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NFun::Dump(std::ostream &os) const {
    os << "(" << "fun " << var_ << " -> ";
    body_->Dump(os);
    os << ")";
}
void NFun::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NUnit::Dump(std::ostream &os) const {
    os << "()";
}
void NUnit::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NProduct::Dump(std::ostream &os) const {
    os << "(";
    expr1_->Dump(os);
    os << ", ";
    expr2_->Dump(os);
    os << ")";
}
void NProduct::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NIdent::Dump(std::ostream &os) const {
    os << str_;
}
void NIdent::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NLet::Dump(std::ostream &os) const {
    os << "(" << "let " << var_ << " = ";
    bexpr_->Dump(os);
    os << " in ";
    cexpr_->Dump(os);
    os << ")";
}
void NLet::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NLetRec::Dump(std::ostream &os) const {
    os << "(" << "let rec " << var_ << " = ";
    bexpr_->Dump(os);
    os << " in ";
    cexpr_->Dump(os);
    os << ")";
}
void NLetRec::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NLetDef::Dump(std::ostream &os) const {
    os << "(" << "let " << var_ << " = ";
    bexpr_->Dump(os);
    os << ")";
}
void NLetDef::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NLetRecDef::Dump(std::ostream &os) const {
    os << "(" << "let rec " << var_ << " = ";
    bexpr_->Dump(os);
    os << ")";
}
void NLetRecDef::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NSeq::Dump(std::ostream &os) const {
    os << "(";
    expr1_->Dump(os);
    os << "; ";
    expr2_->Dump(os);
    os << ")";
}
void NSeq::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NApp::Dump(std::ostream &os) const {
    os << "(";
    fun_->Dump(os);
    os << " ";
    arg_->Dump(os);
    os << ")";
}
void NApp::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NIf::Dump(std::ostream &os) const {
    os << "(" << "if ";
    cond_->Dump(os);
    os << " then ";
    if_clause_->Dump(os);
    os << " else ";
    else_clause_->Dump(os);
    os << ")";
}
void NIf::Accept(Visitor &visitor) { visitor.Visit(*this); }

NHandler::NHandler(std::shared_ptr<NOpCList> opcs)
    : retc_(nullptr), effcs_(std::make_shared<NOpCList>()) {
    std::set<std::string> effns;
    for (auto opc : *opcs) {
        if (opc->IsReturn()) {
            if (retc_ != nullptr) {
                throw AmbiguousHandlerException();
            }
            retc_ = opc;
        }
        else {
            if (effns.find(opc->GetEff()) != effns.end()) {
                throw AmbiguousHandlerException();
            }
            effcs_->push_back(opc);
        }
    }
    if (retc_ == nullptr) {
        retc_ = std::make_shared<NOpC>("x", std::make_shared<NIdent>("x"));
    }
}
void NHandler::Dump(std::ostream &os) const {
    os << "(" << "handler ";
    retc_->Dump(os);
    for (auto effc : *effcs_) {
        os << " | ";
        effc->Dump(os);
    }
    os << ")";
}
void NHandler::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NWith::Dump(std::ostream &os) const {
    os << "(" << "with ";
    handler_->Dump(os);
    os << " handle ";
    body_->Dump(os);
    os << ")";
}
void NWith::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NOpC::Dump(std::ostream &os) const {
    if (IsReturn()) {
        os << var_ << " -> ";
    }
    else {
        os << eff_ << " " << var_ << " " << cont_ << " -> ";
    }
    os << "(";
    body_->Dump(os);
    os << ")";
}
void NOpC::Accept(Visitor &visitor) { visitor.Visit(*this); }

void NBinary::Dump(std::ostream &os) const {
    os << "(";
    left_->Dump(os);
    os << " " << GetOpSym() << " ";
    right_->Dump(os);
    os << ")";
}

void NAdd::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NSub::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NMul::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NDiv::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NLess::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NGreat::Accept(Visitor &visitor) { visitor.Visit(*this); }
void NEqual::Accept(Visitor &visitor) { visitor.Visit(*this); }
