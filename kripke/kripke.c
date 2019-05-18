#include <kripke/kripke.h>

#include <stdio.h>

#include <json-glib/json-glib.h>

static GList *
get_labels_for_state (State      *state,
                      JsonObject *state_object);

static gboolean
add_states_and_labels_from_json_node (KripkeStructure *kripke,
                                      JsonArray       *states_array);

static gboolean add_relations_from_json_node (KripkeStructure *kripke,
                                              JsonArray       *relations_array);

#define KRIPKE_LOAD_ERROR_QUARK kripke_load_error_quark ()

GQuark kripke_load_error_quark ()
{
  return g_quark_from_static_string ("kripke-load-error_quark");
}

static State *
find_state_by_name (KripkeStructure *kripke, const char *name);

KripkeStructure * kripke_structure_new (GList * states,
                                        State *initial_state,
                                        GList *relations,
                                        GList *labels)
{
  KripkeStructure *kripke = malloc (sizeof (KripkeStructure));

  kripke->states = states;
  kripke->relations = relations;
  kripke->initialState = initial_state;
  kripke->labels = labels;

  return kripke;
}

KripkeStructure * kripke_structure_new_from_file (const char *filename,
                                                  GError **error)
{
  if (filename == NULL || strlen (filename) == 0)
  {
    *error = g_error_new (KRIPKE_LOAD_ERROR_QUARK, 3,
                          "Input file name for kripke structure must not be empty or NULL.\n");

    return NULL;
  }

  GError *load_error = NULL;

  GFile *input_file = g_file_new_for_path (filename);

  if (! g_file_query_exists (input_file, NULL))
  {
    *error = g_error_new (KRIPKE_LOAD_ERROR_QUARK, 1,
                          "File does not exist: %s.\n", filename);

    return NULL;
  }

  char *content = NULL;

  gboolean loaded = g_file_load_contents (input_file, NULL, &content, NULL, NULL,
                                          &load_error);

  g_clear_object (&input_file);

  if (!loaded)
  {
    *error = g_error_new (KRIPKE_LOAD_ERROR_QUARK, 2,
                          "Unable to load file contents: %s.\n", filename);

    return NULL;
  }

  KripkeStructure *kripke = kripke_structure_new_from_string (content, error);

  g_free (content);

  return kripke;
}

/**
 * Parses a KripkeStructure from a JSON representation
 *
 * Example input:
 *
 *  {
 *     "states": [
 *       { "name": "s0", "labels": [ "a", "b" ] },
 *       { "name": "s1", "labels": [ "a" ] },
 *       { "name": "s2", "labels": [ "b" ] },
 *       { "name": "s3", "labels": [ "a", "b" ] }
 *     ],
 *     "initialStates": [ "s0" ],
 *     "relations": [ [ "s0", "s1" ], [ "s1", "s2" ], [ "s1", "s0" ], [ "s2", "s3" ], [ "s2", "s0" ], [ "s3", "s3" ] ]
 *   }
 *
 * @param input A JSON encoded Kripke structure.
 * @return
 */
KripkeStructure * kripke_structure_new_from_string (const char  *input,
                                                    GError     **error)
{
  g_return_val_if_fail (error && *error == NULL, NULL);

  JsonNode *node =  json_from_string (input, error);

  if (!JSON_NODE_HOLDS_OBJECT (node))
  {
    printf ("expected an object in the root node\n");
    goto error;
  }

  JsonObject *root = json_node_get_object (node);

  if (!json_object_has_member (root, "states"))
  {
    printf ("expected member named 'states'\n");
    goto error;
  }

  JsonArray *states_array = json_object_get_array_member (root, "states");

  KripkeStructure *kripke = g_malloc0 (sizeof (KripkeStructure));

  gboolean success = add_states_and_labels_from_json_node (kripke, states_array);

  if (!success)
  {
    goto error;
  }

  if (!json_object_has_member (root, "relations"))
  {
    printf ("expected member named 'relations'\n");
    goto error;
  }

  JsonArray * relations_array = json_object_get_array_member (root, "relations");

  success = add_relations_from_json_node (kripke, relations_array);

  if (!success)
  {
    goto error;
  }

  json_node_free (node);

  return kripke;

  error:
  g_free (kripke);
  return NULL;
}

