#include "prims.h"

#include "excepts.h"

template <typename T>
std::shared_ptr<T> Prim::Cast(std::shared_ptr<Prim> before) {
    auto after = std::dynamic_pointer_cast<T>(before);
    if (after == nullptr) {
        throw CastFailureException();
    }
    return after;
}
std::shared_ptr<Prim> Prim::Add(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Sub(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Mul(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Div(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Less(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Great(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}
std::shared_ptr<Prim> Prim::Equal(std::shared_ptr<Prim> other) {
    throw UnsupportedOperatorException();
}

std::map<int, std::shared_ptr<PInt>> PInt::storage_{};
std::shared_ptr<PInt> PInt::GetInstance(int value) {
    if (storage_.find(value) != storage_.end()) {
        return storage_[value];
    }
    auto inst = std::make_shared<PInt>(value);
    storage_[value] = inst;
    return inst;
}
std::shared_ptr<Prim> PInt::Add(std::shared_ptr<Prim> other) {
    return PInt::GetInstance(value_ + Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Sub(std::shared_ptr<Prim> other) {
    return PInt::GetInstance(value_ - Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Mul(std::shared_ptr<Prim> other) {
    return PInt::GetInstance(value_ * Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Div(std::shared_ptr<Prim> other) {
    return PInt::GetInstance(value_ / Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Less(std::shared_ptr<Prim> other) {
    return PBool::GetInstance(value_ < Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Great(std::shared_ptr<Prim> other) {
    return PBool::GetInstance(value_ > Cast<PInt>(other)->value_);
}
std::shared_ptr<Prim> PInt::Equal(std::shared_ptr<Prim> other) {
    return PBool::GetInstance(value_ == Cast<PInt>(other)->value_);
}
void PInt::Dump(std::ostream &os) {
    os << value_;
}

std::map<bool, std::shared_ptr<PBool>> PBool::storage_{};
std::shared_ptr<PBool> PBool::GetInstance(bool value) {
    if (storage_.find(value) != storage_.end()) {
        return storage_[value];
    }
    auto inst = std::make_shared<PBool>(value);
    storage_[value] = inst;
    return inst;
}
std::shared_ptr<Prim> PBool::Equal(std::shared_ptr<Prim> other) {
    return PBool::GetInstance(value_ == Cast<PBool>(other)->value_);
}
void PBool::Dump(std::ostream &os) {
    os << std::boolalpha << value_;
}

std::shared_ptr<Prim> PFun::Equal(std::shared_ptr<Prim> other) {
    return PBool::GetInstance(this == other.get());
}
void PFun::Dump(std::ostream &os) {
    os << "(" << "fun " << var_ << " -> ";
    body_->Dump(os);
    os << ")";
}

