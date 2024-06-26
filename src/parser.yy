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
    SEMICOLON ";"
    LESS    "<"
    GREAT   ">"
    EQUAL   "="
    TRUE    "true"
    FALSE   "false"
    LET     "let"
    IN      "in"
    END     "end"
    FUN     "fun"
    RIGHTARROW "->"
;
%token <std::string> IDENT "ident"
%token <int> NUMBER "number"
%nterm <std::shared_ptr<Node>> expr prog

%printer { yyo << $$; } <*>;

%%
%start top;

top: prog { drv.SetResult(std::make_shared<Top>($1)); } ;

prog:
    %empty { $$ = std::make_shared<Empty>(); }
  | prog expr ";"";" { $$ = std::make_shared<Prog>($1, $2); } ;

%left "=";
%left "<" ">";
%left "+" "-";
%left "*" "/";

expr:
    "true" { $$ = std::make_shared<NBool>(true); }
  | "false" { $$ = std::make_shared<NBool>(false); }
  | "number" { $$ = std::make_shared<NInt>($1); }
  | "ident" { $$ = std::make_shared<Ident>($1); }
  | expr "+" expr { $$ = std::make_shared<Add>($1, $3); }
  | expr "-" expr { $$ = std::make_shared<Sub>($1, $3); }
  | expr "*" expr { $$ = std::make_shared<Mul>($1, $3); }
  | expr "/" expr { $$ = std::make_shared<Div>($1, $3); }
  | expr "<" expr { $$ = std::make_shared<Less>($1, $3); }
  | expr ">" expr { $$ = std::make_shared<Great>($1, $3); }
  | expr "=" expr { $$ = std::make_shared<Equal>($1, $3); }
  | "let" "ident" "=" expr "in" expr "end"
    { $$ = std::make_shared<Let1>($2, $4, $6); }
  | "fun" "ident" "->" expr { $$ = std::make_shared<NFun>($2, $4); }
  | "(" expr expr ")" { $$ = std::make_shared<App>($2, $3); }
  | "(" expr ")" { $$ = $2; }
;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

