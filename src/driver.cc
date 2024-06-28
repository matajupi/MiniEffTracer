#include "driver.h"

int Driver::Parse(const std::string &f) {
    file_ = f;
    location_.initialize(&file_);
    ScanBegin();
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing_);
    int res = parse();
    ScanEnd();
    return res;
}

