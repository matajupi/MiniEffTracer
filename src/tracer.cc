#include "tracer.h"

#include "utils.h"

#define VISIT_HEADER(cls) void Tracer::Visit(cls *node)

Tracer::Tracer(std::ostream &os)
    : os_(os), tabs_(0), env_(new Env(nullptr)) {
    RegisterPrimFun("fst", [](Value *val) {
        auto pair = Cast<VPair>(val);
        return pair->GetVal1();
    });
    RegisterPrimFun("snd", [](Value *val) {
        auto pair = Cast<VPair>(val);
        return pair->GetVal2();
    });
    RegisterPrimFun("print", [&](Value *val) {
        val->Dump(os_);
        os_ << std::endl;
        return VUnit::GetInstance();
    });
}
VISIT_HEADER(NTop) {
    node->GetDec()->Accept(*this);
    Newline();
}
VISIT_HEADER(NInt) {
    ret_ = VInt::GetInstance(node->GetValue());
    ret_->Dump(os_);
}
VISIT_HEADER(NBool) {
    ret_ = VBool::GetInstance(node->GetValue());
    ret_->Dump(os_);
}
VISIT_HEADER(NFun) {
    ret_ = VFun::GetInstance(node->GetVar(), node->GetBody(), env_);
    ret_->Dump(os_);
}
VISIT_HEADER(NUnit) {
    ret_ = VUnit::GetInstance();
    ret_->Dump(os_);
}
VISIT_HEADER(NPair) {
    In();

    os_ << "(";

    node->GetExpr1()->Accept(*this);
    auto val1 = ret_;

    os_ << ", ";

    node->GetExpr2()->Accept(*this);
    auto val2 = ret_;

    os_ << ")";

    ArrowLine();

    ret_ = VPair::GetInstance(val1, val2);
    ret_->Dump(os_);

    Out();
}
VISIT_HEADER(NIdent) {
    auto var = node->GetStr();
    os_ << "[" << var << " => ";

    ret_ = env_->Lookup(var);
    ret_->Dump(os_);

    os_ << "]";
}
VISIT_HEADER(NLet) {
    if (node->IsDec()) {
        auto var = node->GetVar();

        os_ << "let " << var << " = ";

        node->GetBexpr()->Accept(*this);
        env_->Register(var, ret_);
    }
    else {
        In();

        auto var = node->GetVar();

        os_ << "let " << var << " = ";

        node->GetBexpr()->Accept(*this);

        os_ << " in ";

        env_ = new Env(env_);
        env_->Register(var, ret_);
        node->GetCexpr()->Accept(*this);
        env_ = env_->GetParent();

        ArrowLine();

        ret_->Dump(os_);

        Out();
    }
}
VISIT_HEADER(NLetRec) {
    if (node->IsDec()) {
        auto var = node->GetVar();

        os_ << "let rec " << var << " = ";

        env_->Register(var, VUnit::GetInstance());
        node->GetBexpr()->Accept(*this);
        env_->Register(var, ret_);
    }
    else {
        In();

        auto var = node->GetVar();

        os_ << "let rec " << var << " = ";

        env_ = new Env(env_);
        env_->Register(var, VUnit::GetInstance());
        node->GetBexpr()->Accept(*this);

        os_ << " in ";

        env_->Register(var, ret_);
        node->GetCexpr()->Accept(*this);
        env_ = env_->GetParent();

        ArrowLine();

        ret_->Dump(os_);

        Out();
    }
}
VISIT_HEADER(NSeq) {
    switch (node->GetSK()) {
        case SeqKind::Dec: {
            node->GetExpr1()->Accept(*this);
            os_ << " ;;";
            Newline();
            node->GetExpr2()->Accept(*this);
            break;
        }
        case SeqKind::Expr: {
            In();

            node->GetExpr1()->Accept(*this);
            os_ << "; ";
            node->GetExpr2()->Accept(*this);
            ArrowLine();
            ret_->Dump(os_);

            Out();
            break;
        }
        case SeqKind::OpCase: {
            os_ << "TODO:" << std::endl;
            break;
        }
    }
}
VISIT_HEADER(NApp) {
    In();

    node->GetFun()->Accept(*this);
    auto [fun, prim] = Cast<VFun, VPrimFun>(ret_);

    os_ << " ";

    node->GetArg()->Accept(*this);
    auto arg = ret_;

    ArrowLine();

    if (fun) {
        auto tmp = env_;
        env_ = new Env(fun->GetEnv());
        env_->Register(fun->GetVar(), arg);
        fun->GetBody()->Accept(*this);
        env_ = tmp;

        ArrowLine();
    }
    else {
        ret_ = prim->Call(arg);
    }

    ret_->Dump(os_);

    Out();
}
VISIT_HEADER(NBinaryApp) {
    In();

    auto opsym = node->GetOpSym();

    node->GetLeft()->Accept(*this);
    auto lval = ret_;

    os_ << " " << opsym << " ";

    node->GetRight()->Accept(*this);
    auto rval = ret_;

    ArrowLine();

    if (opsym == "+") {
        ret_ = lval->Add(rval);
    }
    else if (opsym == "-") {
        ret_ = lval->Sub(rval);
    }
    else if (opsym == "*") {
        ret_ = lval->Mul(rval);
    }
    else if (opsym == "/") {
        ret_ = lval->Div(rval);
    }
    else if (opsym == "<") {
        ret_ = lval->Less(rval);
    }
    else if (opsym == ">") {
        ret_ = lval->Great(rval);
    }
    else if (opsym == "<=") {
        ret_ = lval->LessEq(rval);
    }
    else if (opsym == ">=") {
        ret_ = lval->GreatEq(rval);
    }
    else if (opsym == "=") {
        ret_ = lval->Equal(rval);
    }
    else if (opsym == "&&") {
        ret_ = lval->LogicAnd(rval);
    }
    else if (opsym == "||") {
        ret_ = lval->LogicOr(rval);
    }
    else {
        throw UnknownOperatorException();
    }

    ret_->Dump(os_);

    Out();
}
VISIT_HEADER(NUnaryApp) {
    In();

    auto opsym = node->GetOpSym();
    os_ << opsym;
    node->GetExpr()->Accept(*this);
    auto val = ret_;

    ArrowLine();

    if (opsym == "+") {
        ret_ = val->Positive();
    }
    else if (opsym == "-") {
        ret_ = val->Negative();
    }
    else {
        throw UnknownOperatorException();
    }

    ret_->Dump(os_);

    Out();
}
VISIT_HEADER(NCond) {
    In();

    os_ << "if ";
    node->GetCond()->Accept(*this);
    os_ << " then ";
    node->GetConseq()->Dump(os_);
    os_ << " else ";
    node->GetAlter()->Dump(os_);

    ArrowLine();

    auto cond = Cast<VBool>(ret_);
    if (cond->GetValue()) {
        node->GetConseq()->Accept(*this);
    }
    else {
        node->GetAlter()->Accept(*this);
    }

    ArrowLine();

    ret_->Dump(os_);

    Out();
}
VISIT_HEADER(NHandler) {
    os_ << "TODO:" << std::endl;
}
VISIT_HEADER(NOpCase) {
    os_ << "TODO:" << std::endl;
}
VISIT_HEADER(NWithHandle) {
    os_ << "TODO:" << std::endl;
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
void Tracer::ArrowLine() {
    Newline();
    os_ << "=>";
    Newline();
}
void Tracer::PrintTabs() {
    for (int i = 0; tabs_ > i; i++) {
        os_ << "  ";
    }
}
void Tracer::RegisterPrimFun(std::string name, VPrimFun::FunType fun) {
    env_->Register(name, VPrimFun::GetInstance(name, fun));
}
