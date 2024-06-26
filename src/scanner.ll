%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include "driver.h"
#include "parser.hh"
%}

%option noyywrap
%option nounput noinput
%option batch
%option debug

%{
yy::parser::symbol_type make_NUMBER(
    const std::string &s, const yy::parser::location_type &loc);
%}

id      [a-zA-Z_][a-zA-Z_0-9]*
int     [0-9]+
blank   [ \t\r]

%{
#define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
yy::location &loc = drv.GetLocation();
loc.step();
%}

{blank}+    loc.step();
\n+         { loc.lines(yyleng); loc.step(); }

"-"         return yy::parser::make_MINUS(loc);
"+"         return yy::parser::make_PLUS(loc);
"*"         return yy::parser::make_ASTER(loc);
"/"         return yy::parser::make_SLASH(loc);
"("         return yy::parser::make_LPAREN(loc);
")"         return yy::parser::make_RPAREN(loc);
";"         return yy::parser::make_SEMICOLON(loc);
"<"         return yy::parser::make_LESS(loc);
">"         return yy::parser::make_GREAT(loc);
"="         return yy::parser::make_EQUAL(loc);
"true"      return yy::parser::make_TRUE(loc);
"false"     return yy::parser::make_FALSE(loc);
"let"       return yy::parser::make_LET(loc);
"in"        return yy::parser::make_IN(loc);
"end"       return yy::parser::make_END(loc);
"fun"       return yy::parser::make_FUN(loc);
"->"        return yy::parser::make_RIGHTARROW(loc);

{int}       return make_NUMBER(yytext, loc);
{id}        return yy::parser::make_IDENT(yytext, loc);
.           {
                throw yy::parser::syntax_error(
                    loc, "invalid character: " + std::string(yytext));
            }
<<EOF>>     return yy::parser::make_YYEOF(loc);

%%

yy::parser::symbol_type make_NUMBER(
    const std::string &s, const yy::parser::location_type &loc) {
    errno = 0;
    long n = strtol(s.c_str(), NULL, 10);
    if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
        throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
    }
    return yy::parser::make_NUMBER(static_cast<int>(n), loc);
}

void Driver::ScanBegin() {
    yy_flex_debug = trace_scanning_;
    if (file_.empty() || file_ == "-") {
        yyin = stdin;
    }
    else if (!(yyin = fopen(file_.c_str(), "r"))) {
        std::cerr << "cannot open " << file_ << ": " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Driver::ScanEnd() {
    fclose(yyin);
}

