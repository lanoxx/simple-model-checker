#include <kripke/kripke.h>

#include <glib.h>
#include <locale.h>

typedef struct {
  long placeholder;
} KripkeFixture;

static void
kripke_fixture_set_up (KripkeFixture *fixture,
                       gconstpointer user_data);

static void
kripke_fixture_tear_down (KripkeFixture *fixture,
                          gconstpointer user_data);

static void
test_load_kripke_from_string (KripkeFixture *fixture,
                              gconstpointer user_data);

int
main (int argc, char *argv[])
{
  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  g_test_add ("/kripke/parse_kripke", KripkeFixture, "",
              kripke_fixture_set_up, kripke_fixture_tear_down,
              test_load_kripke_from_string);

  return g_test_run ();
}

static void
kripke_fixture_set_up (KripkeFixture *fixture,
                       gconstpointer user_data)
{ }

static void
kripke_fixture_tear_down (KripkeFixture *fixture,
                          gconstpointer user_data)
{ }

static void
test_load_kripke_from_string (KripkeFixture *fixture,
                              gconstpointer user_data)
{

  char *kripke_json = "{\n"
                      "  \"states\": [\n"
                      "    { \"name\": \"s0\", \"labels\": [ \"a\", \"b\" ] },\n"
                      "    { \"name\": \"s1\", \"labels\": [ \"a\" ] },\n"
                      "    { \"name\": \"s2\", \"labels\": [ \"b\" ] },\n"
                      "    { \"name\": \"s3\", \"labels\": [ \"a\", \"b\" ] }\n"
                      "  ],\n"
                      "  \"initialStates\": [ \"s0\" ],\n"
                      "  \"relations\": [ [ \"s0\", \"s1\" ], [ \"s1\", \"s2\" ], [ \"s1\", \"s0\" ], [ \"s2\", \"s3\" ], [ \"s2\", \"s0\" ], [ \"s3\", \"s3\" ] ]\n"
                      "}";

  GError *error = NULL;

  KripkeStructure *kripke =
      kripke_structure_new_from_string (kripke_json, &error);

  g_assert_true (g_list_length (kripke->states) == 4);

  State *state0 = kripke->states->data;
  State *state1 = kripke->states->next->data;
  State *state2 = kripke->states->next->next->data;
  State *state3 = kripke->states->next->next->next->data;

  g_assert_cmpstr (state0->name, ==, "s0");
  g_assert_cmpstr (state1->name, ==, "s1");
  g_assert_cmpstr (state2->name, ==, "s2");
  g_assert_cmpstr (state3->name, ==, "s3");

  g_assert_true (g_list_length (kripke->labels) == 6);

  Label *label0 = kripke->labels->data;
  Label *label1 = kripke->labels->next->data;
  Label *label2 = kripke->labels->next->next->data;
  Label *label3 = kripke->labels->next->next->next->data;
  Label *label4 = kripke->labels->next->next->next->next->data;
  Label *label5 = kripke->labels->next->next->next->next->next->data;

  g_assert_true (label0->state == state0);
  g_assert_cmpstr (label0->atom, ==, "a");

  g_assert_true (label1->state == state0);
  g_assert_cmpstr (label1->atom, ==, "b");

  g_assert_true (label2->state == state1);
  g_assert_cmpstr (label2->atom, ==, "a");

  g_assert_true (label3->state == state2);
  g_assert_cmpstr (label3->atom, ==, "b");

  g_assert_true (label4->state == state3);
  g_assert_cmpstr (label4->atom, ==, "a");

  g_assert_true (label5->state == state3);
  g_assert_cmpstr (label5->atom, ==, "b");

  g_assert_true (g_list_length (kripke->relations) == 6);

  g_assert_true (kripke != NULL);
}