/**
 * Prints each state in the given list to stdout.
 *
 * @param states A pointer to a GList<State> of State structures.
 */
void kripke_print_states (GList *states)
{
  GList *state_item = states;

  while (state_item) {
    State *result_state = state_item->data;

    printf ("state: %s\n", result_state->name);

    state_item = g_list_next (state_item);
  }
}

void kripke_print_initial_state (State *initialState)
{
  const char *name = initialState && initialState->name ? initialState->name : "none";

  printf ("initial state: %s\n", name);
}

void kripke_print_labels (GList *labels)
{
  GList *label_item = labels;

  while (label_item)
  {
    Label *label = label_item->data;

    printf ("state: %s, label: %s\n", label->state->name, label->atom);

    label_item = g_list_next (label_item);
  }
}

void kripke_print_relations (GList *relations)
{
  GList *relation_item = relations;

  while (relation_item)
  {
    Relation *relation = relation_item->data;

    printf ("relation: %s -> %s\n", relation->state1->name, relation->state2->name);

    relation_item = g_list_next (relation_item);
  }
}

/**
 * Extracts a list of states from a Json Node
 * @param node A JsonArray pointer that contains the following structure:
 *
 *  "states": [ { "name": "s0", "labels": [ "a, b" ] },
 *              { "name": "s1" "labels": [ "a" ] },
 *              { "name": "s2" "labels": [ "b" ] },
 *              { "name": "s3", "labels": [ "a", "b" ] }
 *            ]
 * @return
 */
static gboolean
add_states_and_labels_from_json_node (KripkeStructure *kripke,
                                      JsonArray       *states_array)
{
  GList *state_node_items = json_array_get_elements (states_array);
  GList *state_node_item = state_node_items;
  GList *result_states = NULL;
  GList *result_labels = NULL;

  State *state = NULL;

  while (state_node_item)
  {
    state = g_malloc0 (sizeof (State));

    JsonNode *state_node = state_node_item->data;


    if (!JSON_NODE_HOLDS_OBJECT (state_node))
    {
      printf ("state_note_item does not hold object\n");
      goto error;
    }

    // get state_object from the node
    JsonObject * state_object = json_node_get_object (state_node);

    if (!json_object_has_member (state_object, "name"))
    {
      printf ("expected a 'name' member in state object\n");
      goto error;
    }

    // get the name of the state
    const gchar * name = json_object_get_string_member (state_object, "name");

    state->name = strdup (name);

    result_states = g_list_append (result_states, state);

    if (!json_object_has_member (state_object, "labels"))
    {
      printf ("expected a 'labels' member in state object\n");
      goto error;
    }

    GList *labels = get_labels_for_state (state, state_object);

    result_labels = g_list_concat (result_labels, labels);

    state_node_item = state_node_item->next;
  }

  kripke->states = result_states;
  kripke->labels = result_labels;

  g_list_free (state_node_items);

  return TRUE;

  error:
  kripke_state_free (state);
  kripke_states_free (result_states);
  kripke_labels_free (result_labels);

  return FALSE;
}

static GList *
get_labels_for_state (State *state, JsonObject *state_object)
{
  // get the labels of the state
  JsonArray * labels_array = json_object_get_array_member (state_object, "labels");

  GList *label_items = json_array_get_elements (labels_array);
  GList *label_item = label_items;
  GList *result_labels = NULL;

  Label *label = NULL;

  while (label_item)
  {
    JsonNode *label_node = label_item->data;

    label = g_malloc0 (sizeof(Label));

    const char *atom = json_node_get_string (label_node);

    if (atom == NULL)
    {
      printf ("expected atom name in labels array\n");
      goto error;
    }

    label->atom = strdup (atom);
    label->state = state;

    result_labels = g_list_append (result_labels, label);

    label_item = label_item->next;
  }

  g_list_free (label_items);

  return result_labels;

  error:
  kripke_label_free (label);
  kripke_labels_free (result_labels);
  return NULL;
}

