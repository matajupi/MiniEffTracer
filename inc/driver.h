#pragma once

#include <string>
#include <map>
#include <memory>
#include "parser.hh"
#include "nodes.h"

#define YY_DECL yy::parser::symbol_type yylex(Driver &drv)

YY_DECL;

class Driver {
public:
    Driver(bool trace_parsing = false, bool trace_scanning = false);

    int Parse(const std::string &f);
    void ScanBegin();
    void ScanEnd();

    yy::location &GetLocation() { return location_; }
    std::shared_ptr<Node> GetResult() { return result_; }
    void SetResult(std::shared_ptr<Node> result) { result_ = result; }

private:
    std::string file_;
    yy::location location_;

    bool trace_parsing_;
    bool trace_scanning_;

    std::shared_ptr<Node> result_;
};

