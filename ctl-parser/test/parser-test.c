#include <ctl-parser/parser.h>

#include <glib.h>
#include <locale.h>

typedef struct {
  Formula *obj;
} CtlParserFixture;

static void
my_object_fixture_set_up (CtlParserFixture *fixture,
                          gconstpointer user_data)
{ }

static void
my_object_fixture_tear_down (CtlParserFixture *fixture,
                             gconstpointer user_data)
{ }

static void
test_parse_always_future_a (CtlParserFixture *fixture,
                            gconstpointer user_data)
{
  Formula * formula = parse_ctl_formula ("AF(a)", FALSE);

  g_assert_true (formula->type == CTL_TYPE_NEGATION);

  g_assert_true (g_list_length (formula->subformulas) == 1);

  Formula *subformula_eg = formula->subformulas->data;

  g_assert_true (subformula_eg->type == CTL_TYPE_EXISTS_GLOBALLY);

  g_assert_true (g_list_length (subformula_eg->subformulas) == 1);

  Formula *subformula_not = subformula_eg->subformulas->data;

  g_assert_true (subformula_not->type == CTL_TYPE_NEGATION);

  g_assert_true (g_list_length (subformula_not->subformulas) == 1);

  Formula *subformula_atom = subformula_not->subformulas->data;

  g_assert_cmpstr (subformula_atom->name, ==, "a");
}

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);
  g_test_bug_base ("http://bugzilla.gnome.org/show_bug.cgi?id=");

  g_test_add ("/ctl-parser/parse_AF(a)", CtlParserFixture, "",
              my_object_fixture_set_up, test_parse_always_future_a,
              my_object_fixture_tear_down);

  return g_test_run ();
}