/**
 *
 * @param kripke
 * @param relations_array
 *
 * [ [ "s0", "s1" ], [ "s1", "s2" ], [ "s1", "s0" ], [ "s2", "s3" ], [ "s2", "s0" ], [ "s3", "s3" ] ]
 *
 * @return
 */
static gboolean add_relations_from_json_node (KripkeStructure *kripke,
                                              JsonArray       *relations_array)
{
  g_return_val_if_fail (kripke != NULL, FALSE);
  g_return_val_if_fail (relations_array != NULL, FALSE);

  GList *relation_array_items = json_array_get_elements (relations_array);
  GList *relation_array_item = relation_array_items;
  GList *result_relations = NULL;

  Relation *relation = NULL;

  while (relation_array_item)
  {
    relation = g_malloc (sizeof (Relation));

    JsonNode *relation_node = relation_array_item->data;

    if (!JSON_NODE_HOLDS_ARRAY (relation_node))
    {
      printf ("expected arrays with relation names nested in the 'relations' array.\n");
      goto error;
    }

    JsonArray *relation_array = json_node_get_array (relation_node);

    if (json_array_get_length (relation_array) != 2)
    {
      printf ("each relation array must contain exactly two state names\n");
      goto error;
    }

    const char * state1_name = json_array_get_string_element (relation_array, 0);
    const char * state2_name = json_array_get_string_element (relation_array, 1);

    if (state1_name == NULL || state2_name == NULL)
    {
      goto error;
    }

    State *state1 = find_state_by_name (kripke, state1_name);
    State *state2 = find_state_by_name (kripke, state2_name);

    relation->state1 = state1;
    relation->state2 = state2;

    result_relations = g_list_append (result_relations, relation);

    relation_array_item = relation_array_item->next;
  }

  kripke->relations = result_relations;

  g_list_free (relation_array_items);

  return TRUE;

  error:
  kripke_relation_free (relation);
  return FALSE;
}

static State *
find_state_by_name (KripkeStructure *kripke, const char *name)
{
  GList *current_state_item = kripke->states;

  while (current_state_item)
  {
    State *state = current_state_item->data;

    if (g_strcmp0 (name, state->name) == 0)
    {
      return state;
    }

    current_state_item = current_state_item->next;
  }

  return NULL;
}

void kripke_print (KripkeStructure *kripke)
{
  kripke_print_states (kripke->states);
  kripke_print_initial_state (kripke->initialState);
  kripke_print_labels (kripke->labels);
  kripke_print_relations (kripke->relations);
}

void kripke_free (KripkeStructure *kripke)
{
  kripke_labels_free (kripke->labels);
  kripke_states_free (kripke->states);
  kripke_relations_free (kripke->relations);

  g_free (kripke);
}

void kripke_state_free  (State *state)
{
  g_free (state->name);

  g_free (state);
}

void kripke_label_free  (Label *label)
{
  g_free (label->atom);

  label->atom = NULL;
  label->state = NULL;

  g_free (label);
}

void kripke_relation_free (Relation *relation)
{
  g_free (relation);
}

void kripke_states_free (GList *list)
{
  g_list_free_full (list, (GDestroyNotify) kripke_state_free);
}

void kripke_labels_free (GList *list)
{
  g_list_free_full (list, (GDestroyNotify) kripke_label_free);
}

void kripke_relations_free (GList *list)
{
  g_list_free_full (list, (GDestroyNotify) kripke_relation_free);
}
