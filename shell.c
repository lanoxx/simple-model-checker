#include "shell.h"

#include <glib.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <kripke/kripke.h>
#include <model-checking/model-checker.h>

typedef struct shell_context
{
  KripkeStructure *kripke;
} ShellContext;

typedef enum shell_command_type
{
  COMMAND_TYPE_NONE,
  COMMAND_TYPE_CHECK,
  COMMAND_TYPE_EXIT,
  COMMAND_TYPE_LOAD
} ShellCommandType;

static gboolean evaluate_line (ShellContext *context,
                               char *line,
                               char **result);

static ShellCommandType get_command_type (const char* command);

static gboolean handle_command (ShellContext *context,
                            ShellCommandType command_type,
                            const char *argument,
                            char ** result);

static void load_kripke_structure (ShellContext *context,
                                   const char *filename,
                                   char **result);

static void check_formula (ShellContext *context,
                           const char *specification,
                           char **result);

static void free_context (ShellContext *context);

void shell_start ()
{
  gboolean running = TRUE;

  ShellContext *context = g_malloc0 (sizeof (ShellContext));

  while (running)
  {
    // read
    char *line = readline (">");
    char *result = NULL;

    running = evaluate_line (context, line, &result);

    free (line);

    if (!result)
    {
      continue;
    }

    printf ("%s\n", result);

    free (result);
  }

  free_context (context);
}

static gboolean evaluate_line (ShellContext *context,
                               char *line,
                               char **result)
{
  // parse line
  char *command = strtok (line, " ");
  char *argument = strtok (NULL, "");

  // identify command
  ShellCommandType command_type = get_command_type (command);

  // handle command (update context)
  return handle_command (context, command_type, argument, result);
}

static ShellCommandType get_command_type (const char* command)
{
  if (g_strcmp0 ("load", command) == 0)
  {
    return COMMAND_TYPE_LOAD;
  }

  if (g_strcmp0 ("check", command) == 0)
  {
    return COMMAND_TYPE_CHECK;
  }

  if (g_strcmp0 ("exit", command) == 0)
  {
    return COMMAND_TYPE_EXIT;
  }

  return COMMAND_TYPE_NONE;
}

static gboolean handle_command (ShellContext *context,
                                ShellCommandType command_type,
                                const char *argument,
                                char ** result)
{
  switch (command_type)
  {
    case COMMAND_TYPE_LOAD:

      load_kripke_structure (context, argument, result);

      break;

    case COMMAND_TYPE_CHECK:

      check_formula (context, argument, result);
      break;

    case COMMAND_TYPE_EXIT:
      return FALSE;

    default:
      *result = strdup ("unknown command\n");
      break;
  }

  return TRUE;
}

static void load_kripke_structure (ShellContext *context,
                                   const char *filename,
                                   char **result)
{

  int state_count = 0;
  int relation_count = 0;
  int label_count = 0;

  GError *error = NULL;

  KripkeStructure *kriple = kripke_structure_new_from_file (filename, &error);

  context->kripke = kriple;

  state_count = g_list_length (kriple->states);
  relation_count = g_list_length (kriple->relations);
  label_count = g_list_length (kriple->labels);

  *result = g_strdup_printf ("Kripke structure loaded: "
                             "%d states, %d relations, %d labels\n",
                             state_count, relation_count, label_count);
}

static void check_formula (ShellContext *context,
                           const char *specification,
                           char **result)
{
  KripkeStructure *kripke = context->kripke;

  printf ("spec: %s\n", specification);

  GList *states = model_check_from_string (kripke, specification);

  kripke_print_states (states);

  g_list_free (states);
}

static void free_context (ShellContext *context)
{
  if (!context)
  {
    return;
  }

  g_free (context->kripke);

  g_free (context);
}
