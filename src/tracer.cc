#include "tracer.h"

#include "utils.h"

Tracer::Tracer(std::ostream &os)
    : tabs_(0), os_(os), env_(std::make_shared<Env>(nullptr)) {
    RegisterPrim("fst", [](std::shared_ptr<Prim> prim) {
        auto prod = Cast<PProduct>(prim);
        return prod->GetVal1();
    });
    RegisterPrim("snd", [](std::shared_ptr<Prim> prim) {
        auto prod = Cast<PProduct>(prim);
        return prod->GetVal2();
    });
    RegisterPrim("print", [&](std::shared_ptr<Prim> prim) {
        prim->Dump(os_);
        os_ << std::endl;
        return PUnit::GetInstance();
    });
}

void Tracer::RegisterPrim(std::string name, PPrimFun::FunType fun) {
    env_->Register(name, PPrimFun::GetInstance(name, fun));
}

void Tracer::Visit(const NTop &top) {
    for (auto p : *top.GetProg()) {
        p->Accept(*this);
        os_ << " ;;" << std::endl;
    }
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
void Tracer::Visit(const NIdent &ident) {
    auto str = ident.GetStr();
    os_ << "[" << str << " => ";

    ret_ = env_->Lookup(str);
    ret_->Dump(os_);

    os_ << "]";
}
void Tracer::Visit(const NLet &let) {
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
void Tracer::Visit(const NLetRec &let) {
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
void Tracer::Visit(const NLetDef &let) {
    In();

    auto var = let.GetVar();

    os_ << "let " << var << " = ";

    let.GetBexpr()->Accept(*this);
    env_->Register(var, ret_);

    Out();
}
void Tracer::Visit(const NLetRecDef &let) {
    In();

    auto var = let.GetVar();

    os_ << "let rec " << var << " = ";

    env_->Register(var, PUnit::GetInstance());
    let.GetBexpr()->Accept(*this);
    env_->Register(var, ret_);

    Out();
}
void Tracer::Visit(const NSeq &seq) {
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
void Tracer::Visit(const NApp &app) {
    In();

    app.GetFun()->Accept(*this);
    auto prim = TryCast<PPrimFun>(ret_);
    auto fun = TryCast<PFun>(ret_);
    auto opc = TryCast<POpC>(ret_);
    if (prim == nullptr && fun == nullptr && opc == nullptr) {
        throw CastFailureException();
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
    else if (fun != nullptr) {
        auto cenv = env_;
        env_ = std::make_shared<Env>(fun->GetEnv());
        env_->Register(fun->GetVar(), arg);
        fun->GetBody()->Accept(*this);
        env_ = cenv;

        Newline();

        os_ << "=>";
        Newline();
    }
    else {
        // TODO: opc
    }

    ret_->Dump(os_);

    Out();
}
void Tracer::Visit(const NIf &ifn) {
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

    auto cond = Cast<PBool>(ret_);
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
void Tracer::Visit(const NHandler &handler) {
    handler.GetRetC()->Accept(*this);
    auto pretc = Cast<POpC>(ret_);
    auto peffcs = std::make_shared<PHandler::POpCList>();
    for (auto neffc : *handler.GetEffCs()) {
        neffc->Accept(*this);
        peffcs->push_back(Cast<POpC>(ret_));
    }
    ret_ = PHandler::GetInstance(pretc, peffcs);
    ret_->Dump(os_);
}
void Tracer::Visit(const NWithHandle &with) {
    // In();

    // os_ << "with ";

    // with.GetHandler()->Accept(*this);
    // auto handler = Cast<PHandler>(ret_);

    // os_ << " handle ";

    // env_ = std::make_shared<Env>(env_);
    // for (auto effc : *handler->GetEffCs()) {
    //     env_->Register(effc->GetEff(), PEffect::GetInstance(effc->GetEff()));
    // }

    // with.GetBody()->Accept(*this);

    // // TODO: process effect

    // Newline();
    // os_ << "=>";
    // Newline();

    // ret_->Dump(os_);
    // env_ = env_->GetParent();

    // Out();
}
void Tracer::Visit(const NOpC &opc) {
    ret_ = POpC::GetInstance(
        opc.GetEff(), opc.GetVar(), opc.GetCont(), opc.GetBody(), env_);
    ret_->Dump(os_);
}
void Tracer::Visit(const NAdd &add) {
    ProcessBinary(add, [](PrimPtr lval, PrimPtr rval) {
        return lval->Add(rval);
    });
}
void Tracer::Visit(const NSub &sub) {
    ProcessBinary(sub, [](PrimPtr lval, PrimPtr rval) {
        return lval->Sub(rval);
    });
}
void Tracer::Visit(const NMul &mul) {
    ProcessBinary(mul, [](PrimPtr lval, PrimPtr rval) {
        return lval->Mul(rval);
    });
}
void Tracer::Visit(const NDiv &div) {
    ProcessBinary(div, [](PrimPtr lval, PrimPtr rval) {
        return lval->Div(rval);
    });
}
void Tracer::Visit(const NLess &less) {
    ProcessBinary(less, [](PrimPtr lval, PrimPtr rval) {
        return lval->Less(rval);
    });
}
void Tracer::Visit(const NGreat &grt) {
    ProcessBinary(grt, [](PrimPtr lval, PrimPtr rval) {
        return lval->Great(rval);
    });
}
void Tracer::Visit(const NEqual &eq) {
    ProcessBinary(eq, [](PrimPtr lval, PrimPtr rval) {
        return lval->Equal(rval);
    });
}
void Tracer::ProcessBinary(const NBinary &bin,
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
