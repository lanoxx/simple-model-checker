#ifndef MODEL_CHECKER_H
#define MODEL_CHECKER_H

#include <glib.h>
#include <ctl-parser/formula.h>
#include <kripke/kripke.h>

GList *
getStatesBooleanLiteral (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithAtomicProposition (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithNegation (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithConjunction (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsNext (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsUntil (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsGlobally (KripkeStructure *kripke, Formula *formula);

/**
 * Applies the model checking algorithm to kripkeStructure using the given
 * CTL formula. The formula must be represented as an abstract syntax tree.
 *
 * @param kripkeStructure
 * @param formula A pointer to a parsed Formula. Use the ctl-parser library
 *        to create the formula.
 * @return A list of states (GList<State> *). Must be freed when no longer needed.
 */
GList *model_check (KripkeStructure *kripkeStructure,
                    Formula *formula);

/**
 * Applies the model checking algorithm to kripkeStructure using the given
 * CTL formula. The formula will be parsed by the ctl-parser library.
 *
 * @param kripkeStructure
 * @param formula A string pointing to a valid CTL* formula.
 * @return A list of states (GList<State> *). Must be freed when no longer needed.
 */
GList *model_check_from_string (KripkeStructure *kripkeStructure,
                                const char *formula);

#endif /* MODEL_CHECKER_H */
