#include "nodes.h"

#include "visitor.h"

void Top::Dump(std::ostream &os) const {
    content_->Dump(os);
    os << std::endl;
}
void Top::Accept(Visitor &visitor) { visitor.Visit(*this); }

void Prog::Dump(std::ostream &os) const {
    prev_->Dump(os);
    expr_->Dump(os);
    os << " ;;" << std::endl;
}
void Prog::Accept(Visitor &visitor) { visitor.Visit(*this); }

void Empty::Dump(std::ostream &os) const {
    os << "[EOF]" << std::endl;
}
void Empty::Accept(Visitor &visitor) { visitor.Visit(*this); }

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

void Ident::Dump(std::ostream &os) const {
    os << str_;
}
void Ident::Accept(Visitor &visitor) { visitor.Visit(*this); }

void Let1::Dump(std::ostream &os) const {
    os << "let " << var_ << " = ";
    bexpr_->Dump(os);
    os << " in ";
    cexpr_->Dump(os);
    os << " end";
}
void Let1::Accept(Visitor &visitor) { visitor.Visit(*this); }

void App::Dump(std::ostream &os) const {
    os << "(";
    fun_->Dump(os);
    os << " ";
    arg_->Dump(os);
    os << ")";
}
void App::Accept(Visitor &visitor) { visitor.Visit(*this); }

void Binary::Dump(std::ostream &os) const {
    os << "(";
    left_->Dump(os);
    os << " " << GetOpSym() << " ";
    right_->Dump(os);
    os << ")";
}

void Add::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Sub::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Mul::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Div::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Less::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Great::Accept(Visitor &visitor) { visitor.Visit(*this); }
void Equal::Accept(Visitor &visitor) { visitor.Visit(*this); }
