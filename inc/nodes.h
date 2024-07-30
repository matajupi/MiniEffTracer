#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <memory>

class Visitor;

class Node {
public:
    virtual void Dump(std::ostream &os) const = 0;
    virtual void Accept(Visitor &visitor) = 0;
};

class NTop : public Node {
public:
    using Prog = std::vector<std::shared_ptr<Node>>;

    NTop(std::shared_ptr<Prog> prog) : prog_(prog) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Prog> GetProg() const { return prog_; }

private:
    std::shared_ptr<Prog> prog_;
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

class NProduct : public Node {
public:
    NProduct(std::shared_ptr<Node> expr1, std::shared_ptr<Node> expr2)
        : expr1_(expr1), expr2_(expr2) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetExpr1() const { return expr1_; }
    std::shared_ptr<Node> GetExpr2() const { return expr2_; }

private:
    std::shared_ptr<Node> expr1_;
    std::shared_ptr<Node> expr2_;
};

class NIdent : public Node {
public:
    NIdent(std::string str) : str_(str) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetStr() const { return str_; }

private:
    std::string str_;
};

class NLet : public Node {
public:
    NLet(std::string var, std::shared_ptr<Node> bexpr,
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

class NLetRec : public Node {
public:
    NLetRec(std::string var, std::shared_ptr<Node> bexpr,
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

class NLetDef : public Node {
public:
    NLetDef(std::string var, std::shared_ptr<Node> bexpr)
        : var_(var), bexpr_(bexpr) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBexpr() const { return bexpr_; }

private:
    std::string var_;
    std::shared_ptr<Node> bexpr_;
};

class NLetRecDef : public Node {
public:
    NLetRecDef(std::string var, std::shared_ptr<Node> bexpr)
        : var_(var), bexpr_(bexpr) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetVar() const { return var_; }
    std::shared_ptr<Node> GetBexpr() const { return bexpr_; }

private:
    std::string var_;
    std::shared_ptr<Node> bexpr_;
};

class NSeq : public Node {
public:
    NSeq(std::shared_ptr<Node> expr1, std::shared_ptr<Node> expr2)
        : expr1_(expr1), expr2_(expr2) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetExpr1() const { return expr1_; }
    std::shared_ptr<Node> GetExpr2() const { return expr2_; }

private:
    std::shared_ptr<Node> expr1_;
    std::shared_ptr<Node> expr2_;
};

class NApp : public Node {
public:
    NApp(std::shared_ptr<Node> fun, std::shared_ptr<Node> arg)
        : fun_(fun), arg_(arg) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetFun() const { return fun_; }
    std::shared_ptr<Node> GetArg() const { return arg_; }

private:
    std::shared_ptr<Node> fun_;
    std::shared_ptr<Node> arg_;
};

class NIf : public Node {
public:
    NIf(std::shared_ptr<Node> cond, std::shared_ptr<Node> if_clause,
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

class NOpC : public Node {
public:
    NOpC(std::string var, std::shared_ptr<Node> body)
        : NOpC("", var, "", body) { }
    NOpC(std::string eff, std::string var,
        std::string cont, std::shared_ptr<Node> body)
        : eff_(eff), var_(var), cont_(cont), body_(body) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::string GetEff() const { return eff_; }
    std::string GetVar() const { return var_; }
    std::string GetCont() const { return cont_; }
    std::shared_ptr<Node> GetBody() const { return body_; }

    bool IsReturn() const { return eff_ == ""; }

private:
    std::string eff_;
    std::string var_;
    std::string cont_;
    std::shared_ptr<Node> body_;
};

class NHandler : public Node {
public:
    using NOpCList = std::vector<std::shared_ptr<NOpC>>;

    NHandler(std::shared_ptr<NOpCList> opcs);
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<NOpC> GetRetC() const { return retc_; }
    std::shared_ptr<NOpCList> GetEffCs() const { return effcs_; }

private:
    std::shared_ptr<NOpC> retc_;
    std::shared_ptr<NOpCList> effcs_;
};

class NWith : public Node {
public:
    NWith(std::shared_ptr<Node> handler, std::shared_ptr<Node> body)
        : handler_(handler), body_(body) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    std::shared_ptr<Node> GetHandler() const { return handler_; }
    std::shared_ptr<Node> GetBody() const { return body_; }

private:
    std::shared_ptr<Node> handler_;
    std::shared_ptr<Node> body_;
};

class NBinary : public Node {
public:
    NBinary(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : left_(left), right_(right) { }
    void Dump(std::ostream &os) const override;

    std::shared_ptr<Node> GetLeft() const { return left_; }
    std::shared_ptr<Node> GetRight() const { return right_; }

    virtual std::string GetOpSym() const = 0;

protected:
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;
};

class NAdd : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "+"; }
};

class NSub : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "-"; }
};

class NMul : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "*"; }
};

class NDiv : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "/"; }
};

class NLess : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "<"; }
};

class NGreat : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return ">"; }
};

class NEqual : public NBinary {
public:
    using NBinary::NBinary;
    void Accept(Visitor &visitor) override;
    std::string GetOpSym() const override { return "="; }
};

