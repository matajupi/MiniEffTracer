#pragma once

#include <string>
#include <ostream>
#include <memory>

class Visitor;

class Node {
public:
    virtual void Dump(std::ostream &os) const = 0;
    virtual void Accept(Visitor &visitor) = 0;
};

class Top : public Node {
public:
    Top(std::shared_ptr<Node> content) : content_(content) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetContent() const { return content_; }

private:
    std::shared_ptr<Node> content_;
};

class Prog : public Node {
public:
    Prog(std::shared_ptr<Node> prev, std::shared_ptr<Node> expr)
        : prev_(prev), expr_(expr) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetPrev() const { return prev_; }
    std::shared_ptr<Node> GetExpr() const { return expr_; }

private:
    std::shared_ptr<Node> prev_;
    std::shared_ptr<Node> expr_;
};

class Empty : public Node {
public:
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;
};

class NInt : public Node {
public:
    NInt(int value) : value_(value) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    int GetValue() const { return value_; }

private:
    int value_;
};

class NBool : public Node {
public:
    NBool(bool value) : value_(value) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    bool GetValue() const { return value_; }

private:
    bool value_;
};

class NFun : public Node {
public:
    NFun(std::string var, std::shared_ptr<Node> body)
        : var_(var), body_(body) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBody() const { return body_; }

private:
    std::string var_;
    std::shared_ptr<Node> body_;
};

class NUnit : public Node {
public:
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;
};

class Ident : public Node {
public:
    Ident(std::string str) : str_(str) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetStr() const { return str_; }

private:
    std::string str_;
};

class Let : public Node {
public:
    Let(std::string var, std::shared_ptr<Node> bexpr,
        std::shared_ptr<Node> cexpr)
        : var_(var), bexpr_(bexpr), cexpr_(cexpr) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBexpr() const { return bexpr_; }
    std::shared_ptr<Node> GetCexpr() const { return cexpr_; }

private:
    std::string var_;
    std::shared_ptr<Node> bexpr_;
    std::shared_ptr<Node> cexpr_;
};

class LetRec : public Node {
public:
    LetRec(std::string var, std::shared_ptr<Node> bexpr,
        std::shared_ptr<Node> cexpr)
        : var_(var), bexpr_(bexpr), cexpr_(cexpr) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBexpr() const { return bexpr_; }
    std::shared_ptr<Node> GetCexpr() const { return cexpr_; }

private:
    std::string var_;
    std::shared_ptr<Node> bexpr_;
    std::shared_ptr<Node> cexpr_;
};

class Seq : public Node {
public:
    Seq(std::shared_ptr<Node> expr1, std::shared_ptr<Node> expr2)
        : expr1_(expr1), expr2_(expr2) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetExpr1() const { return expr1_; }
    std::shared_ptr<Node> GetExpr2() const { return expr2_; }

private:
    std::shared_ptr<Node> expr1_;
    std::shared_ptr<Node> expr2_;
};

class App : public Node {
public:
    App(std::shared_ptr<Node> fun, std::shared_ptr<Node> arg)
        : fun_(fun), arg_(arg) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetFun() const { return fun_; }
    std::shared_ptr<Node> GetArg() const { return arg_; }

private:
    std::shared_ptr<Node> fun_;
    std::shared_ptr<Node> arg_;
};

class If : public Node {
public:
    If(std::shared_ptr<Node> cond, std::shared_ptr<Node> if_clause,
        std::shared_ptr<Node> else_clause)
        : cond_(cond), if_clause_(if_clause), else_clause_(else_clause) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetCond() const { return cond_; }
    std::shared_ptr<Node> GetIfClause() const { return if_clause_; }
    std::shared_ptr<Node> GetElseClause() const { return else_clause_; }

private:
    std::shared_ptr<Node> cond_;
    std::shared_ptr<Node> if_clause_;
    std::shared_ptr<Node> else_clause_;
};

class Binary : public Node {
public:
    Binary(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : left_(left), right_(right) { }
    void Dump(std::ostream &os) const override;

    std::shared_ptr<Node> GetLeft() const { return left_; }
    std::shared_ptr<Node> GetRight() const { return right_; }

    virtual std::string GetOpSym() const = 0;

protected:
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;
};

class Add : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "+"; }
};

class Sub : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "-"; }
};

class Mul : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "*"; }
};

class Div : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "/"; }
};

class Less : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "<"; }
};

class Great : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return ">"; }
};

class Equal : public Binary {
public:
    using Binary::Binary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "="; }
};

