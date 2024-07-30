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
    HANDLER "handler"
    WITH    "with"
    HANDLE  "handle"
    VBAR    "|"
;
%token <std::string> IDENT "ident"
%token <int> NUMBER "number"
%nterm <std::shared_ptr<Node>> topexpr arithexpr appexpr btexpr letexpr def
%nterm <std::shared_ptr<NOpC>> opc
%nterm <std::shared_ptr<NTop::Prog>> prog
%nterm <std::shared_ptr<NHandler::NOpCList>> opcs

// %printer { yyo << $$; } <*>;

%%
%start top;

top: prog { drv.SetResult(std::make_shared<NTop>($1)); } ;

prog:
    %empty { $$ = std::make_shared<NTop::Prog>(); }
  | prog def ";;" { $1->push_back($2); }
;

def:
    topexpr
  | "let" "ident" "=" topexpr { $$ = std::make_shared<NLetDef>($2, $4); }
  | "let" "rec" "ident" "=" topexpr { $$ = std::make_shared<NLetRecDef>($3, $5); }
  | "let" "ident" "ident" "=" topexpr { $$ = std::make_shared<NLetDef>($2, std::make_shared<NFun>($3, $5)); }
  | "let" "rec" "ident" "ident" "=" topexpr { $$ = std::make_shared<NLetRecDef>($3, std::make_shared<NFun>($4, $6)); }
;

%right ";";

topexpr:
    letexpr
  | topexpr ";" topexpr { $$ = std::make_shared<NSeq>($1, $3); }
;

letexpr:
    arithexpr
  | "fun" "ident" "->" topexpr { $$ = std::make_shared<NFun>($2, $4); }
  | "let" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<NLet>($2, $4, $6); }
  | "let" "rec" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<NLetRec>($3, $5, $7); }
  | "let" "ident" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<NLet>($2, std::make_shared<NFun>($3, $5), $7); }
  | "let" "rec" "ident" "ident" "=" topexpr "in" topexpr
    { $$ = std::make_shared<NLetRec>($3, std::make_shared<NFun>($4, $6), $8); }
  | "if" topexpr "then" topexpr "else" letexpr
    { $$ = std::make_shared<NIf>($2, $4, $6); }
  | "handler" opcs
    { $$ = std::make_shared<NHandler>($2); }
  | "with" topexpr "handle" topexpr
    { $$ = std::make_shared<NWith>($2, $4); }
;

%right "|";

opcs:
    %empty
    { $$ = std::make_shared<NHandler::NOpCList>(); }
  | opcs "|" opc
    { $1->push_back($3); }
;

opc:
    "ident" "->" topexpr
    { $$ = std::make_shared<NOpC>($1, $3); }
  | "ident" "ident" "ident" "->" topexpr
    { $$ = std::make_shared<NOpC>($1, $2, $3, $5); }
;

%left "=";
%left "<" ">";
%left "+" "-";
%left "*" "/";

arithexpr:
    appexpr
  | arithexpr "=" arithexpr { $$ = std::make_shared<NEqual>($1, $3); }
  | arithexpr "<" arithexpr { $$ = std::make_shared<NLess>($1, $3); }
  | arithexpr ">" arithexpr { $$ = std::make_shared<NGreat>($1, $3); }
  | arithexpr "+" arithexpr { $$ = std::make_shared<NAdd>($1, $3); }
  | arithexpr "-" arithexpr { $$ = std::make_shared<NSub>($1, $3); }
  | arithexpr "*" arithexpr { $$ = std::make_shared<NMul>($1, $3); }
  | arithexpr "/" arithexpr { $$ = std::make_shared<NDiv>($1, $3); }
;

appexpr:
    btexpr
  | appexpr btexpr { $$ = std::make_shared<NApp>($1, $2); }
;

btexpr:
    "true" { $$ = std::make_shared<NBool>(true); }
  | "false" { $$ = std::make_shared<NBool>(false); }
  | "number" { $$ = std::make_shared<NInt>($1); }
  | "ident" { $$ = std::make_shared<NIdent>($1); }
  | "(" ")" { $$ = std::make_shared<NUnit>(); }
  | "(" topexpr "," topexpr ")" { $$ = std::make_shared<NProduct>($2, $4); }
  | "(" topexpr ")" { $$ = $2; }
;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

