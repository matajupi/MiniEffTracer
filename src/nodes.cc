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
