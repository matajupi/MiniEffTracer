#include "tracer.h"

void Tracer::Visit(const Top &top) {
    top.GetContent()->Accept(*this);
    os_ << std::endl;
}
void Tracer::Visit(const Number &num) {
    ret_ = num.GetValue();
    os_ << ret_;
}
void Tracer::Visit(const Add &add) {
    ProcessBinary(add, [](int lval, int rval) { return lval + rval; });
}
void Tracer::Visit(const Sub &sub) {
    ProcessBinary(sub, [](int lval, int rval) { return lval - rval; });
}
void Tracer::Visit(const Mul &mul) {
    ProcessBinary(mul, [](int lval, int rval) { return lval * rval; });
}
void Tracer::Visit(const Div &div) {
    ProcessBinary(div, [](int lval, int rval) { return lval / rval; });
}
void Tracer::ProcessBinary(const Binary &bin, std::function<int(int, int)> app) {
    os_ << "[" << std::endl;
    tabs_++;

    PrintTabs();
    bin.GetLeft()->Accept(*this);
    auto lval = ret_;
    os_ << " " << bin.GetOpSym() << " ";
    bin.GetRight()->Accept(*this);
    auto rval = ret_;
    os_ << std::endl;

    PrintTabs();
    os_ << "=>" << std::endl;

    PrintTabs();
    ret_ = app(lval, rval);
    os_ << ret_ << std::endl;

    tabs_--;
    PrintTabs();
    os_ << "]";
}
void Tracer::PrintTabs() {
    for (int i = 0; tabs_ > i; i++) {
        os_ << "\t";
    }
}
