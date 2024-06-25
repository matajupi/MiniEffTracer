#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <iostream>
#include <map>

class Prim {
public:
    virtual std::shared_ptr<Prim> Add(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Sub(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Mul(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Div(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Less(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Great(std::shared_ptr<Prim> other) = 0;
    virtual std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) = 0;

    virtual void Dump(std::ostream &os) = 0;

    template<typename T>
    static std::shared_ptr<T> Cast(std::shared_ptr<Prim> value);
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

    std::shared_ptr<Prim> Add(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Sub(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Mul(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Div(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Less(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Great(std::shared_ptr<Prim> other) override;
    std::shared_ptr<Prim> Equal(std::shared_ptr<Prim> other) override;

    void Dump(std::ostream &os) override;

private:
    bool value_;
    static std::map<bool, std::shared_ptr<PBool>> storage_;
};

