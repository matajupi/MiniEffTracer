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
    FUN     "fun"
    RIGHTARROW "->"
    SEMISEMI ";;"
    IF      "if"
    THEN    "then"
    ELSE    "else"
    REC     "rec"
    COMMA   ","
;
%token <std::string> IDENT "ident"
%token <int> NUMBER "number"
%nterm <std::shared_ptr<Node>> prog topexpr arithexpr appexpr btexpr letexpr def

%printer { yyo << $$; } <*>;

%%
%start top;

top: prog { drv.SetResult(std::make_shared<Top>($1)); } ;

prog:
    %empty { $$ = std::make_shared<Empty>(); }
  | prog def ";;" { $$ = std::make_shared<Prog>($1, $2); }
;

def:
    topexpr
  | "let" "ident" "=" topexpr { $$ = std::make_shared<LetDef>($2, $4); }
  | "let" "rec" "ident" "=" topexpr { $$ = std::make_shared<LetRecDef>($3, $5); }
  | "let" "ident" "ident" "=" topexpr { $$ = std::make_shared<LetDef>($2, std::make_shared<NFun>($3, $5)); }
  | "let" "rec" "ident" "ident" "=" topexpr { $$ = std::make_shared<LetRecDef>($3, std::make_shared<NFun>($4, $6)); }
;

%right ";";

topexpr:
    letexpr
  | topexpr ";" topexpr { $$ = std::make_shared<Seq>($1, $3); }
;

letexpr:
    arithexpr
  | "fun" "ident" "->" topexpr { $$ = std::make_shared<NFun>($2, $4); }
  | "let" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<Let>($2, $4, $6); }
  | "let" "rec" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<LetRec>($3, $5, $7); }
  | "let" "ident" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<Let>($2, std::make_shared<NFun>($3, $5), $7); }
  | "let" "rec" "ident" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<LetRec>($3, std::make_shared<NFun>($4, $6), $8); }
  | "if" topexpr "then" topexpr "else" letexpr
    { $$ = std::make_shared<If>($2, $4, $6); }
;

%left "=";
%left "<" ">";
%left "+" "-";
%left "*" "/";

arithexpr:
    appexpr
  | arithexpr "=" arithexpr { $$ = std::make_shared<Equal>($1, $3); }
  | arithexpr "<" arithexpr { $$ = std::make_shared<Less>($1, $3); }
  | arithexpr ">" arithexpr { $$ = std::make_shared<Great>($1, $3); }
  | arithexpr "+" arithexpr { $$ = std::make_shared<Add>($1, $3); }
  | arithexpr "-" arithexpr { $$ = std::make_shared<Sub>($1, $3); }
  | arithexpr "*" arithexpr { $$ = std::make_shared<Mul>($1, $3); }
  | arithexpr "/" arithexpr { $$ = std::make_shared<Div>($1, $3); }
;

appexpr:
    btexpr
  | appexpr btexpr { $$ = std::make_shared<App>($1, $2); }
;

btexpr:
    "true" { $$ = std::make_shared<NBool>(true); }
  | "false" { $$ = std::make_shared<NBool>(false); }
  | "number" { $$ = std::make_shared<NInt>($1); }
  | "ident" { $$ = std::make_shared<Ident>($1); }
  | "(" ")" { $$ = std::make_shared<NUnit>(); }
  | "(" topexpr "," topexpr ")" { $$ = std::make_shared<NProduct>($2, $4); }
  | "(" topexpr ")" { $$ = $2; }
;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

