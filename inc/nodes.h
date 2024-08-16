#pragma once

#include <string>
#include <ostream>
#include <vector>

#define DUMP_DECL void Dump(std::ostream &os) const
#define ACCEPT_DECL void Accept(Visitor &visitor)

class Visitor;

class Node {
public:
    virtual ~Node() { }
    virtual DUMP_DECL = 0;
    virtual ACCEPT_DECL = 0;
};

class NTop : public Node {
public:
    NTop(Node *dec) : dec_(dec) { }
    ~NTop() override { delete dec_; }

    Node *GetDec() const { return dec_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *dec_;
};

class NInt : public Node {
public:
    NInt(int value) : value_(value) { }
    ~NInt() override { }

    int GetValue() const { return value_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    int value_;
};

class NBool : public Node {
public:
    NBool(bool value) : value_(value) { }
    ~NBool() override { }

    bool GetValue() const { return value_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    bool value_;
};

class NFun : public Node {
public:
    NFun(std::string var, Node *body)
        : var_(var), body_(body) { }
    ~NFun() override { delete body_; }

    std::string GetVar() const { return var_; }
    Node *GetBody() const { return body_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string var_;
    Node *body_;
};

class NUnit : public Node {
public:
    ~NUnit() override { }

    DUMP_DECL override;
    ACCEPT_DECL override;
};

class NPair : public Node {
public:
    NPair(Node *expr1, Node *expr2)
        : expr1_(expr1), expr2_(expr2) { }
    ~NPair() override {
        delete expr1_;
        delete expr2_;
    }

    Node *GetExpr1() const { return expr1_; }
    Node *GetExpr2() const { return expr2_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *expr1_;
    Node *expr2_;
};

class NIdent : public Node {
public:
    NIdent(std::string str) : str_(str) { }
    ~NIdent() override { }

    std::string GetStr() const { return str_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string str_;
};

class NLet : public Node {
public:
    NLet(std::string var, Node *bexpr, Node *cexpr)
        : var_(var), bexpr_(bexpr), cexpr_(cexpr) { }
    NLet(std::string var, Node *bexpr)
        : NLet(var, bexpr, nullptr) { }
    ~NLet() override {
        delete bexpr_;
        delete cexpr_;
    }

    bool IsDec() const { return cexpr_ == nullptr; }
    std::string GetVar() const { return var_; }
    Node *GetBexpr() const { return bexpr_; }
    Node *GetCexpr() const { return cexpr_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string var_;
    Node *bexpr_;
    Node *cexpr_;
};

class NLetRec : public Node {
public:
    NLetRec(std::string var, Node *bexpr, Node *cexpr)
        : var_(var), bexpr_(bexpr), cexpr_(cexpr) { }
    NLetRec(std::string var, Node *bexpr)
        : NLetRec(var, bexpr, nullptr) { }
    ~NLetRec() override {
        delete bexpr_;
        delete cexpr_;
    }

    bool IsDec() const { return cexpr_ == nullptr; }
    std::string GetVar() const { return var_; }
    Node *GetBexpr() const { return bexpr_; }
    Node *GetCexpr() const { return cexpr_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string var_;
    Node *bexpr_;
    Node *cexpr_;
};

enum class SeqKind {
    Dec,
    Expr,
    OpCase,
};

class NSeq : public Node {
public:
    NSeq(SeqKind sk, Node *expr1, Node *expr2)
        : sk_(sk), expr1_(expr1), expr2_(expr2) { }
    ~NSeq() override {
        delete expr1_;
        delete expr2_;
    }

    SeqKind GetSK() const { return sk_; }
    Node *GetExpr1() const { return expr1_; }
    Node *GetExpr2() const { return expr2_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    SeqKind sk_;
    Node *expr1_;
    Node *expr2_;
};

class NApp : public Node {
public:
    NApp(Node *fun, Node *arg)
        : fun_(fun), arg_(arg) { }
    ~NApp() override {
        delete fun_;
        delete arg_;
    }

    Node *GetFun() const { return fun_; }
    Node *GetArg() const { return arg_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *fun_;
    Node *arg_;
};

class NUnaryApp : public Node {
public:
    NUnaryApp(std::string opsym, Node *expr)
        : opsym_(opsym), expr_(expr) { }
    ~NUnaryApp() override {
        delete expr_;
    }

    std::string GetOpSym() const { return opsym_; }
    Node *GetExpr() const { return expr_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string opsym_;
    Node *expr_;
};

class NBinaryApp : public Node {
public:
    NBinaryApp(std::string opsym, Node *left, Node *right)
        : opsym_(opsym), left_(left), right_(right) { }
    ~NBinaryApp() override {
        delete left_;
        delete right_;
    }

    std::string GetOpSym() const { return opsym_; }
    Node *GetLeft() const { return left_; }
    Node *GetRight() const { return right_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string opsym_;
    Node *left_;
    Node *right_;
};

class NCond : public Node {
public:
    NCond(Node *cond, Node *conseq, Node *alter)
        : cond_(cond), conseq_(conseq), alter_(alter) { }
    ~NCond() override {
        delete cond_;
        delete conseq_;
        delete alter_;
    }

    Node *GetCond() const { return cond_; }
    Node *GetConseq() const { return conseq_; }
    Node *GetAlter() const { return alter_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *cond_;
    Node *conseq_;
    Node *alter_;
};

class NHandler : public Node {
public:
    NHandler(Node *opcs)
        : opcs_(opcs) { }
    ~NHandler() override {
        delete opcs_;
    }

    Node *GetOpCases() const { return opcs_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *opcs_;
};

class NOpCase : public Node {
public:
    NOpCase(std::string var, Node *body)
        : opname_(""), var_(var), body_(body) { }
    NOpCase(std::string opname, std::string var, std::string cont, Node *body)
        : opname_(opname), var_(var), cont_(cont), body_(body) { }
    ~NOpCase() {
        delete body_;
    }

    bool IsReturnCase() const { return opname_.empty(); }
    std::string GetOpName() const { return opname_; }
    std::string GetVar() const { return var_; }
    Node *GetBody() const { return body_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    std::string opname_;
    std::string var_;
    std::string cont_;
    Node *body_;
};

class NWithHandle : public Node {
public:
    NWithHandle(Node *handler, Node *cexpr)
        : handler_(handler), cexpr_(cexpr) { }
    ~NWithHandle() {
        delete handler_;
        delete cexpr_;
    }

    Node *GetHandler() const { return handler_; }
    Node *GetCexpr() const { return cexpr_; }

    DUMP_DECL override;
    ACCEPT_DECL override;

private:
    Node *handler_;
    Node *cexpr_;
};

