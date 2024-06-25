#include "tracer.h"

void Tracer::Visit(const Top &top) {
    top.GetContent()->Accept(*this);
}
void Tracer::Visit(const Prog &prog) {
    prog.GetPrev()->Accept(*this);
    prog.GetExpr()->Accept(*this);
    os_ << " ;; " << std::endl;
}
void Tracer::Visit(const Empty &empty) {
    // Nop
}
void Tracer::Visit(const NInt &num) {
    ret_ = PInt::GetInstance(num.GetValue());
    ret_->Dump(os_);
}
void Tracer::Visit(const NBool &bol) {
    ret_ = PBool::GetInstance(bol.GetValue());
    ret_->Dump(os_);
}
void Tracer::Visit(const Add &add) {
    ProcessBinary(add, [](PrimPtr lval, PrimPtr rval) {
        return lval->Add(rval);
    });
}
void Tracer::Visit(const Sub &sub) {
    ProcessBinary(sub, [](PrimPtr lval, PrimPtr rval) {
        return lval->Sub(rval);
    });
}
void Tracer::Visit(const Mul &mul) {
    ProcessBinary(mul, [](PrimPtr lval, PrimPtr rval) {
        return lval->Mul(rval);
    });
}
void Tracer::Visit(const Div &div) {
    ProcessBinary(div, [](PrimPtr lval, PrimPtr rval) {
        return lval->Div(rval);
    });
}
void Tracer::Visit(const Less &less) {
    ProcessBinary(less, [](PrimPtr lval, PrimPtr rval) {
        return lval->Less(rval);
    });
}
void Tracer::Visit(const Great &grt) {
    ProcessBinary(grt, [](PrimPtr lval, PrimPtr rval) {
        return lval->Great(rval);
    });
}
void Tracer::Visit(const Equal &eq) {
    ProcessBinary(eq, [](PrimPtr lval, PrimPtr rval) {
        return lval->Equal(rval);
    });
}
void Tracer::ProcessBinary(const Binary &bin,
    std::function<PrimPtr(PrimPtr, PrimPtr)> app) {
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
    ret_->Dump(os_);
    os_ << std::endl;

    tabs_--;
    PrintTabs();
    os_ << "]";
}
void Tracer::PrintTabs() {
    for (int i = 0; tabs_ > i; i++) {
        os_ << "\t";
    }
}
