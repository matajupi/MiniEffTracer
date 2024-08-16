#include "values.h"

#include "utils.h"

#define BINARYOP_HEADER(cls, op) Value *cls::op(Value *other)
#define UNARYOP_HEADER(cls, op) Value *cls::op()
#define DUMP_HEADER(cls) void cls::Dump(std::ostream &os)

BINARYOP_HEADER(Value, Add) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Sub) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Mul) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Div) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Less) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Great) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, LessEq) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, GreatEq) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, Equal) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, LogicAnd) {
    throw UnsupportedOperatorException();
}
BINARYOP_HEADER(Value, LogicOr) {
    throw UnsupportedOperatorException();
}
UNARYOP_HEADER(Value, Positive) {
    throw UnsupportedOperatorException();
}
UNARYOP_HEADER(Value, Negative) {
    throw UnsupportedOperatorException();
}

std::map<int, VInt *> VInt::storage_;
VInt *VInt::GetInstance(int value) {
    if (storage_.find(value) == storage_.end()) {
        storage_[value] = new VInt(value);
    }
    return storage_[value];
}
BINARYOP_HEADER(VInt, Add) {
    return VInt::GetInstance(value_ + Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Sub) {
    return VInt::GetInstance(value_ - Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Mul) {
    return VInt::GetInstance(value_ * Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Div) {
    return VInt::GetInstance(value_ / Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Less) {
    return VBool::GetInstance(value_ < Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Great) {
    return VBool::GetInstance(value_ > Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, LessEq) {
    return VBool::GetInstance(value_ <= Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, GreatEq) {
    return VBool::GetInstance(value_ >= Cast<VInt>(other)->value_);
}
BINARYOP_HEADER(VInt, Equal) {
    return VBool::GetInstance(value_ == Cast<VInt>(other)->value_);
}
UNARYOP_HEADER(VInt, Positive) {
    return this;
}
UNARYOP_HEADER(VInt, Negative) {
    return VInt::GetInstance(-value_);
}
DUMP_HEADER(VInt) {
    os << value_;
}

VBool *VBool::true_inst_ = new VBool(true);
VBool *VBool::false_inst_ = new VBool(false);
VBool *VBool::GetInstance(bool value) {
    if (value) {
        return true_inst_;
    }
    return false_inst_;
}
BINARYOP_HEADER(VBool, Equal) {
    return VBool::GetInstance(value_ == Cast<VBool>(other)->value_);
}
BINARYOP_HEADER(VBool, LogicAnd) {
    return VBool::GetInstance(value_ && Cast<VBool>(other)->value_);
}
BINARYOP_HEADER(VBool, LogicOr) {
    return VBool::GetInstance(value_ || Cast<VBool>(other)->value_);
}
DUMP_HEADER(VBool) {
    os << std::boolalpha << value_;
}

VFun *VFun::GetInstance(std::string var, Node *body, Env *env) {
    return new VFun(var, body, env);
}
BINARYOP_HEADER(VFun, Equal) {
    auto [fother, _] = Cast<VFun, VPrimFun>(other);
    return VBool::GetInstance(this == fother);
}
DUMP_HEADER(VFun) {
    os << "(" << "fun " << var_ << " -> ";
    body_->Dump(os);
    os << ")";
}

VPrimFun *VPrimFun::GetInstance(std::string name, VPrimFun::FunType fun) {
    return new VPrimFun(name, fun);
}
Value *VPrimFun::Call(Value *arg) {
    return fun_(arg);
}
BINARYOP_HEADER(VPrimFun, Equal) {
    auto [_, pother] = Cast<VFun, VPrimFun>(other);
    return VBool::GetInstance(this == pother);
}
DUMP_HEADER(VPrimFun) {
    os << name_;
}

VUnit *VUnit::inst_ = new VUnit();
VUnit *VUnit::GetInstance() {
    return inst_;
}
BINARYOP_HEADER(VUnit, Equal) {
    return VBool::GetInstance(this == Cast<VUnit>(other));
}
DUMP_HEADER(VUnit) {
    os << "()";
}

VPair *VPair::GetInstance(Value *val1, Value *val2) {
    return new VPair(val1, val2);
}
BINARYOP_HEADER(VPair, Equal) {
    auto pother = Cast<VPair>(other);
    auto eq1 = Cast<VBool>(val1_->Equal(pother->val1_));
    auto eq2 = Cast<VBool>(val2_->Equal(pother->val2_));
    return VBool::GetInstance(eq1->GetValue() && eq2->GetValue());
}
DUMP_HEADER(VPair) {
    os << "(";
    val1_->Dump(os);
    os << ", ";
    val2_->Dump(os);
    os << ")";
}

