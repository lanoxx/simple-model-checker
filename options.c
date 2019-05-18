#include "options.h"

#include <glib.h>
#include <stdio.h>

#include <config.h>

static void print_help ();

Options * options_parse_args (int argc, char *argv[])
{
  char* input_filename = NULL;
  char *formula = NULL;

  Options *options = g_malloc (sizeof (Options));

  argv++;

  while (*argv)
  {
    char *option = strdup (*argv);
    char *option_name = strtok(option, "=");
    char *option_value = strtok (NULL, "=");

    if (g_strcmp0 (option_name, "--kripke-file") == 0)
    {
      input_filename = strdup (option_value);
    }

    if (g_strcmp0 (option_name, "--formula") == 0)
    {
      formula = strdup (option_value);
    }

    if (g_strcmp0 (option_name, "--help") == 0)
    {
      print_help ();
      exit (EXIT_SUCCESS);
    }

    free (option);

    argv++;
  }

  options->input_filename = input_filename;
  options->formula = formula;

  return options;
}

void options_free (Options *options)
{
  if (!options)
    return;

  if (options->formula)
  {
    free (options->formula);
  }

  if (options->input_filename)
  {
    free (options->input_filename);
  }

  g_free (options);
}

static void print_help ()
{
  printf ("Simple Model Checker\n");
  printf ("\n");
  printf ("usage: %s [--input-file=FILENAME] [--formula=FORMULA]\n", BINARY_NAME);
  printf ("\n");
}
