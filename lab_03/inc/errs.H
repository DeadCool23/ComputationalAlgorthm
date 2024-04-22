#ifndef __ERRS_H__
#define __ERRS_H__

typedef enum {
    OK,
    ERR_IO,
    ERR_ARGS,
    ERR_FILE,
    ERR_CALC,
    ERR_FUNC,
    ERR_POLINOM
} errs_t;

void print_errs(errs_t err);

#endif // __ERRS_H__