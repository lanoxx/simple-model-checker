#ifndef FORMULA_H
#define FORMULA_H

#include <glib.h>

enum FormulaType
{
  CTL_TYPE_NONE,                /* Use for uninitialized variable for type enum FormulaType. */
  CTL_TYPE_BOOLEAN_LITERAL,
  CTL_TYPE_ATOMIC_PROPOSITION,
  CTL_TYPE_NEGATION,
  CTL_TYPE_CONJUNCTION,
  CTL_TYPE_EXIST_NEXT,
  CTL_TYPE_EXIST_UNTIL,
  CTL_TYPE_EXISTS_GLOBALLY,
  CTL_TYPE_LAST
};

typedef struct formula {
  GList *subformulas;
  enum FormulaType type;
  char *name;
  long value;
} Formula;

void      formula_print_structure (Formula *formula);

Formula * formula_new ();

Formula * formula_new_composite (enum FormulaType type, char* name, ...);

Formula * formula_new_value (enum FormulaType type, long value);

Formula * formula_new_name (enum FormulaType type, char* name);

void      formula_free (Formula *formula);

#endif /* FORMULA_H */
