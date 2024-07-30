#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <iostream>
#include <map>

#include "nodes.h"
#include "excepts.h"

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

    template <typename T>
    static std::shared_ptr<T> Cast(std::shared_ptr<Prim> before) {
        auto after = std::dynamic_pointer_cast<T>(before);
        if (after == nullptr) {
            throw CastFailureException();
        }
        return after;
    }
    template <typename T>
    static std::shared_ptr<T> TryCast(std::shared_ptr<Prim> before) {
        auto after = std::dynamic_pointer_cast<T>(before);
        return after;
    }
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

class PHandler : public Prim {
public:
    static std::shared_ptr<PHandler> GetInstance(
        std::shared_ptr<Node::NodeList> opcs, std::shared_ptr<Env> env) {
        return std::make_shared<PHandler>(opcs, env);
    }

    PHandler(std::shared_ptr<Node::NodeList> opcs, std::shared_ptr<Env> env)
        : opcs_(opcs), env_(env) { };

    // std::shared_ptr<POp> GetOpCs() const { return opcs_; }
    std::shared_ptr<Env> GetEnv() const { return env_; }

    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    std::shared_ptr<Node::NodeList> opcs_;
    std::shared_ptr<Env> env_;
};

