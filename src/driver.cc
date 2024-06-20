#include "driver.h"

Driver::Driver(bool trace_parsing, bool trace_scanning)
    : trace_parsing_(trace_parsing), trace_scanning_(trace_scanning) {
}

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

