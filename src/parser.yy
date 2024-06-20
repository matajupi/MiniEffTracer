%skeleton "lalr1.cc"
%require "3.4"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <string>
#include <memory>
#include "nodes.h"

class Driver;
}

%param { Driver &drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
#include "driver.h"
}

%define api.token.prefix {TOK_}
%token
    MINUS   "-"
    PLUS    "+"
    ASTER   "*"
    SLASH   "/"
    LPAREN  "("
    RPAREN  ")"
;
%token <std::string> IDENT "ident"
%token <int> NUMBER "number"
%nterm <std::shared_ptr<Expr>> expr

%printer { yyo << $$; } <*>;

%%

%start top;
top: expr { drv.SetResult(std::make_shared<Top>($1)); } ;

%left "+" "-";
%left "*" "/";

expr:
   "number"         { $$ = std::make_shared<Number>($1); }
 | expr "+" expr    { $$ = std::make_shared<Add>($1, $3); }
 | expr "-" expr    { $$ = std::make_shared<Sub>($1, $3); }
 | expr "*" expr    { $$ = std::make_shared<Mul>($1, $3); }
 | expr "/" expr    { $$ = std::make_shared<Div>($1, $3); }
 | "(" expr ")"     { $$ = $2; }
 ;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

