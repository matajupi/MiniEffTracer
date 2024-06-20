#include <iostream>
#include <cstdlib>
#include "driver.h"
#include "tracer.h"


int main(int argc, char **argv) {
    Driver drv;
    Tracer trc(std::cout);
    if (argc >= 2 && !drv.Parse(argv[1])) {
        drv.GetResult()->Dump(std::cout);
        drv.GetResult()->Accept(trc);
    }
    return EXIT_SUCCESS;
}
