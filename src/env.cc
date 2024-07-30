#include "env.h"

#include "utils.h"

std::shared_ptr<Prim> Env::Lookup(std::string var) {
    if (table_.find(var) != table_.end()) {
        return table_[var];
    }
    if (parent_ == nullptr) {
        throw VariableNotFoundException();
    }
    return parent_->Lookup(var);
}
void Env::Register(std::string var, std::shared_ptr<Prim> val) {
    table_[var] = val;
}

