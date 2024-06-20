#include "nodes.h"

#include "visitor.h"

void Top::Dump(std::ostream &os) const {
    content_->Dump(os);
    os << std::endl;
}
void Top::Accept(Visitor &visitor) { visitor.Visit(*this); }

void Number::Dump(std::ostream &os) const {
    os << value_;
}
void Number::Accept(Visitor &visitor) { visitor.Visit(*this); }

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
