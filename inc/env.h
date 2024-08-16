#pragma once

#include <map>
#include <string>

#include "values.h"

class Env {
public:
    Env(Env *parent) : parent_(parent) { }

    Value *Lookup(std::string var);
    void Register(std::string var, Value *val);
    Env *GetParent() const { return parent_; }

private:
    std::map<std::string, Value *> table_;
    Env *parent_;
};

