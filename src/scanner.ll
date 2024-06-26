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
#define MKSYM(name) if (!comment) return yy::parser::make_ ## name(loc)

yy::location &loc = drv.GetLocation();
loc.step();
bool comment = false;
%}

{blank}+    loc.step();
\n+         { loc.lines(yyleng); loc.step(); }

"(*"        { comment = true; }
"*)"        { comment = false; }

"true"      { MKSYM(TRUE); }
"false"     { MKSYM(FALSE); }
"let"       { MKSYM(LET); }
"in"        { MKSYM(IN); }
"fun"       { MKSYM(FUN); }
"if"        { MKSYM(IF); }
"then"      { MKSYM(THEN); }
"else"      { MKSYM(ELSE); }
"rec"       { MKSYM(REC); }
"->"        { MKSYM(RIGHTARROW); }
"-"         { MKSYM(MINUS); }
"+"         { MKSYM(PLUS); }
"*"         { MKSYM(ASTER); }
"/"         { MKSYM(SLASH); }
"("         { MKSYM(LPAREN); }
")"         { MKSYM(RPAREN); }
";"         { MKSYM(SEMICOLON); }
"<"         { MKSYM(LESS); }
">"         { MKSYM(GREAT); }
"="         { MKSYM(EQUAL); }

{int}       { if (!comment) return make_NUMBER(yytext, loc); }
{id}        { if (!comment) return yy::parser::make_IDENT(yytext, loc); }
.           {
                if (!comment) throw yy::parser::syntax_error(
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

