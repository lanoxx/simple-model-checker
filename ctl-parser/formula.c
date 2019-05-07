#include "formula.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

Formula *
formula_new ()
{
  Formula *formula = malloc(sizeof(Formula));
  formula->type = CTL_TYPE_NONE;
  formula->subformulas = NULL;
  formula->name = NULL;
  formula->value = 0;

  return formula;
}

Formula *
formula_new_composite (enum FormulaType type, char* name, ...)
{
  va_list args;
  va_start(args, name);

  Formula *formula = formula_new();
  formula->type = type;
  formula->name = strdup(name);

  Formula *subformula;
  GList *subformulas = NULL;

  while ((subformula = va_arg (args, Formula*)) != NULL) {
    subformulas = g_list_append (subformulas, subformula);
  }

  formula->subformulas = subformulas;

  va_end (args);

  return formula;
}

Formula *
formula_new_value (enum FormulaType type, long value)
{
  Formula *formula = formula_new();
  formula->type = type;
  formula->value = value;

  return formula;
}

Formula *
formula_new_name (enum FormulaType type, char* name)
{
  Formula *formula = formula_new();
  formula->type = type;
  formula->name = name;

  return formula;
}

void formula_free (Formula *formula) {

  if (!formula) {
    return;
  }

  GList *current =formula->subformulas;

  while (current) {

    Formula *current_formula = current->data;

    formula_free (current_formula);

    current = g_list_next (current);
  }

  g_list_free (formula->subformulas);
  formula->subformulas = NULL;

  if (formula->name) {
    free (formula->name);
  }

  formula->name = NULL;

  free (formula);
}

void formula_print_structure (Formula *formula)
{
  if (!formula) return;

  Formula *current = formula;
  switch (current->type)
  {
    case CTL_TYPE_BOOLEAN_LITERAL:
      printf ("Result: %s\n", current->value ? "true" : "false");
      break;
    case CTL_TYPE_ATOMIC_PROPOSITION:
      printf("Result: %s\n", current->name);
      break;
    default:
      printf("Result: %s\n", current->name);
      break;
  }
  formula_print_structure (current->subformulas->data);
  formula_print_structure (current->subformulas->next->data);
}
