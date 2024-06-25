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

