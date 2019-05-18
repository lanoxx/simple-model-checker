#ifndef PARSER_H
#define PARSER_H

#include "formula.h"

Formula *
parse_ctl_formula (const char *input, int debug);

#endif /* PARSER_H */
