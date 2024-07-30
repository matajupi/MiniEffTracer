#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <iostream>
#include <map>

#include "nodes.h"

class Env;

class Prim {
public:
    virtual std::shared_ptr<Prim> Add(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Sub(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Mul(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Div(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Less(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Great(std::shared_ptr<Prim> other);
    virtual std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other);

    virtual void Dump(std::ostream &os) = 0;
};

class PInt : public Prim {
public:
    static std::shared_ptr<PInt> GetInstance(int value);
    static void ClearStorage() { storage_.clear(); }

    PInt(int value) : value_(value) { }
    int GetValue() const { return value_; }

    std::shared_ptr<Prim> Add(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Sub(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Mul(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Div(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Less(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Great(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    int value_;
    static std::map<int, std::shared_ptr<PInt>> storage_;
};

class PBool : public Prim {
public:
    static std::shared_ptr<PBool> GetInstance(bool value);
    static void ClearStorage() { storage_.clear(); }

    PBool(bool value) : value_(value) { }
    bool GetValue() const { return value_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    bool value_;
    static std::map<bool, std::shared_ptr<PBool>> storage_;
};

class PFun : public Prim {
public:
    static std::shared_ptr<PFun> GetInstance(
        std::string var, std::shared_ptr<Node> body, std::shared_ptr<Env> env) {
        return std::make_shared<PFun>(var, body, env);
    }

    PFun(std::string var, std::shared_ptr<Node> body, std::shared_ptr<Env> env)
        : var_(var), body_(body), env_(env) { }

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBody() const { return body_; }
    std::shared_ptr<Env> GetEnv() const { return env_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::string var_;
    std::shared_ptr<Node> body_;
    std::shared_ptr<Env> env_;
};

class PUnit : public Prim {
public:
    static std::shared_ptr<PUnit> GetInstance() {
        if (instance_ == nullptr) {
            instance_ = std::make_shared<PUnit>();
        }
        return instance_;
    }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    static std::shared_ptr<PUnit> instance_;
};

class PProduct : public Prim {
public:
    static std::shared_ptr<PProduct> GetInstance(
        std::shared_ptr<Prim> val1, std::shared_ptr<Prim> val2) {
        return std::make_shared<PProduct>(val1, val2);
    }

    PProduct(std::shared_ptr<Prim> val1, std::shared_ptr<Prim> val2)
        : val1_(val1), val2_(val2) { }

    std::shared_ptr<Prim> GetVal1() const { return val1_; }
    std::shared_ptr<Prim> GetVal2() const { return val2_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::shared_ptr<Prim> val1_;
    std::shared_ptr<Prim> val2_;
};

class PPrimFun : public Prim {
public:
    using FunType = std::function<std::shared_ptr<Prim>(std::shared_ptr<Prim>)>;

    static std::shared_ptr<PPrimFun> GetInstance(std::string name, FunType fun) {
        return std::make_shared<PPrimFun>(name, fun);
    }

    PPrimFun(std::string name, FunType fun)
        : name_(name), fun_(fun) { };

    std::string GetName() const { return name_; }
    FunType GetFun() const { return fun_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::string name_;
    FunType fun_;
};

class POpC : public Prim {
public:
    static std::shared_ptr<POpC> GetInstance(std::string var,
        std::shared_ptr<Node> body, std::shared_ptr<Env> env) {
        return std::make_shared<POpC>(var, body, env);
    }
    static std::shared_ptr<POpC> GetInstance(std::string eff, std::string var,
        std::string cont, std::shared_ptr<Node> body, std::shared_ptr<Env> env) {
        return std::make_shared<POpC>(eff, var, cont, body, env);
    }

    POpC(std::string var, std::shared_ptr<Node> body, std::shared_ptr<Env> env)
        : POpC("", var, "", body, env) { }
    POpC(std::string eff, std::string var, std::string cont,
        std::shared_ptr<Node> body, std::shared_ptr<Env> env)
        : eff_(eff), var_(var), cont_(cont), body_(body), env_(env) { }

    std::string GetEff() const { return eff_; }
    std::string GetVar() const { return var_; }
    std::string GetCont() const { return cont_; }
    std::shared_ptr<Node> GetBody() const { return body_; }
    std::shared_ptr<Env> GetEnv() const { return env_; }

    bool IsReturn() const { return eff_ == ""; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::string eff_;
    std::string var_;
    std::string cont_;
    std::shared_ptr<Node> body_;
    std::shared_ptr<Env> env_;
};

class PHandler : public Prim {
public:
    using POpCList = std::vector<std::shared_ptr<POpC>>;

    // TODO: Create TracerでPOpCを作ってくる
    static std::shared_ptr<PHandler> GetInstance(
        std::shared_ptr<POpC> retc, std::shared_ptr<POpCList> effcs) {
        return std::make_shared<PHandler>(retc, effcs);
    }

    PHandler(std::shared_ptr<POpC> retc, std::shared_ptr<POpCList> effcs)
        : retc_(retc), effcs_(effcs) { };

    std::shared_ptr<POpC> GetRetC() const { return retc_; }
    std::shared_ptr<POpCList> GetEffCs() const { return effcs_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::shared_ptr<POpC> retc_;
    std::shared_ptr<POpCList> effcs_;
};
