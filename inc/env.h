#pragma once

#include <map>
#include <string>
#include <memory>

#include "prims.h"

class Env {
public:
    Env(std::shared_ptr<Env> parent) : parent_(parent) { }

    std::shared_ptr<Prim> Lookup(std::string var);
    void Register(std::string var, std::shared_ptr<Prim> val);
    std::shared_ptr<Env> GetParent() const { return parent_; }

private:
    std::map<std::string, std::shared_ptr<Prim>> table_;
    std::shared_ptr<Env> parent_;
};
