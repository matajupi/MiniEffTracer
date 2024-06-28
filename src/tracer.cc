#include "tracer.h"

#include "excepts.h"

Tracer::Tracer(std::ostream &os)
    : tabs_(0), os_(os), env_(std::make_shared<Env>(nullptr)) {
    RegisterPrim("fst", [](std::shared_ptr<Prim> prim) {
        auto prod = Prim::Cast<PProduct>(prim);
        return prod->GetVal1();
    });
    RegisterPrim("snd", [](std::shared_ptr<Prim> prim) {
        auto prod = Prim::Cast<PProduct>(prim);
        return prod->GetVal2();
    });
}

void Tracer::RegisterPrim(std::string name, PPrimFun::FunType fun) {
    env_->Register(name, PPrimFun::GetInstance(name, fun));
}

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
void Tracer::Visit(const NFun &fun) {
    ret_ = PFun::GetInstance(fun.GetVar(), fun.GetBody(), env_);
    ret_->Dump(os_);
}
void Tracer::Visit(const NUnit &unit) {
    ret_ = PUnit::GetInstance();
    ret_->Dump(os_);
}
void Tracer::Visit(const NProduct &prod) {
    In();

    os_ << "(";

    prod.GetExpr1()->Accept(*this);
    auto val1 = ret_;

    os_ << ", ";

    prod.GetExpr2()->Accept(*this);
    auto val2 = ret_;

    os_ << ")";
    Newline();

    os_ << "=>";
    Newline();

    ret_ = PProduct::GetInstance(val1, val2);
    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const Ident &ident) {
    auto str = ident.GetStr();
    os_ << "[" << str << " => ";

    ret_ = env_->Lookup(str);
    ret_->Dump(os_);

    os_ << "]";
}
void Tracer::Visit(const Let &let) {
    In();

    auto var = let.GetVar();

    os_ << "let " << var << " = ";

    let.GetBexpr()->Accept(*this);

    os_ << " in ";

    env_ = std::make_shared<Env>(env_);
    env_->Register(var, ret_);
    let.GetCexpr()->Accept(*this);
    env_ = env_->GetParent();

    Newline();

    os_ << "=>";
    Newline();

    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const LetRec &let) {
    In();

    auto var = let.GetVar();

    os_ << "let rec " << var << " = ";

    env_ = std::make_shared<Env>(env_);
    env_->Register(var, PUnit::GetInstance());
    let.GetBexpr()->Accept(*this);

    os_ << " in ";

    env_->Register(var, ret_);
    let.GetCexpr()->Accept(*this);
    env_ = env_->GetParent();

    Newline();

    os_ << "=>";
    Newline();

    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const LetDef &let) {
    In();

    auto var = let.GetVar();

    os_ << "let " << var << " = ";

    let.GetBexpr()->Accept(*this);
    env_->Register(var, ret_);

    Out();
}
void Tracer::Visit(const LetRecDef &let) {
    In();

    auto var = let.GetVar();

    os_ << "let rec " << var << " = ";

    env_->Register(var, PUnit::GetInstance());
    let.GetBexpr()->Accept(*this);
    env_->Register(var, ret_);

    Out();
}
void Tracer::Visit(const Seq &seq) {
    In();

    seq.GetExpr1()->Accept(*this);

    os_ << "; ";

    seq.GetExpr2()->Accept(*this);

    Newline();

    os_ << "=>";
    Newline();

    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const App &app) {
    In();

    app.GetFun()->Accept(*this);
    auto prim = std::dynamic_pointer_cast<PPrimFun>(ret_);
    auto fun = std::dynamic_pointer_cast<PFun>(ret_);
    if (prim == nullptr && fun == nullptr) {
        throw UnsupportedOperatorException();
    }

    os_ << " ";

    app.GetArg()->Accept(*this);
    auto arg = ret_;

    Newline();

    os_ << "=>";
    Newline();

    if (prim != nullptr) {
        ret_ = prim->GetFun()(arg);
    }
    else {
        auto cenv = env_;
        env_ = std::make_shared<Env>(fun->GetEnv());
        env_->Register(fun->GetVar(), arg);
        fun->GetBody()->Accept(*this);
        env_ = cenv;

        Newline();

        os_ << "=>";
        Newline();
    }

    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const If &ifn) {
    In();

    os_ << "if ";
    ifn.GetCond()->Accept(*this);
    os_ << " then ";
    ifn.GetIfClause()->Dump(os_);
    os_ << " else ";
    ifn.GetElseClause()->Dump(os_);
    Newline();

    os_ << "=>";
    Newline();

    auto cond = std::dynamic_pointer_cast<PBool>(ret_);
    if (cond == nullptr) {
        throw UnsupportedOperatorException();
    }
    if (cond->GetValue()) {
        ifn.GetIfClause()->Accept(*this);
    }
    else {
        ifn.GetElseClause()->Accept(*this);
    }
    Newline();

    os_ << "=>";
    Newline();

    ret_->Dump(os_);

    Out();
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
    In();

    bin.GetLeft()->Accept(*this);
    auto lval = ret_;

    os_ << " " << bin.GetOpSym() << " ";

    bin.GetRight()->Accept(*this);
    auto rval = ret_;

    Newline();

    os_ << "=>";
    Newline();

    ret_ = app(lval, rval);
    ret_->Dump(os_);

    Out();
}
void Tracer::In() {
    os_ << "[" << std::endl;
    tabs_++;
    PrintTabs();
}
void Tracer::Out() {
    os_ << std::endl;
    tabs_--;
    PrintTabs();
    os_ << "]";
}
void Tracer::Newline() {
    os_ << std::endl;
    PrintTabs();
}
void Tracer::PrintTabs() {
    for (int i = 0; tabs_ > i; i++) {
        os_ << "  ";
    }
}
