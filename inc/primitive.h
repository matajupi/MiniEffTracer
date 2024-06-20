#pragma once

class Primitive {
};

class Integer : public Primitive {
public:
    Integer(int value) : value_(value) { }
    int GetValue() const { return value_; }

private:
    int value_;
};

