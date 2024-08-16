#pragma once

#include <iostream>
#include <map>
#include <functional>

#include "nodes.h"

#define BINARYOP_DEC(op) Value *op(Value *other)
#define UNARYOP_DEC(op) Value *op()
#define DUMP_DEC void Dump(std::ostream &os)

class Env;

class Value {
public:
    virtual BINARYOP_DEC(Add);
    virtual BINARYOP_DEC(Sub);
    virtual BINARYOP_DEC(Mul);
    virtual BINARYOP_DEC(Div);
    virtual BINARYOP_DEC(Less);
    virtual BINARYOP_DEC(Great);
    virtual BINARYOP_DEC(LessEq);
    virtual BINARYOP_DEC(GreatEq);
    virtual BINARYOP_DEC(Equal);
    virtual BINARYOP_DEC(LogicAnd);
    virtual BINARYOP_DEC(LogicOr);
    virtual UNARYOP_DEC(Positive);
    virtual UNARYOP_DEC(Negative);

    virtual DUMP_DEC = 0;
};

class VInt : public Value {
public:
    static VInt *GetInstance(int value);

    VInt(int value) : value_(value) { }

    int GetValue() const { return value_; }

    BINARYOP_DEC(Add) override;
    BINARYOP_DEC(Sub) override;
    BINARYOP_DEC(Mul) override;
    BINARYOP_DEC(Div) override;
    BINARYOP_DEC(Less) override;
    BINARYOP_DEC(Great) override;
    BINARYOP_DEC(LessEq) override;
    BINARYOP_DEC(GreatEq) override;
    BINARYOP_DEC(Equal) override;
    UNARYOP_DEC(Positive) override;
    UNARYOP_DEC(Negative) override;
    DUMP_DEC override;

private:
    static std::map<int, VInt *> storage_;

    int value_;
};

class VBool : public Value {
public:
    static VBool *GetInstance(bool value);

    VBool(bool value)
        : value_(value) { }

    bool GetValue() const { return value_; }

    BINARYOP_DEC(Equal) override;
    BINARYOP_DEC(LogicAnd) override;
    BINARYOP_DEC(LogicOr) override;
    DUMP_DEC override;

private:
    static VBool *true_inst_;
    static VBool *false_inst_;

    bool value_;
};

class VFun : public Value {
public:
    static VFun *GetInstance(std::string var, Node *body, Env *env);

    VFun(std::string var, Node *body, Env *env)
        : var_(var), body_(body), env_(env) { }

    std::string GetVar() const { return var_; }
    Node *GetBody() const { return body_; }
    Env *GetEnv() const { return env_; }

    BINARYOP_DEC(Equal) override;
    DUMP_DEC override;

private:
    std::string var_;
    Node *body_;
    Env *env_;
};

class VPrimFun : public Value {
public:
    using FunType = std::function<Value *(Value *)>;

    static VPrimFun *GetInstance(std::string name, FunType fun);

    VPrimFun(std::string name, FunType fun)
        : name_(name), fun_(fun) { }

    std::string GetName() const { return name_; }
    FunType GetFun() const { return fun_; }

    Value *Call(Value *arg);

    BINARYOP_DEC(Equal) override;
    DUMP_DEC override;

private:
    std::string name_;
    FunType fun_;
};

class VUnit : public Value {
public:
    static VUnit *GetInstance();

    BINARYOP_DEC(Equal) override;
    DUMP_DEC override;

private:
    static VUnit *inst_;
};

class VPair : public Value {
public:
    static VPair *GetInstance(Value *val1, Value *val2);

    VPair(Value *val1, Value *val2)
        : val1_(val1), val2_(val2) { }

    Value *GetVal1() const { return val1_; }
    Value *GetVal2() const { return val2_; }

    BINARYOP_DEC(Equal) override;
    DUMP_DEC override;

private:
    Value *val1_;
    Value *val2_;
};

