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

class Expr : public Node { };

class Number : public Expr, public std::enable_shared_from_this<Number> {
public:
    Number(int value) : value_(value) { }
    void Dump(std::ostream &os) const override;
    void Accept(Visitor &visitor) override;

    int GetValue() const { return value_; }

private:
    int value_;
};

class Binary : public Expr {
public:
    Binary(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
        : left_(left), right_(right) { }
    void Dump(std::ostream &os) const override;

    std::shared_ptr<Expr> GetLeft() const { return left_; }
    std::shared_ptr<Expr> GetRight() const { return right_; }

    virtual std::string GetOpSym() const = 0;

protected:
    std::shared_ptr<Expr> left_;
    std::shared_ptr<Expr> right_;
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

