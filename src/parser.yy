%skeleton "lalr1.cc"
%require "3.4"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <string>
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
    LESSEQ  "<="
    GREAT   ">"
    GREATEQ ">="
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
    AMPERAMPER "&&"
    VBARVBAR "||"

    HANDLER "handler"
    WITH    "with"
    HANDLE  "handle"
    VBAR    "|"
;
%token <std::string> IDENT "ident"
%token <int> NUMBER "number"
%nterm <Node *> dec expr topexpr opcs binapp unapp app con

// %printer { yyo << $$; } <*>;

%%
%start prog;

prog: dec {
        drv.SetResult(new NTop($1));
    }
;

%right ";;";

dec:
      expr
    | "let" "ident" "=" expr
        { $$ = new NLet($2, $4); }
    | "let" "ident" "ident" "=" expr
        { $$ = new NLet($2, new NFun($3, $5)); }
    | "let" "rec" "ident" "=" expr
        { $$ = new NLetRec($3, $5); }
    | "let" "rec" "ident" "ident" "=" expr
        { $$ = new NLetRec($3, new NFun($4, $6)); }
    | %empty
        { $$ = new NUnit(); }
    | dec ";;" dec
        { $$ = new NSeq(SeqKind::Dec, $1, $3); }
;

%right ";";

expr:
      topexpr
    | expr ";" expr
        { $$ = new NSeq(SeqKind::Expr, $1, $3); }
;

topexpr:
      binapp
    | "let" "ident" "=" expr "in" expr
        { $$ = new NLet($2, $4, $6); }
    | "let" "ident" "ident" "=" expr "in" expr
        { $$ = new NLet($2, new NFun($3, $5), $7); }
    | "let" "rec" "ident" "=" expr "in" expr
        { $$ = new NLetRec($3, $5, $7); }
    | "let" "rec" "ident" "ident" "=" expr "in" expr
        { $$ = new NLetRec($3, new NFun($4, $6), $8); }
    | "if" expr "then" expr "else" expr
        { $$ = new NCond($2, $4, $6); }
    | "with" expr "handle" expr
        { $$ = new NWithHandle($2, $4); }
    | "fun" "ident" "->" expr
        { $$ = new NFun($2, $4); }
    | "handler" opcs
        { $$ = new NHandler($2); }
;

%right "|";

opcs:
      "ident" "->" expr
        { $$ = new NOpCase($1, $3); }
    | "ident" "ident" "ident" "->" expr
        { $$ = new NOpCase($1, $2, $3, $5); }
    | opcs "|" opcs
        { $$ = new NSeq(SeqKind::OpCase, $1, $3); }
;

%left "&&" "||";
%left "=";
%left "<" ">" "<=" ">=";
%left "+" "-";
%left "*" "/";

binapp:
      unapp
    | topexpr "&&" topexpr
        { $$ = new NBinaryApp("&&", $1, $3); }
    | topexpr "||" topexpr
        { $$ = new NBinaryApp("||", $1, $3); }
    | topexpr "=" topexpr
        { $$ = new NBinaryApp("=", $1, $3); }
    | topexpr "<" topexpr
        { $$ = new NBinaryApp("<", $1, $3); }
    | topexpr ">" topexpr
        { $$ = new NBinaryApp(">", $1, $3); }
    | topexpr "<=" topexpr
        { $$ = new NBinaryApp("<=", $1, $3); }
    | topexpr ">=" topexpr
        { $$ = new NBinaryApp(">=", $1, $3); }
    | topexpr "+" topexpr
        { $$ = new NBinaryApp("+", $1, $3); }
    | topexpr "-" topexpr
        { $$ = new NBinaryApp("-", $1, $3); }
    | topexpr "*" topexpr
        { $$ = new NBinaryApp("*", $1, $3); }
    | topexpr "/" topexpr
        { $$ = new NBinaryApp("/", $1, $3); }
;

unapp:
      app
    | "+" topexpr
        { $$ = new NUnaryApp("+", $2); }
    | "-" topexpr
        { $$ = new NUnaryApp("-", $2); }
;

app:
      con
    | app con
        { $$ = new NApp($1, $2); }
;

con:
      "true"    { $$ = new NBool(true); }
    | "false"   { $$ = new NBool(false); }
    | "number"  { $$ = new NInt($1); }
    | "ident"   { $$ = new NIdent($1); }
    | "(" ")"   { $$ = new NUnit(); }
    | "(" expr ")"
        { $$ = $2; }
    | "(" expr "," expr ")"
        { $$ = new NPair($2, $4); }
;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

