#include "errs.h"

#include <iostream>

static std::string error_message[] = {
    "",
    "Incoorect input",
    "Incoorect args",
    "Incorrect file",
    "Imposible calculations",
    "Lack of functionality",
    "Can't build polinom"
};

void print_errs(errs_t err) {
    std::cout << error_message[err] << std::endl;
}