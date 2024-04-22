#include "errs.H"

#include <iostream>

static std::string error_message[] = {
    "",
    "Incorrect input",
    "Incorrect args",
    "Incorrect file",
    "Imposible calculations",
    "Lack of functionality",
    "Can't build polinom"
};

void print_errs(errs_t err) {
    std::cout << error_message[err] << std::endl;
}